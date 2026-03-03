using StarBase;
using StarIDE.Model;
using StarIDE.View;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel.Design.Serialization;
using System.Data;
using System.IO;
using System.Linq;
using System.Runtime.Remoting.Contexts;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;


namespace StarIDE.ViewModel
{
    public class AssistVM : BaseVM
    {

        #region API Assist Model
        public StackPanel xSP_API;

        string _apiSearch = "";
        public string APISearch { 
            get { return _apiSearch; } 
            set { _apiSearch = value; OnPropertyChanged(nameof(APISearch));
                setAPIList();
            } 
        }

        public void makeBIB()
        {            
            _bibView.Show();
        }

        public void addAPIModel(APIModel am)
        {
            var newExpander = new Expander
            {
                Header = am.Name,
                Margin = new Thickness(1),                
                Foreground = new SolidColorBrush(MConv.IntToMediaColor(0x45c9af)),
                FontWeight = FontWeights.Bold
            };

            
            var newTB = new TextBox
            {
                Text = am.Brief,
                IsReadOnly = true
            };
        
            if (am.Name.IndexOf("BIB") >= 0)
            {
                var newButton = new Button
                {
                    Content = "MAKE BIB",
                    HorizontalAlignment = HorizontalAlignment.Stretch,
                    Background = new SolidColorBrush(MConv.IntToMediaColor(0x3599af)),
                    Margin = new Thickness(1, 0, 1, 1)
                };
                newButton.Click += (s, e) =>
                {
                    Button bt = (Button)s; makeBIB();
                };
                var newStackPanel = new StackPanel { };
                newStackPanel.Children.Add(newTB);
                newStackPanel.Children.Add(newButton);                
                newExpander.Content = newStackPanel;

            }
            else 
                newExpander.Content = newTB;

            xSP_API.Children.Add(newExpander);
        }


        public void setAPIList()
        {
            if (xSP_API == null) return;
            xSP_API.Children.Clear();
            string search = APISearch.ToUpper();
            foreach (APIModel model in Global.gAPIFuns)
            {                
                if (search == "")
                {
                    addAPIModel(model);
                } else
                {
                    if (model.Name.IndexOf(search) != -1)
                        addAPIModel(model);
                }
            }
        }

        public void expandAPI()
        {
            foreach (Expander child in xSP_API.Children)
                child.IsExpanded = true;
        }

        #endregion

        #region REG Assist
        public ObservableCollection<REGModel> _sdefModels= new ObservableCollection<REGModel>();
        public ObservableCollection<REGModel> SdefModels
        {
            get { return _sdefModels; }
            set { _sdefModels = value; OnPropertyChanged(nameof(SdefModels)); }
        }

        public ObservableCollection<REGModel> _opcModels = new ObservableCollection<REGModel>();
        public ObservableCollection<REGModel> OpcModels
        {
            get { return _opcModels; }
            set { _opcModels = value; OnPropertyChanged(nameof(OpcModels)); }
        }

        public ObservableCollection<REGModel> _regModels = new ObservableCollection<REGModel>();
        public ObservableCollection<REGModel> RegModels
        {
            get { return _regModels; }
            set { _regModels = value; OnPropertyChanged(nameof(RegModels)); }
        }

        public DelegateCommand CmdSdefUpdate  { get; set;}

        public void setRegCollection(ObservableCollection<REGModel> collect, string code)
        {
            string[] bufs = code.Split(' ');
            foreach (string s in bufs)
            {
                if (s == "") continue;
                collect.Add(new REGModel { Name = s });
            }
        }

        public Dictionary<string, string> PATAutoComp = new Dictionary<string, string>();

