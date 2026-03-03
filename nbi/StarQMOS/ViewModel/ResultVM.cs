using ExcelDataReader;
using HandyControl.Controls;
using StarBase;
using StarQMOS.Model;
using StarQMOS.View;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml;

namespace StarQMOS.ViewModel
{
    public enum RES_COL : int
    {
        CONTACT_FAIL = 0,       // Dark Gray
        TOTAL_PASS = 1,         // Light Green
        STEP_PASS = 2,          // Green
        STEP_PASS_CON_SOME = 3, // Dark Green
        STEP_SOME= 4,           // Yellow
        STEP_SOME_CON_SOME = 5, // Dark Yellow
        STEP_FAIL = 6,          // Indian Red
        STEP_FAIL_CON_SOME = 7  // Dark Red       
    }


    public class ResultVM : BaseVM
    {

        #region Sub UI Data Items
        // Uint for Show, Hide Proc
        public int _blockDisplay = -1; // 0 = Block(ListBox), 1=Detail(Grid)

        public StackPanel xSP_Bin;

        public Visibility _showDetail;
        public Visibility ShowDetail
        {
            get { return _showDetail; }
            set { _showDetail = value; OnPropertyChanged(nameof(ShowDetail)); }
        }
        public Visibility _showBlock;
        public virtual Visibility ShowBlock
        {
            get { return _showBlock; }
            set { _showBlock = value; OnPropertyChanged(nameof(ShowBlock)); }
        }

        public Visibility _showBin;
        public virtual Visibility ShowBin
        {
            get { return _showBin; }
            set { _showBin = value; OnPropertyChanged(nameof(ShowBin)); }
        }

        public Visibility _showCycle;
        public virtual Visibility ShowCycle
        {
            get { return _showCycle; }
            set { _showCycle = value; OnPropertyChanged(nameof(ShowCycle)); }
        }

        public string _binText;
        public string BinText
        {
            get { return _binText; }
            set { _binText = value; OnPropertyChanged(nameof(BinText)); }
        }


        public string _subItemCount;
        public string SubItemCount
        {
            get => _subItemCount;
            set { _subItemCount = value; OnPropertyChanged(nameof(SubItemCount)); }
        }

        public string _subType;
        public string SubType
        {
            get => _subType;
            set { _subType = value; OnPropertyChanged(nameof(SubType)); }
        }


        public class LineIndex : BaseVM {
            public string line = "";
            public string Line {
                get => line;
                set { line = value; OnPropertyChanged(nameof(Line)); }
            }

            public double colWidth = 0;
            public double ColWidth {
                get => colWidth;
                set { colWidth = value; OnPropertyChanged(nameof(ColWidth)); }
            }
        }

        double _colWidth = 0;
        public double ColWidth {
            get => _colWidth; set {
                _colWidth = value;
                foreach (var line in ColIndexs) {
                    line.ColWidth = _colWidth;
                }
            }
        }

        public ObservableCollection<LineIndex> _colIndexs = new ObservableCollection<LineIndex>();
        public ObservableCollection<LineIndex> ColIndexs {
            get { return _colIndexs; }
            set { _colIndexs = value; OnPropertyChanged(nameof(ColIndexs)); }
        }

        double _rowHeight = 0;
        public double RowHeight { get => _rowHeight; set { _rowHeight = value; OnPropertyChanged(nameof(RowHeight)); } }
        public ObservableCollection<LineIndex> _rowIndexs = new ObservableCollection<LineIndex>();
        public ObservableCollection<LineIndex> RowIndexs {
            get { return _rowIndexs; }
            set { _rowIndexs = value; OnPropertyChanged(nameof(RowIndexs)); }
        }



        public ObservableCollection<SubBLockModel> _subBlockModels = new ObservableCollection<SubBLockModel>();
        public ObservableCollection<SubBLockModel> SubBlockModels
        {
            get { return _subBlockModels; }
            set { _subBlockModels = value; OnPropertyChanged(nameof(SubBlockModels)); }
        }

        public ObservableCollection<SubFDetailModel> _subFDetailBlocks = new ObservableCollection<SubFDetailModel>();
        public ObservableCollection<SubFDetailModel> SubFDetailBlocks
        {
            get { return _subFDetailBlocks; }
            set { _subFDetailBlocks = value; OnPropertyChanged(nameof(SubFDetailBlocks)); }
        }

