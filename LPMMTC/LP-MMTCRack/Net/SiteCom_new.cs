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

        public Task _SiteWriteTask;
        public Task _SiteReadTask;
        private CancellationTokenSource _cancellationTokenSource = new CancellationTokenSource();

        public SerialPort SitePort;
        public SiteCom()
        {
            _SiteWriteTask = Task.Run(SiteWriteFunction, _cancellationTokenSource.Token);
            _SiteReadTask = Task.Run(SiteReadFunction, _cancellationTokenSource.Token);
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

                SitePort = new SerialPort
                {
                    PortName = SitePortName,
                    BaudRate = 38400,
                    DataBits = 8,
                    Parity = Parity.None,
                    Handshake = Handshake.None,
                    StopBits = StopBits.One,
                    ReadTimeout = 500,
                    WriteTimeout = 500
                };

                SitePort.DataReceived += SiteBoard_DataReceivedHandler;
                SitePort.Open();
                lock (site_lock)
                {
                    sendSiteQueue.Clear();
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Connect");
            }
            catch (Exception e)
            {
                SitePort?.Close();
                SitePort?.Dispose();
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
            try
            {
                if (SitePort != null)
                {
                    var rm = Global.gMainPageVM.getRack(SiteCh);
                    rm.MotorStatText = string.Empty;
                    foreach (var bm in rm.Boxes)
                    {
                        bm.HumidityValue = 0;
                    }

                    SitePort.Close();
                    SitePort.Dispose();
                    SitePort = null;
                }
            }
            catch { SitePort.Dispose(); }
        }

        public async Task SiteWriteFunction()
        {
            while (!_cancellationTokenSource.Token.IsCancellationRequested)
            {
                if (SitePort == null || !SitePort.IsOpen)
                {
                    await Task.Delay(5000, _cancellationTokenSource.Token);
                    continue;
                }

                if (getSendCnt() == 0)
                {
                    await Task.Delay(100, _cancellationTokenSource.Token);
                }
                else
                {
                    var packet = popSend();
                    send(packet);
                    Console.WriteLine($"3. SitePort.IsOpen = {SitePort.IsOpen}");
                    //await Task.Delay(10, _cancellationTokenSource.Token);
                }

                await Task.Delay(100, _cancellationTokenSource.Token);
            }
        }

        public byte[] popSend()
        {
            lock (site_lock)
            {
                Console.WriteLine("sendSiteQueue.Count ====>>>> {0}", sendSiteQueue.Count);
                return sendSiteQueue.Count > 0 ? sendSiteQueue.Dequeue() : null;
            }
        }
        public int WriteCmd { get; set; } = 0;
        public void send(byte[] data)
        {
            if (SitePort == null || !SitePort.IsOpen)
                return;

            try
            {
                GetSiteRecv = true;
                var sendmsg = BitConverter.ToString(data).Replace("-", " ");
                Console.WriteLine($"Write = {sendmsg}");

                WriteCmd = data[0];
                Console.WriteLine($"1. SitePort.IsOpen = {SitePort.IsOpen}");
                SitePort.Write(data, 0, data.Length);
                Console.WriteLine($"2. SitePort.IsOpen = {SitePort.IsOpen}");

                if (IsMtrValueUpdated)
                    IsMtrValueUpdated = false;
            }
            catch (Exception ex)
            {
                var msg = $"Site Write Fail, Board{SiteCh + 1} {ex.Message}";
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }

        public int getSendCnt()
        {
            lock (site_lock)
            {
                return sendSiteQueue.Count;
            }
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

            //PreMotorSendMsg();
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
            //PreMotorSendMsg();
        }
        public object site_lock = new object();
        private async void PreMotorSendMsg()
        {
            //lock (site_lock)
            //{
            await MotorSendMessageAsync();
            //}
        }
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
        /*
        private void MotorSendMessage()
        {
            if (SitePort == null)
                return;
            if (!SitePort.IsOpen)
                return;

            try
            {
                int SiteRetry = 0;
                byte[] siteData = null;

                lock (sendSiteQueue)
                {
                    Console.WriteLine("sendSiteQueue.Count ====>>>> {0}", sendSiteQueue.Count);
                    if (sendSiteQueue.Count == 0)
                        return;

                        siteData = sendSiteQueue.Dequeue();
                }

                GetSiteRecv = true;

                SitePort.Write(siteData, 0, siteData.Length);

                if (IsMtrValueUpdated)
                    IsMtrValueUpdated = false;
                while (SiteRetry <= 100)
                {
                    if (!GetSiteRecv)
                    {
                        Console.WriteLine("Recv OK");
                        break;
                    }

                    Thread.Sleep(100);
                    SiteRetry++;
                }

                if (GetSiteRecv)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Write & Read Com Fail ");
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
        */

        public List<byte> RecvDataList { get; private set; } = new List<byte>();
        private readonly object RecvObj = new object();
        public async void SiteReadFunction()
        {
            while (!_cancellationTokenSource.Token.IsCancellationRequested)
            {
                while (!Global.gMainVM._isCloseCom)
                {
                    if (RecvDataList.Count >= 4)
                    {
                        if (RecvDataList.Count > 50)
                        {
                            Console.WriteLine($"########## RecvDataList.Count Over = {RecvDataList.Count}");
                            lock (RecvObj)
                                RecvDataList.Clear(); // 50 넘어가면 초기화
                        }
                        RecvParing();
                    }
                    else
                        await Task.Delay(100);
                }
            }
        }

        public void SiteBoard_DataReceivedHandler(object sender, EventArgs e)
        {
            try
            {
                if (SitePort == null || !SitePort.IsOpen)
                    return;

                int rdCnt = SitePort.BytesToRead;
                if (rdCnt == 0)
                    return;

                byte[] buffer = new byte[rdCnt];
                SitePort.Read(buffer, 0, rdCnt);
                lock (RecvObj)
                    RecvDataList.AddRange(buffer);

                //Console.WriteLine("Data Received: " + BitConverter.ToString(buffer));
            }
            catch (Exception ex)
            {
                SitePort?.DiscardInBuffer();
                SitePort?.DiscardOutBuffer();
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Data Received Error: " + ex.Message);
            }
        }

        void RecvParing()
        {
            int endIndex = 0;
            try
            {
                string msge = string.Empty;


                lock (RecvObj)
                    endIndex = RecvDataList.IndexOf(0x0A);
                if (endIndex != -1)
                {
                    byte[] _readBuf;
                    lock (RecvObj)
                        _readBuf = RecvDataList.Take(endIndex + 1).ToArray();

                    if (_readBuf.Length > 0 && _readBuf[endIndex] == 0x0A)
                    {
                        msge = string.Join(" ", _readBuf.Select(b => b.ToString("X2")));
                        Console.WriteLine($"Read OK = {msge}");

                        switch (_readBuf[0])
                        {
                            case 0x49:
                                SensorCheck(_readBuf);
                                break;
                            case 0x56:
                                FWVerionCheck(_readBuf);
                                break;
                            case 0x41:
                                HumidityCheck(_readBuf);
                                break;
                            case 0x51:
                            case 0x52:
                                MotorInterlockCheck(_readBuf);
                                break;
                            case 0x48:
                            case 0x4c:
                                MotorRoationCheck(_readBuf);
                                break;
                            case 0x11:
                                MotorRotationCntCheck(_readBuf);
                                break;
                            case 0x31:
                            case 0x33:
                                PressureSensorCheck(_readBuf);
                                break;
                            case 0x32:
                                MotorPoweronCheck(_readBuf);
                                break;
                        }
                    }
                    lock (RecvObj)
                        RecvDataList.RemoveRange(0, endIndex + 1);
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Read Parsing Error: " + ex.Message);
                lock (RecvObj)
                    RecvDataList.RemoveRange(0, endIndex + 1);
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
            //SendCheckTimer.Stop();
        }

        void FWVerionCheck(byte[] getBuf)
        {
            RackModel rm = Global.gMainPageVM.getRack(SiteCh);
            Global.gEngineerVM.ManualMotor[rm.RackIndex].GetSiteVer = getBuf[1].ToString();
            GetSiteRecv = false;
            //SendCheckTimer.Stop();
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
            //SendCheckTimer.Stop();
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
            //SendCheckTimer.Stop();
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
            //SendCheckTimer.Stop();
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
            //SendCheckTimer.Stop();
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
            //SendCheckTimer.Stop();
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
            //SendCheckTimer.Stop();
        }
    }
}