        public void findSdef(FileModel fm)
        {
            FileStream fs = new FileStream(fm.Path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            StreamReader reader = new StreamReader(fs);
            string Text = reader.ReadToEnd();
            
            var mats = Regex.Matches(Text, @"SDEF\s+" + "[a-z|A-Z|0-9|_]+" + @"\b");
            foreach(Match mat in mats)
            {
                Console.WriteLine(mat.Groups[1].Value);
                string find = mat.Value.Substring(4).Trim();

                if (SdefModels.FirstOrDefault(m => m.Name == find)!= null)
                    continue;
                SdefModels.Add( new REGModel(){ Name =  find });
            }

            fs.Close();
            reader.Close();

        }

        public bool findDefFolder(FileModel folderFM)
        {
            if (folderFM.Type != FILE_TYPE.FOLDER) return false;
            foreach (FileModel fm in folderFM.InnerFiles)
            {
                if (fm.Type != FILE_TYPE.HEAD) continue;
                findSdef(fm);
            }
            return false;
        }

        public void updateSdef()
        {
            SdefModels.Clear();
            PATAutoComp.Clear();

            foreach (FileModel fm in Global.gFilesVM.FileModels)
            {
                if (fm.Type == FILE_TYPE.FOLDER)
                {
                    findDefFolder(fm);                  // Folder Depth 2단계 까지만   
                }
                else if (fm.Type == FILE_TYPE.HEAD)
                {
                    findSdef(fm);
                    
                }
            }

            foreach (REGModel rm in SdefModels)
            {
                if (!PATAutoComp.ContainsKey(rm.Name.Substring(0, 1)))
                    PATAutoComp[rm.Name.Substring(0, 1)] = "";
                PATAutoComp[rm.Name.Substring(0, 1)] += rm.Name + " ";
            }
        }

        #endregion

        #region BIB Build

        public BIBView _bibView;
        public DataGrid _xDataGrid;

        int _column = 10;
        public int Column
        {
            get { return _column; }
            set { _column = value; OnPropertyChanged(nameof(Column)); }
        }

        int _row = 10;
        public int Row
        {
            get { return _row; }
            set { _row = value; OnPropertyChanged(nameof(Row)); }
        }

        int _ioCount = 8;
        public int IOCount
        {
            get { return _ioCount; }
            set { _ioCount = value; OnPropertyChanged(nameof(IOCount)); }
        }

        int _multiChip = 1;
        public int MultiChip
        {
            get { return _multiChip; }
            set { _multiChip = value; OnPropertyChanged(nameof(MultiChip)); }
        }

        int _scanInc = 0;
        public int ScanInc
        {   
            get =>  _scanInc; 
            set { _scanInc = value; OnPropertyChanged(nameof(ScanInc)); }
        }

        int _ioInc = 0;
        public int IOInc
        {
            get => _ioInc;
            set { _ioInc = value; OnPropertyChanged(nameof(IOInc)); }
        }

        string _bibText = "";
        public string BIBText
        {
            get => _bibText;
            set { _bibText = value; OnPropertyChanged(nameof(BIBText)); }
        }

        public void setBIBSize(string line)
        {
            int pos = line.IndexOf("BIB_SIZE");
            if (pos == -1) return;
            string next = line.Substring(pos, line.Length - pos);
            pos = next.IndexOf("(");
            if (pos == -1) return;
            pos++;
            int pos2 = next.IndexOf(")");
            if (pos2 == -1) return;           
            next = next.Substring(pos, pos2 - pos);
            if (next.Length <= 0) return;
            string[] tok = next.Split(',');
            if (tok.Length != 4) return;
            Column = Convert.ToInt32(tok[0].Trim());
            Row = Convert.ToInt32(tok[1].Trim());
            IOCount = Convert.ToInt32(tok[2].Trim());
            MultiChip = Convert.ToInt32(tok[3].Trim());

            doBibButton("SET");
            _bibView.Show();
        }


        int _startX = 0;
        int _startY = 0;
        int _sizeX = 0;
        int _sizeY = 0;
        public int _xCopyProc = 1;
        public int _yCopyProc = 1;


        void setCellValue(int col, int row, string text)
        {
            DataGridColumn column1 = _xDataGrid.ColumnFromDisplayIndex(col);
            column1.OnPastingCellClipboardContent(_xDataGrid.Items[row], text);

        }

        string  getCellVaule(int col, int row)
        {            
            DataRowView item = _xDataGrid.Items[row] as DataRowView;
            string text = item.Row.ItemArray[col].ToString();
            return text;
        }

        bool doInc(string text, bool isUp, out string val)
        {
             val = "";
            int scProc = isUp ? (ScanInc * _yCopyProc) : (ScanInc * _xCopyProc);
            int ioProc = isUp ? (IOInc * _yCopyProc) : (IOInc * _xCopyProc);

            try
            {
                text = text.Trim();
                int pos = text.IndexOf("=");
                if (pos == -1) return false;                
                int scan = Int32.Parse(text.Substring(0, pos).Trim());
                scan += scProc;
                pos++;
                int ioStart = 0;
                int ioEnd = 0;
                if (IOCount == 1)
                {                    
                    ioStart = Convert.ToInt32(text.Substring(pos, text.Length - pos)) + ioProc;
                    val = $"{scan}={ioStart}";
                }
                else
                {
                    string ioText = text.Substring(pos, text.Length - pos);
                    string[] ios = ioText.Split('-');
                    if (ios.Length != 2) return false;
                    ioStart = Convert.ToInt32(ios[0]) + ioProc;
                    ioEnd = Convert.ToInt32(ios[1]) + ioProc;
                    val = $"{scan}={ioStart}-{ioEnd}";
                }
                if (scan > 255 || ioStart > 512 || ioEnd > 512) 
                    return false;

            }catch(Exception ex)
            {
                return false;   
            }


            return true;
        }

        void doCopy(bool up, bool right)
        {             
            int destX;
            int destY;    
            
            for (int x = 0; x < _sizeX; x++)
            {
                int xpos = x + _startX;                
                for (int y = 0; y < _sizeY; y++)
                {
                    int ypos = y + _startY;

                    destX = xpos;
                    if (right)
                        destX = xpos + (_sizeX * _xCopyProc);
                    
                    destY = ypos;
                    if (up)
                        destY = ypos - (_sizeY * _yCopyProc);

                    if (destX >= Column || destX < 0) continue;
                    if (destY >= Row || destY < 0) continue;
                    string text = getCellVaule(xpos, ypos);
                    string value;
                    if (!doInc(text, up, out value))
                    {
                        HandyControl.Controls.MessageBox.Show($"Context or IO/Scan Number Error", "BIB Make", MessageBoxButton.OK, MessageBoxImage.Hand);
                        return;
                    }
                    setCellValue(destX, destY, value);
                }
            }

            if (up) _yCopyProc++;
            if (right) _xCopyProc++;    
        }


        int getSelectGrid(bool del = false)
        {
            int idx = 0;
            foreach (var cell in _xDataGrid.SelectedCells)
            {
                int columnindex = cell.Column.DisplayIndex;
                int rowIndex = _xDataGrid.Items.IndexOf(cell.Item);
                if (del)
                {
                    (cell.Column.GetCellContent(cell.Item) as TextBlock).Text = "";
                    continue;
                }

                string value = (cell.Column.GetCellContent(cell.Item) as TextBlock).Text;

                if (idx == 0)
                {
                    _startX = columnindex;
                    _startY = rowIndex;                    
                }

                if (idx >= _xDataGrid.SelectedCells.Count -1)
                {
                    _sizeX = columnindex - _startX + 1;
                    _sizeY  = rowIndex - _startY + 1;
                     
                }
                Console.WriteLine($"-------------");
                Console.WriteLine($"x{_startX} -> {_sizeX}, y{_startY} -> {_sizeY}");
                idx++;
            }
            return idx;
        }
        

        public void doBibButton(string uid)
        {
            if (Column == 0 || Row == 0 || MultiChip == 0)
            {
                HandyControl.Controls.MessageBox.Show($"COL/ROW/MC Can't Set 0", "Set Layout", MessageBoxButton.OK, MessageBoxImage.Hand);
                return;
            }

            DataGrid dg = _bibView.xDG_BIB;
            if (uid == "SET")
            {
                dg.ItemsSource = null;
                dg.Columns.Clear();
                dg.Items.Refresh();

                var dt = new DataTable();

                int mc = Convert.ToInt32(MultiChip);
                for (int i = 0; i < Column; i++)
                {
                    string text = $"C{((i) / mc)+1}";
                    if (mc != 1) text += "-" + (((i) % mc)+1).ToString();
                    dt.Columns.Add(columnName: text, type: typeof(string));
                }

                for (int i = Row; i > 0; i--)
                {
                    string[] arr = new string[Column];
                    dt.Rows.Add(arr);
                }
                dg.ItemsSource = dt.DefaultView;
            }
            else if (uid == "UP")
            {
                getSelectGrid();
                doCopy(true, false);
                
            }
            else if (uid == "RIGHT")
            {
                getSelectGrid();
                doCopy(false, true);
                
            }
            else if (uid == "MAKE")
            {
                BIBText = "";
                StringBuilder sb = new StringBuilder();
                sb.Append($"BIB_SIZE({Column},{Row},{IOCount},{MultiChip});\r\n\r\n");
                for (int y = 0; y < Row; y++)                    
                {
                    sb.Append($"BIB({Row - (y+1)},");
                    for (int x = 0; x < Column; x++)
                    {
                        string text = getCellVaule(x, y);
                        Match m = Regex.Match(text, @"[0-9]+=[0-9]+(\-[0-9]+)*");
                        if (!m.Success)
                        {
                            HandyControl.Controls.MessageBox.Show($"Context Error", "BIB Make", MessageBoxButton.OK, MessageBoxImage.Hand);
                            return;
                        }
                        sb.Append($"{text}");
                        if (x != (Column - 1)) sb.Append(",");

                    }
                    sb.Append($");\r\n");
                }
                sb.Append("\r\n");
                BIBText += sb.ToString();
            }else if (uid == "DEL")
            {
                getSelectGrid(true);
            }
            else if (uid == "copy")
            {

            }else if (uid == "past")
            {

            }
        }

        public DelegateCommand CmdBIB { get; set; }
        #endregion

        public AssistVM()
        {           
            Global.gAssistVM = this;
            _bibView = new BIBView();
            CmdSdefUpdate = new DelegateCommand( p => updateSdef());
            setRegCollection(OpcModels, Global.gKeyOpcodePAT);
            setRegCollection(RegModels, Global.gKeyRegPAT);

            CmdBIB = new DelegateCommand(p => { doBibButton((p as Button).Uid); });
        }
    }
}