        public ObservableCollection<SubFDetailModel> _subFDetailPages = new ObservableCollection<SubFDetailModel>();
        public ObservableCollection<SubFDetailModel> SubFDetailPages
        {
            get { return _subFDetailPages; }
            set { _subFDetailPages = value; OnPropertyChanged(nameof(SubFDetailPages)); }
        }

        public ObservableCollection<SubFDetailModel> _subFDetailCols = new ObservableCollection<SubFDetailModel>();
        public ObservableCollection<SubFDetailModel> SubFDetailCols
        {
            get { return _subFDetailCols; }
            set { _subFDetailCols = value; OnPropertyChanged(nameof(SubFDetailCols)); }
        }

        string[] _colDatas = new string[3096];

        #endregion UI Sub Items

        public Collection<SolidColorBrush> ResColors = new Collection<SolidColorBrush>();

        public BIBModel _layout = new BIBModel();
        public ObservableCollection<DutModel> _dutModels = new ObservableCollection<DutModel>();
        public ObservableCollection<DutModel> DutModels
        {
            get { return _dutModels; }
            set
            {
                _dutModels = value;
                OnPropertyChanged(nameof(DutModels));
            }
        }

        public ObservableCollection<DutModel> _mcDutModels = new ObservableCollection<DutModel>();
        public ObservableCollection<DutModel> MCDutModels
        {
            get { return _mcDutModels; }
            set
            {
                _mcDutModels = value;
                OnPropertyChanged(nameof(MCDutModels));
            }
        }

        public DataTable _DT_Cycle = new DataTable();
        public DataTable DT_Cycle
        {
            get { return _DT_Cycle; }
            set
            {
                _DT_Cycle = value;
                OnPropertyChanged(nameof(DT_Cycle));
            }
        }



        public ObservableCollection<StepModel> _stepModels = new ObservableCollection<StepModel>();
        public ObservableCollection<StepModel> StepModels
        {
            get { return _stepModels; }
            set { _stepModels = value; OnPropertyChanged(nameof(StepModels)); }
        }


        string _totalText = "STEP";
        public string TotalText
        {
            get => _totalText;
            set
            {
                _totalText = value;
                OnPropertyChanged("TotalText");
                if (_curStep != null) {
                    setStep(_curStep.Name);
                    if (MultiChipWin.IsEnabled)
                        MultiChipWin.Hide();
                }
            }
        }

        bool _total = false;
        public bool Total
        {
            get => _total;
            set
            {
                _total = value;
                if (value == true) TotalText = "TOTAL";
                else TotalText = "STEP";
                if (_curStep != null)
                {
                    setStep(_curStep.Name);
                    if (MultiChipWin.IsEnabled)
                        MultiChipWin.Hide();
                }
                OnPropertyChanged("Total");
            }
        }

        public int _curDno = 0;
        public int CurDno
        {
            get => _curDno;
            set { _curDno = value; OnPropertyChanged(nameof(CurDno)); }
        }

        public StepModel _curStep = null;
        int _TextMode = 1;
        public int TextMode
        {
            // 0 : NO TEXT                      
            // 1: Num
            // 2: IO        
            // 3: FAILBIT          
            // 4: CHIPID            
            // 5: BIN
            get { return _TextMode; }
            set { _TextMode = value;
                OnPropertyChanged(nameof(TextMode));
                setDutText();
            }
        }


        string _selSlot;
        public string SelSlot
        {
            get => _selSlot;
            set
            {
                _selSlot = value; OnPropertyChanged("SelSlot");
            }
        }

        public double _bibWidth = 0;
        public double _bibHeight = 0;

        #region Select / Set Process

        public bool _busyResult = false;
        public void setSlot(SlotModel slot)
        {
            StepModels.Clear();
            if ((Global.gMainVM.ResultWin.Visibility == Visibility.Hidden)) return;

            _busyResult = true;
            SelSlot = slot.Name;
            setBIB(slot);
            setBinList();

            if (MultiChipWin.IsEnabled)
                MultiChipWin.Hide();
            if (slot._steps.Count == 0)
            {
                initSubItem();
                _busyResult = false;
                return;
            }
            foreach (var step in slot._steps)
            {
                StepModels.Add(MConv.ObjectDeepCopy(step));
            }

            StepModel last = slot._steps.Last();
            setStep(last.Name);

            _busyResult = false;


        }

