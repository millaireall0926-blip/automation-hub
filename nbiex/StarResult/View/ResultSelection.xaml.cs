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
    /// ResultSelection.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class ResultSelection : Page
    {
        public ResultSelection()
        {
            InitializeComponent();
            Global.gResSelVM.xSP_File = xSP_File;
        }

        private void SlotGrid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            int rowIndex = 0;
            foreach (DataGridCellInfo sel in SlotGrid.SelectedCells)
            {
                rowIndex = SlotGrid.Items.IndexOf(sel.Item);
                break;
            }
            Global.gResSelVM.setSlot(rowIndex);            
            Global.gResSelVM.findFile();
        }
    }
}
