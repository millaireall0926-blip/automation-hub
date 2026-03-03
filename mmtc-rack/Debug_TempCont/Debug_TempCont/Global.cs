using Debug_TempCont.Common;
using Debug_TempCont.View;
using Debug_TempCont.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Debug_TempCont
{
    public class Global
    {
        public static Calculator gCalculator;
        public static SerialComm gSerialComm;

        public static TempChVM gTempChVM;
        public static GraphView gGraphView;
        public static MainWindow gMainWindow;

        public static bool setTempFlag = false;
        public static bool sendWaitFlag = false;

        public static string gPortName = null;
        public static string gProtocolName = null;
        public static string gM7Number = null;
    }
}
