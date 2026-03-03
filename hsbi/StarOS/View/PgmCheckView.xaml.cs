using StarOS.ViewModel;
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
	/// PgmCheckView.xaml에 대한 상호 작용 논리
	/// </summary>
	public partial class PgmCheckView : Window
	{
		public PgmCheckView(PgmCheckVM viewModel)
		{
			InitializeComponent();

			this.DataContext = viewModel;
		}

		private void BtnOk_Click(object sender, RoutedEventArgs e)
		{
			this.DialogResult = true;
			this.Close();
		}

		private void BtnCancel_Click(object sender, RoutedEventArgs e)
		{
			this.DialogResult = false;
			this.Close();
		}
	}
}
