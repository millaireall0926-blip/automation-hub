using HtcML.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
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

namespace HtcML
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            MainVM? vm =  DataContext as MainVM;
            
            vm.TrainingParagraph = new Paragraph();
            xTrainingTextBox.Document = new FlowDocument(vm.TrainingParagraph);
            vm.TrainingParagraph.LineHeight = 1;

            vm.TrainingScroll = xTraninigScroll;


            vm.PredictParagraph = new Paragraph();
            xPredictTextBox.Document = new FlowDocument(vm.PredictParagraph);
            vm.PredictParagraph.LineHeight = 1;

            vm.PredictScroll = xPredictScroll;

        }
    }
}
