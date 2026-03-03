using LP_MMTCRack.Base;
using LP_MMTCRack.View.Component;
using LP_MMTCRack.ViewModel;
using PropertyChanged;
using ScottPlot.MarkerShapes;
using Serilog;
using Serilog.Core;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml.Linq;
using static HandyControl.Tools.Interop.InteropValues;

namespace LP_MMTCRack.Model
{
    public enum Box_STAT
    {
        IDLE = 0,
        STOP,
        PRERUN,
        RUN,
        DONE,
        DEFROST,
        ERROR
    }
    public enum DEFROST_TYPE
    {
        NORMAL = 0,
        HUMIDITY,
        ALARM,
        LOWTEMP,
        TAMS,
        TEMPERROR
    }
    public enum TRADING
    {
        RISE,
        FALL
    }
    public enum SVC  //SV Ctrl
    {
        CLEAR = 0,
        TCON = 0x1,
        STABLE = 0x2,
        ACCELERATION = 0x4,
        NEXT = 0x8,

        DOWN = 0x10,
        UP = 0x20
    }
    public class BoxModel : BaseVM
    {
        [DoNotNotify] public int BoxIndex { get; set; } = 0;
        [DoNotNotify] public int BoxViewIndex { get; set; } = 0;
        [DoNotNotify] public int EachBoxIndex { get; set; } = 0;
        public SolidColorBrush BoarderColor { get; set;}
        public SolidColorBrush BoxColor { get; set; } = Global.gBoxColors[0];
        public SolidColorBrush HumRateColor { get; set; } = Global.gWarningColors[0];

        [DoNotNotify] public int GraphIndex { get; set; } = 0;
        [DoNotNotify] public List<TempDataModel> TempDatas { get; set; } = new List<TempDataModel>();

        [DoNotNotify]
        public Logger TempSerilog { get; set; } = null;

        public bool IsBoxTamsEnable { get; set; } = false;

        public Collection<double> SampleLongTerm = new Collection<double>();

        public Collection<double> SampleShortTerm = new Collection<double>();

        public TRADING PrevTrading = TRADING.RISE;

        public int LongTermCount = 10;
        public int ShortTermCount = 8; //기존 5 -> 8

        public int Level = 0;
        public int SVCtrl;

        bool _isboxtcunit = false;
        public bool IsBoxTCUnit
        {
            get => _isboxtcunit;
            set
            {
				if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.PRERUN || BoxStat == Box_STAT.DEFROST)
				{
					return;
				}

				_isboxtcunit = value;
                if (_isboxtcunit)
                {
                    BoxColor = Global.gBoxColors[0];

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
                else
                {
                    BoxColor = Global.gBoxColors[2];

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }

            }
        }
        bool _isboxAS = false;
        public bool IsBoxAS
        {
            get => _isboxAS;
            set
            {
                _isboxAS = value;
                if (_isboxAS)
                {
                    BoxColor = Global.gBoxColors[1];

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
                else
                {
                    BoxColor = Global.gBoxColors[0];

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
            }
        }
        bool _isboxTams = true;
        public bool IsBoxTams
        {
            get => _isboxTams;
            set
            {
                _isboxTams = value;
                if (_isboxTams)
                {
                    IsReadOnlySV = true;

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();

                }
                else
                {
                    IsReadOnlySV = false;

                    if (Global.gConfigVM != null)
                        Global.gConfigVM.saveitems();
                }
            }
        }
        int _humidityValue = 0;
        public int HumidityValue
        {
            get => _humidityValue;
            set
            {
                _humidityValue = value;
            }
        }
        int PreHumidityValue = 0;
        public void SetHumidityValue(int value)
        {
            if (isLoggingHumidity)
            {
                PreHumidityValue = _humidityValue;
                HumidityValue = value;
                HumidityValueCheck();
            }
        }
        int humidityWarningCnt = 0;
        void HumidityValueCheck()
        {
            //0 ~ 99까지 습도값으로 판단 100 이상은 쓰레기값으로 
            if (_humidityValue + HumidityOffset < 0)
                _humidityValue = 0;
            else if (_humidityValue + HumidityOffset >= 100)
            {
                humidityWarningCnt++;
                _humidityValue = PreHumidityValue;
                
                if (humidityWarningCnt > 10) // 10번에 1번만 Warning Msg
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "]  =  HumidityValue 100 Over");
                    humidityWarningCnt = 0;
                }
                //_humidityValue = 100;
            }
            else
            {
                _humidityValue = _humidityValue + HumidityOffset;
            }

            if (_humidityValue > (Global.gConfigVM.humidityWarnval - 10))
                HumRateColor = Global.gWarningColors[1];
            else
                HumRateColor = Global.gWarningColors[0];

            if (_humidityValue > Global.gConfigVM.humidityWarnval)
            {
                if (BoxStat == Box_STAT.RUN) // humidityWarnval 넘어가면 제상스탑
                {
                    BoxDefrost = DEFROST_TYPE.HUMIDITY;
                    Thread.Sleep((BoxIndex % 6) * 100);
                    isLoggingHumidity = false;
                    //습도 제상 루틴
                    Task.Run(() => DEFROSTStep());
                }
            }
        }
        public bool isLoggingHumidity = false;
        public int Hum_TempSenorVal { get; set; } = 0;
        public int HumidityOffset { get; set; } = 0;
        double _boxsetSV = 25;
        public double BoxSetSV
        {
            get => _boxsetSV;
            set
            {
                _boxsetSV = value;
            }
        }

		//온도 보정(Calibration Offset) 로직이 적용된 실제 제어 목표 온도
		public double TargetControlSV
        {
            get
            {
                double originalSV = this.BoxSetSV;

                if (Global.gConfigVM == null) return originalSV;

				//Set 1
				if (Global.gConfigVM.IsUseTempOffset1 && Math.Abs(originalSV - Global.gConfigVM.OffsetTarget1) < 0.01)
					return originalSV + Global.gConfigVM.OffsetValue1;

				//Set 2
				if (Global.gConfigVM.IsUseTempOffset2 && Math.Abs(originalSV - Global.gConfigVM.OffsetTarget2) < 0.01)
					return originalSV + Global.gConfigVM.OffsetValue2;

				//Set 3
				if (Global.gConfigVM.IsUseTempOffset3 && Math.Abs(originalSV - Global.gConfigVM.OffsetTarget3) < 0.01)
					return originalSV + Global.gConfigVM.OffsetValue3;

				return originalSV;
			}
        }

        Box_STAT _boxStat;
        public Box_STAT BoxStat
        {
            get => _boxStat;
            set
            {
                _boxStat = value;
                BoarderColor = Global.gBoarderColors[(int)_boxStat];
                BoxStatText = _boxStat.ToString();
                MasterRack.HidePopupIfNeeded();
            }
        }
        public DEFROST_TYPE BoxDefrost { get; set; } = DEFROST_TYPE.NORMAL;
        bool TempAlarmFlag = false;
        bool TempErrorFlag = false;
        public string BoxPVText { get; set; } = string.Empty;
        public SolidColorBrush BoxPVColor { get; set; } = Global.gWarningColors[0];
        double _boxpv = 0;
        public double BoxPV 
        { 
            get => _boxpv;
            set
            {
                _boxpv = value;

            }
        }

