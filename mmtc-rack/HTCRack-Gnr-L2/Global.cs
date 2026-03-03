using HTCRack.Base;
using HTCRack.Net;
using HTCRack.ViewModel;
using System;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace HTCRack
{

    enum TEMP_STAT : int{
        NONE = 0,
        NORMAL,
        WARNING,
        ERROR
    }    

    static public class Global {

        #region View, VM Object

        static public MainVM gMainVM;
        static public DimmVM gDimmVM;        
        static public EngineerVM gEnginerVM;
        static public MainPageVM gMainPageVM;
        static public ConfigVM gConfigVM;
        static public BoardVM gBoardVM;
        #endregion

        #region Equip Selection Rect
        static public Rect[] gRackRects = new Rect[] {
            new Rect(19, 60, 40, 32),
            new Rect(65, 60, 40, 32),
            new Rect(110, 60, 40, 32),
            new Rect(19, 90, 40, 32),
            new Rect(65, 90, 40, 32),
            new Rect(110, 90, 40, 32)
        };

        static public Rect[] gRackMagins = new Rect[] {
            new Rect(21, 125, 112, 110), 
            new Rect(67, 125, 67, 110),
            new Rect(112, 125, 20, 110),
            new Rect(21, 156, 112, 79),
            new Rect(67, 156, 67, 79),
            new Rect(112, 156, 20, 79),
            /*
            new Rect(23, 91, 92, 134), //Margin="57,95,100,134"
            new Rect(90, 91, 25, 134),
            new Rect(23, 126, 92, 100),
            new Rect(90, 126, 25, 100),
            new Rect(23, 162, 92, 67),
            new Rect(90, 162, 25, 67),
            */
        };

        static public Rect[] gRackRectsCol1 = new Rect[] {
            new Rect(54, 60,  60, 32),            
            new Rect(54, 97,  60, 32),
            new Rect(54, 132, 60, 32),
        };

        static public Rect[] gRackMaginsCol1 = new Rect[] {
            new Rect(57, 91, 58, 134),            
            new Rect(57, 126, 58, 100),
            new Rect(57, 162, 58, 67),
        };
        #endregion

        #region Color Def
        public static SolidColorBrush[] gDimStatColors = new SolidColorBrush[] {
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0xddddff)),   // STOP
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0x55aa55)),   // RUN
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0xffffff)),   // No Unit
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0x9E602D)),   // Warn
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0xAE802D)),   // Alram
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0x113311)),   // Error   
            new SolidColorBrush(MConv.IntToMediaColor(0xAA, 0x1182c3)),
        };

        public static SolidColorBrush[] gCBoxColors = new SolidColorBrush[]  {
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x888888)),   // Normal
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x88ff88)),   // Run
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x99aa99)),   // Mix
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0x99aa99)),   // Tune            
            new SolidColorBrush(MConv.IntToMediaColor(0x44, 0xff8888)),   // Error
         };

        public static SolidColorBrush[] gAlramColors = new SolidColorBrush[]
        {
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0x6D79FF)), // Done
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0xFFCF4C)), // WARAN
            new SolidColorBrush(MConv.IntToMediaColor(0x99, 0xFF5B79)), // ERROR
            
          
        };

        public static SolidColorBrush[] gRackColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.Transparent),    // Normal
            new SolidColorBrush(Colors.IndianRed),            // as mode            
        };

        public static SolidColorBrush[] gBoarderColors = new SolidColorBrush[]
        {
            // RACK_STAT Index
            new SolidColorBrush(Colors.AntiqueWhite),    // STOP            
            new SolidColorBrush(Colors.AntiqueWhite),      // PREV_STOP
            new SolidColorBrush(Colors.PaleGreen),      // PERV_RUN
            new SolidColorBrush(Colors.SpringGreen),      // RUN
            new SolidColorBrush(Colors.DeepSkyBlue),    // Done
            new SolidColorBrush(Colors.Orange),  // TUNING
            new SolidColorBrush(Colors.Plum),       // ERROR
            new SolidColorBrush(Colors.LightGray),  // NOCON
            new SolidColorBrush(Colors.Red),  // INSPECTION -> as mode
			new SolidColorBrush(Colors.YellowGreen)  // BMC_RESET
        };

        public static SolidColorBrush[] gFPGAProgColors = new SolidColorBrush[]
        {
            new SolidColorBrush(Colors.LightGray),    // STOP            
            new SolidColorBrush(Colors.Green),    // Erase
            new SolidColorBrush(Colors.Blue),    // Program
            new SolidColorBrush(Colors.Red),      // Alarm
            new SolidColorBrush(Colors.Red)      // Error
        };
        #endregion


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
                gConfigDoc.Load("HtcRack.xml");
            }
            catch (Exception ex)
            {
                //Console.WriteLine(ex.Message);
                MessageBox.Show($"Can't Load HtcRack.xml {ex.Message}");
                gConfigDoc = null;
                return;
            }
        }

        static public void saveConfig()
        {
            gConfigDoc.Save("HtcRack.xml");
        }

        static public HtcMessage newMsg(int ch, int adr, int func, int Len = 0) // if Len is Not 0, It is Request Byte Count
        {
            HtcMessage msg = new HtcMessage();
            msg.sub = new HtcSub();
            msg.sub.Chanel = (byte)ch;
            msg.sub.Adr = (byte)adr;
            msg.sub.Func = (byte)func;
            msg.sub.Len = (short)Len;

            return msg;
        }

        static public HtcMessage newMsg(byte ch, byte adr, byte func, byte Len = 0)
        {
            HtcMessage msg = new HtcMessage();
            msg.sub = new HtcSub();
            msg.sub.Chanel = ch;
            msg.sub.Adr = adr;
            msg.sub.Func = func;
            msg.sub.Len = (short)Len;

            return msg;
        }


    }



}