        public void addStep(StepModel step)
        {
            StepModels.Add(MConv.ObjectDeepCopy(step));
            setStep(step.Name);
        }


        public void setDutColorPF(SlotModel slot) {
            int mc = _layout._MultiChip;
            if (mc == 0) mc = 1;
            int testCnt = slot._dutModels.Count / mc;


            for (int idx = 0; idx < testCnt; idx++) {

                //if (slot._dutDatas[idx * mc]._conDic.Count == 0) continue;
                int conCum = 0;
                int stepCum = 0;
                int totalCum = 0;

                for (int j = 0; j < mc; j++) {
                    int i = (idx * mc) + j;
                    if (slot._dutDatas[i]._conDic[_curStep.Name])
                        conCum++;
                    if (slot._dutDatas[i]._curDic[_curStep.Name])
                        stepCum++;
                    if (slot._dutDatas[i]._totalDic[_curStep.Name])
                        totalCum++;
                }

                for (int j = 0; j < mc; j++) {
                    int i = (idx * mc) + j;
                    if (!_curStep.IsContact) {
                        if (conCum == 0) // All Contact Fail
                        {
                            DutModels[i].Color = ResColors[((int)RES_COL.CONTACT_FAIL)];
                            continue;
                        }
                    }
                    else
                        conCum = mc;

                    int selCum = stepCum;
                    if (Total) selCum = totalCum;

                    if (conCum == mc) // All Contact
                    {
                        if (selCum == 0)  // All Fail
                        {
                            DutModels[i].Color = ResColors[(int)RES_COL.STEP_FAIL];
                        }
                        else if (selCum == mc)  // All Pass
                        {
                            if (Total)
                                DutModels[i].Color = ResColors[(int)RES_COL.TOTAL_PASS];
                            else
                                DutModels[i].Color = ResColors[(int)RES_COL.STEP_PASS];
                        }
                        else    // Some Fail
                        {
                            DutModels[i].Color = ResColors[((int)RES_COL.STEP_SOME)];
                        }
                    }
                    else             // Some Contact
                    {
                        if (selCum == 0)  // All Fail
                        {
                            DutModels[i].Color = ResColors[(int)RES_COL.STEP_FAIL_CON_SOME];
                        }
                        else if (selCum == mc)  // All Pass
                        {

                            DutModels[i].Color = ResColors[(int)RES_COL.STEP_PASS_CON_SOME];

                        }
                        else    // Some Fail
                        {
                            DutModels[i].Color = ResColors[(int)RES_COL.STEP_SOME];
                        }
                    }

                }
            }

        }

        public void setDutColorBin() {
            int mc = _layout._MultiChip;
            if (mc == 0) mc = 1;
            int testCnt = DutModels.Count / mc;

            for (int idx = 0; idx < testCnt; idx++) {
                for (int j = 0; j < mc; j++) {
                    int i = (idx * mc) + j;
                    DutModel model = DutModels[i];
                    int bin = 0;
                    if (model.Bin != "")
                        bin = Convert.ToInt32(model.Bin);
                    SolidColorBrush cb;
                    if (Global.BinColorDic.ContainsKey(bin))
                        cb = Global.BinColorDic[bin];
                    else
                        cb = new SolidColorBrush(Colors.DarkGray);

                    model.Color = cb;
                }
            }
        }

        public void setBinList() {
            if (xSP_Bin.Children.Count > 0)
                xSP_Bin.Children.Clear();
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (slot == null) return;
            if (slot._binGathering.Count == 0) return;

            foreach (var item in slot._binGathering) {
                int bin = item.Key;
                int count = item.Value;
                SolidColorBrush cb;
                SolidColorBrush fc = new SolidColorBrush(Colors.Black);
                if (Global.BinColorDic.ContainsKey(bin))
                    cb = Global.BinColorDic[bin];
                else
                    cb = new SolidColorBrush(Colors.DarkGray);
                string text = $"BIN[{bin}] = Count {count}";
                TextBlock TB = new TextBlock {
                    Background = cb, Text = text,
                    Margin = new Thickness(3, 1, 3, 1),
                    FontSize = 13,
                    Foreground = fc
                };
                xSP_Bin.Children.Add(TB);
            }

        }

