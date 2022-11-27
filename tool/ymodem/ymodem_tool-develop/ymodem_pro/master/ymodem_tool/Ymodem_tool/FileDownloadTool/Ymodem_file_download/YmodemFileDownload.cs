using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using CCWin; //UI

using System.IO; //文件处理
using System.Threading; //创建多线程使用
using System.IO.Ports; //查看电脑端口

using System.Text.RegularExpressions;

namespace Ymodem_tool
{
    public partial class YmodemFileDownload : Skin_Mac
    {
        #region 窗体初始化
        public YmodemFileDownload()
        {
            InitializeComponent();

            TransmitModeComboBoxInit(customCombox_TransmitMode);
            BaudComboBoxInit(customCombox_Baud);

            ScanAvailableSerialPort();
        }

        //窗体加载
        private void YmodemFileDownload_Load(object sender, EventArgs e)
        {
            this.MaximizeBox = false; //禁用最大化按钮
            this.Width = 385;
            this.Height = 535;
        }

        //传输模式下拉框的选项初始化
        public void TransmitModeComboBoxInit(ComboBox comboBox)
        {
            //设置下拉选项样式，只能从下列选择，不能自已输入
            comboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            //添加元素
            comboBox.Items.Add("Ymodem");
            comboBox.Items.Add("Ymodem-1K");
            //选择默认选择的元素
            comboBox.SelectedIndex = 0;
        }

        //波特率下拉框的选项初始化
        public void BaudComboBoxInit(ComboBox comboBox)
        {
            //设置下拉选项样式，只能从下列选择，不能自已输入
            comboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            //添加元素
            comboBox.Items.Add("9600");
            comboBox.Items.Add("19200");
            comboBox.Items.Add("38400");
            comboBox.Items.Add("115200");
            comboBox.Items.Add("230400");
            comboBox.Items.Add("460800");
            comboBox.Items.Add("921600");
            //选择默认选择的元素
            comboBox.SelectedIndex = 0;
        }
        #endregion

        #region 升级文件数据提取
        private string _updatePath = ""; //升级文件路径
        private bool _loadUpdateFileStatus = false; //加载升级文件状态 (true,加载成功 / false,未加载)

        private void skinButton_OpenUpdateFile_Click(object sender, EventArgs e)
        {
            //创建打开文件对话框对象
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "|*.bin"; //只查看bin格式文件
            if (ofd.ShowDialog() == DialogResult.OK)//判断是选中文件
            {
                FileInfo f = new FileInfo(ofd.FileName); //创建FileInfo对象
                ClearUpdateBinFileDisplay();
                AddUpdateBinFileDisplay("文件名： " + ofd.FileName + "\r\n"); //文件名(即包含绝对路径的文件名)
                AddUpdateBinFileDisplay("文件长度： " + f.Length.ToString() + " 字节\r\n"); //文件字节大小
                AddUpdateBinFileDisplay("文件创建日期： " + f.CreationTime.ToString() + "\r\n"); //文件创建日期
                AddUpdateBinFileDisplay("文件最后修改时间： " + f.LastWriteTime.ToString()); //文件最后次修改日期
                _loadUpdateFileStatus = true;

                _updatePath = ofd.FileName;
            }
            else
            {
                AddUpdateBinFileDisplay("添加BIN文件出错");
                _loadUpdateFileStatus = false;
            }
        }
        #endregion

        #region 调试辅助信息显示
        //文件信息显示窗口清空显示
        private void ClearUpdateBinFileDisplay()
        {
            this.Invoke(new Action(() =>
            {
                richTextBox_UpdateBinFileData.Text = "";
            }));
        }

        //文件信息显示窗口添加内容显示
        private void AddUpdateBinFileDisplay(string info)
        {
            this.Invoke(new Action(() =>
            {
                richTextBox_UpdateBinFileData.AppendText(info);
            }));
        }

        private void ProcessInfoClear()
        {
            this.Invoke(new Action(() =>
            {
                richTextBox_UoloadResultDisplay.Clear();
            }));
        }

        private void ProcessInfoAdd(string text)
        {
            this.Invoke(new Action(() =>
            {
                richTextBox_UoloadResultDisplay.Text += text + "\r\n";
            }));
        }
        #endregion

        #region 串口处理
        //扫描可用串口
        private void button_UartPortScan_Click(object sender, EventArgs e)
        {
            ScanAvailableSerialPort();
        }

        /// <summary>
        /// 获取当前所有串口信息
        /// </summary>
        private void ScanAvailableSerialPort()
        {
            SerialTransmission Serial = new SerialTransmission();
            string currentPort = customCombox_FileDownload_Port.Text; //保存当前选择的串口号
            Serial.GetSerialPort(customCombox_FileDownload_Port);
            customCombox_FileDownload_Port.Text = currentPort;
        }

        //将串口号全名提取处COM口号
        private string GetUartComNumber(string UartPortWholeName)
        {
            try
            {
                string[] sArray = UartPortWholeName.Split(new string[] { "(", ")" }, StringSplitOptions.RemoveEmptyEntries);
                foreach (string e in sArray)
                {
                    if (e.Contains("COM")) return e;
                }
            }
            catch { }

            return "";
        }

