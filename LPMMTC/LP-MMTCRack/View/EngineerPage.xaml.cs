using LP_MMTCRack.ViewModel;
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

namespace LP_MMTCRack.View
{
    /// <summary>
    /// EngineerPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class EngineerPage : Page
    {
        public EngineerPage()
        {
            InitializeComponent();
            Global.gEngineerVM.ListParaLog = xList_Para;
            Global.gEngineerVM.ListMotorLog = xList_Motor;
        }
        private void DataGrid_AutoGeneratingColumn(object sender, DataGridAutoGeneratingColumnEventArgs e)
        {
            var vm = (EngineerVM)this.DataContext;

            if (vm.ColumnCount == 12)
            {
                // "pCh" 이후의 숫자를 파싱하여 12보다 큰 경우 열 생성을 취소
                if (int.TryParse(e.PropertyName.Replace("pCh", ""), out int columnNumber) && columnNumber > 12)
                {
                    e.Cancel = true; // 12개까지만 열을 생성하고 그 이상은 취소
                    //Console.WriteLine($"{columnNumber}번째 열 생성을 취소함");
                }
            }
            else if (vm.ColumnCount == 18)
            {
                // "pCh" 이후의 숫자를 파싱하여 18보다 큰 경우 열 생성을 취소
                if (int.TryParse(e.PropertyName.Replace("pCh", ""), out int columnNumber) && columnNumber > 18)
                {
                    e.Cancel = true; // 18개까지만 열을 생성하고 그 이상은 취소
                    //Console.WriteLine($"{columnNumber}번째 열 생성을 취소함");
                }
            }

            // 헤더의 너비를 설정하는 부분
            if (e.Column is DataGridTextColumn textColumn)
            {
                if (e.PropertyName.StartsWith("pCh"))
                {
                    string headerText = e.PropertyName.Replace("pCh", "CH");
                    textColumn.Header = headerText;
                }

                if (e.PropertyName.Contains("Symbol"))
                    textColumn.Width = new DataGridLength(106);
                else
                    textColumn.Width = new DataGridLength(1, DataGridLengthUnitType.Star);
                // 너비를 고정값으로 설정 (예: 150)
                //textColumn.Width = new DataGridLength(150);

                // 혹은, 비율을 기반으로 설정 (1*: 전체 공간에서 나눠서 사용)
                //textColumn.Width = new DataGridLength(1, DataGridLengthUnitType.Star);
            }
        }
    }
}
