using StarMaint.Common;
using StarMaint.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace StarMaint.Model
{
    
    public class SlotStat : BaseVM
    {
        public STAT_TYPE _statType;
        public bool _isOnline = false;
    
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
    
        string _name;
        public string Name
        {
            get => _name;
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }
        string _bid;
        public string BID
        {
            get => _bid;
            set
            {
                _bid = value;
                OnPropertyChanged("BID");
            }
        }
    
    
        string _stat;
        public string Stat
        {
            get => _stat;
            set
            {
                _stat = value;
                OnPropertyChanged("Stat");
            }
        }
    
        SolidColorBrush _statBr;
        public SolidColorBrush Statbr
        {
            get => _statBr;
            set
            {
                _statBr = value;
                OnPropertyChanged("Statbr");
            }
        }
    }
    
}