        public double prevPV;
        public double zeroCnt = 0;
        public double abnomalCnt = 0;
		public double garbageCnt = 0;
		private int _zeroEscapeCnt = 0;
		/*
        public void GetBoxPV(double value)
        {
            Console.WriteLine($"Box{BoxViewIndex} GetTemp =  {value}");
            // 연속 0 값 카운트를 위한 변수
            const int thresholdForGarbageValue = 5; // 연속적으로 0이 들어온 횟수
            const int realZeroThreshold = 6; // 리얼 0이 연속적으로 나타나는 허용 최대 횟수

            if ((BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST) && (BoxSetSV <= 5))
            {
                double CheckPrevPV = Math.Abs(prevPV);

                // "value"가 0일 때 연속적으로 0 값이 들어왔다면 zeroCnt 증가
                if (value == 0)
                {
                    zeroCnt++;
                }
                else
                {
                    zeroCnt = 0; // 값이 0이 아니면 zeroCnt 초기화
                }

                // prevPV가 0이었고 value가 0일 때는 리얼 0 값으로 판단하고 쓰레기값 처리하지 않음
                if (prevPV == 0 && value == 0)
                {
                    zeroCnt = 0; // 연속된 0을 리얼값으로 처리하여 zeroCnt 초기화
                }

                // 리얼 0 값이 연속적으로 나타나는 경우는 쓰레기값으로 처리하지 않음
                if (zeroCnt <= realZeroThreshold)
                {
                    // 연속적으로 0이 thresholdForGarbageValue번 이상 들어오면 쓰레기 값으로 판단
                    if (zeroCnt >= thresholdForGarbageValue)
                    {
                        BoxPVText = string.Empty; // UI에서 값을 표시하지 않음
                        return; // 쓰레기 값이므로 처리하지 않음
                    }

                    // value가 0이 아니면 일반적인 처리
                    if (value != 0 || (CheckPrevPV == 0.25 && value == 0)) // prevPV가 0.25이고 현재 value가 0일 경우
                    {
                        if (prevPV != 0)
                        {
                            double diffval = prevPV - value;

                            if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
                            {
                                BoxPV = prevPV;
                                abnomalCnt++;
                            }
                            else
                            {
                                BoxPV = value;
                                prevPV = value;
                                abnomalCnt = 0;
                            }
                        }
                        else
                        {
                            BoxPV = value;
                            prevPV = value;
                            abnomalCnt = 0;
                        }

                        BoxPVColor = Global.gWarningColors[0];
                        BoxPVText = BoxPV.ToString();
                        zeroCnt = 0; // zeroCnt 초기화

                        BoxPVCheck();

                        if (abnomalCnt > 4)
                        {
                            BoxPVColor = Global.gWarningColors[1];
                            Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnormal Dimm Temp =  " + value);
                        }
                    }
                    // 쓰레기값일 경우
                    else if (prevPV != 0 && value == 0)
                    {
                        zeroCnt++;
                        BoxPV = value;
                    }

                    if (zeroCnt > 6)
                    {
                        BoxPV = value;
                        prevPV = value;
                        BoxPVText = string.Empty; // BoxPV.ToString();
                    }
                }
                else // 리얼 0 값이 연속적으로 발생한 경우
                {
                    // 리얼 0인 경우에는 계속 처리하고 BoxPVText를 빈 문자열로 두지 않음
                    BoxPV = value;
                    prevPV = value;
                    BoxPVText = BoxPV.ToString(); // 값을 표시
                    zeroCnt = 0; // zeroCnt 초기화
                }
            }
            else
            {
                if (prevPV != 0 && value == 0)
                {
                    zeroCnt++;
                    BoxPV = prevPV;
                }
                else if (value != 0)
                {
                    if (prevPV != 0)
                    {
                        double diffval = prevPV - value;

                        if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
                        {
                            BoxPV = prevPV;
                            abnomalCnt++;
                        }
                        else
                        {
                            BoxPV = value;
                            prevPV = value;
                            abnomalCnt = 0;
                        }
                    }
                    else
                    {
                        BoxPV = value;
                        prevPV = value;
                        abnomalCnt = 0;
                    }
                    BoxPVColor = Global.gWarningColors[0];
                    zeroCnt = 0; // zeroCnt 초기화
                    BoxPVCheck();
                    BoxPVText = BoxPV.ToString();

                    if (abnomalCnt > 4)
                    {
                        BoxPVColor = Global.gWarningColors[1];
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnormal Dimm Temp =  " + value);
                    }
                }

                if (zeroCnt > 3)
                {
                    BoxPV = value;
                    prevPV = value;
                    BoxPVText = string.Empty; // BoxPV.ToString();
                }
            }
        }
        */

		public void GetBoxPV(double value)
		{
			// 1. 하드웨어 스펙 한계치 방어 (가비지 값 처리)
			if (Math.Abs(value) > Global.gConfigVM.GarbageReadTemp)
			{
				garbageCnt++;

				if (garbageCnt > 6)
				{
					// 센서가 완전히 고장남: 화면을 빈칸으로 만들어 작업자에게 알림
					BoxPVText = string.Empty;
				}
				else
				{
					// 순간적인 노이즈: 이전 정상 온도를 유지 (화면 깜빡임 방지)
					BoxPVText = BoxPV.ToString();
				}

				// 🚨 핵심: 그냥 return하지 않고, 알람 검사(BoxPVCheck)는 무조건 실행하게 한다!
				BoxPVCheck();
				return;
			}
			else
			{
				garbageCnt = 0; // 정상 값이 들어오면 가비지 카운터 초기화
			}

			// 2. 값 0에 대한 카운터 처리 (장비 꺼짐 or 0도 통과 판별)
			if (value == 0)
			{
				zeroCnt++; // 0이 들어오면 무조건 카운트 증가
			}
			else
			{
				zeroCnt = 0; // 0이 아닌 값이 들어오면 즉시 정상으로 판단하고 카운터 리셋
			}

			// 3. 값이 0일 때의 시나리오 분기
			if (zeroCnt > 0)
			{
				// 시나리오 A: 진짜 온도가 0도를 지나는 중 (이전 온도가 -2 ~ +2도 사이였고, 처음 3번 이내)
				if (Math.Abs(prevPV) <= 2.0 && zeroCnt <= 3)
				{
					BoxPV = value;
					prevPV = value;
					abnomalCnt = 0;
					// 빈칸이 되지 않고 0.0도를 화면에 자연스럽게 표시합니다.
				}
				// 시나리오 B: 통신 단선 또는 장비 꺼짐 초기 (버티기)
				else if (zeroCnt <= 6)
				{
					return; // 아직 확정할 수 없으니 이전 값(prevPV)을 유지하며 대기합니다.
				}
				// 시나리오 C: 장비 완전히 꺼짐 확정 (6번 초과 연속 0)
				else
				{
					BoxPV = value; // 내부 변수들도 0으로 리셋해 줍니다.
					prevPV = value;
					abnomalCnt = 0;
				}
			}
			// 4. 정상적인 값이 들어올 때 (온도 튐/Noise 방어 로직)
			else
			{
				if (prevPV != 0)
				{
					double diffval = Math.Abs(prevPV - value);

					if (diffval > Global.gConfigVM.NoiseTempDiff)
					{
						abnomalCnt++;
						if (abnomalCnt > 4) // 4번 이상 튀면 진짜 온도로 인정
						{
							BoxPVColor = Global.gWarningColors[1];
							Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Box{BoxViewIndex}] Abnormal Temp Fixed: {value}");
							BoxPV = value;
							prevPV = value;
							abnomalCnt = 0;
						}
						else
						{
							return; // 4번 미만은 노이즈로 보고 무시 (버티기)
						}
					}
					else
					{
						BoxPV = value;
						prevPV = value;
						abnomalCnt = 0;
						BoxPVColor = Global.gWarningColors[0];
					}
				}
				else
				{
					// 빈칸이었다가 장비를 막 켜서 정상값이 처음 들어오는 경우 즉시 복구!
					BoxPV = value;
					prevPV = value;
					abnomalCnt = 0;
					BoxPVColor = Global.gWarningColors[0];
				}
			}

			// 5. 최종 화면 표시 및 에러 검사
			if (zeroCnt > 6)
			{
				BoxPVText = string.Empty; // 장비 꺼짐 -> UI 빈칸 처리 (원하시던 기능!)
			}
			else
			{
				BoxPVText = BoxPV.ToString(); // 정상 온도 표시
				BoxPVCheck(); // 온도가 들어올 때만 알람 검사 진행
			}
		}

		#region 간단한 해결 0에서 쓰레기값 나오는거 막는거
		/*
        public void GetBoxPV(double value)
		{
            if (Math.Abs(value) > Global.gConfigVM.GarbageReadTemp) return;

			if ((BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST) && (TargetControlSV <= 5))
			{
				double CheckPrevPV = Math.Abs(prevPV);
				//Console.WriteLine($"PrevPv = {CheckPrevPV}");

				if (value != 0 || (CheckPrevPV == 0.25 && value == 0))
				{
					if (prevPV != 0)
					{
						double diffval = prevPV - value;
						//Test용 Log 추가 삭제필요
						//Global.gMainPageVM.addLog(LOG_TYPE.INFO, Math.Abs(diffval).ToString() + ", prePV = " + prevPV.ToString());
						if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
						{
							BoxPV = prevPV;
							abnomalCnt++;
						}
						else
						{
							BoxPV = value;
							//Console.WriteLine($"!!!! Rack : {MasterRack.RackViewIndex} Box : {BoxViewIndex} Temp = {BoxPV} , Value = {value}");
							prevPV = value;
							abnomalCnt = 0;
						}
					}
					else
					{
						BoxPV = value;
						prevPV = value;
						abnomalCnt = 0;
					}
					BoxPVColor = Global.gWarningColors[0];
					zeroCnt = 0;
					BoxPVCheck();
					BoxPVText = BoxPV.ToString();

					if (abnomalCnt > 4)
					{
						BoxPVColor = Global.gWarningColors[1];
						Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnomal Dimm Temp =  " + value);
					}
				}
				else if (prevPV != 0 && value == 0)
				{
					zeroCnt++;
					BoxPV = value;
				}

				if (zeroCnt > 6)
				{
					BoxPV = value;
					prevPV = value;
					BoxPVText = string.Empty;//BoxPV.ToString();
				}
			}
			else
			{
				if (prevPV != 0 && value == 0)
				{
					zeroCnt++;
					BoxPV = prevPV;
				}
				else if (value != 0)
				{
					if (prevPV != 0)
					{
						double diffval = prevPV - value;
						//Test용 Log 추가 삭제필요
						//Global.gMainPageVM.addLog(LOG_TYPE.INFO, Math.Abs(diffval).ToString() + ", prePV = " + prevPV.ToString());
						if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
						{
							BoxPV = prevPV;
							abnomalCnt++;
						}
						else
						{
							BoxPV = value;
							//Console.WriteLine($"!!!! Rack : {MasterRack.RackViewIndex} Box : {BoxViewIndex} Temp = {BoxPV} , Value = {value}");
							prevPV = value;
							abnomalCnt = 0;
						}
					}
					else
					{
						BoxPV = value;
						prevPV = value;
						abnomalCnt = 0;
					}
					BoxPVColor = Global.gWarningColors[0];
					zeroCnt = 0;
					BoxPVCheck();
					BoxPVText = BoxPV.ToString();

					if (abnomalCnt > 4)
					{
						BoxPVColor = Global.gWarningColors[1];
						Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnomal Dimm Temp =  " + value);
					}
				}

				if (zeroCnt > 3)
				{
					BoxPV = value;
					prevPV = value;
					BoxPVText = string.Empty;//BoxPV.ToString();
				}
			}

		}
        */
		#endregion

