using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using StarBase;

namespace StarResult.Model
{
    public enum FILTER : int
    {
        NONE = 0,
        LOT = 1,
        TIME = 2
    };

    public class SlotModel : BaseVM
    {
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

    public class DateFile
    {
        public DateFile()
        {
            files = new Collection<string>();
        }

        public DateTime dateTime;
        public Collection<string> files;
    }

    public enum LOG_TYPE : int
    {
        INFO = 0,
        WARNING,
        ERROR
    }

    public class SlotLog : BaseVM
    {
        public SlotLog(string time, string step, string msg, LOG_TYPE type = LOG_TYPE.INFO)
        {
            Step = step;
            Time = time;
            Message = msg;
            Type = type;
        }

        string _step;
        public string Step
        {
            get { return _step; }
            set { _step = value; OnPropertyChanged("Step"); }
        }

        LOG_TYPE _type;
        public LOG_TYPE Type
        {
            get { return _type; }
            set { _type = value; OnPropertyChanged("Type"); }
        }



        string _time;
        public string Time
        {
            get { return _time; }
            set { _time = value; OnPropertyChanged("Time"); }
        }

        string _message;
        public string Message
        {
            get { return _message; }
            set { _message = value; OnPropertyChanged("Message"); }
        }

    }
}
