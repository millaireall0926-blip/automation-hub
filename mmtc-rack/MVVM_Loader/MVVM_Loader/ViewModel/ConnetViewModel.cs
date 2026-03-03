using MVVM_Loader.Common;
using MVVM_Loader.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MVVM_Loader.ViewModel
{
    public class ConnetViewModel : BaseVM
    {
        private ConnetModel ctm = null;
        public DelegateCommand ConnetCommand { get; set; } = new DelegateCommand(p => { Global.gSerialComm.Connect_Run(); });
        public DelegateCommand DisconnetCommand { get; set; } = new DelegateCommand(p => { Global.gSerialComm.Connect_Stop(); });

        public ConnetViewModel()
        {
            //Global.gConnetViewModel = this;
            ctm = new ConnetModel();
        }
        public ConnetModel Ctm
        {
            get { return ctm;}
            set { ctm = value; OnPropertyChanged(nameof(Ctm)); }
            
        }
    }

}
