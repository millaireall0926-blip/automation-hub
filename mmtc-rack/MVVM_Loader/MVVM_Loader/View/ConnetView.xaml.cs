using MVVM_Loader.Common;
using MVVM_Loader.Model;
using MVVM_Loader.ViewModel;
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

namespace MVVM_Loader.View
{
    /// <summary>
    /// ConnetView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ConnetView : Page
    {
        SerialComm sc;
        ConnetViewModel cvm = null;
        public ConnetView()
        {
            InitializeComponent();
            cvm = (ConnetViewModel)App.Current.Services.GetService(typeof(ConnetViewModel));
            DataContext = App.Current.Services.GetService(typeof(ConnetViewModel));
            getComboValue();

            //Global.gConnetView = this;
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
