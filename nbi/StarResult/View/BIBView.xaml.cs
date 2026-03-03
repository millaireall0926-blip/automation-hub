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

namespace StarResult.View
{
    /// <summary>
    /// BIBView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class BIBView : Page
    {
        public BIBView()
        {
            InitializeComponent();
            Global.gBIBView = this;
        }

        private void xCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Global.gBIBVM._bibWidth = e.NewSize.Width - 10;
            Global.gBIBVM._bibHeight = e.NewSize.Height - 10;
            if (Global.gBIBVM._bibWidth < 0) Global.gBIBVM._bibWidth = 0;
            if (Global.gBIBVM._bibHeight < 0) Global.gBIBVM._bibHeight = 0;

            Global.gBIBVM.setSize();
        }

        private void xCanvas_MouseDown(object sender, MouseButtonEventArgs e)
        {
            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            Global.gBIBVM.CurDno = Convert.ToInt32(bt.Uid);
            Global.gBIBVM._blockAdr = -1;
            Global.gBIBVM.selectDut(true);
        }

        public void getMousePos(out double x, out double y)
        {
            x = Mouse.GetPosition(this).X;
            y = Mouse.GetPosition(this).Y;
        }

    }
}
