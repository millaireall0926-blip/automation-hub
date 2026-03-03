using StarResult.Model;
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
    /// StepView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class StepView : Page
    {
        public StepView()
        {
            InitializeComponent();
            DataContext = Global.gMainVM;
        }

        private void StepList_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var item = (sender as ListView).SelectedItem;

            if (item != null)
            {
                StepModel step = item as StepModel;
                Console.WriteLine("Step {0} count {1}", step.Name, (sender as ListView).Items.Count);
                Global.gMainVM.selectStep(step);
            }
        }
    }
}
