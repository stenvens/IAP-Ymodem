using System;
using System.Diagnostics;

using System.IO; //�ļ�����
using System.IO.Ports; //�鿴���Զ˿�
using System.Threading; //�������߳�ʹ��

namespace Ymodem_tool
{
    public enum YModemType
    {
        YModem,
        YModem_1K
    }

    public class Ymodem
    {
        #region ��������
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
        ���նˣ�C
        ���Ͷˣ�SOH 00 FF [41 70 70 2E 62 69 6E 00] [36 32 37 32 00] [00 00 ...] CRC CRC  //�ļ�����App.bin���ļ����ȣ�6272
        ���նˣ�ACK
        ���նˣ�C
        ���Ͷˣ�STX 01 FE data[1024] CRC CRC
        */
        public void YmodemUploadFile(YModemType yModemType, int waitFirstPacketAckMs)
        {
            const byte SOH = 0x01;  /* start of 128-byte data packet - 133�ֽڳ���֡ */
            const byte STX = 0x02;  /* start of 1024-byte data packet - 1024�ֽڳ���֡ */
            const byte EOT = 0x04;  /* end of transmission - �ļ������������*/
            const byte ACK = 0x06;  /* acknowledge - ������ȷӦ������ */
            //const byte NAK = 0x15;  /* negative acknowledge - �ش���ǰ���ݰ��������� */
            const byte CA = 0x18;   /* two of these in succession aborts transfer - ȡ�����������������5�������� */
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

            //��ȡ�ļ���ʼ����
            FileStream fileStream = new FileStream(@_path, FileMode.Open, FileAccess.Read);
            serialPort.PortName = _portName;
            serialPort.BaudRate = _baudRate;
            serialPort.ReadTimeout = 3000;
            serialPort.Open();
            
            try
            {
                //�ȴ��豸�����
                if (SerialPortWaitReadEx(C) != true)
                {
                    Debugging.output("Wait Device Request Overtime!");
                    goto end;
                }

                //���ͳ�ʼ��
                sendYmodemInitialPacket(SOH, packetNumber, data, 128, _path, fileStream);
                Debugging.output("Send Initial Packet");

                //�ȴ��豸�ظ���ʼ��
                if (waitFirstPacketAckMs > 0)
                {
                    Thread.Sleep(waitFirstPacketAckMs); //ĳЩ�豸���յ�InitialPacket��Ͳ�������APP��Flash�����Ҫ����ʱ��
                }
                if (SerialPortWaitRead(ACK) != true)
                {
                    Debugging.output("Wait First Packet Ack Overtime!");
                    goto end;
                }

                //�ȴ��ٴ������ź�
                if (SerialPortWaitRead(C) != true)
                {
                    Debugging.output("Wait Handshake Overtime!");
                    goto end;
                }

                //�ļ����ݴ���
                int fileReadCount;
                do
                {
                    //��ȡ�ļ�
                    fileReadCount = fileStream.Read(data, 0, dataSize);
                    if (fileReadCount == 0) break; //�����ļ������������

                    //������Ч����ʱ��Ҫ���ǵ������һ�����ݵ��Ǵ���SOH֡��STX֡�в�ͬ�Ĵ���
                    //[1] ����SOH֡������������С��128�ֽڣ�����0x1A��䣬��֡������Ϊ133�ֽ�
                    //[2] ����STX֡�迼�Ǽ��������
                    //    �������ݵ���1024�ֽڣ���1029����֡���ͣ�
                    //    ��������С��1024�ֽڣ�������128�ֽڣ���1029�ֽ�֡���ȷ��ͣ���Ч������0x1A��䡣
                    //    �������ݵ���128�ֽڣ���133�ֽ�֡���ȷ��͡�
                    //    ��������С��128�ֽڣ���133�ֽ�֡���ȷ��ͣ���Ч������0x1A��䡣
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

                    //�����ļ����ݰ�
                    packetNumber++;
                    sendYmodemPacket(packetHead, packetNumber, data, dataSize);

                    //�ȴ������ջظ�
                    if (SerialPortWaitRead(ACK) != true)
                    {
                        Debugging.output("Wait Packet Receive Ack Overtime!");
                        goto end;
                    }

                    //ֹͣ�����ж�
                    if (_fileDownloadStop == true) goto end;

                    //����ˢ��
                    int progress = (int)(((float)dataSize * packetNumber) / fileStream.Length * 100);
                    if (progress > 100) progress = 100;
                    Debugging.output("progress:  " + progress);
                    NowDownloadProgressEvent.Invoke(progress, new EventArgs());
                } while (dataSize == fileReadCount);

                //���ͽ����ź�
                serialPort.Write(new byte[] { EOT }, 0, 1);

                //�ȴ������źŻظ�
                if (SerialPortWaitRead(ACK) != true)
                {
                    Debugging.output("Wait EOT Signal Ack Overtime!");
                    goto end;
                }

                //���ͽ�����
                packetNumber = 0;
                dataSize = 128;
                data = new byte[dataSize];
                sendYmodemPacket(SOH, packetNumber, data, dataSize);

                //�ȴ��������ظ�
                if (SerialPortWaitRead(ACK) != true)
                {
                    Debugging.output("Wait EOT Packet Ack Overtime!");
                    goto end;
                }

                transmission_ret = true;
            }
            catch { }

        end:
            if (_fileDownloadStop == true) //ֹͣ���ؽ��
            {
                byte[] packet = new byte[5] { CA, CA, CA, CA, CA};
                serialPort.Write(packet, 0, packet.Length);
                Debugging.output("File Transmission Stop!");
                DownloadResultEvent.Invoke(false, new EventArgs());
            }
            else //�������ؽ��
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

        //���ͳ�ʼ��
        private void sendYmodemInitialPacket(byte head, int packetNumber, byte[] data, int dataSize, string path, FileStream fileStream)
        {
            string fileName = System.IO.Path.GetFileName(path);
            string fileSize = fileStream.Length.ToString();
            
            //����ļ���
            int i;
            for (i = 0; i < fileName.Length && (fileName.ToCharArray()[i] != 0); i++)
            {
                data[i] = (byte)fileName.ToCharArray()[i];
            }
            data[i] = 0;

            //����ļ�����
            int j;
            for (j = 0; j < fileSize.Length && (fileSize.ToCharArray()[j] != 0); j++)
            {
                data[(i + 1) + j] = (byte)fileSize.ToCharArray()[j];
            }
            data[(i + 1) + j] = 0;

            //ʣ���ֽ����0
            for (int k = ((i + 1) + j) + 1; k < dataSize; k++)
            {
                data[k] = 0;
            }

            sendYmodemPacket(head, packetNumber, data, dataSize);
        }

        private void sendYmodemPacket(byte head, int packetNumber, byte[] data, int dataSize)
        {
            byte[] packet = new byte[3 + dataSize + 2];

            //֡ͷ
            packet[0] = head;

            //����
            packet[1] = Convert.ToByte(packetNumber & 0xFF);

            //���ŷ���
            packet[2] = Convert.ToByte((~packet[1]) & 0xFF);

            //������
            Array.Copy(data, 0, packet, 3, dataSize);

            //У��
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
        /// ��ȡʱ���
        /// </summary>
        /// <returns></returns>
        public static long GetMsTimeStamp()
        {
            TimeSpan ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalMilliseconds);
        }

        private bool SerialPortWaitReadEx(byte data)
        {
            long end_msec = GetMsTimeStamp() + serialPort.ReadTimeout; //��ʱʱ��ĺ�����
            long start_msec = GetMsTimeStamp(); //��ǰʱ��ĺ�����

            while (start_msec <= end_msec)
            {
                try
                {
                    if (serialPort.ReadByte() == data) return true;
                }
                catch { }

                //���µ�ǰʱ��
                start_msec = GetMsTimeStamp();
            }

            return false;
        }
    }
}