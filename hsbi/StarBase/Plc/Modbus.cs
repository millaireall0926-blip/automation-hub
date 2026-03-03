using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using SuperSocket.ClientEngine;
using System.Runtime.InteropServices;
using System.Windows.Threading;
using System.Threading;
using System.Collections.ObjectModel;

namespace StarBase.Plc {

    public enum LAMP_TYPE : int {
        R = 0x0001,        // red color
        Y = 0x0002,        // yellow color
        G = 0x0004,        // green color
        RB = 0x0010,
        YB = 0x0020,
        GB = 0x0040,
        BZ = 0x0100,       // buzzer
        ALL = 0x1000,
        NONE = 0
    }

    public enum PLC_STAT_TYPE : int {
        DISCON = 0,
        RUN = 1,
        AMBI = 2,
        STOP = 4

    }

    public enum ALARM_TYPE : int {
        ALARM_IGNORE = 0,
        ALARM_WARNING = 1,
        ALARM_ERROR = 2,
    };

    public struct CHAM_ALARAM {
        public CHAM_ALARAM(ALARM_TYPE type, string text, bool forceStop = false)
        {
            Type = type;
            Text = text;
            isForceStop = forceStop;

        }
        public bool isForceStop;
        public ALARM_TYPE Type;    // 0 = No Err, 1 = Warnning, 2 = Error
        public string Text;
    }


    public class Modbus : AsyncTcpSession {
        #region UI Communication
        public Collection<int> _plcAlram = new Collection<int>();
        public double CurTemp { get; set; }

        // 중복 명령 방지용
        public double SetTargetTemp { get; set; } = 0;
        public double TargetTemp { get; set; }

        // 중복 명령 방지용
        public PLC_STAT_TYPE SetStat { get; set; } = PLC_STAT_TYPE.STOP;  
        public PLC_STAT_TYPE Stat { get; set; }
        public int Remote { get; set; }
        public int Rack { get; set; }
        public int Door { get; set; }
        public int LampStat { get; set; }
        public int LampOwn { get; set; }
        public int LampCtrl { get; set; }

        #endregion

        #region PLC Communication
        public bool _connect = false;
        public string _plcIP;
        public int _port;

        public string _recvData = "";
        public object _lock = new object();

       
        public bool _readDone = false;
        public bool _retOK = true;

        #region delegate Function

        public delegate void readPLCDelegate();
        public readPLCDelegate readPLCFunc;

        public delegate string getDataDelegate(int index, int size = 4);
        public getDataDelegate getDataFunc;

        public delegate bool setOperationDelegate(PLC_STAT_TYPE mode);
        public setOperationDelegate setOperationFunc;

        public delegate bool setRemoteDelegate();
        public setRemoteDelegate setRemoteFunc;

        public delegate bool setTempDelegate(double temp);
        public setTempDelegate setTempFunc;

        public delegate bool setResetDelegate();
        public setResetDelegate setResetFunc;

        public delegate bool enableWatchDocDelegate(bool isEnable);
        public enableWatchDocDelegate enableWatchDocFunc;

        public delegate bool toggleWatchDogDelegate();
        public toggleWatchDogDelegate toggleWatchDogFunc;

        public delegate bool setWatchDogTimeoutDelegate(int sec);
        public setWatchDogTimeoutDelegate setWatchDogTimeoutFunc;

        public delegate bool retBool_setInt_Delegate(int ctrl);
        public retBool_setInt_Delegate setDoorFunc;
        public retBool_setInt_Delegate setRackFunc;
        public retBool_setInt_Delegate setLampOwnFunc;
        public retBool_setInt_Delegate setBuzzerFunc;

        public delegate bool setLampDele(bool on, LAMP_TYPE type, bool blink);
        public setLampDele setLampFunc;
          

        #endregion

        public Modbus()
        {
            _connect = false;
            Connected += done_Connected;            
            Error += done_Error;
            Closed += done_Closed;
            ReceiveBufferSize = 1024;
        }



        public void doConnect()
        {
            try
            {
                if (_plcIP == "") return;
                Connect(new IPEndPoint(IPAddress.Parse(_plcIP), _port));
            }
            catch 
            {

            }
        }


        public bool waitRecv()
        {
            int waitCnt = 0;    
            while (!_readDone)
            {
                Thread.Sleep(1);
                waitCnt++;
                if (waitCnt >= 500) // wait 5S
                    break;
  //              Console.WriteLine($"wait : {waitCnt}");
            }   

            return _readDone;
        }


        public void done_Connected(object sender, EventArgs e)
        {
            _connect = true;
            Console.WriteLine("Connected\n");
        }

        public void done_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {
            Close();
        }
        public void done_Closed(object sender, EventArgs e)
        {
            _connect = false;
        }




        #endregion PLC Communication
    }
}
