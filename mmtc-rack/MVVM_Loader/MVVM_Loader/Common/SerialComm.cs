using MVVM_Loader.Model;
using MVVM_Loader.ViewModel;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Packaging;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media;

namespace MVVM_Loader.Common
{
    public class SerialComm
    {
        public string SelectComm = "";
        public string nBaudRate = "";
        public string nDataBits = "";
        public string nParity = "";

        public const char STX = (char)0x02;
        public const char CR = (char)0x0D;
        public const char LF = (char)0x0A;

        Calculator cal;
        //SendMessge sm;

        public BackgroundWorker _UploadWorker = null;
        public BackgroundWorker _DownloadWorker = null;

        FuncViewModel fvm = null;
        ConnetViewModel cvm = null;
        Item1ViewModel i1vm = null;
        Item2ViewModel i2vm = null;
        Item3ViewModel i3vm = null;

        byte[] hiByte = new byte[2] { 0x00, 0x01 };
        //                               0     1     2     3     4     5     6     7     8     9     10
        byte[] loByte1 = new byte[92] { 0x00, 0x49, 0x4D, 0xB7, 0xB8, 0xBA, 0xBB, 0xA6, 0x5A, 0x5E, 
                                        0xAC, 0x62, 0x33, 0x34, 0x35, 0x39, 0x3A, 0x3B, 0x3F, 0x43,
                                        0x47, 0x4E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x7D, 0x71,
                                        0x72, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
                                        0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91,
                                        0x92, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9C, 0x9D, 0x9E,
                                        0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7, 0xA8, 0xA9,
                                        0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB9,
                                        0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5,
                                        0xC6, 0xC7
                                      };
        byte[] loByte2 = new byte[92] { 0x00, 0x00, 0x00, 0x1B, 0x1C, 0x1E, 0x1F, 0x0A, 0x00, 0x00, 
                                        0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x45, 0x39,
                                        0x3A, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
                                        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
                                        0x5A, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x00, 0x01, 0x02,
                                        0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0B, 0x0C, 0x0D,
                                        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1D,
                                        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
                                        0x2A, 0x2B
                                        };

        public string[] TempNumber = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32" };
        public byte[] TempHNumber = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20 };
        public bool bWorkThread_rx = false;
        public bool bWorkThread_tx = false;
        public bool reSend = false;
        public bool firstWrite = false; // 첫번째 통신 체크
        public bool[] bRunStopFlag = new bool[32];
        public bool[] bContactFlag = new bool[32];
        string RecvStrData = string.Empty;
        private object LockEvent = new object();

        private List<Byte> _RecvDataList, _RecvAlarmDataList;
        private List<Byte> RecvDataList
        {
            get
            {
                if (_RecvDataList == null)
                    _RecvDataList = new List<byte>();
                return _RecvDataList;
            }
        }

        private List<Byte> RecvAlarmDataList
        {
            get
            {
                if (_RecvAlarmDataList == null)
                    _RecvAlarmDataList = new List<byte>();
                return _RecvAlarmDataList;
            }
        }

        public SerialPort sPort;

