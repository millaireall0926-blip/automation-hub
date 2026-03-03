using RDimm_MMTCRACK.Base;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace RDimm_MMTCRACK.Model
{
    public enum DIMM_STAT
    {
        STOP = 0,
        RUN = 1,
        NO_UNIT,
        ALRAM,
        ERROR
    }
    public class DimModel : BaseVM
    {

        public SolidColorBrush DimColor { get; set; }
        public SolidColorBrush DimTextColor { get; set; }
        [DoNotNotify]
        public double _dimTemp = 0;
        public double DimTemp
        {
            get => _dimTemp;
            set
            {

                _dimTemp = value;
                DiffPrev = _dimTemp - PrevTemp;
                PrevTemp = _dimTemp;

                if (PrevTemp == 0) DiffPrev = 0;

                //if (_dimTemp <= 0) //org 10.24
                if (_dimTemp <= -20 || _dimTemp == 0) DimText = "";
                else
                {
                    DimText = DimTemp.ToString();
                    DimTextColor = Global.gWarningColors[0];
                    /*
                    if(DimTextColor == Global.gWarningColors[0])
                        DimTextColor = Global.gWarningColors[1];
                    else
                        DimTextColor = Global.gWarningColors[0];
                    */
                }
                if (_dimTemp == 0) Stat = DIMM_STAT.NO_UNIT;

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

            if (_dimTemp <= Global.gConfigVM.LowTempAlarm - Global.gConfigVM.TempErr
                || _dimTemp >= Global.gConfigVM.HighTempAlarm + Global.gConfigVM.TempErr)
                Stat = DIMM_STAT.ERROR;
            //요청사항을 DIMM_STAT.ALRAM 제거
            //else if (_dimTemp < Global.gConfigVM.LowTempAlarm
            //    || _dimTemp > Global.gConfigVM.HighTempAlarm)
            //    Stat = DIMM_STAT.ALRAM;
            else
                Stat = DIMM_STAT.RUN;
        }

        public string DimText { get; set; } = "";


        DIMM_STAT _stat = DIMM_STAT.STOP;
        public DIMM_STAT Stat
        {
            get => _stat;
            set
            {
                _stat = value;

                //DimColor = Global.gDimStatColors[(int)_stat];
                if (_stat == DIMM_STAT.STOP)
                    DimText = "";
            }
        }

        public DimModel() { DimColor = Global.gDimStatColors[(int)DIMM_STAT.STOP]; }
    }
}
