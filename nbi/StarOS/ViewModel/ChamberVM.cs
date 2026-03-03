using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarBase;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;
using System.Windows.Input;
using System.Windows.Controls;
using System.Threading;
using StarOS.Model;
using System.Collections.ObjectModel;
using System.Windows.Media;
using StarOS.View;
using System.Windows.Threading;
using System.Windows;
using StarBase.Plc;

namespace StarOS.ViewModel
{
    public class ChamberVM : BaseVM
    {
        

        //        public static CommandBindingCollection Commands = new CommandBindingCollection();
        //        public static RoutedCommand CmdPLC = new RoutedCommand("CmdPLC", typeof(ApplicationCommands));
        //        void canExcute_CmdPLC(object sender, CanExecuteRoutedEventArgs e)
        //        {
        //            e.CanExecute = true;
        //        }

        string _equipID = "NBI-805";
        public string EquipID
        {
            get => _equipID;
            set
            {
                _equipID = value; OnPropertyChanged("EquipID");
            }
        }

        public ChamCTRL ChamCtrlWin;

        public Collection<PLCModel> _plcs = new Collection<PLCModel>();

        public Collection<SolidColorBrush> _statColors = new Collection<SolidColorBrush>();

        public int _intervalWebSend = 60; // 60초마다 temp data전송

        #region Thread 관련
        public static void PlcThread(object parent) {
            ChamberVM vm = parent as ChamberVM;
            while (vm._plcThreadClose == false) {
                foreach (PLCModel plc in vm._plcs) {

                    if (plc.isConnect()) {
                        plc.doPLCWork();

                    }
                    else {
                        plc.doConnect();
                        Thread.Sleep(2000);
                    }

                    if (plc.addTempData()) // update flag On
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                            new Action(() => vm.updateTemp()));
                }
                Thread.Sleep(1000);

                if (vm._intervalWebSend > 60)  // web data 전송용
                {
                    string sendPV = "";                    
                    string sendST = "";
                    int idx = 0;
                    foreach (PLCModel plc in vm._plcs) {
                    
                        sendPV += string.Format("{0:0.0}", plc.CurTemp);
                        sendST += plc.Stat.ToString().Substring(0, 1);
                        if (idx != vm._plcs.Count - 1) {
                            sendPV += ",";
                            sendST += ",";
                        }
                            idx++;
                    }
                    if (sendPV.Length > 0) 
                        Global.gNet._client.sendTempData(sendPV, sendST);                    
                    vm._intervalWebSend = 0;
                }
                vm._intervalWebSend++;

            }
        }

        public bool _plcThreadClose = false;
        public static Thread _plcThread = new Thread(PlcThread);

        #endregion

        #region ToolBar 관련
        public void updateMenu(Button bt)
        {

        }
         
        public void excute_CmdPLC(Button bt)
        {
            if (bt == null) return;
            if (CurPLC == null) return; 
            if (bt.Name == "xBT_RackIn")
            {
                CurPLC._modbus.setRackFunc(1);
            }
            else if (bt.Name == "xBT_RackOut")
            {
                CurPLC._modbus.setRackFunc(0);
            }
            else if (bt.Name == "xBT_DoorOpen")
            {
                CurPLC._modbus.setDoorFunc(0);
            }
            else if (bt.Name == "xBT_DoorClose")
            {
                CurPLC._modbus.setDoorFunc(1);
            }
            else if (bt.Name == "xBT_Menual")
            {
                ChamCtrlWin.Left = Global.gMainWin.Left + 300;
                ChamCtrlWin.Top = Global.gMainWin.Top + 300;
                if (CurPLC != null)
                    SetTemp = CurPLC.TargetTemp.ToString("F1");
                ChamCtrlWin.Show();
            }
            else if (bt.Name == "xBT_Alarm")
            {
                CurPLC._modbus.setResetFunc();
            }

        }
        #endregion

        #region Temp Graph 관련

        bool _autoFit = true;
        public bool AutoFit
        {
            get => _autoFit;
            set
            {
                _autoFit = value;
                OnPropertyChanged("AutoFit");
            }
        }


        public WpfPlot xTempGraph;
        SignalPlot TempSig;
        public double[] TempData = new double[1_000_000];



        public void setTempData()
        {
            if (CurPLC == null) return;
            TempSig.MaxRenderIndex = CurPLC._tempGraphIndex;
            if (CurPLC._tempGraphIndex != 0)
            {
                Array.Copy(CurPLC._tempData, TempData, CurPLC._tempGraphIndex);
            }
            xTempGraph.Refresh();
        }

        public void updateTemp()  // power 에서 Temp Data를 UPDATE 할지..
        {
            int index = CurPLC._tempGraphIndex - 1;
            if (index < 0)index = 0;

            TempData[index] = CurPLC.CurTemp;
            TempSig.MaxRenderIndex = index;
                           

            if (AutoFit)
                xTempGraph.Plot.AxisAuto();
            xTempGraph.Refresh();
        }

