using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.Net;
using LP_MMTCRack.View;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
using static HandyControl.Tools.Interop.InteropValues;

namespace LP_MMTCRack.ViewModel
{
    public enum VIEW_SEL : int
    {
        MAIN = 0,
        GRAPH,
        ENGINEER,
        CONFIG
    }
    public enum TOTALRACK_STAT : int
    {
        RUN = 0,
        PARAMETER,
        STOP
    }
    public class MainVM :BaseVM
    {
        /*
        TconCom tc;
        FlowMeterCom fmc;
        AlarmCom ac;
        public string SelectViewName { get; set; }
        public string SelectViewDiscription { get; set; }
        public string SelectName { get; set; }
        public Visibility ShowMain { get; set; }
        public Visibility ShowGraph { get; set; }
        public Visibility ShowFan { get; set; }
        public Visibility ShowPower { get; set; }
        public Visibility ShowEngineer { get; set; }
        public Visibility ShowConfig { get; set; }
        public string LogoPath { get; set; }

        public string LottieFilePath { get; set; }

        public string[] LottieTabPath = new string[4]; 
        
        //[DoNotNotify] public int GraphIndex { get; set; } = 0;
        public TOTALRACK_STAT AllRackSTAT { get; set; } = TOTALRACK_STAT.RUN;

        public CancellationTokenSource _cancellationTokenSource;
        public Task _TconStatTask;
        public Task _SiteStatTask;
        public Task _FlowStatTask;
        public Task _CaptureStatTask;
        */
        #region Fields & Properties

        // 외부 클래스 인스턴스 (TCON, FlowMeter, Alarm 등)
        private TconCom _tconCom;
        private FlowMeterCom _flowMeterCom;
        private AlarmCom _alarmCom;

        // UI 관련 속성
        public string SelectViewName { get; set; }
        public string SelectViewDiscription { get; set; }
        public string SelectName { get; set; }
        public Visibility ShowMain { get; set; }
        public Visibility ShowGraph { get; set; }
        public Visibility ShowFan { get; set; }
        public Visibility ShowPower { get; set; }
        public Visibility ShowEngineer { get; set; }
        public Visibility ShowConfig { get; set; }
        public string LogoPath { get; set; }


        private string lottiefilepath;
        public string LottieFilePath 
        { 
            get => lottiefilepath; 
            set
            {
                lottiefilepath = value;
                PC("LottieFilePath");
            }
        }

        public string[]LottieEachPath { get; set; } = new string[4];
        public string LottieTabPath { get; set; }

        public TOTALRACK_STAT AllRackSTAT { get; set; } = TOTALRACK_STAT.RUN;

        // 취소 토큰 및 작업(Task) 관리
        public CancellationTokenSource _cancellationTokenSource;
        public Task _tconStatTask;
        public Task _siteStatTask;
        public Task _flowStatTask;
        public Task _captureStatTask;

        // 플래그 및 루프 시간 기록
        public bool IsCloseCom { get; set; } = false;
        public bool IsCloseFlow { get; set; } = false;
        public DateTime SiteLoopTime { get; set; }
        public DateTime CaptureLoopTime { get; set; }

        #endregion

        #region 생성자 및 초기화

        public MainVM()
        {
            // Global 참조 업데이트 (글로벌에서 MainVM에 접근 가능하도록)
            Global.gMainVM = this;

            // 관련 클래스 인스턴스 생성
            _tconCom = new TconCom();
            _flowMeterCom = new FlowMeterCom();
            _alarmCom = new AlarmCom();

            // CancellationTokenSource 생성
            _cancellationTokenSource = new CancellationTokenSource();

            // 비동기 작업 시작 (앞서 리팩토링한 기능 클래스를 사용한다고 가정)
            _tconStatTask = Task.Run(() => TconStatFunctionAsync(_cancellationTokenSource.Token));
            _siteStatTask = Task.Run(() => SiteStaFunctionAsync(_cancellationTokenSource.Token));
            _flowStatTask = Task.Run(() => FlowStatFunctionAsync(_cancellationTokenSource.Token));
            _captureStatTask = Task.Run(() => CaptureFunctionAsync(_cancellationTokenSource.Token));

            // 초기 AllRackSTAT 값 설정 (기본적으로 STOP 상태)
            AllRackSTAT = TOTALRACK_STAT.STOP;
        }