        //获取串口号名 (如COM1)
        private string GetSerialPortName(string portText)
        {
            Regex reg = new Regex(@"COM\d+");
            Match matchs = reg.Match(portText);
            if (matchs != null && matchs.Groups[0].ToString() != string.Empty)
            {
                Debugging.output("port_name = " + matchs.Groups[0].ToString());
                return matchs.Groups[0].ToString();
            }
            else
            {
                Debugging.output("无法识别的串口。");
            }

            return "";
        }
        #endregion

        #region 下载线程处理
        private Thread _fileDownloadThread;
        private SerialPort serialPort = new SerialPort();
        private Ymodem ymodem = new Ymodem();
        private YModemType _yModemType = YModemType.YModem;

        //传输模式选择
        private void customCombox_TransmitMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = customCombox_TransmitMode.SelectedIndex;

            if (index != -1) //非空值
            {
                if (index == 0) //Ymodem
                {
                    _yModemType = YModemType.YModem;
                }
                else if (index == 1) //Ymodem-1K
                {
                    _yModemType = YModemType.YModem_1K;
                }
            }
        }

        //停止文件下载
        private void FileDownloadStop()
        {
            try
            {
                serialPort.Close();
            }
            catch
            {
            }
        }

        //文件下载处理
        private bool FileDownloadProc()
        {
            int baud = 0;
            string portText = "";

            //清空信息
            ProcessInfoClear();
            this.Invoke(new Action(() =>
            {
                this.progressBar_Upload.Value = 0;
                this.label_UploadProgress.Text = "0 %";
            }));

            //串口占用检测
            try
            {
                this.Invoke(new Action(() =>
                {
                    baud = Convert.ToInt32(customCombox_Baud.SelectedItem.ToString());
                    portText = customCombox_FileDownload_Port.Text;
                }));
                serialPort.PortName = GetSerialPortName(portText);
                serialPort.BaudRate = baud;
                serialPort.Open();

                Debugging.output("uart name=" + serialPort.PortName);
            }
            catch
            {
                ProcessInfoAdd("打开设备的串口失败！");
                this.Invoke(new Action(() =>
                {
                    this.skinButton_FileDownload_StartDownload.Text = "开始下载";
                    this.skinButton_FileDownload_StartDownload.BackgroundImage = ymodem_tool.Properties.Resources.Light_blue_button_64px;
                }));
                return false;
            }
            serialPort.Close();

            //开始下载
            ProcessInfoAdd("开始下载...");
            ymodem = new Ymodem();
            ymodem.path = _updatePath;
            ymodem.portName = GetSerialPortName(portText);
            ymodem.baudRate = baud;
            ymodem.NowDownloadProgressEvent += new EventHandler(NowDownloadProgressEvent);
            ymodem.DownloadResultEvent += new EventHandler(DownloadFinishEvent);
            ymodem.YmodemUploadFile(_yModemType, 0);

            return true;
        }

        //下载线程
        private void FileDownloadThreadProc()
        {
            //中途停止下载
            if (_fileDownloadThread != null && _fileDownloadThread.IsAlive)
            {
                ymodem.YmodemUploadFileStop();
                return;
            }

            //开始下载
            this.skinButton_FileDownload_StartDownload.Text = "取消下载";
            this.skinButton_FileDownload_StartDownload.BackgroundImage = ymodem_tool.Properties.Resources.Red_button_64px;
            _fileDownloadThread = new Thread(() =>
            {
                FileDownloadProc();
            });
            _fileDownloadThread.IsBackground = true; //设置为后台进程,关闭当前窗体或线程运行结束就会结束该线程
            _fileDownloadThread.Start();
        }

        //开始下载升级
        private void skinButton_FileDownload_StartDownload_Click(object sender, EventArgs e)
        {
            //确认升级文件加载合法性
            if (_loadUpdateFileStatus == false)
            {
                MessageBox.Show("请先加载升级文件！");
                return;
            }

            //下载流程
            FileDownloadThreadProc();
        }
        #endregion

        #region 下载进度委托及事件响应
        private void NowDownloadProgressEvent(object sender, EventArgs e)
        {
            int value = Convert.ToInt32(sender);
            this.Invoke(new Action(() =>
            {
                this.progressBar_Upload.Value = value;
                this.label_UploadProgress.Text = value.ToString() + " %";
            }));
        }
        #endregion

        #region 下载完成委托及事件响应
        private delegate void DownloadFinish(bool finish);

        private void DownloadFinishEvent(object sender, EventArgs e)
        {
            bool finish = (Boolean)sender;
            DownloadFinish status = new DownloadFinish(UploadFileResult);
            this.Invoke(status, finish);
        }

        private void UploadFileResult(bool result)
        {
            if (result == true)
            {
                ProcessInfoAdd("下载成功！");
            }
            else
            {
                ProcessInfoAdd("下载失败！");
            }
            this.skinButton_FileDownload_StartDownload.Text = "开始下载";
            this.skinButton_FileDownload_StartDownload.BackgroundImage = ymodem_tool.Properties.Resources.Light_blue_button_64px;
        }

        #endregion

    }
}
