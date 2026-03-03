using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.View.Component;
using LP_MMTCRack.ViewModel;
using ScottPlot.Statistics;
using Serilog;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

namespace LP_MMTCRack.Net
{
    public class SiteCom
    {
        public int SiteCh { get; set; } = 0;
        public string SitePortName { get; set; } = string.Empty;

        public string UsbHwSitePort { get; set; } = string.Empty;
        public string UsbSiteNameing { get; set; } = string.Empty;
        public string MotorValue { get; set; } = string.Empty;
        public bool SitePortUse { get; set; } = false;
        public string MotorFinalStep { get; set; } = string.Empty;
        public bool MotorPositon { get; set; } = false;
        public bool GetSiteRecv { get; set; } = true;
        public bool IsMotorMove { get; set; } = false;
        public bool IsMtrValueUpdated { get; set; } = false;
        public bool IsGetSiteRecv { get; set; } = false;
        public bool IsMotorDown { get; set; } = false;

        public bool Board_SensingOK { get; set; } = false;
        public bool Board_MotorPowerOn { get; set; } = false;
        public bool isMotorStatus { get; set; } = true;
        public SerialPort SitePort;
        public SiteCom()
        {
            Global.gSiteCom = this;
        }
        /*
        public async void SiteConnect(int ch)
        {
            try
            {

                Global.gMainPageVM.Racks[SiteCh].MotorStatText = string.Empty;// 비우고 다시 받을지
                Global.gMainPageVM.Racks[SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                Global.gEngineerVM.ManualMotor[SiteCh].eMotorStatColor = Global.gMotorStatColors[1];
                foreach (BoxModel bm in Global.gMainPageVM.Racks[SiteCh].Boxes)
                {
                    bm.HumidityValue = 0;
                }
                SitePort = new SerialPort();
                //if (SitePort != null)
                if (SitePort.IsOpen)
                {
                    SitePort.Close();
                    SitePort.Dispose();
                    Thread.Sleep(100);
                }

                SiteCh = ch;

                if (SitePortName == string.Empty)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"SitePort {SiteCh + 1} Com Port Name Emtpy");
                    return;
                }

                var CheckPortStatus = MConv.CheckPortAlive(SitePortName, "USB\\VID_10C4&PID_EA60\\" + UsbHwSitePort);

                if (CheckPortStatus.Item1)
                {
                    var MatchDev = await Task.Run(() => MConv.GetComPortDetails(CheckPortStatus.Item2, UsbSiteNameing, UsbHwSitePort));

                    if (MatchDev.Item1)
                    {
                        if (SitePortName != MatchDev.Item2)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} {SitePortName} -> {MatchDev.Item2} Com Port Chage ");
                            Global.gConfigVM.MotorPortName[ch] = MatchDev.Item2;

                            Global.gConfigVM.saveitems();
                        }
                        
                        SitePort.PortName = SitePortName;
                        SitePort.BaudRate = 38400; // 9600;
                        SitePort.DataBits = 8;
                        SitePort.Parity = Parity.None; // Parity.None;
                        SitePort.Handshake = Handshake.None;
                        SitePort.StopBits = StopBits.One;

                        SitePort.DataReceived += new SerialDataReceivedEventHandler(SiteBoard_DataReceivedHandler);
                        SitePort.Open();
                        lock (sendSiteQueue)
                        {
                            sendSiteQueue.Clear();
                        }
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");
                    }
                    else
                        Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SitePort {SiteCh + 1} Usb Info Mismatch. Check Com Port");
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SitePort {SiteCh + 1} None of the Ports Match");
            }
            catch (Exception e)
            {
                SitePort.Close();
                SitePort.Dispose();
                string msg = "[SitePort" + (SiteCh + 1) + "exception] " + e.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }
        */
        public void SiteConnect(int ch)
        {
            try
            {

                Global.gMainPageVM.Racks[SiteCh].MotorStatText = string.Empty;// 비우고 다시 받을지
                Global.gMainPageVM.Racks[SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                Global.gEngineerVM.ManualMotor[SiteCh].eMotorStatColor = Global.gMotorStatColors[1];
                foreach (BoxModel bm in Global.gMainPageVM.Racks[SiteCh].Boxes)
                {
                    bm.HumidityValue = 0;
                }

                if (SitePort != null)
                {
                    SitePort.Close();
                    SitePort.Dispose();
                    Thread.Sleep(100);
                }

                SiteCh = ch;

                if (SitePortName == string.Empty)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"SitePort {SiteCh + 1} Com Port Name Emtpy");
                    return;
                }

                SitePort = new SerialPort();

                SitePort.PortName = SitePortName;
                SitePort.BaudRate = 38400; // 9600;
                SitePort.DataBits = 8;
                SitePort.Parity = Parity.None; // Parity.None;
                SitePort.Handshake = Handshake.None;
                SitePort.StopBits = StopBits.One;

                

                //SitePort.DataReceived += new SerialDataReceivedEventHandler(SiteBoard_DataReceivedHandler);
                SitePort.Open();
                lock (sendSiteQueue)
                {
                    sendSiteQueue.Clear();
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");

                //var MatchDev = MConv.GetComPortDetails(UsbSiteNameing, UsbHwSitePort);

                //var CheckPortStatus = MConv.CheckPortAlive(SitePortName, "USB\\VID_10C4&PID_EA60\\" + UsbHwSitePort);
                /*
                if (CheckPortStatus.Item1)
                {
                    if (SitePortName != CheckPortStatus.Item2)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} {SitePortName} -> {CheckPortStatus.Item2} Com Port Chage ");
                        Global.gConfigVM.MotorPortName[ch] = CheckPortStatus.Item2;

                        Global.gConfigVM.saveitems();
                    }

                    SitePort.PortName = SitePortName;
                    SitePort.BaudRate = 38400; // 9600;
                    SitePort.DataBits = 8;
                    SitePort.Parity = Parity.None; // Parity.None;
                    SitePort.Handshake = Handshake.None;
                    SitePort.StopBits = StopBits.One;

                    SitePort.DiscardInBuffer();
                    SitePort.DiscardOutBuffer();

                    //SitePort.DataReceived += new SerialDataReceivedEventHandler(SiteBoard_DataReceivedHandler);
                    SitePort.Open();
                    lock (sendSiteQueue)
                    {
                        sendSiteQueue.Clear();
                    }
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SitePort {SiteCh + 1} Usb Info Mismatch. Check Com Port");
                */
                /*
                SitePort.PortName = SitePortName;
                SitePort.BaudRate = 38400; // 9600;
                SitePort.DataBits = 8;
                SitePort.Parity = Parity.None; // Parity.None;
                SitePort.Handshake = Handshake.None;
                SitePort.StopBits = StopBits.One;

                SitePort.DataReceived += new SerialDataReceivedEventHandler(SiteBoard_DataReceivedHandler);
                SitePort.Open();
                lock (sendSiteQueue)
                {
                    sendSiteQueue.Clear();
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");
                */
            }
            catch (Exception e)
            {
                SitePort.Close();
                SitePort.Dispose();
                string msg = "[SitePort" + (SiteCh + 1) + "exception] " + e.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }

        public void ReSiteConnect(int ch)
        {
            try
            {

                //Global.gMainPageVM.Racks[SiteCh].MotorStatText = string.Empty;// 비우고 다시 받을지
                //Global.gMainPageVM.Racks[SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                //Global.gEngineerVM.ManualMotor[SiteCh].eMotorStatColor = Global.gMotorStatColors[1];
                //foreach (BoxModel bm in Global.gMainPageVM.Racks[SiteCh].Boxes)
                //{
                //    bm.HumidityValue = 0;
                //}

                if (SitePort != null)
                {
                    SitePort.Close();
                    SitePort.Dispose();
                    Thread.Sleep(100);
                }

                SiteCh = ch;

                if (SitePortName == string.Empty)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"SitePort {SiteCh + 1} Com Port Name Emtpy");
                    return;
                }

                SitePort = new SerialPort();

                SitePort.PortName = SitePortName;
                SitePort.BaudRate = 38400; // 9600;
                SitePort.DataBits = 8;
                SitePort.Parity = Parity.None; // Parity.None;
                SitePort.Handshake = Handshake.None;
                SitePort.StopBits = StopBits.One;



                //SitePort.DataReceived += new SerialDataReceivedEventHandler(SiteBoard_DataReceivedHandler);
                SitePort.Open();
                lock (sendSiteQueue)
                {
                    sendSiteQueue.Clear();
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match ReConnect");

            }
            catch (Exception e)
            {
                SitePort.Close();
                SitePort.Dispose();
                string msg = "[SitePort" + (SiteCh + 1) + "exception] " + e.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }

        public bool SiteFWReadWrite()
        {
            bool rtnVal = false;
            uint selStienum = 0;
            bool[] chkVal = new bool[2] { false, false };

            uint numDevices;
            int result = MConv.CP210x_GetNumDevices(out numDevices);

            if (result != MConv.CP210x_SUCCESS || numDevices == 0)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드를 찾을수 없음.");
                return false;
            }

            // Read Reg Info
            for (uint i = 0; i < numDevices; i++)
            {
                IntPtr deviceHandle;
                result = MConv.CP210x_Open(i, out deviceHandle); // 첫 번째 장치 열기

                if (result != MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드를 연결 할 수 없음. {i}");
                    return false;
                }

                StringBuilder productString = new StringBuilder(256);
                byte length = 0;

                // CP210x_GetDeviceProductString 호출
                result = MConv.CP210x_GetDeviceProductString(deviceHandle, productString, ref length, true);

                if (result == MConv.CP210x_SUCCESS)
                {
                    if (productString.ToString() == UsbSiteNameing)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"{i} , Site{SiteCh} BD ProductString Match");
                        chkVal[0] = true;
                    }
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 제품 정보를 가져올 수 없음. {i}");
                }

