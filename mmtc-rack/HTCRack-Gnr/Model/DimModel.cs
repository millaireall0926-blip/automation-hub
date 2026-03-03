using HTCRack.Base;
using HTCRack.ViewModel;
using PropertyChanged;
using System;
using System.Windows.Media;

namespace HTCRack.Model
{
    public enum DIMM_STAT
    {
        STOP = 0,
        RUN = 1,
        NO_UNIT,
        WARN,
        ALRAM,
        ERROR
    }

    public class Maker
    {
        public int x;
        public int y;
        public string text;

        public Maker() { }

    }

    public class DimModel : BaseVM
    {
        public SolidColorBrush DimColor { get; set; }
        [DoNotNotify]
        public double _dimTemp = 0;
        public double DimTemp { get => _dimTemp; 
            set {
                              
                _dimTemp = value;
                DiffPrev = _dimTemp - PrevTemp;
                PrevTemp = _dimTemp;

                if (PrevTemp == 0) DiffPrev = 0;

                if (_dimTemp <= 0) DimText = "";                    
                else DimText = DimTemp.ToString();

                if (_dimTemp == 0) Stat = DIMM_STAT.NO_UNIT;
                else
                {
                    if (Stat == DIMM_STAT.NO_UNIT)
                        Stat = DIMM_STAT.RUN;
                }
            }
        }

        [DoNotNotify]
        public double PrevTemp { get; set; }
        public double DiffPrev { get; set; }



        public double DimTempLow { get; set; } = 0;
        public double DimTempHigh { get; set; } = 0;

        public void doDimStat()
        {
            if (Stat == DIMM_STAT.NO_UNIT) return;

            if (_dimTemp < Global.gConfigVM.MinDimm || _dimTemp > Global.gConfigVM.MaxDimm)
                Stat = DIMM_STAT.WARN;
            else if (_dimTemp < Global.gConfigVM.AlarmLowLimit
                || _dimTemp > Global.gConfigVM.AlarmHighLimit)
                Stat = DIMM_STAT.ALRAM;
            else if (_dimTemp < Global.gConfigVM.MinDimm - Global.gConfigVM.DimmLimit
                || _dimTemp > Global.gConfigVM.MaxDimm + Global.gConfigVM.DimmLimit)
                Stat = DIMM_STAT.ERROR;
            else
                Stat = DIMM_STAT.RUN;
        }

        public string DimText { get; set; } = ""; 


        DIMM_STAT _stat = DIMM_STAT.STOP;
        public DIMM_STAT Stat { get => _stat;
            set {
                _stat = value;

                DimColor = Global.gDimStatColors[(int)_stat];
                if (_stat == DIMM_STAT.STOP)
                    DimText = "";                                    
            } 
        }

        

        public DimModel() { DimColor = Global.gDimStatColors[(int)DIMM_STAT.STOP]; }
    }
}
