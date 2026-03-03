using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace StarResult.Model
{
    public class DPSModel : BaseVM
    {
        public DPSModel()
        {
            _graphDataV = new double[200_000];
            _graphDataC = new double[200_000];
        }

        public void setData(double v, double c, int index)
        {
            _graphDataV[index] = v;
            _graphDataC[index] = c;
        }

        public double[] _graphDataV;
        public double[] _graphDataC;

        public int Index { get; set; }
        public string Name { get; set; }
        public bool Show { get; set; }
        public SolidColorBrush ColorV { get; set; }
        public SolidColorBrush ColorC { get; set; }
    }
}
