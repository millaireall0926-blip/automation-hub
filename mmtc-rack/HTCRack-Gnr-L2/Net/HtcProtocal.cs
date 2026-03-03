using System;
using System.Runtime.InteropServices;

namespace HTCRack.Net
{
    enum PROTOCAL : int
    {
        TYPE_FAN = 0,   // Fan Rate
        TYPE_CUR,       // INA Current
        TYPE_TEMP,      // FET Switch Temp
        TYPE_FP,        // Front Pannel
        TYPE_DIMM,      // Dimm Temp
        TYPE_TCON,      // Temp Controller
        TYPE_GPIO,      // GPIO
        TYPE_PDO,        // Pre Define. Ver, All Status

        SUB_SIZE = 5,
        HEAD_SIZE = 6,

        MAGIC_VAL = 0xc0de
    }

    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class HtcHeader
    {
        public short Magic;
        public short Len;
        public byte Type;
        public byte Res;
    }

    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class HtcSub
    {
        public byte Chanel;
        public byte Adr;
        public byte Func;
        public short Len;

    }


}