		#region 검증필요한 로직 12/8
		/*
		public void GetBoxPV(double value)
        {
			// [1] 저온 모드 (설정온도 5도 이하)
			if ((BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST) && (BoxSetSV <= 5))
            {
                double CheckPrevPV = Math.Abs(prevPV);
                //Console.WriteLine($"PrevPv = {CheckPrevPV}");

                if (value != 0 || (CheckPrevPV == 0.25 && value == 0))
                {
					// 이미 정상 온도가 측정되고 있던 경우
					if (prevPV != 0)
                    {
                        _zeroEscapeCnt = 0; // 정상 동작 중이니 카운터 리셋

                        double diffval = prevPV - value;

						//Test용 Log 추가 삭제필요
						//Global.gMainPageVM.addLog(LOG_TYPE.INFO, Math.Abs(diffval).ToString() + ", prePV = " + prevPV.ToString());

						// 노이즈 필터링 로직
						if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
                        {
                            BoxPV = prevPV;// 노이즈면 이전 값 유지
							abnomalCnt++;
                        }
                        else
                        {
                            BoxPV = value; // 정상 범위면 값 갱신
							//Console.WriteLine($"!!!! Rack : {MasterRack.RackViewIndex} Box : {BoxViewIndex} Temp = {BoxPV} , Value = {value}");
							prevPV = value;
                            abnomalCnt = 0;
                        }
                    }
                    else
                    {
                        //0 에서 값 변경 3번 연속 확인
                        _zeroEscapeCnt++;

                        if (_zeroEscapeCnt >= 3)
                        {
                            BoxPV = value;
                            prevPV = value;
                            abnomalCnt = 0;
                            _zeroEscapeCnt = 0; //정상 값 읽었으니 리셋
                        }
                        else
                        {
                            // 3번 미만은 상태 유지
                        }
                    }


                    BoxPVColor = Global.gWarningColors[0];
                    zeroCnt = 0;
                    BoxPVCheck();
                    BoxPVText = BoxPV.ToString();

                    if (abnomalCnt > 4)
                    {
                        BoxPVColor = Global.gWarningColors[1];
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnomal Dimm Temp =  " + value);
                    }
                }
                else if (prevPV != 0 && value == 0)
                {
                    _zeroEscapeCnt = 0;
                    zeroCnt++;
					BoxPV = prevPV;  // 0 이 들어오면 이전값 유지
				}
                else
                {
                    _zeroEscapeCnt = 0;
                }

                if (zeroCnt > 6)
                {
                    BoxPV = value;
                    prevPV = value;
                    BoxPVText = string.Empty;//BoxPV.ToString();
                    _zeroEscapeCnt = 0;
                }
            }
			//일반 모드 (동일 로직 적용) 
			else
			{
                if (prevPV != 0 && value == 0)
                {
                    _zeroEscapeCnt = 0;
                    zeroCnt++;
                    BoxPV = prevPV;
                }
                else if (value != 0)
                {
                    if (prevPV != 0)
                    {
                        _zeroEscapeCnt = 0;
                        double diffval = prevPV - value;
                        //Test용 Log 추가 삭제필요
                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, Math.Abs(diffval).ToString() + ", prePV = " + prevPV.ToString());
                        if (Math.Abs(diffval) > Global.gConfigVM.NoiseTempDiff)
                        {
                            BoxPV = prevPV;
                            abnomalCnt++;
                        }
                        else
                        {
                            BoxPV = value;
                            //Console.WriteLine($"!!!! Rack : {MasterRack.RackViewIndex} Box : {BoxViewIndex} Temp = {BoxPV} , Value = {value}");
                            prevPV = value;
                            abnomalCnt = 0;
                        }
                    }
                    else
                    {
                        _zeroEscapeCnt ++;
                        if (_zeroEscapeCnt >= 3)
                        {
                            BoxPV = value;
                            prevPV = value;
                            abnomalCnt = 0;
                            _zeroEscapeCnt = 0;
                        }
                    }

                    BoxPVColor = Global.gWarningColors[0];
                    zeroCnt = 0;
                    BoxPVCheck();
                    BoxPVText = BoxPV.ToString();

                    if (abnomalCnt > 4)
                    {
                        BoxPVColor = Global.gWarningColors[1];
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Box" + BoxViewIndex + "] Abnomal Dimm Temp =  " + value);
                    }
                }
                else
                {
                    _zeroEscapeCnt = 0;
                }

                if (zeroCnt > 3)
                {
                    BoxPV = value;
                    prevPV = value;
                    BoxPVText = string.Empty;//BoxPV.ToString();
                    _zeroEscapeCnt = 0;
                }
            }
        }
        */
		#endregion

		public void BoxPVCheck()
		{
			if (BoxStat != Box_STAT.RUN) return;

			if (BoxPV == 0) return; // 0도는 통신 단절이므로 스킵

			// 1. Stable Time (안정화) 체크: 설정 시간 이후부터 검사 시작
			if (TimeStart.AddMinutes(Global.gConfigVM.StableTime) < DateTime.Now)
			{
				double stableDiff = Math.Abs(TargetControlSV - BoxPV);

				// 오차가 10도를 넘어갔을 때
				if (stableDiff > 10)
				{
					// 10도 이상 차이나기 시작한 최초 시점에 30초 타이머 세팅
					if (StableErrorTime == null)
					{
						StableErrorTime = DateTime.Now.AddSeconds(30);
					}
					else if (StableErrorTime < DateTime.Now)
					{
						// 유예 시간이 지났는데도 오차가 안 줄어들면 진짜 에러!
						BoxDefrost = DEFROST_TYPE.TEMPERROR;
						isLoggingHumidity = false;

						BoxStat = Box_STAT.DEFROST;

						// [수정] Thread.Sleep 제거: Task 내부에서 딜레이를 주도록 캡슐화
						int delayTime = (BoxIndex % 6) * 100;
						Task.Run(async () =>
						{
							await Task.Delay(delayTime);
							await DEFROSTStep();
						});
					}
				}
				else
				{
					// 오차가 10도 이내로 돌아오면 타이머 리셋
					StableErrorTime = null;
				}
			}

			// 2. High / Low Temp Alarm (단순 경고)
			if (Global.gConfigVM.HighTempAlarm < _boxpv || Global.gConfigVM.LowTempAlarm > _boxpv)
			{
				if (TempAlarmFlag)
				{
					if (TempAlarmTime == null)
					{
						TempAlarmTime = DateTime.Now.AddMinutes(Global.gConfigVM.AlarmCondition);
					}
					else if (TempAlarmTime < DateTime.Now)
					{
						Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[B{BoxViewIndex}] HighTemp = {Global.gConfigVM.HighTempAlarm} , LowTemp = {Global.gConfigVM.LowTempAlarm} , Cur = {_boxpv}");
					}
				}
				else
				{
					TempAlarmFlag = true;
					TempAlarmTime = null;
				}
			}
			else
			{
				TempAlarmFlag = false;
				TempAlarmTime = null;
			}

			// 3. High / Low Temp Error (장비 멈춤)
			if ((Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr) < _boxpv ||
				(Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr) > _boxpv)
			{
				if (TempErrorFlag)
				{
					if (TempErrorTime == null)
					{
						TempErrorTime = DateTime.Now.AddMinutes(Global.gConfigVM.TempErrCondition);
					}
					else if (TempErrorTime < DateTime.Now)
					{
						BoxDefrost = DEFROST_TYPE.ALARM;
						isLoggingHumidity = false;

						BoxStat = Box_STAT.DEFROST;

						// [수정] Thread.Sleep 제거
						int delayTime = (BoxIndex % 6) * 100;
						Task.Run(async () =>
						{
							await Task.Delay(delayTime);
							await DEFROSTStep();
						});
					}
				}
				else
				{
					TempErrorFlag = true;
					TempErrorTime = null;
				}
			}
			else
			{
				TempErrorFlag = false;
				TempErrorTime = null;
			}
		}

