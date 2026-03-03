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
        #region Fields
        private readonly object site_lock = new object();
        private readonly Queue<byte[]> sendSiteQueue = new Queue<byte[]>();
        // 수신 버퍼: DataReceived 이벤트에서 받은 데이터를 누적합니다.
        private List<byte> _recvBuffer = new List<byte>();
        private const int READ_TIMEOUT_MS = 7000;
        private const int READ_POLL_INTERVAL_MS = 10;

        // Watchdog 관련 필드
        private DateTime _lastActivityTime = DateTime.Now;
        private Timer _watchdogTimer;
        private const int WATCHDOG_INTERVAL_MS = 5000;      // 5초 간격
        private readonly TimeSpan WATCHDOG_THRESHOLD = TimeSpan.FromSeconds(10); // 10초 이상 응답없으면 재연결 시도
        #endregion

        #region Properties
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
        public SerialPort SitePort { get; private set; }
        private byte _packetStartByte = 0x00; // 패킷 시작 바이트 저장 변수
        #endregion

        #region Constructor
        public SiteCom()
        {
            Global.gSiteCom = this;
            
        }
        #endregion

        #region Connection Methods
        //bool frist_Send = false;
        public void SiteConnect(int ch)
        {
            try
            {
                // 초기화: 화면 갱신 및 박스 값 초기화
                Global.gMainPageVM.Racks[SiteCh].MotorStatText = string.Empty;
                Global.gMainPageVM.Racks[SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                Global.gEngineerVM.ManualMotor[SiteCh].eMotorStatColor = Global.gMotorStatColors[1];
                foreach (BoxModel bm in Global.gMainPageVM.Racks[SiteCh].Boxes)
                {
                    bm.HumidityValue = 0;
                }

                // 기존 포트가 있으면 닫고 Dispose
                if (SitePort != null)
                {
                    try
                    {
                        if (SitePort.IsOpen)
                        {
                            SitePort.DataReceived -= SitePort_DataReceived;
                            SitePort.Close();
                        }
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
                    Thread.Sleep(100);
                }

                // 채널 업데이트
                SiteCh = ch;

                if (string.IsNullOrEmpty(SitePortName))
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"SitePort {SiteCh + 1} Com Port Name Empty");
                    return;
                }

                

                //StartWatchdog();

				SitePortReconnetCnt = 0;

				// 큐 초기화 (동기화)
				lock (sendSiteQueue)
                {
                    sendSiteQueue.Clear();
                }
                // 수신 버퍼 초기화
                lock (_recvBuffer)
                {
                    _recvBuffer.Clear();
                }
                // 새 SerialPort 생성 및 설정
                SitePort = new SerialPort
                {
                    PortName = SitePortName,
                    BaudRate = 38400,
                    DataBits = 8,
                    Parity = Parity.None,
                    Handshake = Handshake.None,
                    StopBits = StopBits.One,
                    //ReadTimeout = READ_TIMEOUT_MS,
                    //WriteTimeout = READ_TIMEOUT_MS
                };

                // DataReceived 이벤트 핸들러 등록 (비동기 데이터 수신)SitePort_DataReceived
                SitePort.DataReceived += new SerialDataReceivedEventHandler(SitePort_DataReceived);
                SitePort.Open();
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");
                //frist_Send = true;
                /*
                var CheckPortStatus = MConv.CheckPortAlive(SitePortName, "USB\\VID_10C4&PID_EA60\\" + UsbHwSitePort);

                if (CheckPortStatus.Item1)
                {
                    if (SitePortName != CheckPortStatus.Item2)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} {SitePortName} -> {CheckPortStatus.Item2} Com Port Chage ");
                        Global.gConfigVM.MotorPortName[ch] = CheckPortStatus.Item2;

                        Global.gConfigVM.saveitems();
                    }
                    // 새 SerialPort 생성 및 설정
                    SitePort = new SerialPort
                    {
                        PortName = SitePortName,
                        BaudRate = 38400,
                        DataBits = 8,
                        Parity = Parity.None,
                        Handshake = Handshake.None,
                        StopBits = StopBits.One,
                        //ReadTimeout = READ_TIMEOUT_MS,
                        //WriteTimeout = READ_TIMEOUT_MS
                    };

                    // DataReceived 이벤트 핸들러 등록 (비동기 데이터 수신)SitePort_DataReceived
                    SitePort.DataReceived += new SerialDataReceivedEventHandler(SitePort_DataReceived);
                    SitePort.Open();
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Usb Info Match Connect");
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SitePort {SiteCh + 1} Usb Info Mismatch. Check Com Port");

                */
            }
            catch (Exception e)
            {
                // 예외 발생 시 안전하게 포트 해제
                try
                {
                    if (SitePort != null)
                    {
                        SitePort.DataReceived -= SitePort_DataReceived;
                        if (SitePort.IsOpen)
                            SitePort.Close();
                    }
                }
                catch { }
                finally
                {
                    SitePort?.Dispose();
                    SitePort = null;
                }
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[SitePort {SiteCh + 1} Exception] {e.Message}");
            }
        }
        public void PortCheck()
        {
            var CheckPortStatus = MConv.CheckPortAlive(SitePortName, "USB\\VID_10C4&PID_EA60\\" + UsbHwSitePort);

            if (CheckPortStatus.Item1)
            {
                if (SitePortName != CheckPortStatus.Item2)
                {
                    SiteDisConnect();

                    Thread.Sleep(2000);

                    SiteConnect(SiteCh);
                }
                //else
                //    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[SitePort {SiteCh + 1}] Port {SitePortName} Check OK");
            }
            else
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"SitePort {SiteCh + 1} Usb Info Mismatch. Check Com Port");
        }
        public void SiteDisConnect()
        {
            try
            {
                if (SitePort != null && SitePort.IsOpen)
                {
                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                    rm.MotorStatText = string.Empty;
                    foreach (var bm in rm.Boxes)
                        bm.HumidityValue = 0;

                    // 이벤트 핸들러 제거 후 포트 닫기
                    SitePort.DataReceived -= SitePort_DataReceived;
                    SitePort.Close();
                    //SitePort.Dispose();
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
        #endregion

        #region Message Sending Methods
        public void SiteWirteMsg(string val)
        {
            if (string.IsNullOrEmpty(val))
                return;

            int rotation = 0;
            int sendLen = MConv.CheckSendLength(val);

            try
            {
                // 첫 문자 이후 숫자 파싱 (오류 발생 시 0)
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
                txData[2] = 0x0A;
            }
            else
            {
                // 필요한 경우 다른 값 처리
                return;
            }

            if (SitePort == null || !SitePort.IsOpen)
                return;

            lock (sendSiteQueue)
            {
                sendSiteQueue.Enqueue(txData);
            }
            PreMotorSendMsg();
        }

        private void PreMotorSendMsg()
        {
            lock (site_lock)
            {
                //Console.WriteLine("MotorSendMessage() in");
                MotorSendMessage();
                //Console.WriteLine("MotorSendMessage() out");
            }
        }

        private int SitePortReconnetCnt = 0;
        // 전송 큐에서 하나의 메시지를 꺼내 전송하고, Write 이후 DataReceived 이벤트에서 응답을 기다립니다.
        private void MotorSendMessage()
        {
            if (SitePort == null || !SitePort.IsOpen)
                return;

            try
            {
                //if(frist_Send)
                //{
                //    await Task.Delay(10000);
                //    frist_Send = false;
                //}
                byte[] siteData = null;
                lock (sendSiteQueue)
                {
                    //Console.WriteLine("sendSiteQueue.Count ====>>>> {0}", sendSiteQueue.Count);
                    if (sendSiteQueue.Count == 0)
                        return;
                    siteData = sendSiteQueue.Dequeue();
                }

                Console.WriteLine("Data Write: " + BitConverter.ToString(siteData));
                GetSiteRecv = true;
                IsMtrValueUpdated = false;
                // 패킷 시작 바이트 저장
                _packetStartByte = siteData[0];

                // 전송 전에 수신 버퍼 초기화
                lock (_recvBuffer)
                {
                    _recvBuffer.Clear();
                }
                // 전송 후 최근 활동 시간 갱신
                _lastActivityTime = DateTime.Now;

                SitePort.Write(siteData, 0, siteData.Length);
                // 전송 후 타임아웃까지 기다릴 수 있는데,
                // DataReceived 이벤트에서 응답이 도착하면 HandleReceivedData에서 처리합니다.
                // 타임아웃 시 추가 처리가 필요하면 Timer 등을 활용할 수 있습니다.
                // 타임아웃 처리: 예를 들어 7000ms 동안 응답을 기다림
                const int TIMEOUT_MS = 10000;
                DateTime startTime = DateTime.Now;
                while (GetSiteRecv && (DateTime.Now - startTime).TotalMilliseconds < TIMEOUT_MS)
                {
                    Thread.Sleep(10);
                }

                // 타임아웃이 발생한 경우
                if (GetSiteRecv)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Write & Read Com Fail (Timeout)");
                    try
                    {
                        if (SitePort != null)
                        {
                            SitePort.DiscardInBuffer();
                            SitePort.DiscardOutBuffer();
                        }
                        /*
                        if (SitePortReconnetCnt > 10)
                        {
                            _watchdogTimer.Dispose();
                            SitePortReconnetCnt = 0;
							SiteDisConnect();
							string getDeviceId = MConv.GetDeviceInstanceId(SitePortName);
							MConv.SetComPortState(getDeviceId, false);

							Thread.Sleep(5000);

							MConv.SetComPortState(getDeviceId, true);
                            PortCheck();
                        }
                        else
                        {

                            if (SitePort != null)
                            {
                                SitePort.DiscardInBuffer();
                                SitePort.DiscardOutBuffer();
                            }
                            SitePortReconnetCnt++;
						}
                        */
                    }
                    catch (Exception ex)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Cleanup error: {ex.Message}");
                    }
                    GetSiteRecv = false; ;
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"Motor Write Fail, Board {SiteCh + 1}: {ex.Message}");
            }
            finally
            {
                // 타임아웃 또는 응답 후 후속 처리
                if (GetSiteRecv)
                {
                    // 아직 응답 플래그가 true라면, 응답 미도착 상태이므로 추가 정리 수행
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Final Cleanup: No response received, cleaning up buffers.");
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
                    // 플래그 재설정
                    GetSiteRecv = false;
                }
                else
                {
                    // 응답이 정상 처리된 경우
                    //Console.WriteLine($"[S{SiteCh + 1}] Final Cleanup: Response successfully processed.");
                }
            }
        }

        #endregion

        #region Response Reading
        // DataReceived 이벤트 핸들러: 수신되는 데이터를 누적하고 0x0A(개행) 문자가 있으면 패킷 완성
        private Queue<byte> _recvQueue = new Queue<byte>();

        private void SitePort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int bytesToRead = SitePort.BytesToRead;
                if (bytesToRead <= 0)
                    return;

                byte[] tempBuffer = new byte[bytesToRead];
                int readCount = SitePort.Read(tempBuffer, 0, bytesToRead);

                Console.WriteLine("tempBuffer : " + BitConverter.ToString(tempBuffer.ToArray()));
                if (readCount > 0)
                {
                    lock (_recvQueue)
                    {
                        // 데이터를 큐에 삽입
                        foreach (var byteData in tempBuffer)
                        {
                            _recvQueue.Enqueue(byteData);
                        }

                        _lastActivityTime = DateTime.Now;

                        // 패킷 싱크: Write한 데이터의 첫 번째 바이트(_packetStartByte)가 나올 때까지 버림
                        while (_recvQueue.Count > 0 && _recvQueue.Peek() != _packetStartByte)
                        {
                            _recvQueue.Dequeue();
                        }
                        
                        // 패킷의 끝(0x0A) 확인
                        if (_recvQueue.Contains(0x0A))
                        {
                            List<byte> completeData = new List<byte>();
                            while (_recvQueue.Count > 0)
                            {
                                byte byteData = _recvQueue.Dequeue();
                                completeData.Add(byteData);

                                // 패킷 종료 바이트(0x0A) 도달 시 처리
                                if (byteData == 0x0A)
                                {
                                    break;
                                }
                            }

                            Console.WriteLine("Data Received: " + BitConverter.ToString(completeData.ToArray()));
                            HandleReceivedData(completeData.ToArray());
                            GetSiteRecv = false; // 응답 수신 완료
                        }
                    }
                }
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Timeout in DataReceived: {tex.Message}");
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] IO Error in DataReceived: {ioex.Message}");
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] General Error in DataReceived: {ex.Message}");
            }
        }
        /*
        private void SitePort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int bytesToRead = SitePort.BytesToRead;
                if (bytesToRead <= 0)
                    return;

                byte[] tempBuffer = new byte[bytesToRead];
                int readCount = SitePort.Read(tempBuffer, 0, bytesToRead);
                if (readCount > 0)
                {
                    lock (_recvBuffer)
                    {
                        _recvBuffer.AddRange(tempBuffer);
                        // 만약 0x0A가 포함되어 있으면 패킷의 끝으로 판단
                        _lastActivityTime = DateTime.Now;
                        if (_recvBuffer.Contains(0x0A))
                        {
                            byte[] completeData = _recvBuffer.ToArray();
                            //Console.WriteLine("Data Received: " + BitConverter.ToString(completeData));
                            HandleReceivedData(completeData);
                            _recvBuffer.Clear();
                            // 플래그 재설정 (응답 수신 완료)
                            GetSiteRecv = false;
                        }
                    }
                }
            }
            catch (TimeoutException tex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Timeout in DataReceived: {tex.Message}");
            }
            catch (IOException ioex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] IO Error in DataReceived: {ioex.Message}");
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] General Error in DataReceived: {ex.Message}");
            }
        }
        */
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
                UInt16 rotattion = (UInt16)((getBuf[1] << 8) | (getBuf[2]));
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1}]Up Recv OK : {rotattion}");
                Global.gEngineerVM.addMotorLog($"[S{SiteCh + 1}]Up Recv OK : {rotattion}");
            }
            else if (getBuf[0] == 0x4c)
            {
                IsMotorDown = true;
                rm.MotorStatText = "CLOSE";
                UInt16 rotattion = (UInt16)((getBuf[1] << 8) | (getBuf[2]));
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[S{SiteCh + 1}]Down Recv OK : {rotattion}");
                Global.gEngineerVM.addMotorLog($"[S{SiteCh + 1}]Down Recv OK : {rotattion}");
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
                    //1231 Org
                    if((sense1 == 1) && (sense3 == 1))
                    {
                        Board_SensingOK = true;
                    }
                    
                    /*// 1Para Test 용
                    if (sense3 == 1)
                        Board_SensingOK = true;
                    */
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

        #region Watchdog & Reconnection

        private void StartWatchdog()
        {
            // WATCHDOG_INTERVAL_MS마다 WatchdogCallback를 호출
            _watchdogTimer = new Timer(WatchdogCallback, null, WATCHDOG_INTERVAL_MS, WATCHDOG_INTERVAL_MS);
        }

        private void WatchdogCallback(object state)
        {
            // 최근 활동이 일정 시간 이상 없으면 포트가 먹통 상태로 간주하고 재연결 시도
            if (DateTime.Now - _lastActivityTime > WATCHDOG_THRESHOLD)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[S{SiteCh + 1}] Watchdog: No activity for {_lastActivityTime.ToString()} - attempting reconnection.");
                try
                {
                    if (SitePortName == "")
                    {
                        _watchdogTimer.Dispose();
                        return;
                    }
                    //Global.gMainPageVM.Racks[SiteCh].CheckSiteConnet("Site");
                    /*
                    // 안전하게 연결 해제 후 재연결 시도
                    SiteDisConnect();
                    Thread.Sleep(1000); // 재연결 전 딜레이
                    SiteConnect(SiteCh);
                    */
                    SiteDisConnect();
                    string getDeviceId = MConv.GetDeviceInstanceId(SitePortName);
                    MConv.SetComPortState(getDeviceId, false);

                    Thread.Sleep(5000);

                    MConv.SetComPortState(getDeviceId, true);
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[S{SiteCh + 1}] Reconnection failed: {ex.Message}");
                }
            }
        }

        #endregion
    }
}
