using HTCRack.Base;
using HTCRack.Model;
using PropertyChanged;
using ScottPlot.Drawing.Colormaps;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace HTCRack.ViewModel
{
    public class BoardVM : BaseVM
    {
        public enum SETUP_CHECK : int
        {
            UNCHECK = 0,
            CHECK
        };

        public SolidColorBrush[] CheckStatColor = new SolidColorBrush[]
        {
            // RACK_STAT Index
            new SolidColorBrush(Colors.DarkGoldenrod),    // UnCheck            
            new SolidColorBrush(Colors.CadetBlue),      // Checked            
        };

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

        #region HeatSink1
        public SolidColorBrush HeatSink1Color { get; set; }
        public string HeatSink1Text { get; set; }
        [DoNotNotify]
        bool HeatSink1Check { get; set; } = false;
        #endregion

        #region HeatSink2
        public SolidColorBrush HeatSink2Color { get; set; }
        public string HeatSink2Text { get; set; }
        [DoNotNotify]
        bool HeatSink2Check { get; set; } = false;
        #endregion

        #region HeatSink3
        public SolidColorBrush HeatSink3Color { get; set; }
        public string HeatSink3Text { get; set; }
        [DoNotNotify]
        bool HeatSink3Check { get; set; } = false;
        #endregion

        #region HeatSink4
        public SolidColorBrush HeatSink4Color { get; set; }
        public string HeatSink4Text { get; set; }
        [DoNotNotify]
        bool HeatSink4Check { get; set; } = false;
        #endregion

        #region RPM Peltier1
        public SolidColorBrush Peltier1Color { get; set; }
        public string Peltier1Text { get; set; }
        [DoNotNotify]
        bool Peltier1Check { get; set; } = false;
        #endregion

        #region PeltierCase2 
        public SolidColorBrush PeltierCase2Color { get; set; }
        public string PeltierCase2Text { get; set; }
        [DoNotNotify]
        bool PeltierCase2Check { get; set; } = false;
        #endregion

        #region TempBox1
        public SolidColorBrush TempBox1Color { get; set; }
        public string TempBox1Text { get; set; }
        [DoNotNotify]
        bool TempBox1Check { get; set; } = false;
        #endregion

        #region TempBox3
        public SolidColorBrush TempBox3Color { get; set; }
        public string TempBox3Text { get; set; }
        [DoNotNotify]
        bool TempBox3Check { get; set; } = false;
        #endregion

        #region TempBox5
        public SolidColorBrush TempBox5Color { get; set; }
        public string TempBox5Text { get; set; }
        [DoNotNotify]
        bool TempBox5Check { get; set; } = false;
        #endregion

        #region TempBox7
        public SolidColorBrush TempBox7Color { get; set; }
        public string TempBox7Text { get; set; }
        [DoNotNotify]
        bool TempBox7Check { get; set; } = false;
        #endregion

        public string UnitInfo1 { get; set; }
        public string UnitInfo2 { get; set; }
        public string UnitInfo3 { get; set; }
        public string UnitInfo4 { get; set; }
        public string UnitInfo5 { get; set; }
        public string UnitInfo6 { get; set; }
        public string UnitInfo7 { get; set; }
        public string UnitInfo8 { get; set; }


        public SolidColorBrush BoarderColor { get; set; }
        public DelegateCommand DGCmdBoard { get; set; }


        public BoardVM()
        {
            Global.gBoardVM = this;
            DGCmdBoard = new DelegateCommand((p) => doCmdBoard((string)p));



            resetCheck();
            setRack(isRead: false);
        }

        public void doCmdBoard(string type)
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            if (type == "reset")
            {
                resetCheck();
            }
            else if (type == "hot")
            {
                Global.gMainVM.PlcCtl.setSlotMC(rack.RackIndex, true);
                Thread.Sleep(1000);
                //int cnt = 0;
                foreach (CBoxModel box in rack.CBoxs)
                {
                    int ssr = 0;
                    foreach (CBoxSub sub in box.BoxSubs)
                    {
                        ssr |= sub.SSRValue;
                    }

                 //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"Box{cnt} : SSR -> {ssr.ToString("X")}");
                 //   cnt++;
                    box.sendSV(70);
                    box.sendRun();
                    box.sendFan(100);

                    rack.addSSR(ssr);
                }
            }
            else if (type == "cold")
            {
                Global.gMainVM.PlcCtl.setSlotMC(rack.RackIndex, true);
                Thread.Sleep(1000);

                foreach (CBoxModel box in rack.CBoxs)
                {
                    int ssr = 0;
                    foreach (CBoxSub sub in box.BoxSubs)
                    {
                        ssr |= sub.SSRValue;
                    }

                    //Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"Box{cnt} : SSR -> {ssr.ToString("X")}");
                    //   cnt++;

                    box.sendSV(5);
                    box.sendRun();
                    box.sendFan(100);

                    rack.addSSR(ssr);
                }
            }
            else if (type == "stop")
            {
                foreach (CBoxModel box in rack.CBoxs)
                {
                    box.sendStop();
                    box.sendFan(40);
                }
                rack.clearSSR();
                Thread.Sleep(1000);
                Global.gMainVM.PlcCtl.setSlotMC(rack.RackIndex, false);
            }
        }

        public void resetCheck()
        {
            HeatSink1Check = false;
            HeatSink2Check = false;
            HeatSink3Check = false;
            HeatSink4Check = false;
            Peltier1Check = false;
            PeltierCase2Check = false;
            TempBox1Check = false;
            TempBox3Check = false;
            TempBox5Check = false;
            TempBox7Check = false;


        }

        public void checkSetup()
        {
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            int index = RegMap.Fan_GetTach + 9;
            if (rack.FanRegs[index].ReadValue > 1000) HeatSink1Check = true;
            index = RegMap.Fan_GetTach + 10;
            if (rack.FanRegs[index].ReadValue > 1000) HeatSink2Check = true;
            index = RegMap.Fan_GetTach + 11;
            if (rack.FanRegs[index].ReadValue > 1000) HeatSink3Check = true;
            index = RegMap.Fan_GetTach + 12;
            if (rack.FanRegs[index].ReadValue > 1000) HeatSink4Check = true;
            index = RegMap.Fan_GetTach + 13;
            if (rack.FanRegs[index].ReadValue > 1000) Peltier1Check = true;
            index = RegMap.Fan_GetTach + 14;
            if (rack.FanRegs[index].ReadValue > 1000) PeltierCase2Check = true;


            index = RegMap.Fan_GetTach;
            if (rack.FanRegs[index].ReadValue > 1000) TempBox1Check = true;
            index = RegMap.Fan_GetTach + 2;
            if (rack.FanRegs[index].ReadValue > 1000) TempBox3Check = true;
            index = RegMap.Fan_GetTach + 4;
            if (rack.FanRegs[index].ReadValue > 1000) TempBox5Check = true;
            index = RegMap.Fan_GetTach + 6;
            if (rack.FanRegs[index].ReadValue > 1000) TempBox7Check = true;



            HeatSink1Color = CheckStatColor[(int)(HeatSink1Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            HeatSink2Color = CheckStatColor[(int)(HeatSink2Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            HeatSink3Color = CheckStatColor[(int)(HeatSink3Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            HeatSink4Color = CheckStatColor[(int)(HeatSink4Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];

            Peltier1Color = CheckStatColor[(int)(Peltier1Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            PeltierCase2Color = CheckStatColor[(int)(PeltierCase2Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];

            TempBox1Color = CheckStatColor[(int)(TempBox1Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            TempBox3Color = CheckStatColor[(int)(TempBox3Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            TempBox5Color = CheckStatColor[(int)(TempBox5Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];
            TempBox7Color = CheckStatColor[(int)(TempBox7Check ? SETUP_CHECK.CHECK : SETUP_CHECK.UNCHECK)];

        }


        public void setRack(bool isRead)
        {
            if (Global.gMainVM.ShowBoard != Visibility.Visible) return;
            RackModel rack = Global.gMainPageVM.getRack(CurRack);
            int index = RegMap.Fan_GetTach + 9;
            HeatSink1Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 10;
            HeatSink2Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 11;
            HeatSink3Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 12;
            HeatSink4Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 13;
            Peltier1Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 14;
            PeltierCase2Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach;
            TempBox1Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 2;
            TempBox3Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 4;
            TempBox5Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            index = RegMap.Fan_GetTach + 6;
            TempBox7Text = rack.FanRegs[index].Discription +
                "\r\n\t" + rack.FanRegs[index].ReadValue.ToString();

            checkSetup();
            BoarderColor = Global.gBoarderColors[(int)rack.RackStat];

            index = 2;
            UnitInfo1 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo1 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo2 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo2 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo3 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo3 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo4 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo4 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo5 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo5 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo6 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo6 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo7 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo7 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";
            index++;
            UnitInfo8 = rack.TempRegs[index].ReadValue.ToString("F1") + " °C" + "\r\n";
            UnitInfo8 += rack.CurRegs[index].ReadValue.ToString("F1") + " A";


        }
    }
}
