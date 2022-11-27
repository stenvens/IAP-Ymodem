using System;
using System.Diagnostics;

using System.IO; //文件处理
using System.IO.Ports; //查看电脑端口
using System.Threading; //创建多线程使用

namespace Ymodem_tool
{
    public enum YModemType
    {
        YModem,
        YModem_1K
    }

    public class Ymodem
    {
        #region 变量定义
        private SerialPort serialPort = new SerialPort();
        public event EventHandler NowDownloadProgressEvent;
        public event EventHandler DownloadResultEvent;

        private string _path;
        private string _portName;
        private int _baudRate;
        private bool _fileDownloadStop = false;

        public string path
        {
            get
            {
                return _path;
            }

            set
            {
                _path = value;
            }
        }

        public string portName
        {
            get
            {
                return _portName;
            }

            set
            {
                _portName = value;
            }
        }

        public int baudRate
        {
            get
            {
                return _baudRate;
            }

            set
            {
                _baudRate = value;
            }
        }
        #endregion

        public void YmodemUploadFileStop()
        {
            _fileDownloadStop = true;
        }

        /*
        接收端：C
        发送端：SOH 00 FF [41 70 70 2E 62 69 6E 00] [36 32 37 32 00] [00 00 ...] CRC CRC  //文件名：App.bin，文件长度：6272
        接收端：ACK
        接收端：C
        发送端：STX 01 FE data[1024] CRC CRC
        */
        public void YmodemUploadFile(YModemType yModemType, int waitFirstPacketAckMs)
        {
            const byte SOH = 0x01;  /* start of 128-byte data packet - 133字节长度帧 */
            const byte STX = 0x02;  /* start of 1024-byte data packet - 1024字节长度帧 */
            const byte EOT = 0x04;  /* end of transmission - 文件传输结束命令*/
            const byte ACK = 0x06;  /* acknowledge - 接收正确应答命令 */
            //const byte NAK = 0x15;  /* negative acknowledge - 重传当前数据包请求命令 */
            const byte CA = 0x18;   /* two of these in succession aborts transfer - 取消传输命令，连续发送5个该命令 */
            const byte C = 0x43;    /* capital letter C */

            int dataSize = 0;
            int packetNumber = 0;
            byte packetHead = 0;
            bool transmission_ret = false;

            if (yModemType == YModemType.YModem_1K)
            {
                dataSize = 1024;
            }
            else if (yModemType == YModemType.YModem)
            {
                dataSize = 128;
            }
            byte[] data = new byte[dataSize];

            //读取文件开始传输
            FileStream fileStream = new FileStream(@_path, FileMode.Open, FileAccess.Read);
            serialPort.PortName = _portName;
            serialPort.BaudRate = _baudRate;
            serialPort.ReadTimeout = 3000;
            serialPort.Open();
            
            try
            {
                //等待设备请求包
                if (SerialPortWaitReadEx(C) != true)
                {
                    Debugging.output("Wait Device Request Overtime!");
                    goto end;
                }

                //发送初始包
                sendYmodemInitialPacket(SOH, packetNumber, data, 128, _path, fileStream);
                Debugging.output("Send Initial Packet");

                //等待设备回复初始包
                if (waitFirstPacketAckMs > 0)
                {
                    Thread.Sleep(waitFirstPacketAckMs); //某些设备在收到InitialPacket后就擦除整个APP区Flash则会需要大量时间
                }
                if (SerialPortWaitRead(ACK) != true)
                {
                    Debugging.output("Wait First Packet Ack Overtime!");
                    goto end;
                }

                //等待再次握手信号
                if (SerialPortWaitRead(C) != true)
                {
                    Debugging.output("Wait Handshake Overtime!");
                    goto end;
                }

                //文件数据传输
                int fileReadCount;
                do
                {
                    //读取文件
                    fileReadCount = fileStream.Read(data, 0, dataSize);
                    if (fileReadCount == 0) break; //传输文件数据内容完毕

                    //传输有效数据时主要考虑的是最后一包数据的是处理，SOH帧和STX帧有不同的处理
                    //[1] 对于SOH帧，若余下数据小于128字节，则以0x1A填充，该帧长度仍为133字节
                    //[2] 对于STX帧需考虑几种情况：
                    //    余下数据等于1024字节，以1029长度帧发送；
                    //    余下数据小于1024字节，但大于128字节，以1029字节帧长度发送，无效数据以0x1A填充。
                    //    余下数据等于128字节，以133字节帧长度发送。
                    //    余下数据小于128字节，以133字节帧长度发送，无效数据以0x1A填充。
                    if (yModemType == YModemType.YModem)
                    {
                        if (fileReadCount < 128)
                        {
                            for (int i = fileReadCount; i < 128; i++) data[i] = 0x1A;
                        }
                        packetHead = SOH;
                    }
                    else if (yModemType == YModemType.YModem_1K)
                    {
                        if (fileReadCount < 128)
                        {
                            for (int i = fileReadCount; i < 128; i++) data[i] = 0x1A;
                            dataSize = 128;
                            packetHead = SOH;
                        }
                        else if (fileReadCount < 1024)
                        {
                            for (int i = fileReadCount; i < dataSize; i++) data[i] = 0x1A;
                            dataSize = 1024;
                            packetHead = STX;
                        }
                        else
                        {
                            packetHead = STX;
                        }
                    }

                    //发送文件数据包
                    packetNumber++;
                    sendYmodemPacket(packetHead, packetNumber, data, dataSize);

                    //等待包接收回复
                    if (SerialPortWaitRead(ACK) != true)
                    {
                        Debugging.output("Wait Packet Receive Ack Overtime!");
                        goto end;
                    }

                    //停止下载判断
                    if (_fileDownloadStop == true) goto end;

                    //进度刷新
                    int progress = (int)(((float)dataSize * packetNumber) / fileStream.Length * 100);
                    if (progress > 100) progress = 100;
                    Debugging.output("progress:  " + progress);
                    NowDownloadProgressEvent.Invoke(progress, new EventArgs());
                } while (dataSize == fileReadCount);

                //发送结束信号
                serialPort.Write(new byte[] { EOT }, 0, 1);

                //等待结束信号回复
                if (SerialPortWaitRead(ACK) != true)
                {
                    Debugging.output("Wait EOT Signal Ack Overtime!");
                    goto end;
                }

                //发送结束包
                packetNumber = 0;
                dataSize = 128;
                data = new byte[dataSize];
                sendYmodemPacket(SOH, packetNumber, data, dataSize);

                //等待结束包回复
                if (SerialPortWaitRead(ACK) != true)
                {
                    Debugging.output("Wait EOT Packet Ack Overtime!");
                    goto end;
                }

                transmission_ret = true;
            }
            catch { }

        end:
            if (_fileDownloadStop == true) //停止下载结果
            {
                byte[] packet = new byte[5] { CA, CA, CA, CA, CA};
                serialPort.Write(packet, 0, packet.Length);
                Debugging.output("File Transmission Stop!");
                DownloadResultEvent.Invoke(false, new EventArgs());
            }
            else //正常下载结果
            {
                if (transmission_ret == true)
                {
                    Debugging.output("File Transmission Succesfully!");
                    DownloadResultEvent.Invoke(true, new EventArgs());
                }
                else
                {
                    Debugging.output("File Transmission Fail!");
                    DownloadResultEvent.Invoke(false, new EventArgs());
                }
            }
            
            try
            {
                fileStream.Close();
                serialPort.Close();
            }
            catch { }
            
            return;
        }

