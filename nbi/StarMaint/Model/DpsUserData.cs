using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarMaint.Common;

namespace StarMaint.Model
{
    
    public class DpsUserData : BaseVM
    {
        public DpsUserData(string name) 
        { 
            Name = name;
            _userData1 = "-";
            _userData2 = "-";
            _userData3 = "-";            
        }

        string _name;
        public string Name { 
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged(nameof(Name));
            } 
        }

        string _userData1;
        public string UserData1 { 
            get => _userData1; 
            set
            {
                _userData1 = value;
                OnPropertyChanged(nameof(UserData1));
            }
        }

        string _userData2;
        public string UserData2
        {
            get => _userData2;
            set
            {
                _userData2 = value;
                OnPropertyChanged(nameof(UserData2));
            }
        }

        string _userData3;
        public string UserData3
        {
            get => _userData3;
            set
            {
                _userData3 = value;
                OnPropertyChanged(nameof(UserData3));
            }
        }

        bool _isLock = true;
        public bool IsLock
        {
            get => _isLock;
            set
            {
                _isLock = value;
                OnPropertyChanged(nameof(IsLock));
            }
        }
    }

}
