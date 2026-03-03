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
using System.Windows.Shapes;

namespace StarOS.View
{
    /// <summary>
    /// Diag.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 

    public enum DIAG_TYPE : int
    {        
        SYS = 1,
        PG = 2,
        POWER = 4,
        SHMOO = 0x10,
        TDR = 0x20,
        PE_CAL = 0x100,
        DPS_CAL = 0x200,
        DPS_CHECK_BD = 0x400,
        DPS_CHECK_NBD = 0x800,

    }

    public partial class Diag : Window
    {
        public int _diagFlag = 0;


        public Diag()
        {
            InitializeComponent();
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                this.DialogResult = true;
            else if (e.Key == Key.Escape)
                this.DialogResult = false;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Button bt = e.Source as Button;
            if (bt.Content as string == "Execute")
            {
                _diagFlag = 0;
                if ((bool)xCB_PG.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.PG;
                if ((bool)xCB_Shmoo.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.SHMOO;
                if ((bool)xCB_TDR.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.TDR;
                if ((bool)xCB_PE.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.PE_CAL;
                if ((bool)xCB_DPS.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.DPS_CAL;
                if ((bool)xCB_CH_DPS.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.DPS_CHECK_BD;
                if ((bool)xCB_CH_DPS_NB.IsChecked)
                    _diagFlag |= (int)DIAG_TYPE.DPS_CHECK_NBD;

                if (_diagFlag == 0) return;
                this.DialogResult = true;
            }
            else if (bt.Content as string == "Cancel")
            {
                this.DialogResult = false;
            }
        }

        private void Rectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
                this.DragMove();
        }
    }
}
