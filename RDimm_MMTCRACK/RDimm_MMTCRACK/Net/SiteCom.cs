using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.View.Component;
using RDimm_MMTCRACK.ViewModel;
using ScottPlot.Statistics;
using Serilog;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace RDimm_MMTCRACK.Net
{
    public class SiteCom
    {
        #region Fields
        private readonly object _syncRoot = new object();
        private readonly ConcurrentQueue<byte[]> sendSiteQueue = new ConcurrentQueue<byte[]>();
        private readonly ConcurrentQueue<byte> _recvQueue = new ConcurrentQueue<byte>();
        private readonly object _portLock = new object();
        private volatile bool _getSiteRecv = true;
        private volatile bool _isMtrValueUpdated = false;
        private bool _isProcessing = false;
        private volatile byte _packetStartByte = 0x00;
        private const int READ_TIMEOUT_MS = 7000;
        private const int READ_POLL_INTERVAL_MS = 10;
        #endregion

        #region Properties
        public int SiteCh { get; set; } = 0;
        public bool SiteinitStat { get; set; } = false;

        public int ConnetSiteid { get; set; } = 0;
        public string SitePortName { get; set; } = string.Empty;
        public string UsbHwSitePort { get; set; } = string.Empty;
        public string UsbSiteNameing { get; set; } = string.Empty;
        public string MotorValue { get; set; } = string.Empty;
        public bool SitePortUse { get; set; } = false;
        public string MotorFinalStep { get; set; } = string.Empty;
        public bool MotorPositon { get; set; } = false;
        public bool GetSiteRecv 
        { 
            get => _getSiteRecv;
            set => _getSiteRecv = value;
        }
        public bool IsMotorMove { get; set; } = false;
        public bool IsMtrValueUpdated
        {
            get => _isMtrValueUpdated;
            set => _isMtrValueUpdated = value;
        }
        public bool IsGetSiteRecv { get; set; } = false;
        public bool IsMotorDown { get; set; } = false;
        public bool Board_SensingOK { get; set; } = false;
        public bool Board_MotorPowerOn { get; set; } = false;
        public bool isMotorStatus { get; set; } = true;
        public SerialPort SitePort { get; private set; }
        #endregion

        #region Constructor
        public SiteCom()
        {
            Global.gSiteCom = this;
        }
        #endregion

        #region Connection Methods
        public async void SiteConnect(int ch)
        {
            try
            {
                //SiteinitStat = false;
                // 초기화: 화면 갱신 및 박스 값 초기화
                Global.gMainPageVM.Racks[SiteCh].MotorStatText = string.Empty;
                Global.gMainPageVM.Racks[SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                Global.gEngineerVM.ManualMotor[SiteCh].eMotorStatColor = Global.gMotorStatColors[1];

                foreach (CBoxModel bm in Global.gMainPageVM.Racks[SiteCh].CBoxs)
                {
                    bm.HumidityValue = 0;
                }

                // 기존 포트가 있으면 닫고 Dispose
                if (SitePort != null)
                {
                    try
                    {
                        SitePort.DataReceived -= SitePort_DataReceived;
                        if (SitePort.IsOpen)
                            SitePort.Close();

                    }
                    catch (Exception ex)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Close Exception: {ex.Message}");
                    }
                    finally
                    {
                        SitePort.Dispose();
                        SitePort = null;
                    }
                }

                await Task.Delay(100);

                // 채널 업데이트
                SiteCh = ch;

                if (string.IsNullOrEmpty(SitePortName))
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"SitePort {SiteCh + 1} Com Port Name Empty");
                    return;
                }
                if (!MConv.CheckPortAvailble(SitePortName))
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"SitePort {SiteCh + 1} Com Port {SitePortName} Not Available");
                    return;
                }

                // 큐 초기화
                while (sendSiteQueue.TryDequeue(out _)) { }
                while (_recvQueue.TryDequeue(out _)) { }

                // 새 SerialPort 생성 및 설정
                // 새 SerialPort 생성 및 설정
                SitePort = new SerialPort
                {
                    PortName = SitePortName,
                    BaudRate = 38400,
                    DataBits = 8,
                    Parity = Parity.None,
                    Handshake = Handshake.None,
                    StopBits = StopBits.One
                };

                // DataReceived 이벤트 핸들러 등록 (비동기 데이터 수신)SitePort_DataReceived
                SitePort.DataReceived += new SerialDataReceivedEventHandler(SitePort_DataReceived);
                SitePort.Open();
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");

                //SiteWirteMsg("D");

                //Thread.Sleep(1000);
                //await Task.Delay(1000);

                //if (ConnetSiteid == (SiteCh +1))
                //{
                //  SiteinitStat = true;
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");
                //}
                //else
                //{
                //    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SitePort {SiteCh + 1} {ConnetSiteid} , {SiteCh + 1} UnMatch. Cehck Site BD");
                //    SiteDisConnect();
                //}
            }
            catch (Exception e)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[SitePort {SiteCh + 1} Exception] {e.Message}");

                if (SitePort != null)
                {
                    try
                    {
                        SitePort.DataReceived -= SitePort_DataReceived;
                        if (SitePort.IsOpen)
                            SitePort.Close();
                    }
                    catch { }
                    finally
                    {
                        SitePort?.Dispose();
                        SitePort = null;
                    }
                }
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[SitePort {SiteCh + 1} Exception] {e.Message}");
            }
        }

        public void SiteDisConnect()
        {
            try
            {
                if (SitePort != null && SitePort.IsOpen)
                {
                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                    rm.MotorStatText = string.Empty;
                    foreach (var bm in rm.CBoxs)
                        bm.HumidityValue = 0;

                    SitePort.DataReceived -= SitePort_DataReceived;
                    SitePort.Close();
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1}] DisConnect Done");
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] DisConnect Exception: {ex.Message}");
            }
            finally
            {
                if (SitePort != null)
                {
                    SitePort.Dispose();
                    SitePort = null;
                }
            }
        }

        /*
        private void SafePortOperation(Action operation)
        {
            lock (_portLock)
            {
                operation();
            }
        }
        */
        #endregion

        #region Message Sending Methods
        /*
        public async Task<bool> SiteWirteMsg(string val)
        {
            if (string.IsNullOrEmpty(val))
                return false;

            try
            {
                // 1. 데이터 준비
                int rotation = 0;
                int sendLen = MConv.CheckSendLength(val);

                try
                {
                    string strCount = val.Substring(1);
                    if (string.IsNullOrEmpty(strCount))
                        strCount = "0";
                    rotation = int.Parse(strCount);
                }
                catch
                {
                    rotation = 0;
                }

                byte[] txData = MConv.MakeSiteMeg(val, rotation, sendLen);

                if (SitePort == null || !SitePort.IsOpen)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Port is not available");
                    return false;
                }

                // 2. 큐에 데이터 추가
                lock (_syncRoot)
                {
                    sendSiteQueue.Enqueue(txData);

                    if (_isProcessing)
                        return true;

                    _isProcessing = true;
                }

                // 3. 비동기 전송 시작
                return await ProcessMessageQueueAsync();
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SiteWirteMsg Error: {ex.Message}");
                return false;
            }
        }

        public async Task<bool> SiteHeightWriteMsg(int val)
        {
            byte[] txData = new byte[3];

            if (val == 17)
            {
                txData[0] = 0x11;
                txData[1] = 0x11;
                txData[2] = 0x0A;
            }
            else
            {
                // 필요한 경우 다른 값 처리
                return false;
            }

            if (SitePort == null || !SitePort.IsOpen)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Port is not available");
                return false;
            }

            lock (_syncRoot)
            {
                sendSiteQueue.Enqueue(txData);

                if (_isProcessing)
                    return true;

                _isProcessing = true;
            }

            return await ProcessMessageQueueAsync();
        }

        private async Task<bool> ProcessMessageQueueAsync()
        {
            while (true)
            {
                byte[] message = null;

                // 큐에서 메시지 가져오기
                lock (_syncRoot)
                {
                    if (sendSiteQueue.Count == 0)
                    {
                        _isProcessing = false;
                        return true;
                    }
                    sendSiteQueue.TryDequeue(out message);
                }

                // 메시지 처리
                try
                {
                    bool result = await SendMessageAsync(message);
                    if (!result)
                        return false;
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"Message processing error: {ex.Message}");
                    return false;
                }
            }
        }
        */

        public async Task<bool> SiteWirteMsg(string val)
        {
            if (string.IsNullOrEmpty(val))
                return false;

            try
            {
                int rotation = 0;
                int sendLen = MConv.CheckSendLength(val);

                try
                {
                    string strCount = val.Substring(1);
                    rotation = int.Parse(string.IsNullOrEmpty(strCount) ? "0" : strCount);
                }
                catch
                {
                    rotation = 0;
                }

                byte[] txData = MConv.MakeSiteMeg(val, rotation, sendLen);

                if (SitePort == null || !SitePort.IsOpen)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Port is not available");
                    return false;
                }

                bool shouldStartProcessing = false;
                lock (_syncRoot)
                {
                    sendSiteQueue.Enqueue(txData);

                    if (!_isProcessing)
                    {
                        _isProcessing = true;
                        shouldStartProcessing = true;
                    }
                }

                if (shouldStartProcessing)
                {
                    _ = ProcessMessageQueueAsync(); // await 안 걸고 백그라운드로 시작
                }

                await Task.Delay(10);
                return true;
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SiteWriteMsg Error: {ex.Message}");
                return false;
            }
        }

        public async Task<bool> SiteHeightWriteMsg(int val)
        {
            byte[] txData = new byte[3];

            if (val == 17)
            {
                txData[0] = 0x11;
                txData[1] = 0x11;
                txData[2] = 0x0A;
            }
            else
            {
                return false;
            }

            if (SitePort == null || !SitePort.IsOpen)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Port is not available");
                return false;
            }

            bool shouldStartProcessing = false;
            lock (_syncRoot)
            {
                sendSiteQueue.Enqueue(txData);

                if (!_isProcessing)
                {
                    _isProcessing = true;
                    shouldStartProcessing = true;
                }
            }

            if (shouldStartProcessing)
            {
                _ = ProcessMessageQueueAsync();
            }

            await Task.Delay(10);
            return true;
        }

        private async Task ProcessMessageQueueAsync()
        {
            while (true)
            {
                byte[] message = null;

                lock (_syncRoot)
                {
                    if (sendSiteQueue.Count == 0)
                    {
                        _isProcessing = false;
                        return;
                    }
                    sendSiteQueue.TryDequeue(out message);
                }

                try
                {
                    bool result = await SendMessageAsync(message);
                    if (!result)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"Failed to send message.");
                        // 실패해도 큐가 남아있으면 다시 처리하도록 계속 돌릴 수도 있지만,
                        // 일단 실패하면 정지하도록 설계
                        lock (_syncRoot)
                        {
                            _isProcessing = false;
                        }
                        return;
                    }
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"Message processing error: {ex.Message}");
                    lock (_syncRoot)
                    {
                        _isProcessing = false;
                    }
                    return;
                }
            }
        }
        private async Task<bool> SendMessageAsync(byte[] siteData)
        {
            if (SitePort == null || !SitePort.IsOpen)
                return false;
            try
            {
                GetSiteRecv = true;
                IsMtrValueUpdated = false;
                _packetStartByte = siteData[0];

                SitePort.Write(siteData, 0, siteData.Length);

                // 5. 비동기 응답 대기
                return await WaitForResponseAsync();
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SendMessage Error: {ex.Message}");
                return false;
            }
        }
        private async Task<bool> WaitForResponseAsync()
        {
            const int TIMEOUT_MS = 7000;
            var cts = new CancellationTokenSource(TIMEOUT_MS);

            try
            {
                // 응답 대기
                while (GetSiteRecv && !cts.Token.IsCancellationRequested)
                {
                    await Task.Delay(10, cts.Token);
                }

                if (GetSiteRecv) // 타임아웃 발생
                {
                    HandleTimeout();
                    return false;
                }

                return true;
            }
            catch (OperationCanceledException)
            {
                HandleTimeout();
                return false;
            }
            finally
            {
                cts.Dispose();
            }
        }

        private void HandleTimeout()
        {
            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Write & Read Com Fail (Timeout)");
            try
            {
                if (SitePort != null)
                {
                    SitePort.DiscardInBuffer();
                    SitePort.DiscardOutBuffer();
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Cleanup error: {ex.Message}");
            }
            GetSiteRecv = false;
        }
        #endregion

        #region Response Reading
        private readonly Dictionary<byte, int> _commandLengths = new Dictionary<byte, int>
        {
            { 0x41, 10 },   // 0x41: 10바이트 (시작 바이트 + 9바이트 + 0x0A)
            { 0x48, 5 },    // 0x48 0x4C: 5바이트
            { 0x4C, 5 },    
            { 0x49, 5 },    // 0x49: 5바이트
            { 0x46, 2 },    // 0x46: 2바이트
            { 0x51, 4 },    // 0x51 0x52: 4바이트
            { 0x52, 4 },
            { 0x56, 4 },    // 0x56: 4바이트
            { 0x11, 5 },    // 0x11: 5바이트
            { 0x31, 6 },    // 0x31: 6바이트
            { 0x33, 6 },    // 0x33: 6바이트
            { 0x32, 4 },    // 0x32: 4바이트
            { 0x66, 4 }     // 0x66: 4바이트
        };

        private void SitePort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int bytesToRead = SitePort.BytesToRead;
                if (bytesToRead <= 0)
                    return;

                byte[] tempBuffer = new byte[bytesToRead];
                int readCount = 0;

                readCount = SitePort.Read(tempBuffer, 0, bytesToRead);

                if (readCount <= 0)
                    return;

                foreach (byte byteData in tempBuffer)
                {
                    _recvQueue.Enqueue(byteData);
                }

                while (_recvQueue.Count > 0)
                {
                    // 1. _packetStartByte로 시작하는지 확인
                    if (!_recvQueue.TryPeek(out byte firstByte) || firstByte != _packetStartByte)
                    {
                        _recvQueue.TryDequeue(out _); // 시작 바이트 안 맞으면 버림
                        continue;
                    }

                    // 2. 패킷 길이 확인
                    if (!_commandLengths.TryGetValue(_packetStartByte, out int packetLength))
                    {
                        _recvQueue.TryDequeue(out _); // 정의되지 않은 바이트 무시
                        continue;
                    }

                    // 3. 충분한 길이인지 확인
                    if (_recvQueue.Count < packetLength)
                        break; // 더 기다려야 함

                    // 4. 패킷 추출
                    byte[] packet = new byte[packetLength];
                    bool isValid = true;
                    for (int i = 0; i < packetLength; i++)
                    {
                        if (_recvQueue.TryDequeue(out byte b))
                        {
                            packet[i] = b;
                        }
                        else
                        {
                            isValid = false;
                            break;
                        }
                    }

                    if (!isValid)
                        continue;

                    // 5. 종료 바이트 검사
                    if (packet[packetLength - 1] == 0x0A)
                    {
                        Console.WriteLine($"[S{SiteCh + 1}] Valid Packet: {BitConverter.ToString(packet)}");
                        HandleReceivedData(packet);
                        GetSiteRecv = false;
                    }
                    else
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING,
                            $"[S{SiteCh + 1}] Invalid Packet: {BitConverter.ToString(packet)}");
                    }
                }
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Timeout in DataReceived: {tex.Message}");
                GetSiteRecv = false;
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] IO Error in DataReceived: {ioex.Message}");
                GetSiteRecv = false;
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] General Error in DataReceived: {ex.Message}");
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
                case 0xff:
                    FailCheck(data);
                    break;
                case 0x66:
                    IDCheck(data);
                    break;
            }
        }

        void IDCheck(byte[] data)
        {
            ConnetSiteid = data[1];
        }
        void FailCheck(byte[] data)
        {
            RackModel rm = Global.gMainPageVM.getRack(SiteCh);
            if (rm == null) return;

            if (data[1] == 0xff)
            {
                if (data[2] == 0x01)
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[S ({SiteCh + 1}] buffer overflow error");
                else if (data[2] == 0x02)
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[S ({SiteCh + 1}] data cehcksum error");
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

                humidity[0] = getBuf[2];
                humidity[1] = getBuf[4];
                humidity[2] = getBuf[3];

                if ((temperature[0] & 0x80) > 0)
                    temperature[0] = -temperature[0];
                if ((temperature[1] & 0x80) > 0)
                    temperature[1] = -temperature[1];
                if ((temperature[2] & 0x80) > 0)
                    temperature[2] = -temperature[2];

                for (int ch = 0; ch < rm.CBoxs.Count; ch++)
                {
                    rm.CBoxs[ch].SetHumidityValue(humidity[ch]);
                    rm.CBoxs[ch].Hum_TempSenorVal = temperature[ch];
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

                msg = "[S" + (SiteCh + 1).ToString() + "] ";
                if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                if (sense2 == 1) { msg += "C(ON) "; } else { msg += "C(OFF) "; }
                if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                Global.gEngineerVM.addMotorLog(msg);

                if ((sense1 == 1) && (sense2 == 1) && (sense3 == 1))
                {
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
        #endregion
    }
}