        public void setStep(string name)
        {

            _curStep = StepModels.FirstOrDefault(a => a.Name == name);
            if (_curStep == null) return;
            //Dut Data 는 Slot data로 운영
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (slot._dutModels.Count != DutModels.Count) return;
            if (TextMode != 5)
                setDutColorPF(slot);
            else
                setDutColorBin();


            setDutText();
            initSubItem();
            if (_curStep._subType == SUB_TYPE.CYCLE)
            {
                displayBlockResult(SUB_TYPE.CYCLE);
                loadCycle();
            }
            else
                displayBlockResult(SUB_TYPE.DATA);

        }

        public void setDutText()
        {
            if (_curStep == null) return;
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (slot._dutModels.Count != DutModels.Count) return;
            for (int i = 0; i < slot._dutModels.Count; i++)
            {
                switch (TextMode)
                {
                    case 0: // NO TEXT
                        DutModels[i].Text = ""; break;
                    case 1: // NO
                        DutModels[i].Text = slot._dutModels[i].Num.ToString(); break;
                    case 2:  // IO
                        DutModels[i].Text = slot._dutModels[i].IO; break;
                    case 3:  // FAILBIT
                        if (slot._dutDatas[i]._fBit.ContainsKey(_curStep.Name))
                            DutModels[i].Text = Convert.ToString(slot._dutDatas[i]._fBit[_curStep.Name], 16);
                        else
                            DutModels[i].Text = "";
                        break;
                    case 4:  // CHIPID
                        DutModels[i].Text = slot._dutModels[i].ChipID; break;
                    case 5:  // BIN
                        DutModels[i].Text = slot._dutModels[i].Bin; break;


                    default: break;


                }
            }
        }
        public void setBIB(SlotModel slot)
        {
            _layout = MConv.ObjectDeepCopy(slot._layout);
            DutModels.Clear();
            if (slot._dutModels.Count == 0) return;
            foreach (var dut in slot._dutModels)
            {
                DutModels.Add(MConv.ObjectDeepCopy(dut));
            }

            RowIndexs.Clear();
            for (int i = _layout._ySize; i > 0; i--)
                RowIndexs.Add(new LineIndex { Line = $"C{i}" });
            ColIndexs.Clear();
            for (int i = 0; i < _layout._xSize / _layout._MultiChip; i++)
                ColIndexs.Add(new LineIndex { Line = $"R{i + 1}" });

            setSize();
        }

        public bool _showMulti = false;
        public void selectDut()
        {
            // select result detail
            if (CurDno < 1) return;
            int dno = CurDno - 1;
            _busySub = true;
            //            Console.WriteLine($"st loadSub Busy : {_busySub}");
            loadSub(dno);
            _busySub = false;
            //          Console.WriteLine($"ed loadSub Busy : {_busySub}");

            if (_showMulti)
            {
                int mc = _layout._MultiChip;
                if (mc <= 1) return;
                if (StepModels.Count == 0) return;

                double x = Global.gMainVM.ResultWin.Left + Global.gMainVM.ResultWin.xCard.ActualWidth
                    + DutModels[dno].Width + DutModels[dno].Left;
                double y = Global.gMainVM.ResultWin.Top + DutModels[dno].Top - 350;
                MultiChipWin.Left = x;
                MultiChipWin.Top = (y > 0) ? y : 0;

                MultiChipWin.Show();
                MCDutModels.Clear();
                for (int i = 0; i < mc; i++)
                {
                    try
                    {
                        MCDutModels.Add(MConv.ObjectDeepCopy(DutModels[dno + i]));
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.Message);
                    }

                }
                setMcSize();

            }
        }

        #endregion Select / Set Process

