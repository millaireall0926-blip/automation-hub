using HandyControl.Controls;
using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using Serilog;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using MessageBox = HandyControl.Controls.MessageBox;

namespace LP_MMTCRack.ViewModel
{
    public class MotorLog : BaseVM
    {
        public string Time { get; set; }
        public string Message { get; set; }
        public MotorLog(string msg)
        {
            Time = DateTime.Now.ToString("yyyy-MM-dd H:mm:ss");
            Message = msg;
        }
    }
    public class ParaLog : BaseVM
    {
        public string Time { get; set; }
        public string Message { get; set; }
        public ParaLog(string msg)
        {
            Time = DateTime.Now.ToString("yyyy-MM-dd H:mm:ss");
            Message = msg;
        }
    }
    public class TconParameter : BaseVM
    {
        public ushort Address { get; set; } // D-Register 번호
        public string Symbol { get; set; }
        public string Name { get; set; }
        public double Scale { get; set; } = 1.0;
        public bool CanWrite { get; set; }
        public string Group { get; set; }

		private string _value = string.Empty;

		public string Value
		{
			get => _value;
			set { _value = value; PC(nameof(Value)); }
		}

		private string _editValue = string.Empty;

		public string EditValue
		{
			get => _editValue;
			set { _editValue = value; PC(nameof(EditValue)); }
		}
	}
    public class EngineerVM :BaseVM
    {
        private bool _is12Columns;
        public bool Is12Columns
        {
            get { return _is12Columns; }
            set
            {
                _is12Columns = value;
                OnPropertyChanged(nameof(Is12Columns));
                UpdateColumnCount();
            }
        }

        private int _columnCount;
        public int ColumnCount
        {
            get { return _columnCount; }
            set
            {
                _columnCount = value;
                OnPropertyChanged(nameof(ColumnCount));
            }
        }

        public void UpdateColumnCount()
        {
            ColumnCount = Is12Columns ? 12 : 18;
        }
        
        public int uiPbint { get; set; } = 0;
        public ListView ListParaLog;
        ObservableCollection<ParaLog> _paraLogs = new ObservableCollection<ParaLog>();
        public ObservableCollection <ParaLog> ParaLogs { get { return _paraLogs; } set { _paraLogs = value; PC("ParaLogs"); } }
        public ListView ListMotorLog;
        ObservableCollection<MotorLog> _motorlogs = new ObservableCollection<MotorLog>();
        public ObservableCollection<MotorLog> MotorLogs { get { return _motorlogs; } set { _motorlogs = value; PC("MotorLogs"); } }
        ObservableCollection<ManMotorModel> _manualmotor = new ObservableCollection<ManMotorModel>();
        public ObservableCollection<ManMotorModel> ManualMotor { get { return _manualmotor; } set { _manualmotor = value; PC("ManualMotor"); } }
        public DelegateCommand ParaValueRead { get; set; }
        public DelegateCommand SingleValueWrite {  get; set; } 
        public BackgroundWorker _ReadParaWorker = null;

        public int TconWriteCnt { get; set; } = 0;
        public int TconReadCnt { get; set; } = 0;

        public string TConLoopTime {  get; set; } = string.Empty;

		public int LastReadStartAddress { get; set; } = 1;

		private int _selectedIndex;
		public int SelectedIndex
		{
			get => _selectedIndex;
			set
			{
				if (_selectedIndex == value) return;
				_selectedIndex = value;
				OnPropertyChanged(nameof(SelectedIndex)); // PropertyChanged 알림

				// 중요: 탭이 바뀔 때마다 파라미터 리스트를 갱신합니다.
				RefreshParameters();
			}
		}

		//채널 선택 목록
		public ObservableCollection<int> ChannelList { get; set; }

        // 현재 선택된 채널 번호
        private int _selectedChannel = 1;
        public int SelectedChannel
        {
            get => _selectedChannel;
            set
            {
                _selectedChannel = value;
				PC(nameof(SelectedChannel));
				RefreshParameters();
			}
        }

        public ObservableCollection<TconParameter> CurrentParams { get; set; }

		public List<TconParameter> AllParameters = new List<TconParameter>();

