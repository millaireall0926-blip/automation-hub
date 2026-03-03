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

        public MainVM()
        {
            Global.gMainVM = this;
            tc = new TconCom();
            fmc = new FlowMeterCom();
            ac = new AlarmCom();
            
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
                Global.gMainWindow.ChagneLottie(name);
            }
            else if (name == VIEW_SEL.GRAPH.ToString())
            {
                SelectViewName = "Temperature";
                SelectViewDiscription = "Output Real-Time Temperature And Graph";
                ShowGraph = Visibility.Visible;
                Global.gGraphVM.SetBox();
                Global.gMainWindow.ChagneLottie(name);

            }
            else if (name == VIEW_SEL.ENGINEER.ToString())
            {

                bool show = doPasswordDialog();
                if (show)
                {
                    SelectViewName = "Engineer Mode";
                    SelectViewDiscription = "Set up Register Manunaly And Excute Command";
                    ShowEngineer = Visibility.Visible;
                    Global.gMainWindow.ChagneLottie(name);
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
                    Global.gMainWindow.ChagneLottie(name);
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

        public async void quit()
        {
            try
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[OS] MAIN UI CLICK CLOSE.");
                _isCloseCom = true;
                _isCloseFlow = true;

                tc.TConquit();

                // 장비 연결 해제
                Global.gTconCom.TConDisConnet(true);
                Global.gAlarmCom.AlarmDisConnect();
                Global.gFlowMeterCom.FlowMeterDiConnect();

                // 사이트 및 캡처 연결 해제
                for (int i = 0; i < 6; i++)
                {
                    Global.gMainPageVM.sc[i].SiteDisConnect();
                    Global.gMainPageVM.cc[i].Disconnect();
                }

                // 작업 취소를 위한 토큰 소스 생성
                _cancellationTokenSource?.Cancel();

                // 비동기 작업이 종료될 때까지 대기 (각 작업에 대해 취소 요청 후)
                //Task.WhenAll(_TconStatTask, _SiteStatTask, _FlowStatTask, _CaptureStatTask).Wait();
                //_CaptureStatTask?.Wait();

                await StopAllTasksAsync();

                // 종료 후 TCon 연결 해제
                
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[OS] 종료 중 오류 발생: {ex.Message}");
            }
        }
        public bool _isCloseCom = false;
        public bool _isCloseFlow = false;
        public DateTime SiteLoopTime { get; set; }
        public DateTime CaptureLoopTime { get; set; }
        //Task SitePool;
        public async void SiteStaFunction(CancellationToken cancellationToken)
        {
            int humLogCounter = 0;
            int fwCheckCnt = 0;

            while (!_isCloseCom)
            {
                try
                {
                    //Stopwatch sw = new Stopwatch();
                    //sw.Start();

                    SiteLoopTime = DateTime.Now;

                    if (Global.gConfigVM == null)
                    {
                        await Task.Delay(500, cancellationToken);
                        continue;
                    }

                    //await Task.Delay(100, cancellationToken);

                    // "I" 메시지 전송 작업 실행
                    await ProcessSiteMessages("I", cancellationToken);

                    await Task.Delay(1000, cancellationToken);

                    // "E" 메시지 전송 작업 실행
                    await ProcessSiteMessages("E", cancellationToken);

                    //await Task.Delay(500, cancellationToken);

                    // 습도 로그 기록
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

                    // 작업 취소 요청 확인
                    cancellationToken.ThrowIfCancellationRequested();


                    if (SiteLoopTime.AddSeconds(1.6) > DateTime.Now)
                    {
                        // 정확한 시간 차이를 밀리초 단위로 계산
                        double remainingMs = (SiteLoopTime.AddSeconds(1.6) - DateTime.Now).TotalMilliseconds;

                        // 남은 시간만큼 대기
                        if (remainingMs > 0) // 음수 방지
                        {
                            await Task.Delay((int)remainingMs, cancellationToken);
                        }
                    }

                    //if (sw.IsRunning)
                    //{
                    //    sw.Stop();
                    //
                    //    Console.WriteLine($"Looping Time = {sw.Elapsed.ToString()}");
                    //    //if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                    //    //    Global.gEngineerVM.SiteLoopTime = sw.Elapsed.ToString();
                    //}
                }
                catch (TaskCanceledException)
                {
                    // 취소된 경우 처리
                    Console.WriteLine("작업이 취소되었습니다.");
                    break; // 루프 종료
                }
                catch (Exception ex)
                {
                    // 일반적인 예외 처리
                    Console.WriteLine($"예외 발생: {ex.Message}");
                }
            }
        }

        // Site 메시지 처리 로직을 메서드로 분리
        private async Task ProcessSiteMessages(string message, CancellationToken cancellationToken)
        {
            var tasks = new List<Task>();

            foreach (SiteCom sc in Global.gMainPageVM.sc)
            {
                if (sc.SitePort == null) return;

                //Console.WriteLine($"Main 1. {sc.SiteCh + 1}  SitePort.IsOpen = {sc.SitePort.IsOpen}");
                /*
                if (sc.isMotorStatus)
                {
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
                    //Console.WriteLine($"Main 2. {sc.SiteCh + 1}  SitePort.IsOpen = {sc.SitePort.IsOpen}");
                }
                */
                if (sc.isMotorStatus)
                {
                    //if (Global.gMainPageVM.Racks[sc.SiteCh].CheckRackState(message))
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
                    //Console.WriteLine($"Main 2. {sc.SiteCh + 1}  SitePort.IsOpen = {sc.SitePort.IsOpen}");
                }
                else
                {
                    Console.WriteLine("###### Motor Moving #########");
                    //Console.WriteLine($"Main 3. {sc.SiteCh + 1}  SitePort.IsOpen = {sc.SitePort.IsOpen}");
                }
            }

            // 모든 작업 완료 대기
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


        public async void CaptureFunction(CancellationToken cancellationToken)
        {
            int ProcessSVCnt = 0;
            var taskList = new List<Task>();
            //var semaphore = new SemaphoreSlim(5); // 병렬 작업 제한

            while (!_isCloseCom)
            {
                try
                {
                    //Stopwatch sw = new Stopwatch();
                    //sw.Start();

                    cancellationToken.ThrowIfCancellationRequested();

                    CaptureLoopTime = DateTime.Now;

                    if (Global.gConfigVM == null)
                    {
                        await Task.Delay(500, cancellationToken);
                        continue;
                    }

                    await Task.Delay(100, cancellationToken);

                    foreach (CaptureCom cc in Global.gMainPageVM.cc)
                    {
                        if ((cc.CapturePort == null || !cc.CapturePort.IsOpen) && cc.CaputurePortName != "")
                        {
                            cc.Connect(cc.CaputureCh);
                            continue;
                        }
                        taskList.Add(Task.Run(async () =>
                        {
                            //await semaphore.WaitAsync(cancellationToken); // 병렬 제한
                            try
                            {
                                cancellationToken.ThrowIfCancellationRequested();
                                if (cc.CapturePort != null)
                                {
                                    if (cc.CapturePort.IsOpen)
                                        await cc.WriteAndProcessAsync(cancellationToken);
                                }
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
                                //semaphore.Release();
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

                    if (ProcessSVCnt >= Global.gConfigVM.ProcessBoxSVCycle)
                    {
                        ProcessBoxStates();

                        ProcessSVCnt = 0;
                    }
                    ProcessSVCnt++;

                    cancellationToken.ThrowIfCancellationRequested();

                    await Task.Delay((int)(Global.gConfigVM.CaptureCycle * 1000), cancellationToken);

                    if (CaptureLoopTime.AddSeconds(3) > DateTime.Now)
                    {
                        TimeSpan diff = CaptureLoopTime.AddSeconds(2.5) - DateTime.Now;

                        cancellationToken.ThrowIfCancellationRequested();

                        await Task.Delay(Math.Abs((int)diff.TotalMilliseconds), cancellationToken);
                    }

                    /*
                    sw.Stop();
                    Global.gEngineerVM.CaptureLoopTime = sw.Elapsed.ToString();
                    */
                    /*
                    if (ProcessSVCnt >= Global.gConfigVM.ProcessBoxSVCycle)
                    {
                        foreach (RackModel rm in Global.gMainPageVM.Racks)
                        {
                            foreach (BoxModel bm in rm.Boxes)
                            {
                                //Test 용 랜덤데이터
                                //bm.GetBoxPV(random.Next(61, 79));

                                //PreRun 모니터링 시간에 제어 추가
                                if ((bm.BoxStat == Box_STAT.PRERUN && bm.PreRunControlTemp) || (bm.BoxStat == Box_STAT.RUN))
                                {
                                    if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.NORMAL)
                                    {
                                        bm.sampleingTempData();
                                        bm.ProcessBoxSV_Logic_Org();
                                    }
                                    else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.MMLOGIC)
                                    {
                                        bm.ProcessBoxSV();
                                    }
                                    else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.ONLYLONG)
                                    {
                                        bm.onlylongsampleingTempData();
                                        bm.ProcessBoxSV_Logic_Long();
                                    }

                                    bm.TempDatas[0].GraphData[bm.GraphIndex] = bm.TempDatas[0].Temp = bm.BoxPV;
                                    bm.TempDatas[0].TimeData[bm.GraphIndex] = bm.TempDatas[0].nowTime = DateTime.Now;
                                    bm.TempDatas[0].TempSV = bm.GetSV;
                                    bm.TempDatas[0].TempPV = bm.GetPV;

                                    if (bm.GraphIndex == 0)
                                    {
                                        Global.gGraphVM.TempSigs.OffsetX = bm.TempDatas[0].TimeData[0].ToOADate();
                                        Global.gGraphVM.TempGraph.Plot.XAxis.DateTimeFormat(true);
                                    }

                                    bm.GraphIndex++;

                                    bm.addTempLog();

                                    int gindex = bm.GraphIndex;
                                    if (Global.gMainVM.ShowGraph == Visibility.Visible && Global.gGraphVM.CurBox == bm.BoxIndex)
                                        await System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gGraphVM.UpdateGraph(gindex, bm)));
                                }
                                else
                                {
                                    if (Global.gConfigVM.GraphMode)
                                    {
                                        bm.TempDatas[0].GraphData[bm.GraphIndex] = bm.TempDatas[0].Temp = bm.BoxPV;
                                        bm.TempDatas[0].TimeData[bm.GraphIndex] =  bm.TempDatas[0].nowTime = DateTime.Now;

                                        if (bm.GraphIndex == 0)
                                        {
                                            Global.gGraphVM.TempSigs.OffsetX = bm.TempDatas[0].TimeData[0].ToOADate();
                                            Global.gGraphVM.TempGraph.Plot.XAxis.DateTimeFormat(true);
                                        }

                                        bm.GraphIndex++;

                                        bm.addTempLog();

                                        int gindex = bm.GraphIndex;
                                        if (Global.gMainVM.ShowGraph == Visibility.Visible && Global.gGraphVM.CurBox == bm.BoxIndex)
                                            await System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gGraphVM.UpdateGraph(gindex, bm)));
                                    }
                                }

                                if (bm.BoxStat == Box_STAT.RUN && bm.ISBoxTams)
                                {
                                    if (bm.findEndFile())
                                    {
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + bm.BoxViewIndex + "] Tams End File Find");
                                        bm.PreStopTest();
                                    }
                                }
                            }
                        }
                        ProcessSVCnt = 0;
                    }

                    ProcessSVCnt++;

                    await Task.Delay((int)(Global.gConfigVM.CaptureCycle *1000), cancellationToken);

                    if (CaptureLoopTime.AddSeconds(3) > DateTime.Now)
                    {
                        TimeSpan diif = CaptureLoopTime.AddSeconds(2.5) - DateTime.Now;
                        int diffsec = Math.Abs(diif.Seconds);

                        await Task.Delay(diffsec * 1000, cancellationToken);
                    }

                    if (sw.IsRunning)
                    {
                        sw.Stop();
                        //if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                            Global.gEngineerVM.CaptureLoopTime = sw.Elapsed.ToString();
                    }
                    */
                }
                catch (TaskCanceledException)
                {
                    // 취소된 경우 처리
                    Console.WriteLine("작업이 취소되었습니다.");
                    break; // 루프 종료
                }
                catch (Exception ex)
                {
                    // 일반적인 예외 처리
                    Console.WriteLine($"예외 발생: {ex.Message}");
                }
            }
        }
        //Random rd = new Random();
        private async void ProcessBoxStates()
        {
            foreach (var rm in Global.gMainPageVM.Racks)
            {
                foreach (var bm in rm.Boxes)
                {
                    //bm.GetBoxPV(rd.Next(50, 80));
                    if (ShouldProcessBox(bm))
                    {
                        ProcessBoxTemperature(bm);
                    }
                    else if (Global.gConfigVM.GraphMode)
                    {
                        await UpdateBoxGraphDataAsync(bm , isControl: false);
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

        private async void ProcessBoxTemperature(BoxModel bm)
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

            await UpdateBoxGraphDataAsync(bm, isControl: true);
        }

        private async Task UpdateBoxGraphDataAsync(BoxModel bm, bool isControl = false)
        {
            var tempData = bm.TempDatas[0];

            // 공통 데이터 업데이트
            tempData.GraphData[bm.GraphIndex] = tempData.Temp = bm.BoxPV;
            tempData.TimeData[bm.GraphIndex] = tempData.nowTime = DateTime.Now;

            // 제어 데이터 포함 여부
            if (isControl)
            {
                tempData.TempSV = bm.GetSV;
                tempData.TempPV = bm.GetPV;
            }

            // 첫 번째 데이터인 경우 그래프 초기화
            if (bm.GraphIndex == 0)
            {
                Global.gGraphVM.TempSigs.OffsetX = tempData.TimeData[0].ToOADate();
                Global.gGraphVM.TempGraph.Plot.XAxis.DateTimeFormat(true);
            }

            bm.GraphIndex++;
            bm.addTempLog();

            if (Global.gMainVM.ShowGraph == Visibility.Visible && Global.gGraphVM.CurBox == bm.BoxIndex)
                await System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => Global.gGraphVM.UpdateGraph(bm.GraphIndex, bm)));
        }

        public async void TconStatFunction(CancellationToken cancellationToken)
        {
            while (!_isCloseCom)
            {
                if (Global.gConfigVM != null)
                {
                    break;
                }
                //await Task.Delay(500, cancellationToken); // Using cancellation token
                await Task.Delay(500);
            }

            //await Task.Delay(300, cancellationToken); // Using cancellation token
            await Task.Delay(300);

            //전체 온도조절기 상태 Read
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                foreach (BoxModel bm in rm.Boxes)
                {
                    bm.Read_BoxTConStat();
                    //await Task.Delay(150, cancellationToken); // Using cancellation token
                    await Task.Delay(150);
                }
            }

            //await Task.Delay(5 * 1000, cancellationToken); // Using cancellation token
            await Task.Delay(5 * 1000); // Using cancellation token

            //Stat Check 하여 마지막 동작으로 복귀
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                foreach (BoxModel bm in rm.Boxes)
                {
                    int SRetryCnt = 0;
                    while (true)
                    {
                        if (Global.gEngineerVM.ManualMotor[rm.RackIndex].Tcons[bm.EachBoxIndex].EngrRackTconstat == string.Empty)
                        {
                            //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[TCON {rm.RackViewIndex} - {bm.EachBoxIndex} ] Retry Status Read.");
                            
                            bm.Read_BoxTConStat();

                            //await Task.Delay(200, cancellationToken); // Using cancellation token
                            await Task.Delay(200);


                            SRetryCnt++;
                            
                            if (SRetryCnt >= 4)
                            {
                                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[TCON {rm.RackViewIndex} - {bm.EachBoxIndex} ] Status Read Error. Check TCON System");
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (bm.TconIsRun && rm.MotorStatText == "CLOSE")
                    {
                        if (!Global.gConfigVM.EngrMode)
                        {
                            if(!bm.setTamsTemp())
                                bm.BoxSetSV = bm.GetSV;
                        }       
                        else
                            bm.BoxSetSV = bm.GetSV;

                        bm.BoxStat = Box_STAT.RUN;
                        if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus == false)
                        {
                            Global.gAlarmCom.AlarmWrite("R"); //경광등 RUN
                            Global.gAlarmCom.AllRackStatus = true;
                            Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.RUN;
                        }
                        rm.BoxRunCnt++;
                        Global.gMainPageVM.AllRackRunCnt++;

                        bm.isLoggingHumidity = true; //습도 Logging Start
                        bm.GraphIndex = 0;
                        bm.startTempLog();
                    }
                }
                rm.ShowBox = Visibility.Hidden;
            }

            int StatchkCnt = 0;
            while (!_isCloseCom)
            {
                try
                {
                    Stopwatch sw = new Stopwatch();
                    sw.Start();

                    // Exit if cancellation is requested
                    cancellationToken.ThrowIfCancellationRequested();

                    foreach (RackModel rm in Global.gMainPageVM.Racks)
                    {
                        foreach (BoxModel bm in rm.Boxes)
                        {
                            if (AllRackSTAT != TOTALRACK_STAT.PARAMETER)
                            {
                                //box Stat 가 Run 인 상황 PRERUN , RUN , DEFROST 에만 TCON STATUS READ 
                                //다른 Stat 에는 Tcon STOP
                                if (bm.BoxStat == Box_STAT.PRERUN || bm.BoxStat == Box_STAT.RUN || bm.BoxStat == Box_STAT.DEFROST)
                                {
                                    bm.Read_BoxTConStat();
                                    await Task.Delay(200, cancellationToken); // Using cancellation token
                                    if (bm.TconIsRun == false && bm.BoxStat == Box_STAT.RUN) // Run일 때만 온도조절기 Stop 시 RUN하게
                                    {
                                        bm.TCon_BoxRun();
                                    }
                                }
                                else
                                {
                                    //Engr mode 시에는 정지 안함
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
                    { StatchkCnt = 0; }

                    if (sw.IsRunning)
                    {
                        sw.Stop();
                        if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                            Global.gEngineerVM.TConLoopTime = sw.Elapsed.ToString();
                    }
                }
                catch (TaskCanceledException)
                {
                    // 취소된 경우 처리
                    Console.WriteLine("작업이 취소되었습니다.");
                    break; // 루프 종료
                }
                catch (Exception ex)
                {
                    // 일반적인 예외 처리
                    Console.WriteLine($"예외 발생: {ex.Message}");
                }
            }
        }
        public async void FlowStatFunction(CancellationToken cancellationToken)
        {
            const int logInterval = 60;  // 로그 주기 (초 단위)
            int flowlogcnt = 0;

            while (!_isCloseFlow)
            {
                try
                {
                    // Exit gracefully if cancellation is requested
                    cancellationToken.ThrowIfCancellationRequested();

                    if (Global.gConfigVM == null)
                    {
                        await Task.Delay(500, cancellationToken);  // 설정이 로드되지 않았으면 잠시 대기
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
                catch (TaskCanceledException)
                {
                    // 취소된 경우 처리
                    Console.WriteLine("작업이 취소되었습니다.");
                    break; // 루프 종료
                }
                catch (Exception ex)
                {
                    // 일반적인 예외 처리
                    Console.WriteLine($"예외 발생: {ex.Message}");
                }
            }
            /*
            const int logInterval = 60;  // 로그 주기 (초 단위)
            int flowlogcnt = 0;

            while (!_isCloseFlow)
            {
                // Exit gracefully if cancellation is requested
                cancellationToken.ThrowIfCancellationRequested();

                if (Global.gConfigVM == null)
                {
                    await Task.Delay(500, cancellationToken);  // 설정이 로드되지 않았으면 잠시 대기
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
            */
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
    }
}
