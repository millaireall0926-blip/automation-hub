using Debug_TempCont.Common;
using Debug_TempCont.Model;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.IO.Packaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Debug_TempCont.ViewModel
{
    public enum STAT_TYPE : int
    {
        READY,
        RUN,
        DEFROSTING,
        STOP,
        PATTERN_RUN,
        PATTERN_STOP
    }
    public class TempChVM : BaseVM
    {
        ObservableCollection<EachTempCH> _tempDatas = new ObservableCollection<EachTempCH>();
        public ObservableCollection<EachTempCH> TempDatas
        {
            get { return _tempDatas; }
            set
            {
                _tempDatas = value;
                OnPropertyChanged(nameof(TempDatas));
            }
        }
        public TempChVM()
        {
            TempDatas = new ObservableCollection<EachTempCH>
            {
                new EachTempCH("1", "-", "-", "READY", "LightGreen", 0,0 ),
                new EachTempCH("2", "-", "-", "READY", "LightGreen", 0,1 ),
                new EachTempCH("3", "-", "-", "READY", "LightGreen", 1,0 ),
                new EachTempCH("4", "-", "-", "READY", "LightGreen", 1,1 )
            };
            Global.gTempChVM = this;
        }
        public void UpdateTemp(double[] getTempValue)
        {
            for (int i = 0; i < 4; i++)
            {
                if (getTempValue[(i * 2)] < 1.0 && getTempValue[(i * 2)] > -1.0)           // Now Temp Display
                    TempDatas[i].TxtNowValue = getTempValue[(i * 2)].ToString("0.0");
                else
                    TempDatas[i].TxtNowValue = getTempValue[(i * 2)].ToString(".0");

                if (getTempValue[(i * 2) + 1] < 1.0 && getTempValue[(i * 2) + 1] > -1.0)           //Set Temp Display
                    TempDatas[i].TxtSetValue = getTempValue[(i * 2) + 1].ToString("0.0");
                else
                    TempDatas[i].TxtSetValue = getTempValue[(i * 2) + 1].ToString(".0");
            }
        }
    }
    public class EachTempCH : BaseVM
    {
        #region UI Property
        private string _TxtNumerCh = "1"; //Channel
        public string TxtNumerCh
        {
            get { return _TxtNumerCh; }
            set { _TxtNumerCh = value; OnPropertyChanged(nameof(TxtNumerCh)); }
        }
        private string _TxtSetValue = "-"; //SET TEMP VALUE
        public string TxtSetValue
        {
            get { return _TxtSetValue; }
            set { _TxtSetValue = value; OnPropertyChanged(nameof(TxtSetValue)); }
        }
        private string _TxtNowValue = "-"; //NOW TEMP VALUE
        public string TxtNowValue
        {
            get { return _TxtNowValue; }
            set { _TxtNowValue = value; OnPropertyChanged(nameof(TxtNowValue)); }
        }
        private string _TxtStatusValue = "READY"; //STATUS VALUE
        public string TxtStatusValue
        {
            get { return _TxtStatusValue; }
            set { _TxtStatusValue = value; OnPropertyChanged(nameof(TxtStatusValue)); }
        }
        private string _EllipseColorValue = "GRAY"; //ELLIPSE COLOR VALUE
        public string EllipseColorValue
        {
            get { return _EllipseColorValue; }
            set { _EllipseColorValue = value; OnPropertyChanged(nameof(EllipseColorValue)); }
        }
        public int RowCh { get; set; }
        public int ColCh { get; set; }
        public DelegateCommand CheckCommand { get; set; }
        public DelegateCommand ButtonCommand { get; set; }
        #endregion UI Property
        public EachTempCH(string numberCh, string setValue, string nowValue, string statusValue, string colorValue, int rowch, int colch)
        {
            TxtNumerCh = numberCh;
            TxtSetValue = setValue;
            TxtNowValue = nowValue;
            TxtStatusValue = statusValue;
            EllipseColorValue = colorValue;
            RowCh = rowch;
            ColCh = colch;
            CheckCommand = new DelegateCommand(p => { StatusCheck(); });
            ButtonCommand = new DelegateCommand(p => { ButtonCheck(); });
        }
        private void StatusCheck()
        {
            if (_EllipseColorValue == "GRAY")
                EllipseColorValue = "LightGreen";
            else if (_EllipseColorValue == "LightGreen")
                EllipseColorValue = "GRAY";
        }
        private void ButtonCheck()
        {
            if (Global.gSerialComm.sPort.IsOpen)
            {
                if (PrevStat == STAT_TYPE.RUN)
                {
                    Global.setTempFlag = true;
                    StatType = STAT_TYPE.READY;
                    Global.sendWaitFlag = false;
                    Global.setTempFlag = false;
                }
                else if (PrevStat == STAT_TYPE.READY)
                {
                    Global.setTempFlag = true;
                    StatType = STAT_TYPE.RUN;
                    Global.sendWaitFlag = false;
                    Global.setTempFlag = false;
                }
            }
        }
        public void SettingTemp(string getTemp)
        {
            if (_EllipseColorValue == "LightGreen")
            {
                while (true)
                {
                    //if (Global.gProtocolName == "Modbus-RTU")
                    {
                        if (Global.sendWaitFlag)
                        {
                            string SetHex = Global.gCalculator.tranfer_StringtoDoubletoHex(getTemp); //Set Temp Hex

                            ArrayList alReturn = new ArrayList();

                            alReturn.Add((byte)Convert.ToByte(Global.gM7Number));
                            alReturn.Add((byte)0x06);

                            alReturn.Add((byte)0x00); //Register Address Hi
                            byte LoAddr = Global.gCalculator.get_SVaddress(TxtNumerCh);
                            alReturn.Add((byte)LoAddr); //Register Address Lo

                            string a = SetHex.Substring(0, 2);
                            string b = SetHex.Substring(2, 2);
                            string c = " ";
                            a = a + c + b;

                            foreach (string s in a.Split(' '))
                            {
                                if (s != null && s != "")
                                    alReturn.Add(Convert.ToByte(s, 16));
                            }

                            Global.gSerialComm.sendMessage(alReturn);

                            Global.gSerialComm.waitRecv();

                            alReturn.Clear();

                            break;
                        }
                    }
                }
            }
        }
        public STAT_TYPE PrevStat = STAT_TYPE.READY;
        STAT_TYPE _statType;
        public STAT_TYPE StatType
        {
            get => _statType;
            set
            {
                _statType = value;
                if (_EllipseColorValue == "LightGreen")
                {
                    if ((_statType == STAT_TYPE.RUN))// && (PrevStat != _statType))
                        RunStop_Send(true);
                    else if ((_statType == STAT_TYPE.READY))// && (PrevStat != _statType))
                        RunStop_Send(false);

                    PrevStat = _statType;
                }
            }
        }
        public void RunStop_Send(bool runstop)
        {
            while (true)
            {
                if (Global.gProtocolName == "Modbus-RTU")
                {
                    if (Global.sendWaitFlag)
                    {
                        ArrayList alReturn = new ArrayList();

                        alReturn.Add((byte)Convert.ToByte(Global.gM7Number));
                        alReturn.Add((byte)0x06);

                        alReturn.Add((byte)0x00); //Register Address Hi
                        byte LoAddr = Global.gCalculator.get_RUNSTOPaddress(TxtNumerCh);
                        alReturn.Add((byte)LoAddr); //Register Address Lo

                        alReturn.Add((byte)0x00);
                        if (runstop)
                            alReturn.Add((byte)0x01);
                        else
                            alReturn.Add((byte)0x00);

                        Global.gSerialComm.sendMessage(alReturn);

                        Global.gSerialComm.waitRecv();

                        alReturn.Clear();

                        break;
                    }
                }
            }
        }
    }
}
