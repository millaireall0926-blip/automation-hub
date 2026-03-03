using StarLauncher.Common;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarLauncher.Model;
using System.Xml;
using System.Reflection;
using System.Windows.Controls;
using System.Windows;
using System.Diagnostics;
using System.Threading;
using System.Windows.Threading;
using System.IO;

namespace StarLauncher.ViewModel
{
    public class MainVM : BaseVM
    {
        ObservableCollection<ToolModel> _toolModels = new ObservableCollection<ToolModel>();
        public ObservableCollection<ToolModel> ToolModels
        {
            get { return _toolModels; }
            set { _toolModels = value; OnPropertyChanged(nameof(ToolModels)); }
        }

        public bool _aliveOS = false;
        public bool AliveOS
        {
            get => _aliveOS;
            set
            {
                _aliveOS = value; OnPropertyChanged(nameof(AliveOS));
            }
        }

        DispatcherTimer _aliveTimer = new DispatcherTimer();

        public MainVM()
        {
            loadConfig();
            //    ToolModels.Add(new ToolModel("StarOS.exe") { Title = "OS", Tip= "Star-Test Operation System"});
            //    ToolModels.Add(new ToolModel("StarIDE\\StarIDE.exe") { Title = "Edit", Tip = "Star-Test IDE(Integrated Development Environment)" });
            //    ToolModels.Add(new ToolModel("StarResult\\StarResult.exe") { Title = "View", Tip = "Star-Test Result Viewer" });
            //    ToolModels.Add(new ToolModel("StarMaint.exe") { Title = "Maint", Tip = "Star-Test maintenance Program" });
            //    ToolModels.Add(new ToolModel("StarRouter\\StarRouter.exe") { Title = "Router", Tip = "Star-Test Slot Routing Program" });
            _aliveTimer.Interval = TimeSpan.FromSeconds(10);
            _aliveTimer.Tick += aliveTick;
            _aliveTimer.Start();

        }

        string _title = "STAR PROGRAMS";
        public string Title { get => _title; set { _title = value; OnPropertyChanged(nameof(Title)); } }
        public int WindowWidth = 600;

        public Visibility ShowLog { get; set; } = Visibility.Visible;

        public void loadConfig()
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load("StarLauncher.xml");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }

            var node = doc.SelectSingleNode("/StarLauncher");
            if (node != null)
            {
                if (node.Attributes["logo"] != null)
                {
                    if (Convert.ToInt32(node.Attributes["logo"].Value) == 0)
                        ShowLog = Visibility.Hidden;
                }
            }

            node = doc.SelectSingleNode("/StarLauncher/frame");
            Title = node.Attributes["name"].Value;

            var nodeList = doc.SelectNodes("/StarLauncher/tools/tool");
            int cnt = 0;
            foreach (XmlNode tool in nodeList)
            {
                string name = tool.Attributes["name"].Value;
                string path = tool.Attributes["path"].Value;
                string tip = tool.Attributes["tip"].Value;
                ToolModels.Add(new ToolModel(path) { Title = name, Tip = tip });
                cnt++;
            }
            WindowWidth = 120 * cnt + 60;

        }


        public bool CheckAlive(string name)
        {
            Mutex mutex = new Mutex(false, name, out bool createdNew);
            Console.WriteLine($"{name} {createdNew}");
            mutex.Close();
            if (createdNew)
                return false;

            return true;
        }

        public void aliveTick(object sender, EventArgs e)
        {
            if (!AliveOS) return;

            bool ret = CheckAlive("StarOS");
            if (!ret) Process.Start("StarOS.exe");

            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "StarRouter", "StarRouter.exe");
            ret = CheckAlive("StarRouter");
            if (!ret) Process.Start(path);
        }

    }
    
}
