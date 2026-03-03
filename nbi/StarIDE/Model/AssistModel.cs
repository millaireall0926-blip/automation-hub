using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarIDE.Model
{
    public class AssistModel : BaseVM
    {

    }

    public class APIModel : BaseVM
    {
        string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }
        string _brief;
        public string Brief
        {
            get { return _brief; }
            set { _brief = value; OnPropertyChanged(nameof(Brief)); }
        }
    }

    public class REGModel : BaseVM
    {
        string _name;
        public string Name
        {
            get { return _name; }
            set { _name= value; OnPropertyChanged(nameof(Name));}
        }
      
        
    }
}