		#region 기존 BoxPvCheck
		/*
		 * int StableAlarmCnt = 0;
        public void BoxPVCheck()
        {
            if (BoxStat == Box_STAT.RUN)
            {
                if (BoxPV == 0)
                    return;
                //Stable Time after Set Temp 보다 +-10도 차이나면 이상으로 알람 후 10번이상 연속이면 멈춤
                if (TimeStart.AddMinutes(Global.gConfigVM.StableTime) < DateTime.Now)
                {
                    double stableDiff = Math.Abs(TargetControlSV - BoxPV);

                    if (stableDiff > 10)
                        StableAlarmCnt++;
                    else
                        StableAlarmCnt = 0;

                    if (StableAlarmCnt >= 10)
                    {
                        BoxDefrost = DEFROST_TYPE.TEMPERROR;
                        Thread.Sleep((BoxIndex % 6) * 100);
                        isLoggingHumidity = false;
                        Task.Run(() => DEFROSTStep());
                    }
                }

                //High , Low Temp Error Part

                if (Global.gConfigVM.HighTempAlarm < _boxpv || Global.gConfigVM.LowTempAlarm > _boxpv)
                {
                    if (TempAlarmFlag)
                    {
                        if (TempAlarmTime == null)
                        {
                            //Console.WriteLine("{0} : Box {1} Alarm Check Start", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            TempAlarmTime = DateTime.Now.AddMinutes(Global.gConfigVM.AlarmCondition);
                        }
                        else
                        {
                            if (TempAlarmTime < DateTime.Now)
                            {
                                //Console.WriteLine("{0} : Box {1} Alarm Check Over", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[B" + BoxViewIndex + "] HighTemp = " + Global.gConfigVM.HighTempAlarm.ToString() +
                            " , LowTemp = " + Global.gConfigVM.LowTempAlarm.ToString() + " , Cur = " + _boxpv.ToString());
                            }
                            else
                            {
                                //Console.WriteLine("{0} : Box {1} Alarm Check Waitting", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            }
                        }
                    }
                    else
                    {
                        //Console.WriteLine("{0} : Box {1}  TempTimeStart = null", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                        TempAlarmFlag = true;
                        TempAlarmTime = null;
                    }
                }
                else
                {
                    //Console.WriteLine("{0} : Box {1} TempAlarmFlag = false", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                    TempAlarmFlag = false;
                    TempAlarmTime = null;
                }

                //Console.WriteLine("Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr = {0} , Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr = {1}", Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr, Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr);
                if ((Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr) < _boxpv || (Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr) > _boxpv)
                {
                    if (TempErrorFlag)
                    {
                        if (TempErrorTime == null)
                        {
                            //Console.WriteLine("{0} : Box {1} Error Check Start", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            TempErrorTime = DateTime.Now.AddMinutes(Global.gConfigVM.TempErrCondition);
                        }
                        else
                        {
                            if (TempErrorTime < DateTime.Now)
                            {
                                //Console.WriteLine("{0} : Box {1} Error Check Over", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                                //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[B" + BoxViewIndex + "] HighTemp = " + Global.gConfigVM.HighTempAlarm.ToString() + " , LowTemp = " + Global.gConfigVM.LowTempAlarm.ToString() + " , Cur = " + _getpv.ToString());
                                BoxDefrost = DEFROST_TYPE.ALARM;
                                Thread.Sleep((BoxIndex % 6) * 100);
                                isLoggingHumidity = false;
                                Task.Run(() => DEFROSTStep());
                            }
                            else
                            {
                                //Console.WriteLine("{0} : Box {1} Error Check Waitting", DateTime.Now.ToString("HH:mm:ss"), BoxViewIndex);
                            }
                        }
                    }
                    else
                    {
                        TempErrorFlag = true;
                        TempErrorTime = null;
                    }
                }
                else
                {
                    TempErrorFlag = false;
                    TempErrorTime = null;
                }
            }
        }
        */
		#endregion

