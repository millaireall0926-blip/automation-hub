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

namespace StarIDE.View
{
    /// <summary>
    /// FilesView.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class FilesView : Page
    {
        public FilesView()
        {
            InitializeComponent();
        }

        FileModel prevFile = null;
        private void xTV_Files_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            FileModel fm = (FileModel)e.NewValue;
            if (fm == null) return;

            Global.gDockVM.addDocPane(fm);
            if (prevFile != null)
            {
                Visibility rename = prevFile.IsTextBox;                
                if (rename == Visibility.Visible)
                    prevFile.doRename();
            }
            
            prevFile = fm;
        }

        void menuCommand(object sender, RoutedEventArgs e)
        {
            MenuItem menuItem = e.Source as MenuItem;
            if (menuItem == null) return;
            if (menuItem.Uid == "BuildItem")
            {
                MakedModel mm = menuItem.DataContext as MakedModel;
                Global.gFilesVM.biuldtemContext(menuItem.Header.ToString(), mm);
            }
            else  // File Item
            {
                FileModel fm = menuItem.DataContext as FileModel;
                Global.gFilesVM.doContextMenu(menuItem.Header.ToString(), fm);
            }
        }
        
    }
}
