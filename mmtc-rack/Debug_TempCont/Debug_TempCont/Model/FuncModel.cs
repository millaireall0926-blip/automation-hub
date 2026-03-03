using Debug_TempCont.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Debug_TempCont.Model
{
    public class FuncModel : BaseVM
    {
        private string _statustxt = "통신 불가";
        public string statustxt
        {
            get { return _statustxt; }
            set
            {
                if (_statustxt != null)
                    _statustxt = value;

                OnPropertyChanged(nameof(statustxt));
            }
        }
        private Brush _statusBackColor1 = Brushes.OrangeRed;
        public Brush statusBackgroundColor1
        {
            get { return _statusBackColor1; }
            set
            {
                if (_statusBackColor1 != null)
                    _statusBackColor1 = value;

                OnPropertyChanged(nameof(statusBackgroundColor1));
            }
        }
        private string _tempSetValue = "20.0";
        public string tempSetValue
        {
            get { return _tempSetValue; }
            set
            {
                if (_tempSetValue != null)
                    _tempSetValue = value;

                OnPropertyChanged(nameof(tempSetValue));
            }
        }
    }
}
