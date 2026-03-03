using MVVM_Loader.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MVVM_Loader.Model
{
    public class ConnetModel : BaseVM
    {
        public List<string> PortList { get; set; } = new List<string>();
        public List<string> ProtocolList { get; set; } = new List<string>()
        {
            "H-SUM", "H-TL", "Modbus-ASCII", "Modbus-RTU"
        };
        public List<string> SpeedList { get; set; } = new List<string>()
        {
            "9600", "19200", "38400", "76800", "115200"
        };
        public List<string> ParityList { get; set; } = new List<string>()
        {
            "None", "Odd", "Even"
        };
        public List<string> StopBitList { get; set; } = new List<string>()
        {
            "1", "2"
        };
        public List<string> DataLengthList { get; set; } = new List<string>()
        {
            "7", "8"
        };
        private string _SelPort = "";
        public string SelPort
        {
            get { return _SelPort; }
            set
            {
                _SelPort = value; OnPropertyChanged(nameof(SelPort));
            }
        }
        private string _M7num = "01";
        public string M7num
        {
            get { return _M7num; }
            set
            {
                _M7num = value; OnPropertyChanged(nameof(M7num));
            }
        }
        private string _Wrespon = "1";
        public string Wrespon
        {
            get { return _Wrespon; }
            set
            {
                _Wrespon = value; OnPropertyChanged(nameof(Wrespon));
            }
        }
        private string _Protocoltxt = "";
        public string Protocoltxt
        {
            get { return _Protocoltxt; }
            set
            {
                _Protocoltxt = value; OnPropertyChanged(nameof(Protocoltxt));
            }
        }
        private string _Selspeed = "";
        public string Selpeed
        {
            get { return _Selspeed; }
            set
            {
                _Selspeed = value; OnPropertyChanged(nameof(Selpeed));
            }
        }
        private string _Selparity = "";
        public string Selparity
        {
            get { return _Selparity; }
            set
            {
                _Selparity = value; OnPropertyChanged(nameof(Selparity));
            }
        }
        private string _Selstopbit = "";
        public string Selstopbit
        {
            get { return _Selstopbit; }
            set
            {
                _Selstopbit = value; OnPropertyChanged(nameof(Selstopbit));
            }
        }
        private string _Seldatalen = "";
        public string Seldatalen
        {
            get { return _Seldatalen; }
            set
            {
                _Seldatalen = value; OnPropertyChanged(nameof(Seldatalen));
            }
        }
        private string _Isenabed = "True";
        public string Isenabed
        {
            get { return _Isenabed; }
            set
            {
                _Isenabed = value; OnPropertyChanged(nameof(Isenabed));
            }
        }
        private string _Isenabed2 = "False";
        public string Isenabed2
        {
            get { return _Isenabed2; }
            set
            {
                _Isenabed2 = value; OnPropertyChanged(nameof(Isenabed2));
            }
        }
        private string _Statusck = "";
        public string Statusck
        {
            get { return _Statusck; }
            set
            {
                _Statusck = value; OnPropertyChanged(nameof(Statusck));
            }
        }
    }
}
