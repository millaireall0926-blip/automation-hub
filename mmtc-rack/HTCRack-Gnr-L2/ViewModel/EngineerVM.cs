using HTCRack.Base;
using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Reflection;
using System.Windows;
using HTCRack.Model;
using ScottPlot.Drawing.Colormaps;
using HTCRack.Net;
using System.Linq;
using System.Windows.Threading;
using System.Windows.Controls;

namespace HTCRack.ViewModel
{

    public enum CMD_TYPE : int
    {
        SEND = 0,
        RECV,
        ERROR
    }

    public class CommandLog : BaseVM
    {

        public int Index { get; set; }
        public CMD_TYPE Type { get; set; }        
        public string Message { get; set; }
        public CommandLog(int index, CMD_TYPE type, string msg)
        {
            Index = index;
            Type = type;            
            Message = msg;
        }
    }

    public class EngineerVM : BaseVM
    {
        public string OutputMode { get; set; } = "Value";

        public ListView ListCmdLog;
        public ObservableCollection<CommandLog> CmdLogs { get; set; } = new ObservableCollection<CommandLog>();
        public void pushCmdLog(CommandLog cmd)
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                if (CmdLogs.Count >= 100)
                    CmdLogs.RemoveAt(0);
                CmdLogs.Add(cmd);

