using LP_MMTCRack.Base;
using LP_MMTCRack.Net;
using LP_MMTCRack.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace LP_MMTCRack
{
    public class Global
    {
        static public MainWindow gMainWindow;
        static public MainVM gMainVM;
        static public MainPageVM gMainPageVM;
        static public ConfigVM gConfigVM;
        static public EngineerVM gEngineerVM;
        static public GraphVM gGraphVM;

        static public TconCom gTconCom;
        static public SiteCom gSiteCom;
        static public FlowMeterCom gFlowMeterCom;
        static public AlarmCom gAlarmCom;
        static public CaptureCom gCaptureCom;

        static public int BimetalErrorCnt = 10;

        static public bool SiteonoffEnable = false;

        public static SolidColorBrush[] gRackColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Transparent),      // Normal
            new SolidColorBrush(Colors.LightPink),            // as mode            
            //new SolidColorBrush(Colors.Gray)           // tcunit 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x3D3837)),
        };
        public static SolidColorBrush[] gBoxColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Transparent),           // Normal
            new SolidColorBrush(Colors.LightPink),          // as mode
           // new SolidColorBrush(Colors.Gray)           // tcunit 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x3D3837)),
        };
        public static SolidColorBrush[] gBoarderColors = new SolidColorBrush[]
        {
            // RACK_STAT Index
            new SolidColorBrush(Colors.AntiqueWhite),  // IDEL            
            new SolidColorBrush(Colors.AntiqueWhite),  // STOP
            new SolidColorBrush(Colors.PaleGreen),     // PERV_RUN
            new SolidColorBrush(Colors.Lime),          // RUN
            new SolidColorBrush(Colors.DodgerBlue),    // Done
            new SolidColorBrush(Colors.Aquamarine),        //DEFOSET 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)),       // ERROR
            new SolidColorBrush(Colors.LightGray),  // NOCON
            new SolidColorBrush(Colors.Red)  // INSPECTION -> as mode
        };
        public static SolidColorBrush[] gWarningColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Black),
            new SolidColorBrush(Colors.Red)
        };
        public static SolidColorBrush[] gMotorStatColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Plum),    // motor close
            new SolidColorBrush(Colors.Yellow)   // motor open
        };
        public static SolidColorBrush[] gAlramColors = new SolidColorBrush[]
        {
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0x6D79FF)), // Done
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(Colors.LightPink),
            new SolidColorBrush(MConv.IntToMediaColor(0xDD, 0xD3D3D3))
            /*
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFFC18C)), 
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79)), // WARAN
            new SolidColorBrush(MConv.IntToMediaColor(0xFF, 0xFF5B79))  // ERROR
            */
        };

        public static Color[] GraphColors = new Color[] {
                Colors.Maroon,
                Colors.Coral,
                Colors.DeepPink,
                Colors.DarkOrchid,
                Colors.Teal,

                Colors.Indigo,
                Colors.DarkRed,
                Colors.PowderBlue,

                Colors.LimeGreen,
                Colors.Tomato,
                Colors.Peru,
                Colors.Plum,
                Colors.Gold,

                Colors.DarkKhaki,
                Colors.Sienna,
                Colors.Purple,
                Colors.Khaki,
                Colors.BurlyWood,

                /*
                Colors.Maroon,
                Colors.MediumPurple,
                Colors.Brown,
                Colors.DarkOrchid,
                Colors.Teal,

                Colors.Indigo,
                Colors.DarkSlateBlue,
                Colors.PowderBlue,
                */
                };

        public static string msg_TestCompleted = "테스트 완료";
        public static string msg_TamsTestCompleted = "TAMS 테스트 완료";
        public static string msg_Unbalance = "온도동작 이상감지";
        public static string msg_Cbox_25_temp = "온도 25도이하";
        public static string msg_SlideOpen = "Slide Open";
        public static string msg_Alarm = "이상온도 발생";
        public static string msg_Humidity = "습도제상";
        public static string msg_TamsFile = "탐스파일에러";
        public static string msg_SenseError = "감압센서에러";
        public static string msg_Uncheck = "TC Unit UnCheck";
        public static string msg_bimetal = "바이메탈 감지";
        public static string msg_sOpen = "S.OPEN 감지";
        public static string msg_MotorUP = "Box 동작 중";
        public static string msg_MotorError = "모터 이상 동작";
        public static string msg_InitRack = "FIXTURE Initialize";
        public static string msg_ClearStatus = "Box Status Clear";

        public static string[] Motor_STAT = new string[2] { "CLOSE", "OPEN" };
        public static string[] FlowMeter_STAT = new string[3] { "정상 유량", "저유량 경고", "고유량 경고" };

        public static string currentDirectory = System.IO.Directory.GetCurrentDirectory();
        public static string[] Lottie_Path = new string[4] {"Animation4.json", "Animation_Temprature.json",
                                                            "Animation_engineer.json", "Animation1.json"};
        /*
        public static string[] Lottie_Path = new string[4] {@"D:\Project\Temp\0206\LP-MMTCRack\Resource\Animation4.json", @"D:\Project\Temp\0206\LP-MMTCRack\Resource\Animation_Temprature.json",
                                                            @"D:\Project\Temp\0206\LP-MMTCRack\Resource\Animation_engineer.json", @"D:\Project\Temp\0206\LP-MMTCRack\Resource\Animation1.json"};
        */

        static Global()
        {
            loadConfig();
        }
        static public XmlDocument gConfigDoc = new XmlDocument();

        static public void loadConfig()
        {
            gConfigDoc = new XmlDocument();
            try
            {
                gConfigDoc.Load("LPCAMRack.xml");
            }
            catch (Exception ex)
            {
                //Console.WriteLine(ex.Message);
                MessageBox.Show($"Can't Load LPCAMRack.xml {ex.Message}");
                gConfigDoc = null;
                return;
            }
        }
        static public void saveConfig()
        {
            gConfigDoc.Save("LPCAMRack.xml");
        }
    }
}
public static class VerInfo
{