		public EngineerVM()
        {
            foreach (RackModel rm in Global.gMainPageVM.Racks)
            {
                ManualMotor.Add(new ManMotorModel(rm.RackViewIndex, rm));
            }

            ChannelList = new ObservableCollection<int>(Enumerable.Range(1, Global.gMainPageVM.AllBoxCnt * 6));
            CurrentParams = new ObservableCollection<TconParameter>();

            InitializeParameterMap();

			SelectedIndex = 0;
			RefreshParameters();

			Global.gEngineerVM = this;

            ParaValueRead = new DelegateCommand(p => PreReadParaValue());

			SingleValueWrite = new DelegateCommand(p => {
				if (p is TconParameter param)
					Global.gTconCom.SingleWriteMsg((byte)SelectedChannel, param);
			});
		}

		private void InitializeParameterMap()
		{
			// --- [PROCESS] (D0001 ~ D0040 -> 0 ~ 39) ---
			AllParameters.Add(new TconParameter { Address = 1, Symbol = "NPV", Name = "현재 PV", Scale = 0.1, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 2, Symbol = "NSP", Name = "현재 SP", Scale = 0.1, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 5, Symbol = "SPSL", Name = "운전중인 SP 번호", Scale = 1.0, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 6, Symbol = "MVOUT", Name = "제어 출력치", Scale = 0.1, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 7, Symbol = "HOUT", Name = "Heating 출력", Scale = 0.1, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 8, Symbol = "COUT", Name = "Cooling 출력", Scale = 0.1, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 9, Symbol = "PIDNO", Name = "현재 적용중인 P.I.D 번호", Scale = 1.0, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 10, Symbol = "NOWSTS", Name = "현재 운전 상태", Scale = 1.0, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 14, Symbol = "ALSTS", Name = "현재 Alarm 발생 상태", Scale = 1.0, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 19, Symbol = "ERROR", Name = "현재 Error 상태", Scale = 1.0, CanWrite = false, Group = "PROCESS" });
			AllParameters.Add(new TconParameter { Address = 20, Symbol = "PROC_TIME", Name = "운전해야 할 시간", Scale = 1.0, CanWrite = false, Group = "PROCESS" });

			// --- [FUNCTION] (D0101 ~ D0142 -> 100 ~ 141) ---
			AllParameters.Add(new TconParameter { Address = 101, Symbol = "R-S,RUN/STOP", Name = "운전 상태를 ‘Run[0]’ 또는 ‘Stop[1]’으로 설정", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 105, Symbol = "A/M", Name = "제어 상태를 자동 또는 수동으로 설정", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 106, Symbol = "H.OUT(MVOUT)", Name = "수동 동작에서의 Heating 출력", Scale = 0.1, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 107, Symbol = "C.OUT(MVOUTC)", Name = "수동 동작에서의 Cooling 출력", Scale = 0.1, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 121, Symbol = "AT", Name = "Auto Tuning 실행을 설정", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 122, Symbol = "AT-G", Name = "PID값을 비례적으로 적용", Scale = 0.1, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 131, Symbol = "S-TM", Name = "예약 운전 사용시 운전까지의 대기시간을 설정", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 132, Symbol = "P-TM", Name = "운전시 운전할 시간을 설정", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 134, Symbol = "ON/OFF", Name = "On/Off 제어를 설정", Scale = 0.1, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 137, Symbol = "LOCK", Name = "Lock On시 모든 파라메터의 설정이 금지", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 138, Symbol = "DI.SL", Name = "외부접점입력에 의한 조절계의 동작종류를 설정", Scale = 1.0, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 139, Symbol = "DSP.H", Name = "PV표시창에 표시되는 센서입력값의 표시 상한치 설정", Scale = 0.1, CanWrite = true, Group = "FUNCTION" });
			AllParameters.Add(new TconParameter { Address = 140, Symbol = "DSP.L", Name = "PV표시창에 표시되는 센서입력값의 표시 하한치 설정", Scale = 0.1, CanWrite = true, Group = "FUNCTION" });

			// --- [SET POINT] (D0200 ~ D0217 -> 199 ~ 216) ---
			AllParameters.Add(new TconParameter { Address = 200, Symbol = "SPSL", Name = "운전하고자 하는 SP의 종류를 설정", Scale = 1.0, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 201, Symbol = "SP1", Name = "조절계에서 운전하고자 하는 설정치(SP1)를 설정", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 202, Symbol = "SP2", Name = "조절계에서 운전하고자 하는 설정치(SP2)를 설정", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 203, Symbol = "SP3", Name = "조절계에서 운전하고자 하는 설정치(SP3)를 설정", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 204, Symbol = "SP4", Name = "조절계에서 운전하고자 하는 설정치(SP4)를 설정", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 211, Symbol = "SPRH", Name = "SP의 상한값을 설정", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 212, Symbol = "SPRL", Name = "SP의 하한값을 설정", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 214, Symbol = "TMU", Name = "Time Unit를 설정", Scale = 1.0, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 216, Symbol = "U.SLP", Name = "시간에 비례하여 SP를 점진적으로 상승", Scale = 0.1, CanWrite = true, Group = "SET POINT" });
			AllParameters.Add(new TconParameter { Address = 217, Symbol = "D.SLP", Name = "시간에 비례하여 SP를 점진적으로 하강", Scale = 0.1, CanWrite = true, Group = "SET POINT" });

			// --- [ALARM] (D0401 ~ D0428 -> 400 ~ 427) ---
			AllParameters.Add(new TconParameter { Address = 401, Symbol = "ALT1", Name = "경보-1의 종류를 설정", Scale = 1.0, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 402, Symbol = "ALT2", Name = "경보-2의 종류를 설정", Scale = 1.0, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 403, Symbol = "ALT3", Name = "경보-3의 종류를 설정", Scale = 1.0, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 406, Symbol = "AL-1", Name = "ALT1에 의해 설정된 경보종류에 대한 경보점을 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 407, Symbol = "AL-2", Name = "ALT2에 의해 설정된 경보종류에 대한 경보점을 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 408, Symbol = "AL-3", Name = "ALT3에 의해 설정된 경보종류에 대한 경보점을 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 411, Symbol = "A1DB", Name = "경보-1의 Dead Band(Hysterisis)를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 412, Symbol = "A2DB", Name = "경보-2의 Dead Band(Hysterisis)를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 413, Symbol = "A3DB", Name = "경보-3의 Dead Band(Hysterisis)를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 416, Symbol = "A1DY", Name = "경보-1 발생시 경보 출력의 지연시간을 설정", Scale = 1.0, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 417, Symbol = "A2DY", Name = "경보-2 발생시 경보 출력의 지연시간을 설정", Scale = 1.0, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 418, Symbol = "A3DY", Name = "경보-3 발생시 경보 출력의 지연시간을 설정", Scale = 1.0, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 421, Symbol = "AL1.H", Name = "ATL1에서 편차알람 선택 시 경보점 High를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 422, Symbol = "AL2.H", Name = "ATL2에서 편차알람 선택 시 경보점 High를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 423, Symbol = "AL3.H", Name = "ATL3에서 편차알람 선택 시 경보점 High를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 426, Symbol = "AL1.L", Name = "ATL1에서 편차알람 선택 시 경보점 Low를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 427, Symbol = "AL2.L", Name = "ATL2에서 편차알람 선택 시 경보점 Low를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });
			AllParameters.Add(new TconParameter { Address = 428, Symbol = "AL3.L", Name = "ATL3에서 편차알람 선택 시 경보점 Low를 설정", Scale = 0.1, CanWrite = true, Group = "ALARM" });

			// --- [PID] (D0501 ~ D0549 -> 500 ~ 548) ---
			AllParameters.Add(new TconParameter { Address = 501, Symbol = "ARW", Name = "과적분을 방지하기 위한 편차폭을 설정", Scale = 0.1, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 502, Symbol = "FUZZY", Name = "조절계의 제어시 FUZZY 기능의 사용 유무를 설정", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 511, Symbol = "1.P", Name = "PID1 제어를 위한 비례대 동작을 설정", Scale = 0.1, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 512, Symbol = "1.I", Name = "PID1 제어를 위한 적분시간을 설정", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 513, Symbol = "1.D", Name = "PID1 제어를 위한 미분시간을 설정", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 514, Symbol = "1.MR", Name = "PID1 적분시간 항목에 수동으로 설정된 값을 적용", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 515, Symbol = "1.Pc", Name = "냉각측 PID1 제어를 위한 비례대 동작을 설정", Scale = 0.1, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 516, Symbol = "1.Ic", Name = "냉각측 PID1 제어를 위한 적분시간을 설정", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 517, Symbol = "1.Dc", Name = "냉각측 PID1 제어를 위한 미분시간을 설정", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 518, Symbol = "1.DB", Name = "PID1 가열/냉각 동작의 불감대(Dead Band)를 설정", Scale = 0.1, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 519, Symbol = "RP1", Name = "REFERENCE POINT 1 설정", Scale = 1.0, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 539, Symbol = "RHY", Name = "PID HYSTERISIS 설정", Scale = 0.1, CanWrite = true, Group = "PID" });
			AllParameters.Add(new TconParameter { Address = 549, Symbol = "RDV", Name = "REFERENCE DEVIATION 설정", Scale = 1.0, CanWrite = true, Group = "PID" });

			// --- [IN/OUT] 그룹 정밀 보정 (매뉴얼 D-Register 번호와 Address 일치화) ---
			// 입력 설정 (D0601 ~ D0610)
			AllParameters.Add(new TconParameter { Address = 601, Symbol = "IN-T", Name = "센서입력 종류 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 602, Symbol = "IN-U", Name = "온도단위(℃/℉) 선택", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 603, Symbol = "IN.RH", Name = "센서입력 범위 상한치", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 604, Symbol = "IN.RL", Name = "센서입력 범위 하한치", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 605, Symbol = "IN.DP", Name = "측정입력 소수점 위치", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 606, Symbol = "IN.SH", Name = "측정입력 Scale 상한치", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 607, Symbol = "IN.SL", Name = "측정입력 Scale 하한치", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 608, Symbol = "IN.FL", Name = "PV Filter 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 609, Symbol = "BSL", Name = "센서 단선시 PV 동작방향", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 610, Symbol = "RSL", Name = "열전대 사용시 RJC 사용여부", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });

			// PV 보정 구간 (D0611 ~ D0619)
			AllParameters.Add(new TconParameter { Address = 611, Symbol = "BSP1", Name = "PV 보정구간 1 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 612, Symbol = "BSP2", Name = "PV 보정구간 2 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 613, Symbol = "BSP3", Name = "PV 보정구간 3 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 615, Symbol = "BS0", Name = "IN.RL 적용 PV 보정값(BIAS)", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 616, Symbol = "BS1", Name = "보정구간 1 적용 PV 보정값", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 617, Symbol = "BS2", Name = "보정구간 2 적용 PV 보정값", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 618, Symbol = "BS3", Name = "보정구간 3 적용 PV 보정값", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 619, Symbol = "BS4", Name = "IN.RH 적용 PV 보정값(BIAS)", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });

			// 출력 설정 (D0621 ~ D0634)
			AllParameters.Add(new TconParameter { Address = 621, Symbol = "OUT1", Name = "OUT1(RELAY) 동작 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 622, Symbol = "OUT2", Name = "OUT2(4-20mA/PULSE) 동작 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 623, Symbol = "OUT3", Name = "OUT3(4-20mA/PULSE) 동작 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 625, Symbol = "SUB1", Name = "SUB1(RELAY) 동작 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 626, Symbol = "SUB2", Name = "SUB2(RELAY) 동작 설정", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 631, Symbol = "HEAT2", Name = "OUT2 Heating 출력 종류", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 632, Symbol = "COOL2", Name = "OUT2 Cooling 출력 종류", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 633, Symbol = "HEAT3", Name = "OUT3 Heating 출력 종류", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 634, Symbol = "COOL3", Name = "OUT3 Cooling 출력 종류", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });

			// 출력 상세 및 전송 출력 (D0636 ~ D0653)
			AllParameters.Add(new TconParameter { Address = 636, Symbol = "O.ACT", Name = "제어출력 역동작/정동작(SLxx)", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 637, Symbol = "O.ACT", Name = "제어출력 역동작/정동작(STxx)", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 638, Symbol = "CT", Name = "출력 ON/OFF 1주기 시간", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 639, Symbol = "CTc", Name = "냉각측 출력 ON/OFF 1주기 시간", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 641, Symbol = "OH", Name = "제어출력 상한값 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 642, Symbol = "OL", Name = "제어출력 하한값 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 644, Symbol = "HYS", Name = "ON/OFF 제어 Hysteresis(STxx)", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 645, Symbol = "HYS", Name = "ON/OFF 제어 Hysteresis(SLxx)", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 646, Symbol = "PO", Name = "비상시 Preset Output 값", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 647, Symbol = "POc", Name = "비상시 냉각측 Preset Output 값", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 648, Symbol = "HYS.H", Name = "Hysteresis High 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 649, Symbol = "HYS.L", Name = "Hysteresis Low 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 651, Symbol = "RET", Name = "전송출력 종류 선택", Scale = 1.0, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 652, Symbol = "RETH", Name = "전송출력 상한값 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
			AllParameters.Add(new TconParameter { Address = 653, Symbol = "RETL", Name = "전송출력 하한값 설정", Scale = 0.1, CanWrite = true, Group = "IN/OUT" });
		}

		// 현재 선택된 채널과 그룹에 맞는 파라미터 리스트를 갱신하는 함수
		private void RefreshParameters()
        {
			if (AllParameters == null || AllParameters.Count == 0) return;
			// 1. 현재 탭 인덱스로 그룹 이름을 가져옴
			string currentGroup = GetGroupNameByIndex(SelectedIndex);
			// 2. 전체 맵(AllParameters)에서 해당 그룹에 속한 파라미터들만 추출
			var filtered = AllParameters.Where(p => p.Group == currentGroup).ToList();
			// 3. UI에 바인딩된 목록에 업데이트
			CurrentParams.Clear();
			foreach (var p in filtered)
			{
				p.Value = string.Empty;
				p.EditValue = string.Empty;
				CurrentParams.Add(p);
			}
		}
		private string GetGroupNameByIndex(int index)
		{
			switch (index)
			{
				case 0: return "PROCESS";   // D0000~D0099 범위 
				case 1: return "FUNCTION";  // D0100~D0199 범위 
				case 2: return "SET POINT"; // D0200~D0299 범위 
				case 3: return "ALARM";     // D0400~D0499 범위 
				case 4: return "PID";       // D0500~D0599 범위 
				case 5: return "IN/OUT";    // D0600~D0699 범위 
				default: return "PROCESS";
			}
		}

        public void PreReadParaValue()
        {
            if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.STOP)
                return;

            _ReadParaWorker = new BackgroundWorker();
            _ReadParaWorker.WorkerReportsProgress = true;
            _ReadParaWorker.WorkerSupportsCancellation = true;
            _ReadParaWorker.DoWork += ReadParaValue;
            _ReadParaWorker.ProgressChanged += _ReadWorker_Progresschanged;
            _ReadParaWorker.RunWorkerAsync();
        }
        private void _ReadWorker_Progresschanged(object sender, ProgressChangedEventArgs e)
        {
            uiPbint = e.ProgressPercentage;
        }
        public int writereadindex = 0;
        public void ReadParaValue(object sender, DoWorkEventArgs e)
        {
            if(Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.STOP)
            {
				addParaLog("장비 ALL STOP 상태에서만 가능");
				return;
            }
            try
            {
                _ReadParaWorker.ReportProgress(0);
                Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.PARAMETER;

                writereadindex = SelectedIndex;

				switch (SelectedIndex)
				{
					case 0: LastReadStartAddress = 1; break;   // PROCESS (D0001~)
					case 1: LastReadStartAddress = 101; break; // FUNCTION (D0101~)
					case 2: LastReadStartAddress = 200; break; // SET POINT (D0200~)
					case 3: LastReadStartAddress = 400; break; // ALARM (D0401~)
					case 4: LastReadStartAddress = 500; break; // PID (D0501~)
					case 5: LastReadStartAddress = 600; break; // IN/OUT (D0601~)
				}

				addParaLog($"[CH {SelectedChannel}] {GetGroupNameByIndex(SelectedIndex)} 읽기 시작 (Addr: {LastReadStartAddress})...");

				Global.gTconCom.ParaReadMsg((byte)SelectedChannel, SelectedIndex);

				Thread.Sleep(500);

				_ReadParaWorker.ReportProgress(100);
			}
			catch (Exception ex)
			{
				addParaLog($"Read Error: {ex.Message}");
			}
			finally
			{
				// 작업 완료 후 상태 복구
				Global.gMainVM.AllRackSTAT = TOTALRACK_STAT.STOP;
			}
		}

        public void addParaLog(string msg)
        {
			if (Global.gMainVM.ShowEngineer == Visibility.Visible)
			{
				ParaLog plog = new ParaLog(msg);
				Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
				new Action(() =>
				{
					ParaLogs.Add(plog);
					if (ParaLogs.Count > 5000)
						ParaLogs.RemoveAt(0);

					if (ListParaLog != null)
					{
						ListParaLog.ScrollIntoView(plog);
					}
				}));
			}
		}
        public void addMotorLog(string msg)
        {
            if (Global.gMainVM.ShowEngineer == Visibility.Visible)
            {
                MotorLog Mlog = new MotorLog(msg);
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    MotorLogs.Add(Mlog);
                    if (MotorLogs.Count > 10000)
                        MotorLogs.RemoveAt(0);
                    ListMotorLog.ScrollIntoView(Mlog);
                }));
            }
        }
    }
}
