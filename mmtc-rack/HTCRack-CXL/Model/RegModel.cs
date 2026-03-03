using HTCRack_CXL.Base;
using HTCRack_CXL.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace HTCRack_CXL.Model
{
    #region Net.HtcProtocal의 PROTOCAL과 동일 해야함
    public enum REG_TYPE : int
    {
        FAN = 0,
        CUR,
        TEMP,
        FP,
        DIMM,
        TCON,
        GPIO,
        PDO = 10
    }
    #endregion

    public enum REG_SIZE : int
    {
        FAN = 1,
        CUR = 2,
        TEMP = 2,
        FP = 4,
        DIMM = 4,
        TCON = 4,
        GPIO = 1,
        PDO = -1
    }
    public class RegModel : BaseVM
    {
        public REG_TYPE Type { get; set; }
        public string CH { get; set; }
        public string ADR { get; set; }
        public string FUNC { get; set; }
        public string Discription { get; set; }
        public string Kind { get; set; }
        string _read = "0";
        public string Read
        {
            get => _read;
            set
            {
                if (_read == value) return;
                _read = value;
                if (funcReadVlaue != null)
                    ReadValue = funcReadVlaue(_read);
            }
        }
        public string Write { get; set; } = "0"; // Only Use Engineer Mode 
        public double ReadValue { get; set; } = 0;

        double _writeValue = 0;
        public double WriteValue
        {
            get => _writeValue;
            set
            {
                if (_writeValue == value) return;
                _writeValue = value;
                if (funcWriteToHex != null)
                    Write = funcWriteToHex(_writeValue);

            }
        } // Only Use Engineer Mode 
        public bool SendEnable { get; set; }
        public bool Single { get; set; }
        public bool ReadEnable { get; set; }
        public String Mode { get; set; }
        public int Len { get; set; }

        public delegate double DLGRead(string read);
        public DLGRead funcReadVlaue = null;
        public delegate string DLGWrite(double value);
        public DLGWrite funcWriteToHex = null;



        public Visibility VisibleValue { get; set; } = Visibility.Visible;
        public Visibility VisibleHex { get; set; } = Visibility.Hidden;
        public RegModel(REG_TYPE type = REG_TYPE.FAN,
                        string ch = "ff", string adr = "0",
                        string func = "", string kind = "0x", string disc = "",
                        bool send = false, bool single = true, bool noRead = false,
                        int len = -1)
        {
            Type = type; CH = ch; ADR = adr; FUNC = func;
            Discription = disc; Kind = kind;
            SendEnable = send; ReadEnable = !noRead;
            Single = single;
            if (single) Mode = "Single";
            else Mode = "Multi";

            if (Kind == "0x")
            {
                VisibleValue = Visibility.Hidden;
                VisibleHex = Visibility.Visible;
            }

            if (len == -1)
            {
                switch (Type)
                {
                    case REG_TYPE.FAN:
                        Len = 1; break;
                    case REG_TYPE.CUR:
                        Len = 2; break;
                    case REG_TYPE.TEMP:
                        Len = 2; break;
                    case REG_TYPE.FP:
                        Len = 4; break;
                    case REG_TYPE.DIMM:
                        Len = 4; break;
                    case REG_TYPE.TCON:
                        Len = 2; break;
                    case REG_TYPE.GPIO:
                        Len = 1; break;
                    case REG_TYPE.PDO:
                        Len = 2; break;

                }
            }
            else
                Len = len;
        }
        public RegModel() { }
    }
}
