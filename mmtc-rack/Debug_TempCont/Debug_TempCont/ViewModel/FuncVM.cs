using Debug_TempCont.Common;
using Debug_TempCont.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Debug_TempCont.ViewModel
{
    public class FuncVM :BaseVM
    {
        private FuncModel fcm = null;
        private TempChVM tcm = (TempChVM)App.Current.Services.GetService(typeof(TempChVM));
        public DelegateCommand TempSetCommand { get; set; }
        public DelegateCommand TempRunCommand { get; set; }
        public DelegateCommand TempStopCommand { get; set; }
        public FuncVM() 
        {
            fcm = new FuncModel();
            TempSetCommand = new DelegateCommand(p => { TempSetCheck(); });
            TempRunCommand = new DelegateCommand(p => { TempRunStopCheck(true); });            
            TempStopCommand = new DelegateCommand(p => { TempRunStopCheck(false); });
        }
        public FuncModel Fcm
        {
            get { return fcm; }
            set 
            { 
                fcm = value; 
                OnPropertyChanged(nameof(Fcm));
            }
        }
        private void TempSetCheck()
        {
            Global.setTempFlag = true;
            for (int i = 0; i < 4; i++)
            {
                tcm.TempDatas[i].SettingTemp(fcm.tempSetValue);
            }
            Global.sendWaitFlag = false;
            Global.setTempFlag = false;
        }
        private void TempRunStopCheck(bool rsFlag)
        {
            Global.setTempFlag = true;
            for (int i = 0; i < 4; i++)
            {
                if(rsFlag)
                    tcm.TempDatas[i].StatType = STAT_TYPE.RUN;
                else
                    tcm.TempDatas[i].StatType = STAT_TYPE.READY;
            }
            Global.sendWaitFlag = false;
            Global.setTempFlag = false;
        }
    }
}