        public void setSize()
        {
            if (DutModels.Count == 0) return;
            int mce = _layout._MultiChip;
            if (mce < 0) mce = 1;

            int xSize = _layout._xSize / mce;
            int ySize = _layout._ySize;
            double xGap = 2;
            if (xSize != 0) xGap = 40 / xSize;
            double yGap = 2;
            if (ySize != 0) yGap = 40 / ySize;

            double xOfs = 5; // Left 시작 공백
            double yOfs = 5; // Top 시작 공백

            double dutWidth = (_bibWidth / xSize);
            double dutHeight = (_bibHeight / ySize);
            RowHeight = dutHeight;
            ColWidth = dutWidth;

            for (int y = 0; y < ySize; y++)
            {
                int yPos = _layout._ySize - (y + 1);

                for (int xPos = 0; xPos < xSize; xPos++)
                {
                    int dutIdx = ((xSize * yPos) + xPos);
                    dutIdx *= mce;
                    //Console.WriteLine("dno {0}", dutIdx);
                    DutModels[dutIdx].Left = (xPos * dutWidth) + xOfs;
                    double width = dutWidth - xGap;
                    if (width < 0) width = 0;
                    DutModels[dutIdx].Width = width;
                    DutModels[dutIdx].Top = ((_layout._ySize - (yPos + 1)) * dutHeight) + yOfs;
                    double height = dutHeight - yGap;
                    if (height < 0) height = 0;
                    DutModels[dutIdx].Height = height;

                }

            }
        }


        #region Multi Chip Process 

        public double _mcWidth = 0;
        public double _mcHeight = 0;

        public MultiChip MultiChipWin;

        public void setMcDutText()
        {
            if (_curStep == null) return;
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (Global.gSlotVM.CurSlot == null) return;
            if (slot._dutModels.Count != DutModels.Count) return;
            if (MCDutModels.Count != _layout._MultiChip) return;

            for (int i = 0; i < _layout._MultiChip; i++)
            {
                int dno = MCDutModels[i].Num - 1;
                if (MCDutModels[i].Num < 0) continue;
                switch (TextMode)
                {
                    case 0: // NO TEXT
                        MCDutModels[i].Text = ""; break;
                    case 1: // NO
                        MCDutModels[i].Text = slot._dutModels[dno].Num.ToString(); break;
                    case 2:  // IO
                        MCDutModels[i].Text = slot._dutModels[dno].IO; break;
                    case 3:  // FAILBIT
                        if (slot._dutDatas[dno]._fBit.ContainsKey(_curStep.Name))
                            MCDutModels[i].Text = Convert.ToString(slot._dutDatas[dno]._fBit[_curStep.Name], 16);
                        else
                            MCDutModels[i].Text = "";
                        break;
                    case 4:  // CHIPID
                        MCDutModels[i].Text = slot._dutModels[dno].ChipID; break;
                    case 5:  // BIN
                        MCDutModels[i].Text = slot._dutModels[dno].Bin; break;


                    default: break;


                }
            }
        }

        public void setMcStep()
        {
            if (_curStep == null) return;
            SlotModel slot = Global.gSlotVM.CurSlot;
            if (Global.gSlotVM.CurSlot == null) return;
            if (MCDutModels.Count != _layout._MultiChip) return;

            for (int i = 0; i < _layout._MultiChip; i++)
            {
                int dno = MCDutModels[i].Num - 1;
                if (dno < 0) continue;

                if (TextMode == 5) {
                    int bin = 0;
                    if (MCDutModels[i].Bin != "")
                        bin = Convert.ToInt32(MCDutModels[i].Bin);

                    SolidColorBrush cb;
                    if (Global.BinColorDic.ContainsKey(bin))
                        cb = Global.BinColorDic[bin];
                    else
                        cb = new SolidColorBrush(Colors.DarkGray);

                    MCDutModels[i].Color = cb;
                }
                else {
                    if (!_curStep.IsContact) {
                        if (!slot._dutDatas[dno]._conDic[_curStep.Name]) {
                            MCDutModels[i].Color = ResColors[((int)RES_COL.CONTACT_FAIL)];
                            continue;
                        }
                    }

                    if (Total) {
                        if (slot._dutDatas[dno]._totalDic[_curStep.Name])
                            MCDutModels[i].Color = ResColors[(int)RES_COL.TOTAL_PASS];
                        else
                            MCDutModels[i].Color = ResColors[(int)RES_COL.STEP_FAIL];
                    }
                    else {
                        if (slot._dutDatas[dno]._curDic[_curStep.Name])
                            MCDutModels[i].Color = ResColors[(int)RES_COL.STEP_PASS];
                        else
                            MCDutModels[i].Color = ResColors[(int)RES_COL.STEP_FAIL];
                    }
                }
            }
        }