                ListCmdLog.ScrollIntoView(cmd);
            }));                
        }   

        public void clearCmdLog()
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() => CmdLogs.Clear() ));
        }



        #region Rack Select
        int _curRack = 0;
        public int CurRack
        {
            get => _curRack;
            set { _curRack = value; CurName = $"FIX" + (_curRack + 1).ToString(); }
        }

        public string CurName { get; set; } = "FIX1";

        public string EquipImg { get; set; } = "/Resource/equip_3x2.png";
        #endregion

        #region FAN : EMC2303 – 3CH Fan Controller
        public bool ExpandFan { get; set; } = false;
        public ObservableCollection<RegModel> FanRegModels { get; set; } = new ObservableCollection<RegModel>();
        #endregion

        #region CUR : INA226 – 1CH Current Monitor
        public bool ExpandCur { get; set; } = false;
        public ObservableCollection<RegModel> CurRegModels { get; set; } = new ObservableCollection<RegModel>();
        #endregion


        #region TEMP : TMP102 – 1CH Temperature Sensor
        public bool ExpandTemp { get; set; } = false;
        public ObservableCollection<RegModel> TempRegModels { get; set; } = new ObservableCollection<RegModel>();
        #endregion

        #region FP : Front Pannel – SV/PV Display
        public bool ExpandFP { get; set; } = false;
        public ObservableCollection<RegModel> FPRegModels { get; set; } = new ObservableCollection<RegModel>();

        #endregion

        #region DIMM : TSOD Temperature
        public bool ExpandDimm { get; set; } = false;
        public ObservableCollection<RegModel> DimRegModels { get; set; } = new ObservableCollection<RegModel>();

        #endregion

        #region TCON : M74 – PID Controller
        public bool ExpandTCon { get; set; } = false;
        public ObservableCollection<RegModel> TConRegModels { get; set; } = new ObservableCollection<RegModel>();

        #endregion

        #region GPIO
        public bool ExpandGPIO { get; set; } = false;
        public ObservableCollection<RegModel> GPIORegModels { get; set; } = new ObservableCollection<RegModel>();

        #endregion

        public EngineerVM() { 
            Global.gEnginerVM = this;

            if (Global.gMainPageVM.LayoutColumnCount == 1)
                EquipImg = "/Resource/equip_col1.png";
            else if (Global.gMainPageVM.LayoutColumnCount == 2)
                EquipImg = "/Resource/equip_3x2.png";

            RegMap.Fan_Multi =  FanRegModels.Count;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "", "", "0","0x", "WRITE REG(Multi)", noRead: true, send: true, single: false));
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "", "", "1","0x", "Read REG(Count Input \"Write\")", send: true, single: false));

            RegMap.Fan_SetDuty = FanRegModels.Count;
            int adr = 0;                 //  Type       CH         ADR          FUNC   DISC                            SEND
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN1",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN2",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN3",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN4",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN5",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN6",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN7",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Temp Box FAN8",            noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Spare FAN ",               noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Heat Sink1 Block FAN1",    noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Heat Sink1 Block FAN2",    noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Heat Sink1 Block FAN3",    noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Heat Sink1 Block FAN4",    noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Peltier Ctrl FAN1",        noRead: true,  send: true)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff",  adr.ToString(), "10", "%", "SET Peltier Ctrl CASE FAN2",   noRead: true,  send: true));

            RegMap.Fan_GetDuty = FanRegModels.Count;
            adr = 0;                 //  Type       CH         ADR          FUNC   DISC                            SEND
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN1"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN2"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN3"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN4"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN5"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN6"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN7"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Temp Box FAN8"        )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Spare FAN "           )); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Heat Sink1 Block FAN1")); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Heat Sink1 Block FAN2")); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Heat Sink1 Block FAN3")); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Heat Sink1 Block FAN4")); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Peltier Ctrl FAN1")); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "11", "%", "GET Peltier Ctrl CASE FAN2"));

            RegMap.Fan_GetTach = FanRegModels.Count;
            adr = 0;                 //  Type       CH         ADR          FUNC   DISC                            SEND
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN1"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN2"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN3"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN4"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN5"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN6"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN7"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Temp Box FAN8"            , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Spare FAN "               , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Heat Sink1 Block FAN1"    , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Heat Sink1 Block FAN2"    , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Heat Sink1 Block FAN3"    , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Heat Sink1 Block FAN4"    , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Peltier Ctrl FAN1"        , len:2)); adr++;
            FanRegModels.Add(new RegModel(REG_TYPE.FAN, "ff", adr.ToString(), "20", "D", "RPM Peltier Ctrl CASE FAN2"   , len:2));

            RegMap.Cur_Multi = CurRegModels.Count;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "", "", "0", "0x", "WRITE REG(Multi)", noRead: true, send: true, single: false));
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "", "", "1", "0x", "Read REG(Count Input \"Write\")", send: true, single: false));
            RegMap.Cur_GetCur = CurRegModels.Count;
            adr = 0;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 1 (Temp Box1) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 2 (Temp Box1) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 3 (Temp Box2) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 4 (Temp Box2) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 5 (Temp Box3) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 6 (Temp Box3) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 7 (Temp Box4) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Peltier 8 (Temp Box4) 24V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Water Pump 12V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Radiator FAN 12V Monitor"));adr++;
            CurRegModels.Add(new RegModel(REG_TYPE.CUR, "ff", adr.ToString(), "11", "A", "Back Plane Output Power 12V Monitor"));adr++;

            RegMap.Temp_Multi = TempRegModels.Count;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "", "", "0", "0x", "WRITE REG(Multi)", noRead: true, send: true, single: false));
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "", "", "1", "0x", "Read REG(Count Input \"Write\")", send: true, single: false));
            RegMap.Temp_GetTemp = TempRegModels.Count;
            adr = 0;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL1 Monitor0")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL1 Monitor1")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL2 Monitor0")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL2 Monitor1")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL3 Monitor0")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL3 Monitor1")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL4 Monitor0")); adr++;
            TempRegModels.Add(new RegModel(REG_TYPE.TEMP, "ff", adr.ToString(), "11", "°C", "FET Switch BL4 Monitor1")); 


            adr = 0;            
            FPRegModels.Add(new RegModel(REG_TYPE.FP, "ff", adr.ToString(), "10", "°C", $"SET Display SV ", noRead: true, send: true)); adr++;
            
            adr = 0;            
            FPRegModels.Add(new RegModel(REG_TYPE.FP, "ff", adr.ToString(), "11", "D",$"SET PVMap 4byte(EachByte represents Index)", noRead: true, send: true)); adr++;
            

            RegMap.Dim_GetDim = DimRegModels.Count;
            adr = 0;
            for (int i = 0; i < (Global.gMainPageVM.MaxFixDimmCount * 2); i++)
            { 
                DimRegModels.Add(new RegModel(REG_TYPE.DIMM, "ff", adr.ToString(), "10", "°C", $"DIMM{adr + 1}")); adr++;
            }
            RegMap.DimTempCount = DimRegModels.Count;
            DimRegModels.Add(new RegModel(REG_TYPE.DIMM, "ff", "0", "21", "D", "Dimm Cnt : 16/24/31 (default 16)", send: true, len: 2));


            #region TCON Reg Setup
            RegMap.TCon_Multi = TConRegModels.Count;
            TConRegModels.Add(new RegModel(REG_TYPE.TCON, "", "", "0","0x", "WRITE REG(Multi)", noRead: true, send: true, single:false));
            TConRegModels.Add(new RegModel(REG_TYPE.TCON, "", "", "1","0x", "Read REG(Count Input \"Write\")", send: true, single: false)); 
            
            RegMap.TCon_SetSV = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "10", "°C", $"Set TCON SV{adr + 1}", noRead: true, send: true)); adr++;
            }

            RegMap.TCon_SetCtrl = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "11", "0x", $"Set TCON CTRL{adr + 1}", noRead: true, send: true)); adr++;
            }

            RegMap.TCon_GetSV = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "20", "°C", $"GET TCON SV{adr+1}")); adr++;
            }

            RegMap.TCon_GetPV = TConRegModels.Count;            
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "21", "°C", $"Get TCON PV{i + 1}")); adr++;
            }

            RegMap.TCon_GetMV = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "22", "%", $"Get TCON MV{adr + 1}")); adr++;
            }

            RegMap.TCon_GetCHSTS = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "23", "0x", $"Get TCON CH.STS{adr + 1}")); adr++;
            }

            RegMap.TCon_GetALM = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < 8; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "24", "0x", $"Get TCON ALM.STS{adr + 1}")); adr++;
            }

            RegMap.TCon_GetRunST = TConRegModels.Count;
            TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", "0", "40", "0x", $"RUN STAT (SW for Output)"));
            RegMap.TCon_GetTuneST = TConRegModels.Count;
            TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", "0", "41", "0x", $"TUNNING (SW for Output)"));

            RegMap.TCon_GetCoolerPV = TConRegModels.Count;
            adr = 0;
            for (int i = 0; i < RegMap.MaxCoolerCount; i++)
            {
                TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", adr.ToString(), "21", "°C", $"Get COOLER PV{adr + 1}")); adr++;
            }

            RegMap.TCon_SetBS1 = TConRegModels.Count;
            TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", "0", "30", "°C", $"SetTCON_BS1", noRead: true, send: true));
            RegMap.TCon_GetBS1 = TConRegModels.Count;
            TConRegModels.Add(new RegModel(REG_TYPE.TCON, "ff", "0", "31", "°C", $"GetTCON_BS1"));


            #endregion
            RegMap.GPIO_SSRCtrl = GPIORegModels.Count;
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "10", "0x","Set SSR Ctrl. : 0 - Off / 1 - On", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "11", "0x","Set SSR IN: 0 - Bypass / 1 - Inv", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "12", "0x","Set SSR OUT: 0 - Bypass / 1 - Inv", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "13", "0x","Set SSR CH: 0 - Single / 1 - Dual", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "14", "0x","Set SSR ST EN : 0 - Dis / 1 - En", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "15", "0x","Set Ext FAN Ctrl. : 0 - Off / 1 - On", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "16", "0x","Set Pump Ctrl. : 0 - Off / 1 - On", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "17", "0x","Set RAD FAN Ctrl. : 0 - Off / 1 - On", send: true));
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "18", "0x","Get Slot ID: 0 ~9", send: true));
            RegMap.GPIO_BDCon = GPIORegModels.Count;
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "19", "0x","Get BD Con Check : 0 - N / 1 - Yes", send: true));
            RegMap.GPIO_SideClose = GPIORegModels.Count;
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "1a", "0x","Get Slide Close: 0 - No / 1 - Yes", send: true));
            RegMap.GPIO_SSR_Pulse = GPIORegModels.Count;
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "20", "D", "Set SSRPulse:0-Normal/Pulse(ms) ex)5ms=100hz,50ms=10hz", send: true));
            RegMap.GPIO_SSR_Cold = GPIORegModels.Count;
            GPIORegModels.Add(new RegModel(REG_TYPE.GPIO, "ff", "0", "30", "0x", "Set SSRCold In:0-Bypass/1-Inv.", send: true));


            foreach (var rack in Global.gMainPageVM.Racks)          
                rack.initRegs();

            foreach (RegModel reg in FanRegModels)
            {
                if (reg.FUNC == "10") reg.funcWriteToHex += doHexToFanSetDuty;
            }

            foreach (RegModel reg in FPRegModels)
            {
                reg.funcWriteToHex += doHexToFPSet;
            }

            foreach (RegModel reg in TConRegModels)
            {
                if (reg.FUNC == "10")
                    reg.funcWriteToHex += doHexToTconSV;
            }

        }

        #region Write Value Change Delegate Functions
        public string doHexToFanSetDuty(double value)
        {
            int conv = (int)value;
            conv = (conv * 256) / 100;
            if (conv > 0xff) conv = 0xff;
            string ret = conv.ToString("X2");
            return ret;
        }

        public string doHexToFPSet(double value)
        {
            int conv = (int)(value * 10.0);

            byte flag = 0;
            byte ten = 0;
            byte one = 0;
            byte point = 0;
            if (conv < 0) { flag = 1; conv *= -1; }
            ten = (byte)(conv / 100);
            one = (byte)((conv%100) / 10);
            point = (byte)(conv % 10);

            string ret = flag.ToString("X2") + ten.ToString("X2") + one.ToString("X2") + point.ToString("X2");

            return ret;

        }

        public string doHexToTconSV(double value)
        {
            int conv = (int)(value * 10.0);
            string ret = conv.ToString("X4");
            return ret;
        }

        #endregion

        public void setRack()
        {
            if (Global.gMainVM.ShowEngineer != Visibility.Visible) return;
            //CurRack 
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            int index = 0;
            foreach(RegModel rm in rack.FanRegs)
            {
                FanRegModels[index].Read = rm.Read;
                FanRegModels[index].ReadValue = rm.ReadValue;
                index++;
            }
            index = 0;
            foreach (RegModel rm in rack.CurRegs)
            {
                CurRegModels[index].Read = rm.Read;
                CurRegModels[index].ReadValue = rm.ReadValue;
                index++;
            }
            index = 0;
            foreach (RegModel rm in rack.TempRegs)
            {
                TempRegModels[index].Read = rm.Read;
                TempRegModels[index].ReadValue = rm.ReadValue;
                index++;
            }
            index = 0;
            foreach (RegModel rm in rack.DimRegs)
            {
                DimRegModels[index].Read = rm.Read;
                DimRegModels[index].ReadValue = rm.ReadValue;
                index++;
            }
            index = 0;
            foreach (RegModel rm in rack.TConRegs)
            {
                TConRegModels[index].Read = rm.Read;
                TConRegModels[index].ReadValue = rm.ReadValue;
                index++;
            }
            index = 0;
            foreach (RegModel rm in rack.GPIORegs)
            {
                GPIORegModels[index].Read = rm.Read;
                GPIORegModels[index].ReadValue = rm.ReadValue;
                index++;
            }


        }





        public void Send(RegModel rm)
        {
            if (rm == null)
                return;
            try
            {
                

                if (rm.CH == "" || rm.ADR == "")
                {
                    MessageBox.Show("Empty CH or ADR"); return;
                }

                HtcMessage msg = Global.newMsg(Convert.ToByte(rm.CH, 16), Convert.ToByte(rm.ADR), Convert.ToByte(rm.FUNC, 16));                
                string[] datas = rm.Write.Split(' ');
                int count = datas.Length;
                //msg.sub.Len = (byte)(count * rm.Len);                 
                msg.data = new byte[(byte)(count * rm.Len)];
 
                int pos = 0;
                for (int i = 0; i < datas.Length; i++)
                {
                    for (int j = 0; j < rm.Len; j++)
                    {
                        int iVal = Convert.ToInt32(datas[i], 16);
                        byte bVal= (byte)((iVal >> (j * 8)) & 0xff);
                        msg.data[pos++] = bVal;                         
                    }
                }

 
                RackModel rack = Global.gMainPageVM.getRack(CurRack);
                rack._client.pushSendQue(rm.Type, msg);

            }catch (Exception ex)
            {
                //Console.WriteLine(ex.ToString());
                MessageBox.Show(ex.Message);
            }

        }

        public string getProperty(string name)
        {
            
            PropertyInfo info =  typeof(EngineerVM).GetProperty(name);
            if (info == null) return "";
            return info.GetValue(this) as string;
           
            
        }

        public void setProperty(string name, string value)
        {
            PropertyInfo info = typeof(EngineerVM).GetProperty(name);
            if (info == null) return;
            info.SetValue(this, value);
        }


        public void setField(string name, string value)
        {
            Type type = typeof(EngineerVM);
            FieldInfo fld = type.GetField(name, (BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance));
            fld.SetValue(this, "value");
        }

        public string getField(string name) 
        {
            Type type = typeof(EngineerVM);
            FieldInfo field = type.GetField(name, (BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance));
            if (field == null) return "";
            return  field.GetValue(this) as string;            
        }

    }
}
