using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarOS.Model
{
    public enum LOG_TYPE : int
    {
        INFO = 0,
        WARNING,
        ERROR,
        REQUEST,
        REQUEST_COMPLATE
    }

    public class SystemLog : BaseVM
    {


        public SystemLog(LOG_TYPE type, string unit,  string msg)
        {
            Type = type;
            Uint = unit;
            Time = DateTime.Now.ToString();
            Message = msg;
        }

        LOG_TYPE _type;
        public LOG_TYPE Type
        {
            get { return _type; }
            set { _type = value; OnPropertyChanged("Type"); }
        }

        string _uint;
        public string Uint
        {
            get { return _uint; }
            set { _uint = value; OnPropertyChanged("Uint"); }
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

    public class SlotLog : BaseVM
    {
        public SlotLog(string step, string msg, LOG_TYPE type = LOG_TYPE.INFO)
        {
            Step = step;
            Time = DateTime.Now.ToString();
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