        public void setMcSize()
        {
            if (MCDutModels.Count == 0) return;
            //if (MCDutModels.Count != _layout._MultiChip) return;

            double dutWidth = (_mcWidth);
            double dutHeight = (_mcHeight / _layout._MultiChip);
            double yGap = 2;
            for (int y = 0; y < _layout._MultiChip; y++)
            {
                MCDutModels[y].Left = 0;
                MCDutModels[y].Width = dutWidth;
                MCDutModels[y].Top = y * dutHeight;
                MCDutModels[y].Height = (dutHeight - yGap) > 0 ? (dutHeight - yGap) : 0;
            }

            MultiChipWin.Activate();
            setMcStep();
        }
        #endregion Multi Chip Process


        #region Sub Item Process

        public void displayBlockResult(SUB_TYPE type)
        {
            if ((type == SUB_TYPE.FBLOCK || type == SUB_TYPE.BLOCK_FBIT_COUNT)
                && _blockDisplay != 0)
            {
                _blockDisplay = 0;
                ShowBlock = Visibility.Visible;

                ShowDetail = Visibility.Hidden;
                ShowBin = Visibility.Hidden;
                ShowCycle = Visibility.Hidden;
            }
            else if (type == SUB_TYPE.BLOCK_FBIT_COUNT_DETAIL && _blockDisplay != 1)
            {
                _blockDisplay = 1;
                ShowDetail = Visibility.Visible;

                ShowBin = Visibility.Hidden;
                ShowBlock = Visibility.Hidden;
                ShowCycle = Visibility.Hidden;
            }
            else if ((type == SUB_TYPE.DATA || type == SUB_TYPE.MEAS_DATA) && _blockDisplay != 2)
            {
                _blockDisplay = 2;
                ShowBin = Visibility.Visible;

                ShowDetail = Visibility.Hidden;
                ShowBlock = Visibility.Hidden;
                ShowCycle = Visibility.Hidden;
            }
            else if (type == SUB_TYPE.CYCLE && _blockDisplay != 3)
            {
                _blockDisplay = 3;
                ShowCycle = Visibility.Visible;

                ShowBin = Visibility.Visible;
                ShowDetail = Visibility.Hidden;
                ShowBlock = Visibility.Hidden;

            }

        }

        public void initSubItem()
        {
            _subBlockModels.Clear();
            SubItemCount = "";
            SubType = "";

            _subFDetailBlocks.Clear();
            _subFDetailPages.Clear();
            _subFDetailCols.Clear();

            BinText = "";
        }

        public void selectPage(int page)
        {
            _subFDetailCols.Clear();
            if (page >= 3096) return
                    ;
            string[] sbuf = _colDatas[page].Split(',');
            int colAdr = 0; int colAdd = 0x800;
            foreach (string s in sbuf)
            {
                if (s == "") break;
                SubFDetailModel sd = new SubFDetailModel();
                sd.Text = string.Format($"Column 0x{colAdr.ToString("x4")}(XADR)   : {s}");
                colAdr += colAdd;
                _subFDetailCols.Add(sd);

            }

        }

        public void addPadding(StringBuilder Build, string val)
        {
            Build.Append(val.PadLeft(6));
        }

        public void loadCsv(int dno)
        {
            SlotModel slot = Global.gSlotVM.CurSlot;
            string csvFile = string.Format($"{Global.gDirData}\\{slot.LotID}_{_curStep.Name}_{slot.Name}_D{dno.ToString("D4")}.csv");
            string[] lines;
            try
            {
                lines = File.ReadAllLines(csvFile);
            }
            catch (FileNotFoundException e)
            {
                Growl.InfoGlobal(e.Message);
                return;
            }
            StringBuilder tb = new StringBuilder();
            addPadding(tb, "Seq");
            addPadding(tb, "XH");
            addPadding(tb, "XL");
            addPadding(tb, "YH");
            addPadding(tb, "YL");
            addPadding(tb, "ZH");
            addPadding(tb, "ZL");
            addPadding(tb, "DRH");
            addPadding(tb, "DRL");
            addPadding(tb, "IOH");
            addPadding(tb, "IOL");
            addPadding(tb, "DEVH");
            addPadding(tb, "DEVL");
            tb.Append("\r\n------------------------------------------------------------------\r\n");

            int LineNo = 0;
            foreach (string line in lines)
            {
                string[] tokens = line.Split(',');
                if (tokens.Length < 12) continue;
                addPadding(tb, (LineNo++).ToString());
                for (int i = 0; i<12; i++)
                {
                    addPadding(tb, tokens[i]);
                }
                tb.Append("\r\n");
            }
            BinText = tb.ToString();
        }