        #endregion

        #region Public Methods (View 변경, 종료 등)

        /// <summary>
        /// 모든 비동기 작업을 취소하고 종료 대기
        /// </summary>
        public async Task StopAllTasksAsync()
        {
            try
            {
                _cancellationTokenSource.Cancel();

                // 모든 작업이 정상적으로 종료될 때까지 대기
                await Task.WhenAll(_tconStatTask, _siteStatTask, _flowStatTask, _captureStatTask);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[OS] 비동기 작업 종료 중 오류 발생: {ex.Message}");
            }
        }

        /// <summary>
        /// 화면 전환 처리 (메인, 그래프, 엔지니어, 설정 등)
        /// </summary>
        public void ChangeView(string viewName)
        {
            // 모든 화면 숨김 처리
            SelectName = viewName;
            ShowMain = Visibility.Hidden;
            ShowGraph = Visibility.Hidden;
            ShowFan = Visibility.Hidden;
            ShowPower = Visibility.Hidden;
            ShowEngineer = Visibility.Hidden;
            ShowConfig = Visibility.Hidden;

            if (viewName == VIEW_SEL.MAIN.ToString())
            {
                SelectViewName = "Main Control";
                SelectViewDiscription = "Operate The Chamber And Perform Test Functions";
                ShowMain = Visibility.Visible;
                Global.gMainWindow.ChagneLottie(viewName);
            }
            else if (viewName == VIEW_SEL.GRAPH.ToString())
            {
                SelectViewName = "Temperature";
                SelectViewDiscription = "Output Real-Time Temperature And Graph";
                ShowGraph = Visibility.Visible;
                Global.gGraphVM.SetBox();
                Global.gMainWindow.ChagneLottie(viewName);
            }
            else if (viewName == VIEW_SEL.ENGINEER.ToString())
            {
                if (ShowPasswordDialog())
                {
                    SelectViewName = "Engineer Mode";
                    SelectViewDiscription = "Set up Register Manually And Execute Command";
                    ShowEngineer = Visibility.Visible;
                    Global.gMainWindow.ChagneLottie(viewName);
                }
            }
            else if (viewName == VIEW_SEL.CONFIG.ToString())
            {
                if (ShowPasswordDialog())
                {
                    SelectViewName = "Program Configuration";
                    SelectViewDiscription = "Check and change the settings required for the Program";
                    ShowConfig = Visibility.Visible;
                    Global.gMainWindow.ChagneLottie(viewName);
                }
            }
        }

        /// <summary>
        /// 비밀번호 대화상자 표시 후, 올바른 비밀번호 입력 시 true 반환.
        /// </summary>
        private bool ShowPasswordDialog()
        {
            // 비밀번호 사용하지 않을 경우 바로 true 반환
            if (!Global.gConfigVM.IsUsePassword)
                return true;

            Global.gConfigVM.GetPassword = "";
            SelectViewName = "Password Page";
            SelectViewDiscription = "You'll need to enter a password to open this page";

            PassWordView pwDialog = new PassWordView
            {
                WindowStartupLocation = WindowStartupLocation.CenterOwner
            };

            bool? dialogResult = pwDialog.ShowDialog();
            return (dialogResult.HasValue && dialogResult.Value);
        }

