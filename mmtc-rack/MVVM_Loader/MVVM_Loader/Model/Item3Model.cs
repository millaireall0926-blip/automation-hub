using MVVM_Loader.Common;
using MVVM_Loader.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM_Loader.Model
{
    public class Item3Model : BaseVM
    {
        public int Ch3 { get; set; }
        public string Symbol3 { get; set; }
        public string Explain3 { get; set; }
        string _g3CH1_1 = "0";
        public string g3CH1_1
        {
            get { return _g3CH1_1; }
            set
            {
                _g3CH1_1 = value; OnPropertyChanged(nameof(g3CH1_1));
            }
        }
        string _g3CH2_1 = "0";
        public string g3CH2_1
        {
            get { return _g3CH2_1; }
            set
            {
                _g3CH2_1 = value; OnPropertyChanged(nameof(g3CH2_1));
            }
        }
        string _g3CH3_1 = "0";
        public string g3CH3_1
        {
            get { return _g3CH3_1; }
            set
            {
                _g3CH3_1 = value; OnPropertyChanged(nameof(g3CH3_1));
            }
        }
        string _g3CH4_1 = "0";
        public string g3CH4_1
        {
            get { return _g3CH4_1; }
            set
            {
                _g3CH4_1 = value; OnPropertyChanged(nameof(g3CH4_1));
            }
        }
        string _g3flag2_1 = "Visible";
        public string g3flag2_1
        {
            get { return _g3flag2_1; }
            set
            {
                _g3flag2_1 = value; OnPropertyChanged(nameof(g3flag2_1));
            }
        }
        public DelegateCommand Item3Command { get; set; }
        public DelegateCommand Descp3Command { get; set; }
        public Item3Model(int ch3, string symbol3, string explain3, string r3CH1_1, string r3CH2_1, string r3CH3_1, string r3CH4_1, string r3flag2_1)
        {
            this.Ch3 = ch3;
            this.Symbol3 = symbol3;
            this.Explain3 = explain3;
            this.g3CH1_1 = r3CH1_1;
            this.g3CH2_1 = r3CH2_1;
            this.g3CH3_1 = r3CH3_1;
            this.g3CH4_1 = r3CH4_1;
            this.g3flag2_1 = r3flag2_1;

            Item3Command = new DelegateCommand(p => {
                Global.gSerialComm.Check_Upload(Ch3, g3CH1_1, g3CH2_1, g3CH3_1, g3CH4_1, g3flag2_1, symbol3);
            });
            Descp3Command = new DelegateCommand(p => {
                //Global.gDesciViewModel.Description = Global.gDescription.infoDic[Explain3];
                DesciViewModel vml = (DesciViewModel)App.Current.Services.GetService(typeof(DesciViewModel));
                vml.Description = Global.gDescription.infoDic[Explain3];

            });
        }
    }
}
