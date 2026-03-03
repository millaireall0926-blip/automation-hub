using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace Debug_TempCont.Common
{
    public class Calculator
    {
        private int checksum_int = 0;

        public Calculator()
        {
            Global.gCalculator = this;
        }

        public void checksumData(List<int> tarData, ArrayList alReturn)
        {
            checksum_int = 0;

            foreach (int a in tarData)
            {
                checksum_int += a;
                alReturn.Add((byte)a);
            }
            string sub_checksum = checksum_int.ToString("X4").Substring(2);
            List<int> int_checksum = convert_ascii(sub_checksum);

            foreach (int b in int_checksum)
                alReturn.Add((byte)b);
        }
        public bool checksumData(string input, string checkSum)
        {
            byte[] bytes = Encoding.ASCII.GetBytes(input);
            byte result = 0;
            foreach (byte c in bytes)
                result += c;
            result &= 0xff;

            if (result.ToString("X2") == checkSum)
                return true;
            else
                return false;
        }
        public void addchecksumData(List<int> tarData, ArrayList alReturn)
        {
            checksum_int = 0;

            foreach (int a in tarData)
            {
                checksum_int += a;
                alReturn.Add((byte)a);
            }
            string sub_checksum = checksum_int.ToString("X4").Substring(2);
            List<int> int_checksum = convert_ascii(sub_checksum);

            foreach (int b in int_checksum)
                alReturn.Add((byte)b);
        }
        public double tranfer_HextoDouble(string input)
        {
            short st_output = Convert.ToInt16(input, 16);
            double doub_output = st_output / 10.0;

            return doub_output;
        }
        public string tranfer_StringtoHexString(string input)
        {
            int strToint = 0;
            try
            {
                strToint = Convert.ToInt16(input);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
            string Tempstr = strToint.ToString("X4");
            return Tempstr;
        }
        public List<int> convert_ascii(string strtarData)
        {
            List<int> ascii = strtarData.ToCharArray().ToList().ConvertAll(c => Convert.ToInt32(c));
            return ascii;
        }

        public string tranfer_StringtoDoubletoHex(string getvalue)
        {
            string outData;

            outData = Convert.ToInt16( Convert.ToDouble(getvalue) *10).ToString("X4");
            //outData = Convert.ToInt16(Global.writeTempValue * 10).ToString("X4");

            return outData;
        }
        public byte get_SVaddress(string getaddr)
        {
            byte outbyte = 0x00;
            
            if(getaddr == "1")
                outbyte = 0x52;
            else if(getaddr == "2") 
                outbyte = 0x53;
            else if(getaddr== "3")
                outbyte = 0x54;
            else if(getaddr == "4")
                outbyte = 0x55;

            return outbyte;
        }
        public byte get_RUNSTOPaddress(string getaddr)
        {
            byte outbyte = 0x00;

            if (getaddr == "1")
                outbyte = 0x3B;
            else if (getaddr == "2")
                outbyte = 0x3C;
            else if (getaddr == "3")
                outbyte = 0x3D;
            else if (getaddr == "4")
                outbyte = 0x3F;

            return outbyte;
        }
        public string tranfer_DoubletoString(double douData)
        {
            string outData = "";

            outData = Convert.ToInt16(douData * 10).ToString("X4");

            return outData;
        }
        public string tranfer_DoubletointString(double douData)
        {
            string outData = "";

            outData = Convert.ToInt16(douData * 10).ToString();

            return outData;
        }
        public UInt16 CalculateCRC(Byte dchar, UInt16 crc16)
        {
            UInt16 mask = (UInt16)(dchar & 0x00FF);
            crc16 = (UInt16)(crc16 ^ mask);
            for (int i = 0; i < 8; i++)
            {
                if ((UInt16)(crc16 & 0x0001) == 1)
                {
                    mask = (UInt16)(crc16 / 2);
                    crc16 = (UInt16)(mask ^ 0xA001);
                }
                else
                {
                    mask = (UInt16)(crc16 / 2);
                    crc16 = mask;
                }
            }
            return crc16;
        }
        public char[] converthextobin(string input)
        {
            int varCnt = 0;
            char[] ret = new char[8];
            ret = Enumerable.Repeat('0', 8).ToArray();

            int h_dec = Convert.ToInt32(input, 16);
            string strBin10_2 = Convert.ToString(h_dec, 2);
            string RevstrBin10_2 = new string(strBin10_2.Reverse().ToArray());

            foreach (var c in RevstrBin10_2)
            {
                ret[varCnt] = c;
                varCnt++;
            }

            return ret;
        }
        public int checkuiindex(int intdata)
        {
            int checkdata = 0;

            if ((intdata >= 0) && (intdata < 4)) //0,1,2,3
                checkdata = 0;
            else if ((intdata >= 4) && (intdata < 8))//4,5,6,7
                checkdata = 1;
            else if ((intdata >= 8) && (intdata < 12))//8,9,10,11
                checkdata = 2;
            else if ((intdata >= 12) && (intdata < 16))//12,13,14,15
                checkdata = 3;
            else if ((intdata >= 16) && (intdata < 20))//16,17,18,19
                checkdata = 4;
            else if ((intdata >= 20) && (intdata < 24))//20,21,22,23
                checkdata = 5;
            else if ((intdata >= 24) && (intdata < 28))//24,25,26,27
                checkdata = 6;
            else if ((intdata >= 28) && (intdata < 32))//28,29,30,31
                checkdata = 7;

            return checkdata;
        }
    }
}