        /// <summary>
        /// 프로그램 종료 처리 (연결 해제, 작업 취소 등)
        /// </summary>
        public async Task QuitAsync()
        {
            try
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[OS] MAIN UI CLICK CLOSE.");

                // 연결 종료 플래그 업데이트
                IsCloseCom = true;
                IsCloseFlow = true;

                // TCON 관련 종료 처리
                _tconCom.TConquit();

                // 각 장비 연결 해제
                Global.gTconCom.TConDisConnet(true);
                Global.gAlarmCom.AlarmDisConnect();
                Global.gFlowMeterCom.FlowMeterDiConnect();

                // 사이트 및 캡처 연결 해제 (배열의 인덱스 0~5)
                for (int i = 0; i < 6; i++)
                {
                    Global.gMainPageVM.sc[i].SiteDisConnect();
                    Global.gMainPageVM.cc[i].Disconnect();
                }

                // 비동기 작업 종료
                _cancellationTokenSource?.Cancel();
                await StopAllTasksAsync();

                // (필요하다면) 추가적인 종료 처리...
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[OS] 종료 중 오류 발생: {ex.Message}");
            }
        }

        #endregion


        #region Site 작업

        /// <summary>
        /// Site 관련 메시지 전송 및 작업 루프.
        /// </summary>
        public async Task SiteStaFunctionAsync(CancellationToken cancellationToken)
        {
            int humLogCounter = 0;

            //int PortCheckCnt = 0;

            while (!IsCloseCom)
            {
                try
                {
                    SiteLoopTime = DateTime.Now;

                    if (Global.gConfigVM == null)
                    {
                        await SafeDelayAsync(500, cancellationToken);
                        continue;
                    }

                    // "I" 메시지 전송
                    await ProcessSiteMessagesAsync("I", cancellationToken);

                    await SafeDelayAsync(1000, cancellationToken);

                    // "E" 메시지 전송
                    await ProcessSiteMessagesAsync("E", cancellationToken);

                    /*
                    if(PortCheckCnt >= 40)
                    {
                        if(Global.gConfigVM.PortCheckMode)
                            await ProcessSiteMessagesAsync("Port", cancellationToken); // Port Check
                        PortCheckCnt = 0;
                    }
                    */
                    // 습도 로그 기록 (5회 주기)
                    if (humLogCounter >= 5)
                    {
                        try
                        {
                            Global.gMainPageVM.addHumdityLog();
                        }
                        catch (Exception ex)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[SiteStaFunction] Error while adding humidity log: {ex.Message}");
                        }
                        humLogCounter = 0;
                    }
                    humLogCounter++;
                    //PortCheckCnt++;
                    cancellationToken.ThrowIfCancellationRequested();

                    // 루프 사이에 남은 시간 계산 및 딜레이 (1.6초 주기)
                    double remainingMs = (SiteLoopTime.AddSeconds(1.6) - DateTime.Now).TotalMilliseconds;
                    if (remainingMs > 0)
                    {
                        await SafeDelayAsync((int)remainingMs, cancellationToken);
                    }
                }
                catch (TaskCanceledException)
                {
                    Console.WriteLine("SiteStaFunction 작업이 취소되었습니다.");
                    break;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[SiteStaFunction] 예외 발생: {ex.Message}");
                }
            }
        }

        /// <summary>
        /// Site 메시지들을 비동기적으로 처리.
        /// </summary>
        private async Task ProcessSiteMessagesAsync(string message, CancellationToken cancellationToken)
        {
            var tasks = new List<Task>();

            foreach (SiteCom sc in Global.gMainPageVM.sc)
            {
                if (sc.SitePort == null)
                    continue;

                if (message != "Port")
                {
                    if (sc.isMotorStatus)
                    {
                        // 각 포트에 메시지 전송 작업 생성
                        tasks.Add(Task.Run(() =>
                        {
                            try
                            {
                                sc.SiteWirteMsg(message);
                            }
                            catch (Exception ex)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[SiteCom] Error while sending message '{message}': {ex.Message}");
                            }
                        }, cancellationToken));
                    }
                    else
                    {
                        Console.WriteLine("###### Motor Moving #########");
                    }
                }
                else if(message == "Port")
                {
                    sc.PortCheck();
                }
            }

            // 모든 작업 완료 대기 (개별 작업의 예외는 내부에서 처리)
            if (tasks.Any())
            {
                try
                {
                    await Task.WhenAll(tasks);
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[ProcessSiteMessages] Error while processing messages '{message}': {ex.Message}");
                }
            }
        }

        #endregion

        #region Capture 작업

        /// <summary>
        /// Capture 관련 비동기 작업 루프.
        /// </summary>
        public async Task CaptureFunctionAsync(CancellationToken cancellationToken)
        {
            int processSVCnt = 0;
            var semaphore = new SemaphoreSlim(6); // 병렬 작업 제한

            while (!IsCloseCom)
            {
                try
                {
                    cancellationToken.ThrowIfCancellationRequested();
                    CaptureLoopTime = DateTime.Now;

                    if (Global.gConfigVM == null)
                    {
                        await SafeDelayAsync(500, cancellationToken);
                        continue;
                    }

                    await SafeDelayAsync(100, cancellationToken);

                    var taskList = new List<Task>();

                    foreach (CaptureCom cc in Global.gMainPageVM.cc)
                    {
                        if (cc.CapturePort == null || !cc.CapturePort.IsOpen)
                            continue;

                        // 각 CaptureCom에 대한 비동기 작업 생성
                        taskList.Add(Task.Run(async () =>
                        {
                            await semaphore.WaitAsync(cancellationToken);
                            try
                            {
                                cancellationToken.ThrowIfCancellationRequested();
                                await cc.WriteAndProcessAsync(cancellationToken);
                            }
                            catch (OperationCanceledException)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[CAPTURE {cc.CaputureCh}] 작업이 취소되었습니다.");
                            }
                            catch (Exception ex)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[CAPTURE {cc.CaputureCh}] Write 중 예외 발생 : {ex.Message}.");
                            }
                            finally
                            {
                                semaphore.Release();
                            }
                        }, cancellationToken));
                    }

                    // 모든 Capture 작업 완료 대기 (개별 작업 내 예외 처리는 내부에서)
                    await Task.WhenAll(taskList);

                    cancellationToken.ThrowIfCancellationRequested();

                    // 주기적으로 박스 상태 처리
                    if (processSVCnt >= Global.gConfigVM.ProcessBoxSVCycle)
                    {
                        await ProcessBoxStates();
                        processSVCnt = 0;
                    }
                    processSVCnt++;

                    cancellationToken.ThrowIfCancellationRequested();

                    await SafeDelayAsync((int)(Global.gConfigVM.CaptureCycle * 1000), cancellationToken);

                    // 추가 딜레이: CaptureLoopTime 기준 남은 시간 (최대 2.5초까지)
                    TimeSpan diff = CaptureLoopTime.AddSeconds(2.5) - DateTime.Now;
                    if (diff.TotalMilliseconds > 0)
                    {
                        await SafeDelayAsync((int)diff.TotalMilliseconds, cancellationToken);
                    }
                }
                catch (TaskCanceledException)
                {
                    Console.WriteLine("CaptureFunction 작업이 취소되었습니다.");
                    break;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[CaptureFunction] 예외 발생: {ex.Message}");
                }
            }
        }

        //Random rd = new Random();
        /// <summary>
        /// 박스 상태 처리 (온도 제어 및 그래프 업데이트)
        /// </summary>
        

        /* Test Code
        Random random = new Random();
        private int _activeBoxCountForTest = 1;

        // 마지막으로 박스가 추가 활성화된 시간을 기록합니다.
        private DateTime _lastActivationTimeForTest = DateTime.Now;
        */
        private async Task ProcessBoxStates()
        {
            /*
            //Test Code
            if (Global.gConfigVM.GraphMode)
            {
                // 1. "2분이 지났는가?" 확인하고, 활성화할 박스 개수 업데이트
                // -----------------------------------------------------------------
                // 아직 모든 박스(12개로 가정)가 활성화되지 않았고, 마지막 활성화 시간으로부터 2분이 지났다면
                if (_activeBoxCountForTest < 12 && DateTime.Now >= _lastActivationTimeForTest.AddMinutes(2))
                {
                    _activeBoxCountForTest++; // 활성화할 박스 개수를 1 늘립니다.
                    _lastActivationTimeForTest = DateTime.Now; // 기준 시간을 현재로 갱신합니다.
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[GraphTest] Box {_activeBoxCountForTest} 활성화. (총 {_activeBoxCountForTest}개 동작 중)");
                }

                // 2. 현재 활성화된 개수만큼 박스를 순회하며 데이터 생성
                // -----------------------------------------------------------------
                var allBoxes = Global.gMainPageVM.Racks.SelectMany(r => r.Boxes).ToList();

                // 전체를 순회하는 'foreach' 대신, 활성화된 개수만큼만 'for' 루프를 실행합니다.
                for (int i = 0; i < _activeBoxCountForTest; i++)
                {
                    if (i >= allBoxes.Count) break; // 안전장치

                    BoxModel currentBox = allBoxes[i];

                    // 활성화된 박스에 대해서만 데이터 생성 및 그래프 업데이트 작업을 수행합니다.
                    currentBox.GetBoxPV(random.Next(61, 79));
                    await UpdateBoxGraphDataAsync(currentBox, isControl: false);
                }
            }
            */
            //Org
            foreach (var rm in Global.gMainPageVM.Racks)
            {
                foreach (var bm in rm.Boxes)
                {
                    //bm.GetBoxPV(random.Next(61, 79));
                    if (ShouldProcessBox(bm))
                    {
                        ProcessBoxTemperature(bm);
                    }
                    else if (Global.gConfigVM.GraphMode)
                    {
						UpdateBoxGraphData(bm, isControl: false);
                    }

                    // Tams 상태 확인 및 종료 처리
                    if (bm.BoxStat == Box_STAT.RUN && bm.IsBoxTams && bm.findEndFile())
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[B{bm.BoxViewIndex}] Tams End File Found");
                        bm.PreStopTest();
                    }
                }
            }
        }

        private bool ShouldProcessBox(BoxModel bm)
        {
            return (bm.BoxStat == Box_STAT.PRERUN && bm.PreRunControlTemp) || bm.BoxStat == Box_STAT.RUN;
        }
        private void ProcessBoxTemperature(BoxModel bm)
        {
            switch (Global.gConfigVM.ControlMode)
            {
                case (int)CONTROL_MODE.NORMAL:
                    bm.sampleingTempData();
                    bm.ProcessBoxSV_Logic_Org();
                    break;
                case (int)CONTROL_MODE.MMLOGIC:
                    bm.ProcessBoxSV();
                    break;
                case (int)CONTROL_MODE.ONLYLONG:
                    bm.onlylongsampleingTempData();
                    bm.ProcessBoxSV_Logic_Long();
                    break;
            }
			UpdateBoxGraphData(bm, isControl: true);
        }

		/// <summary>
		/// 박스 그래프 데이터 업데이트
		/// </summary>
		//private async Task UpdateBoxGraphDataAsync(BoxModel bm, bool isControl = false)
		private void UpdateBoxGraphData(BoxModel bm, bool isControl = false)
		{
            var tempData = bm.TempDatas[0];

            // 배열 범위 초과 방어 코드
            if (bm.GraphIndex >= tempData.GraphData.Length) return;

            // 데이터 업데이트
            tempData.GraphData[bm.GraphIndex] = tempData.Temp = bm.BoxPV;
            tempData.TimeData[bm.GraphIndex] = tempData.nowTime = DateTime.Now;

            if (isControl)
            {
                tempData.TempSV = bm.GetSV;
                tempData.TempPV = bm.GetPV;
            }

            /*
            // 첫 번째 데이터이면 그래프 초기화
            if (bm.GraphIndex == 0)
            {
                Global.gGraphVM.TempSigs.OffsetX = tempData.TimeData[0].ToOADate();
                Global.gGraphVM.TempGraph.Plot.XAxis.DateTimeFormat(true);
            }
            */

            bm.GraphIndex++;
            bm.addTempLog();

            // 그래프 업데이트 (UI 스레드에서 실행)
            if (Global.gMainVM.ShowGraph == Visibility.Visible && Global.gGraphVM.CurBox == bm.BoxIndex)
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gGraphVM.UpdateGraph(bm.GraphIndex, bm)));
            }
        }

        #endregion

        #region TCON 작업

        /// <summary>
        /// 온도조절기(TCON) 상태 처리 및 모니터링 루프
        /// </summary>
        public async Task TconStatFunctionAsync(CancellationToken cancellationToken)
        {
            // Global.gConfigVM가 로드될 때까지 대기
            while (!IsCloseCom)
            {
                if (Global.gConfigVM != null)
                    break;
                await SafeDelayAsync(500, cancellationToken);
            }

            // 초기 딜레이
            await SafeDelayAsync(300, cancellationToken);

            // 전체 온도조절기 상태 Read (각 박스별)
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                foreach (BoxModel bm in rm.Boxes)
                {
                    bm.Read_BoxTConStat();
                    await SafeDelayAsync(150, cancellationToken);
                }
            }

            await SafeDelayAsync(5 * 1000, cancellationToken);

            // 각 박스별 상태 재확인 및 초기 동작 설정
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                foreach (BoxModel bm in rm.Boxes)
                {
                    int retryCount = 0;
                    while (string.IsNullOrEmpty(Global.gEngineerVM.ManualMotor[rm.RackIndex].Tcons[bm.EachBoxIndex].EngrRackTconstat))
                    {
                        bm.Read_BoxTConStat();
                        await SafeDelayAsync(200, cancellationToken);
                        retryCount++;
                        if (retryCount >= 4)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[TCON {rm.RackViewIndex} - {bm.EachBoxIndex} ] Status Read Error. Check TCON System");
                            break;
                        }
                    }

                    if (bm.TconIsRun && rm.MotorStatText == "CLOSE")
                    {
                        if (!Global.gConfigVM.EngrMode)
                        {
                            if (!bm.setTamsTemp())
                            {
                                bm.BoxSetSV = bm.GetSV;
                                //Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{rm.RackViewIndex} - {bm.EachBoxIndex} ] Find Tams File OK. SetTemp {bm.BoxSetSV}");
                            }
                        }
                        else
                        {
							bm.SyncBoxSetSVFromTcon(bm.GetSV);
						}

						bm.TimeStart = DateTime.Now;

						bm.BoxStat = Box_STAT.RUN;
                        if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus == false)
                        {
                            Global.gAlarmCom.AlarmWrite("R");
                            Global.gAlarmCom.AllRackStatus = true;
                            Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.RUN;
                        }
                        rm.BoxRunCnt++;
                        Global.gMainPageVM.AllRackRunCnt++;

                        bm.isLoggingHumidity = true; // 습도 로깅 시작
                        bm.GraphIndex = 0;
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{bm.MasterRack.RackViewIndex} - {bm.BoxViewIndex}]bm.startTempLog() main TconStatFunctionAsync()");
                        bm.startTempLog();
                    }
                }
                rm.ShowBox = Visibility.Hidden;
                /*
				rm.showAlram(ALRAM_TYPE.ERROR, Global.msg_SenseError, false);
				foreach (BoxModel bm in rm.Boxes)
					bm.BoxStat = Box_STAT.ERROR;
				Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[All] Motor " + rm.RackViewIndex + " Down is Fail");
                */
			}

            int statChkCnt = 0;
            //bool testbool = false;
            // TCON 상태 모니터링 루프
            while (!IsCloseCom)
            {
                try
                {
                    Stopwatch sw = Stopwatch.StartNew();
					//testbool = !testbool;
					cancellationToken.ThrowIfCancellationRequested();

                    foreach (RackModel rm in Global.gMainPageVM.Racks)
                    {
						//rm.GetSlideOpen(!testbool);
                        foreach (BoxModel bm in rm.Boxes)
                        {
                            if (AllRackSTAT != TOTALRACK_STAT.PARAMETER)
                            {
                                // PRERUN, RUN, DEFROST 상태의 경우 TCON 상태 READ
                                if (bm.BoxStat == Box_STAT.PRERUN || bm.BoxStat == Box_STAT.RUN || bm.BoxStat == Box_STAT.DEFROST)
                                {
                                    bm.Read_BoxTConStat();
                                    await SafeDelayAsync(200, cancellationToken);
                                    // RUN 상태인데 TCON이 멈춘 경우 재시작 시도
                                    if (!bm.TconIsRun && bm.BoxStat == Box_STAT.RUN)
                                    {
                                        bm.TCon_BoxRun();
                                    }
                                }
                                else if (!Global.gConfigVM.EngrMode)
                                {
                                    // 다른 상태의 경우 일정 주기마다 TCON 상태 READ 후, 실행중이면 Stop
                                    if (statChkCnt >= 15)
                                    {
                                        bm.Read_BoxTConStat();
                                        await SafeDelayAsync(250, cancellationToken);
                                    }
                                    if (bm.IsBoxTCUnit && bm.TconIsRun)
                                    {
                                        bm.TCon_BoxStop();
                                        await SafeDelayAsync(100, cancellationToken);
                                        bm.Read_BoxTConStat();
                                        await SafeDelayAsync(100, cancellationToken);
                                    }
                                }
                            }
                            await SafeDelayAsync(90, cancellationToken);
                        }
                    }

                    statChkCnt = (statChkCnt >= 16) ? 0 : statChkCnt + 1;
                    sw.Stop();
                    if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                        Global.gEngineerVM.TConLoopTime = sw.Elapsed.ToString();
                }
                catch (TaskCanceledException)
                {
                    Console.WriteLine("TconStatFunction 작업이 취소되었습니다.");
                    break;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[TconStatFunction] 예외 발생: {ex.Message}");
                }
            }
        }

        #endregion

        #region Flow 작업

        /// <summary>
        /// Flow 상태 작업 및 로그 기록 루프
        /// </summary>
        public async Task FlowStatFunctionAsync(CancellationToken cancellationToken)
        {
            const int logInterval = 60;  // 로그 주기 (초)
            int flowLogCounter = 0;

            while (!IsCloseFlow)
            {
                try
                {
                    cancellationToken.ThrowIfCancellationRequested();

                    if (Global.gConfigVM == null)
                    {
                        await SafeDelayAsync(500, cancellationToken);
                        continue;
                    }

                    // 흐름 미터에 값 쓰기
                    await HandleFlowMeterWriteAsync();

                    // 로그 기록 주기 체크
                    if (flowLogCounter >= logInterval)
                    {
                        Global.gMainPageVM.addFlowLog();
                        flowLogCounter = 0;
                    }
                    flowLogCounter++;

                    await SafeDelayAsync(1000, cancellationToken);
                }
                catch (TaskCanceledException)
                {
                    Console.WriteLine("FlowStatFunction 작업이 취소되었습니다.");
                    break;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[FlowStatFunction] 예외 발생: {ex.Message}");
                }
            }
        }

        /// <summary>
        /// 흐름 미터 값을 기록하는 메서드.
        /// </summary>
        private async Task HandleFlowMeterWriteAsync()
        {
            try
            {
                await Task.Run(() => Global.gFlowMeterCom.FlowMeterWrite());
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"FlowMeterWrite error: {ex.Message}");
            }
        }

        #endregion

        #region 헬퍼 메서드

        /// <summary>
        /// cancellationToken을 사용하는 안전한 딜레이 메서드.
        /// </summary>
        private async Task SafeDelayAsync(int millisecondsDelay, CancellationToken cancellationToken)
        {
            try
            {
                await Task.Delay(millisecondsDelay, cancellationToken);
            }
            catch (TaskCanceledException)
            {
                // 딜레이가 취소된 경우 예외 무시
            }
        }

        #endregion
    }
}
