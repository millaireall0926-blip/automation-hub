using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarIDE.Model
{
    public enum REPORT_TYPE : int
    {
        INFO = 0,
        OK,
        ERR, 
        WARNING
    }
    

    public class ReportModel : BaseVM
    {

        REPORT_TYPE _type;
        public REPORT_TYPE Type
        {
            get { return _type; }
            set { _type = value; OnPropertyChanged(nameof(Type)); }
        }
            
        string _text;
        public string Text
        {
            get { return _text; }
            set { _text = value; OnPropertyChanged(nameof(Text)); }
        }

        int _liine;
        public int Line
        { 
            get { return _liine; }
            set { _liine = value; OnPropertyChanged(nameof(Line));}
        }      

        public ReportModel(REPORT_TYPE type, string text, int line = -1)
        {
            _type = type;
            Text = text;
            Line = line;

        }

        public FileModel _file = null;  // FileName

    }
}