        public void loadBin(int dno)
        {
            SlotModel slot = Global.gSlotVM.CurSlot;
            string binFile = string.Format($"{Global.gDirData}\\{slot.LotID}_{_curStep.Name}_{slot.Name}_D{dno.ToString("D4")}.bin");
            FileStream fsr;
            BinaryReader br;
            try
            {
                fsr = new FileStream(binFile, FileMode.Open, FileAccess.Read);
                br = new BinaryReader(fsr);
            }
            catch (FileNotFoundException e)
            {
                Growl.InfoGlobal(e.Message);
                return;
            }

            int length = (int)fsr.Length;
            byte[] buffer = br.ReadBytes(length);
            //            BinText = BitConverter.ToString(buffer);
            StringBuilder textBuild = new StringBuilder();
            for (int i = 0; i< length; i++)
            {
                if (i % 16 == 0)
                {
                    if (i != 0) textBuild.Append("\r\n");
                    textBuild.Append(i.ToString("D5") + " : ");
                }
                textBuild.Append(buffer[i].ToString("X2") + " ");
                if (i > 1024 * 100) // 100k
                {
                    textBuild.Append("\r\n  ..............  \r\n");
                    textBuild.Append("File Size Too Big. Verify File To Hex Viewer");
                    break;
                }                
            }
            BinText = textBuild.ToString();

            fsr.Close();
            br.Close();
        }

        public void loadCycle()
        {

            SlotModel slot = Global.gSlotVM.CurSlot;
            string csvFile = string.Format($"{Global.gDirSub}\\{slot.LotID}_{_curStep.Name}_{slot.Name}.csv");
            try
            {
                var stream = new FileStream(csvFile, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);

                using (var reader = ExcelReaderFactory.CreateCsvReader(stream))
                {
                    var result = reader.AsDataSet(new ExcelDataSetConfiguration()
                    {
                        ConfigureDataTable = (_) => new ExcelDataTableConfiguration()
                        {
                            EmptyColumnNamePrefix = "C",
                            UseHeaderRow = true
                        }
                    });

                    DT_Cycle = result.Tables[0];
                }
            }
            catch (Exception ex)
            {
                Growl.InfoGlobal(ex.Message);
            }

        }

        public bool _busySub = false;
        public int _blockAdr = -1;
        public void loadSub(int dno)
        {

            initSubItem();

            if (_curStep == null) return;
            if (Global.gSlotVM.CurSlot == null) return;
            SlotModel slot = Global.gSlotVM.CurSlot;

            //            Console.WriteLine(String.Format($"{slot.Name} {_curStep.Name} {_curStep._subType.ToString()}"));
            displayBlockResult(_curStep._subType);
            SubType = _curStep._subType.ToString();
            if (_curStep._subType == SUB_TYPE.NONE ) return;            
            if (slot._dutDatas[dno]._conDic[_curStep.Name] == false) return;
            if (_curStep._subType == SUB_TYPE.CYCLE) return;            
            if ( _curStep._subType == SUB_TYPE.DATA )
            {
                loadBin(dno);return;
            }
            if (_curStep._subType == SUB_TYPE.MEAS_DATA)
            {
                loadCsv(dno); return;
            }

            string subFile = string.Format($"{Global.gDirSub}\\{slot.LotID}_{_curStep.Name}_{slot.Name}.xml");
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load(subFile);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }

            var node = doc.SelectSingleNode("/sub");
            if (node == null) return;
            SUB_TYPE type = StepModel.findSubType(node.Attributes["type"].Value);
            if (type == SUB_TYPE.FBLOCK || type == SUB_TYPE.BLOCK_FBIT_COUNT)
            {
                
                if (type == SUB_TYPE.FBLOCK)
                    node = doc.SelectSingleNode("/sub/blk");
                else
                    node = doc.SelectSingleNode("/sub/fc");
                if (node == null) return;


                string str = string.Format($"//d[@n='{dno}']");
                
                var dutNode = node.SelectSingleNode(str) ;
                if (dutNode == null) return;

                SubItemCount = dutNode.Attributes["c"].Value;
                string data = dutNode.InnerText;

                if (data == "") return;
                string[] blks = data.Split(' ');
                if (blks.Length == 0) return; 
                foreach (string blk in blks)
                {
                    if (blk == "") break;
                    SubBLockModel sub = new SubBLockModel();

                    if (blk.Substring(0, 1) == "!")  // NEW
                    {
                        sub.Type = 1;
                        sub.Block = blk.Substring(1, 4);
                    }else if (blk.Substring(0, 1) == ".") // MASK
                    {
                        sub.Type = 2;
                        sub.Block = blk.Substring(1, 4);
                    }
                    else if (blk.Substring(0, 1) == "*") // MASK new
                    {
                        sub.Type = 3;
                        sub.Block = blk.Substring(1, 4);
                    }
                    else                   // Existing
                        sub.Block = blk;
                    SubBlockModels.Add(sub);
                }
            }
            if (type == SUB_TYPE.BLOCK_FBIT_COUNT_DETAIL)
            {
                
                node = doc.SelectSingleNode("/sub/fc-d");
                if (node == null) return; 
                var dlists = node.ChildNodes;

                if (dlists.Count == 0) return; 
                string str = string.Format($"//d[@n='{dno}']");
                var dutNode = node.SelectSingleNode(str);
                if (dutNode == null) return; 
                var bList = dutNode.ChildNodes;
                XmlNode bh = null;
                for (int i = 0; i < bList.Count; i++)
                {
                    bh = bList[i];
                    SubFDetailModel sd = new SubFDetailModel();
                    sd.No = Convert.ToInt32(bh.Attributes["a"].Value);
                    sd.Text = $"Block {sd.No}";
                    SubFDetailBlocks.Add(sd);
                }
                
                if (_blockAdr == -1)
                    bh = dutNode.FirstChild;
                else                
                    bh = dutNode.SelectSingleNode($"//b[@a='{_blockAdr}']");

                if (bh == null) return; 
                var pList = bh.ChildNodes;
                XmlNode ph = null;
                for (int i = 0;i < pList.Count; i++)
                {
                    ph = pList[i];
                    SubFDetailModel sd = new SubFDetailModel();
                    sd.No = Convert.ToInt32(ph.Attributes["n"].Value);
                    if (i < 3096) _colDatas[sd.No] = ph.InnerText;

                    string[] sbuf = ph.InnerText.Split(',');
                    int sum = 0;
                    foreach (string s in sbuf)
                    {
                        if (s == "") break;
                        sum += Convert.ToInt32(s);
                    }

                    sd.Text = string.Format($"Page {sd.No}\t\tCount : {sum}");
                    SubFDetailPages.Add(sd);
                }

            }
        }

        #endregion SUB Item Process

        #region Constructor

        public ResultVM()
        {
            Global.gResultVM = this;

            ResColors.Add(new SolidColorBrush(Colors.DarkGray));       // CONTACT_FAIL = 0,
            ResColors.Add(new SolidColorBrush(Colors.DarkSeaGreen));   // TOTAL_PASS = 1,
            ResColors.Add(new SolidColorBrush(Colors.ForestGreen));    // STEP_PASS = 2,
            ResColors.Add(new SolidColorBrush(Colors.DarkGreen));      // STEP_PASS_CON_SOME = 3,
            ResColors.Add(new SolidColorBrush(Colors.Goldenrod));      // STEP_SOME = 4,
            ResColors.Add(new SolidColorBrush(Colors.DarkGoldenrod));  // STEP_SOME_CON_SOME = 5,
            ResColors.Add(new SolidColorBrush(Colors.IndianRed));      // STEP_FAIL = 6,
            ResColors.Add(new SolidColorBrush(Colors.DarkRed));        // STEP_FAIL_CON_SOME = 7,

            MultiChipWin = new MultiChip();
            MultiChipWin.Hide();
            MultiChipWin.DataContext = this;

            displayBlockResult(SUB_TYPE.FBLOCK);        
        }

        #endregion
    }
}
