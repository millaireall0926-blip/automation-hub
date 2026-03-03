using TempView.Base;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace TempView.Model {
    public class DimDataModel : BaseVM {

        
        public string Name { get; set; }
        public bool Show { get; set; } = true;
        public double Temp { get; set; } = 0;
        public double Left { get; set; }
        public double Top { get; set; }
        
        public SolidColorBrush ChanelColor { get; set; }        

        [DoNotNotify]
        public double[] GraphData { get; set; } = new double[1_000_000];
       
        public DelegateCommand DGToggle { get; set; } 
        public DimDataModel() {
            
            DGToggle = new DelegateCommand( fun => ToggleClick(), can => true);
            DGToggle.CanExecute(true); 

        }

        public void addTemp(int index)
        {
            GraphData[index] = Temp;
        }

        public void ToggleClick()
        {
            Global.gGraphVM.setShow();

        }

        public class MarkerModel
        {
            public MarkerModel() { }
            public int X;  // index
            public double Y;  // dimm Temp
            public string Name;
        }


    }
}
