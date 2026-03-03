using HTCRack_CXL.Base;
using HTCRack_CXL.ViewModel;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Xml.Linq;

namespace HTCRack_CXL.Model
{
    public enum SITE_STAT
    {
        STOP = 0,
        PREV_STOP,
        PREV_RUN,
        RUN,
        DONE,
        TUNNING,
        ERROR,
        DISCONNECT,
        INSPECTION,
        DEFROST,
    }
    public class SiteModel : BaseVM
    {
        [DoNotNotify] public int SiteIndex { get; set; } = 0;
        public SolidColorBrush BoarderColor { get; set; }
        public SolidColorBrush SiteColor { get; set; } = Global.gRackColors[0];

        [DoNotNotify]
        public int SSRValue { get; set; } = 0;
        [DoNotNotify]
        public List<int> SVCtrlList { get; set; } = new List<int>();
        [DoNotNotify]
        public List<int> PVCtrlList { get; set; } = new List<int>();
        [DoNotNotify]
        public List<int> FanCtrlList { get; set; } = new List<int>();
        [DoNotNotify]
        public int FPIndex { get; set; } = 0;

        [DoNotNotify]
        public BoxModel MasterBox { get; set; }

        SITE_STAT _siteStat;
        public SITE_STAT SiteStat
        {
            get => _siteStat;
            set
            {
                _siteStat = value;
                BoarderColor = Global.gBoarderColors[(int)_siteStat];
            }
        }

        //todo PV SV 여기에 입력하게
        private double? _tcongetpv = 0;
        public double? TConGetPV
        {
            get => _tcongetpv;
            set => _tcongetpv = value;
        }

        private double _tcongetsv = 0;
        public double TConGetSV
        {
            get => _tcongetsv;
            set => _tcongetsv = value;
        }
        private bool? _tconisrun = null;
        public bool? TConIsRun
        {
            get => _tconisrun;
            set
            {
                _tconisrun= value;
                if (_tconisrun == null) return;
                if((bool)_tconisrun) // true TCon Run
                {
                    if(SiteStat != SITE_STAT.RUN && SiteStat != SITE_STAT.DEFROST)
                        SiteStat = SITE_STAT.RUN;
                }
                else
                {
                    SiteStat = SITE_STAT.STOP;
                }
            }
        }

        public SiteModel(int index, BoxModel masterBox)
        {
            SiteIndex = index;
            MasterBox = masterBox;

            SiteStat = SITE_STAT.DISCONNECT;
        }
        
    }
}
