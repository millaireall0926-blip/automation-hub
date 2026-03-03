using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ScintillaNET;
using StarBase;
using Xceed.Wpf.AvalonDock.Layout;
using ScintillaNET_FindReplaceDialog;
using System.Windows.Forms;
using System.Windows;
using System.Text.RegularExpressions;
using StarIDE.View;
using System.Windows.Input;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Window;

namespace StarIDE.Model
{
    public class PrevPosition
    {
        public int _pos;
        public FileModel _file;
        public PrevPosition(FileModel fm, int pos)
        {
            _file = fm;
            _pos = pos;
        }
    }

    public class EditModel : BaseVM
    {
        public Scintilla _sci;
        public FileModel _file;      // key?                
        public LayoutDocument _pane;
        public bool _isModify = false;
        public bool _isBin = false;


        private const int BACK_COLOR = 0x212121;//0x2A211C;
        private const int FORE_COLOR = 0xB7B7B7;

        private const int NUMBER_MARGIN = 1;
        private const int BOOKMARK_MARGIN = 2;
        private const int BOOKMARK_MARKER = 2;
        private const int FOLDING_MARGIN = 3;
        private const int LINE_NUMBERS_MARGIN_WIDTH = 40; // TODO - don't hardcode this

        private const bool CODEFOLDING_CIRCULAR = true;

        public FindView _findView;
//        private FindReplace MyFindReplace;
        private GoTo MyGoTo;

        public bool IsModified
        {
            get { return _isModify; }
            set {
                if (_isBin) { _isModify = false; return; }

                bool old = _isModify;
                _isModify = value;
                if (old == value) return;
                if (_pane != null)
                {
                    if (_isModify == true)
                        _pane.Title = _file.Name + "*";
                    else
                        _pane.Title = _file.Name;

                }
            }
        }

        public EditModel(FileModel fm, LayoutDocument ld)
        {
            _sci = new Scintilla();
//            MyFindReplace = new FindReplace();
//            MyFindReplace.KeyPressed += MyFindReplace_KeyPressed;
//            MyFindReplace.Scintilla = _sci;
            
            MyGoTo = new GoTo(_sci);


            _sci.KeyDown += keyDownSci;
            _sci.CharAdded += scintilla_CharAdded;  // AutoComplete 용
            _sci.InsertCheck += scintilla_InsertCheck;
            _sci.AutoCCompleted += scintilla_AutoComped;


            initColor();

            _file = fm;
            _pane = ld;
            _file._edit = this;
            setType();

            if (_file.Type != FILE_TYPE.VEC)
            {
                openFile();
            }
            else
            {
                _isBin = checkBin();
                if (_isBin)
                    openFileBin();
                else
                    openFile();

            }
        }

        int _autoCompLen = 0;
        void scintilla_AutoComped(object sender, AutoCSelectionEventArgs e)
        {
            if (_autoCompLen == 0) return;
            _sci.DeleteRange(_sci.CurrentPosition - e.Text.Length, _autoCompLen);
        }

        void cppCharAdded(object sender, ScintillaNET.CharAddedEventArgs e)
        {            
            if (e.Char == '}')
            {
                var currentPos = _sci.CurrentPosition;
                string line = _sci.Lines[_sci.CurrentLine].Text;
                Console.WriteLine("}add : " + line);    
                _sci.SearchFlags = SearchFlags.None;

                // Search back from the current position
                _sci.TargetStart = currentPos;
                _sci.TargetEnd = 0;

                // Is the bracket following 4 spaces or a tab?
                if (line.IndexOf("    }") != -1)
                {
                    // Delete the leading 4 spaces
                    _sci.DeleteRange((currentPos - 5), 4);
                }
                else if (line.IndexOf("\t}") != -1)
                {
                    // Delete the leading tab
                    _sci.DeleteRange((currentPos - 2), 1);
                }
            }
        }

        void scintilla_CharAdded(object sender, ScintillaNET.CharAddedEventArgs e)
        {
            //string list = "aa bb cc dd ";

            IsModified = true;
            if (_file.Type == FILE_TYPE.CPP)
            {
                cppCharAdded(sender, e);
                //                int nStartSel = _sci.SelectionStart;
                //                int nEndSel = 4;
                string range = _sci.GetWordFromPosition(_sci.CurrentPosition);
                //                string range = _sci.GetTextRange(nStartSel, nEndSel);
                //                Console.WriteLine($"-{range}- \n");
                if (Global.gAPIAutoComp.ContainsKey(range))
                {
                    _autoCompLen = range.Length;
                    _sci.AutoCShow(0, Global.gAPIAutoComp[range]);
                }
            }
            else if (_file.Type == FILE_TYPE.PAT)
            {
                string range = _sci.GetWordFromPosition(_sci.CurrentPosition);
                if (Global.gAssistVM.PATAutoComp.ContainsKey(range))
                {
                    _autoCompLen = range.Length;
                    _sci.AutoCShow(0, Global.gAssistVM.PATAutoComp[range]);
                }
            }
        }

