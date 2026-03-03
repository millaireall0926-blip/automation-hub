using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.ViewModel;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;

namespace RDimm_MMTCRACK.Model
{
    public class ManMotorModel : BaseVM
    {
        public int MotorIndex { get; set; } = 0;
        public int ManualDownVal { get; set; } = 1000;
        public int TotalMotorVal { get; set; } = 0;
        public SolidColorBrush eMotorStatColor { get; set; } = Global.gMotorStatColors[1];
        public string eMotorStatText { get; set; } = string.Empty;
        public string GetSiteVer { get; set; } = string.Empty;

        ObservableCollection<TconModel> _tcons = new ObservableCollection<TconModel>();
        public ObservableCollection<TconModel> Tcons
        {
            get { return _tcons; }
            set { _tcons = value; PC(nameof(Tcons)); }
        }

        public RackModel MasterRack { get; set; }
        public DelegateCommand CmdManualUp { get; set; }
        public DelegateCommand CmdManualDown { get; set; }
        public DelegateCommand CmdAutoUp { get; set; }
        public DelegateCommand CmdAutoDown { get; set; }
        public DelegateCommand RackCmdStatusClear { get; set; }

        public DelegateCommand CmdSiteVer { get; set; }

        public DelegateCommand RackTconRun { get; set; }
        public DelegateCommand RackTConStop { get; set; }

        public ManMotorModel(int ch , RackModel rm) 
        {
            MotorIndex = ch;
            MasterRack = rm;

            for (int i = 0; i < 3; i++)
                Tcons.Add(new TconModel(i , MasterRack));

            CmdManualUp = new DelegateCommand(p => { Task.Run(() => MasterRack.PreManualMotorOpen(ManualDownVal.ToString())); });
            CmdManualDown = new DelegateCommand(p => { Task.Run(() => MasterRack.PreManualMotorDown(ManualDownVal.ToString())); });
            
            CmdAutoUp = new DelegateCommand(p => { Task.Run(() => MasterRack.PreMotorAutoOpen(true)); });
            CmdAutoDown = new DelegateCommand(p => { Task.Run(() => MasterRack.PreMotorAutoDown()); });

            CmdSiteVer = new DelegateCommand(p => { Task.Run(() => MasterRack.SiteBDVersionCheck()); });

            RackCmdStatusClear = new DelegateCommand(p => { Task.Run(() => MasterRack.RackStatusClear()); });
        
            RackTconRun = new DelegateCommand(p => { Task.Run(() => MasterRack.RackTconRun()); });
            RackTConStop = new DelegateCommand(p => { Task.Run(() => MasterRack.RackTconStop()); });
        }
    }
}
