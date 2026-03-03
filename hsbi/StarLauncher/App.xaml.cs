using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace StarLauncher
{
    /// <summary>
    /// App.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class App : Application
    {
        Mutex mutex;

        /// <summary>
        /// 중복 실행 방지 코드
        /// </summary>
        /// <param name="e"></param>
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            string mutexName = "Star Launcher HSBI";
            bool createNew;

            mutex = new Mutex(true, mutexName, out createNew);

            if (!createNew)
            {
                MessageBox.Show($"Already \'{mutexName}\' is started.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                Shutdown();
            }
        }
    }
}
