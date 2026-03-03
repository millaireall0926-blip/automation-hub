using ScintillaNET;
using ScintillaNET_FindReplaceDialog;
using StarMaint.Common;
using StarMaint.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using System.Xml;
using System.Windows.Forms;

namespace StarMaint.ViewModel
{

    public partial class MainVM
    {
        public Scintilla _scintilla;

        private const int BACK_COLOR = 0x2A211C;
        private const int FORE_COLOR = 0xB7B7B7;

        private const int NUMBER_MARGIN = 1;
        private const int BOOKMARK_MARGIN = 2;
        private const int BOOKMARK_MARKER = 2;
        private const int FOLDING_MARGIN = 3;
        private const int LINE_NUMBERS_MARGIN_WIDTH = 40; // TODO - don't hardcode this

        private const bool CODEFOLDING_CIRCULAR = true;

        private FindReplace MyFindReplace;

        const string TMP_FILE = "maint.tmp";


        public Loading loadingWin;
        public void initSci()
        {
            MyFindReplace = new FindReplace();
            MyFindReplace.KeyPressed += MyFindReplace_KeyPressed;
            MyFindReplace.Scintilla = _scintilla;
            _scintilla.Zoom = 1;

            initColor();
            setCPPKeyword();
            _scintilla.KeyDown += keyDownSci;

            initFileResource();


            loadingWin = new Loading();


        }

