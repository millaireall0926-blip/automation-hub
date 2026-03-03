using LP_MMTCRack.Model;
using LP_MMTCRack.Net;
using LP_MMTCRack.ViewModel;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Management;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Forms;

namespace LP_MMTCRack.Base
{
    public class MConv
    {
        private static readonly ushort[] CrcTable = {
            0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
            0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
            0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
            0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
            0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
            0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
            0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
            0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
            0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
            0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
            0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
            0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
            0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
            0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
            0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
            0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
            0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
            0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
            0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
            0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
            0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
            0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
            0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
            0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
            0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
            0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
            0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
            0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
            0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
            0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
            0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
            0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
        };
        public static char[] StructToChar(object obj)
        {
            int size = Marshal.SizeOf(obj);
            var returnArray = new char[size];

            var ptr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(obj, ptr, true);
            Marshal.Copy(ptr, returnArray, 0, size);

            Marshal.FreeHGlobal(ptr);

            return returnArray;
        }

        public static byte[] StructToByte(object obj)
        {
            int size = Marshal.SizeOf(obj);
            var returnArray = new byte[size];

            var ptr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(obj, ptr, true);
            Marshal.Copy(ptr, returnArray, 0, size);

            Marshal.FreeHGlobal(ptr);

            return returnArray;
        }
        public static T ByteToStruct<T>(int size, byte[] buffer)
        {
            var ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, 0, ptr, size);

            T obj = (T)Marshal.PtrToStructure(ptr, typeof(T));

            Marshal.FreeHGlobal(ptr);

            return obj;
        }

        public static T ByteToStruct<T>(int ofs, int size, byte[] buffer)
        {
            var ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, ofs, ptr, size);

            T obj = (T)Marshal.PtrToStructure(ptr, typeof(T));

            Marshal.FreeHGlobal(ptr);

