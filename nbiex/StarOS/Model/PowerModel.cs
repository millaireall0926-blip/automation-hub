using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;
using StarBase;

namespace StarOS.Model
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

        public string Name { get; set; }    
        
        public bool Show { get; set; }
        public string SetV { get; set; }
        public string GetV { get; set; }
        public string OC { get; set; }        
        public string GetC { get; set; }        
        public SolidColorBrush ColorV { get; set; }
        public SolidColorBrush ColorC { get; set; }


    }

    public class HVModel : BaseVM
    {
        public HVModel()
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

		public string Name { get; set; }
		public bool Show { get; set; }
		public string SetV {  set; get; }
		public string GetV { get; set; }
		public string OC { get; set; }
		public string GetC { get; set; }
		public SolidColorBrush ColorV { get; set; }
		public SolidColorBrush ColorC { get; set; }
	}

    public class DACModel : BaseVM
    {
		public string Name { get; set; }
		public string SetV { set; get; }
		public SolidColorBrush ColorV { get; set; }
		public SolidColorBrush ColorC { get; set; }
	}
}