        public void addPrev(int pos)
        {
            if (Global.gDockVM._prevPositions.Count > 100 )
            {
                Global.gDockVM._prevPositions.RemoveAt(0);
            }

            Global.gDockVM._prevPositions.Add(new PrevPosition(_file, pos));
        }
        

        public void gotoPrev()
        {
            if (Global.gDockVM._prevPositions.Count == 0) return;
            PrevPosition p = Global.gDockVM._prevPositions.LastOrDefault();
            
            Global.gDockVM.addDocPane(p._file);                
            p._file._edit._sci.GotoPosition(p._pos);

            
            Global.gDockVM._prevPositions.Remove(p);
        }

        public int doFind(MatchCollection mats,int st, int ed,  bool isBackWord, bool isReplace = false)
        {
            int pos = -1;
            
            if (isBackWord) {
                for (int i = mats.Count -1; i >= 0; i--)
                {
                    Match mat = mats[i];
                    if (mat.Index < st)
                    {
                        if (isReplace)
                        {
                            Regex rgx = new Regex(Global.gDockVM.FindText, RegexOptions.None);
                            _sci.Text = rgx.Replace(_sci.Text, Global.gDockVM.ReplaceText, 1, mat.Index);
                        }
                        pos = mat.Index; break;
                    }
                    else if (mat.Index < ed)
                        break;
                }
            }
            else
            {
                for (int i = 0; i < mats.Count; i++)
                {
                    Match mat = mats[i];
                    if (mat.Index > st)
                    {                                        
                        if (isReplace)
                        {
                            Regex rgx = new Regex(Global.gDockVM.FindText, RegexOptions.None);
                            _sci.Text = rgx.Replace(_sci.Text, Global.gDockVM.ReplaceText, 1, mat.Index);                                   
                        }

                        pos = mat.Index;

                        break;
                    }
                    else if (mat.Index > ed)
                        break;
                }
            }

            return pos;
        }

