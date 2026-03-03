using Debug_TempCont.Common;
using Debug_TempCont.ViewModel;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Debug_TempCont.View
{
    /// <summary>
    /// ConnetView.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 
    public partial class ConnetView : Page
    {
        SerialComm sc;
        ConnetVM cvm = null;
        public ConnetView()
        {
            InitializeComponent();
            cvm = (ConnetVM)App.Current.Services.GetService(typeof(ConnetVM));
            DataContext = App.Current.Services.GetService(typeof(ConnetVM));
            getComboValue();
        }
        public void getComboValue()
        {
            string[] portsarray = SerialPort.GetPortNames();

            foreach (string portnumber in portsarray)
            {
                cvm.Ctm.PortList.Add(portnumber);
            }
            cvm.Ctm.SelPort = cvm.Ctm.PortList[0];
            cvm.Ctm.Protocoltxt = cvm.Ctm.ProtocolList[3];
            cvm.Ctm.Seldatalen = cvm.Ctm.DataLengthList[1];
            cvm.Ctm.Selstopbit = cvm.Ctm.StopBitList[0];
            cvm.Ctm.Selpeed = cvm.Ctm.SpeedList[1];
            cvm.Ctm.Selparity = cvm.Ctm.ParityList[2];
            sc = new SerialComm();
        }
    }
}
