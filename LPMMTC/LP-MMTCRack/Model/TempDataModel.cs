using LP_MMTCRack.Base;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace LP_MMTCRack.Model
{
    public class TempDataModel : BaseVM
    {
        public string Name { get; set; } = string.Empty;
        public bool Show { get; set; } = true;
        public int boxindex { get; set; } = 0;
        public double Temp { get; set; } = 0;
        public double TempSV { get; set; } = 0;
        public double TempPV { get; set; } = 0;

        public DateTime nowTime {  get; set; } = DateTime.Now;
        SolidColorBrush _chanelColor;
        public SolidColorBrush ChannelColor
        {
            get => _chanelColor;
            set
            {
                _chanelColor = value;
                PC("ChannelColor");
            }
        }
        [DoNotNotify]
        public double[] GraphData { get; set; } = new double[1_000_000];

        [DoNotNotify]
        public DateTime[] TimeData { get; set; } = new DateTime[1_000_000];

        public DelegateCommand DGCButton { get; set; }
        public DelegateCommand DGToggle { get; set; }
        public TempDataModel()
        {
            DGCButton = new DelegateCommand(p => 
            {
                int Getval = (Convert.ToInt32(p) - 1);
                Global.gGraphVM.CurBox = Getval;
                Global.gGraphVM.SetBox();
            });
                //DGCButton = new DelegateCommand(fun => ButtonClick(), can => true);
                //DGCButton.CanExecute(true);
            DGToggle = new DelegateCommand(fun => ToggleClick(), can => true);
            DGToggle.CanExecute(true);
        }
        public void addTemp(int index)
        {
            GraphData[index] = Temp;
        }
        public void ToggleClick()
        {
            bool isShow = Show;

        }
    }
}
