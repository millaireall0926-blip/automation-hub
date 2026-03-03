using HTCRack.Base;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace HTCRack.Model {
    public class DimDataModel : BaseVM {

        
        public string Name { get; set; }
        public bool Show { get; set; } = true;

        public string TempText { get; set; } = "";
        public double Temp { get; set; } = 0;
        public double TempLow { get; set; } = 0;
        public double TempHigh { get; set; } = 0;

        public double Left { get; set; }
        public double Top { get; set; }

        SolidColorBrush _chanelColor;
        public SolidColorBrush ChanelColor { get => _chanelColor; set { _chanelColor = value; PC("ChanelColor"); }}

        SolidColorBrush _statColor;
        public SolidColorBrush StatColor { get => _statColor; set { _statColor = value; PC("StatColor"); } }

        [DoNotNotify]
        public double[] GraphData { get; set; } = new double[1_000_000];
       
        public DelegateCommand DGToggle { get; set; } 
        public DimDataModel() {

            StatColor = Global.gDimStatColors[5];
            DGToggle = new DelegateCommand( fun => ToggleClick(), can => true);
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

    public class MarkerModel
    {
        public MarkerModel() { }
        public int X;  // index
        public double Y;  // dimm Temp
        public string Name;
    }
}
