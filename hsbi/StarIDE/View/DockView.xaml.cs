using StarIDE.Model;
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
using Xceed.Wpf.AvalonDock;
using Xceed.Wpf.AvalonDock.Layout;
using Xceed.Wpf.AvalonDock.Themes;

namespace StarIDE.View
{
    /// <summary>
    /// DockView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class DockView : Page
    {
        public DockView()
        {
            InitializeComponent();

            xDockManager.Theme = new VS2010Theme();
            Global.gDockVM._documentManager = xPane_Document;
            Global.gDockVM.LB_Output = xLB_Output;
        }

        private void xDockManager_ActiveContentChanged(object sender, EventArgs e)
        {
            DockingManager dm = sender as DockingManager;
            LayoutDocument ld = dm.ActiveContent as LayoutDocument;
            if (ld == null) return;

            Global.gMainVM.CurEdit = Global.gDockVM.findEdit(ld.Title);
  

        }

        private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {           
            ListBox lb = sender as ListBox;
            if (lb == null) return;
            ReportModel rm = lb.SelectedItem as ReportModel;
            if (rm == null) return;
            if (rm.Line == -1) return;
            Global.gDockVM.focusError(rm);



        }
    }
}