                // 장치 일련번호 가져오기
                StringBuilder serialNumber = new StringBuilder(64);
                result = MConv.CP210x_GetDeviceSerialNumber(deviceHandle, serialNumber, ref length, true);

                if (result == MConv.CP210x_SUCCESS)
                {
                    // 일련번호가 정상적으로 반환되었을 경우
                    if (serialNumber.ToString() == UsbHwSitePort)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"{i} , Site{SiteCh} BD SerialNumber Match");
                        chkVal[1] = true;
                    }
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 일련번호를 가져올 수 없음. {i}");
                }

                // 장치 닫기
                result = MConv.CP210x_Close(deviceHandle);
                if (result != MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 닫을 수 없음. {i}");
                }
                if (chkVal[0] == true && chkVal[1] == true)
                {
                    selStienum = i;
                    break;
                }
            }

            // Write Reg Info
            if (chkVal[0] == true && chkVal[1] == true)
            {
                IntPtr deviceHandle;
                result = MConv.CP210x_Open(selStienum, out deviceHandle); // 첫 번째 장치 열기

                if (result != MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드를 연결 할 수 없음. {selStienum}");
                    return false;
                }

                result = MConv.CP210x_Reset(deviceHandle);

                if (result == MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"Site{SiteCh} BD Chip Reset Done");
                }
                else
                {
                    //MessageBox.Show("장치를 리셋할 수 없습니다.");
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 칩 리셋할 수 없음. {selStienum}");
                }

                // 장치 닫기
                result = MConv.CP210x_Close(deviceHandle);
                if (result != MConv.CP210x_SUCCESS)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"연결된 Site 보드 닫을 수 없음. {selStienum}");
                }
                else
                    rtnVal = true;
            }

            return rtnVal;
        }

        public async void CheckSiteConnect()
        {
            Global.SiteonoffEnable = true;
            try
            {
                //Rack 위치 site Board Close
                SiteDisConnect();

                await Task.Delay(2000);

                //Silicon Lab FW Read/Reset
                SiteFWReadWrite();

                await Task.Delay(2000);

                //Rack 위치 site Board Open
                SiteConnect(SiteCh);
                Global.SiteonoffEnable = false;
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $" CheckSiteConnect 중 예외 발생 : {ex.Message}.");
            }
        }

        public void SiteDisConnect()
        {
            if (SitePort != null)
            {
                try
                {
                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                    rm.MotorStatText = string.Empty;
                    foreach (var bm in rm.Boxes)
                        bm.HumidityValue = 0;

                    if (SitePort.IsOpen)
                    {
                        SitePort.Close();
                    }
                    SitePort.Dispose(); // 완전 해제
                    Console.WriteLine("[INFO] Serial port closed.");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[ERROR] SiteClose failed: {ex.Message}");
                }
            }
            /*
            try
            {
                if (SitePort != null && SitePort.IsOpen)
                {
                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                    rm.MotorStatText = string.Empty;
                    foreach (var bm in rm.Boxes)
                        bm.HumidityValue = 0;

                    SitePort.Close();
                    SitePort.Dispose();
                }
            }
            catch { SitePort.Dispose(); }
            */
        }
        public Queue<byte[]> sendSiteQueue = new Queue<byte[]>();
        public void SiteWirteMsg(string val)
        {
            int rotattion = 0;
            int sendlen = MConv.CheckSendLength(val);
            byte[] txData = new byte[sendlen];

            try
            {
                string str_count = val.Substring(1);
                if (str_count == "")
                {
                    str_count = "0";
                }

                rotattion = Int32.Parse(str_count);
            }
            catch
            {
                rotattion = 0;
            }
            txData = MConv.MakeSiteMeg(val, rotattion, sendlen);

            if (SitePort == null)
                return;
            if (!SitePort.IsOpen)
                return;

            lock (sendSiteQueue)
            {
                sendSiteQueue.Enqueue(txData);
            }

            PreMotorSendMsg();
        }
        public void SiteHeightWriteMsg(int val)
        {
            byte[] txData = new byte[3];

            if (val == 17)
            {
                txData[0] = 0x11;
                txData[1] = 0x11;
                txData[2] = 0x0a;
            }

            if (SitePort == null)
                return;
            if (!SitePort.IsOpen)
                return;

            lock (sendSiteQueue)
            {
                sendSiteQueue.Enqueue(txData);
            }
            PreMotorSendMsg();
        }
        public object site_lock = new object();
        private void PreMotorSendMsg()
        {
            //lock (site_lock)
            //{
            //await MotorSendMessageAsync();
            //}
            lock (site_lock)
            {
                Console.WriteLine($"MotorSendMessage() in");
                MotorSendMessage();
                Console.WriteLine($"MotorSendMessage() out");
            }
        }
        

        private  void MotorSendMessage()
        {
            if (SitePort == null || !SitePort.IsOpen)
                return;

            try
            {
                byte[] siteData = null;
                lock (sendSiteQueue)
                {
                    Console.WriteLine("sendSiteQueue.Count ====>>>> {0}", sendSiteQueue.Count);
                    if (sendSiteQueue.Count == 0)
                        return;

                    siteData = sendSiteQueue.Dequeue();
                }

                Console.WriteLine("Data Write: " + BitConverter.ToString(siteData));
                GetSiteRecv = true;
                SitePort.Write(siteData, 0, siteData.Length);
                IsMtrValueUpdated = false;
                
                /*
                if (Global.gMainPageVM.Racks[SiteCh].WriteMotorDown)
                {
                    if (Global.gMainPageVM.Racks[SiteCh].motorsetval == 0 || Global.gMainPageVM.Racks[SiteCh].motorsetval >= 35000)
                        Thread.Sleep(5000);
                    else
                        Thread.Sleep(1500);
                }
                else
                    Thread.Sleep(300);
                */

                ReadSiteResponse(siteData);

                if (GetSiteRecv)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Write & Read Com Fail");
                }
            }
            catch (Exception ex)
            {
                //string msg = "Motor Write Fail, Board" + (SiteCh + 1) + " " + ex.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Motor Write Fail, Board" + (SiteCh + 1) + " " + ex.Message);
            }
            finally
            {
                // 작업 완료 후 항상 플래그 초기화
                GetSiteRecv = false;
            }
        }

        private void ReadSiteResponse(byte[] siteData)
        {
            try
            {
                List<byte> recvBuffer = new List<byte>();
                bool endDetected = false;
                DateTime startTime = DateTime.Now;

                while ((DateTime.Now - startTime).TotalMilliseconds < 7000) // 7000ms 타임아웃
                {
                    int inputByteLength = SitePort.BytesToRead;

                    if (inputByteLength > 0)
                    {
                        byte[] tempBuffer = new byte[inputByteLength];
                        SitePort.Read(tempBuffer, 0, inputByteLength);
                        recvBuffer.AddRange(tempBuffer);

                        // 데이터 종단(0x0A)을 찾음
                        if (recvBuffer.Contains(0x0A))
                        {
                            endDetected = true;
                            break;
                        }
                    }

                    Thread.Sleep(10); // 잠시 대기 후 다시 읽기
                }

                if (!endDetected)
                {
                    // 타임아웃 처리
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Data reception timeout: No 0x0A detected");
                    SitePort.DiscardInBuffer();
                    SitePort.DiscardOutBuffer();
                    return;
                }

                // recvBuffer에서 첫 번째 데이터와 0x0A로 끝나는 패킷을 처리
                if (recvBuffer.Count >= 4 && recvBuffer[0] == siteData[0]) // 데이터 시작 검증
                {
                    // 유효한 데이터 패킷 처리
                    byte[] recvData = recvBuffer.ToArray();
                    Console.WriteLine("Data Received: " + BitConverter.ToString(recvData));
                    HandleReceivedData(recvData);
                    // 받은 데이터를 처리 후 recvBuffer 초기화
                    recvBuffer.Clear();  // 필요한 데이터만 남기고 나머지 초기화
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Invalid data start");
                    byte[] ErrrecvData = recvBuffer.ToArray();
                    Console.WriteLine($"!!!!! ERR Data Received   siteData[0]{BitConverter.ToString(siteData)}   :  {BitConverter.ToString(ErrrecvData)}");
                    //SitePort.DiscardInBuffer();
                    //SitePort.DiscardOutBuffer();
                    recvBuffer.Clear(); // 버퍼 초기화
                }
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Timeout: " + tex.Message);
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] IO Error: " + ioex.Message);
                ReSiteConnect(SiteCh);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] General Error: " + ex.Message);
                ReSiteConnect(SiteCh);
            }
        }



        /*
        private void ReadSiteResponse(byte[] siteData)
        {
            try
            {
                List<byte> recvBuffer = new List<byte>();
                bool endDetected = false;
                DateTime startTime = DateTime.Now;

                while ((DateTime.Now - startTime).TotalMilliseconds < 7000) // 5000ms 타임아웃
                {
                    int inputByteLength = SitePort.BytesToRead;

                    if (inputByteLength > 0)
                    {
                        byte[] tempBuffer = new byte[inputByteLength];
                        SitePort.Read(tempBuffer, 0, inputByteLength);
                        recvBuffer.AddRange(tempBuffer);

                        // 데이터 종단(0x0A)을 찾음
                        if (recvBuffer.Contains(0x0A))
                        {
                            endDetected = true;
                            break;
                        }
                    }

                    Thread.Sleep(10); // 잠시 대기 후 다시 읽기
                }

                if (!endDetected)
                {
                    // 타임아웃 처리
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Data reception timeout: No 0x0A detected");
                    SitePort.DiscardInBuffer();
                    SitePort.DiscardOutBuffer();
                    return;
                }

                // recvBuffer에서 첫 번째 데이터와 0x0A로 끝나는 패킷을 처리
                if (recvBuffer.Count > 4 && recvBuffer[0] == siteData[0]) // 데이터 시작 검증
                {
                    // 유효한 데이터 패킷 처리
                    byte[] recvData = recvBuffer.ToArray();
                    Console.WriteLine("Data Received: " + BitConverter.ToString(recvData));
                    HandleReceivedData(recvData);
                    //GetSiteRecv = false; // 정상적으로 처리 완료
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Invalid data start");
                    byte[] ErrrecvData = recvBuffer.ToArray();
                    Console.WriteLine("!!!!! ERR Data Received: " + BitConverter.ToString(ErrrecvData));
                    SitePort.DiscardInBuffer();
                    SitePort.DiscardOutBuffer();
                }
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Timeout: " + tex.Message);
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] IO Error: " + ioex.Message);
                SiteConnect(SiteCh);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] General Error: " + ex.Message);
            }
        }
        */
        private async Task MotorSendMessageAsync()
        {
            if (SitePort == null || !SitePort.IsOpen)
                return;

            try
            {
                byte[] siteData = null;
                lock (sendSiteQueue)
                {
                    Console.WriteLine("sendSiteQueue.Count ====>>>> {0}", sendSiteQueue.Count);
                    if (sendSiteQueue.Count == 0)
                        return;

                    siteData = sendSiteQueue.Dequeue();
                }

                GetSiteRecv = true;
                await SitePort.BaseStream.WriteAsync(siteData, 0, siteData.Length);
                IsMtrValueUpdated = false;

                int retryCount = 0;
                int timeout = 10000; // 10 seconds timeout
                while (retryCount * 100 < timeout)
                {
                    if (!GetSiteRecv)
                    {
                        Console.WriteLine("Recv OK");
                        break;
                    }
                    await Task.Delay(100);
                    retryCount++;
                }

                if (GetSiteRecv)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Write & Read Com Fail");
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Motor Write Fail, Board" + (SiteCh + 1) + " " + ex.Message);
            }
            finally
            {
                GetSiteRecv = false;
            }
        }

        private void HandleReceivedData(byte[] data)
        {
            // 데이터 처리 로직
            if (data.Length < 4)
                return;

            byte cmdType = data[0]; // 두 번째 바이트로 명령 처리
            switch (cmdType)
            {
                case 0x49:
                    //Motor Up,Down Sensor , Slide Open/Close Sensor
                    SensorCheck(data);
                    break;
                case 0x56:
                    //FW Ver
                    FWVerionCheck(data);
                    break;
                case 0x41:
                    //Humidity 
                    HumidityCheck(data);
                    break;
                case 0x51:
                case 0x52:
                    //Motor Interlock On/Off
                    MotorInterlockCheck(data);
                    break;
                case 0x48:
                case 0x4c:
                    //Motor Moving Rotation
                    MotorRoationCheck(data);
                    break;
                case 0x11:
                    //Motor Rotaion Count
                    MotorRotationCntCheck(data);
                    break;
                case 0x31:
                case 0x33:
                    //pressure sensor
                    PressureSensorCheck(data);
                    break;
                case 0x32:
                    //Motor Power On
                    MotorPoweronCheck(data);
                    break;
            }
        }


        void SensorCheck(byte[] getBuf)
        {
            RackModel rm = Global.gMainPageVM.getRack(SiteCh);
            if (rm == null) return;

            if ((getBuf[1] & 0x01) == 0x01)
            {
                IsMotorDown = false;

                rm.MotorStatText = "OPEN";
            }
            else if ((~getBuf[1] & 0x01) == 0x01)
            {
                IsMotorDown = false;

                rm.MotorStatText = "CLOSE";
            }

            if ((getBuf[1] & 0x02) == 0x02) //Slide Close
            {
                rm.GetSlideOpen(false);
            }
            else if ((~getBuf[1] & 0x02) == 0x02) // Slide Open
            {
                rm.GetSlideOpen(true);
            }

            GetSiteRecv = false;
        }

        void FWVerionCheck(byte[] getBuf)
        {
            RackModel rm = Global.gMainPageVM.getRack(SiteCh);
            Global.gEngineerVM.ManualMotor[rm.RackIndex].GetSiteVer = getBuf[1].ToString();
            GetSiteRecv = false;
        }

        int[] humidity = new int[3];
        int[] temperature = new int[3];
        void HumidityCheck(byte[] getBuf)
        {
            RackModel rm = Global.gMainPageVM.getRack(SiteCh);
            if (rm == null) return;

            if (getBuf.Length < 9) return;

            if (getBuf[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
            {
                temperature[0] = getBuf[5];
                temperature[1] = getBuf[6];
                temperature[2] = getBuf[7];

                if (Global.gMainPageVM.AllBoxCnt == 3)
                {
                    humidity[0] = getBuf[2];
                    humidity[1] = getBuf[4];
                    humidity[2] = getBuf[3];

                    if ((temperature[0] & 0x80) > 0)
                        temperature[0] = -temperature[0];
                    if ((temperature[1] & 0x80) > 0)
                        temperature[1] = -temperature[2];
                    if ((temperature[2] & 0x80) > 0)
                        temperature[2] = -temperature[1];

                }
                else if (Global.gMainPageVM.AllBoxCnt == 2)
                {
                    humidity[0] = getBuf[2];
                    humidity[1] = getBuf[4];

                    if ((temperature[0] & 0x80) > 0)
                        temperature[0] = -temperature[0];
                    if ((temperature[1] & 0x80) > 0)
                        temperature[1] = -temperature[2];
                }

                for (int ch = 0; ch < rm.Boxes.Count; ch++)
                {
                    rm.Boxes[ch].SetHumidityValue(humidity[ch]);
                    rm.Boxes[ch].Hum_TempSenorVal = temperature[ch];
                }
            }
            else
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[S ({SiteCh + 1}] Site B/D Mismatching Address Number");

            GetSiteRecv = false;
        }

        void MotorInterlockCheck(byte[] getBuf)
        {
            if (getBuf[0] == 0x51) // Interlock on
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1}] InterLock Open ");
                Global.gEngineerVM.addMotorLog($"[S{SiteCh + 1}] InterLock Open ");
                Board_MotorPowerOn = true;
            }
            else if (getBuf[0] == 0x52) // Interlock off
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1} InterLock Close ");
                Global.gEngineerVM.addMotorLog($"[S{SiteCh + 1}] InterLock Close ");
            }
            GetSiteRecv = false;
        }

        void MotorRoationCheck(byte[] getBuf)
        {
            RackModel rm = Global.gMainPageVM.getRack(SiteCh);
            if (rm == null) return;

            if (getBuf[0] == 0x48)
            {
                IsMotorDown = false;
                rm.MotorStatText = "OPEN";
                //rotattion = (getBuf[1] << 8) & (getBuf[2]);

            }
            else if (getBuf[0] == 0x4c)
            {
                IsMotorDown = true;
                rm.MotorStatText = "CLOSE";
                //rotattion = (getBuf[1] << 8) & (getBuf[2]);

            }
            GetSiteRecv = false;
        }

        void MotorRotationCntCheck(byte[] getBuf)
        {
            if (!IsMtrValueUpdated)
            {
                UInt16 repeat_count = (UInt16)((getBuf[1] << 8) | (getBuf[2]));
                string msg = Convert.ToString(repeat_count, 10);

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1}] Rx : {repeat_count}");
                Global.gEngineerVM.addMotorLog($"[S{SiteCh + 1}] Rx : {repeat_count}");
                IsMtrValueUpdated = true;
            }
            GetSiteRecv = false;
        }

        void PressureSensorCheck(byte[] getBuf)
        {
            if (getBuf[1] == Convert.ToByte((SiteCh + 1).ToString("X")))
            {
                int sense1 = (getBuf[2] & 0xF0) >> 4;
                int sense2 = (getBuf[2] & 0x0F);
                int sense3 = (getBuf[3] & 0xF0) >> 4;

                string msg = string.Empty;
                if (Global.gMainPageVM.AllBoxCnt == 3)
                {
                    msg = "[S" + (SiteCh + 1).ToString() + "] ";
                    if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                    if (sense2 == 1) { msg += "C(ON) "; } else { msg += "C(OFF) "; }
                    if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }
                }
                else if (Global.gMainPageVM.AllBoxCnt == 2)
                {
                    msg = "[S" + (SiteCh + 1).ToString() + "] ";
                    if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                    if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                Global.gEngineerVM.addMotorLog(msg);

                if (Global.gMainPageVM.AllBoxCnt == 3)
                {
                    if ((sense1 == 1) && (sense2 == 1) && (sense3 == 1))
                    {
                        Board_SensingOK = true;
                    }
                }
                else if (Global.gMainPageVM.AllBoxCnt == 2)
                {
                    /*//1231 Org
                    if((sense1 == 1) && (sense3 == 1))
                    {
                        Board_SensingOK = true;
                    }
                    */
                    if (sense3 == 1)
                        Board_SensingOK = true;
                }
            }
            else
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[S ({SiteCh + 1}] Site B/D Mismatching Address Number");

            GetSiteRecv = false;
        }

        void MotorPoweronCheck(byte[] getBuf)
        {
            if (getBuf[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1}] MotorPower is On");
                Global.gEngineerVM.addMotorLog($"[S{SiteCh + 1}] MotorPower is On");
                Board_MotorPowerOn = true;
            }
            else
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[S ({SiteCh + 1}] Site B/D Mismatching Address Number");

            GetSiteRecv = false;
        }

        private void ReadSiteResponse()
        {
            int rotattion = 0;
            int boardnum = SiteCh;
            
            try
            {
                /*
                int inputByteLength = SitePort.BytesToRead;
                byte[] recvData = new byte[inputByteLength];

                if (rxByteCnt >= 18)
                {
                    rxByteCnt = 0;
                    Array.Clear(recvResult, 0x0, recvResult.Length);
                }

                SitePort.Read(recvData, 0, inputByteLength);
                */
                int inputByteLength = SitePort.BytesToRead;
                byte[] recvData = new byte[inputByteLength];
                SitePort.Read(recvData, 0, inputByteLength);

                //if (inputByteLength < 19)
                //{
                //for (int i = 0; i <= inputByteLength - 1; i++)

                Console.WriteLine("Data Received: " + BitConverter.ToString(recvData));
                for (int i = 0; i < inputByteLength; i++)
                {
                    /*
                        if ((recvData[i] != 0x00) || (recvResult[0] + recvResult[1] + recvResult[2] >= 0x100))
                        {
                            recvResult[rxByteCnt] = recvData[i];
                            rxByteCnt++;
                        }
                        if ((recvData[0] == 0x41) && (i < 8))
                        {
                            continue;
                        }
                        if (recvData[i] == 0x0a)
                        */
                    if (recvData[i] != 0x00 || (recvResult[0] + recvResult[1] + recvResult[2] >= 0x100))
                    {
                        recvResult[rxByteCnt] = recvData[i];
                        rxByteCnt++;
                    }

                    if (recvData[i] == 0x0a)
                    {
                            if (recvData[0] == 0x11)
                            {
                                if (!IsMtrValueUpdated)
                                {


                                    string msg = "";
                                    UInt16 repeat_count = 0;

                                    /*
                                    for (int k = 0; k <= i; k++)
                                    {
                                        msg = msg + " " + recvData[k].ToString("X2");
                                    }
                                    
                                    Console.WriteLine(msg);
                                    */

                                    repeat_count = (UInt16)((recvData[1] << 8) | (recvData[2]));
                                    msg = Convert.ToString(repeat_count, 10);

                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Rx : " + repeat_count.ToString());
                                    Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] Rx : " + repeat_count.ToString());
                                    GetSiteRecv = false;
                                    IsMtrValueUpdated = true;
                                }
                            }

                            if (recvData[0] != 0x0a && recvData[1] != 0x0a)
                            {
                                if (recvData[1] != recvResult[1])
                                {
                                    for (int j = 4; j >= 2; j--)
                                    {
                                        recvResult[j] = recvResult[j - 1];
                                        if (j == 2)
                                        {
                                            recvResult[1] = 0x00;
                                        }
                                    }
                                }
                                if (recvResult[0] == 0x51) // "P" InterLock Open
                                {
                                    GetSiteRecv = false;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, " [S" + (SiteCh + 1) + "] InterLock Open ");
                                    Global.gEngineerVM.addMotorLog(" [S" + (SiteCh + 1) + "] InterLock Open ");
                                    Board_MotorPowerOn = true;
                                }
                                else if (recvResult[0] == 0x52) // "R"  InterLock Close
                                {
                                    GetSiteRecv = false;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, " [S" + (SiteCh + 1) + "] InterLock Close");
                                    Global.gEngineerVM.addMotorLog(" [S" + (SiteCh + 1) + "] InterLock Close");
                                }

                                if (recvResult[0] == 0x48) //모터상승
                                {
                                    GetSiteRecv = false;
                                    IsMotorDown = false;

                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    if (rm == null) return;

                                    rm.MotorStatText = "OPEN";
                                    //rm.MotorStatColor = Global.gMotorStatColors[1];

                                    //회전수
                                    rotattion = (recvResult[1] << 8) & (recvResult[2]);
                                    //Global.gMain.Txt_display[boardnum].Text = rotattion.ToString();
                                }
                                else if (recvResult[0] == 0x4c) //모터하강
                                {
                                    GetSiteRecv = false;
                                    IsMotorDown = true;

                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    if (rm == null) return;

                                    rm.MotorStatText = "CLOSE";
                                    //rm.MotorStatColor = Global.gMotorStatColors[0];

                                    //회전수
                                    rotattion = (recvResult[1] << 8) & (recvResult[2]);
                                    //Global.gMain.Txt_display[boardnum].Text = rotattion.ToString();
                                }
                                else if (recvResult[0] == 0x49) //Sense Info
                                {
                                    if ((recvResult[1] & 0x01) == 0x01) //상승
                                    {
                                        GetSiteRecv = false;
                                        //Console.WriteLine("Sense Info Data Open in");
                                        IsMotorDown = false;

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.MotorStatText = "OPEN";
                                        //rm.MotorStatColor = Global.gMotorStatColors[1];
                                    }
                                    if (((~recvResult[1]) & 0x01) == 0x01) //하강
                                    {
                                        GetSiteRecv = false;
                                        //Console.WriteLine("Sense Info Data Close in");
                                        IsMotorDown = false;

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.MotorStatText = "CLOSE";
                                        //rm.MotorStatColor = Global.gMotorStatColors[0];
                                    }
                                    if ((~recvResult[1] & 0x02) == 0x02) // Slide Open status
                                    {
                                        GetSiteRecv = false;
                                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Slide Open");
                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.GetSlideOpen(true);
                                    }
                                    else if ((recvResult[1] & 0x02) == 0x02) //Slide Close Status
                                    {
                                        GetSiteRecv = false;
                                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Slide Close");
                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.GetSlideOpen(false);
                                    }
                                }
                                else if (recvResult[0] == 0x56) // FW Version
                                {
                                    GetSiteRecv = false;
                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    Global.gEngineerVM.ManualMotor[rm.RackIndex].GetSiteVer = recvResult[1].ToString();
                                    //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] SiteBoard FW Version = " + recvResult[1].ToString());
                                    //Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] SiteBoard FW Version = " + recvResult[1].ToString());
                                }

                                if ((recvResult[0] == 0x31) || (recvResult[0] == 0x33)) //MCU Statue - InterLock On
                                {
                                    // 230526 Siteboard 적용건으로 인한 수정
                                    //1번 바이트 - Board Address
                                    if (recvResult[1] == Convert.ToByte((SiteCh + 1).ToString("X")))
                                    {
                                        //2번, 3번 Sense
                                        int sense1 = (recvResult[2] & 0xF0) >> 4;
                                        int sense2 = (recvResult[2] & 0x0F);
                                        int sense3 = (recvResult[3] & 0xF0) >> 4;

                                        //L R 위치 변경
                                        //YHS 20231031
                                        string msg = string.Empty;
                                        if (Global.gMainPageVM.AllBoxCnt == 3)
                                        {
                                            msg = "[S" + (SiteCh + 1).ToString() + "] ";
                                            if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                                            if (sense2 == 1) { msg += "C(ON) "; } else { msg += "C(OFF) "; }
                                            if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }
                                        }
                                        else if(Global.gMainPageVM.AllBoxCnt == 2)
                                        {
                                            msg = "[S" + (SiteCh + 1).ToString() + "] ";
                                            if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                                            if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }
                                        }
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                                        Global.gEngineerVM.addMotorLog(msg);
                                        //test
                                        //if ((sense2 == 1))
                                        //{
                                        //    Board_SensingOK = true;
                                        //}

                                        //org
                                        if (Global.gMainPageVM.AllBoxCnt == 3)
                                        {
                                            if ((sense1 == 1) && (sense2 == 1) && (sense3 == 1))
                                            {
                                                Board_SensingOK = true;
                                            }
                                        }
                                        else if(Global.gMainPageVM.AllBoxCnt == 2)
                                        {
                                            /*
                                            if((sense1 == 1) && (sense3 == 1))
                                            {
                                                Board_SensingOK = true;
                                            }
                                            */
                                            if (sense3 == 1)
                                                Board_SensingOK = true;
                                        }
                                        GetSiteRecv = false;
                                    }
                                    else
                                    {
                                        string msg = "[S" + (SiteCh + 1) + "] Interlock On Cmd SiteBoard Address Error.";
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                                    }
                                }
                                else if (recvResult[0] == 0x32) //MCU Motor Reponse - Interlock Off
                                {
                                    // 230524 사이트보드 적용 수정 
                                    if (recvResult[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
                                    {
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] MotorPower is On");
                                        Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] MotorPower is On");
                                        Board_MotorPowerOn = true;
                                        GetSiteRecv = false;
                                    }
                                }
                                else if (recvResult[0] == 0x41) //습도체크 humidity check
                                {
                                    // 230524 사이트보드 적용 수정 
                                    if (recvResult[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
                                    {
                                        //boardnum = 1;
                                        //Console.WriteLine("humidity Data in");
                                        int[] humidity = new int[3];
                                        humidity[0] = recvResult[2];
                                        humidity[1] = recvResult[3];
                                        humidity[2] = recvResult[4];

                                        int[] temperature = new int[3];
                                        temperature[0] = recvResult[5];
                                        temperature[1] = recvResult[6];
                                        temperature[2] = recvResult[7];

                                        if (Global.gMainPageVM.AllBoxCnt == 3)
                                        {
                                            humidity[0] = recvResult[2];
                                            humidity[1] = recvResult[3];
                                            humidity[2] = recvResult[4];

                                            if ((temperature[0] & 0x80) > 0)
                                                temperature[0] = -temperature[0];
                                            if ((temperature[1] & 0x80) > 0)
                                                temperature[1] = -temperature[1];
                                            if ((temperature[2] & 0x80) > 0)
                                                temperature[2] = -temperature[2];
                                        }
                                        else if(Global.gMainPageVM.AllBoxCnt == 2)
                                        {
                                            humidity[0] = recvResult[2];
                                            humidity[1] = recvResult[4];

                                            if ((temperature[0] & 0x80) > 0)
                                                temperature[0] = -temperature[0];
                                            if ((temperature[1] & 0x80) > 0)
                                                temperature[1] = -temperature[2];
                                        }

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        for (int ch = 0; ch < rm.Boxes.Count; ch++)
                                        {
                                            rm.Boxes[ch].SetHumidityValue (humidity[ch]);
                                            rm.Boxes[ch].Hum_TempSenorVal = temperature[ch];
                                        }

                                        GetSiteRecv = false;
                                    }
                                }
                            }
                            rxByteCnt = 0;

                            Array.Clear(recvResult, 0x0, recvResult.Length);
                        }
                    }
                //}
                //else
                //{
                //    rxByteCnt = 0;
                //    Array.Clear(recvResult, 0x0, recvResult.Length);
                //}
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Timeout: " + tex.Message);
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] IO Error: " + ioex.Message);
                //SiteConnect(SiteCh);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] General Error: " + ex.Message);
            }
        }
        public byte[] recvResult = new byte[20];
        private int rxByteCnt = 0;
        public void SiteBoard_DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            //Thread.Sleep(50);
            int rotattion = 0;
            int boardnum = SiteCh;
            
            try
            {
                /*
                int inputByteLength = SitePort.BytesToRead;
                byte[] recvData = new byte[inputByteLength];

                if (rxByteCnt >= 18)
                {
                    rxByteCnt = 0;
                    Array.Clear(recvResult, 0x0, recvResult.Length);
                }

                SitePort.Read(recvData, 0, inputByteLength);
                */
                int inputByteLength = SitePort.BytesToRead;
                byte[] recvData = new byte[inputByteLength];
                SitePort.Read(recvData, 0, inputByteLength);

                //if (inputByteLength < 19)
                //{
                //for (int i = 0; i <= inputByteLength - 1; i++)
                for (int i = 0; i < inputByteLength; i++)
                {
                    /*
                        if ((recvData[i] != 0x00) || (recvResult[0] + recvResult[1] + recvResult[2] >= 0x100))
                        {
                            recvResult[rxByteCnt] = recvData[i];
                            rxByteCnt++;
                        }
                        if ((recvData[0] == 0x41) && (i < 8))
                        {
                            continue;
                        }
                        if (recvData[i] == 0x0a)
                        */
                    if (recvData[i] != 0x00 || (recvResult[0] + recvResult[1] + recvResult[2] >= 0x100))
                    {
                        recvResult[rxByteCnt] = recvData[i];
                        rxByteCnt++;
                    }

                    if (recvData[i] == 0x0a)
                    {
                            if (recvData[0] == 0x11)
                            {
                                if (!IsMtrValueUpdated)
                                {


                                    string msg = "";
                                    UInt16 repeat_count = 0;

                                    /*
                                    for (int k = 0; k <= i; k++)
                                    {
                                        msg = msg + " " + recvData[k].ToString("X2");
                                    }
                                    
                                    Console.WriteLine(msg);
                                    */

                                    repeat_count = (UInt16)((recvData[1] << 8) | (recvData[2]));
                                    msg = Convert.ToString(repeat_count, 10);

                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Rx : " + repeat_count.ToString());
                                    Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] Rx : " + repeat_count.ToString());
                                    GetSiteRecv = false;
                                    IsMtrValueUpdated = true;
                                }
                            }

                            if (recvData[0] != 0x0a && recvData[1] != 0x0a)
                            {
                                if (recvData[1] != recvResult[1])
                                {
                                    for (int j = 4; j >= 2; j--)
                                    {
                                        recvResult[j] = recvResult[j - 1];
                                        if (j == 2)
                                        {
                                            recvResult[1] = 0x00;
                                        }
                                    }
                                }
                                if (recvResult[0] == 0x51) // "P" InterLock Open
                                {
                                    GetSiteRecv = false;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, " [S" + (SiteCh + 1) + "] InterLock Open ");
                                    Global.gEngineerVM.addMotorLog(" [S" + (SiteCh + 1) + "] InterLock Open ");
                                    Board_MotorPowerOn = true;
                                }
                                else if (recvResult[0] == 0x52) // "R"  InterLock Close
                                {
                                    GetSiteRecv = false;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, " [S" + (SiteCh + 1) + "] InterLock Close");
                                    Global.gEngineerVM.addMotorLog(" [S" + (SiteCh + 1) + "] InterLock Close");
                                }

                                if (recvResult[0] == 0x48) //모터상승
                                {
                                    GetSiteRecv = false;
                                    IsMotorDown = false;

                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    if (rm == null) return;

                                    rm.MotorStatText = "OPEN";
                                    //rm.MotorStatColor = Global.gMotorStatColors[1];

                                    //회전수
                                    rotattion = (recvResult[1] << 8) & (recvResult[2]);
                                    //Global.gMain.Txt_display[boardnum].Text = rotattion.ToString();
                                }
                                else if (recvResult[0] == 0x4c) //모터하강
                                {
                                    GetSiteRecv = false;
                                    IsMotorDown = true;

                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    if (rm == null) return;

                                    rm.MotorStatText = "CLOSE";
                                    //rm.MotorStatColor = Global.gMotorStatColors[0];

                                    //회전수
                                    rotattion = (recvResult[1] << 8) & (recvResult[2]);
                                    //Global.gMain.Txt_display[boardnum].Text = rotattion.ToString();
                                }
                                else if (recvResult[0] == 0x49) //Sense Info
                                {
                                    if ((recvResult[1] & 0x01) == 0x01) //상승
                                    {
                                        GetSiteRecv = false;
                                        //Console.WriteLine("Sense Info Data Open in");
                                        IsMotorDown = false;

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.MotorStatText = "OPEN";
                                        //rm.MotorStatColor = Global.gMotorStatColors[1];
                                    }
                                    if (((~recvResult[1]) & 0x01) == 0x01) //하강
                                    {
                                        GetSiteRecv = false;
                                        //Console.WriteLine("Sense Info Data Close in");
                                        IsMotorDown = false;

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.MotorStatText = "CLOSE";
                                        //rm.MotorStatColor = Global.gMotorStatColors[0];
                                    }
                                    if ((~recvResult[1] & 0x02) == 0x02) // Slide Open status
                                    {
                                        GetSiteRecv = false;
                                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Slide Open");
                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.GetSlideOpen(true);
                                    }
                                    else if ((recvResult[1] & 0x02) == 0x02) //Slide Close Status
                                    {
                                        GetSiteRecv = false;
                                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Slide Close");
                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.GetSlideOpen(false);
                                    }
                                }
                                else if (recvResult[0] == 0x56) // FW Version
                                {
                                    GetSiteRecv = false;
                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    Global.gEngineerVM.ManualMotor[rm.RackIndex].GetSiteVer = recvResult[1].ToString();
                                    //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] SiteBoard FW Version = " + recvResult[1].ToString());
                                    //Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] SiteBoard FW Version = " + recvResult[1].ToString());
                                }

                                if ((recvResult[0] == 0x31) || (recvResult[0] == 0x33)) //MCU Statue - InterLock On
                                {
                                    // 230526 Siteboard 적용건으로 인한 수정
                                    //1번 바이트 - Board Address
                                    if (recvResult[1] == Convert.ToByte((SiteCh + 1).ToString("X")))
                                    {
                                        //2번, 3번 Sense
                                        int sense1 = (recvResult[2] & 0xF0) >> 4;
                                        int sense2 = (recvResult[2] & 0x0F);
                                        int sense3 = (recvResult[3] & 0xF0) >> 4;

                                        //L R 위치 변경
                                        //YHS 20231031
                                        string msg = string.Empty;
                                        if (Global.gMainPageVM.AllBoxCnt == 3)
                                        {
                                            msg = "[S" + (SiteCh + 1).ToString() + "] ";
                                            if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                                            if (sense2 == 1) { msg += "C(ON) "; } else { msg += "C(OFF) "; }
                                            if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }
                                        }
                                        else if(Global.gMainPageVM.AllBoxCnt == 2)
                                        {
                                            msg = "[S" + (SiteCh + 1).ToString() + "] ";
                                            if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                                            if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }
                                        }
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                                        Global.gEngineerVM.addMotorLog(msg);
                                        //test
                                        //if ((sense2 == 1))
                                        //{
                                        //    Board_SensingOK = true;
                                        //}

                                        //org
                                        if (Global.gMainPageVM.AllBoxCnt == 3)
                                        {
                                            if ((sense1 == 1) && (sense2 == 1) && (sense3 == 1))
                                            {
                                                Board_SensingOK = true;
                                            }
                                        }
                                        else if(Global.gMainPageVM.AllBoxCnt == 2)
                                        {
                                            /*
                                            if((sense1 == 1) && (sense3 == 1))
                                            {
                                                Board_SensingOK = true;
                                            }
                                            */
                                            if (sense3 == 1)
                                                Board_SensingOK = true;
                                        }
                                        GetSiteRecv = false;
                                    }
                                    else
                                    {
                                        string msg = "[S" + (SiteCh + 1) + "] Interlock On Cmd SiteBoard Address Error.";
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                                    }
                                }
                                else if (recvResult[0] == 0x32) //MCU Motor Reponse - Interlock Off
                                {
                                    // 230524 사이트보드 적용 수정 
                                    if (recvResult[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
                                    {
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] MotorPower is On");
                                        Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] MotorPower is On");
                                        Board_MotorPowerOn = true;
                                        GetSiteRecv = false;
                                    }
                                }
                                else if (recvResult[0] == 0x41) //습도체크 humidity check
                                {
                                    // 230524 사이트보드 적용 수정 
                                    if (recvResult[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
                                    {
                                        //boardnum = 1;
                                        //Console.WriteLine("humidity Data in");
                                        int[] humidity = new int[3];
                                        humidity[0] = recvResult[2];
                                        humidity[1] = recvResult[3];
                                        humidity[2] = recvResult[4];

                                        int[] temperature = new int[3];
                                        temperature[0] = recvResult[5];
                                        temperature[1] = recvResult[6];
                                        temperature[2] = recvResult[7];

                                        if (Global.gMainPageVM.AllBoxCnt == 3)
                                        {
                                            humidity[0] = recvResult[2];
                                            humidity[1] = recvResult[3];
                                            humidity[2] = recvResult[4];

                                            if ((temperature[0] & 0x80) > 0)
                                                temperature[0] = -temperature[0];
                                            if ((temperature[1] & 0x80) > 0)
                                                temperature[1] = -temperature[1];
                                            if ((temperature[2] & 0x80) > 0)
                                                temperature[2] = -temperature[2];
                                        }
                                        else if(Global.gMainPageVM.AllBoxCnt == 2)
                                        {
                                            humidity[0] = recvResult[2];
                                            humidity[1] = recvResult[4];

                                            if ((temperature[0] & 0x80) > 0)
                                                temperature[0] = -temperature[0];
                                            if ((temperature[1] & 0x80) > 0)
                                                temperature[1] = -temperature[2];
                                        }

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        for (int ch = 0; ch < rm.Boxes.Count; ch++)
                                        {
                                            rm.Boxes[ch].SetHumidityValue (humidity[ch]);
                                            rm.Boxes[ch].Hum_TempSenorVal = temperature[ch];
                                        }

                                        GetSiteRecv = false;
                                    }
                                }
                            }
                            rxByteCnt = 0;

                            Array.Clear(recvResult, 0x0, recvResult.Length);
                        }
                    }
                //}
                //else
                //{
                //    rxByteCnt = 0;
                //    Array.Clear(recvResult, 0x0, recvResult.Length);
                //}
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Timeout: " + tex.Message);
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] IO Error: " + ioex.Message);
                SiteConnect(SiteCh);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] General Error: " + ex.Message);
            }
        }
    }
}