    const string Program = "LPMMRACK";
    const string Date = "2026/2/27";
    const string Ver = "V1.5.0";
    const string Author = "-";
    public static string getVer() => Program + "    " + Date + " " + Ver;
	/*
	   const string Date = "2026/2/27";
       const string Ver = "V1.5.0";
        1. UI 및 통신 속도 최적화 (MainVM.cs)
            그래프 업데이트 병목(CS1998) 해결: UpdateBoxGraphDataAsync 메서드에서 불필요한 async/await를 제거하고 일반 동기 메서드(void)로 변경. UI 렌더링 대기로 인한 전체 통신 스레드 멈춤(루프 밀림) 현상 해결.
            TCON 정지 루프 버그 수정: 사용자가 IsBoxTCUnit 체크를 해제했을 때, 장비가 끊임없이 정지(Stop) 명령을 보내 무한 루프에 빠지던 현상을 if (bm.IsBoxTCUnit && bm.TconIsRun) 조건으로 방어.
        2. 온도 보정(Calibration) 기능 신규 추가
            설정 데이터 저장 (ConfigVM.cs & LPCAMRack.xml): * 3개의 온도 보정 세트(사용유무, 타겟 온도, 오프셋 값) 변수 9개 추가.
            initItems(), saveitems()에 <tempcalibration> 태그 파싱 및 저장 로직 구현.
            보정 제어용 변수 도입 (BoxModel.cs): * 사용자가 입력한 BoxSetSV와 오프셋을 계산하여 실제 장비 제어에 사용할 TargetControlSV 프로퍼티 신규 생성.
            TCON에 온도를 전송하는 setTemp(), ControlsetTemp() 내부 로직을 TargetControlSV 전송으로 교체.
            내부 MMLOGIC 제어(ProcessBoxSV...) 및 안정화 오차 비교(BoxPVCheck) 시 기준값을 TargetControlSV로 교체하여 화면 표시는 그대로 두고 내부 제어만 보정되도록 완벽 분리.
            UI 추가 (ConfigPage.xaml): Control 탭 내부에 보정값을 입력할 수 있는 파스텔 블루(#FFE1F0FF) 배경의 체크박스/텍스트박스 표 레이아웃 추가.
        3. 에러 및 예외 처리 로직 견고화 (BoxModel.cs)
            미세 제어 시 에러 "무적 모드" 방어: ControlsetTemp() 호출 시 TimeStart, TempErrorTime, TempAlarmTime이 초기화되던 문제 제거. (실제 에러가 발생해야 할 때 타이머가 리셋되어 알람이 울리지 않던 치명적 버그 수정)
            스레드 블로킹(Thread.Sleep) 제거 (BoxPVCheck): 제상(Defrost) 진입 시 전체 시스템을 멈추던 Thread.Sleep을 Task.Run 내부의 await Task.Delay로 캡슐화하여 비동기 처리.
            제상 루틴 중복 진입 차단 (BoxPVCheck): 비동기 딜레이 대기 중 통신 루프가 다시 돌아와 제상 루틴이 중복 실행되는 것을 막기 위해, Task 진입 직전에 BoxStat = Box_STAT.DEFROST;로 상태를 미리 선언하여 락(Lock) 효과 구현.
            통신 속도 의존성 제거 (BoxPVCheck): 기존 카운트 방식(StableAlarmCnt)의 안정화 에러 검사를 절대 시간 타이머 방식(StableErrorTime = DateTime.Now.AddSeconds(30))으로 변경하여 통신 속도나 장비 대수에 상관없이 일정한 스펙(30초 유지 시 에러) 보장.
            로그 도배 방지: 알람 발생 시 TempAlarmTime = DateTime.Now.AddMinutes(1); 처리하여 경고 로그가 초당 수십 개씩 쌓이는 현상 방지.
        4. 온도 제어 알고리즘(MMLOGIC) 개선 (BoxModel.cs)
            단기/장기(Short/Long Term) 간섭 해결 (ProcessBoxSV_Logic_Org): 기존 goto SVDone; 사용으로 인해 LongTerm 제어 발동 시 ShortTerm 배열 갱신이 무시(Skip)되던 현상을 isAdjusted 플래그를 도입하여 순차적으로 안전하게 처리되도록 구조 변경. (오류로 수정했던 isAdjusted = true 위치도 정확히 보정 완료)
            정체 구간 판단 현실화: up == 0 && down == 0이라는 비현실적인 조건을 삭제하고, Math.Abs(avg) < 0.3 조건만으로 실제 산업 환경의 노이즈를 반영하여 제자리걸음(정체)을 유연하게 판단하도록 개선.
        5. 노이즈 및 0도 통과 필터링 (GetBoxPV) - (사용자 자체 검증 사항)
            함수 첫 줄에 if (Math.Abs(value) > Global.gConfigVM.GarbageReadTemp) return; 추가하여 하드웨어 스펙 한계치 이상의 쓰레기값 원천 차단.

	 *const string Date = "2026/2/26";
      const string Ver = "V1.4.9"; 
	  - TconCom.cs read 하는 부분 개선 및 run 상황일때 tcunit 체크박스 비활성화
     * const string Date = "2026/2/24";
       const string Ver = "V1.4.8";
    - 온도조절기 파마레터 구조 변경

	*const string Date = "2025/1/20";
      const string Ver = "V1.4.7";  
      - Silde Open 팝업과 다른 에러 팝업 문제로 파악되어 Silde Open 팝업 추가 만들어거 따로 팝업 제어하게함

    *const string Date = "2025/1/20";
      const string Ver = "V1.4.6";  
      - 감압센서 에러 팝업 제대로 안나오는 상황 수정

     *const string Date = "2025/1/16";
      const string Ver = "V1.4.5";  
      - 개별 Rack , Box 팝업 Clear 버튼 추가
     
	 *const string Date = "2025/1/6";
      const string Ver = "V1.4.4";  
	        - LongTerm 제어 로직에서 오차가 설정 범위(10도 등)를 초과하더라도, 현재 SV가 Min/Max 
              한계치에 도달해 있는 경우에는 강제 리셋을 수행하지 않고 최대 출력을 유지하도록 예외 조건을 추가함.

	 *const string Date = "2025/12/19";
      const string Ver = "V1.4.3"; 
	        -PreStartTest() 진행 안될때 로그 보강하여 원인체크
            - 기타 버그 수정

	 *const string Date = "2025/12/9";
      const string Ver = "V1.4.2"; 
	        - GetBoxPV(double value) 함수 첫줄에 if (Math.Abs(value) > Global.gConfigVM.GarbageReadTemp) return; 추가하여 지정된 값 이상 들어오면 처리안하게 리턴
            - 그리고 검증할 GetBoxPV(double value) 함수 주석 처리해놓음 나중에 테스트 해봐야함
              Read Temp Data 0일때 다음온도는 무조건 3번 이상 0이상 온도가 들어와야 3번째 들어온 온도로 업데이트 ( 연속 3번 쓰레기값 안들어온다는 가정)
              나머지상황은 기존과 동일하게 동작하게
            - Capture Board Read 실패 시 4번 재연결 시도, 5번 이후 재연결 중단하되 통신은 계속 시도
	 
     *const string Date = "2025/11/11";
      const string Ver = "V1.4.1"; 
            - 저온모드일때 PrevPV 값이 0.25 or -0.25 이고 vlaue 가 0 일때는 0으로 표현
              근데 PrevPV 값이 0.25 나 -0.25 가 아니고 vlaue 0 일 경우에는 PrevPV 값으로 표현하게 수정
      
    *   const string Date = "2025/7/2";
        const string Ver = Ver = "V1.4.0";
            - 자동 모터 업 시 안올라갈때 Log로 Motor Stat 표시 및 상태 초기화 하라는 메세지 추가
            - Temp Log Run Log 생성 전 Log 추가 생성되는 문제 수정. startTest()내 PreRunControlTemp = false; 위치수정
            - 
    */
}

