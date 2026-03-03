using RedfishTest.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RedfishTest.Model
{
    public class DimmTemp : BaseVM
    {
        public DimmTemp()
        {
            // Constructor
        }

        string _path;
        public string Path
        {
            get { return _path; }
            set { _path = value; OnPropertyChanged(nameof(Path)); }
        }

        double _temp;
        public double Temp
        {
            get { return _temp; }
            set { _temp = value; OnPropertyChanged(nameof(Temp)); }
        }
    }
}
