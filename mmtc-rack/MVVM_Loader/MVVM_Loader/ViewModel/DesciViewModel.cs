using MVVM_Loader.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM_Loader.ViewModel
{
    public class DesciViewModel : BaseVM
    {
        private string _Description = "";
        public string Description
        {
            get => _Description;
            set
            {
                _Description = value; OnPropertyChanged(nameof(Description));
            }
        }
        public DesciViewModel()
        {
            //Global.gDesciViewModel = this;
        }
    }
}
