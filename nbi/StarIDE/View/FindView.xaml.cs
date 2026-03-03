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
    /// FindView.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 
    public enum FIND_OPTION : int
    {
        NONE = 0,
        REPLACE = 1,
        ALL = 2
    }

    public partial class FindView : UserControl
    {
        public FindView()
        {
            InitializeComponent();            
           
        }

        public EditModel _parentEM;

        public void focusText()
        {
//            xTB_Find.Focus();        
            Keyboard.Focus(xTB_Find);
            xTB_Find.SelectAll();
        }

        public void show(bool isShow, FIND_OPTION option = FIND_OPTION.NONE) // 0 = none, 1=replace, 2 = all, 3 = replace all
        {
            if (isShow)
            {
                Height = 62;
                Visibility = Visibility.Visible;
                if ((option & FIND_OPTION.REPLACE) != 0)
                {
                    Global.gDockVM.ReplaceVisible = Visibility.Visible;
                    Global.gDockVM.FindVisible = Visibility.Collapsed;
                    
                }
                else
                {                    
                    Global.gDockVM.ReplaceVisible = Visibility.Collapsed;
                    Global.gDockVM.FindVisible = Visibility.Visible;
                }

                if ((option & FIND_OPTION.ALL) != 0)
                    Global.gDockVM.FindAll = 1;
                else
                    Global.gDockVM.FindAll = 0;


            }
            else
            {
                Height = 0;
                Visibility = Visibility.Hidden;
            }
        }

        private void xTB_Find_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                bool isBack = false;
                bool isReplace = false;
                if (Keyboard.IsKeyDown(Key.LeftShift))
                    isBack = true;
                if (Global.gDockVM.ReplaceVisible == Visibility.Visible)
                    isReplace = true;

                _parentEM.findWord(isBack, isReplace);                

            }else if (e.Key == Key.F3)
            {
                _parentEM.findWord();
            }
            else if (e.Key == Key.F2)
            {
                _parentEM.findWord(true);
            }
            else if (e.Key == Key.Escape)
            {
                show(false);
                if (_parentEM != null)
                {
                    _parentEM._sci.Focus();
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            show(false);
            if (_parentEM != null)
            {
                _parentEM._sci.Focus();
            }
        }
    }
}