        public void initItems()
        {

            xTempGraph.Plot.Title("Chamber");
            xTempGraph.Plot.YLabel("℃");

            xTempGraph.Plot.Style(ScottPlot.Style.Hazel);
            xTempGraph.Plot.Palette = Palette.Amber;

            TempSig = xTempGraph.Plot.AddSignal(TempData);
            TempSig.FillBelow(System.Drawing.Color.RosyBrown, System.Drawing.Color.IndianRed);
            TempSig.LineWidth = 5;
            TempSig.MarkerSize = 0;

            TempSig.MaxRenderIndex = 0;
            xTempGraph.Refresh();
        }

        #endregion

        public ChamberVM()
        {
            Global.gChamberVM = this;
            if (Global.gChamberUse > 0)
                _plcThread.Start(this);
            _statColors.Add(new SolidColorBrush(Colors.Gray)); // DISCON
            _statColors.Add(new SolidColorBrush(Colors.LimeGreen)); // RUN
            _statColors.Add(new SolidColorBrush(Colors.LightYellow)); // AMBI
            _statColors.Add(new SolidColorBrush(Colors.Gray)); // None
            _statColors.Add(new SolidColorBrush(Colors.LightBlue)); // STOP

            ChamCtrlWin = new ChamCTRL();
            ChamCtrlWin.Hide();
            ChamCtrlWin.DataContext = this;

            EquipID = Global.gEquipID;

            //            Commands.Add(new CommandBinding(CmdPLC, excute_CmdPLC, canExcute_CmdPLC));
        }


        public PLCModel getPLC(string name)
        {
            foreach(var plc in _plcs)
            {
                if (plc.Name == name)
                    return plc;
            }
            return null;
        }





        #region Chamber Ctrl Define
        PLCModel _curPLC = null;
        public PLCModel CurPLC
        {
            get { return _curPLC; }
            set { _curPLC = value; OnPropertyChanged("CurPLC"); }
        }

        string _setTemp = "25.0";
        public string SetTemp
        {
            get { return _setTemp; }
            set { _setTemp = value; OnPropertyChanged("SetTemp"); }
        }

        public void excute_CmdCtrl(Button bt)
        {
            if (bt == null) return;
            if (CurPLC == null) return;
            switch (bt.Content.ToString())
            {
                case "RUN":
                    CurPLC._modbus.setOperationFunc(PLC_STAT_TYPE.RUN);
                    break;
                case "STOP":
                    CurPLC._modbus.setOperationFunc(PLC_STAT_TYPE.STOP);
                    break;
                case "AMBIENT":
                    CurPLC._modbus.setOperationFunc(PLC_STAT_TYPE.AMBI);
                    break;
                case "REMOTE":
                    CurPLC._modbus.setRemoteFunc();
                    break;
                case "SET":
                    CurPLC._modbus.setTempFunc(Convert.ToDouble(SetTemp));
                    break;
                case "IN":
                    CurPLC._modbus.setRackFunc(1);
                    break;
                case "OUT":
                    CurPLC._modbus.setRackFunc(0);
                    break;
                case "OPEN":
                    CurPLC._modbus.setDoorFunc(0);
                    break;
                case "CLOSE":
                    CurPLC._modbus.setDoorFunc(1);
                    break;
                case "RED":
                    CurPLC._modbus.setLampFunc((CurPLC.LampStat & (int)LAMP_TYPE.R) == 0 ? true : false,
                        LAMP_TYPE.R, false);
                    break;
                case "RBLINK":
                    CurPLC._modbus.setLampFunc((CurPLC.LampStat & (int)LAMP_TYPE.RB) == 0 ? true : false,
                        LAMP_TYPE.R, true);
                    break;
                case "YELLOW":
                    CurPLC._modbus.setLampFunc((CurPLC.LampStat & (int)LAMP_TYPE.Y) == 0 ? true : false,
                        LAMP_TYPE.Y, false);
                    break;
                case "YBLINK":
                    CurPLC._modbus.setLampFunc((CurPLC.LampStat & (int)LAMP_TYPE.YB) == 0 ? true : false,
                        LAMP_TYPE.Y, true);
                    break;
                case "GREEN":
                    CurPLC._modbus.setLampFunc((CurPLC.LampStat & (int)LAMP_TYPE.G) == 0 ? true : false,
                        LAMP_TYPE.G, false);
                    break;
                case "GBLINK":
                    CurPLC._modbus.setLampFunc((CurPLC.LampStat & (int)LAMP_TYPE.GB) == 0 ? true : false,
                        LAMP_TYPE.G, true);
                    break;
                case "OWNER":
                    CurPLC._modbus.setLampOwnFunc( CurPLC.LampOwn == 1 ? 0 : 1 );
                    break;
                case "CTRL":

                    break;
                case "BUZZER":
                    CurPLC._modbus.setBuzzerFunc((CurPLC.LampStat & (int)LAMP_TYPE.BZ) == 0 ? 1 : 0);
                    break;

                    default: break;
            }

        }

            #endregion

    }
}
