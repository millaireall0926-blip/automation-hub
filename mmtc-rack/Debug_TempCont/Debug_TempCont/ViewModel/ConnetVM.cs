using Debug_TempCont.Common;
using Debug_TempCont.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Debug_TempCont.ViewModel
{
    internal class ConnetVM : BaseVM
    {
        private ConnetModel ctm = null;
        public DelegateCommand ConnetCommand { get; set; } = new DelegateCommand(p => { Global.gSerialComm.Connect_Run(); });
        public DelegateCommand DisconnetCommand { get; set; } = new DelegateCommand(p => { Global.gSerialComm.Connect_Stop(); });

        public ConnetVM()
        {
            ctm = new ConnetModel();
        }
        public ConnetModel Ctm
        {
            get { return ctm; }
            set { ctm = value; OnPropertyChanged(nameof(Ctm)); }

        }
    }
}
