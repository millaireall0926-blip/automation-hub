using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Extensions.DependencyInjection;
using StarRouter.ViewModel;

namespace StarRouter {
    /// <summary>
    /// App.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class App : Application {

        public App() {
            Services = ConfigureServices();
            this.InitializeComponent();
        }
        public new static App Current => (App)Application.Current;
        public IServiceProvider Services { get; }

        /// <summary>
        /// Configures the services for the application.
        /// </summary>
        private static IServiceProvider ConfigureServices() {
            var services = new ServiceCollection();    
            
            services.AddSingleton(typeof(Config));    
            services.AddSingleton(typeof(MainVM));

            return services.BuildServiceProvider();
        }

        Mutex mutex;
        protected override void OnStartup(StartupEventArgs e) {
            base.OnStartup(e);

            string mutexName = "StarRouter";
            bool createNew;

            mutex = new Mutex(true, mutexName, out createNew);

            if (!createNew) {
                MessageBox.Show($"Already \'{mutexName}\' is started.", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                Shutdown();
            }
        }
    }
}
