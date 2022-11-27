using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Windows.Forms;
using System.IO.Ports; //查看电脑端口
using System.Management; //调用系统API用到(同时在工程里添加该引用)

namespace Ymodem_tool
{
    /// <summary>
    /// 串口通讯类
    /// </summary>
    public class SerialTransmission
    {
        /// <summary>
        /// 枚举win32 api
        /// </summary>
        public enum HardwareEnum
        {
            // 硬件
            Win32_Processor, // CPU 处理器
            Win32_PhysicalMemory, // 物理内存条
            Win32_Keyboard, // 键盘
            Win32_PointingDevice, // 点输入设备，包括鼠标。
            Win32_FloppyDrive, // 软盘驱动器
            Win32_DiskDrive, // 硬盘驱动器
            Win32_CDROMDrive, // 光盘驱动器
            Win32_BaseBoard, // 主板
            Win32_BIOS, // BIOS 芯片
            Win32_ParallelPort, // 并口
            Win32_SerialPort, // 串口
            Win32_SerialPortConfiguration, // 串口配置
            Win32_SoundDevice, // 多媒体设置，一般指声卡。
            Win32_SystemSlot, // 主板插槽 (ISA & PCI & AGP)
            Win32_USBController, // USB 控制器
            Win32_NetworkAdapter, // 网络适配器
            Win32_NetworkAdapterConfiguration, // 网络适配器设置
            Win32_Printer, // 打印机
            Win32_PrinterConfiguration, // 打印机设置
            Win32_PrintJob, // 打印机任务
            Win32_TCPIPPrinterPort, // 打印机端口
            Win32_POTSModem, // MODEM
            Win32_POTSModemToSerialPort, // MODEM 端口
            Win32_DesktopMonitor, // 显示器
            Win32_DisplayConfiguration, // 显卡
            Win32_DisplayControllerConfiguration, // 显卡设置
            Win32_VideoController, // 显卡细节。
            Win32_VideoSettings, // 显卡支持的显示模式。

            // 操作系统
            Win32_TimeZone, // 时区
            Win32_SystemDriver, // 驱动程序
            Win32_DiskPartition, // 磁盘分区
            Win32_LogicalDisk, // 逻辑磁盘
            Win32_LogicalDiskToPartition, // 逻辑磁盘所在分区及始末位置。
            Win32_LogicalMemoryConfiguration, // 逻辑内存配置
            Win32_PageFile, // 系统页文件信息
            Win32_PageFileSetting, // 页文件设置
            Win32_BootConfiguration, // 系统启动配置
            Win32_ComputerSystem, // 计算机信息简要
            Win32_OperatingSystem, // 操作系统信息
            Win32_StartupCommand, // 系统自动启动程序
            Win32_Service, // 系统安装的服务
            Win32_Group, // 系统管理组
            Win32_GroupUser, // 系统组帐号
            Win32_UserAccount, // 用户帐号
            Win32_Process, // 系统进程
            Win32_Thread, // 系统线程
            Win32_Share, // 共享
            Win32_NetworkClient, // 已安装的网络客户端
            Win32_NetworkProtocol, // 已安装的网络协议
            Win32_PnPEntity,//all device
        }

        /// <summary>
        /// WMI取硬件信息
        /// </summary>
        /// <param name="hardType"></param>
        /// <param name="propKey"></param>
        /// <returns></returns>
        public string[] MulGetHardwareInfo(HardwareEnum hardType, string propKey)
        {

            List<string> strs = new List<string>();
            try
            {
                using (ManagementObjectSearcher searcher = new ManagementObjectSearcher("select * from " + hardType))
                {
                    var hardInfos = searcher.Get();
                    foreach (var hardInfo in hardInfos)
                    {
                        if (hardInfo.Properties[propKey].Value != null) //需要先判断当前值是否有效
                        {
                            if (hardInfo.Properties[propKey].Value.ToString().Contains("COM"))
                            {
                                strs.Add(hardInfo.Properties[propKey].Value.ToString());
                            }
                        }
                    }
                    searcher.Dispose();
                }
                return strs.ToArray();
            }
            catch
            {
                return null;
            }
            finally
            { strs = null; }
        }

        /// <summary>
        /// 获取所有串口的详细信息
        /// </summary>
        /// <returns></returns>
        public string[] GetAllSerialPortInfo()
        { 
            return MulGetHardwareInfo(HardwareEnum.Win32_PnPEntity, "Name"); //通过WMI获取COM端口
        }

