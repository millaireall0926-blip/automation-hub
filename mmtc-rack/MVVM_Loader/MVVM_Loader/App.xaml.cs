using Microsoft.Extensions.DependencyInjection;
using MVVM_Loader.ViewModel;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace MVVM_Loader
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

            services.AddSingleton(typeof(ConnetViewModel));
            services.AddSingleton(typeof(DesciViewModel));
            services.AddSingleton(typeof(FuncViewModel));
            services.AddSingleton(typeof(Item1ViewModel));
            services.AddSingleton(typeof(Item2ViewModel));
            services.AddSingleton(typeof(Item3ViewModel));
            return services.BuildServiceProvider();
        }
    }
}
