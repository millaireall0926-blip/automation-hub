using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using HandyControl.Controls;
using StarBase;
using StarResult.Model;

namespace StarResult.ViewModel
{
    public class ResultSectionVM : BaseVM
    {

        public StackPanel xSP_File;

        public string _selSlot = "ALL";
        public int _selIndex = 0;

        public DelegateCommand _findCmd;
        public DelegateCommand FindCmd
        {
            get { return _findCmd; }
            set { _findCmd = value; }
        }

        public string _selLotID = "";
        public string SelLotID 
        {
            get { return _selLotID; }
            set { _selLotID = value;  OnPropertyChanged(nameof(SelLotID)); }
        }

        public string _selBID;
        public string SelBID
        {
            get { return _selBID; }
            set { _selBID = value; OnPropertyChanged(nameof(SelBID)); }
        }

        DateTime _selFrom = DateTime.Now;
        public DateTime SelFrom
        {
            get { return _selFrom; }
            set { _selFrom = value; OnPropertyChanged(nameof(SelFrom)); }
        }

        DateTime _selTo = DateTime.Now;
        public DateTime SelTo
        {
            get { return _selTo; }
            set { _selTo = value; OnPropertyChanged(nameof(SelTo)); }
        }

        public Button _prevSelect = null;
        public SolidColorBrush _selectColor = new SolidColorBrush(Colors.OrangeRed);
        public SolidColorBrush _dismissColor = new SolidColorBrush(Colors.White);


        ObservableCollection<SlotModel> _slots;
        public ObservableCollection<SlotModel> Slots
        {
            get { return _slots; }
            set
            {
                _slots = value;
                OnPropertyChanged(nameof(Slots));
            }
        }

        public ResultSectionVM()
        {
            Global.gResSelVM = this;
            initSlot();

            FindCmd = new DelegateCommand(p =>
           {
               findFile( (FILTER) Convert.ToInt32(p) );
           });
        }


        public void initSlot()
        {
            Slots = new ObservableCollection<SlotModel>();

            SlotModel slot = new SlotModel()
            {
                BID = "",
                Name = "ALL"
                //                    Statbr = StatColArr[0]
            };
            Slots.Add(slot);

            for (int i = 0; i < 16; i++)
            {
                slot = new SlotModel()
                {
                    BID = "",
                    Name = String.Format("Slot" + (i + 1).ToString("D2")),                    
//                    Statbr = StatColArr[0]
                };
                Slots.Add(slot);
            }
        }

        public void setSlot(int idx)
        {
            _selSlot =  Slots[idx].Name;
            _selIndex = idx;
        }

        
        public void openFile(string lot)
        {            

            Global.gMainVM.selectFile(lot);
        }

        

        public void findFile(FILTER filter = FILTER.NONE)
        {
            _prevSelect = null;
            xSP_File.Children.Clear();
            if (_selSlot == "") return;

            string lotFilter = "*";

            string slotFilter = "";
            if (_selSlot != "ALL")
                slotFilter = _selSlot;

            string[] datas;
            try
            {
                datas = Directory.GetFiles(Global.gResultPath + "\\result", lotFilter + slotFilter + ".xml", SearchOption.TopDirectoryOnly);
            }catch (Exception ex)
            {
                Growl.InfoGlobal(ex.Message);
                return;
            }

            
            Collection<DateFile> dateFiles = new Collection<DateFile>(); ;
            foreach (string data in datas)
            {
                var info = new FileInfo(data);
                if (!info.Exists) continue;
                if (filter == FILTER.LOT)
                {
                    if (SelLotID != "")
                        if (data.IndexOf(SelLotID) == -1)
                            continue;
                }
                if (filter == FILTER.TIME)
                {
                    int from = DateTime.Compare(info.LastWriteTime, SelFrom);
                    int to = DateTime.Compare(SelTo.AddDays(1), info.LastWriteTime);
                    if (!(from >= 0 && to >= 0)) continue;
                }


                DateFile df = dateFiles.FirstOrDefault(a => a.dateTime.Date == info.LastWriteTime.Date);
                if (df == null) {
                    df = new DateFile();
                    df.dateTime = info.LastWriteTime;
                    dateFiles.Add(df);
                }

                int st = data.IndexOf(@"\result\") + 8;
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
                    Background = new SolidColorBrush(Colors.LightPink),
                    FontWeight = FontWeights.Bold
                    
                };
                var newStackPanel = new StackPanel {};

                foreach (string file in df.files)
                {
                    var newButton = new Button 
                    { 
                        Content = file, 
                        HorizontalAlignment = HorizontalAlignment.Stretch, 
                        Margin = new Thickness(3,1,3,0)
                        
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
    }
}
