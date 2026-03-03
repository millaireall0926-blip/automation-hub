using StarMaint.ViewModel;
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

namespace StarMaint.View
{
    /// <summary>
    /// BoardNo.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class BoardNo : Page
    {
        
        public BoardNo()
        {
            InitializeComponent();

            MainWindow mw = Window.GetWindow(App.Current.MainWindow) as MainWindow;
            mw.vm._boardNoPage = this;
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            foreach (CommandBinding binding in MainVM.Commands)
            {
                CommandBindings.Add(binding);
            }

            MainWindow mw = Window.GetWindow(App.Current.MainWindow) as MainWindow;
            this.DataContext = mw.DataContext;
        }
    }
}
