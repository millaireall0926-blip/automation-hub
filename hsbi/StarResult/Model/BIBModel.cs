using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace StarResult.Model
{

    public enum SUB_TYPE : int
    {
        NONE = 0,
        FBLOCK = 1,
        BLOCK_FBIT_COUNT = 2,
        BLOCK_FBIT_COUNT_DETAIL = 3,        
        DATA = 4,
        CYCLE
    }

    public class DutModel : BaseVM
    {
        public Dictionary<string, bool> ContactDic = new Dictionary<string, bool>();
        public Dictionary<string, bool> CurrentDic = new Dictionary<string, bool>();
        public Dictionary<string, bool> TotalDic = new Dictionary<string, bool>();

        public Dictionary<string, int> _fBit = new Dictionary<string, int>();

        int _num;
        public int Num
        {
            get => _num;
            set
            { _num = value; OnPropertyChanged(nameof(Num)); }
        }

        double _left = 10;
        public double Left
        {
            get => _left;
            set
            { _left = value; OnPropertyChanged(nameof(Left)); }

        }

        double _top = 20;
        public double Top
        {
            get => _top;
            set
            { _top = value; OnPropertyChanged(nameof(Top)); }
        }

        public double _width = 100;
        public double Width
        {
            get => _width;
            set
            { _width = value; OnPropertyChanged(nameof(Width)); }
        }

        public double _height = 100;
        public double Height
        {
            get => _height;
            set
            { _height = value; OnPropertyChanged(nameof(Height)); }
        }

        SolidColorBrush _color = new SolidColorBrush(Colors.LightGreen);
        public SolidColorBrush Color
        {
            get => _color;
            set
            { _color = value; OnPropertyChanged(nameof(Color)); }
        }

        string _text;
        public string Text
        {
            get => _text;
            set { _text = value; OnPropertyChanged(nameof(Text)); }
        }

        public bool _hide;
        public bool Hide
        {
            get => _hide;
            set { _hide = value; OnPropertyChanged(nameof(Hide)); }
        }

        public string FBit { get; set; } = "";
        public string IO { get; set; } = "";
        public string Bin { get; set; } = "";

        public string ChipID { get; set; } = "";

    }

    public class StepModel : BaseVM
    {
        bool _isContact = false;
        public bool IsContact
        {
            get => _isContact;
            set { _isContact = value; OnPropertyChanged(nameof(IsContact)); }
        }

        public string _name;
        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }

        public string _start;
        public string Start
        {
            get => _start;
            set { _start = value; OnPropertyChanged(nameof(Start)); }
        }

        public string _end;
        public string End
        {
            get => _end;
            set { _end = value; OnPropertyChanged(nameof(End)); }
        }

        public string _yeild;
        public string Yeild
        {
            get { return _yeild; }
            set { _yeild = value; OnPropertyChanged(nameof(Yeild)); }
        }

        public string _subFile = "";

        static public SUB_TYPE findSubType(string type)
        {
            SUB_TYPE subType = SUB_TYPE.NONE;
            if (type == "blk")
                subType = SUB_TYPE.FBLOCK;
            else if (type == "fc")
                subType = SUB_TYPE.BLOCK_FBIT_COUNT;
            else if (type == "fc-d")
                subType = SUB_TYPE.BLOCK_FBIT_COUNT_DETAIL;
            else if (type == "data")
                subType = SUB_TYPE.DATA;
            else if (type == "cycle")
                subType = SUB_TYPE.CYCLE;
            return subType;
        }

        public SUB_TYPE _subType = SUB_TYPE.NONE;
    }

    public class BIBModel : BaseVM
    {
        public int _xSize;
        public int _ySize;
        public int _ioCount;
        public int _MultiChip;

        public int _dutCnt;

        public double _width;
        public double _height;

        public string _date;
        public string _pgm;
        public string _bid;

        public BIBModel()
        {
            _xSize = 0;
            _ySize = 0;

            _MultiChip = 1;
        }

    }

    public class SubBLockModel : BaseVM
    {
        public int _type = 0; // 0 = existing, 1 = new, 2 = mask, 3 = new mask
        public int Type
        {
            get => _type;
            set { _type = value; OnPropertyChanged(nameof(Type)); }
        }

        public string _block;
        public string Block
        {
            get => _block;
            set { _block = value; OnPropertyChanged(nameof(Block)); }
        }
    }

    public class SubFDetailModel : BaseVM
    {
        int _no;
        public int No
        {
            get => _no;
            set { _no = value; OnPropertyChanged(nameof(No)); }
        }

        public string _text;
        public string Text
        {
            get => _text;
            set { _text = value; OnPropertyChanged(nameof(Text)); }
        }
    }


}
