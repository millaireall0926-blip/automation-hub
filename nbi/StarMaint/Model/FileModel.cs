using StarMaint.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace StarMaint.Model
{
    public class FileModel : BaseVM
    {
        string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }

        BitmapImage _icon;
        public BitmapImage Icon
        {
            get => _icon;
            set
            {
                _icon = value;
                OnPropertyChanged("Icon");
            }
        }

        
        public int Type { get; set; } = 0;
        
    }


    public class FolderModel : BaseVM
    {
        string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }

        BitmapImage _icon;
        public BitmapImage Icon
        {
            get => _icon;
            set
            {
                _icon = value;
                OnPropertyChanged("Icon");
            }
        }
    }
}
