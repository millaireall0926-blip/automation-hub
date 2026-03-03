using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;


namespace HTCRack.Base {
    public class MConv
    {
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

        public static T ByteToStruct<T>(int ofs, int size, byte[] buffer) {
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
            return (T)Process(obj, new Dictionary<object, object>() { });
        }
        private static object Process(object obj, Dictionary<object, object> circular)
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
                        objCopy = Process(element, circular);
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
                    object objCopy = circular.ContainsKey(fieldValue) ? circular[fieldValue] : Process(fieldValue, circular);
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
    }
}