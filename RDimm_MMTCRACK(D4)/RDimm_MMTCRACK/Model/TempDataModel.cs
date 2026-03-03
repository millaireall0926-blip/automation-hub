using RDimm_MMTCRACK.Base;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace RDimm_MMTCRACK.Model
{
    public class TempDataModel : BaseVM
    {
        public string Name { get; set; }
        public bool Show { get; set; } = true;
        public double Temp { get; set; } = 0;
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
        public DelegateCommand DGToggle { get; set; }
        public TempDataModel()
        {
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
