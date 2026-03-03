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
            _graphDataV = new double[1_000_000];
            _graphDataC = new double[1_000_000];
        }

        public void setData(double v, double c, int index)
        {
            _graphDataV[index] = v;
            _graphDataC[index] = c;
        }



        public double[] _graphDataV;
        public double[] _graphDataC;

        bool _show = false;
        public bool Show
        {
            get { return _show; }
            set { _show = value; OnPropertyChanged("Show"); }
        }


        string _setV;
        public string SetV
        {
            get { return _setV; }
            set {
                _setV = value;
                OnPropertyChanged("SetV");
            }
        }

        string _getV;
        public string GetV
        {
            get { return _getV; }
            set
            {
                _getV = value;
                OnPropertyChanged("GetV");
            }
        }

        string _oc;
        public string OC
        {
            get { return _oc; }
            set
            {
                _oc = value;
                OnPropertyChanged("OC");
            }
        }

        string _getC;
        public string GetC
        {
            get { return _getC; }
            set
            {
                _getC = value;
                OnPropertyChanged("GetC");
            }
        }

        SolidColorBrush _colorV;
        public SolidColorBrush ColorV
        {
            get { return _colorV; }
            set
            {
                _colorV = value;
                OnPropertyChanged("ColorV");
            }
        }

        SolidColorBrush _colorC;
        public SolidColorBrush ColorC
        {
            get { return _colorC; }
            set
            {
                _colorC = value;
                OnPropertyChanged("ColorC");
            }
        }

    }

    public class PEModel : BaseVM
    {
        string _ioV;
        public string IOV
        {
            get => _ioV;
            set
            {
                _ioV = value;
                OnPropertyChanged("IOV");
            }
        }

        string _clkV;
        public string CLKV
        {
            get => _clkV;
            set
            {
                _clkV = value;
                OnPropertyChanged("CLKV");
            }
        }

        string _drV;
        public string DRV
        {
            get => _drV;
            set
            {
                _drV = value;
                OnPropertyChanged("DRV");
            }
        }

        string _scanV;
        public string ScanV
        {
            get => _scanV;
            set
            {
                _scanV = value;
                OnPropertyChanged("ScanV");
            }
        }
    }
}