        //发送初始包
        private void sendYmodemInitialPacket(byte head, int packetNumber, byte[] data, int dataSize, string path, FileStream fileStream)
        {
            string fileName = System.IO.Path.GetFileName(path);
            string fileSize = fileStream.Length.ToString();
            
            //添加文件名
            int i;
            for (i = 0; i < fileName.Length && (fileName.ToCharArray()[i] != 0); i++)
            {
                data[i] = (byte)fileName.ToCharArray()[i];
            }
            data[i] = 0;

            //添加文件长度
            int j;
            for (j = 0; j < fileSize.Length && (fileSize.ToCharArray()[j] != 0); j++)
            {
                data[(i + 1) + j] = (byte)fileSize.ToCharArray()[j];
            }
            data[(i + 1) + j] = 0;

            //剩余字节填充0
            for (int k = ((i + 1) + j) + 1; k < dataSize; k++)
            {
                data[k] = 0;
            }

            sendYmodemPacket(head, packetNumber, data, dataSize);
        }

        private void sendYmodemPacket(byte head, int packetNumber, byte[] data, int dataSize)
        {
            byte[] packet = new byte[3 + dataSize + 2];

            //帧头
            packet[0] = head;

            //包号
            packet[1] = Convert.ToByte(packetNumber & 0xFF);

            //包号反码
            packet[2] = Convert.ToByte((~packet[1]) & 0xFF);

            //数据区
            Array.Copy(data, 0, packet, 3, dataSize);

            //校验
            Crc16Ccitt crc16Ccitt = new Crc16Ccitt(InitialCrcValue.Zeros);
            ushort crc = crc16Ccitt.ComputeChecksum(data, dataSize);
            packet[3 + dataSize] = (byte)(crc / 256);
            packet[3 + dataSize + 1] = (byte)(crc % 256);

            serialPort.Write(packet, 0, 3 + dataSize + 2);
        }

        private bool SerialPortWaitRead(byte data)
        {
            try
            {
                if (serialPort.ReadByte() == data) return true;
            }
            catch { }

            return false;
        }

        /// <summary>
        /// 获取时间戳
        /// </summary>
        /// <returns></returns>
        public static long GetMsTimeStamp()
        {
            TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalMilliseconds);
        }

        private bool SerialPortWaitReadEx(byte data)
        {
            long end_msec = GetMsTimeStamp() + serialPort.ReadTimeout; //超时时间的毫秒数
            long start_msec = GetMsTimeStamp(); //当前时间的毫秒数

            while (start_msec <= end_msec)
            {
                try
                {
                    if (serialPort.ReadByte() == data) return true;
                }
                catch { }

                //更新当前时间
                start_msec = GetMsTimeStamp();
            }

            return false;
        }
    }
}