            return obj;
        }

        public static byte[] StringToByte(string text)
        {
            byte[] strByte = Encoding.UTF8.GetBytes(text);
            return strByte;
        }

        public static string ByteToString(byte[] buffer)
        {
            string str = Encoding.UTF8.GetString(buffer);
            return str;
        }


        public static byte[] GetCombined(byte[] A, byte[] B)
        {
            var temp = new byte[A.Length + B.Length];

            Buffer.BlockCopy(A, 0, temp, 0, A.Length);
            Buffer.BlockCopy(B, 0, temp, A.Length, B.Length);

            return temp;
        }

        public static T SerialDeepCopy<T>(T obj)
        {
            using (var stream = new MemoryStream())
            {
                var formatter = new BinaryFormatter();
                formatter.Serialize(stream, obj);
                stream.Position = 0;

                return (T)formatter.Deserialize(stream);
            }
        }


        public static T ObjectDeepCopy<T>(T obj)
        {
            if (obj == null)
                throw new ArgumentNullException("Object cannot be null.");
            return (T)Processes(obj, new Dictionary<object, object>() { });
        }
        private static object Processes(object obj, Dictionary<object, object> circular)
        {
            if (obj == null)
                return null;
            Type type = obj.GetType();
            if (type.IsValueType || type == typeof(string))
            { return obj; }
            if (type.IsArray)
            {
                if (circular.ContainsKey(obj))
                    return circular[obj];
                string typeNoArray = type.FullName.Replace("[]", string.Empty);
                Type elementType = Type.GetType(typeNoArray + ", " + type.Assembly.FullName);
                var array = obj as Array;
                Array arrCopied = Array.CreateInstance(elementType, array.Length);
                circular[obj] = arrCopied;
                for (int i = 0; i < array.Length; i++)
                {
                    object element = array.GetValue(i);
                    object objCopy = null;
                    if (element != null && circular.ContainsKey(element))
                        objCopy = circular[element];
                    else
                        objCopy = Processes(element, circular);
                    arrCopied.SetValue(objCopy, i);
                }
                return Convert.ChangeType(arrCopied, obj.GetType());
            }
            if (type.IsClass)
            {
                if (circular.ContainsKey(obj))
                    return circular[obj];
                object objValue = Activator.CreateInstance(obj.GetType());
                circular[obj] = objValue;
                FieldInfo[] fields = type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
                foreach (FieldInfo field in fields)
                {
                    object fieldValue = field.GetValue(obj);
                    if (fieldValue == null)
                        continue;
                    object objCopy = circular.ContainsKey(fieldValue) ? circular[fieldValue] : Processes(fieldValue, circular);
                    field.SetValue(objValue, objCopy);
                }
                return objValue;

            }
            else throw new ArgumentException("Unknown type");
        }

        public static string convertTimeText(string text)
        {
            if (text.Length != 15) return text;

            string time;
            string tmp = text;
            time = tmp.Substring(0, 4);
            time += ("/" + tmp.Substring(4, 2));
            time += ("/" + tmp.Substring(6, 2));
            time += (" " + tmp.Substring(9, 2));
            time += (":" + tmp.Substring(11, 2));
            time += (":" + tmp.Substring(13, 2));

            return time;
        }

        public static long GetActualPosition(StreamReader reader)
        {
            System.Reflection.BindingFlags flags = System.Reflection.BindingFlags.DeclaredOnly | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.GetField;

            // The current buffer of decoded characters
            char[] charBuffer = (char[])reader.GetType().InvokeMember("charBuffer", flags, null, reader, null);

            // The index of the next char to be read from charBuffer
            int charPos = (int)reader.GetType().InvokeMember("charPos", flags, null, reader, null);

            // The number of decoded chars presently used in charBuffer
            int charLen = (int)reader.GetType().InvokeMember("charLen", flags, null, reader, null);

            // The current buffer of read bytes (byteBuffer.Length = 1024; this is critical).
            byte[] byteBuffer = (byte[])reader.GetType().InvokeMember("byteBuffer", flags, null, reader, null);

            // The number of bytes read while advancing reader.BaseStream.Position to (re)fill charBuffer
            int byteLen = (int)reader.GetType().InvokeMember("byteLen", flags, null, reader, null);

            // The number of bytes the remaining chars use in the original encoding.
            int numBytesLeft = reader.CurrentEncoding.GetByteCount(charBuffer, charPos, charLen - charPos);

            // For variable-byte encodings, deal with partial chars at the end of the buffer
            int numFragments = 0;
            if (byteLen > 0 && !reader.CurrentEncoding.IsSingleByte)
            {
                if (reader.CurrentEncoding.CodePage == 65001) // UTF-8
                {
                    byte byteCountMask = 0;
                    while ((byteBuffer[byteLen - numFragments - 1] >> 6) == 2) // if the byte is "10xx xxxx", it's a continuation-byte
                        byteCountMask |= (byte)(1 << ++numFragments); // count bytes & build the "complete char" mask
                    if ((byteBuffer[byteLen - numFragments - 1] >> 6) == 3) // if the byte is "11xx xxxx", it starts a multi-byte char.
                        byteCountMask |= (byte)(1 << ++numFragments); // count bytes & build the "complete char" mask
                                                                      // see if we found as many bytes as the leading-byte says to expect
                    if (numFragments > 1 && ((byteBuffer[byteLen - numFragments] >> 7 - numFragments) == byteCountMask))
                        numFragments = 0; // no partial-char in the byte-buffer to account for
                }
                else if (reader.CurrentEncoding.CodePage == 1200) // UTF-16LE
                {
                    if (byteBuffer[byteLen - 1] >= 0xd8) // high-surrogate
                        numFragments = 2; // account for the partial character
                }
                else if (reader.CurrentEncoding.CodePage == 1201) // UTF-16BE
                {
                    if (byteBuffer[byteLen - 2] >= 0xd8) // high-surrogate
                        numFragments = 2; // account for the partial character
                }
            }
            return reader.BaseStream.Position - numBytesLeft - numFragments;
        }

        public static System.Drawing.Color IntToColor(byte trans, int rgb)
        {

            return System.Drawing.Color.FromArgb(trans, (byte)(rgb >> 16), (byte)(rgb >> 8), (byte)rgb);

        }

        public static System.Windows.Media.Color IntToMediaColor(byte trans, int rgb)
        {
            return System.Windows.Media.Color.FromArgb(trans, (byte)(rgb >> 16), (byte)(rgb >> 8), (byte)rgb);
        }

        public static System.Drawing.Color toMediaColor(System.Windows.Media.Color color)
        {
            return System.Drawing.Color.FromArgb(color.A, color.R, color.G, color.B);
        }

        public static T FindEnumValue<T>(int index)
        {
            return (T)Enum.ToObject(typeof(T), index);
        }
        public static UInt16 ComputeCrc(SendMessage gmeg)
        {
            List<byte> packet = new List<byte>
            {
                gmeg.sData.ch, gmeg.sData.fun,
                gmeg.sData.sad_h, gmeg.sData.sad_l,
                gmeg.sData.len_h, gmeg.sData.len_l
            };
            ushort crc = 0xFFFF;

            foreach (byte datum in packet)
            {
                crc = (ushort)((crc >> 8) ^ CrcTable[(crc ^ datum) & 0xFF]);
            }

            return crc;
        }
        public static UInt16 CalculateCRC(Byte dchar, UInt16 crc16)
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
        public static UInt16 ComputeCrc(byte[] data)
        {
            ushort crc = 0xFFFF;
            foreach (byte datum in data)
            {
                crc = (ushort)((crc >> 8) ^ CrcTable[(crc ^ datum) & 0xFF]);
            }
            return crc;
        }
        public static string StructToString(byte[] gpk)
        {
            string SendMeg = "";

            foreach (byte a in gpk)
            {
                SendMeg += a.ToString("X").PadLeft(2, '0') + " ";
            }

            return SendMeg;
        }
        public static byte[] SVStringToByte(string s)
        {
            string a = s.Substring(0, 2);
            string b = s.Substring(2, 2);
            string c = " ";
            a = a + c + b;
            byte[] setsvval = new byte[2];
            int cnt = 0;

            foreach (string sb in a.Split(' '))
            {
                if (sb != null && sb != "")
                    setsvval[cnt] = Convert.ToByte(sb, 16);

                cnt++;
            }
            return setsvval;
        }
        public static int CheckSendLength(string cval)
        {
            int TempLength = 0;

            if (cval[0] == 'L')
            {
                TempLength = 5;
            }
            else if (cval[0] == 'H')
            {
                TempLength = 5;
            }
            else if (cval[0] == 'I')
            {
                ;
                TempLength = 3;
            }
            else if (cval[0] == 'P') // 'P' (0x50) -> 0x51 - Interlock OFF
            {
                TempLength = 3;
            }
            else if (cval[0] == 'R')// 0x52 - Interlock ON
            {
                TempLength = 3;
            }
            else if (cval[0] == 'E')
            {
                TempLength = 3;
            }
            else if (cval[0] == 'C')
            {
                TempLength = 3;
            }
            else if (cval[0] == 'F')
            {
                TempLength = 3;
            }

            return TempLength;
        }
        public static byte[] MakeSiteMeg(string cval, int Temprotattion, int len)
        {
            byte[] TempData = new byte[len];

            if (cval[0] == 'L')
            {
                TempData[0] = 0x4c;
                TempData[1] = (byte)((Temprotattion & 0xff00) >> 8);
                TempData[2] = (byte)(Temprotattion & 0xff);
                TempData[3] = (byte)((TempData[0] + TempData[1] + TempData[2]) & 0xff);
                TempData[4] = 0x0a;
            }
            else if (cval[0] == 'H')
            {
                TempData[0] = 0x48;
                TempData[1] = (byte)((Temprotattion & 0xff00) >> 8);
                TempData[2] = (byte)(Temprotattion & 0xff);
                TempData[3] = (byte)((TempData[0] + TempData[1] + TempData[2]) & 0xff);
                TempData[4] = 0x0a;
            }
            else if (cval[0] == 'I')
            {
                TempData[0] = 0x49;
                TempData[1] = 0x49;
                TempData[2] = 0x0a;
            }
            else if (cval[0] == 'P') // 'P' (0x50) -> 0x51 - Interlock OFF
            {
                TempData[0] = 0x31;
                TempData[1] = 0x31;
                TempData[2] = 0x0a;
            }
            else if (cval[0] == 'R')// 0x52 - Interlock ON
            {
                TempData[0] = 0x32;
                TempData[1] = 0x32;
                TempData[2] = 0x0a;
            }
            else if (cval[0] == 'E')
            {
                TempData[0] = 0x41;
                TempData[1] = 0x41;
                TempData[2] = 0x0a;
            }
            else if (cval[0] == 'C')
            {
                TempData[0] = 0x52;
                TempData[1] = 0x52;
                TempData[2] = 0x0a;
            }
            else if (cval[0] == 'F')
            {
                TempData[0] = 0x56;
                TempData[1] = (byte)(TempData[0] & 0xff);
                TempData[2] = 0x0a;
            }

            return TempData;
        }
        public static string makebyte(string tbx)
        {
            short _sh = Convert.ToInt16(tbx);
            string _st = _sh.ToString("X4");
            string a = _st.Substring(0, 2);
            string b = _st.Substring(2, 2);
            string c = " ";

            a = a + c + b;

            return a;
        }
        public static string makebyte_dob(string tbx)
        {
            double _dou = Convert.ToDouble(tbx) * 10;
            short _stdou = Convert.ToInt16(_dou);
            string shstr = _stdou.ToString("X4");
            string a = shstr.Substring(0, 2);
            string b = shstr.Substring(2, 2);
            string c = " ";

            a = a + c + b;

            return a;
        }
        public static DateTime Delay(int MS)
        {
            DateTime ThisMoment = DateTime.Now;
            TimeSpan duration = new TimeSpan(0, 0, 0, 0, MS);
            DateTime AfterWards = ThisMoment.Add(duration);

            while (AfterWards >= ThisMoment)
            {
                System.Windows.Forms.Application.DoEvents();
                ThisMoment = DateTime.Now;
            }

            return DateTime.Now;
        }

        public static (bool, string) GetComPortDetails(string comPortName, string HwProtname)
        {
            bool rtnval = false;
            string rtnPortNum = string.Empty;

            var mos = new ManagementObjectSearcher("select DeviceID, Name from Win32_PnPEntity");
            var mbos = new ArrayList(mos.Get());
            var data = new Dictionary<string, string[]>();

            try
            {
                for (var i = 0; i < mbos.Count; i++)
                {
                    var managementBaseObject = mbos[i] as ManagementBaseObject;
                    if (managementBaseObject == null) continue;

                    var deviceId = managementBaseObject.Properties["DeviceID"].Value as string;
                    var deviceName = managementBaseObject.Properties["Name"].Value as string;

                    if (deviceId == null || deviceName == null || !deviceId.StartsWith("USB")) continue;

                    if (!data.ContainsKey(deviceId))
                    {
                        data.Add(deviceId, new string[8]);
                    }

                    // COM 포트 번호 찾기
                    if (deviceName.Contains("COM"))
                    {
                        string[] parts = deviceName.Split('(');
                        if (parts.Length > 1)
                        {
                            string comPort = parts[1].Replace(")", "").Trim(); // "COMx" 추출
                            if (!string.IsNullOrEmpty(comPort))
                            {
                                data[deviceId][0] = comPort;
                                //Console.WriteLine($"Detected COM Port: {comPort}");
                            }
                        }
                    }

                    var mo = managementBaseObject as ManagementObject;
                    var inParams = mo.GetMethodParameters("GetDeviceProperties");

                    var result = mo.InvokeMethod("GetDeviceProperties", inParams, new InvokeMethodOptions());
                    if (result?.Properties["deviceProperties"].Value == null) continue;

                    foreach (var deviceProperties in result.Properties["deviceProperties"].Value as ManagementBaseObject[])
                    {
                        var keyName = deviceProperties.Properties["KeyName"].Value as string;
                        var value = deviceProperties.Properties["Data"].Value as string;

                        if (string.IsNullOrWhiteSpace(value) || string.IsNullOrWhiteSpace(keyName)) continue;

                        switch (keyName)
                        {
                            case "DEVPKEY_Device_BusReportedDeviceDesc":
                                if (comPortName == value)
                                {
                                    rtnval = true;
                                    rtnPortNum = data[deviceId][0]; // 해당 장치의 COM 포트 반환
                                    Console.WriteLine($"Device: {value}, Port: {rtnPortNum}");
                                }
                                break;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
            }

            return (rtnval, rtnPortNum);
        }

        public static string reScanUsbPort(string value)
        {
            string[] getAllPort = System.IO.Ports.SerialPort.GetPortNames();

            foreach (string portname in getAllPort)
            {

                string query = $"SELECT * FROM Win32_SerialPort WHERE DeviceID = '{portname}'";

                using (ManagementObjectSearcher searcher = new ManagementObjectSearcher(query))
                {
                    foreach (ManagementObject device in searcher.Get())
                    {
                        // 장치 설명 (Description)
                        //string description = device["Description"]?.ToString();

                        // 장치 인스턴스 경로 (DeviceID)
                        string deviceId = device["DeviceID"]?.ToString();

                        // 제조사 (Manufacturer)
                        //string manufacturer = device["Manufacturer"]?.ToString();

                        // PNP 장치 ID (PnPDeviceID)
                        string pnpDeviceId = device["PnPDeviceID"]?.ToString();

                        if(pnpDeviceId == value)
                        {

                            return deviceId;
                        }
                        // 상태 (Status)
                        //string status = device["Status"]?.ToString();

                        //Console.WriteLine($"Device Description: {description}");
                        Console.WriteLine($"Device Instance Path: {deviceId}");
                        //Console.WriteLine($"Manufacturer: {manufacturer}");
                        Console.WriteLine($"PnP Device ID: {pnpDeviceId}");
                        //Console.WriteLine($"Status: {status}");

                        //Console.WriteLine("-----------------------------------");
                    }
                }
            }
            return string.Empty;
        }

        public static (bool, string) CheckPortAlive(string value , string HWID)
        {
            string[] getAllPort = System.IO.Ports.SerialPort.GetPortNames();

            bool CheckPort = getAllPort.Contains(value);

            string getNewPort = string.Empty;
            bool rtnStaus = false;

            if (!CheckPort)
            {
                foreach (string portname in getAllPort)
                {

                    string query = $"SELECT * FROM Win32_SerialPort WHERE DeviceID = '{portname}'";

                    using (ManagementObjectSearcher searcher = new ManagementObjectSearcher(query))
                    {
                        foreach (ManagementObject device in searcher.Get())
                        {
                            // 장치 인스턴스 경로 (DeviceID)
                            string deviceId = device["DeviceID"]?.ToString();

                            // PNP 장치 ID (PnPDeviceID)
                            string pnpDeviceId = device["PnPDeviceID"]?.ToString();

                            if (pnpDeviceId == HWID)
                            {
                                getNewPort = deviceId;
                                rtnStaus = true;
                            }
                            Console.WriteLine($"Device Instance Path: {deviceId}");
                            Console.WriteLine($"PnP Device ID: {pnpDeviceId}");
                        }
                    }
                }
            }
            else
            {
                rtnStaus = true;
                getNewPort = value;
            }
            return (rtnStaus, getNewPort);
        }

        // DLL 파일 이름 정의
        public const string DLL_NAME = "CP210xManufacturing.dll";
        // 함수 선언
        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_GetNumDevices(out uint numDevices);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_GetProductString(uint deviceNum, IntPtr deviceString, uint flags);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_Open(uint device, out IntPtr handle);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_Close(IntPtr handle);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_GetPartNumber(IntPtr handle, out byte partNumber);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_GetDeviceSerialNumber(IntPtr deviceHandle, StringBuilder serialNumber, ref byte length, bool convertToASCII);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_GetDeviceProductString(IntPtr deviceHandle, StringBuilder productString, ref byte length, bool convertToASCII);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_SetSerialNumber(IntPtr deviceHandle, IntPtr productString, byte length, bool convertToUnicode);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_SetProductString(IntPtr deviceHandle, IntPtr productString, byte length, bool convertToUnicode);

        [DllImport(DLL_NAME, CallingConvention = CallingConvention.StdCall)]
        public static extern int CP210x_Reset(IntPtr handle); // Reset 함수 추가

        // Error codes
        public static int CP210x_SUCCESS = 0x00;
        public static int CP210x_DEVICE_NOT_FOUND = 0xFF;
        public static int CP210x_INVALID_HANDLE = 0x01;

        public static string GetDeviceInstanceId(string getPortName)
        {
            string deviceId = null;
            var searcher = new ManagementObjectSearcher("SELECT * FROM Win32_SerialPort WHERE DeviceID='" + getPortName + "'");

            foreach (ManagementObject port in searcher.Get())
            {
                deviceId = port["PNPDeviceID"].ToString();
                Console.WriteLine($"Port: {getPortName}, Instance Path: {deviceId}");
            }

            Console.WriteLine(deviceId);
            return deviceId;
            //SetComPortState(deviceId , enable);
        }

        public static void SetComPortState(string deviceId, bool enable)
        {
            string pnputilPath = @"C:\Windows\System32\pnputil.exe";


            // pnpunti.exe가 있는지 확인
            if (!File.Exists(pnputilPath))
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"pnpunti.exe 파일을 찾을 수 없습니다.\n경로: {pnputilPath}");
                return;
            }

            // 사용하려는 명령어 (예: "disable" 또는 "enable")
            string command = enable ? "/enable-device " : "/disable-device ";
            // 인수 생성 (장치 식별자를 따옴표로 감쌈)
            string arguments = $"{command} \"{deviceId}\"";

            // ProcessStartInfo 설정
            ProcessStartInfo psi = new ProcessStartInfo
            {
                FileName = pnputilPath,
                Arguments = arguments,
                Verb = "runas",         // 관리자 권한으로 실행
                UseShellExecute = true, // 셸 실행 사용
                CreateNoWindow = true,  // 새 콘솔 창 없이 실행
                WindowStyle = ProcessWindowStyle.Hidden  // 창 스타일을 숨김으로 설정
            };

            try
            {
                // 실행 전 로그 출력 (디버깅용)
                Console.WriteLine($"Executing: {pnputilPath} {arguments}");

                using (Process process = Process.Start(psi))
                {
                    process.WaitForExit();
                    // 종료 코드 0이면 정상 종료
                    if (process.ExitCode == 0)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"명령어 실행 완료: {arguments}");
                    }
                    else
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"명령어 실행 후 오류 발생");
                    }
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"pnpunti 실행 중 예외 발생: {ex.Message}");
            }
        }
	}
}