        public void setCPPKeyword()
        {
            // Configure the CPP (C#) lexer styles
            _scintilla.Styles[ScintillaNET.Style.Cpp.Identifier].ForeColor = IntToColor(0xD0DAE2);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Comment].ForeColor = IntToColor(0xBD758B);
            _scintilla.Styles[ScintillaNET.Style.Cpp.CommentLine].ForeColor = IntToColor(0x40BF57);
            _scintilla.Styles[ScintillaNET.Style.Cpp.CommentDoc].ForeColor = IntToColor(0x2FAE35);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Number].ForeColor = IntToColor(0xFFFF00);
            _scintilla.Styles[ScintillaNET.Style.Cpp.String].ForeColor = IntToColor(0xFFFF00);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Character].ForeColor = IntToColor(0xE95454);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Preprocessor].ForeColor = IntToColor(0x8AAFEE);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Operator].ForeColor = IntToColor(0xE0E0E0);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Regex].ForeColor = IntToColor(0xff00ff);
            _scintilla.Styles[ScintillaNET.Style.Cpp.CommentLineDoc].ForeColor = IntToColor(0x77A7DB);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Word].ForeColor = IntToColor(0x48A8EE);
            _scintilla.Styles[ScintillaNET.Style.Cpp.Word2].ForeColor = IntToColor(0xF98906);
            _scintilla.Styles[ScintillaNET.Style.Cpp.CommentDocKeyword].ForeColor = IntToColor(0xB3D991);
            _scintilla.Styles[ScintillaNET.Style.Cpp.CommentDocKeywordError].ForeColor = IntToColor(0xFF0000);
            _scintilla.Styles[ScintillaNET.Style.Cpp.GlobalClass].ForeColor = IntToColor(0x48A8EE);

            _scintilla.Lexer = Lexer.Cpp;

            _scintilla.SetKeywords(0, "class extends implements import interface new case do while else if for in switch throw get set function var try catch finally while with default break continue delete return each const namespace package include use is as instanceof typeof author copy default deprecated eventType example exampleText exception haxe inheritDoc internal link mtasc mxmlc param private return see serial serialData serialField since throws usage version langversion playerversion productversion dynamic private public partial static intrinsic internal native override protected AS3 final super this arguments null Infinity NaN undefined true false abstract as base bool break by byte case catch char checked class const continue decimal default delegate do double descending explicit event extern else enum false finally fixed float for foreach from goto group if implicit in int interface internal into is lock long new null namespace object operator out override orderby params private protected public readonly ref return switch struct sbyte sealed short sizeof stackalloc static string select this throw true try typeof uint ulong unchecked unsafe ushort using var virtual volatile void while where yield");
            _scintilla.SetKeywords(1, "void Null ArgumentError arguments Array Boolean Class Date DefinitionError Error EvalError Function int Math Namespace Number Object RangeError ReferenceError RegExp SecurityError String SyntaxError TypeError uint XML XMLList Boolean Byte Char DateTime Decimal Double Int16 Int32 Int64 IntPtr SByte Single UInt16 UInt32 UInt64 UIntPtr Void Path File System Windows Forms _scintilla");

        }
        public void initColor()
        {
            /// initColor ///
            _scintilla.CaretForeColor = IntToColor(0xFFFFFF);
            _scintilla.SetSelectionBackColor(true, IntToColor(0x114D9C));

            /// Syntext Color /// 
            _scintilla.StyleResetDefault();
            _scintilla.Styles[ScintillaNET.Style.Default].Font = "Consolas";
            _scintilla.Styles[ScintillaNET.Style.Default].Size = 9;
            _scintilla.Styles[ScintillaNET.Style.Default].BackColor = IntToColor(0x212121);
            _scintilla.Styles[ScintillaNET.Style.Default].ForeColor = IntToColor(0xFFFFFF);
            _scintilla.StyleClearAll();


            /// NumberMargin
            _scintilla.Styles[ScintillaNET.Style.LineNumber].BackColor = IntToColor(BACK_COLOR);
            _scintilla.Styles[ScintillaNET.Style.LineNumber].ForeColor = IntToColor(FORE_COLOR);
            _scintilla.Styles[ScintillaNET.Style.IndentGuide].ForeColor = IntToColor(BACK_COLOR);
            _scintilla.Styles[ScintillaNET.Style.IndentGuide].BackColor = IntToColor(FORE_COLOR);

            var nums = _scintilla.Margins[NUMBER_MARGIN];
            nums.Width = LINE_NUMBERS_MARGIN_WIDTH;
            nums.Type = MarginType.Number;
            nums.Sensitive = true;
            nums.Mask = 0;


            /// BookMarkMargin
            var margin = _scintilla.Margins[BOOKMARK_MARGIN];
            margin.Width = 0;
            margin.Sensitive = true;
            margin.Type = MarginType.Symbol;
            margin.Mask = (1 << BOOKMARK_MARKER);

            var marker = _scintilla.Markers[BOOKMARK_MARKER];
            marker.Symbol = MarkerSymbol.Circle;
            marker.SetBackColor(IntToColor(0x30303B));
            marker.SetForeColor(IntToColor(0x000000));
            marker.SetAlpha(100);

            /// CodeFolding
            _scintilla.SetFoldMarginColor(true, IntToColor(BACK_COLOR));
            _scintilla.SetFoldMarginHighlightColor(true, IntToColor(BACK_COLOR));

            // Enable code folding
            _scintilla.SetProperty("fold", "1");
            _scintilla.SetProperty("fold.compact", "1");

            // Configure a margin to display folding symbols
            _scintilla.Margins[FOLDING_MARGIN].Type = MarginType.Symbol;
            _scintilla.Margins[FOLDING_MARGIN].Mask = Marker.MaskFolders;
            _scintilla.Margins[FOLDING_MARGIN].Sensitive = true;
            _scintilla.Margins[FOLDING_MARGIN].Width = 9;

            // Set colors for all folding markers
            for (int i = 25; i <= 31; i++)
            {
                _scintilla.Markers[i].SetForeColor(IntToColor(BACK_COLOR)); // styles for [+] and [-]
                _scintilla.Markers[i].SetBackColor(IntToColor(FORE_COLOR)); // styles for [+] and [-]
            }

            // Configure folding markers with respective symbols
            _scintilla.Markers[Marker.Folder].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CirclePlus : MarkerSymbol.BoxPlus;
            _scintilla.Markers[Marker.FolderOpen].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CircleMinus : MarkerSymbol.BoxMinus;
            _scintilla.Markers[Marker.FolderEnd].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CirclePlusConnected : MarkerSymbol.BoxPlusConnected;
            _scintilla.Markers[Marker.FolderMidTail].Symbol = MarkerSymbol.TCorner;
            _scintilla.Markers[Marker.FolderOpenMid].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CircleMinusConnected : MarkerSymbol.BoxMinusConnected;
            _scintilla.Markers[Marker.FolderSub].Symbol = MarkerSymbol.VLine;
            _scintilla.Markers[Marker.FolderTail].Symbol = MarkerSymbol.LCorner;

            // Enable automatic folding
            _scintilla.AutomaticFold = (AutomaticFold.Show | AutomaticFold.Click | AutomaticFold.Change);


        }

        public static System.Drawing.Color IntToColor(int rgb)
        {
            return System.Drawing.Color.FromArgb(255, (byte)(rgb >> 16), (byte)(rgb >> 8), (byte)rgb);
        }

        public static System.Windows.Media.Color IntToMediaColor(int rgb)
        {
            return System.Windows.Media.Color.FromArgb(255, (byte)(rgb >> 16), (byte)(rgb >> 8), (byte)rgb);
        }

        private void MyFindReplace_KeyPressed(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            keyDownSci(sender, e);
        }

        void keyDownSci(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.F)
            {
                MyFindReplace.ShowFind();
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.S)
            {
                save();
                e.SuppressKeyPress = true;
            }
            else if (e.Shift && e.KeyCode == Keys.F3)
            {
                MyFindReplace.Window.FindPrevious();
                e.SuppressKeyPress = true;
            }
            else if (e.KeyCode == Keys.F3)
            {
                MyFindReplace.Window.FindNext();
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.H)
            {
                MyFindReplace.ShowReplace();
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.I)
            {
                MyFindReplace.ShowIncrementalSearch();
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.G)
            {
                GoTo MyGoTo = new GoTo((Scintilla)sender);
                MyGoTo.ShowGoToDialog();
                e.SuppressKeyPress = true;
            } else
            {
                IsModify = true;
            }

                        TextArea_KeyDown(sender, e);
        }

        bool _isCap = false;

        class SelWrap
        {
            public int Begin { get; set; }
            public int Length { get; set; }
        }


        string convString(Keys key, bool shift)
        {
            bool cap = Control.IsKeyLocked(Keys.CapsLock);
            bool upper = cap;
            if (shift) upper = !upper;

                string ret = "";
            if (key >= Keys.A && key <= Keys.Z)
            {
                if (upper)
                    ret = Convert.ToChar((int) key).ToString();
                else
                    ret = Convert.ToChar((int) (key + 0x20)).ToString();
                return ret;
            }

            if (key >= Keys.NumPad0 && key <= Keys.NumPad9)
                key -= 0x30;
            if (key >= Keys.D0 && key <= Keys.D9)  // Number
            {
                if (shift)
                {
                    switch (key)
                    {
                        case Keys.D0: ret = ")"; break;
                        case Keys.D1: ret = "!"; break;
                        case Keys.D2: ret = "@"; break;
                        case Keys.D3: ret = "#"; break;
                        case Keys.D4: ret = "$"; break;
                        case Keys.D5: ret = "%"; break;
                        case Keys.D6: ret = "^"; break;
                        case Keys.D7: ret = "&"; break;
                        case Keys.D8: ret = "*"; break;
                        case Keys.D9: ret = "("; break;
                    }
                } else
                    ret = Convert.ToChar((int)key).ToString();
            } else if (key == Keys.Space || key == Keys.Tab) {
                ret = Convert.ToChar((int)key).ToString();
            } else  // spacail charictor
            {
                if (shift)
                {
                    switch (key)
                    {
                        case Keys.Oemtilde: ret = "~"; break;
                        case Keys.OemMinus: ret = "_"; break;
                        case Keys.Oemplus: ret = "+"; break;
                        case Keys.OemOpenBrackets: ret = "{"; break;
                        case Keys.Oem6: ret = "}"; break;
                        case Keys.Oem5: ret = "|"; break;
                        case Keys.Oem1: ret = ":"; break;
                        case Keys.Oem7: ret = "\""; break;
                        case Keys.Oemcomma: ret = "<"; break;
                        case Keys.OemPeriod: ret = ">"; break;
                        case Keys.OemQuestion: ret = "?"; break;
                    }
                }else
                {
                    switch (key)
                    {
                        case Keys.Oemtilde: ret = "`"; break;
                        case Keys.OemMinus: ret = "-"; break;
                        case Keys.Oemplus: ret = "="; break;
                        case Keys.OemOpenBrackets: ret = "["; break;
                        case Keys.Oem6: ret = "]"; break;
                        case Keys.Oem5: ret = "\\"; break;
                        case Keys.Oem1: ret = ";"; break;
                        case Keys.Oem7: ret = "'"; break;
                        case Keys.Oemcomma: ret = ","; break;
                        case Keys.OemPeriod: ret = "."; break;
                        case Keys.OemQuestion: ret = "/"; break;
                    }
                }
            }
            
            return ret;
        }


        private void TextArea_KeyDown(object sendor, System.Windows.Forms.KeyEventArgs e)
        {
            var edit = _scintilla;
            if (edit.Selections.Count < 2) return; // no in column mode
            
            if (e.KeyCode >= Keys.PageUp && e.KeyCode <= Keys.Down) return;
            
            Console.WriteLine("Down Key: " + e.KeyCode.ToString() + "-> char:" + Convert.ToChar(e.KeyValue));

            if (e.KeyCode == Keys.Enter) return;
            if (e.KeyCode == Keys.CapsLock) return;
            if (
                    !(  (e.KeyCode >= Keys.A && e.KeyCode <= Keys.Z) ||
                        (e.KeyCode >= Keys.NumPad0 && e.KeyCode <= Keys.NumPad9) || 
                        (e.KeyCode >= Keys.D0 && e.KeyCode <= Keys.D9) ||
                        
                        e.KeyCode == Keys.Space ||
                        e.KeyCode == Keys.Tab ||
                        e.KeyCode == Keys.Back ||
                        e.KeyCode == Keys.Delete ||

                        e.KeyCode == Keys.Oemtilde ||
                        e.KeyCode == Keys.OemMinus ||
                        e.KeyCode == Keys.Oemplus ||
                        e.KeyCode == Keys.OemOpenBrackets ||
                        e.KeyCode == Keys.Oem6 ||
                        e.KeyCode == Keys.Oem5 ||
                        e.KeyCode == Keys.Oem1 ||
                        e.KeyCode == Keys.Oem7 ||
                        e.KeyCode == Keys.Oemcomma ||
                        e.KeyCode == Keys.OemPeriod ||
                        e.KeyCode == Keys.OemQuestion

                    )
                ) return;

            if (e.Control && e.KeyCode == Keys.C) return;
            if (e.Control && e.KeyCode == Keys.V) return;

            e.SuppressKeyPress = true;



            var array = edit.Selections
                .OrderBy(Selection => Selection.Start)
                .Select(Selection => new SelWrap { Begin = Selection.Start, Length = Selection.End - Selection.Start })
                .ToArray();

            for (var i = 0; i < array.Length; i++)
            {
                var lineInfo = array[i];
                if (lineInfo.Length > 0)
                {
                    edit.DeleteRange(lineInfo.Begin, lineInfo.Length);
                    for (var j = i + 1; j < array.Length; j++)                    
                        array[j].Begin -= lineInfo.Length;                    
                }

                if (e.KeyCode == Keys.Back)
                {
                    if (lineInfo.Length != 0) continue;
                    edit.DeleteRange(lineInfo.Begin - 1, 1);
                    for (var j = i; j < array.Length; j++)
                        array[j].Begin--;
                } 
                else if (e.KeyCode == Keys.Delete)
                {
                    if (lineInfo.Length != 0) continue;
                    edit.DeleteRange(lineInfo.Begin, 1);
                    for (var j = i + 1; j < array.Length; j++)
                        array[j].Begin--;
                }
                else
                {
                    try
                    {
                        edit.InsertText(lineInfo.Begin, convString(e.KeyCode, e.Shift));
                        for (var j = i; j < array.Length; j++)
                            array[j].Begin++;
                    }
                    catch { }
                }

            }


            edit.ClearSelections();
            
            edit.SetSelection(array[0].Begin, array[0].Begin);
            for (var i = 1; i < array.Length; i++)
            {
                var item = array[i];
                edit.AddSelection(item.Begin, item.Begin);
            }

            //            edit.SetSelection(array[0].Begin, array[0].Begin);
            //            edit.MultipleSelection = true;
            //            edit.AdditionalSelectionTyping = true;
            //            edit.AdditionalCaretsBlink = true;
            //            edit.AdditionalCaretsVisible = true;

        }



        public void clickToolbar(object sender, RoutedEventArgs e)
        {
            System.Windows.Controls.Button button = ((System.Windows.Controls.Button)e.Source);
            string tooltip = button.ToolTip.ToString();

            if (tooltip.IndexOf("Save") != -1)
            {
                save();
            }
            else if (tooltip.IndexOf("Cut") != -1)
            {
                _scintilla.Cut();
            }
            else if (tooltip.IndexOf("Copy") != -1)
            {
                _scintilla.Copy();
            }
            else if (tooltip.IndexOf("Paste") != -1)
            {
                _scintilla.Paste();
            }
            else if (tooltip.IndexOf("Undo") != -1)
            {
                _scintilla.Undo();
            }
            else if (tooltip.IndexOf("Redo") != -1)
            {
                _scintilla.Redo();
            }
        }

        void save()
        {
            try
            {
                string filename = Directory.GetCurrentDirectory() + $@"\{TMP_FILE}";
                File.Delete(filename);
                FileStream fs = File.Create(filename);
                BinaryWriter bw = new BinaryWriter(fs);
                bw.Write(_scintilla.Text.ToCharArray(), 0, _scintilla.Text.Length - 1); // Omit trailing NULL
                bw.Close();
                IsModify = false;
                _scintilla.SetSavePoint();


                _sendProcess.start(_selSlot, _selFile, filename);

                //_client.sendFile(_selSlot, "", _selFile, data);
                loadingWin.Show();
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("File Aceess Error. Try Again");
            }


            
        }

         
        /// <summary>
        /// Scintilla 외 
        /// 
        /// </summary>
        /// 

        bool _isModify = false;
        bool IsModify
        {
            get { return _isModify; }
            set {
                if (value == true && _isModify == false)
                    ModifyFile += "*";
                if (value == false && _isModify == true)
                    ModifyFile = ModifyFile.Substring(0, ModifyFile.Length - 1);
                _isModify = value; 

            }
        }

        string _modifyFile = "";
        public string ModifyFile
        {
            get { return _modifyFile; }
            set
            {
                _modifyFile = value;
                OnPropertyChanged(nameof(ModifyFile));
            }
        }

        string _selFolder;
        string _selFile;

        public Collection<BitmapImage> FileBmArr;
        public Collection<BitmapImage> FolderBmArr;

        ObservableCollection<FileModel> _files = new ObservableCollection<FileModel>();
        public ObservableCollection<FileModel> Files
        {
            get { return _files; }
            set { _files = value; OnPropertyChanged(nameof(Files)); }
        }
        ObservableCollection<FolderModel> _folders = new ObservableCollection<FolderModel>();
        public ObservableCollection<FolderModel> Folders
        {
            get { return _folders; }
            set { _folders = value; OnPropertyChanged(nameof(Files)); }
        }

        void initFileResource()
        {
            FileBmArr = new Collection<BitmapImage>();
            FileBmArr.Add(new BitmapImage(new Uri(@"/Resources/fileNormal.png", UriKind.RelativeOrAbsolute)));
            FileBmArr.Add(new BitmapImage(new Uri(@"/Resources/fileXml.png", UriKind.RelativeOrAbsolute)));

            FolderBmArr = new Collection<BitmapImage>();
            FolderBmArr.Add(new BitmapImage(new Uri(@"/Resources/folderClose.png", UriKind.RelativeOrAbsolute)));
            FolderBmArr.Add(new BitmapImage(new Uri(@"/Resources/folderOpen.png", UriKind.RelativeOrAbsolute)));
            
            Folders.Add(new FolderModel { Name = "StarAPI", Icon = FolderBmArr[0] });
            Folders.Add(new FolderModel { Name = "test",    Icon = FolderBmArr[0] });
            Folders.Add(new FolderModel { Name = "result",  Icon = FolderBmArr[0] });
            Folders.Add(new FolderModel { Name = "diag",    Icon = FolderBmArr[0] });
            Folders.Add(new FolderModel { Name = "diag/cal",Icon = FolderBmArr[0] });
            Folders.Add(new FolderModel { Name = "diag/log",Icon = FolderBmArr[0] });            

        }


        
        public void selectFolder(object obj)
        {
            if (obj is FolderModel)
            {
                foreach (FolderModel elem in Folders)
                {
                    elem.Icon = FolderBmArr[0];
                }               FolderModel fm = obj as FolderModel;
                fm.Icon = FolderBmArr[1];
                _selFolder = string.Format("/star/{0}", fm.Name);
                sendReqFileList();
                Files.Clear();
            }
            
            if (obj is FileModel)
            {                
                FileModel fm = obj as FileModel;
                _selFile = _selFolder + "/" + fm.Name;
                ModifyFile = _selSlot+ " - " + _selFile;
                _client.sendReqFile(_selSlot, _selFile, Directory.GetCurrentDirectory() + $@"\{TMP_FILE}");
                loadingWin.Show();
            }

        }

        public void sendReqFileList()
        {
            if (_selSlot == "") return;

            string xml = string.Format("<ctrl-cm cmd=\"sh ls -1lp {0}\">", _selFolder);
            _client.sendCmd((int)MSG_TYPE.DEST_DAEMON, _selSlot, "", xml);
        }

        public void setSciText(string text, bool isAdd)
        {
            if (isAdd)
                System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                        new Action(() => _scintilla.Text += text));
            else
                System.Windows.Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                        new Action(() => _scintilla.Text = text));
        }
    }




}
