using HandyControl.Controls;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
using TempView.Base;


namespace TempView.ViewModel
{
    public enum FILTER : int
    {
        NONE = 0,
        NAME = 1,
        TIME = 2
    };

    public class DateFile
    {
        public DateFile()
        {
            files = new Collection<string>();
        }

        public DateTime dateTime;
        public Collection<string> files;
    }
    public class SelectionVM : BaseVM
    {
        public StackPanel xSP_File;

        public string FindName { get; set; } = "";
        public DelegateCommand FindCmd { get; set; }
        public DateTime SelFrom { get; set; } = DateTime.Now;
        public DateTime SelTo { get; set; } = DateTime.Now;

        public Button _prevSelect = null;
        public SolidColorBrush _selectColor = new SolidColorBrush(Colors.CadetBlue);
        public SolidColorBrush _dismissColor = new SolidColorBrush(Colors.White);

        public SelectionVM() 
        {
            FindCmd = new DelegateCommand(p => { findFile((FILTER)Convert.ToInt32(p)); });
            Global.gSelectVM = this;
        }

        public void clearFileList()
        {
            xSP_File.Children.Clear();
        }

        public void findFile (FILTER filter = FILTER.NONE)
        {
            clearFileList();

            string RackFilter = "*";
            if (FindName != "")
                RackFilter = FindName;

            string[] datas;
            try
            {
                string path = Global.gMainVM.DataPath;
                datas = Directory.GetFiles(path, RackFilter + ".csv", SearchOption.TopDirectoryOnly);
            }
            catch (Exception ex)
            {
                Growl.InfoGlobal(ex.Message);
                return;
            }

            Collection<DateFile> dateFiles = new Collection<DateFile>(); ;
            foreach (string data in datas)
            {
                var info = new FileInfo(data);
                if (!info.Exists) continue;
                if (filter == FILTER.NAME)
                {
                    if (FindName != "")
                        if (data.IndexOf(FindName) == -1)
                            continue;
                }
                if (filter == FILTER.TIME)
                {
                    int from = DateTime.Compare(info.LastWriteTime, SelFrom);
                    int to = DateTime.Compare(SelTo.AddDays(1), info.LastWriteTime);
                    if (!(from >= 0 && to >= 0)) continue;
                }


                DateFile df = dateFiles.FirstOrDefault(a => a.dateTime.Date == info.LastWriteTime.Date);
                if (df == null)
                {
                    df = new DateFile();
                    df.dateTime = info.LastWriteTime;
                    dateFiles.Add(df);
                }

                int st = data.IndexOf(Global.gMainVM.DataPath) + Global.gMainVM.DataPath.Length +1;
                int len = data.Length - (st + 4); // +4 = .xml
                if (len <= 0) continue;
                string file = data.Substring(st, len);

                df.files.Add(file);
            }

            var list2 = new List<DateFile>();
            list2.AddRange(dateFiles);
            list2.Sort((a, b) => b.dateTime.CompareTo(a.dateTime));
            Console.WriteLine("[{0}]", string.Join(", ", list2));


            if (dateFiles.Count == 0) return;

            bool expand = true;
            foreach (DateFile df in list2)
            {

                var newExpander = new Expander
                {
                    Header = df.dateTime.Date.ToString("yyyy-MM-dd"),
                    Margin = new Thickness(5),
                    Background = new SolidColorBrush(Colors.LightSkyBlue),
                    FontWeight = FontWeights.Bold

                };
                var newStackPanel = new StackPanel { };

                foreach (string file in df.files)
                {
                    var newButton = new Button
                    {
                        Content = file,
                        HorizontalAlignment = HorizontalAlignment.Stretch,
                        Margin = new Thickness(3, 1, 3, 0)

                    };
                    newButton.Click += (s, e) => { Button bt = (Button)s; openFile(bt.Content.ToString());
                        if (_prevSelect != null) _prevSelect.Background = _dismissColor;
                        bt.Background = _selectColor;
                        _prevSelect = bt;
                    };
                    newStackPanel.Children.Add(newButton);
                }

                newExpander.Content = newStackPanel;
                newExpander.IsExpanded = expand;
                expand = false;
                xSP_File.Children.Add(newExpander);

            }
        }

        public void openFile(string lot)
        {
            Global.gMainVM.selectFile(lot);
        }



    }
}
