
using StarMaint.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

namespace StarMaint
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {


        public MainVM vm;
        public MainWindow()
        {

            InitializeComponent();
            vm = this.DataContext as MainVM;
            vm._mainWin = this;
            vm._scintilla = xSintilla;
            vm.initSci();
            vm.initCommand();            

            this.Title = VerInfo.getVer();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            foreach (CommandBinding binding in MainVM.Commands)
            {
                CommandBindings.Add(binding);
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            MainVM._isClose = true;
            vm.loadingWin.Close();
        }

        private void SlotGrid_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {            
            int rowIndex = 0;
            foreach (DataGridCellInfo sel in SlotGrid.SelectedCells)
            {
                rowIndex = SlotGrid.Items.IndexOf(sel.Item);
                break;
            }
            vm.selectSlot(rowIndex);
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {       
            ListBox lb = sender as ListBox;
            if (lb == null) return;
            if (lb.SelectedItem == null) return;
            vm.selectFolder(lb.SelectedItem);
        }

        private void ToolbarButton_Click(object sender, RoutedEventArgs e)
        {
            vm.clickToolbar(sender, e);
        }


    }

}
