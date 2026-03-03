using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace StarMaint.Common
{
    public class MConv
    {
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

        public static T DeepCopy<T>(T obj)
        {
            using (var stream = new MemoryStream())
            {
                var formatter = new BinaryFormatter();
                formatter.Serialize(stream, obj);
                stream.Position = 0;

                return (T)formatter.Deserialize(stream);
            }
        }
    }
}
