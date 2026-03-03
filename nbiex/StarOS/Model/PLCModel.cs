using StarBase;

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
using StarBase.Plc;

namespace StarOS.Model
{
    public class PLCModel : BaseVM
    {
        public Modbus _modbus;

        public string _name      = "";
        public string Name
        {
            get => _name;
            set
            {
                _name = value;OnPropertyChanged("Name");
            }

        }
        public double _curTemp  = 0;
        public double CurTemp
        {
            get => _curTemp;
            set
            {
                _curTemp = value; OnPropertyChanged("CurTemp");
            }
        }

        public double _targetTemp = 0;
        public double TargetTemp
        {
            get => _targetTemp;
            set
            {
                _targetTemp = value; OnPropertyChanged("TargetTemp");
            }
        }

        public int _statValue   = 0;

        PLC_STAT_TYPE _stat = 0;
        public PLC_STAT_TYPE Stat
        {
            get => _stat;
            set
            {
                _stat = value; OnPropertyChanged("Stat");
            }
        }


        int _remote = 0;
        public int Remote
        {
            get => _remote;
            set
            {
                _remote = value; OnPropertyChanged("Remote");
            }
        }

        int _rack = 0;
        public int Rack
        {
            get => _rack;
            set
            {
                _rack = value; OnPropertyChanged("Rack");
            }
        }

        int _door = 0;
        public int Door
        {
            get => _door;
            set
            {
                _door = value; OnPropertyChanged("Door");
            }
        }

        int _lampStat = 0;
        public int LampStat
        {
            get => _lampStat;
            set
            {
                _lampStat = value; OnPropertyChanged("LampStat");
            }
        }

        int _lampOwn = 0;
        public int LampOwn
        {
            get => _lampOwn;
            set
            {
                _lampOwn = value; OnPropertyChanged("LampOwn");
            }
        }


        int _lampCtrl = 0;
        public int LampCtrl
        {
            get => _lampCtrl;
            set
            {
                _lampCtrl = value; OnPropertyChanged("LampCtrl");
            }
        }



        public Collection<int> _prevAlarms = new Collection<int>();
        ObservableCollection<int> _alarms = new ObservableCollection<int>();
        public ObservableCollection<int> Alarms
        {
            get => _alarms;
            set
            {
                _alarms = value; OnPropertyChanged("Alarms");
            }
        }
        //public int[] _alarms = new int[7];

        public double[] _tempData = new double[1_000_000];
        public int      _tempGraphIndex = 0;

        public PLCModel()
        {
            if (Global.gChamberModel == "jst")
                _modbus = new Modbus_jst(); // 관리는 VM에서
            else
                _modbus = new Modbus_jst(); // 관리는 VM에서

            Stat = PLC_STAT_TYPE.DISCON;

            int alramCount = 0;
            if (Global.gChamberModel == "jst")
                alramCount =  PLCMap_jst.AlramCount;

            for (int i = 0; i < alramCount; i++)
            {
                _prevAlarms.Add(0);
                Alarms.Add(0);
                _modbus._plcAlram.Add(0);
            }

            LampOwn = Global.gLampOwn;
            LampCtrl = Global.gLampCtrl;            

        }
        public bool isConnect() {
            return _modbus.IsConnected;
        }

        public void doConnect() {
            _modbus.doConnect();
        }


        public bool addTempData()
        {
            if (!_modbus._connect) return false;

            bool isAdd = false;
            foreach (var slot in Global.gSlotVM.Slots)
            {
                if (slot._plcModel != this) continue;
                if (slot.StatType == STAT_TYPE.CONTACTING || slot.StatType == STAT_TYPE.RUNNING)
                    isAdd = true;
            }
            if (isAdd)
            {
                _tempData[_tempGraphIndex] = CurTemp;
                _tempGraphIndex++;
            }
            else
                _tempGraphIndex = 0;
            
            bool isCur = (Global.gChamberVM.CurPLC == this);
            bool isUpdate = (isCur & isAdd);
            return isUpdate;
        }

        public void doPLCWork() {
            _modbus.readPLCFunc();
            CurTemp = _modbus.CurTemp;
            TargetTemp = _modbus.TargetTemp;
            Stat = _modbus.Stat;
            Remote = _modbus.Remote;
            Rack = _modbus.Rack;
            Door = _modbus.Door;
            LampStat = _modbus.LampStat;    
            LampOwn = _modbus.LampOwn;
            LampCtrl = _modbus.LampCtrl;

            int alramCount = 0;
            if (Global.gChamberModel == "jst")
                alramCount = PLCMap_jst.AlramCount;            

            CHAM_ALARAM[,] cHAM_ALARAMs = PLCMap_jst.ALRAM_MSGS;

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() => {
                    for (int i = 0; i < alramCount; i++) {
                        Alarms[i] = _modbus._plcAlram[i]; // ALARM read       
                    }
                }));

            bool isCritialAlram = false;
            for (int i = 0; i < alramCount; i++)
            {
                if (_prevAlarms[i] != Alarms[i])
                {
                    for (int shift = 0; shift < 16; shift++)
                    {
                        int bit = 1 << shift;
                        if ((_prevAlarms[i] & bit) != (Alarms[i] & bit))
                        {
                            CHAM_ALARAM chamAlarm = cHAM_ALARAMs[i, shift];
                            if (chamAlarm.Type == ALARM_TYPE.ALARM_IGNORE) continue;
                            LOG_TYPE type = (LOG_TYPE)chamAlarm.Type;
                            Global.gTestVM.addSysLog(type, Name, chamAlarm.Text);
                            if (chamAlarm.isForceStop)
                                isCritialAlram = true;
                        }
                    }
                    _prevAlarms[i] = Alarms[i];
                }
            }
            if (isCritialAlram)
            {
                Global.gMainVM.stopedByChamber();
            }


            if (Stat == PLC_STAT_TYPE.AMBI) {
                if (CurTemp >= 24.0 && CurTemp <= 26.0)
                    _modbus.setOperationFunc(PLC_STAT_TYPE.STOP);
            }

        }



    }
}