public static class VerInfo {
    
    const string Program = "HTCRack-Gnr-L2";
    const string Date = "2025/08/27";
    const string Ver = "V1.0.7";
    const string Author = "uiop";
    public static string getVer() => Program + "    " + Date + " " + Ver;
}

/*
 * const string Program = "HTCRack-Gnr-L2";
    const string Date = "2025/08/27";
    const string Ver = "V1.0.7";
        - DimmCheckState 초기화 부분 수정 
 * 
 * * const string Date = "2025/08/26";
      const string Ver = Ver = "V1.0.6";
        - BMC Reset 5번 Retry 제외
        - BMC Reset 1번 보내고 응답 확인 성공/실패는 Dimm 초기화 50초 대기 하여 50초 동안 초기화 안되면 다시 
          BMC Reset 하게 수정 초기화 될때까지 계속 시도
        - Use BMC Reset 유/무는 DIMM 소실을 감시만 하냐 안하냐로 변경 나머지 로그는 다 나오게 수정.
                            
 * 
 * const string Date = "2025/08/25";
      const string Ver = Ver = "V1.0.5";
        - BMC Reset 5번 Retry 추가
 * 
 * const string Date = "2025/08/25";
      const string Ver = Ver = "V1.0.4";
        - 모든 DIMM의 온도가 0으로 떨어지는 것을 감지 후 DIMM 온도 하나라도 값 올라오는 거 확인 순으로 루틴 추가
 * 
 * 
 * const string Date = "2025/08/25";
      const string Ver = Ver = "V1.0.3";
        - BMC 경로 수정

 * *  const string Date = "2025/08/25";
      const string Ver = Ver = "V1.0.2 ";
        - BMC RESET 기능 추가
        - RUN 버튼 클릭 시 10초 대기 후 BMC RESET 수행. 정상 완료면 다음 동작 진행 에러면 팝업 발생하고 STOP.
        - Dimm Cnt 기준은  Dimm Temp 가 0 아닌 값이 들어오고 10번 Read 후 Dimm Cnt 기준 잡고 이후 기준 Dimm Cnt 보다 적게 Read 되면 BMC 리셋하게 변경.반복수행
        - Config 탭에 Error condition 에 Use BMC Reset 체크박스 추가 하여 BMC 리셋 사용유무 선택 가능
          BMC 리셋 사용유무 는 Dimm Cnt 보다 적게 Read 되면 BMC 리셋 에서만 적용되면  Temp CTL Run 시는 항시 BMC Reset 동작 함.


 *  const string Date = "2025/08/04";
    const string Ver = Ver = "V1.0.1 ";
        - 그래프 X측 누적 시간으로 변경.
        - Stop 시 Temp File 하나 더 생성되는 문제 수정.
        - side open -> Slide Open 변경
        
 *  
 *  
 *  const string Date = "2025/06/11";
    const string Ver = "V1.0.0 (No Dimm Box Stop)";
        box 10분이상 dimm 없으면 stop
        ctrl svmin , svmax xml 에 추가 및 ctrl 적용
        tamsFileCheckTimer_Tick 에 Error PoPup 발생 시 자동 Start 안하게 return 추가
        TCON CHSTAT 처리 부분 수정
        double diff = InsideBoxTemp - PrevInsideBoxTemp;
        diff > 10 에서 diff > 20 으로 변경

-----여기서 부터 청주꺼 새로시작임 ------------------------------------------------------------
 *  
 *  const string Date = "2025/04/16";
    const string Ver = "V1.1.1 (No Dimm Box Stop)";
        box 10분이상 dimm 없으면 stop
 *  
 *  
 *  const string Date = "2025/03/07";
    const string Ver = "V1.1.0 (BMC DimmTemp)";
        RedfishLib 포함 BMC Communication 기능 추가 
        xml <redfish use="1" > 부분 으로 사용         

  *  
 *  PV InsideBoxTemp 10도 이상 차이 발생시 이전값 적용  
 *  
 *  
 *  const string Ver = "V1.0.3 (Fan RPM)";
 *  const string Date = "2024/12/11";
 *      Fan rpm Monitoring  15 -> 10 개 선별하여 감시
 *  
 *  *  2024/11/06
 *      thottle offset은 제거 기존로직에서 처럼.
 *      maxdimm array로 clear로 memory exception 수정
 *  
 *  
 *  
 *  *  2024/10/09
 *      Graph tick cnt -> time format으로 
 * 
 * 2024/09/06
 *    08/20 추가 부분 제거. by x13
 * 
 * 2024/08/20
   Up 제한 조작. Dwon 발생 후 up 되면 기준온도를 낮쳐 이후 up되는 기준점을 낮춤. 
 * 
 2024/08/12
  config double to int bug fix
  board setup page run hot / cold

 * 
 * const string Date = "2024/08/08";
    const string Ver = "V1.0.0 - Graph More";
    
 * 
 * const string Date = "2024/07/08";
    const string Ver = "V1.0.0 - Graph More";
 * 
 * const string Date = "2024/07/03";
    const string Ver = "V1.0.0 - Tams Seq Mod";
 * 
 * 2024/07/01
 * *"V1.0.0 - File IO Exception";
 * 
  2024/06/14
 * Ver = "V1.0.0 - FP / TParam";
 *
 ** * 2024/06/11
 * Ver = "V1.0.0 - FP Display";
 * 
 * * 2024/06/05
 * Ver = "V1.0.0 - Auto / Tams";
 *   Auto Flag = Run 조건
 *   Tams Flag = Temp 조건
 * 
 * 2024/05/30
 * Ver = "V1.0.0 - Max Ctrl / Fan Mod / Auto Run / Side";
 * 
 * 2024/05/27 V1.0.0 - max, fan, auto tams";
 * 
 2024/05/17 V1.0.0 - Max Ctrl";
 * 
"2024/04/27 V1.0.0 - First Release version";
    mod : stop 중에도 side open되면 popup on, close 시 popup hide     
    mod : box 내 dimm 온도 없으면 이전 sv 로
    mod : run 아니여도 dimm온도 올라오면 화면 출력
    mod : plc 접속하면 ac power on, ac power는 slot 외 따로 관리
    mod : 토출구(inside box) 온도 감시 16도 이하
 

 * 
 "V1.0.0 - First Release version";
    add : log add
    
 "V1.1.6 - PLC 6 Rack Ver";
    mod : PLC Rack 3 -> 6
    mod : rack count from config.xml

 *
 "V1.1.5 - SSR Cold / Freq Add";
    mod : config 
    mod : operation
 
 V1.1.4 - Log Retention
    mod : Cooler PV 8 -> 4
    mod : Cooler PV Index Read in Config
 * 
 V1.1.3 - Log Retention
    add : Log Retention 기능
    mod : as, tams, prev sv save 추가
    mod : cur register 수식 변경
 
V1.1.2 - SSR Pulse Setting Add
 *  mod :   Engineer Mode Register 
    mod :   Water Temp Noise Value 120도 이상 Discard

 V1.1.1 - Reflecting requirements
 *  mod :   Test중 - Door Open시 멈춤 / close 시 재시작
 *          Test 중 door open 시간l limit
 *          Tams 기능 Check
 *          UI 꺼지면 상태 복원
 *          Temp H/L Save Select or Temp 하나만 Save
 */