        int _findAllMatchCount = 0;
        public void doFindFile(string word, FileModel fm)
        {
            FileStream fs = new FileStream(fm.Path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            StreamReader reader = new StreamReader(fs);
            string Text = reader.ReadToEnd();
            reader.Close();
            fs.Close();

            int orgCnt = _findAllMatchCount;

            RegexOptions option = RegexOptions.IgnoreCase;
            if (Global.gDockVM.IsUpperLower) option = RegexOptions.None;

            var lines = Text.Split(new[] { Environment.NewLine }, StringSplitOptions.None);
            for (int i = 0; i < lines.Length; i++)
            {
                foreach (Match m in Regex.Matches(lines[i], word, option))
                {
                    string msg = $"{fm.Path}({i}) : {lines[i]}";
                    Global.gDockVM.addReportResult(REPORT_TYPE.INFO, msg, i, fm);
                    _findAllMatchCount++;
                }
            }


            if (Global.gDockVM.ReplaceVisible == Visibility.Visible)
            {
                if (orgCnt == _findAllMatchCount) return;
                fs = new FileStream(fm.Path, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
                StreamWriter writer = new StreamWriter(fs);

                Text = Regex.Replace(Text, word, Global.gDockVM.ReplaceText, option);

                if (fm._edit != null)
                {
                    fm._edit._sci.Text = Text;
                }

                writer.Write(Text);
                writer.Flush();
                writer.Close();
                fs.Close();

                if (fm._edit != null)
                {
                    fs = new FileStream(fm.Path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                    reader = new StreamReader(fs);
                    _sci.Text = reader.ReadToEnd();
                    reader.Close();
                    fs.Close() ;
                }                
            }
            
            


        }


        public void doFindFolder(string word, FileModel folderFM)
        {
            if (folderFM.Type != FILE_TYPE.FOLDER) return;
            foreach (FileModel fm in folderFM.InnerFiles)
            {
                if (fm.Type != FILE_TYPE.HEAD) continue;  // Folder Depth 1단계 까지만
                doFindFile(word, fm);
            }
            return;
        }

        public void doFindAll(string word)
        {
            Global.gDockVM.clearReport();
            _findAllMatchCount = 0;
            foreach (FileModel fm in Global.gFilesVM.FileModels)
            {
                if (fm.Type == FILE_TYPE.FOLDER)
                {
                    doFindFolder(word, fm);
                    
                }
                else if (fm.Type == FILE_TYPE.CPP || fm.Type == FILE_TYPE.PAT || fm.Type == FILE_TYPE.HEAD)
                {
                    doFindFile(word, fm);
                    
                }
            }

            if (Global.gDockVM.ReplaceVisible == Visibility.Visible)
                Global.gDockVM.addReportResultLast(REPORT_TYPE.WARNING, $"-- Replace \"{word}\" -> \"{Global.gDockVM.ReplaceText}\",  Changed : {_findAllMatchCount} --");            else
                Global.gDockVM.addReportResultLast(REPORT_TYPE.WARNING, $"-- Find \"{word}\", Matched : {_findAllMatchCount} --");
        }

        public void findWord(bool isBackWord = false, bool isReplace = false)
        {
            string word = Global.gDockVM.FindText;            
            if (word == "") return;
            if (Global.gDockVM.ReplaceVisible == Visibility.Visible)
            {
                if (Global.gDockVM.ReplaceText == "")
                {
                    HandyControl.Controls.MessageBox.Show($"Replace Text Empty", "", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }
            }


            string regexp_reserve = "( ) { } [ ] * + ? . ^ | $";
            string[] rarr = regexp_reserve.Split(' ');
            foreach (var s in rarr)
            {
                if (word.IndexOf(s) != -1) word = word.Replace(s, string.Format(@"\{0}", s));             
            }

            if (Global.gDockVM.FindAll == 1) { doFindAll(word); return; }

            int current = _sci.CurrentPosition;



            
            MatchCollection mats;
            RegexOptions option = RegexOptions.IgnoreCase;
            if (Global.gDockVM.IsUpperLower) option = RegexOptions.None;

            mats = Regex.Matches(_sci.Text, word, option);
            int pos = -1;
            if (isBackWord)
            {
                pos = doFind(mats, current - word.Length , 0, true, isReplace);
                if (pos == -1 && Global.gDockVM.IsWrap) 
                    pos = doFind(mats, _sci.Text.Length, current - word.Length, true, isReplace);
            }else
            {
                pos = doFind(mats, current, _sci.Text.Length, false, isReplace);
                if (pos == -1 && Global.gDockVM.IsWrap) 
                    pos = doFind(mats, 0, current, false, isReplace);
            }
            if (pos != -1)
            {
                int len = word.Length;
                if (isReplace) len = Global.gDockVM.ReplaceText.Length;
                _sci.SetSel(pos, pos + len);
                addPrev(current);
            }
            
            
        }

        public void gotoWordCPP(string word)
        {
            int current = _sci.CurrentPosition;
            //  back-word
            //            _sci.TargetStart = 0;
            //            _sci.TargetEnd = _sci.CurrentPosition - 1;
            //  fore-word
            //            _sci.TargetStart = _sci.CurrentPosition;
            //            _sci.TargetEnd = _sci.TextLength;

            //            if (_sci.SearchInTarget(word) != -1 )
            //               _sci.SetSel(_sci.TargetStart, _sci.TargetEnd);
            Line line = _sci.Lines[_sci.CurrentLine];            
            string lineText = line.Text;

            bool bFind = false;
            if (lineText.IndexOf("RUN_STEP") != -1)
            {
                Match matStep = Regex.Match(_sci.Text, @"STEP(I)?\s*\(\s*" + word );
                if (matStep.Success)
                {
                    int st = matStep.Index + (matStep.Length - word.Length);
                    _sci.SetSel(st, st + word.Length);
                    bFind = true;
                }
            }
            else if (lineText.IndexOf("SEND_MPAT") != -1)
            {                
                foreach (FileModel fm in Global.gFilesVM.FileModels)
                {
                    if (fm.Type != FILE_TYPE.PAT) continue;
                    if (fm.Name == word)
                    {
                        Global.gDockVM.addDocPane(fm); 
                        bFind = true; break;
                    }
                }
            }
            else
            {
                Match matC = Regex.Match(_sci.Text, @"(void|int|float|double|define|char|long|bool|>|FILE)\s*\**\s*" + word + @"\b");
                if (matC.Success)
                {
                    int st = matC.Index + matC.Value.IndexOf(word);
                    int ed = st + word.Length;                    
                    _sci.SetSel(st, ed);
                    bFind = true;
                }
            }

            if (bFind) addPrev(current);
        }

        

        public bool findDef(string word, FileModel fm)
        {
            FileStream fs = new FileStream(fm.Path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            StreamReader reader = new StreamReader(fs);
            string Text = reader.ReadToEnd();
            
            int st = -1;
            int ed = -1;
            Match matSDEF = Regex.Match(Text, @"SDEF\s+" + word + @"\b");
            if (matSDEF.Success)
            {
                st = matSDEF.Index + matSDEF.Value.IndexOf(word);
                ed = st + word.Length;
                goto FindDone;
            } 
            
            Match matFunc = Regex.Match(Text, @"FUNCTION\s+" + word);
            if (matFunc.Success)
            {
                st = matFunc.Index + matFunc.Value.IndexOf(word);
                ed = st + word.Length;
                goto FindDone;
            }
                
            Match matLavel = Regex.Match(_sci.Text, word + ":");
            if (matLavel.Success)
            {
                st = matLavel.Index;
                ed = st + word.Length;
                goto FindDone;
            }

        FindDone:
            fs.Close();
            reader.Close();

            if (st != -1) 
            {
                
                Global.gDockVM.addDocPane(fm);  // no Focus
                fm._edit._sci.SetSel(st, ed);
            }

            return (st != -1) ? true : false ;
        }


        public bool findDefFolder(string word, FileModel folderFM)
        {
            if (folderFM.Type != FILE_TYPE.FOLDER) return false;
            foreach (FileModel fm in folderFM.InnerFiles)
            {
                if (fm.Type != FILE_TYPE.HEAD) continue;  // Folder Depth 1단계 까지만
                if (findDef(word, fm)) return true;
            }
            return false;
        }



        public void gotoWordPAT(string word)
        {
            int current = _sci.CurrentPosition;

            if (findDef(word, _file))
                addPrev(current);
            else
            {
                bool bFind = false;
                foreach (FileModel fm in Global.gFilesVM.FileModels)
                {
                    if (fm.Type == FILE_TYPE.FOLDER)
                    {
                        if (findDefFolder(word, fm))
                        { bFind = true; break; }
                    }
                    else if (fm.Type == FILE_TYPE.HEAD)
                    {
                        if (findDef(word, fm))
                        { bFind = true; break; }
                    }

                }
                if (bFind) addPrev(current);
            }            
            
        }

        public void gotoError(int line)
        {
            if (line > _sci.Lines.Count)
                return;
            Line l =  _sci.Lines[line - 1];
            l.Goto();
            _sci.SetSel(l.Position, l.EndPosition);            
            _sci.Focus();

        }

        public bool checkBin()
        {
            Stream objStream = new FileStream(_file.Path, FileMode.Open, FileAccess.Read);
            bool bFlag = true;
            
            for (int nPosition = 0; nPosition < objStream.Length; nPosition++)
            {
                int a = objStream.ReadByte();

                if (!(a >= 0 && a <= 127))
                {
                    break;            // Binary File
                }
                else if (objStream.Position == (objStream.Length) || nPosition >= 256)
                {                     // 256 byte 까지 Text면 Text로 인식
                    bFlag = false;    // Text File
                }
            }
            objStream.Dispose();

            return bFlag;            
        }

        public async void openFileBin()
        {
            
            _sci.Text = "";
            StringBuilder tb = new StringBuilder();
            Global.gFilesVM.CompVisible = Visibility.Visible;
            Task t = Task.Run(() =>
            {
                using (FileStream fs = new FileStream(_file.Path, FileMode.Open))
                {
                    int size = (int)fs.Length;
                    byte[] buff = new byte[size];
                    fs.Read(buff, 0, size);
                    int idx = 0;

                    
                    foreach (byte b in buff)
                    {
                        if (idx != 0 && (idx % 16 == 0))
                            tb.Append("\r\n");
                        tb.Append(b.ToString("X2") + " ");
                        idx++;
//                        Console.WriteLine($"idx {idx}");

                    }                    
                }
            });
            await t;
            Global.gFilesVM.CompVisible = Visibility.Hidden;
            _sci.Text += tb.ToString();
            _sci.ReadOnly = true;
        }

        public void findHeader()
        {
            var reader = new StringReader(_sci.Text);
            while (true)
            {
                string line = reader.ReadLine();
                if (line == null)
                    break;

                bool isFind = false;
                if (line.Contains("#include"))
                {
                    if (line.Contains("STAR_TEST.h"))
                    {
                        Global.gAPIHeader = "STAR_TEST.h";
                        isFind = true;
                    }
                    else if (line.Contains("STAR_HSBI.h"))
                    {
                        Global.gAPIHeader = "STAR_HSBI.h";
                        isFind = true;    
                    }
                    if (isFind)
                    {
                        Global.gMainVM.Title = Global.gFilesVM._folderPath;
                        break;
                    }
                }
            }
        }

        public void openFile()
        {
            FileStream fs = new FileStream(_file._path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
            StreamReader reader = new StreamReader(fs);

            _sci.Text = reader.ReadToEnd();
            _sci.EmptyUndoBuffer();

            if (_file.Type == FILE_TYPE.CPP)
            {
                findHeader();
            }

            reader.Close();
            fs.Close();
            
        }

        public void setType()
        {
            if (_file.Type == FILE_TYPE.CPP)
                setCPPKeyword();
            else if (_file.Type == FILE_TYPE.HEAD || _file.Type == FILE_TYPE.PAT)
                setPATKeyword();
        }

        public bool save()
        {
            IsModified = false;
            if (_sci.Text == "") return false;
            if (_isBin) return false;

            using (FileStream fs = File.Create(_file.Path))
            {                
                using (BinaryWriter bw = new BinaryWriter(fs))
                {
                    bw.Write(_sci.Text.ToCharArray(), 0, _sci.Text.Length ); // Omit trailing NULL
                }
            }
            
            _sci.SetSavePoint();
            
            //_sci.GotoPosition(curPos);

            return true;

        }

        public void checkSave()
        {
            if (IsModified)
            {
                MessageBoxResult res = System.Windows.MessageBox.Show(
                    $"{_file.Name}.{_file.Type.ToString().ToLower()} Modified. Do You Wan\'t Change?", "Modified"
                    , MessageBoxButton.YesNo, MessageBoxImage.Question );
                if (res == MessageBoxResult.Yes)
                    save();
            }
        }



//        private void MyFindReplace_KeyPressed(object sender, System.Windows.Forms.KeyEventArgs e)
//        {
//            keyDownSci(sender, e);
//        }


        void keyDownSci(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            #region KeyDown Find 

            if (e.Control && e.KeyCode == Keys.F)
            {
                FIND_OPTION opt = FIND_OPTION.NONE;
                if (e.Shift) opt = FIND_OPTION.ALL;

                _findView.show(true, opt);
                Global.gDockVM.FindText =_sci.GetWordFromPosition(_sci.CurrentPosition);                                
                _findView.focusText();
                
                e.SuppressKeyPress = true;
            }
            else if (e.KeyCode == Keys.Escape)
            {
                _findView.show(false);
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.H)
            {
                FIND_OPTION opt = FIND_OPTION.REPLACE;
                if (e.Shift) opt |= FIND_OPTION.ALL;

                _findView.show(true, opt);
                Global.gDockVM.FindText = _sci.GetWordFromPosition(_sci.CurrentPosition);                
                _findView.focusText();

                e.SuppressKeyPress = true;
                
            }
            else if (e.KeyCode == Keys.F2)
            {
                findWord(true);
                e.SuppressKeyPress = true;
            }
            else if (e.KeyCode == Keys.F3)
            {
                findWord();
                Global.gDockVM.doFind("Find Next");
                e.SuppressKeyPress = true;
            }

            #endregion KeyDown Find

            else if (e.Control && e.KeyCode == Keys.G)
            {
                MyGoTo.ShowGoToDialog();
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.S)
            {
                save();
                e.SuppressKeyPress = true;
            }

            else if (e.KeyCode == Keys.F11 || (e.Control && e.KeyCode == Keys.OemMinus))
            {
                gotoPrev();
            }
            else if (e.KeyCode == Keys.F12)
            {
                string find = _sci.GetWordFromPosition(_sci.CurrentPosition);
                if (find != "")
                {
                    int val = 0;
                    bool isNum = int.TryParse(find, out val);
                    if (!isNum)  // except number
                    {
                        if (_file.Type == FILE_TYPE.CPP)
                        {
                            if (Global.gExceptGotoCPP.FirstOrDefault(a => a == find) == null)
                            {
                                gotoWordCPP(find);
                            }else if (Global.gAPIFuns.FirstOrDefault(a => a.Name == find) != null)
                            {
                                Global.gAssistVM.APISearch = find;
                                Global.gAssistVM.expandAPI();
                                if (find == "BIB_SIZE")
                                {
                                    Line line = _sci.Lines[_sci.CurrentLine];
                                    string lineText = line.Text;
                                    try
                                    {
                                        Global.gAssistVM.setBIBSize(lineText);
                                    }catch (Exception ex) { }
                                }

                            }
                        }
                        else if (_file.Type == FILE_TYPE.PAT)
                        {
                            if (Global.gExceptGotoPAT.FirstOrDefault(a => a == find) == null)
                                gotoWordPAT(find);

                        }
                    }
                }
                e.SuppressKeyPress = true;
            }

            #region KeyDown Compile
            else if (e.Control && e.Shift && e.KeyCode == Keys.B)
            {
                Global.gMainVM.excuteMenu("COMPILE ALL");
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.B)
            {
                Global.gMainVM.excuteMenu("COMPILE FILE");
                e.SuppressKeyPress = true;
            }
            #endregion

            #region undo / redo
            else if (e.Control && e.KeyCode == Keys.Z)
            {
                _sci.Undo();
                e.SuppressKeyPress = true;
            }
            else if (e.Control && e.KeyCode == Keys.Y)
            {
                _sci.Redo();
                e.SuppressKeyPress = true;
            }

            #endregion

            else
            {
                if (_sci.Modified)
                {                    
                    IsModified = true;
                }

                TextArea_KeyDown(sender, e);
            }

            
        }

        class SelWrap
        {
            public int Begin { get; set; }
            public int Length { get; set; }
        }


        string convString(Keys key, bool shift)
        {
            bool cap = System.Windows.Forms.Control.IsKeyLocked(Keys.CapsLock);
            bool upper = cap;
            if (shift) upper = !upper;

            string ret = "";
            if (key >= Keys.A && key <= Keys.Z)
            {
                if (upper)
                    ret = Convert.ToChar((int)key).ToString();
                else
                    ret = Convert.ToChar((int)(key + 0x20)).ToString();
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
                }
                else
                    ret = Convert.ToChar((int)key).ToString();
            }
            else if (key == Keys.Space || key == Keys.Tab)
            {
                ret = Convert.ToChar((int)key).ToString();
            }
            else  // spacail charictor
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
                }
                else
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
            var edit = _sci;
            if (edit.Selections.Count < 2) return; // no in column mode

            if (e.KeyCode >= Keys.PageUp && e.KeyCode <= Keys.Down) return;

//            Console.WriteLine("Down Key: " + e.KeyCode.ToString() + "-> char:" + Convert.ToChar(e.KeyValue));

            if (e.KeyCode == Keys.Enter) return;
            if (e.KeyCode == Keys.CapsLock) return;
            if (
                    !((e.KeyCode >= Keys.A && e.KeyCode <= Keys.Z) ||
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
            //edit.AdditionalSelectionTyping = true;
            //edit.AdditionalCaretsBlink = true;
            //edit.AdditionalCaretsVisible = true;
        }



        /*         
         * Keyword Set	style	description
                    0	Style.Cpp.Word	                Primary keywords and identifiers
                    1	Style.Cpp.Word2	                Secondary keywords and identifiers
                    2	Style.Cpp.CommentDocKeyword	    Documentation comment keywords
                    3	Style.Cpp.GlobalClass	        Global classes and typedefs
                    4	Style.Cpp.Preprocessor	        Preprocessor definitions
                    5	Style.Cpp.TaskMarker	        Task marker and error marker keywords
        */

        private void scintilla_InsertCheck(object sender, InsertCheckEventArgs e)
        {
            if ((e.Text.EndsWith("\r") || e.Text.EndsWith("\n")))
            {
                var curLine = _sci.LineFromPosition(e.Position);
                var curLineText = _sci.Lines[curLine].Text;

                var indent = Regex.Match(curLineText, @"^\s+");
                string add = indent.Value;
                if (indent.Value.IndexOf("\r\n") != -1)
                    add = indent.Value.Remove(indent.Value.IndexOf("\r\n"));
                e.Text += add; // Add indent following "\r\n"

                // Current line end with bracket?
                if (Regex.IsMatch(curLineText, @"{\s+$"))
                    e.Text += '\t'; // Add tab
            }
        }

        public void setCPPKeyword()
        {
            // Configure the CPP (C#) lexer styles
            _sci.Styles[ScintillaNET.Style.Cpp.Identifier].ForeColor = MConv.IntToColor(0xb0edff);    // variable
            _sci.Styles[ScintillaNET.Style.Cpp.Comment].ForeColor = MConv.IntToColor(0xBD758B);       //  // ...
            _sci.Styles[ScintillaNET.Style.Cpp.CommentLine].ForeColor = MConv.IntToColor(0x40BF57);   //  // char
            _sci.Styles[ScintillaNET.Style.Cpp.CommentDoc].ForeColor = MConv.IntToColor(0x2FAE35);    // /*  ... */

            _sci.Styles[ScintillaNET.Style.Cpp.Number].ForeColor = MConv.IntToColor(0xc6f4ba);            // 1234 ...
            _sci.Styles[ScintillaNET.Style.Cpp.String].ForeColor = MConv.IntToColor(0xffb394);        // " "
            _sci.Styles[ScintillaNET.Style.Cpp.Character].ForeColor = MConv.IntToColor(0xE8785F);     //' '
            _sci.Styles[ScintillaNET.Style.Cpp.Preprocessor].ForeColor = MConv.IntToColor(0xc586b7);  // # ../
            _sci.Styles[ScintillaNET.Style.Cpp.Operator].ForeColor = MConv.IntToColor(0xFFFFFF);      // + - = ..
            _sci.Styles[ScintillaNET.Style.Cpp.Word].ForeColor = MConv.IntToColor(0x48A8EE);          // gKeyDefCPP 
            _sci.Styles[ScintillaNET.Style.Cpp.Word2].ForeColor = MConv.IntToColor(0xEF678D);         // gKeyFunCPP                         
            _sci.Styles[ScintillaNET.Style.Cpp.GlobalClass].ForeColor = MConv.IntToColor(0x45c9af);   // gKeyApiCPP


            _sci.Lexer = Lexer.Cpp;

            _sci.SetKeywords(0, Global.gKeyDefCPP);
            _sci.SetKeywords(1, Global.gKeyFunCPP);
            _sci.SetKeywords(3, Global.gKeyApiCPP);

            initCodeFolding();

           
        }



        public void setPATKeyword()
        {
            // Configure the CPP (C#) lexer styles
            _sci.Styles[ScintillaNET.Style.Cpp.Identifier].ForeColor = MConv.IntToColor(0xE2D78E);    // variable
            _sci.Styles[ScintillaNET.Style.Cpp.Comment].ForeColor = MConv.IntToColor(0xBD758B);       //  // ...
            _sci.Styles[ScintillaNET.Style.Cpp.CommentLine].ForeColor = MConv.IntToColor(0x40BF57);   //  // char
            _sci.Styles[ScintillaNET.Style.Cpp.CommentDoc].ForeColor = MConv.IntToColor(0x2FAE35);    // /*  ... */

            _sci.Styles[ScintillaNET.Style.Cpp.Number].ForeColor = MConv.IntToColor(0xc6f4ba);            // 1234 ...
            _sci.Styles[ScintillaNET.Style.Cpp.String].ForeColor = MConv.IntToColor(0xffb394);        // " "
            _sci.Styles[ScintillaNET.Style.Cpp.Character].ForeColor = MConv.IntToColor(0xE8785F);     //' '
            _sci.Styles[ScintillaNET.Style.Cpp.Preprocessor].ForeColor = MConv.IntToColor(0xc586b7);  // # ../
            _sci.Styles[ScintillaNET.Style.Cpp.Operator].ForeColor = MConv.IntToColor(0xFFFFFF);      // + - = ..
            _sci.Styles[ScintillaNET.Style.Cpp.Word].ForeColor = MConv.IntToColor(0x87A2E5);          // SetKeyWord 0
            _sci.Styles[ScintillaNET.Style.Cpp.Word2].ForeColor = MConv.IntToColor(0xE84548);         // SetKeyword 1                        
            _sci.Styles[ScintillaNET.Style.Cpp.GlobalClass].ForeColor = MConv.IntToColor(0x27C6B1);   // SetKeyword 3

            _sci.Lexer = Lexer.Cpp;

            _sci.SetKeywords(0, Global.gKeyOpcodePAT);
            _sci.SetKeywords(1, Global.gKeyFuncPAT);            
            _sci.SetKeywords(3, Global.gKeyRegPAT);

            initCodeFolding();

            
        }


        /// <summary>
        ///
        /// </summary>
        public void initColor()
        {
            /// initColor ///
            _sci.CaretForeColor = MConv.IntToColor(0xFFFFFF);
            _sci.SetSelectionBackColor(true, MConv.IntToColor(0x114D9C));

            /// Syntext Color /// 
            _sci.StyleResetDefault();
            _sci.Styles[ScintillaNET.Style.Default].Font = "Consolas";
            _sci.Styles[ScintillaNET.Style.Default].Size = 11;
            _sci.Styles[ScintillaNET.Style.Default].BackColor = MConv.IntToColor(BACK_COLOR);
            _sci.Styles[ScintillaNET.Style.Default].ForeColor = MConv.IntToColor(0xFFFFFF);
            _sci.StyleClearAll();


            /// NumberMargin
            _sci.Styles[ScintillaNET.Style.LineNumber].BackColor = MConv.IntToColor(BACK_COLOR);
            _sci.Styles[ScintillaNET.Style.LineNumber].ForeColor = MConv.IntToColor(FORE_COLOR);
            _sci.Styles[ScintillaNET.Style.IndentGuide].ForeColor = MConv.IntToColor(BACK_COLOR);
            _sci.Styles[ScintillaNET.Style.IndentGuide].BackColor = MConv.IntToColor(FORE_COLOR);

            var nums = _sci.Margins[NUMBER_MARGIN];
            nums.Width = LINE_NUMBERS_MARGIN_WIDTH;
            nums.Type = MarginType.Number;
            nums.Sensitive = true;
            nums.Mask = 0;


            /// BookMarkMargin
            var margin = _sci.Margins[BOOKMARK_MARGIN];
            margin.Width = 0;
            margin.Sensitive = true;
            margin.Type = MarginType.Symbol;
            margin.Mask = (1 << BOOKMARK_MARKER);

            var marker = _sci.Markers[BOOKMARK_MARKER];
            marker.Symbol = MarkerSymbol.Circle;
            marker.SetBackColor(MConv.IntToColor(0x30303B));
            marker.SetForeColor(MConv.IntToColor(0x000000));
            marker.SetAlpha(100);
        }

        void initCodeFolding()
        {
            /// CodeFolding
            _sci.SetFoldMarginColor(true, MConv.IntToColor(BACK_COLOR));
            _sci.SetFoldMarginHighlightColor(true, MConv.IntToColor(BACK_COLOR));

            // Enable code folding
            _sci.SetProperty("fold", "1");
            _sci.SetProperty("fold.compact", "1");

            // Configure a margin to display folding symbols
            _sci.Margins[FOLDING_MARGIN].Type = MarginType.Symbol;
            _sci.Margins[FOLDING_MARGIN].Mask = Marker.MaskFolders;
            _sci.Margins[FOLDING_MARGIN].Sensitive = true;
            _sci.Margins[FOLDING_MARGIN].Width = 20;

            // Set colors for all folding markers
            for (int i = 25; i <= 31; i++)
            {
                _sci.Markers[i].SetForeColor(MConv.IntToColor(BACK_COLOR)); // styles for [+] and [-]
                _sci.Markers[i].SetBackColor(MConv.IntToColor(FORE_COLOR)); // styles for [+] and [-]
            }

            // Configure folding markers with respective symbols
            _sci.Markers[Marker.Folder].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CirclePlus : MarkerSymbol.BoxPlus;
            _sci.Markers[Marker.FolderOpen].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CircleMinus : MarkerSymbol.BoxMinus;
            _sci.Markers[Marker.FolderEnd].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CirclePlusConnected : MarkerSymbol.BoxPlusConnected;
            _sci.Markers[Marker.FolderMidTail].Symbol = MarkerSymbol.TCorner;
            _sci.Markers[Marker.FolderOpenMid].Symbol = CODEFOLDING_CIRCULAR ? MarkerSymbol.CircleMinusConnected : MarkerSymbol.BoxMinusConnected;
            _sci.Markers[Marker.FolderSub].Symbol = MarkerSymbol.VLine;
            _sci.Markers[Marker.FolderTail].Symbol = MarkerSymbol.LCorner;

            // Enable automatic folding
            _sci.AutomaticFold = (AutomaticFold.Show | AutomaticFold.Click | AutomaticFold.Change);
        }

    }
}
