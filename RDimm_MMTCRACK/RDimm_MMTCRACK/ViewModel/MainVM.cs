using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.Net;
using RDimm_MMTCRACK.View;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using static HandyControl.Tools.Interop.InteropValues;
using System.Windows.Threading;

namespace RDimm_MMTCRACK.ViewModel
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
        
        [DoNotNotify] public int GraphIndex { get; set; } = 0;
        public TOTALRACK_STAT AllRackSTAT { get; set; } = TOTALRACK_STAT.RUN;

        public CancellationTokenSource _cancellationTokenSource;
        public Task _TconStatTask;
        public Task _SiteStatTask;
        public Task _FlowStatTask;
        public Task _CaptureStatTask;

        public DateTime SiteLoopTime { get; set; }
        public DateTime CaptureLoopTime { get; set; }

        public MainVM()
        {
            Global.gMainVM = this;

            //가비지 강제 처리 막아놈
            //_timer = new DispatcherTimer();
            //_timer.Interval = TimeSpan.FromMinutes(5); // 5분 간격
            //_timer.Tick += Timer_Tick; // 타이머 이벤트 핸들러 연결
            //_timer.Start(); // 타이머 시작


            tc = new TconCom();
            fmc = new FlowMeterCom();
            ac = new AlarmCom();
            changeView("MAIN");

            _cancellationTokenSource = new CancellationTokenSource();

            _TconStatTask = Task.Run(() => TconStatFunction(_cancellationTokenSource.Token));
            _SiteStatTask = Task.Run(() => SiteStaFunction(_cancellationTokenSource.Token));
            _FlowStatTask = Task.Run(() => FlowStatFunction(_cancellationTokenSource.Token));
            _CaptureStatTask = Task.Run(() => CaptureFunction(_cancellationTokenSource.Token));

            AllRackSTAT = TOTALRACK_STAT.STOP;
        }
        public async Task StopAllTasksAsync()
        {
            try
            {
                _cancellationTokenSource.Cancel();  // 취소 요청

                // 비동기 작업을 취소하고 완료될 때까지 대기
                await Task.WhenAll(_TconStatTask, _SiteStatTask, _FlowStatTask, _CaptureStatTask);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[OS] 비동기 작업 종료 중 오류 발생: {ex.Message}");
            }
        }
        private void Timer_Tick(object sender, EventArgs e)
        {
            //모든 세대의 가비지 수집
            GC.Collect();
            //0세대부터 (매개변수)세대까지 가비지 수집
            GC.Collect(1);
            //GCCollectionMode에 따라 즉시 실행 or 최적 타이밍에 실행
            GC.Collect(0, GCCollectionMode.Forced);
            GC.Collect(0, GCCollectionMode.Optimized);
            //blocking에 따라 차단 후 실행 or 백그라운드 실행
            GC.Collect(1, GCCollectionMode.Default, true);
            GC.Collect(1, GCCollectionMode.Default, false);
            //compacting에 따라 수집 후 compact 수행 여부 결정
            GC.Collect(2, GCCollectionMode.Default, true, true);
            GC.Collect(2, GCCollectionMode.Default, false, false);

            //메모리에 현재 할당된 바이트 수 반환(long)
            //forceFullCollection에 따라 반환 전에 가비지 수집
            GC.GetTotalMemory(true);
            GC.GetTotalMemory(false);



            GC.Collect(); //사전 메모리 정리
            /*
            가비지 생성량 측정할 코드
            여기에 작성
            */

            //실행 완료 후 메모리 확인
            long before = GC.GetTotalMemory(false);
            //Console.WriteLine(before);
            //가비지 컬렉팅 후 잠시 대기
            GC.Collect();
            Thread.Sleep(3000);
            //가비지 컬렉팅 후 메모리 확인
            long after = GC.GetTotalMemory(false);
            //Console.WriteLine(after);

            //Console.WriteLine($"발생한 가비지: {before - after}");
        }
        public void changeView(string name)
        {
            SelectName = name;
            ShowMain = Visibility.Hidden;
            ShowGraph = Visibility.Hidden;
            ShowFan = Visibility.Hidden;
            ShowPower = Visibility.Hidden;
            ShowEngineer = Visibility.Hidden;
            ShowConfig = Visibility.Hidden;

            if (name == VIEW_SEL.MAIN.ToString())
            {
                SelectViewName = "Main Control";
                SelectViewDiscription = "Operate The Chamber And Perform Test Functions";
                ShowMain = Visibility.Visible;
            }
            else if (name == VIEW_SEL.GRAPH.ToString())
            {
                SelectViewName = "Temperature";
                SelectViewDiscription = "Output Real-Time Temperature And Graph";
                ShowGraph = Visibility.Visible;
                Global.gDimmVM.setRack();

            }
            else if (name == VIEW_SEL.ENGINEER.ToString())
            {

                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Engineer Mode";
                    SelectViewDiscription = "Set up Register Manunaly And Excute Command";
                    ShowEngineer = Visibility.Visible;
                    //Global.gEnginerVM.setRack();
                }
            }
            else if (name == VIEW_SEL.CONFIG.ToString())
            {
                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Program Configration";
                    SelectViewDiscription = "Check and change the settings required for the Prgram";
                    ShowConfig = Visibility.Visible;
                }
            }
        }
        bool doPasswordDialog()
        {
            if (!Global.gConfigVM.IsUsePassword) return true;
            Global.gConfigVM.GetPassword = "";

            SelectViewName = "Password Page";
            SelectViewDiscription = "You'll need to enter a password to open this page";


            PassWordView pwDialog = new PassWordView();

            pwDialog.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            pwDialog.ShowDialog();
            if (pwDialog.DialogResult.Value && pwDialog.DialogResult.HasValue)
                return true;

            return false;
        }

        public void quit()
        {
            try
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[OS] MAIN UI CLICK CLOSE.");
                _isCloseCom = true;
                _isCloseFlow = true;
                // 작업 취소를 위한 토큰 소스 생성
                _cancellationTokenSource?.Cancel();

                // 비동기 작업이 종료될 때까지 대기 (각 작업에 대해 취소 요청 후)
                // 비동기 작업이 종료될 때까지 대기 및 예외 처리
                try
                {
                    Task.WhenAll(_TconStatTask, _SiteStatTask, _FlowStatTask, _CaptureStatTask)?.Wait();
                }
                catch (AggregateException ae)
                {
                    foreach (var ex in ae.InnerExceptions)
                    {
                        // 각 작업에서 발생한 예외를 처리
                        Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[OS] 비동기 작업 중 오류 발생: {ex.Message}");
                    }
                }

                // 종료 후 TCon 연결 해제
                tc.TConquit();

                // 장비 연결 해제
                Global.gTconCom.TConDisConnet();
                Global.gAlarmCom.AlarmDisConnect();
                Global.gFlowMeterCom.FlowMeterDiConnect();

                // 사이트 및 캡처 연결 해제
                for (int i = 0; i < 6; i++)
                {
                    Global.gMainPageVM.sc[i].SiteDisConnect();
                    Global.gMainPageVM.cc[i].CaptureDisConnect();
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[OS] 종료 중 오류 발생: {ex.Message}");
            }
        }
        public bool _isCloseCom = false;
        public bool _isCloseFlow = false;

        #region Site 작업
        public async Task SiteStaFunction(CancellationToken cancellationToken)
        {
            int humlogcnt = 0;
            while (!_isCloseCom)
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

                    if (humlogcnt >= 5)
                    {
                        try
                        {
                            Global.gMainPageVM.addHumdityLog();
                        }
                        catch (Exception ex)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[SiteStaFunction] Error while adding humidity log: {ex.Message}");
                        }
                        humlogcnt = 0;
                    }

                    humlogcnt++;

                    // 취소 요청이 들어오면 루프 종료
                    cancellationToken.ThrowIfCancellationRequested();

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
                //if (sc.SitePort == null || !sc.SiteinitStat)
                if (sc.SitePort == null)
                {
                    //await Task.Delay(500);
                    continue;
                }

                if (!sc.SitePort.IsOpen)
                {
                    //await Task.Delay(500);
                    continue;
                }
                if (message != "Port")
                {
                    if (sc.isMotorStatus)
                    {
                        // 각 포트에 메시지 전송 작업 생성
                        tasks.Add(Task.Run(async () =>
                        {
                            try
                            {
                                await sc.SiteWirteMsg(message);
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
        public async Task CaptureFunction(CancellationToken cancellationToken)
        {
            var taskList = new List<Task>();
            var semaphore = new SemaphoreSlim(5); // 병렬 작업 제한

            while (!_isCloseCom)
            {
                // 취소 요청이 있을 경우 종료
                cancellationToken.ThrowIfCancellationRequested();

                if (Global.gConfigVM == null)
                {
                    await Task.Delay(500, cancellationToken);
                    continue;
                }
                await Task.Delay(100, cancellationToken);

                foreach (CaptureCom cc in Global.gMainPageVM.cc)
                {
                    if (cc.CapturePort == null || !cc.CapturePort.IsOpen)
                        continue;

                    var isPortOpen = cc.CapturePort.IsOpen;
                    taskList.Add(Task.Run(async () =>
                    {
                        await semaphore.WaitAsync(cancellationToken); // 병렬 제한
                        try
                        {
                            cancellationToken.ThrowIfCancellationRequested();
                            if (isPortOpen)
                                cc.CaptureWrite();
                        }
                        catch (OperationCanceledException)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[CAPTURE {cc.CaputureCh} ] 작업이 취소되었습니다.");
                        }
                        catch (Exception ex)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[CAPTURE {cc.CaputureCh} ] Write 중 예외 발생 : {ex.Message}.");
                        }
                        finally
                        {
                            semaphore.Release();
                        }
                    }, cancellationToken));
                }

                try
                {
                    await Task.WhenAll(taskList.Select(async task =>
                    {
                        try
                        {
                            await task;
                        }
                        catch (Exception ex)
                        {
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CaptureFunction 작업 중 예외 발생 : {ex.Message}.");
                        }
                    }));
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CaptureFunction 전체 실행 중 예외 발생 : {ex.Message}.");
                }
                finally
                {
                    taskList.Clear();
                }

                cancellationToken.ThrowIfCancellationRequested();

                await Task.Delay(Global.gConfigVM.CaptureCycle * 1000, cancellationToken);
            }
        }

        public async Task TconStatFunction(CancellationToken cancellationToken)
        {
            while (!_isCloseCom)
            {
                if (Global.gConfigVM != null)
                {
                    break;
                }
                await Task.Delay(500, cancellationToken); // Using cancellation token
            }

            await Task.Delay(300, cancellationToken); // Using cancellation token

            // Process each Rack and CBox model
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                foreach (CBoxModel bm in rm.CBoxs)
                {
                    bm.Read_BoxTConStat();
                    await Task.Delay(150, cancellationToken); // Using cancellation token
                }
            }

            await Task.Delay(5 * 1000, cancellationToken); // Using cancellation token
                                                           // Stat Check 하여 마지막 동작으로 복귀
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                if (!Global.gConfigVM.EngrMode)
                    rm.setRackTamsTemp();

                foreach (CBoxModel bm in rm.CBoxs)
                {
                    int SRetryCnt = 0;
                    while (true)
                    {
                        if (Global.gEngineerVM.ManualMotor[rm.RackIndex].Tcons[bm.BoxIndex].EngrRackTconstat != string.Empty)
                        {
                            break;
                        }
                        else
                        {
                            //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[TCON {rm.RackViewIndex} - {bm.BoxViewIndex} ] Retry Status Read.");
                            bm.Read_BoxTConStat();
                            await Task.Delay(200, cancellationToken); // Using cancellation token
                            SRetryCnt++;
                            if (SRetryCnt >= 4)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[TCON {rm.RackViewIndex} - {bm.BoxViewIndex} ] Status Read Error. Check TCON System");
                                break;
                            }
                        }
                    }

                    if (bm.TconIsRun && Global.gEngineerVM.ManualMotor[rm.RackIndex].Tcons[bm.BoxIndex].EngrRackTconstat == "RUN" && rm.MotorStatText == "CLOSE")
                    {
                        if (!Global.gConfigVM.EngrMode)
                        {
                            bm.BoxSetSV = rm.RackSetSV;
                        }
                        else
                        {
                            if (bm.TargetSV == 0)
                                bm.TargetSV = 5;
                            bm.BoxSetSV = bm.TargetSV;
                        }

                        bm.SVST(SVC.TCON);
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[Fix {rm.RackViewIndex} - {bm.BoxViewIndex} ] Temp Set {bm.BoxSetSV}");

                        bm.CBoxStat = CBOX_STAT.RUN;
                        if (rm.RackStat != RACK_STAT.RUN)
                            rm.RackStat = RACK_STAT.RUN;

                        if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus == false)
                        {
                            Global.gAlarmCom.AlarmWrite("R"); // 경광등 RUN
                            Global.gAlarmCom.AllRackStatus = true;
                            Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.RUN;
                        }

                        Global.gMainPageVM.AllRackRunCnt++;
                        bm.humidityWarningCnt = 0;
                        bm.isLoggingHumidity = true; // 습도 Logging Start
                        if (bm.BoxIndex == 0)
                        {
                            rm.GraphIndex = 0;
                            rm.startTempLog();
                        }
                    }

                    if (rm.RackMsg == Global.msg_InitRack)
                        rm.ShowBox = Visibility.Hidden;

					//Test코드
					//rm.showAlram(ALRAM_TYPE.ERROR, Global.msg_SenseError, false);
				}
			}

            int StatchkCnt = 0;
			//bool testbool = false;
			while (!_isCloseCom)
            {
				// Exit if cancellation is requested
				//testbool = !testbool;
				cancellationToken.ThrowIfCancellationRequested();

                foreach (RackModel rm in Global.gMainPageVM.Racks)
                {
                    foreach (CBoxModel bm in rm.CBoxs)
                    {
						//rm.GetSlideOpen(!testbool);
						if (AllRackSTAT != TOTALRACK_STAT.PARAMETER)
                        {
                            // box Stat가 Run인 상황 PRERUN, RUN, DEFROST 에만 TCON STATUS READ
                            if (bm.CBoxStat == CBOX_STAT.PRERUN || bm.CBoxStat == CBOX_STAT.RUN || bm.CBoxStat == CBOX_STAT.DEFROST)
                            {
                                bm.Read_BoxTConStat();
                                await Task.Delay(200, cancellationToken); // Using cancellation token
                                if (bm.TconIsRun == false && bm.CBoxStat == CBOX_STAT.RUN) // Run일 때만 온도조절기 Stop 시 RUN하게
                                {
                                    bm.TCon_BoxRun();
                                }
                            }
                            else
                            {
                                // Engr mode 시에는 정지 안함
                                if (!Global.gConfigVM.EngrMode)
                                {
                                    if (StatchkCnt >= 15)
                                    {
                                        bm.Read_BoxTConStat();
                                        await Task.Delay(250, cancellationToken); // Using cancellation token
                                    }
                                    if (bm.TconIsRun)
                                    {
                                        bm.TCon_BoxStop();
                                        await Task.Delay(100, cancellationToken); // Using cancellation token
                                        bm.Read_BoxTConStat();
                                        await Task.Delay(100, cancellationToken); // Using cancellation token
                                    }
                                }
                            }
                        }
                        await Task.Delay(90, cancellationToken); // Using cancellation token
                    }
                }

                StatchkCnt++;
                if (StatchkCnt >= 16)
                {
                    StatchkCnt = 0;
                }
            }
        }
        public async Task FlowStatFunction(CancellationToken cancellationToken)
        {
            const int logInterval = 60;  // 로그 주기 (초 단위)
            int flowlogcnt = 0;

            while (!_isCloseFlow)
            {
                // Exit gracefully if cancellation is requested
                cancellationToken.ThrowIfCancellationRequested();

                if (Global.gConfigVM == null || Global.gFlowMeterCom.FlowMeterPort == null)
                {
                    await Task.Delay(500, cancellationToken);  // 설정이 로드되지 않았으면 잠시 대기
                    continue;
                }
                if (!Global.gFlowMeterCom.FlowMeterPort.IsOpen)
                {
                    await Task.Delay(500, cancellationToken);
                    continue;
                }

                // 흐름 미터에 값 쓰기
                await HandleFlowMeterWrite();

                // 로그 주기 체크
                if (flowlogcnt >= logInterval)
                {
                    Global.gMainPageVM.addFlowLog();  // 로그 추가
                    flowlogcnt = 0;
                }

                flowlogcnt++;
                await Task.Delay(1000, cancellationToken);  // 1초 대기
            }
        }

        // FlowMeter 쓰기 처리
        private async Task HandleFlowMeterWrite()
        {
            try
            {
                // 비동기적으로 흐름 미터 값을 기록 (여기서는 비동기 작업이 아니지만, 실제 비동기 처리가 필요할 수 있습니다)
                await Task.Run(() => Global.gFlowMeterCom.FlowMeterWrite());
            }
            catch (Exception ex)
            {
                // 예외 처리: 흐름 미터 기록 중 오류가 발생한 경우
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"FlowMeterWrite error: {ex.Message}");
            }
        }
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
