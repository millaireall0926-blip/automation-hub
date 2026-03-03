using LP_MMTCRack.Base;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LP_MMTCRack.Model
{
    public class TconModel :BaseVM
    {
        [DoNotNotify] public int TconIndex { get; set; } = 0;
        [DoNotNotify] public string TconName { get; set; } = string.Empty;

        public RackModel MasterRack { get; set; }
        public string EngrRackTconstat { get; set; } = string.Empty;
        public string EngrRackSV { get; set; } = string.Empty;
        public string EngrRackPV { get; set; } = string.Empty;

        public TconModel(int ch , RackModel rm)
        {
            TconIndex = ch;
            MasterRack = rm;
            TconName = $"CH{MasterRack.RackIndex * Global.gMainPageVM.AllBoxCnt +(ch+1)}"; 
        }
    }
}
