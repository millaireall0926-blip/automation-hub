using MVVM_Loader.Common;
using MVVM_Loader.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MVVM_Loader.Model
{
    public class Item1Model : BaseVM
    {
        public int Ch1 { get; set; }
        public string Symbol1 { get; set; }
        public string Explain1 { get; set; }

        string _g1CH1_1 = "0";
        public string g1CH1_1
        {
            get { return _g1CH1_1; }
            set
            {
                _g1CH1_1 = value; OnPropertyChanged(nameof(g1CH1_1));
            }
        }
        string _g1CH2_1 = "0";
        public string g1CH2_1
        {
            get { return _g1CH2_1; }
            set
            {
                _g1CH2_1 = value; OnPropertyChanged(nameof(g1CH2_1));
            }
        }
        string _g1CH3_1 = "0";
        public string g1CH3_1
        {
            get { return _g1CH3_1; }
            set
            {
                _g1CH3_1 = value; OnPropertyChanged(nameof(g1CH3_1));
            }
        }
        string _g1CH4_1 = "0";
        public string g1CH4_1
        {
            get { return _g1CH4_1; }
            set
            {
                _g1CH4_1 = value; OnPropertyChanged(nameof(g1CH4_1));
            }
        }

        string _g1flag2_1 = "Visible";
        public string g1flag2_1
        {
            get { return _g1flag2_1; }
            set
            {
                _g1flag2_1 = value; OnPropertyChanged(nameof(g1flag2_1));
            }
        }
        public DelegateCommand Item1Command { get; set; }
        public DelegateCommand Descp1Command { get; set; }

        public Item1Model(int ch1, string symbol1, string explain1, string r1CH1_1, string r1CH2_1, string r1CH3_1, string r1CH4_1, string r1flag2_1)
        {
            this.Ch1 = ch1;
            this.Symbol1 = symbol1;
            this.Explain1 = explain1;
            this.g1CH1_1 = r1CH1_1;
            this.g1CH2_1 = r1CH2_1;
            this.g1CH3_1 = r1CH3_1;
            this.g1CH4_1 = r1CH4_1;
            this.g1flag2_1 = r1flag2_1;

            Item1Command = new DelegateCommand(p => { 
                Global.gSerialComm.Check_Upload(Ch1, g1CH1_1, g1CH2_1, g1CH3_1, g1CH4_1, _g1flag2_1, Symbol1); 
            });
            Descp1Command = new DelegateCommand(p => {
                //Global.gDesciViewModel.Description = Global.gDescription.infoDic[Explain1];
                DesciViewModel vml = (DesciViewModel)App.Current.Services.GetService(typeof(DesciViewModel));
                vml.Description = Global.gDescription.infoDic[Explain1]; 
                //viewModel.
                //Global.gDesciViewModel.Description = Global.gDescription.infoDic[Explain1];
            });
        }
    }
}
