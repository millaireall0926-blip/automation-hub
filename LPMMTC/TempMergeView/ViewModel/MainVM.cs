using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TempMergeView.Base;

namespace TempMergeView.ViewModel
{
    public class MainVM : BaseVM
    {

        public string Selected { get; set; } = "";

        public double TimeLineInput { get; set; } = 0;
        public double TempLineInput { get; set; } = 0;

        public DelegateCommand LineCmd { get; set; }

        public DelegateCommand PathCmd { get; set; }

        public string DataPath { get; set; } = "";

        public MainVM() { 
            Global.gMainVM = this;
            LineCmd = new DelegateCommand(p => {
                if (p as string == "addTime")
                    Global.gGraphVM.addVLine(TimeLineInput);
                else if (p as string == "delTime")
                    Global.gGraphVM.removeVLine(); 
                else if (p as string == "addTemp")
                    Global.gGraphVM.addHLine(TempLineInput);    
                else if (p as string == "delTemp")
                    Global.gGraphVM.removeHLine(); 
                else if (p as string == "clear")
                    Global.gGraphVM.clearGuid();
            });

            DataPath = Global.gTempPath;

            PathCmd = new DelegateCommand(p => changePath());

        }

        public void changePath()
        {
            System.Windows.Forms.FolderBrowserDialog fdb = new System.Windows.Forms.FolderBrowserDialog();            
            if (fdb.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                DataPath = fdb.SelectedPath;
                Global.gSelectVM.clearFileList();
            }
        }
        

        public void selectFile(string name)
        {
            Selected = name;
            Global.gGraphVM.setFile(name);
        }
    }
}
