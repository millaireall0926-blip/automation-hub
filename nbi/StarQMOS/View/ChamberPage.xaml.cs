using StarQMOS.ViewModel;
using System;
using System.Collections.Generic;
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

namespace StarQMOS.View
{
    /// <summary>
    /// ChamberPage.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 
    public partial class ChamberPage : Page
    {
        public ChamberPage()
        {
            InitializeComponent();

            Global.gChamberVM.xTempGraph = xTempGraph;

            Global.gChamberVM.initItems();

//            foreach (CommandBinding command in ChamberVM.Commands)
//            {
//                CommandBindings.Add(command);
//            }
        }

        private void PLC_Menu_Click(object sender, RoutedEventArgs e)
        {
            Button bt= e.Source as Button;
            Global.gChamberVM.excute_CmdPLC(bt);

        }
    }
}
