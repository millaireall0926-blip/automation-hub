using MVVM_Loader.Common;
using MVVM_Loader.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM_Loader.Model
{
    public class Item2Model:BaseVM
    {
        public int Ch2 { get; set; }
        public string Symbol2 { get; set; }
        public string Explain2 { get; set; }
        string _g2CH1_1 = "0";
        public string g2CH1_1
        {
            get { return _g2CH1_1; }
            set
            {
                _g2CH1_1 = value; OnPropertyChanged(nameof(g2CH1_1));
            }
        }
        string _g2CH2_1 = "0";
        public string g2CH2_1
        {
            get { return _g2CH2_1; }
            set
            {
                _g2CH2_1 = value; OnPropertyChanged(nameof(g2CH2_1));
            }
        }
        string _g2CH3_1 = "0";
        public string g2CH3_1
        {
            get { return _g2CH3_1; }
            set
            {
                _g2CH3_1 = value; OnPropertyChanged(nameof(g2CH3_1));
            }
        }
        string _g2CH4_1 = "0";
        public string g2CH4_1
        {
            get { return _g2CH4_1; }
            set
            {
                _g2CH4_1 = value; OnPropertyChanged(nameof(g2CH4_1));
            }
        }
        string _g2flag2_1 = "Visible";
        public string g2flag2_1
        {
            get { return _g2flag2_1; }
            set
            {
                _g2flag2_1 = value; OnPropertyChanged(nameof(g2flag2_1));
            }
        }
        public DelegateCommand Item2Command { get; set; }
        public DelegateCommand Descp2Command { get; set; }
        public Item2Model(int ch2, string symbol2, string explain2, string r2CH1_1, string r2CH2_1, string r2CH3_1, string r2CH4_1, string r2flag2_1)
        {
            this.Ch2 = ch2;
            this.Symbol2 = symbol2;
            this.Explain2 = explain2;
            this.g2CH1_1 = r2CH1_1;
            this.g2CH2_1 = r2CH2_1;
            this.g2CH3_1 = r2CH3_1;
            this.g2CH4_1 = r2CH4_1;
            this.g2flag2_1 = r2flag2_1;

            Item2Command = new DelegateCommand(p => {
                Global.gSerialComm.Check_Upload(Ch2, g2CH1_1, g2CH2_1, g2CH3_1, g2CH4_1, g2flag2_1, symbol2);
            });
            Descp2Command = new DelegateCommand(p => {
                //Global.gDesciViewModel.Description = Global.gDescription.infoDic[Explain2];
                DesciViewModel vml = (DesciViewModel)App.Current.Services.GetService(typeof(DesciViewModel));
                vml.Description = Global.gDescription.infoDic[Explain2];
            });
        }
    }
}
