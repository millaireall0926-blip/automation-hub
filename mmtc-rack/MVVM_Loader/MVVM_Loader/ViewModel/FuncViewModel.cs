using MVVM_Loader.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace MVVM_Loader.ViewModel
{
    public class FuncViewModel : BaseVM
    {
        public List<string> InitList { get; set; } = new List<string>()
        {
            "OFF","CH 1","CH 2","CH 3","CH 4","CH ALL"
        };
        public List<string> PidList { get; set; } = new List<string>()
        {
            "CH ALL","CH 1","CH 2","CH 3","CH 4"
        };
        private Brush _BackColor1 = Brushes.OrangeRed;
        public Brush BackgroundColor1
        {
            get { return _BackColor1; }
            set
            {
                if (_BackColor1 != null)
                    _BackColor1 = value;

                OnPropertyChanged(nameof(BackgroundColor1));
            }
        }
        private string _statustxt = "통신 불가";
        public string statustxt
        {
            get { return _statustxt; }
            set
            {
                if (_statustxt != null)
                    _statustxt = value;

                OnPropertyChanged(nameof(statustxt));
            }
        }
        private int _uiPbint = 0;
        public int uiPbint
        {
            get { return _uiPbint; }
            set
            {
                _uiPbint = value;
                OnPropertyChanged(nameof(uiPbint));
            }
        }
        private int _Selinit = 0;
        public int Selinit
        {
            get { return _Selinit; }
            set
            {
                _Selinit = value;
                OnPropertyChanged(nameof(Selinit));
            }
        }
        private int _Selpid = 0;
        public int Selpid
        {
            get { return _Selpid; }
            set
            {
                _Selpid = value;
                OnPropertyChanged(nameof(Selpid));
            }
        }
        public DelegateCommand ResetCommand { get; set; }
        public DelegateCommand StartCommand { get; set; }
        public DelegateCommand StopCommand { get; set; }
        public DelegateCommand DownloadCommand { get; set; } = new DelegateCommand(p => { Global.gSerialComm.check_download(); });
        public FuncViewModel()
        {
            //Global.gFuncViewModel = this;
            ResetCommand = new DelegateCommand(p => { Global.gSerialComm.Cehck_Reset(_Selinit); });
            StartCommand = new DelegateCommand(p => { Global.gSerialComm.Cehck_AutoPid(_Selpid, 1); });
            StopCommand = new DelegateCommand(p => { Global.gSerialComm.Cehck_AutoPid(_Selpid, 0); });
        }
    }
}
