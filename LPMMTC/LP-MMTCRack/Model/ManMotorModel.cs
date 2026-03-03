using LP_MMTCRack.Base;
using LP_MMTCRack.ViewModel;
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

namespace LP_MMTCRack.Model
{
    public class ManMotorModel : BaseVM
    {
        public int MotorIndex { get; set; } = 0;
        public int ManualDownVal { get; set; } = 1000;
        public int TotalMotorVal { get; set; } = 0;

        public string GetSiteVer {  get; set; } = string.Empty;
        public string StatusClear1 { get; set; } = string.Empty;
        public string StatusClear2 { get; set; } = string.Empty;
        public string StatusClear3 { get; set; } = string.Empty;

        public SolidColorBrush eMotorStatColor { get; set; } = Global.gMotorStatColors[1];
        public string eMotorStatText { get; set; } = string.Empty;

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
        public DelegateCommand CmdSiteVer { get; set; }
        public DelegateCommand CmdSiteReset { get; set; }

        public DelegateCommand RackCmdStatusClearAll { get; set; }
        public DelegateCommand RackCmdStatusClear1 { get; set; }
        public DelegateCommand RackCmdStatusClear2 { get; set; }
        public DelegateCommand RackCmdStatusClear3 { get; set; }

        public Visibility ClearBtn1 { get; set; } = Visibility.Hidden;
        public Visibility ClearBtn2 { get; set; } = Visibility.Hidden;
        public Visibility ClearBtn3 { get; set; } = Visibility.Hidden;

        public DelegateCommand RackTconRun { get; set; }
        public DelegateCommand RackTConStop { get; set; }

        public ManMotorModel(int ch, RackModel rm)
        {
            MotorIndex = ch;
            MasterRack = rm;

            if (Global.gMainPageVM.AllBoxCnt == 3)
            {
                StatusClear1 = $"B{rm.RackIndex * 3 + 1} Clear";
                StatusClear2 = $"B{rm.RackIndex * 3 + 2} Clear";
                StatusClear3 = $"B{rm.RackIndex * 3 + 3} Clear";

                ClearBtn1 = Visibility.Visible;
                ClearBtn2 = Visibility.Visible;
                ClearBtn3 = Visibility.Visible;

                for (int i = 0; i < 3; i++)
                    Tcons.Add(new TconModel(i, MasterRack));
            }
            else if(Global.gMainPageVM.AllBoxCnt == 2)
            {
                StatusClear1 = $"B{rm.RackIndex * 2 + 1} Clear";
                StatusClear2 = $"B{rm.RackIndex * 2 + 2} Clear";

                ClearBtn1 = Visibility.Visible;
                ClearBtn2 = Visibility.Visible;

                for (int i = 0; i < 2; i++)
                    Tcons.Add(new TconModel(i, MasterRack));
            }
            

            CmdManualUp = new DelegateCommand(p => { Task.Run(() => MasterRack.PreManualMotorOpen(ManualDownVal.ToString())); });
            CmdManualDown = new DelegateCommand(p => { Task.Run(() => MasterRack.PreManualMotorDown(ManualDownVal.ToString())); });

            CmdAutoUp = new DelegateCommand(p => { Task.Run(() => MasterRack.PreMotorAutoOpen(MOTOR_TYPE.NOMAL)); });
            CmdAutoDown = new DelegateCommand(p => { Task.Run(() => MasterRack.PreMotorAutoDown()); });
            CmdSiteVer = new DelegateCommand(p => { Task.Run(() => MasterRack.SiteBDVersionCheck()); });
            CmdSiteReset = new DelegateCommand(p => { Task.Run(() => MasterRack.CheckSiteConnet("Site")); });
            //CmdSiteVer = new DelegateCommand(p => { Task.Run(() => Global.gMainPageVM.sc[MasterRack.RackIndex].CheckSiteConnect()); });

            try
            {
                RackCmdStatusClearAll = new DelegateCommand(p => { Task.Run(() => MasterRack.RackStatusClear()); });
                RackCmdStatusClear1 = new DelegateCommand(p => { Task.Run(() => MasterRack.Boxes[0].BoxStatusClear()); });
                RackCmdStatusClear2 = new DelegateCommand(p => { Task.Run(() => MasterRack.Boxes[1].BoxStatusClear()); });
                RackCmdStatusClear3 = new DelegateCommand(p => { Task.Run(() => MasterRack.Boxes[2].BoxStatusClear()); });
            }
            catch (Exception ex) { }

            RackTconRun = new DelegateCommand(p => { Task.Run(() => MasterRack.RackTconRun()); });
            RackTConStop = new DelegateCommand(p => { Task.Run(() => MasterRack.RackTconStop()); });
        }
    }
}