        public SerialComm()
        {
            Global.gSerialComm = this;
            cal = new Calculator();
            sPort = new SerialPort();
            fvm = (FuncViewModel)App.Current.Services.GetService(typeof(FuncViewModel));
            cvm = (ConnetViewModel)App.Current.Services.GetService(typeof(ConnetViewModel));
            i1vm = (Item1ViewModel)App.Current.Services.GetService(typeof(Item1ViewModel));
            i2vm = (Item2ViewModel)App.Current.Services.GetService(typeof(Item2ViewModel));
            i3vm = (Item3ViewModel)App.Current.Services.GetService(typeof(Item3ViewModel));
        }
        public void _Sport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
            {
                int bufferOffset = 0;
                int bytesToRead = sPort.BytesToRead;
                byte[] bytesBuffer = new byte[bytesToRead];

                while (bytesToRead > 0)
                {
                    try
                    {
                        if ((bufferOffset > -1) && (bytesToRead - bufferOffset > -1))
                        {
                            int readBytes = sPort.Read(bytesBuffer, bufferOffset, bytesToRead - bufferOffset);
                            bytesToRead -= readBytes;
                            bufferOffset += readBytes;
                        }
                        else
                        {
                            bufferOffset = 0;
                            bytesToRead = 0;
                        }
                    }
                    catch (Exception ex)
                    {
                        sPort.DiscardInBuffer();
                        sPort.DiscardOutBuffer();
                        bufferOffset = 0;
                        bytesToRead = 0;
                    }
                }

                RecvDataList.AddRange(bytesBuffer);
            }
        }
        public void Connect_Run()
        {
            if (!sPort.IsOpen)
            {
                sPort.PortName = cvm.Ctm.SelPort;
                sPort.BaudRate = Convert.ToInt32(cvm.Ctm.Selpeed);
                sPort.DataBits = Convert.ToInt32(cvm.Ctm.Seldatalen);
                if (cvm.Ctm.Selparity == "None")
                    sPort.Parity = Parity.None;
                else if (cvm.Ctm.Selparity == "Odd")
                    sPort.Parity = Parity.Odd;
                else if (cvm.Ctm.Selparity == "Even")
                    sPort.Parity = Parity.Even;
                else
                    sPort.Parity = Parity.None;

                sPort.Handshake = Handshake.None;
                if (cvm.Ctm.Selstopbit == "1")
                    sPort.StopBits = StopBits.One;
                else if (cvm.Ctm.Selstopbit == "0")
                    sPort.StopBits = StopBits.Two;
                sPort.ReadTimeout = 500;
                sPort.WriteTimeout = 500;
                sPort.DataReceived += new SerialDataReceivedEventHandler(_Sport_DataReceived);

                sPort.Open();

                if (sPort.IsOpen)
                {
                    cvm.Ctm.Isenabed = "False";
                    cvm.Ctm.Isenabed2 = "True";
                    fvm.BackgroundColor1 = Brushes.GreenYellow;
                    fvm.statustxt = "통신 가능";
                }
            }
            else
            {

            }
        }
        public void Connect_Stop()
        {
            if (sPort.IsOpen)
            {
                cvm.Ctm.Isenabed = "True";
                cvm.Ctm.Isenabed2 = "False";
                sPort.Close();
                sPort.Dispose();
                fvm.BackgroundColor1 = Brushes.OrangeRed;
                fvm.statustxt = "통신 불가";
            }
        }

        public void sendMessage(ArrayList alToSend)
        {
            lock (LockEvent)
            {
                if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
                {
                    if (alToSend.Count > 0)
                    {
                        byte[] bytesToSend = new byte[alToSend.Count + 2]; // the 2 is for the CRC we'll add at the end
                        String sMessageSent = "";
                        UInt16 crc16 = 0xFFFF;
                        for (int i = 0; i < alToSend.Count; i++)
                        {
                            Byte byteFromArray = (Byte)alToSend[i];
                            bytesToSend[i] = byteFromArray;
                            crc16 = cal.CalculateCRC(byteFromArray, crc16);
                            sMessageSent += bytesToSend[i].ToString("X").PadLeft(2, '0') + " ";
                        }

                        bytesToSend[bytesToSend.Count() - 2] = (Byte)(crc16 % 0x100);
                        sMessageSent += bytesToSend[bytesToSend.Count() - 2].ToString("X").PadLeft(2, '0') + " ";

                        bytesToSend[bytesToSend.Count() - 1] = (Byte)(crc16 / 0x100);
                        sMessageSent += bytesToSend[bytesToSend.Count() - 1].ToString("X").PadLeft(2, '0') + " ";

                        try
                        {
                            if (sPort.IsOpen)
                            {
                                sPort.Write(bytesToSend, 0, bytesToSend.Length);
                                Console.WriteLine("SEND : {0} ", sMessageSent);
                            }
                            sMessageSent = "";
                        }
                        catch (Exception ex)
                        {

                        }
                    }
                }

            }

        }
        public void check_download()
        {
            _DownloadWorker = new BackgroundWorker();
            _DownloadWorker.WorkerReportsProgress = true;
            _DownloadWorker.WorkerSupportsCancellation = true;
            _DownloadWorker.DoWork += Serial_Download;
            _DownloadWorker.ProgressChanged += _UploadWorker_Progresschanged;
            _DownloadWorker.RunWorkerAsync();
        }
        private void _UploadWorker_Progresschanged(object sender, ProgressChangedEventArgs e)
        {
            fvm.uiPbint = e.ProgressPercentage;
        }
        public void download_Serial(byte hitem, byte litem, int cnt)
        {
            ArrayList alReturn = new ArrayList();
            alReturn.Add((byte)Convert.ToByte(cvm.Ctm.M7num));

            alReturn.Add((byte)0x03);

            alReturn.Add(hitem);
            alReturn.Add(litem);

            alReturn.Add((byte)0x00);
            alReturn.Add((byte)0x20);

            sendMessage(alReturn);
            alReturn.Clear();

            waitRecv(cnt);
        }
        public void upload_Serial(byte hitem, byte litem, string txtname, int cnt)
        {
            string uploadstr = "";
            string HiStr = "";
            string LoStr = "";
            ArrayList alReturn = new ArrayList();

            alReturn.Add(Convert.ToByte(cvm.Ctm.M7num, 16));
            alReturn.Add((byte)0x06);

            alReturn.Add(hitem);
            alReturn.Add(litem);
            uploadstr = Global.gCalculator.tranfer_StringtoHexString(txtname);
            HiStr = uploadstr.Substring(0, 2);
            LoStr = uploadstr.Substring(2, 2);
            alReturn.Add(Convert.ToByte(HiStr, 16));
            alReturn.Add(Convert.ToByte(LoStr, 16));
            sendMessage(alReturn);
            alReturn.Clear();

            waitRecv(cnt);
        }
        public void Serial_Download(object sender, DoWorkEventArgs e)
        {
            if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
            {
                _DownloadWorker.ReportProgress(0);
                download_Serial(0x00, 0x33, 1); //51 ~ 82
                _DownloadWorker.ReportProgress(7);
                download_Serial(0x00, 0x53, 2); //83 ~ 114
                _DownloadWorker.ReportProgress(14);
                download_Serial(0x00, 0x73, 3); //115 ~ 146
                _DownloadWorker.ReportProgress(21);
                download_Serial(0x00, 0x94, 4); //148 ~ 179
                _DownloadWorker.ReportProgress(28);
                download_Serial(0x00, 0xB4, 5); //180 ~ 211
                _DownloadWorker.ReportProgress(35);
                download_Serial(0x00, 0xD4, 6); //212 ~ 243
                _DownloadWorker.ReportProgress(42);
                download_Serial(0x00, 0xF4, 7); //244 ~ 275
                _DownloadWorker.ReportProgress(49);
                download_Serial(0x01, 0x14, 8); //276 ~ 307
                _DownloadWorker.ReportProgress(56);
                download_Serial(0x01, 0x34, 9); //308 ~ 339
                _DownloadWorker.ReportProgress(63);
                download_Serial(0x01, 0x54, 10); //340 ~ 371
                _DownloadWorker.ReportProgress(70);
                download_Serial(0x01, 0x74, 11); //372 ~ 403
                _DownloadWorker.ReportProgress(77);
                download_Serial(0x01, 0x94, 12); //404 ~ 435
                _DownloadWorker.ReportProgress(84);
                download_Serial(0x01, 0xB4, 13); //436 ~ 467
                _DownloadWorker.ReportProgress(93);
                download_Serial(0x01, 0xD4, 14); //468 ~ 499
                _DownloadWorker.ReportProgress(100);
                cvm.Ctm.Statusck = "Download Done.";
            }
        }
        public void Cehck_AutoPid(int ch, int val)
        {
            Global.gch_Pid = ch;
            Global.gch_Pid_value = val;
            if (_UploadWorker != null)
                _UploadWorker.CancelAsync();

            _UploadWorker = new BackgroundWorker();
            _UploadWorker.WorkerReportsProgress = true;
            _UploadWorker.WorkerSupportsCancellation = true;
            _UploadWorker.DoWork += Serial_Pid;
            _UploadWorker.ProgressChanged += _UploadWorker_Progresschanged;
            _UploadWorker.RunWorkerAsync();
        }
        public void Serial_Pid(object sender, DoWorkEventArgs e)
        {
            _UploadWorker.ReportProgress(0);
            if (Global.gch_Pid == 0)
            {
                byte ain = 0x56;
                for (int i = 0; i < 4; i++)
                    upload_Serial(0x00, ain++, Global.gch_Pid_value.ToString(), 0);
            }
            else if(Global.gch_Pid == 1)
                upload_Serial(0x00, 0x56, Global.gch_Pid_value.ToString(), 0);
            else if (Global.gch_Pid == 2)
                upload_Serial(0x00, 0x57, Global.gch_Pid_value.ToString(), 0);
            else if (Global.gch_Pid == 3)
                upload_Serial(0x00, 0x58, Global.gch_Pid_value.ToString(), 0);
            else if (Global.gch_Pid == 4)
                upload_Serial(0x00, 0x59, Global.gch_Pid_value.ToString(), 0);

            _UploadWorker.ReportProgress(100);
        }
        public void Cehck_Reset(int ch)
        {
            Global.gchresetch = ch;
            if(_UploadWorker != null)
                _UploadWorker.CancelAsync();
                
            _UploadWorker = new BackgroundWorker();
            _UploadWorker.WorkerReportsProgress = true;
            _UploadWorker.WorkerSupportsCancellation = true;
            _UploadWorker.DoWork += Serial_Reset;
            _UploadWorker.ProgressChanged += _UploadWorker_Progresschanged;
            _UploadWorker.RunWorkerAsync();
        }
        public void Serial_Reset(object sender, DoWorkEventArgs e)
        {
            _UploadWorker.ReportProgress(0);
            upload_Serial(0x00, 0x48, Global.gchresetch.ToString(), 0);
            _UploadWorker.ReportProgress(100);
        }
            public void Check_Upload(int chnum, string ch1_value, string ch2_value, string ch3_value, string ch4_value, string chflag, string chsymbol)
        {
            Global.gchnum = chnum;
            Global.gsymbol = chsymbol;
            Global.gchflag = chflag;
            Global.gch_value1 = ch1_value;
            Global.gch_value2 = ch2_value;
            Global.gch_value3 = ch3_value;
            Global.gch_value4 = ch4_value;
            if(_UploadWorker != null)
                _UploadWorker.CancelAsync();

            _UploadWorker = new BackgroundWorker();
            _UploadWorker.WorkerReportsProgress = true;
            _UploadWorker.WorkerSupportsCancellation = true;
            _UploadWorker.DoWork += Serial_Upload;
            _UploadWorker.ProgressChanged += _UploadWorker_Progresschanged;
            _UploadWorker.RunWorkerAsync();
        }
        public void Serial_Upload(object sender, DoWorkEventArgs e)
        {
            if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
            {
                if (Global.gchflag == "Hidden") // 파라메타 1개짜리 업로드
                {
                    step_Upload(true, 0, hiByte[0], loByte1[Global.gchnum]);
                }
                else if (Global.gchflag == "Visible")
                {
                    if (Global.gchnum == 1 || Global.gchnum == 8 || Global.gchnum == 9 ||
                        Global.gchnum == 14 || Global.gchnum == 17 || Global.gchnum == 18 || Global.gchnum == 21)
                    {
                        step_Upload(false, 0, hiByte[0], loByte1[Global.gchnum]);
                    }
                    else if (Global.gchnum == 19 || Global.gchnum == 70) // Double
                    {
                        step_Upload(false, 3, hiByte[0], loByte1[Global.gchnum]);
                    }
                    else if((Global.gchnum >= 41 && Global.gchnum <= 44) || (Global.gchnum >= 51 && Global.gchnum <= 53))
                    {
                        step_Upload(false, 8, hiByte[0], loByte1[Global.gchnum]);
                        step_Upload(false, 9, hiByte[1], loByte2[Global.gchnum]);
                    }
                    else if(Global.gchnum == 57 || Global.gchnum == 60 || Global.gchnum == 61 ||
                       Global.gchnum == 72 || (Global.gchnum >= 75 && Global.gchnum <= 78))
                    {
                        step_Upload(false, 6, hiByte[0], loByte1[Global.gchnum]);
                        step_Upload(false, 7, hiByte[1], loByte2[Global.gchnum]);
                    }
                    else if((Global.gchnum >= 22 && Global.gchnum <= 56)) // CH3 부터 0x01
                    {
                        step_Upload(false, 4, hiByte[0], loByte1[Global.gchnum]);
                        step_Upload(false, 5, hiByte[1], loByte2[Global.gchnum]);
                    }
                    else if ((Global.gchnum >= 3 && Global.gchnum <= 7) || Global.gchnum == 10 ||
                        (Global.gchnum >= 57 && Global.gchnum <= 91)) // CH2 부터 0x01
                    {
                        step_Upload(false, 1, hiByte[0], loByte1[Global.gchnum]);
                        step_Upload(false, 2, hiByte[1], loByte2[Global.gchnum]);
                    }
                }
                cvm.Ctm.Statusck = Global.gsymbol + " Upload Done.";
            }
        }
        public void step_Upload(bool nFlag,int substep, byte gHi, byte gLo)
        {
            double tempdou = 0.0;
            string tempstr = "";
            byte addvalue = 0x64;

            if (nFlag)
            {
                _UploadWorker.ReportProgress(0);
                upload_Serial(gHi, gLo, Global.gch_value1, Global.gchnum); 
                _UploadWorker.ReportProgress(100);
            }
            else
            {
                switch(substep)
                {
                    case 0:
                        _UploadWorker.ReportProgress(0);
                        upload_Serial(gHi, gLo++, Global.gch_value1, Global.gchnum); 
                        _UploadWorker.ReportProgress(25);
                        upload_Serial(gHi, gLo++, Global.gch_value2, Global.gchnum); 
                        _UploadWorker.ReportProgress(50);
                        upload_Serial(gHi, gLo++, Global.gch_value3, Global.gchnum); 
                        _UploadWorker.ReportProgress(75);
                        upload_Serial(gHi, gLo++, Global.gch_value4, Global.gchnum); 
                        _UploadWorker.ReportProgress(100);
                        break;
                    case 1:
                        _UploadWorker.ReportProgress(0);
                        upload_Serial(gHi, gLo, Global.gch_value1, Global.gchnum); 
                        _UploadWorker.ReportProgress(25);
                        break;
                    case 2: //CH2부터 gHi 0x01
                        upload_Serial(gHi, gLo, Global.gch_value2, Global.gchnum); 
                        _UploadWorker.ReportProgress(50);
                        upload_Serial(gHi, gLo += addvalue, Global.gch_value3, Global.gchnum); 
                        _UploadWorker.ReportProgress(75);
                        upload_Serial(gHi, gLo += addvalue, Global.gch_value4, Global.gchnum); 
                        _UploadWorker.ReportProgress(100);
                        break;
                    case 3: // Double
                        _UploadWorker.ReportProgress(0);
                        tempdou = Convert.ToDouble(Global.gch_value1);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo++, tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(25);
                        tempdou = Convert.ToDouble(Global.gch_value2);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo++, tempstr, Global.gchnum); 
                        _UploadWorker.ReportProgress(50);
                        tempdou = Convert.ToDouble(Global.gch_value3);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo++, tempstr, Global.gchnum); 
                        _UploadWorker.ReportProgress(75);
                        tempdou = Convert.ToDouble(Global.gch_value4);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo++, tempstr, Global.gchnum); 
                        _UploadWorker.ReportProgress(100);
                        break;
                    case 4: //CH3부터 gHi 0x01
                        _UploadWorker.ReportProgress(0);
                        upload_Serial(gHi, gLo, Global.gch_value1, Global.gchnum);
                        _UploadWorker.ReportProgress(25);
                        upload_Serial(gHi, gLo += addvalue, Global.gch_value2, Global.gchnum);
                        _UploadWorker.ReportProgress(50);
                        break;
                    case 5: //CH3부터 gHi 0x01
                        upload_Serial(gHi, gLo += addvalue, Global.gch_value3, Global.gchnum);
                        _UploadWorker.ReportProgress(75);
                        upload_Serial(gHi, gLo += addvalue, Global.gch_value4, Global.gchnum);
                        _UploadWorker.ReportProgress(100);
                        break;
                    case 6: // Double CH2 0x01
                        _UploadWorker.ReportProgress(0);
                        tempdou = Convert.ToDouble(Global.gch_value1);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo, tempstr, Global.gchnum);
                        break;
                    case 7: // Double CH2 0x01
                        _UploadWorker.ReportProgress(25);
                        tempdou = Convert.ToDouble(Global.gch_value2);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo , tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(50);
                        tempdou = Convert.ToDouble(Global.gch_value3);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo += addvalue, tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(75);
                        tempdou = Convert.ToDouble(Global.gch_value4);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo += addvalue, tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(100);
                        break;
                    case 8: // Double CH3 0x01
                        _UploadWorker.ReportProgress(0);
                        tempdou = Convert.ToDouble(Global.gch_value1);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo, tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(25);
                        tempdou = Convert.ToDouble(Global.gch_value2);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo += addvalue, tempstr, Global.gchnum);
                        break;
                    case 9: // Double CH3 0x01
                        _UploadWorker.ReportProgress(50);
                        tempdou = Convert.ToDouble(Global.gch_value3);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo, tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(75);
                        tempdou = Convert.ToDouble(Global.gch_value4);
                        tempstr = Global.gCalculator.tranfer_DoubletointString(tempdou);
                        upload_Serial(gHi, gLo += addvalue, tempstr, Global.gchnum);
                        _UploadWorker.ReportProgress(100);
                        break;
                }
            }
        }
        public void waitRecv(int num)
        {
            if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
            {
                Thread.Sleep(100);
                if (RecvDataList.Count >= 3)
                {
                    String sMessageRecieved = "";
                    bool bCRCPasses = false;
                    UInt16 crc16 = 0xFFFF;
                    byte fun_byte;
                    int packet_length;
                    int data_length;

                    /*
                    if ((RecvDataList[0] < 1) || (RecvDataList[0] > 12))
                    {
                        RecvDataList.Clear();
                        return;
                    }
                    */

                    //RTU 첫번째 바이트 : 통신주소
                    fun_byte = RecvDataList[1];

                    if ((fun_byte != 03) && (fun_byte != 06) && (fun_byte != 16))
                    {
                        RecvDataList.Clear();
                        return;
                    }
                    if (fun_byte == 0x03) //D-Register의 연속읽기      --> 수신 : 8바이트이상
                    {
                        data_length = RecvDataList[2];
                        packet_length = 2 + 1 + data_length + 2;
                        if (packet_length > RecvDataList.Count)
                        {
                            //RecvDataList.RemoveRange(0, packet_length);
                            //sPort.DiscardInBuffer();
                            //sPort.DiscardOutBuffer();
                            return;
                        }
                    }
                    else if (fun_byte == 0x06)
                    {
                        if (RecvDataList.Count == 5)
                            packet_length = 5;
                        else
                            packet_length = 8;
                    }
                    else
                    {
                        packet_length = 1;
                        return;
                    }


                    for (int i = 0; i < packet_length; i++)
                    {
                        sMessageRecieved += RecvDataList[i].ToString("X2").PadLeft(2, '0') + " ";
                        crc16 = cal.CalculateCRC((Byte)RecvDataList[i], crc16);
                    }

                    Console.WriteLine("RECV : {0} ", sMessageRecieved);

                    if (crc16 == 0)
                        bCRCPasses = true;


                    if (bCRCPasses == true)
                    {
                        string[] tempStr = new string[32];
                        short[] tempshort = new short[32];
                        double[] tempdouble = new double[32];
                        int j = 0, k = 0;
                        string funcd = RecvDataList[1].ToString();
                        int index = RecvDataList[0] - 1;

                        if (funcd == "3")
                        {
                            for (int i = 0; i < 32; i++)
                            {
                                j = (i + 3) + i;//5 7 9
                                k = (i + 4) + i;//6 8 10
                                if (i == 0)
                                    tempStr[i] = RecvDataList[i + 3].ToString("X2") + RecvDataList[i + 4].ToString("X2"); // 3, 4
                                else
                                    tempStr[i] = RecvDataList[j].ToString("X2") + RecvDataList[k].ToString("X2"); // 

                                tempshort[i] = Convert.ToInt16(tempStr[i], 16);
                                tempdouble[i] = tempshort[i] / 10.0;
                            }

                            paring_text(num, tempStr, tempshort, tempdouble);
                        }
                    }

                    RecvDataList.RemoveRange(0, packet_length);
                    sPort.DiscardInBuffer();
                    sPort.DiscardOutBuffer();
                }
            }
        }
        public void paring_text(int cnt, string[] GetStr, short[] Getshort, double[] Getdouble)
        {
            switch (cnt)
            {
                case 1://51 ~ 82
                    i3vm.Item3Datas[0].g3CH1_1 = Getshort[0].ToString();  //51
                    i3vm.Item3Datas[1].g3CH1_1 = GetStr[1];               //52
                    i3vm.Item3Datas[2].g3CH1_1 = Getshort[2].ToString();  //53  
                    i3vm.Item3Datas[2].g3CH2_1 = Getshort[2].ToString();  //54
                    i3vm.Item3Datas[2].g3CH3_1 = Getshort[2].ToString();  //55
                    i3vm.Item3Datas[2].g3CH4_1 = Getshort[2].ToString();  //56
                    i3vm.Item3Datas[3].g3CH1_1 = Getshort[6].ToString();  //57
                    i3vm.Item3Datas[4].g3CH1_1 = Getshort[7].ToString();  //58
                    i3vm.Item3Datas[5].g3CH1_1 = Getshort[8].ToString();  //59
                    i3vm.Item3Datas[5].g3CH2_1 = Getshort[9].ToString();  //60
                    i3vm.Item3Datas[5].g3CH3_1 = Getshort[10].ToString(); //61
                    i3vm.Item3Datas[5].g3CH4_1 = Getshort[11].ToString(); //62
                    i3vm.Item3Datas[6].g3CH1_1 = Getshort[12].ToString(); //63
                    i3vm.Item3Datas[6].g3CH2_1 = Getshort[13].ToString(); //64
                    i3vm.Item3Datas[6].g3CH3_1 = Getshort[14].ToString(); //65
                    i3vm.Item3Datas[6].g3CH4_1 = Getshort[15].ToString(); //66
                    i3vm.Item3Datas[7].g3CH1_1 = Getdouble[16].ToString("0.0"); //67
                    i3vm.Item3Datas[7].g3CH2_1 = Getdouble[17].ToString("0.0"); //68
                    i3vm.Item3Datas[7].g3CH3_1 = Getdouble[18].ToString("0.0"); //69
                    i3vm.Item3Datas[7].g3CH4_1 = Getdouble[19].ToString("0.0"); //70
                    i3vm.Item3Datas[8].g3CH1_1 = Getshort[20].ToString();                    //71
                    i1vm.Item1Datas[0].g1CH1_1 = Getshort[22].ToString(); //73
                    i1vm.Item1Datas[0].g1CH2_1 = Getshort[23].ToString(); //74
                    i1vm.Item1Datas[0].g1CH3_1 = Getshort[24].ToString(); //75
                    i1vm.Item1Datas[0].g1CH4_1 = Getshort[25].ToString(); //76
                    i1vm.Item1Datas[1].g1CH1_1 = Getshort[26].ToString(); //77
                    i3vm.Item3Datas[9].g3CH1_1 = Getshort[27].ToString(); //78
                    i3vm.Item3Datas[9].g3CH2_1 = Getshort[28].ToString(); //79
                    i3vm.Item3Datas[9].g3CH3_1 = Getshort[29].ToString(); //80
                    i3vm.Item3Datas[9].g3CH4_1 = Getshort[30].ToString(); //81
                    break;
               case 2://83 ~ 114
                    i2vm.Item2Datas[5].g2CH1_1 = Getshort[7].ToString();  //90
                    i2vm.Item2Datas[5].g2CH2_1 = Getshort[8].ToString();  //91
                    i2vm.Item2Datas[5].g2CH3_1 = Getshort[9].ToString();  //92
                    i2vm.Item2Datas[5].g2CH4_1 = Getshort[10].ToString(); //93
                    i2vm.Item2Datas[6].g2CH1_1 = Getshort[11].ToString(); //94
                    i2vm.Item2Datas[6].g2CH2_1 = Getshort[12].ToString(); //95
                    i2vm.Item2Datas[6].g2CH3_1 = Getshort[13].ToString(); //96
                    i2vm.Item2Datas[6].g2CH4_1 = Getshort[14].ToString(); //97
                    i2vm.Item2Datas[8].g2CH1_1 = Getshort[15].ToString(); //98
                    i3vm.Item3Datas[17].g3CH1_1 = Getshort[30].ToString();//113
                    i3vm.Item3Datas[18].g3CH1_1 = Getshort[31].ToString();//114
                    break;
               case 3://115 ~ 146
                    i3vm.Item3Datas[10].g3CH1_1 = Getshort[0].ToString();   //115
                    i3vm.Item3Datas[11].g3CH1_1 = Getshort[1].ToString();  //116
                    i3vm.Item3Datas[12].g3CH1_1 = Getshort[2].ToString();  //117
                    i3vm.Item3Datas[13].g3CH1_1 = Getshort[3].ToString();  //118
                    i3vm.Item3Datas[14].g3CH1_1 = Getshort[4].ToString();  //119  
                    i3vm.Item3Datas[15].g3CH1_1 = Getshort[5].ToString();  //120
                    i3vm.Item3Datas[16].g3CH1_1 = Getshort[10].ToString(); //125
                    i3vm.Item3Datas[19].g3CH1_1 = GetStr[12].ToString(); //127
                    i3vm.Item3Datas[20].g3CH1_1 = Getshort[13].ToString();
                    i3vm.Item3Datas[21].g3CH1_1 = Getshort[14].ToString();
                    i3vm.Item3Datas[22].g3CH1_1 = Getshort[15].ToString(); //130
                    i3vm.Item3Datas[23].g3CH1_1 = Getshort[16].ToString();
                    i3vm.Item3Datas[24].g3CH1_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[25].g3CH1_1 = Getshort[18].ToString();
                    i3vm.Item3Datas[26].g3CH1_1 = Getshort[19].ToString();
                    i3vm.Item3Datas[27].g3CH1_1 = Getshort[20].ToString(); //135
                    i3vm.Item3Datas[28].g3CH1_1 = Getshort[21].ToString();
                    i3vm.Item3Datas[29].g3CH1_1 = Getdouble[22].ToString("0.0"); //137
                    i3vm.Item3Datas[30].g3CH1_1 = Getdouble[23].ToString("0.0");
                    i3vm.Item3Datas[31].g3CH1_1 = Getdouble[24].ToString("0.0");
                    i3vm.Item3Datas[32].g3CH1_1 = Getdouble[25].ToString("0.0"); //140
                    i3vm.Item3Datas[33].g3CH1_1 = Getshort[26].ToString();
                    i3vm.Item3Datas[34].g3CH1_1 = Getshort[27].ToString();
                    i3vm.Item3Datas[35].g3CH1_1 = Getshort[28].ToString();
                    i3vm.Item3Datas[36].g3CH1_1 = Getshort[29].ToString();
                    i3vm.Item3Datas[37].g3CH1_1 = Getshort[30].ToString(); //145
                    i3vm.Item3Datas[38].g3CH1_1 = Getshort[31].ToString();
                    break;
               case 4://148 ~ 179
                    i3vm.Item3Datas[39].g3CH1_1 = Getdouble[0].ToString("0.0"); //148
                    i3vm.Item3Datas[40].g3CH1_1 = Getdouble[1].ToString("0.0");
                    i3vm.Item3Datas[41].g3CH1_1 = Getdouble[2].ToString("0.0"); //150
                    i3vm.Item3Datas[42].g3CH1_1 = Getshort[3].ToString();
                    i3vm.Item3Datas[43].g3CH1_1 = Getshort[4].ToString();
                    i3vm.Item3Datas[44].g3CH1_1 = Getshort[5].ToString();
                    i3vm.Item3Datas[45].g3CH1_1 = Getdouble[8].ToString("0.0"); //156
                    i3vm.Item3Datas[46].g3CH1_1 = Getshort[9].ToString();
                    i3vm.Item3Datas[47].g3CH1_1 = Getshort[10].ToString();
                    i3vm.Item3Datas[48].g3CH1_1 = Getdouble[11].ToString("0.0");
                    i3vm.Item3Datas[49].g3CH1_1 = Getdouble[12].ToString("0.0"); //160
                    i3vm.Item3Datas[50].g3CH1_1 = Getshort[13].ToString();
                    i3vm.Item3Datas[51].g3CH1_1 = Getshort[14].ToString();
                    i3vm.Item3Datas[52].g3CH1_1 = Getshort[15].ToString();
                    i3vm.Item3Datas[53].g3CH1_1 = Getshort[16].ToString();
                    i3vm.Item3Datas[54].g3CH1_1 = Getshort[17].ToString(); //165

                    i2vm.Item2Datas[4].g2CH1_1 = Getshort[18].ToString();  //166

                    i3vm.Item3Datas[55].g3CH1_1 = Getshort[19].ToString(); //167

                    if (i1vm.Item1Datas[0].g1CH1_1 == "2" || i1vm.Item1Datas[0].g1CH1_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH1_1 == "4" || i1vm.Item1Datas[0].g1CH1_1 == "5")
                        i3vm.Item3Datas[56].g3CH1_1 = Getdouble[20].ToString("0.0");
                    else
                        i3vm.Item3Datas[56].g3CH1_1 = Getshort[20].ToString();

                    if (i1vm.Item1Datas[0].g1CH1_1 == "2" || i1vm.Item1Datas[0].g1CH1_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH1_1 == "4" || i1vm.Item1Datas[0].g1CH1_1 == "5")
                        i3vm.Item3Datas[57].g3CH1_1 = Getdouble[21].ToString("0.0"); //169
                    else
                        i3vm.Item3Datas[57].g3CH1_1 = Getshort[21].ToString(); //169

                    i2vm.Item2Datas[7].g2CH1_1 = Getshort[24].ToString();  //172

                    i3vm.Item3Datas[58].g3CH1_1 = Getshort[25].ToString(); //173
                    i3vm.Item3Datas[59].g3CH1_1 = Getshort[26].ToString();
                    i3vm.Item3Datas[60].g3CH1_1 = Getdouble[27].ToString("0.0"); //175 
                    i3vm.Item3Datas[61].g3CH1_1 = Getshort[28].ToString();
                    i3vm.Item3Datas[62].g3CH1_1 = Getshort[29].ToString();
                    i3vm.Item3Datas[63].g3CH1_1 = Getdouble[30].ToString("0.0");
                    i3vm.Item3Datas[64].g3CH1_1 = Getdouble[31].ToString("0.0"); //179
                    break;
                case 5://180 ~ 211
                    i3vm.Item3Datas[65].g3CH1_1 = Getdouble[0].ToString("0.0"); //180
                    i3vm.Item3Datas[66].g3CH1_1 = Getdouble[1].ToString("0.0"); //181

                    if( i1vm.Item1Datas[0].g1CH1_1 == "2" || i1vm.Item1Datas[0].g1CH1_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH1_1 == "4" || i1vm.Item1Datas[0].g1CH1_1 == "5")
                        i2vm.Item2Datas[0].g2CH1_1 = Getdouble[3].ToString("0.0");  //183
                    else
                        i2vm.Item2Datas[0].g2CH1_1 = Getshort[3].ToString();  //183

                    if (i1vm.Item1Datas[0].g1CH1_1 == "2" || i1vm.Item1Datas[0].g1CH1_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH1_1 == "4" || i1vm.Item1Datas[0].g1CH1_1 == "5")
                        i2vm.Item2Datas[1].g2CH1_1 = Getdouble[4].ToString("0.0");  //184
                    else
                        i2vm.Item2Datas[1].g2CH1_1 = Getshort[4].ToString();  //184

                    i3vm.Item3Datas[67].g3CH1_1 = Getshort[5].ToString(); //185
                    i2vm.Item2Datas[2].g2CH1_1 = Getshort[6].ToString();  //186
                    i2vm.Item2Datas[3].g2CH1_1 = Getshort[7].ToString();  //187

                    i3vm.Item3Datas[68].g3CH1_1 = Getshort[8].ToString(); //188
                    i3vm.Item3Datas[69].g3CH1_1 = Getshort[9].ToString();
                    i3vm.Item3Datas[70].g3CH1_1 = Getshort[10].ToString(); //190
                    i3vm.Item3Datas[71].g3CH1_1 = Getshort[11].ToString();
                    i3vm.Item3Datas[72].g3CH1_1 = Getshort[12].ToString();
                    i3vm.Item3Datas[73].g3CH1_1 = Getshort[13].ToString();
                    i3vm.Item3Datas[74].g3CH1_1 = Getshort[14].ToString();
                    i3vm.Item3Datas[75].g3CH1_1 = Getshort[15].ToString(); //195
                    i3vm.Item3Datas[76].g3CH1_1 = Getshort[16].ToString();
                    i3vm.Item3Datas[77].g3CH1_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[78].g3CH1_1 = Getshort[18].ToString();
                    i3vm.Item3Datas[79].g3CH1_1 = Getshort[19].ToString(); //199
                    break;
                case 6://212 ~ 243
                    i3vm.Item3Datas[17].g3CH2_1 = Getshort[1].ToString();  //213
                    i3vm.Item3Datas[18].g3CH2_1 = Getshort[2].ToString();  //214
                    i3vm.Item3Datas[10].g3CH2_1 = Getshort[3].ToString();   //215
                    i3vm.Item3Datas[11].g3CH2_1 = Getshort[4].ToString();  //216
                    i3vm.Item3Datas[12].g3CH2_1 = Getshort[5].ToString();  //217
                    i3vm.Item3Datas[13].g3CH2_1 = Getshort[6].ToString();  //218
                    i3vm.Item3Datas[14].g3CH2_1 = Getshort[7].ToString();  //219
                    i3vm.Item3Datas[15].g3CH2_1 = Getshort[8].ToString();  //220
                    i3vm.Item3Datas[16].g3CH2_1 = Getshort[13].ToString(); //225
                    i3vm.Item3Datas[19].g3CH2_1 = GetStr[15].ToString();   //227
                    i3vm.Item3Datas[20].g3CH2_1 = Getshort[16].ToString(); //228
                    i3vm.Item3Datas[21].g3CH2_1 = Getshort[17].ToString(); //229
                    i3vm.Item3Datas[22].g3CH2_1 = Getshort[18].ToString(); //230
                    i3vm.Item3Datas[23].g3CH2_1 = Getshort[19].ToString(); //231
                    i3vm.Item3Datas[24].g3CH2_1 = Getshort[20].ToString(); //232
                    i3vm.Item3Datas[25].g3CH2_1 = Getshort[21].ToString(); //233
                    i3vm.Item3Datas[26].g3CH2_1 = Getshort[22].ToString(); //234
                    i3vm.Item3Datas[27].g3CH2_1 = Getshort[23].ToString(); //235
                    i3vm.Item3Datas[28].g3CH2_1 = Getshort[24].ToString(); //236
                    i3vm.Item3Datas[29].g3CH2_1 = Getdouble[25].ToString("0.0"); //237
                    i3vm.Item3Datas[30].g3CH2_1 = Getdouble[26].ToString("0.0"); //238
                    i3vm.Item3Datas[31].g3CH2_1 = Getdouble[27].ToString("0.0"); //239
                    i3vm.Item3Datas[32].g3CH2_1 = Getdouble[28].ToString("0.0"); //240
                    i3vm.Item3Datas[33].g3CH2_1 = Getshort[29].ToString(); //241
                    i3vm.Item3Datas[34].g3CH2_1 = Getshort[30].ToString(); //242
                    i3vm.Item3Datas[35].g3CH2_1 = Getshort[31].ToString(); //243
                    break;
                case 7://244 ~ 275
                    i3vm.Item3Datas[36].g3CH2_1 = Getshort[0].ToString(); //244
                    i3vm.Item3Datas[37].g3CH2_1 = Getshort[1].ToString(); //245
                    i3vm.Item3Datas[38].g3CH2_1 = Getshort[2].ToString(); //246
                    i3vm.Item3Datas[39].g3CH2_1 = Getdouble[4].ToString("0.0"); //248
                    i3vm.Item3Datas[40].g3CH2_1 = Getdouble[5].ToString("0.0");
                    i3vm.Item3Datas[41].g3CH2_1 = Getdouble[6].ToString("0.0"); //250
                    i3vm.Item3Datas[42].g3CH2_1 = Getshort[7].ToString();
                    i3vm.Item3Datas[43].g3CH2_1 = Getshort[8].ToString();
                    i3vm.Item3Datas[44].g3CH2_1 = Getshort[9].ToString();
                    i3vm.Item3Datas[45].g3CH2_1 = Getdouble[12].ToString("0.0"); //256
                    i3vm.Item3Datas[46].g3CH2_1 = Getshort[13].ToString();
                    i3vm.Item3Datas[47].g3CH2_1 = Getshort[14].ToString();
                    i3vm.Item3Datas[48].g3CH2_1 = Getdouble[15].ToString("0.0");
                    i3vm.Item3Datas[49].g3CH2_1 = Getdouble[16].ToString("0.0"); //260
                    i3vm.Item3Datas[50].g3CH2_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[51].g3CH2_1 = Getshort[18].ToString();
                    i3vm.Item3Datas[52].g3CH2_1 = Getshort[19].ToString();
                    i3vm.Item3Datas[53].g3CH2_1 = Getshort[20].ToString();
                    i3vm.Item3Datas[54].g3CH2_1 = Getshort[21].ToString(); //265

                    i2vm.Item2Datas[4].g2CH2_1 = Getshort[22].ToString();  //266

                    if (i1vm.Item1Datas[0].g1CH2_1 == "2" || i1vm.Item1Datas[0].g1CH2_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH2_1 == "4" || i1vm.Item1Datas[0].g1CH2_1 == "5")
                        i3vm.Item3Datas[55].g3CH2_1 = Getdouble[23].ToString("0.0");
                    else
                        i3vm.Item3Datas[55].g3CH2_1 = Getshort[23].ToString();

                    if (i1vm.Item1Datas[0].g1CH2_1 == "2" || i1vm.Item1Datas[0].g1CH2_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH2_1 == "4" || i1vm.Item1Datas[0].g1CH2_1 == "5")
                        i3vm.Item3Datas[56].g3CH2_1 = Getdouble[24].ToString("0.0");
                    else
                        i3vm.Item3Datas[56].g3CH2_1 = Getshort[24].ToString();

                    i3vm.Item3Datas[57].g3CH2_1 = Getshort[25].ToString(); //269

                    i2vm.Item2Datas[7].g2CH2_1 = Getshort[28].ToString();  //272

                    i3vm.Item3Datas[58].g3CH2_1 = Getshort[29].ToString(); //273
                    i3vm.Item3Datas[59].g3CH2_1 = Getshort[30].ToString();
                    i3vm.Item3Datas[60].g3CH2_1 = Getdouble[31].ToString("0.0"); //275 
                    break;
                case 8://276 ~ 307
                    i3vm.Item3Datas[61].g3CH2_1 = Getshort[0].ToString(); //276
                    i3vm.Item3Datas[62].g3CH2_1 = Getshort[1].ToString();
                    i3vm.Item3Datas[63].g3CH2_1 = Getdouble[2].ToString("0.0");
                    i3vm.Item3Datas[64].g3CH2_1 = Getdouble[3].ToString("0.0"); //279
                    i3vm.Item3Datas[65].g3CH2_1 = Getdouble[4].ToString("0.0"); //280
                    i3vm.Item3Datas[66].g3CH2_1 = Getdouble[5].ToString("0.0");

                    if (i1vm.Item1Datas[0].g1CH2_1 == "2" || i1vm.Item1Datas[0].g1CH2_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH2_1 == "4" || i1vm.Item1Datas[0].g1CH2_1 == "5")
                        i2vm.Item2Datas[0].g2CH2_1 = Getdouble[7].ToString("0.0");   //283
                    else
                        i2vm.Item2Datas[0].g2CH2_1 = Getshort[7].ToString();   //283

                    if (i1vm.Item1Datas[0].g1CH2_1 == "2" || i1vm.Item1Datas[0].g1CH2_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH2_1 == "4" || i1vm.Item1Datas[0].g1CH2_1 == "5")
                        i2vm.Item2Datas[1].g2CH2_1 = Getdouble[8].ToString("0.0");   //284
                    else
                        i2vm.Item2Datas[1].g2CH2_1 = Getshort[8].ToString();   //284

                    i3vm.Item3Datas[67].g3CH2_1 = Getshort[9].ToString(); //285

                    i2vm.Item2Datas[2].g2CH2_1 = Getshort[10].ToString();  //286
                    i2vm.Item2Datas[3].g2CH2_1 = Getshort[11].ToString();  //287

                    i3vm.Item3Datas[68].g3CH2_1 = Getshort[12].ToString(); //288
                    i3vm.Item3Datas[69].g3CH2_1 = Getshort[13].ToString();
                    i3vm.Item3Datas[70].g3CH2_1 = Getshort[14].ToString(); //290
                    i3vm.Item3Datas[71].g3CH2_1 = Getshort[15].ToString();
                    i3vm.Item3Datas[72].g3CH2_1 = Getshort[16].ToString();
                    i3vm.Item3Datas[73].g3CH2_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[74].g3CH2_1 = Getshort[18].ToString();
                    i3vm.Item3Datas[75].g3CH2_1 = Getshort[19].ToString(); //295
                    i3vm.Item3Datas[76].g3CH2_1 = Getshort[20].ToString();
                    i3vm.Item3Datas[77].g3CH2_1 = Getshort[21].ToString();
                    i3vm.Item3Datas[78].g3CH2_1 = Getshort[22].ToString();
                    i3vm.Item3Datas[79].g3CH2_1 = Getshort[23].ToString();
                    break;
                case 9://308 ~ 339
                    i3vm.Item3Datas[17].g3CH3_1 = Getshort[5].ToString(); //313
                    i3vm.Item3Datas[18].g3CH3_1 = Getshort[6].ToString(); //314
                    i3vm.Item3Datas[10].g3CH3_1 = Getshort[7].ToString();   //315
                    i3vm.Item3Datas[11].g3CH3_1 = Getshort[8].ToString();  //316
                    i3vm.Item3Datas[12].g3CH3_1 = Getshort[9].ToString();  //317
                    i3vm.Item3Datas[13].g3CH3_1 = Getshort[10].ToString(); //318
                    i3vm.Item3Datas[14].g3CH3_1 = Getshort[11].ToString(); //319
                    i3vm.Item3Datas[15].g3CH3_1 = Getshort[12].ToString(); //320
                    i3vm.Item3Datas[18].g3CH3_1 = Getshort[17].ToString(); //325
                    i3vm.Item3Datas[19].g3CH3_1 = GetStr[19].ToString();  //327
                    i3vm.Item3Datas[20].g3CH3_1 = Getshort[20].ToString(); //328
                    i3vm.Item3Datas[21].g3CH3_1 = Getshort[21].ToString(); //329
                    i3vm.Item3Datas[22].g3CH3_1 = Getshort[22].ToString(); //330
                    i3vm.Item3Datas[23].g3CH3_1 = Getshort[23].ToString(); //331
                    i3vm.Item3Datas[24].g3CH3_1 = Getshort[24].ToString(); //332
                    i3vm.Item3Datas[25].g3CH3_1 = Getshort[25].ToString(); //333
                    i3vm.Item3Datas[26].g3CH3_1 = Getshort[26].ToString(); //334
                    i3vm.Item3Datas[27].g3CH3_1 = Getshort[27].ToString(); //335
                    i3vm.Item3Datas[28].g3CH3_1 = Getshort[28].ToString(); //336
                    i3vm.Item3Datas[29].g3CH3_1 = Getdouble[29].ToString("0.0"); //337
                    i3vm.Item3Datas[30].g3CH3_1 = Getdouble[30].ToString("0.0"); //338
                    i3vm.Item3Datas[31].g3CH3_1 = Getdouble[31].ToString("0.0"); //339
                    break;
                case 10://340 ~ 371
                    i3vm.Item3Datas[32].g3CH3_1 = Getdouble[0].ToString("0.0"); //340
                    i3vm.Item3Datas[33].g3CH3_1 = Getshort[1].ToString(); //341
                    i3vm.Item3Datas[34].g3CH3_1 = Getshort[2].ToString(); //342
                    i3vm.Item3Datas[35].g3CH3_1 = Getshort[3].ToString(); //343
                    i3vm.Item3Datas[36].g3CH3_1 = Getshort[4].ToString(); //344
                    i3vm.Item3Datas[37].g3CH3_1 = Getshort[5].ToString(); //345
                    i3vm.Item3Datas[38].g3CH3_1 = Getshort[6].ToString(); //346
                    i3vm.Item3Datas[39].g3CH3_1 = Getdouble[8].ToString("0.0"); //348
                    i3vm.Item3Datas[40].g3CH3_1 = Getdouble[9].ToString("0.0");
                    i3vm.Item3Datas[41].g3CH3_1 = Getdouble[10].ToString("0.0"); //350
                    i3vm.Item3Datas[42].g3CH3_1 = Getshort[11].ToString();
                    i3vm.Item3Datas[43].g3CH3_1 = Getshort[12].ToString();
                    i3vm.Item3Datas[44].g3CH3_1 = Getshort[13].ToString();
                    i3vm.Item3Datas[45].g3CH3_1 = Getdouble[16].ToString("0.0"); //356
                    i3vm.Item3Datas[46].g3CH3_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[47].g3CH3_1 = Getshort[18].ToString();
                    i3vm.Item3Datas[48].g3CH3_1 = Getdouble[19].ToString("0.0");
                    i3vm.Item3Datas[49].g3CH3_1 = Getdouble[20].ToString("0.0"); //360
                    i3vm.Item3Datas[50].g3CH3_1 = Getshort[21].ToString();
                    i3vm.Item3Datas[51].g3CH3_1 = Getshort[22].ToString();
                    i3vm.Item3Datas[52].g3CH3_1 = Getshort[23].ToString();
                    i3vm.Item3Datas[53].g3CH3_1 = Getshort[24].ToString();
                    i3vm.Item3Datas[54].g3CH3_1 = Getshort[25].ToString(); //365

                    i2vm.Item2Datas[4].g2CH3_1 = Getshort[26].ToString();  //366

                    if (i1vm.Item1Datas[0].g1CH3_1 == "2" || i1vm.Item1Datas[0].g1CH3_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH3_1 == "4" || i1vm.Item1Datas[0].g1CH3_1 == "5")
                        i3vm.Item3Datas[55].g3CH3_1 = Getdouble[27].ToString("0.0");
                    else
                        i3vm.Item3Datas[55].g3CH3_1 = Getshort[27].ToString();

                    if (i1vm.Item1Datas[0].g1CH3_1 == "2" || i1vm.Item1Datas[0].g1CH3_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH3_1 == "4" || i1vm.Item1Datas[0].g1CH3_1 == "5")
                        i3vm.Item3Datas[56].g3CH3_1 = Getdouble[28].ToString("0.0");
                    else
                        i3vm.Item3Datas[56].g3CH3_1 = Getshort[28].ToString();

                    i3vm.Item3Datas[57].g3CH3_1 = Getshort[29].ToString(); //369
                    break;
                case 11://372 ~ 403
                    i2vm.Item2Datas[7].g2CH3_1 = Getshort[0].ToString();  //372

                    i3vm.Item3Datas[58].g3CH3_1 = Getshort[1].ToString(); //373
                    i3vm.Item3Datas[59].g3CH3_1 = Getshort[2].ToString();
                    i3vm.Item3Datas[60].g3CH3_1 = Getdouble[3].ToString("0.0"); //375 
                    i3vm.Item3Datas[61].g3CH3_1 = Getshort[4].ToString(); //376
                    i3vm.Item3Datas[62].g3CH3_1 = Getshort[5].ToString();
                    i3vm.Item3Datas[63].g3CH3_1 = Getdouble[6].ToString("0.0");
                    i3vm.Item3Datas[64].g3CH3_1 = Getdouble[7].ToString("0.0"); //379
                    i3vm.Item3Datas[65].g3CH3_1 = Getdouble[8].ToString("0.0"); //380
                    i3vm.Item3Datas[66].g3CH3_1 = Getdouble[9].ToString("0.0");

                    if (i1vm.Item1Datas[0].g1CH3_1 == "2" || i1vm.Item1Datas[0].g1CH3_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH3_1 == "4" || i1vm.Item1Datas[0].g1CH3_1 == "5")
                        i2vm.Item2Datas[0].g2CH3_1 = Getdouble[11].ToString("0.0");  //383
                    else
                        i2vm.Item2Datas[0].g2CH3_1 = Getshort[11].ToString();  //383

                    if (i1vm.Item1Datas[0].g1CH3_1 == "2" || i1vm.Item1Datas[0].g1CH3_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH3_1 == "4" || i1vm.Item1Datas[0].g1CH3_1 == "5")
                        i2vm.Item2Datas[1].g2CH3_1 = Getdouble[12].ToString("0.0");  //384
                    else
                        i2vm.Item2Datas[1].g2CH3_1 = Getshort[12].ToString();  //384


                    i3vm.Item3Datas[67].g3CH3_1 = Getshort[13].ToString(); //385

                    i2vm.Item2Datas[2].g2CH3_1 = Getshort[14].ToString();  //386
                    i2vm.Item2Datas[3].g2CH3_1 = Getshort[15].ToString();  //387

                    i3vm.Item3Datas[68].g3CH3_1 = Getshort[16].ToString(); //388
                    i3vm.Item3Datas[69].g3CH3_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[70].g3CH3_1 = Getshort[18].ToString(); //390
                    i3vm.Item3Datas[71].g3CH3_1 = Getshort[19].ToString();
                    i3vm.Item3Datas[72].g3CH3_1 = Getshort[20].ToString();
                    i3vm.Item3Datas[73].g3CH3_1 = Getshort[21].ToString();
                    i3vm.Item3Datas[74].g3CH3_1 = Getshort[22].ToString();
                    i3vm.Item3Datas[75].g3CH3_1 = Getshort[23].ToString(); //395
                    i3vm.Item3Datas[76].g3CH3_1 = Getshort[24].ToString();
                    i3vm.Item3Datas[77].g3CH3_1 = Getshort[25].ToString();
                    i3vm.Item3Datas[78].g3CH3_1 = Getshort[26].ToString();
                    i3vm.Item3Datas[79].g3CH3_1 = Getshort[27].ToString();
                    break;
                case 12://404 ~ 435
                    i3vm.Item3Datas[17].g3CH4_1 = Getshort[9].ToString();  //413
                    i3vm.Item3Datas[18].g3CH4_1 = Getshort[10].ToString(); //414
                    i3vm.Item3Datas[10].g3CH4_1 = Getshort[11].ToString();  //415
                    i3vm.Item3Datas[11].g3CH4_1 = Getshort[12].ToString(); //416
                    i3vm.Item3Datas[12].g3CH4_1 = Getshort[13].ToString(); //417
                    i3vm.Item3Datas[13].g3CH4_1 = Getshort[14].ToString(); //418
                    i3vm.Item3Datas[14].g3CH4_1 = Getshort[15].ToString(); //419
                    i3vm.Item3Datas[15].g3CH4_1 = Getshort[16].ToString(); //420
                    i3vm.Item3Datas[18].g3CH4_1 = Getshort[21].ToString(); //425
                    i3vm.Item3Datas[19].g3CH4_1 = GetStr[23].ToString();   //427
                    i3vm.Item3Datas[20].g3CH4_1 = Getshort[24].ToString(); //428
                    i3vm.Item3Datas[21].g3CH4_1 = Getshort[25].ToString(); //429
                    i3vm.Item3Datas[22].g3CH4_1 = Getshort[26].ToString(); //430
                    i3vm.Item3Datas[23].g3CH4_1 = Getshort[27].ToString(); //431
                    i3vm.Item3Datas[24].g3CH4_1 = Getshort[28].ToString(); //432
                    i3vm.Item3Datas[25].g3CH4_1 = Getshort[29].ToString(); //433
                    i3vm.Item3Datas[26].g3CH4_1 = Getshort[30].ToString(); //434
                    i3vm.Item3Datas[27].g3CH4_1 = Getshort[31].ToString(); //435
                    break;
                case 13://436 ~ 467
                    i3vm.Item3Datas[28].g3CH4_1 = Getshort[0].ToString(); //436
                    i3vm.Item3Datas[29].g3CH4_1 = Getdouble[1].ToString("0.0"); //437
                    i3vm.Item3Datas[30].g3CH4_1 = Getdouble[2].ToString("0.0"); //438
                    i3vm.Item3Datas[31].g3CH4_1 = Getdouble[3].ToString("0.0"); //439
                    i3vm.Item3Datas[32].g3CH4_1 = Getdouble[4].ToString("0.0"); //440
                    i3vm.Item3Datas[33].g3CH4_1 = Getshort[5].ToString(); //441
                    i3vm.Item3Datas[34].g3CH4_1 = Getshort[6].ToString(); //442
                    i3vm.Item3Datas[35].g3CH4_1 = Getshort[7].ToString(); //443
                    i3vm.Item3Datas[36].g3CH4_1 = Getshort[8].ToString(); //444
                    i3vm.Item3Datas[37].g3CH4_1 = Getshort[9].ToString(); //445
                    i3vm.Item3Datas[38].g3CH4_1 = Getshort[10].ToString(); //446
                    i3vm.Item3Datas[39].g3CH4_1 = Getdouble[12].ToString("0.0"); //448
                    i3vm.Item3Datas[40].g3CH4_1 = Getdouble[13].ToString("0.0");
                    i3vm.Item3Datas[41].g3CH4_1 = Getdouble[14].ToString("0.0"); //450
                    i3vm.Item3Datas[42].g3CH4_1 = Getshort[15].ToString();
                    i3vm.Item3Datas[43].g3CH4_1 = Getshort[16].ToString();
                    i3vm.Item3Datas[44].g3CH4_1 = Getshort[17].ToString();
                    i3vm.Item3Datas[45].g3CH4_1 = Getdouble[20].ToString("0.0"); //456
                    i3vm.Item3Datas[46].g3CH4_1 = Getshort[21].ToString();
                    i3vm.Item3Datas[47].g3CH4_1 = Getshort[22].ToString();
                    i3vm.Item3Datas[48].g3CH4_1 = Getdouble[23].ToString("0.0");
                    i3vm.Item3Datas[49].g3CH4_1 = Getdouble[24].ToString("0.0"); //460
                    i3vm.Item3Datas[50].g3CH4_1 = Getshort[25].ToString();
                    i3vm.Item3Datas[51].g3CH4_1 = Getshort[26].ToString();
                    i3vm.Item3Datas[52].g3CH4_1 = Getshort[27].ToString();
                    i3vm.Item3Datas[53].g3CH4_1 = Getshort[28].ToString();
                    i3vm.Item3Datas[54].g3CH4_1 = Getshort[29].ToString(); //465

                    i2vm.Item2Datas[4].g2CH4_1 = Getshort[30].ToString();  //466

                    if (i1vm.Item1Datas[0].g1CH4_1 == "2" || i1vm.Item1Datas[0].g1CH4_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH4_1 == "4" || i1vm.Item1Datas[0].g1CH4_1 == "5")
                        i3vm.Item3Datas[55].g3CH4_1 = Getdouble[31].ToString("0.0"); //467
                    else
                        i3vm.Item3Datas[55].g3CH4_1 = Getshort[31].ToString(); //467


                    break;
                case 14://468 ~ 499
                    if (i1vm.Item1Datas[0].g1CH4_1 == "2" || i1vm.Item1Datas[0].g1CH4_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH4_1 == "4" || i1vm.Item1Datas[0].g1CH4_1 == "5")
                        i3vm.Item3Datas[56].g3CH4_1 = Getdouble[0].ToString("0.0");
                    else
                        i3vm.Item3Datas[56].g3CH4_1 = Getshort[0].ToString();

                    i3vm.Item3Datas[57].g3CH4_1 = Getshort[1].ToString(); //469

                    i2vm.Item2Datas[7].g2CH4_1 = Getshort[4].ToString();  //472

                    i3vm.Item3Datas[58].g3CH4_1 = Getshort[5].ToString(); //473
                    i3vm.Item3Datas[59].g3CH4_1 = Getshort[6].ToString();
                    i3vm.Item3Datas[60].g3CH4_1 = Getdouble[7].ToString("0.0"); //475 
                    i3vm.Item3Datas[61].g3CH4_1 = Getshort[8].ToString(); //476
                    i3vm.Item3Datas[62].g3CH4_1 = Getshort[9].ToString();
                    i3vm.Item3Datas[63].g3CH4_1 = Getdouble[10].ToString("0.0");
                    i3vm.Item3Datas[64].g3CH4_1 = Getdouble[11].ToString("0.0"); //479
                    i3vm.Item3Datas[65].g3CH4_1 = Getdouble[12].ToString("0.0"); //480
                    i3vm.Item3Datas[66].g3CH4_1 = Getdouble[13].ToString("0.0");

                    if (i1vm.Item1Datas[0].g1CH4_1 == "2" || i1vm.Item1Datas[0].g1CH4_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH4_1 == "4" || i1vm.Item1Datas[0].g1CH4_1 == "5")
                        i2vm.Item2Datas[0].g2CH4_1 = Getdouble[15].ToString("0.0");  //483
                    else
                        i2vm.Item2Datas[0].g2CH4_1 = Getshort[15].ToString();  //483

                    if (i1vm.Item1Datas[0].g1CH4_1 == "2" || i1vm.Item1Datas[0].g1CH4_1 == "3" ||
                        i1vm.Item1Datas[0].g1CH4_1 == "4" || i1vm.Item1Datas[0].g1CH4_1 == "5")
                        i2vm.Item2Datas[1].g2CH4_1 = Getdouble[16].ToString("0.0");  //484
                    else
                        i2vm.Item2Datas[1].g2CH4_1 = Getshort[16].ToString();  //484

                    i3vm.Item3Datas[67].g3CH4_1 = Getshort[17].ToString(); //485

                    i2vm.Item2Datas[2].g2CH4_1 = Getshort[18].ToString();  //486
                    i2vm.Item2Datas[3].g2CH4_1 = Getshort[19].ToString();  //487

                    i3vm.Item3Datas[68].g3CH4_1 = Getshort[20].ToString(); //488
                    i3vm.Item3Datas[69].g3CH4_1 = Getshort[21].ToString();
                    i3vm.Item3Datas[70].g3CH4_1 = Getshort[22].ToString(); //490
                    i3vm.Item3Datas[71].g3CH4_1 = Getshort[23].ToString();
                    i3vm.Item3Datas[72].g3CH4_1 = Getshort[24].ToString();
                    i3vm.Item3Datas[73].g3CH4_1 = Getshort[25].ToString();
                    i3vm.Item3Datas[74].g3CH4_1 = Getshort[26].ToString();
                    i3vm.Item3Datas[75].g3CH4_1 = Getshort[27].ToString(); //495
                    i3vm.Item3Datas[76].g3CH4_1 = Getshort[28].ToString();
                    i3vm.Item3Datas[77].g3CH4_1 = Getshort[29].ToString();
                    i3vm.Item3Datas[78].g3CH4_1 = Getshort[30].ToString();
                    i3vm.Item3Datas[79].g3CH4_1 = Getshort[31].ToString();
                    break;
            }
        }
            /*
             Slave Address         1Byte
             Function              1Byte
             Starting Address   Hi 1Byte
                                Lo 1Byte
             No. of Register    Hi 1Byte
                                Lo 1Byte
             Byte Count            1Byte
             Data               Hi 1Byte
                                Lo 1Byte
             .
             .
             .
             Error Check(CRC16) Hi 1Byte
                                Lo 1Byte
            09 10 00 49 00 05 00 0A 00 01 00 01 00 01 00 01 00 00 7A 04  
             */

        public void reset_send(int ch)
        {
            //Global.gSendMessge.upload_Serial(0x00, 0x48, ch.ToString(), 0);
        }

        public void clearbuffer()
        {
            RecvDataList.Clear();
            sPort.DiscardInBuffer();
            sPort.DiscardOutBuffer();
        }
    }
}
