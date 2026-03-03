using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Extensions.DependencyInjection;
using Debug_TempCont.ViewModel;

namespace Debug_TempCont
{
    /// <summary>
    /// App.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class App : Application
    {
        public App()
        {
            Services = ConfigureServices();
            this.InitializeComponent();
        }
        public new static App Current => (App)Application.Current;
        public IServiceProvider Services { get; }

        /// <summary>
        /// Configures the services for the application.
        /// </summary>
        private static IServiceProvider ConfigureServices()
        {
            var services = new ServiceCollection();

            services.AddSingleton(typeof(ConnetVM));
            services.AddSingleton(typeof(FuncVM));
            services.AddSingleton(typeof(TempChVM));
            services.AddSingleton(typeof(GraphVM));

            return services.BuildServiceProvider();
        }
    }
}
