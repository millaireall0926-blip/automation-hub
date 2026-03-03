using StarQMOS.ViewModel;
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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace StarQMOS.View
{
    /// <summary>
    /// HandlerPage.xaml에 대한 상호 작용 논리
    /// </summary>
    /// 


    public partial class HandlerPage : Page
    {
        
        DispatcherTimer StoryTimer = new DispatcherTimer();
        
        Storyboard SBLoad;
        Storyboard SBTest;        
        
        public HandlerPage()
        {
            InitializeComponent();
            StoryTimer.Interval = TimeSpan.FromSeconds(1);
            StoryTimer.Tick += timer_func;
            StoryTimer.Start();

        
            SBLoad = (Storyboard)FindResource("story_load");
            SBTest = (Storyboard)FindResource("story_test");

            


        }

        public void timer_func(object sender, EventArgs e)
        {
            if (Global.gHandlerVM == null) return;
            if (Global.gHandlerVM.StoryMode == STORY_MODE.NONE) return;            
            switch (Global.gHandlerVM.StoryMode)
            {                
                case STORY_MODE.LOAD_DONE:
                    SBLoad.Begin(); break;
                case STORY_MODE.TEST_DONE:
                    SBTest.Begin(); break;                
            }
        }



        //private void Button_Click(object sender, RoutedEventArgs e)
        //{            
        //    byte[] datas = new byte[8];
        //    for (int i = 0; i < 8; i++) datas[i] = 0x01;
        //
        //    Global.gNet._handler.send(0x10, datas);
        //}

    }
}