		public DateTime TimeStart {  get; set; }
        public DateTime TimeEnd { get; set; }
		Nullable<DateTime> StableErrorTime { get; set; }
		Nullable<DateTime> TempAlarmTime { get; set; }
        Nullable<DateTime> TempErrorTime { get; set; }
        public bool IsReadOnlySV { get; set; } = false;
        bool _bimtal_stat = true;
        public bool Bimetal_Stat 
        { 
            get => _bimtal_stat; 
            set
            {
                _bimtal_stat = value;
                if(_bimtal_stat == false)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_bimetal, true , BoxViewIndex);
                    Task.Run(() => TCon_BoxStop());
                    if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST)
                    {
                        lock (lockThis)
                        {
                            MasterRack.BoxRunCnt--;
                            Global.gMainPageVM.AllRackRunCnt --;
                            //Global.gMainPageVM.AllRackPreRunCnt--;
                        }
                    }
                    BoxStat = Box_STAT.ERROR;
                    //Console.WriteLine("BoxViewIndex [{0}] MasterRack.BoxRunCnt = {1}", BoxViewIndex, MasterRack.BoxRunCnt);
                    //바이메탈 alarm msg box 
                }
                else
                {
                    BoxStat = Box_STAT.STOP;
                    BoxShowBox = Visibility.Hidden;
                    //msg box Visibility false
                }
            }
        }
        public int Bimetal_Stat_Cnt { get; set; } = 0;
        bool _sopen_stat = true;
        public bool Sopen_Stat 
        { 
            get => _sopen_stat; 
            set
            {
                _sopen_stat = value;
                if( _sopen_stat == false)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_sOpen, true, BoxViewIndex);
                    Task.Run(() => TCon_BoxStop());
                    if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST)
                    {
                        lock (lockThis)
                        {
                            MasterRack.BoxRunCnt--;
                            Global.gMainPageVM.AllRackRunCnt --;
                            //Global.gMainPageVM.AllRackPreRunCnt--;
                        }
                    }
                    BoxStat = Box_STAT.ERROR;
                    //Console.WriteLine("BoxViewIndex [{0}] MasterRack.BoxRunCnt = {1}", BoxViewIndex, MasterRack.BoxRunCnt);
                    //sopen alarm msg box 
                }
                else
                {
                    BoxStat = Box_STAT.STOP;
                    BoxShowBox = Visibility.Hidden;
                    //msg box Visibility false
                }
            }
        }
        public Visibility BoxShowBox { get; set; } = Visibility.Hidden;
        public string BoxMsg { get; set; } = "";
        public SolidColorBrush BoxMsgColor { get; set; }
        public int Sopen_Stat_Cnt { get; set; } = 0;
        bool _tconisrun = false;
        public bool TconIsRun
        {
            get => _tconisrun;
            set
            {
                _tconisrun = value;
                /*
                if(_tconisrun)
                    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackTconstat = "RUN";
                else
                    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackTconstat = "STOP";
                */
            }
        }
		public void SyncBoxSetSVFromTcon(double tconSV)
		{
			double calcUI = tconSV;
			if (Global.gConfigVM != null)
			{
				// TCON 온도가 (타겟+오프셋)과 같다면, UI는 타겟 온도로 보여줘야 함
				if (Global.gConfigVM.IsUseTempOffset1 && Math.Abs(tconSV - (Global.gConfigVM.OffsetTarget1 + Global.gConfigVM.OffsetValue1)) < 0.01) calcUI = Global.gConfigVM.OffsetTarget1;
				else if (Global.gConfigVM.IsUseTempOffset2 && Math.Abs(tconSV - (Global.gConfigVM.OffsetTarget2 + Global.gConfigVM.OffsetValue2)) < 0.01) calcUI = Global.gConfigVM.OffsetTarget2;
				else if (Global.gConfigVM.IsUseTempOffset3 && Math.Abs(tconSV - (Global.gConfigVM.OffsetTarget3 + Global.gConfigVM.OffsetValue3)) < 0.01) calcUI = Global.gConfigVM.OffsetTarget3;
			}
			BoxSetSV = calcUI;
		}
		public void GetTconStatus(bool GetStatus)
        {
            if (GetStatus)
            {
                TconIsRun = GetStatus;
                //if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackTconstat = "RUN";
            }
            else
            {
                TconIsRun = GetStatus;
                //if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackTconstat = "STOP";
            }
        }
        double _getsv = 0.0;
        public double GetSV 
        { 
            get => _getsv; 
            set
            {
                _getsv = value;
                //if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                //    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackSV = _getsv.ToString();
            }
        }
        public void GetsvValue(double getSvVal)
        {
            GetSV = getSvVal;
            if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackSV = _getsv.ToString();
        }
        double _getpv = 0.0;
        public double GetPV
        {
            get => _getpv;
            set
            {
                _getpv = value;
                //if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                //    Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackPV = _getpv.ToString();
            }
        }
        public void GetpvValue(double getPvVal)
        {
            GetPV = getPvVal;
            if (Global.gMainVM.ShowEngineer == Visibility.Visible)
                Global.gEngineerVM.ManualMotor[MasterRack.RackIndex].Tcons[EachBoxIndex].EngrRackPV = _getpv.ToString();
        }
        public string BoxStatText { get; set; } = string.Empty;
        public string DefostTime {  get; set; } = string.Empty;

        public bool PreRunControlTemp {  get; set; } = false;   

        public DelegateCommand BoxCmdSetTemp { get; set; }
        public RackModel MasterRack { get; set; }
        public static System.Object lockThis = new System.Object();
        public void showBoxAlarm(ALRAM_TYPE type, string msg , bool isLogging, int ch)
        {
            BoxMsgColor = Global.gAlramColors[(int)type];
            BoxMsg = msg;
            BoxShowBox = Visibility.Visible;
            if (isLogging)
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, "[B" + ch + "]" + msg);
        }
        public BoxModel(int ch, int eachch, RackModel master) 
        {
            BoxViewIndex = (ch + 1);
            BoxIndex = ch;
            EachBoxIndex = eachch;
            MasterRack = master;
            BoxStat = Box_STAT.STOP;
            IsBoxTCUnit = true;
            initTempData();
            BoxCmdSetTemp = new DelegateCommand(p => { setTemp(); });
        }
        public void initTempData()
        {
            TempDatas.Add(new TempDataModel());
        }
        public async void BoxStatusClear()
        {
            //BoxShowBox = Visibility.Hidden;
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Box Status Clear In");
            showBoxAlarm(ALRAM_TYPE.INIT, Global.msg_ClearStatus, true, BoxViewIndex);

            if (BoxStat == Box_STAT.RUN || BoxStat == Box_STAT.DEFROST)
            {
                lock (lockThis)
                {
                    MasterRack.BoxRunCnt--;
                    Global.gMainPageVM.AllRackRunCnt--;
                    //Global.gMainPageVM.AllRackPreRunCnt--;
                }
            }
            
            if (BoxStat == Box_STAT.DEFROST)
            {
                BoxStat = Box_STAT.STOP;
                await Task.Delay(60 * 1000);
            }
            else
            {
                BoxStat = Box_STAT.STOP;
                await Task.Delay(30 * 1000);
            }
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Box Status Clear Out");

            BoxShowBox = Visibility.Hidden;

            BoxDefrost = DEFROST_TYPE.NORMAL;
            DefostTime = string.Empty;
            StableErrorTime = null;

			Sopen_Stat = true;
            Bimetal_Stat = true;
            isLoggingHumidity = false;
            MasterRack.Overlap_PreRun = false;
        }
        public bool PreStartTest()
        {
            //Console.WriteLine("{0} : PreStartTest", BoxIndex);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Pre Start Test");
            if((BoxStat == Box_STAT.DONE) || ((BoxMsg == Global.msg_TamsFile) && BoxShowBox == Visibility.Visible ))
            { 
                BoxShowBox = Visibility.Hidden;
                BoxStat = Box_STAT.STOP;
            }
            if (BoxStat != Box_STAT.IDLE && BoxStat != Box_STAT.STOP)
            {
                //msg 추가
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"==> [B{BoxViewIndex}] BoxStat {BoxStat}");
                return false;
            }
            if (MasterRack.RackMotorStat == MOTOR_STAT.WORKING)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Motor Downing, Retry Run Button Click");
                return false;
            }
            //slide open check 유무? 

            //if (!IsBoxTCUnit || IsBoxAS || MasterRack.IsSlideOpen) // true Slide Open , false Slide Close
            if (!IsBoxTCUnit || IsBoxAS)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Not TC Unit or AS Box, Cannot Run");
                return false;
            }
            if (MasterRack.IsSlideOpen && MasterRack.MotorStatText == "OPEN")
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Slide Opened, Cannot Run");
                return false;
            }
            if (IsBoxTams)
            {
                int tamscount = 0;
                while (true)
                {
                    if (setTamsTemp())
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Find Tams File OK.");
                        break;
                        //showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_TamsFile, true, BoxViewIndex);
                        //return false;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                        tamscount++;
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==> [B" + BoxViewIndex + "] Retty = " + tamscount + " Tams File Check" );
                        if (tamscount >= 5)
                        {
                            showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_TamsFile, true, BoxViewIndex);
                            return false;
                        }
                    }
                }
            }
            TempErrorTime = null;
            TempAlarmTime = null;
			StableErrorTime = null;
			BoxStat = Box_STAT.PRERUN;
            Bimetal_Stat_Cnt = 0;
            Sopen_Stat_Cnt = 0;
            HumidityValue = 0;
            DefostTime = string.Empty;
            isLoggingHumidity = false;
            //Errmsg.log" ?? 필요 유무?
            return true;
        }
        public async void startTest()
        {
            if (BoxStat != Box_STAT.PRERUN)
                return;

            GraphIndex = 0;
            //그래프 x측 초기화
            //Global.gGraphVM.TempSigs.OffsetX = TempDatas[0].TimeData[0].ToOADate();
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{MasterRack.RackViewIndex} - {BoxViewIndex}] startTempLog() Boxmodel startTest()");
            startTempLog();

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] " + BoxSetSV + " Temp Setting");
            setTemp();
            await Task.Delay(100);
            TCon_BoxRun();

            PreRunControlTemp = true;

            if (TargetControlSV < 30)
            {
                for (int count = 0; count <= 18; count++)
                {
                    if (BoxStat != Box_STAT.PRERUN)
                        return;
                    await Task.Delay(10000);
                    //Thread.Sleep(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] Monitoring Timer " + (count * 10).ToString());
                }
            }
            else
            {
                for (int count = 0; count < 3; count++)
                {
                    if (BoxStat != Box_STAT.PRERUN)
                        return;

                    await Task.Delay(10000);
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] Hot Temp Set Monitoring Timer " + (count * 10).ToString());
                }
            }
            // 습도 log save & display
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[B" + BoxViewIndex + "] SetTemp : " + BoxSetSV + " Monitoring Timer End");
            BoxStat = Box_STAT.RUN;
            PreRunControlTemp = false;
            TimeStart = DateTime.Now;
            if (Global.gMainPageVM.AllRackRunCnt == 0 && Global.gAlarmCom.AllRackStatus == false)
            {
                //Console.WriteLine("경광등 RUN");
                Global.gAlarmCom.AlarmWrite("R"); //경광등 RUN
                Global.gAlarmCom.AllRackStatus = true;
                Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.RUN;
            }
            lock (lockThis)
            {
                MasterRack.BoxRunCnt++;
                
                Global.gMainPageVM.AllRackRunCnt++;
                //Global.gMainPageVM.AllRackPreRunCnt--;
            }

            //Console.WriteLine("BoxViewIndex [{0}] BoxRunCnt = {1} ,AllRackRunCnt STOP IN = {2}", BoxViewIndex, MasterRack.BoxRunCnt, Global.gMainPageVM.AllRackRunCnt.ToString());
            isLoggingHumidity = true;
        }
        public async void PreStopTest()
        {
            BoxDefrost = DEFROST_TYPE.TAMS;
            await stopTest();
        }
        public async Task<bool> stopTest()
        {
            //Console.WriteLine("{0} :PrestopTest", BoxIndex);
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] Pre Stop Test");

            if (!IsBoxTCUnit || IsBoxAS)
                return false;

            if (BoxStat != Box_STAT.RUN)
            {
                //await Task.Delay(100);
                return false;
            }
            else
            {
                bool DefrostResult = await DEFROSTStep();
                endTempLog();
                isLoggingHumidity = false;
                return DefrostResult;
                
            }
        }
        public async Task<bool> DEFROSTStep() //DEFROST
        {
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] DefrostStep In");
            BoxStat = Box_STAT.DEFROST;

            //double finalTemp = 30.0;
            int finalWaitTime = 18;
            int Cnt = 0;

            int sub_targetwait;
            int min;
            int sec;
            int step_waittime;
            int step_temp;
            int targettemp;
            
            /*
            int lowtempCriteria;
            double lowtempsetp;
            int lowtempwaittime;
            */
            try
            {
                targettemp = Global.gConfigVM.TargetTemp;
                step_temp = Global.gConfigVM.TempStep;
                step_waittime = Global.gConfigVM.TempStepTime;
                sub_targetwait = Global.gConfigVM.TargetWaitTime * 60;
                min = sub_targetwait % 3600 / 60;
                sec = sub_targetwait % 3600 % 60;
                //lowtempCriteria = Global.gConfigVM.LowTempCriteria;
                //lowtempsetp = Global.gConfigVM.LowTempDefrostStep;
                //lowtempwaittime = Global.gConfigVM.LowTempDefrostTime;
            }
            catch
            {
                targettemp = 60;
                step_temp = 1;
                step_waittime = 60;
                sub_targetwait = 20 * 60;
                min = sub_targetwait % 3600 / 60;
                sec = sub_targetwait % 3600 % 60;
                //lowtempCriteria = 10;
                //lowtempsetp = 0.5;
                //lowtempwaittime = 60;
            }
            double DEFROST_val = GetPV;
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Box Temp = " + DEFROST_val.ToString());
            if (DEFROST_val <= -10.0) DEFROST_val = -9.0;

            int TVal_Int = Convert.ToInt16(Math.Truncate(DEFROST_val) * 10);
            double TVal_Dou = Math.Truncate(DEFROST_val) * 10;

            if (TVal_Int < (targettemp * 10))
            {
                while(true)
                {
                    if(BoxStat != Box_STAT.DEFROST)
                    {
                        return false;
                    }

                    TVal_Dou = TVal_Dou + (step_temp * 10);

                    /*
                    if(TVal_Dou > (lowtempCriteria *10))
                        TVal_Dou = TVal_Dou + (step_temp * 10);
                    else
                        TVal_Dou = TVal_Dou + (lowtempsetp * 10);
                    */

                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Temp = " + TVal_Dou.ToString());

                    if(TVal_Dou < Convert.ToDouble(targettemp * 10))
                    {
                        setTemp(TVal_Dou);
                        await Task.Delay(100);
                    }
                    else
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Target Temp is OK = " + TVal_Dou.ToString());

                        setTemp(TVal_Dou);
                        await Task.Delay(100);

                        while(sub_targetwait >= 0)
                        {
                            if (BoxStat != Box_STAT.DEFROST)
                            {
                                return false;
                            }

                            DefostTime = min.ToString() + " : " + sec.ToString();
                            await Task.Delay(1000);
                            sub_targetwait -= 1;
                            min = sub_targetwait % 3600 / 60;
                            sec = sub_targetwait % 3600 % 60;
                        }
                        break;
                    }

                    await Task.Delay(step_waittime * 1000);

                    /*
                    if (TVal_Dou > (lowtempCriteria * 10))
                        await Task.Delay(step_waittime * 1000);
                    else
                        await Task.Delay(lowtempwaittime * 1000);
                    */
                }
            }
            else
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] NO Defrost = " + TVal_Dou.ToString());
            }

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Temp Down ");

            //setTemp(Convert.ToDouble(Global.gConfigVM.TargetTemp + 5) * 10); //? 
            setTemp(40 * 10);

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Waiting Time");
            while (true) // 무조건 3분 대기하게 변경
            {
                if (BoxStat != Box_STAT.DEFROST)
                {
                    return false;
                }
                //FinmalTemp 보다 온도다 낮으면 종료
                //if((GetPV <= finalTemp) || (Cnt > finalWaitTime))
                if (Cnt > finalWaitTime)
                {
                    DefostTime = "Done";
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Waiting Finished");
                    break;
                }
                await Task.Delay(10000);
                Cnt++;
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] WaitTimer " + (Cnt * 10).ToString());
            }

            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] Defrost Process Done");

            //Console.WriteLine("BoxViewIndex [{0}] AllRackRunCnt STOP IN = {1}", BoxViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());

            //Console.WriteLine("BoxViewIndex [{0}] AllRackRunCnt STOP OUT = {1}", BoxViewIndex, Global.gMainPageVM.AllRackRunCnt.ToString());
            lock (lockThis)
            {
                MasterRack.BoxRunCnt--;
                Global.gMainPageVM.AllRackRunCnt--;
                //Global.gMainPageVM.AllRackPreRunCnt--;
            }
            //Console.WriteLine("BoxViewIndex [{0}] BoxRunCnt = {1} ,AllRackRunCnt STOP IN = {2}", BoxViewIndex, MasterRack.BoxRunCnt, Global.gMainPageVM.AllRackRunCnt.ToString());

            //CenterTemp 체크하여 모터 올리지 말지 Check
            double CenterTemp = 25.0;
            double HumTempVal = Convert.ToDouble(Hum_TempSenorVal);
            //Console.WriteLine("BoxViewIndex [{0}] GetPV = {1} , HumTempVal = {2}", BoxViewIndex, GetPV , HumTempVal);
            //if (GetPV < CenterTemp|| GetPV < HumTempVal)
            if (GetPV < CenterTemp)
                BoxDefrost = DEFROST_TYPE.LOWTEMP;

            if (BoxDefrost == DEFROST_TYPE.HUMIDITY || BoxDefrost == DEFROST_TYPE.TAMS || BoxDefrost == DEFROST_TYPE.ALARM || BoxDefrost == DEFROST_TYPE.TEMPERROR)
            {
                //온도조절기 스탑하고 모터 업하러
                TCon_BoxStop();
                if (MasterRack.BoxRunCnt == 0)
                    MasterRack.RackCheckStop();

                if (BoxDefrost == DEFROST_TYPE.HUMIDITY)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Humidity, true, BoxViewIndex);
                    BoxStat = Box_STAT.ERROR;
                }
                else if(BoxDefrost == DEFROST_TYPE.ALARM)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Alarm, true, BoxViewIndex);
                    BoxStat = Box_STAT.ERROR;
                }
                else if(BoxDefrost == DEFROST_TYPE.TEMPERROR)
                {
                    showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Unbalance, true, BoxViewIndex);
                    BoxStat = Box_STAT.ERROR;
                }

            }
            else if (BoxDefrost == DEFROST_TYPE.LOWTEMP)
            {
                showBoxAlarm(ALRAM_TYPE.ERROR, Global.msg_Cbox_25_temp, true, BoxViewIndex);
                BoxStat = Box_STAT.ERROR;
            }
            //온도조절기 스탑하고 모터 업하러
            TCon_BoxStop();
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "==>[B" + BoxViewIndex + "] DefrostStep Out");
            return true;
        }
        public void setTemp()
        {
            try
            {
                //Console.WriteLine("{0} : setTemp : {1}" , BoxIndex, BoxSetSV);
                if (BoxStat == Box_STAT.DEFROST)
                    return;

                byte ch = (byte)BoxViewIndex;
                string iTempVal = Convert.ToInt16(TargetControlSV * 10).ToString("X4");
                Global.gTconCom.SetTempMsg(ch, iTempVal);
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] SetTemp : " + BoxSetSV);
                TimeStart = DateTime.Now;
                TempErrorTime = null;
                TempAlarmTime = null;
				StableErrorTime = null;
				SVST(SVC.TCON);
                SampleLongTerm.Clear();
                SampleShortTerm.Clear();
            }
            catch (Exception e) { }
        }
        public void ControlsetTemp(double getSetSV)
        {
            byte ch = (byte)BoxViewIndex;
            string iTempVal = Convert.ToInt16(getSetSV * 10).ToString("X4");
            Global.gTconCom.SetTempMsg(ch, iTempVal);
            
            //제거 동작중 제어에서는 초기화 하면 안됌
            //TimeStart = DateTime.Now;
            //TempErrorTime = null;
            //TempAlarmTime = null;
        }
        public void setTemp(double getSetSV)
        {
            //Console.WriteLine("{0} : setTemp : {1}", BoxIndex, getSetSV);

            byte ch = (byte)BoxViewIndex;
            string iTempVal = Convert.ToInt16(getSetSV).ToString("X4");
            Global.gTconCom.SetTempMsg(ch, iTempVal);
            TimeStart = DateTime.Now;
            TempErrorTime = null;
            TempAlarmTime = null;
			StableErrorTime = null;

		}
        public void Read_BoxTConStat()
        {
            //if (!IsBoxTCUnit)
            //    return;

            byte ch = (byte)BoxViewIndex;
            Global.gTconCom.TConStatMsg(ch);
        }
        public void TCon_BoxStop()
        {
            if (!IsBoxTCUnit)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] BoxTCUnit UnCheck, NO STOP");
                return;
            }
            byte ch = (byte)BoxViewIndex;
            Global.gTconCom.RunStopMsg(ch, 0x01);
        }
        public void TCon_BoxRun()
        {
            if (!IsBoxTCUnit)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[" + (MasterRack.RackViewIndex) + "-" + BoxViewIndex + "] BoxTCUnit UnCheck, NO RUN");
                return;
            }
            byte ch = (byte)BoxViewIndex;
            Global.gTconCom.RunStopMsg(ch, 0x00);
        }
        public bool setTamsTemp()
        {
            string fn = $"{Global.gConfigVM.TamsSVPath}\\" +
                $"{Global.gConfigVM.TamsSVFolder}{BoxViewIndex}\\" +
                $"{Global.gConfigVM.TamsSVFile}";
            FileInfo info = new FileInfo(fn);

            if (!info.Exists) return false;
            string[] lines = File.ReadAllLines(@info.FullName);
            foreach (string line in lines)
            {
                if (line.Contains("TEMP"))
                {
                    string[] buf = line.Split(':');
                    if (!buf[1].Equals(""))
                    {
                        BoxSetSV = double.Parse(buf[1]);
                        return true;
                    }
                }
            }
            return false;
        }
        public bool findEndFile()
        {
            string fn = $"{Global.gConfigVM.TamsEndPath}\\" +
                $"{Global.gConfigVM.TamsEndFolder}{BoxViewIndex}\\" +
                $"{Global.gConfigVM.TamsEndFile}";

            FileInfo finfo = new FileInfo(fn);

            if (finfo.Exists)
                return true;

            return false;
        }
        public void ProcessBoxSV() // lp 온도 제어 로직
        {
            //포트가 닫혀있으면 Skip
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if(BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty) return;

            double ctrl = GetSV;
            double diff = TargetControlSV - GetPV;
            double setp = Global.gConfigVM.ControlSetp;
            double offset = Global.gConfigVM.ControlOffset;
            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            if (Math.Abs(diff) <= Global.gConfigVM.ControlDiff)
            {
                if (BoxPV <= TargetControlSV - offset)
                    ctrl += setp;
                else if (BoxPV >= TargetControlSV + offset)
                    ctrl -= setp;
            }
            else
                ctrl = TargetControlSV;

            //if(ctrl > BoxSetSV) ctrl = BoxSetSV;
            //else if (ctrl < min) ctrl = min;
            if(ctrl < min) ctrl = min;
            else if(ctrl > max) ctrl = max;

            //setTemp(ctrl);
            ControlsetTemp(ctrl);
        }

        double prevDimAvg = 0;
        public void preTempData()
        {
            prevDimAvg = BoxPV;
        }
        public void sampleingTempData()
        {
            SampleShortTerm.Add(BoxPV - prevDimAvg);
            if (SampleShortTerm.Count > ShortTermCount)
                SampleShortTerm.RemoveAt(0);

            SampleLongTerm.Add(BoxPV);
            if (SampleLongTerm.Count > LongTermCount)
                SampleLongTerm.RemoveAt(0);
        }
        public void onlylongsampleingTempData()
        {
            SampleLongTerm.Add(BoxPV);
            if (SampleLongTerm.Count > LongTermCount)
                SampleLongTerm.RemoveAt(0);
        }
        public (double, int, int) AnalysTrend(Collection<double> collect)
        {
            // collect의 복사본을 만들어서 안전하게 사용
            var copy = collect.ToList();

            double avgSum = copy.Sum();
            int down = 0, up = 0;
            foreach (double val in copy)
            {
                if (val > 0) up++;
                if (val < 0) down++;
            }


            return (avgSum, up, down);
        }

        // 표준 편차
        public double calcSD(Collection<double> collect)
        {
            if (collect.Count == 0) return 0;

            var copy = collect.ToList();

            double sum = copy.Sum();
            double mean = sum / copy.Count;

            double variance = copy.Sum(value => Math.Pow(value - mean, 2));
            return Math.Sqrt(variance / copy.Count);
        }
        public void SVST(SVC set)
        {
            SVCtrl = (int)set;
        }
        public bool checkSV(SVC check)
        {
            int ch = (int)check;
            if ((SVCtrl & ch) != 0) return true;
            return false;
        }
        public double TradingOrgSV = 0;
        public int Level1Check = 0;

		#region 기존 로직
		/*
        public void ProcessBoxSV_Logic_Org()
        {
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if (BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty)
            {
                SampleLongTerm.Clear();
                SampleShortTerm.Clear();
                return;
            }

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = GetSV; //RealTime SV

            //double DIM_SV_Diff = BoxSetSV - BoxPV; //Org - LPCAMMTemp
            double DIM_SV_Diff = TargetControlSV - BoxPV;
            double ctrl = GetSV;

            TRADING trading = TRADING.RISE;

            if (DIM_SV_Diff < 0) trading = TRADING.FALL;

            double direction = 1;

            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);
            
            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff) //몇도 내 들어오는지에 따라 제어방법 상이
            {
                //////////////////////////
                //////////////  Done ACCELERATION
                /////////////////////////////
                if (checkSV(SVC.ACCELERATION))
                {
                    ctrl = TargetControlSV;
                    SVST(SVC.TCON);
                }

                //////////////////////////
                ////  DOWN Action
                //////////////////////////

                if (checkSV(SVC.DOWN))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);

                    if (up > 0)  // 오르는중.
                    {
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.DOWN;
                        Console.WriteLine($"--- Down 제어완료 ---");
                    }

                    else if (down > 2)
                    {
                        // [수정] DIM_SV_Diff * 1.5 대신 0.5 사용 및 최대폭 1.0도 제한
                        double adjust = Math.Min(1.0, DIM_SV_Diff * 0.5);
                        ctrl += adjust;
                        //기존
                        //ctrl += (DIM_SV_Diff / 2);
                    }

                    //SV 무한 증가 방지 //add 0618

                    else if (Math.Abs(avg) < 0.2 && up == 0 && down == 0)
                    {
                        // 완전 정체 → 종료
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.DOWN;
                        Console.WriteLine("--- 평균 정체 감지 → DOWN 종료 ---");
                    }
                    goto SVDone;
                }

                //////////////////////////
                ////  UP Action
                //////////////////////////

                if (checkSV(SVC.UP))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);

                    if (down > 0)
                    {
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 제어완료 ---");
                    }
                    else if (up > 2)
                    {
                        double adjust = Math.Min(1.0, DIM_SV_Diff * 0.5);
                        ctrl -= adjust;
                        //기존
                        //ctrl -= (DIM_SV_Diff / 2);
                    }
                    //SV 무한 감소 방지 //add 0618
                    else if (Math.Abs(avg) < 0.2 && up == 0 && down == 0)
                    {
                        // 완전 정체 → 종료
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine("--- 평균 정체 감지 → UP 종료 ---");
                    }

                    goto SVDone;
                }

                //////////////////////////

                //////////// DO LOGN Term

                /////////////////////////////

                if (SampleLongTerm.Count == LongTermCount)
                {
                    double avg = SampleLongTerm.Average();
                    double sd = calcSD(SampleLongTerm);

                    //Console.WriteLine($"{Name} SD : {sd}");

                    if (sd < 2) SVST(SVC.STABLE);

                    if (checkSV(SVC.STABLE))
                    {
                        double stable_diff = Math.Abs(TargetControlSV - avg);
                        stable_diff = Math.Round(stable_diff, 1);

                        double Control_diff = Math.Abs(GetSV - GetPV);
                        Control_diff = Math.Round(Control_diff, 1);

                        if ((stable_diff > 0.5) && (Control_diff <= 1))
                        {
                            double org = ctrl;
                            //ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            ctrl += Math.Round((stable_diff / 2) * direction, 1);
                            Console.WriteLine($"{BoxIndex} LongTerm Adjust {org} -> {ctrl}");
                            SampleLongTerm.Clear();

                            goto SVDone;

                        }
                        //else if((stable_diff > 0.5) && (Control_diff >= 5))
                        else if ((stable_diff > 1) && (Control_diff >= Global.gConfigVM.ControlDiff) && (ctrl > Global.gConfigVM.ControlMinTemp + 0.1) && (ctrl < Global.gConfigVM.ControlMaxTemp - 0.1))
                        {
                            ctrl = GetPV + (direction * 2.0);
                            Console.WriteLine($"{BoxIndex} LongTerm Adjust Chage SV {stable_diff} , {Control_diff} , {ctrl}");
                            SampleLongTerm.Clear();

                            goto SVDone;
                        }
                        else
                        {
                            Console.WriteLine($"{BoxIndex} LongTerm Adjust Wait {stable_diff} , {Control_diff}");
                        }
                    }
                }

                //////////////////////////////
                ///////////// Prevent Down / UP Action 
                /////////////////////////////////

                if (PrevTrading != trading)
                    SampleShortTerm.Clear();

                PrevTrading = trading;

                //////////////////////////
                ////////////////  DO SortTerm
                /////////////////////////////

                if (SampleShortTerm.Count == ShortTermCount)
                {
                    if (checkSV(SVC.STABLE))
                    {
                        if (!(checkSV(SVC.DOWN) || checkSV(SVC.UP)))
                        {
                            if (DIM_SV_Diff <= 5)
                            {
                                (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                                double sd = Math.Round(calcSD(SampleShortTerm), 3);

                                avg = Math.Round(avg, 1);

                                //Console.WriteLine($"{Name} SortTerm 편차:{sd} avg:{avg} u:{up} d:{down} T:{trading.ToString()}");

                                if (avg <= -0.8 && down > 1 &&
                                    ((trading == TRADING.RISE) ||
                                    (trading == TRADING.FALL && DIM_SV_Diff < 0.6)))
                                {
                                    Level1Check = 0;
                                    Level = 1;
                                    TradingOrgSV = ctrl;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    ctrl += Math.Round(DIM_SV_Diff * 1.5);

                                    Console.WriteLine($"다운제어 시작 SV: {TradingOrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.DOWN;
                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }

                                if (avg >= 0.8 && up > 1 &&
                                    ((trading == TRADING.FALL) ||
                                    (trading == TRADING.RISE && DIM_SV_Diff < 0.6)))
                                {
                                    Level = 1;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    TradingOrgSV = ctrl;
                                    ctrl -= Math.Round(DIM_SV_Diff * 1.5);

                                    Console.WriteLine($"업 제어 시작 SV: {TradingOrgSV} -> {ctrl}");
                                    SVCtrl |= (int)SVC.UP;
                                    SampleShortTerm.Clear();
                                    SampleLongTerm.Clear();
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                SVST(SVC.ACCELERATION);
                ctrl = TargetControlSV + (5.0 * direction);
            }

        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
            {
                Console.WriteLine("{0} : Control SV : {1}", BoxIndex, ctrl);
                ControlsetTemp(ctrl);
            }
        }
        */
		#endregion

		public void ProcessBoxSV_Logic_Org()
        {
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if (BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty)
            {
                SampleLongTerm.Clear();
                SampleShortTerm.Clear();
                return;
            }

            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = GetSV; // TCON에 들어가 있는 현재 세팅 온도
									   //double DIM_SV_Diff = BoxSetSV - BoxPV; //Org - LPCAMMTemp
			double DIM_SV_Diff = TargetControlSV - BoxPV;
            double ctrl = GetSV;

			TRADING trading = (DIM_SV_Diff < 0) ? TRADING.FALL : TRADING.RISE;
			double direction = (trading == TRADING.FALL) ? -1.0 : 1.0;

			DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            bool isAdjusted = false;

            if (DIM_SV_Diff < Global.gConfigVM.ControlDiff) //몇도 내 들어오는지에 따라 제어방법 상이
            {
				// 1. ACCELERATION 상태 해제 (가속 종료, 정상 목표 온도로 복귀)
				if (checkSV(SVC.ACCELERATION))
                {
                    ctrl = TargetControlSV;
                    SVST(SVC.TCON);
					isAdjusted = true;
				}

				// 2. DOWN Action 제어 중일 때
				if (checkSV(SVC.DOWN))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);
                    if (up > 0)  // 온도가 오르기 시작함 -> 목적 달성, DOWN 종료
					{
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.DOWN;
                        //Console.WriteLine($"--- Down 제어완료 ---");
                    }
                    else if(down > 2)
                    {
						// [수정] DIM_SV_Diff * 1.5 대신 0.5 사용 및 최대폭 1.0도 제한
						double adjust = Math.Min(1.0, DIM_SV_Diff * 0.5);
						ctrl += adjust;
						isAdjusted = true;
						//기존
						//ctrl += (DIM_SV_Diff / 2);
					}
                    //SV 무한 증가 방지 //add 0618
                    else if (Math.Abs(avg) < 0.2 && up == 0 && down == 0)
                    {
                        // 완전 정체 → 종료
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.DOWN;
                        Console.WriteLine("--- 평균 정체 감지 → DOWN 종료 ---");
                    }
				}

				// 3. UP Action 제어 중일 때
				if (!isAdjusted && checkSV(SVC.UP))
                {
                    (double avg, int up, int down) = AnalysTrend(SampleShortTerm);

                    if (down > 0)
                    {
                        SampleShortTerm.Clear();
                        SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 제어완료 ---");
                    }
                    else if(up > 2)
                    {
						double adjust = Math.Min(1.0, DIM_SV_Diff * 0.5);
						ctrl -= adjust;
						isAdjusted = true;
						//기존
						//ctrl -= (DIM_SV_Diff / 2);
					}
					else if (Math.Abs(avg) < 0.3) // [수정] 현실적인 정체 판단
					{
						SampleShortTerm.Clear();
						SVCtrl ^= (int)SVC.UP;
						Console.WriteLine("--- 평균 정체 감지 → UP 종료 ---");
					}
				}

				// 4. 장기(Long Term) 안정화 제어
				if (!isAdjusted && SampleLongTerm.Count == LongTermCount)
                {
                    double avg = SampleLongTerm.Average();
                    double sd = calcSD(SampleLongTerm);

                    //Console.WriteLine($"{Name} SD : {sd}");

                    if (sd < 2) SVST(SVC.STABLE);

					if (checkSV(SVC.STABLE))
					{
						double stable_diff = Math.Round(Math.Abs(TargetControlSV - avg), 1);
						double Control_diff = Math.Round(Math.Abs(GetSV - GetPV), 1);

						if ((stable_diff > 0.5) && (Control_diff <= 1))
						{
							double org = ctrl;
							ctrl += Math.Round((stable_diff / 2) * direction, 1);
							Console.WriteLine($"{BoxIndex} LongTerm Adjust {org} -> {ctrl}");
							SampleLongTerm.Clear();
							isAdjusted = true;
						}
						else if ((stable_diff > 1) && (Control_diff >= Global.gConfigVM.ControlDiff) &&
								 (ctrl > Global.gConfigVM.ControlMinTemp + 0.1) &&
								 (ctrl < Global.gConfigVM.ControlMaxTemp - 0.1))
						{
							ctrl = GetPV + (direction * 2.0);
							Console.WriteLine($"{BoxIndex} LongTerm Adjust Chage SV {stable_diff} , {Control_diff} , {ctrl}");
							SampleLongTerm.Clear();
							isAdjusted = true;
						}
					}
				}

                if (PrevTrading != trading)
                    SampleShortTerm.Clear();

                PrevTrading = trading;

                if (!isAdjusted && SampleShortTerm.Count == ShortTermCount)
                {
					if (checkSV(SVC.STABLE) && !(checkSV(SVC.DOWN) || checkSV(SVC.UP)))
					{
						if (DIM_SV_Diff <= 5)
						{
							(double avg, int up, int down) = AnalysTrend(SampleShortTerm);
							avg = Math.Round(avg, 1);

							// 하강 오버슈트 방지 제어 발동
							if (avg <= -0.8 && down > 1 &&
								((trading == TRADING.RISE) || (trading == TRADING.FALL && DIM_SV_Diff < 0.6)))
							{
								Level = 1;
								TradingOrgSV = ctrl;
								ctrl += Math.Round(DIM_SV_Diff * 1.5);

								Console.WriteLine($"다운제어 시작 SV: {TradingOrgSV} -> {ctrl}");
								SVCtrl |= (int)SVC.DOWN;

								SampleShortTerm.Clear();
								SampleLongTerm.Clear();
							}
							// 상승 오버슈트 방지 제어 발동
							else if (avg >= 0.8 && up > 1 &&
								((trading == TRADING.FALL) || (trading == TRADING.RISE && DIM_SV_Diff < 0.6)))
							{
								Level = 1;
								TradingOrgSV = ctrl;
								ctrl -= Math.Round(DIM_SV_Diff * 1.5);

								Console.WriteLine($"업 제어 시작 SV: {TradingOrgSV} -> {ctrl}");
								SVCtrl |= (int)SVC.UP;

								SampleShortTerm.Clear();
								SampleLongTerm.Clear();
							}
						}
					}
				}
            }
            else
            {
                SVST(SVC.ACCELERATION);
                ctrl = TargetControlSV + (5.0 * direction);
            }

            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
            {
				Console.WriteLine("{0} : Control SV : {1}", BoxIndex, ctrl);
				ControlsetTemp(ctrl);
            }
        }
		
		public void ProcessBoxSV_Logic_Long()
        {
            if (Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort == null) return;
            if (!Global.gMainPageVM.cc[MasterRack.RackIndex].CapturePort.IsOpen) return;
            if (BoxPVText == "0" || BoxPVText == "" || BoxPVText == string.Empty)
            {
                SampleLongTerm.Clear();
                return;
            }
            double max = Global.gConfigVM.ControlMaxTemp;
            double min = Global.gConfigVM.ControlMinTemp;

            double prevTarget = GetSV; //RealTime SV
            double DIM_SV_Diff = TargetControlSV - BoxPV; //Org - LPCAMMTemp
            double ctrl = GetSV;

            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            try
            {
                if (DIM_SV_Diff < Global.gConfigVM.ControlDiff) //몇도 내 들어오는지에 따라 제어방법 상이
                {
                    //////////////////////////
                    //////////////  Done ACCELERATION
                    /////////////////////////////
                    if (checkSV(SVC.ACCELERATION))
                    {
                        ctrl = TargetControlSV;
                        SVST(SVC.TCON);
                    }

                    //////////////////////////
                    //////////// DO LOGN Term
                    /////////////////////////////
                    if (SampleLongTerm.Count == LongTermCount)
                    {
                        double avg = SampleLongTerm.Average();
                        double sd = calcSD(SampleLongTerm);

                        //Console.WriteLine($"{Name} SD : {sd}");

                        if (sd < 2) SVST(SVC.STABLE);

                        if (checkSV(SVC.STABLE))
                        {
                            double stable_diff = Math.Abs(TargetControlSV - avg);
                            stable_diff = Math.Round(stable_diff, 1);

                            if (stable_diff > 0.5)
                            {
                                double org = ctrl;
                                //ctrl += Math.Round((stable_diff / 2) * direction, 1);
                                ctrl += Math.Round((stable_diff / 2) * direction, 1);
                                Console.WriteLine($" LongTerm Adjust {org} -> {ctrl}");
                                SampleLongTerm.Clear();
                                goto SVDone;
                            }
                        }
                    }
                }
                else
                {
                    SVST(SVC.ACCELERATION);
                    ctrl = TargetControlSV + (5.0 * direction);
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[ProcessBoxSV] 예외 발생: {ex.Message}");
            }

        SVDone:
            if (ctrl > max) ctrl = max;
            else if (ctrl < min) ctrl = min;

            if (prevTarget != ctrl)
                ControlsetTemp(ctrl);
        }

        public void startTempLog()
        {
            if (TempSerilog != null)
                TempSerilog.Dispose();
            string file = $"{Global.gConfigVM.TempLogPath}\\Box{BoxViewIndex}@{DateTime.Now.ToString("yyyy_MM_dd_HHmm")}" + 
                $"{(Global.gConfigVM.GraphMode ? "_NoRun" : "")}.csv";
            TempSerilog = new LoggerConfiguration()
                .WriteTo.File(file, rollingInterval: RollingInterval.Infinite, outputTemplate: "{Message:lj}{NewLine}")
                .CreateLogger();

            //ADD 
            if(BoxIndex == 0 && Global.gMainPageVM.AllTempSerilog != null)
                Global.gMainPageVM.AllTempSerilog.Dispose();

        }
        public void addTempLog()
        {
            if (TempSerilog == null)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[{MasterRack.RackViewIndex} - {BoxViewIndex}]startTempLog() Boxmodel addTempLog()");
                startTempLog();
            }
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();
            strBuilder.Append(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));

            strBuilder.Append("," + TempDatas[0].Temp);
            strBuilder.Append("," + GetSV);
            strBuilder.Append("," + GetPV);
            TempSerilog.Information(strBuilder.ToString());
        }
        public void endTempLog()
        {
            if (TempSerilog == null) return;

            StringBuilder strBuilder = new StringBuilder();

            strBuilder.Append("\r\n");

            TempSerilog.Information(strBuilder.ToString());

            if (TempSerilog == null)
                return;
            TempSerilog.Dispose();
            TempSerilog = null;
            Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[{MasterRack.RackViewIndex} - {BoxViewIndex}]endTempLog()");
        }
    }
}
