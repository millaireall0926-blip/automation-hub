using RDimm_MMTCRACK.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RDimm_MMTCRACK.Model
{
    public class EngineerModel : BaseVM
    {
        public string Symbol { get; set; } = string.Empty;
        public string pCh1 { get; set; } = string.Empty;
        public string pCh2 { get; set; } = string.Empty;
        public string pCh3 { get; set; } = string.Empty;
        public string pCh4 { get; set; } = string.Empty;
        public string pCh5 { get; set; } = string.Empty;
        public string pCh6 { get; set; } = string.Empty;
        public string pCh7 { get; set; } = string.Empty;
        public string pCh8 { get; set; } = string.Empty;
        public string pCh9 { get; set; } = string.Empty;
        public string pCh10 { get; set; } = string.Empty;
        public string pCh11 { get; set; } = string.Empty;
        public string pCh12 { get; set; } = string.Empty;
        public string pCh13 { get; set; } = string.Empty;
        public string pCh14 { get; set; } = string.Empty;
        public string pCh15 { get; set; } = string.Empty;
        public string pCh16 { get; set; } = string.Empty;
        public string pCh17 { get; set; } = string.Empty;
        public string pCh18 { get; set; } = string.Empty;

        public EngineerModel(string symbol, string pCh1, string pCh2, string pCh3, string pCh4, string pCh5, string pCh6, string pCh7, string pCh8,  
            string pCh9, string pCh10, string pCh11, string pCh12, string pCh13, string pCh14, string pCh15, string pCh16, string pCh17, string pCh18)
        {
            Symbol = symbol;
            this.pCh1 = pCh1;
            this.pCh2 = pCh2;
            this.pCh3 = pCh3;
            this.pCh4 = pCh4;
            this.pCh5 = pCh5;
            this.pCh6 = pCh6;
            this.pCh7 = pCh7;
            this.pCh8 = pCh8;
            this.pCh9 = pCh9;
            this.pCh10 = pCh10;
            this.pCh11 = pCh11;
            this.pCh12 = pCh12;
            this.pCh13 = pCh13;
            this.pCh14 = pCh14;
            this.pCh15 = pCh15;
            this.pCh16 = pCh16;
            this.pCh17 = pCh17;
            this.pCh18 = pCh18;
        }
        public void ValueSet(string pCh1, string pCh2, string pCh3, string pCh4, string pCh5, string pCh6, string pCh7, string pCh8,
            string pCh9, string pCh10, string pCh11, string pCh12, string pCh13, string pCh14, string pCh15, string pCh16, string pCh17, string pCh18)
        {
            this.pCh1 = pCh1;
            this.pCh2 = pCh2;
            this.pCh3 = pCh3;
            this.pCh4 = pCh4;
            this.pCh5 = pCh5;
            this.pCh6 = pCh6;
            this.pCh7 = pCh7;
            this.pCh8 = pCh8;
            this.pCh9 = pCh9;
            this.pCh10 = pCh10;
            this.pCh11 = pCh11;
            this.pCh12 = pCh12;
            this.pCh13 = pCh13;
            this.pCh14 = pCh14;
            this.pCh15 = pCh15;
            this.pCh16 = pCh16;
            this.pCh17 = pCh17;
            this.pCh18 = pCh18;
        }
        public void ConvertVal(int ch, string val)
        {
            switch (ch)
            {
                case 0: pCh1 = val; break;
                case 1: pCh2 = val; break;
                case 2: pCh3 = val; break;
                case 3: pCh4 = val; break;
                case 4: pCh5 = val; break;
                case 5: pCh6 = val; break;
                case 6: pCh7 = val; break;
                case 7: pCh8 = val; break;
                case 8: pCh9 = val; break;
                case 9: pCh10 = val; break;
                case 10: pCh11 = val; break;
                case 11: pCh12 = val; break;
                case 12: pCh13 = val; break;
                case 13: pCh14 = val; break;
                case 14: pCh15 = val; break;
                case 15: pCh16 = val; break;
                case 16: pCh17 = val; break;
                case 17: pCh18 = val; break;
            }
        }
        public void ValueClear()
        {
            pCh1 = "0";
            pCh2 = "0";
            pCh3 = "0";
            pCh4 = "0";
            pCh5 = "0";
            pCh6 = "0";
            pCh7 = "0";
            pCh8 = "0";
            pCh9 = "0";
            pCh10 = "0";
            pCh11 = "0";
            pCh12 = "0";
            pCh13 = "0";
            pCh14 = "0";
            pCh15 = "0";
            pCh16 = "0";
            pCh17 = "0";
            pCh18 = "0";
        }
        public string[] sendVal()
        {
            string[] sendStr = new string[19];
            sendStr[0] = pCh1;
            sendStr[1] = pCh2;
            sendStr[2] = pCh3;
            sendStr[3] = pCh4;
            sendStr[4] = pCh5;
            sendStr[5] = pCh6;
            sendStr[6] = pCh7;
            sendStr[7] = pCh8;
            sendStr[8] = pCh9;
            sendStr[9] = pCh10;
            sendStr[10] = pCh11;
            sendStr[11] = pCh12;
            sendStr[12] = pCh13;
            sendStr[13] = pCh14;
            sendStr[14] = pCh15;
            sendStr[15] = pCh16;
            sendStr[16] = pCh17;
            sendStr[17] = pCh18;
            return sendStr;
        }
    }
}