        /// <summary>
        /// 获取所有串口的端口名字
        /// </summary>
        /// <returns></returns>
        public string[] GetAllSerialPortName()
        {
            return SerialPort.GetPortNames();
        }

        /// <summary>
        /// 设置串口波特率的下拉框控件
        /// </summary>
        /// <param name="Buad_ComboBox"></param>
        public void SetSerialBaud(ComboBox Buad_ComboBox)
        {
            //设置下拉选项样式，只能从下列选择，不能自已输入
            Buad_ComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            //添加元素
            Buad_ComboBox.Items.Add("1200");
            Buad_ComboBox.Items.Add("2400");
            Buad_ComboBox.Items.Add("4800");
            Buad_ComboBox.Items.Add("9600");
            Buad_ComboBox.Items.Add("19200");
            Buad_ComboBox.Items.Add("57600");
            Buad_ComboBox.Items.Add("115200");
            Buad_ComboBox.Items.Add("460800");
            Buad_ComboBox.Items.Add("921600");
            //选择默认选择的元素
            Buad_ComboBox.SelectedIndex = 3; //默认选择9600
        }

        /// <summary>
        /// 设置串口波特率的下拉框控件(简化版)
        /// </summary>
        /// <param name="Buad_ComboBox"></param>
        public void SetSerialBaud_Simple(ComboBox Buad_ComboBox)
        {
            //设置下拉选项样式，只能从下列选择，不能自已输入
            Buad_ComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            //添加元素
            Buad_ComboBox.Items.Add("9600");
            Buad_ComboBox.Items.Add("19200");
            Buad_ComboBox.Items.Add("115200");
            Buad_ComboBox.Items.Add("256000");
            Buad_ComboBox.Items.Add("921600");
            //选择默认选择的元素
            Buad_ComboBox.SelectedIndex = 2; //默认选择9600
        }

        private string GetComputerSystemVersionInfo()
        {
            var name = (from x in new ManagementObjectSearcher("SELECT Caption FROM Win32_OperatingSystem").Get().Cast<ManagementObject>()
                        select x.GetPropertyValue("Caption")).FirstOrDefault();
            return name != null ? name.ToString() : "Unknown";
        }

        /// <summary>
        /// 设置串口端口信息的下拉框控件 
        /// </summary>
        /// <param name="Port_ComboBox"></param>
        public void GetSerialPort(ComboBox Port_ComboBox)
        {
            string[] port_info = new string[50]; //最多支持50个串口
            string[] temp_port_info = new string[50];
            int i = 0; //记录索引

            //设置下拉选项样式，只能从下列选择，不能自已输入
            Port_ComboBox.DropDownStyle = ComboBoxStyle.DropDownList;

            temp_port_info = port_info; //先缓存一下未获取新串口列表前的串口信息

            //获取使用的电脑的WINDOWS版本信息，对于WIN7获取所有完整信息(经过测试)，WIN10不支持该操作，故显示简要信息
            string SystemVersion = GetComputerSystemVersionInfo();
            port_info = GetAllSerialPortInfo(); //获取当前所有串口的完整信息
            if (port_info == null) //某些WIN7版本会获取失败，那么就获取简要信息
            {
                port_info = GetAllSerialPortName(); //获取当前所有串口的简要信息
            }

            if (port_info != null) //要加判断，否则调用GetAllSerialPortInfo时，若当前电脑无串口连接，则会出错
            {
                //仅当串口信息列表变化时，更新列表
                if (Enumerable.SequenceEqual(temp_port_info, port_info) == false)
                {
                    //先清除之前的元素
                    Port_ComboBox.Items.Clear();

                    //添加元素
                    foreach (string s in port_info)
                    {
                        Port_ComboBox.Items.Add(s);

                        //专用于识别特征字符（可以不加）
                        if (FindCharacterInSerialPortComboBox(s, "USB") == true)
                        {
                            Port_ComboBox.SelectedIndex = i;
                        }
                        i++;
                    }
                }
            }
        }

        //专用于，识别连接设备串口的端口名特征字符，比如USB、CH340等
        private bool FindCharacterInSerialPortComboBox(string port_name, string character)
        {
            if (port_name.Contains(character) == true)
                return true;
            else
                return false;
        }
    }
}
