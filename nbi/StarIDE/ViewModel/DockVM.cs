using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarBase;
using Xceed.Wpf.AvalonDock.Layout;
using StarIDE.Model;
using System.Collections.ObjectModel;
using System.Windows.Forms.Integration;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using StarIDE.View;
using HandyControl.Controls;
using System.Xml.Linq;
using ScintillaNET;

namespace StarIDE.ViewModel
{



    public class DockVM : BaseVM
    {
        #region Report Model
        public ListBox LB_Output; // last item focus용

        ObservableCollection<ReportModel> _reports = new ObservableCollection<ReportModel>();
        public ObservableCollection<ReportModel> Reports
        {
            get { return _reports; }
            set { _reports = value; OnPropertyChanged(nameof(Reports)); }
        }

        public void clearReport()
        {
            Reports.Clear();
        }

        public void addReportResult(REPORT_TYPE type, string str, int line, FileModel fm)
        {
            ReportModel rm = new ReportModel(type, str, line) { _file = fm };
            Reports.Add(rm);
        }

            public void addReportResultLast(REPORT_TYPE type, string str)
        {            
            ReportModel rm = new ReportModel(type, str);
            Reports.Add(rm);

            LB_Output.ScrollIntoView(rm);
        }

        public bool addReportCPP(FileModel fm, string output)
        {
            string[] messages = output.Split('\n');
            bool pf = false;

            string srcName = $"{ fm.Name }.{fm.Type.ToString().ToLower()}";
            REPORT_TYPE type = REPORT_TYPE.INFO;
            ReportModel rm = new ReportModel(type, $"   < Build : {srcName} >") ;
            Reports.Add(rm);
            
            for (int idx = 2; idx< messages.Length; idx++)
            {
                string message = messages[idx];
                //                if (message.IndexOf("\\") != -1) continue;
                //Console.WriteLine($"{idx} ({message.Length}): " + message);
                if (message == "\r") continue;
                if (message.Length == 0) continue;
                if (message.IndexOf("wsl make -C") != -1) continue;
                if (message.IndexOf("make: Entering") != -1) continue;
                if (message.IndexOf("make: Leaving") != -1) continue;
                int line = -1;
                type = REPORT_TYPE.INFO;

                if (message.IndexOf("error:") != -1)
                {
                    type = REPORT_TYPE.ERR;
                    string[] toks = message.Split(':');
                    for (int i = 0; i < toks.Length; i++)
                    {
                        string s = toks[i];
                        if (s.IndexOf(srcName) != -1)
                        {
                            if (toks.Length > i + 1)
                            {
                                line = Int32.Parse(toks[i + 1]);
                                break;
                            }
                        }
                    }
                } 
                else if (message.IndexOf("warning:") != -1)
                {
                    type = REPORT_TYPE.WARNING;
                    string[] toks = message.Split(':');
                    for (int i = 0; i < toks.Length; i++)
                    {
                        string s = toks[i];
                        if (s.IndexOf(srcName) != -1)
                        {
                            if (toks.Length > i + 1)
                            {
                                line = Int32.Parse(toks[i + 1]);
                                break;
                            }
                        }
                    }
                }
                else if (message.IndexOf("Compiled PGM") != -1)
                {
                    type = REPORT_TYPE.OK;
                    pf = true;
                }

                if (idx == messages.Length - 2)
                    if (pf == true) continue;

                rm = new ReportModel(type, message, line) { _file = fm};                
                Reports.Add(rm);                
             }

            return pf;
        }

        public bool addReportPAT(FileModel fm, string output)
        {
            string[] messages = output.Split('\n');
            bool pf = true;
            REPORT_TYPE res = REPORT_TYPE.INFO;

            string srcName = $"{ fm.Name }.{fm.Type.ToString().ToLower()}";
            REPORT_TYPE type = REPORT_TYPE.INFO;
            ReportModel rm = new ReportModel(type, $"   < Build : {srcName} >");
            Reports.Add(rm);
            
            for (int idx = 6; idx < messages.Length; idx++)
            {
                string message = messages[idx];
                //Console.WriteLine($"{idx} ({message.Length}): " + message);

                if (message == "\r") continue;
                if (message.Length == 0) continue;

                int line = -1;
                type = REPORT_TYPE.INFO;
                if (message.IndexOf("Error :") != -1 || message.IndexOf("Error Code :") != -1)
                {
                    res = REPORT_TYPE.ERR;
                    pf = false;
                    string[] toks = message.Split(':');
                    for (int i = 0; i < toks.Length; i++)
                    {
                        string s = toks[i];
                        if (s.IndexOf(srcName) != -1)
                        {
                            if (toks.Length > i + 1)
                            {
                                line = Int32.Parse(toks[i + 1]);
                                break;
                            }
                        }
                    }
                }
                rm = new ReportModel(res, message, line) { _file = fm};
                Reports.Add(rm);
 
            }
            if (pf)
                rm = new ReportModel(REPORT_TYPE.OK, $"Compiled MPA:{fm.Name}") ;                
            else
                rm = new ReportModel(REPORT_TYPE.INFO, $"make: [ {fm.Name} ] Error");

            Reports.Add(rm);

            return pf;
        }

        public void focusError(ReportModel rm)
        {
            if (rm == null) return;
            if (rm.Line == -1) return;

            addDocPane(rm._file);            
            if (rm._file._edit == null) 
                return;
            rm._file._edit.gotoError(rm.Line);
        }

        public void closeOutput()
        {
            Reports.Clear();
        }

        #endregion

        #region Find View
        public string _findText;
        public string FindText
        {
            get { return _findText; }
            set { _findText = value; OnPropertyChanged(nameof(FindText)); }
        }

        public string _replaceText;
        public string ReplaceText
        {
            get { return _replaceText; }
            set { _replaceText = value; OnPropertyChanged(nameof(ReplaceText)); }
        }

        public Visibility _findVisible = Visibility.Visible;
        public Visibility FindVisible
        {
            get => _findVisible;
            set
            {  _findVisible = value; OnPropertyChanged(nameof(FindVisible)); }
        }

        public Visibility _replaceVisible = Visibility.Collapsed;
        public Visibility ReplaceVisible
        {
            get => _replaceVisible;
            set { _replaceVisible = value; OnPropertyChanged(nameof(ReplaceVisible)); }
        }

        public bool _isUpperLower = false;
        public bool IsUpperLower
        {
            get => _isUpperLower;
            set { _isUpperLower = value; OnPropertyChanged(nameof(IsUpperLower)); }
        }

        public bool _isWrap = true;
        public bool IsWrap
        {
            get => _isWrap;
            set { _isWrap = value; OnPropertyChanged(nameof(IsWrap)); }
        }

        public int _findAll = 0;
        public int FindAll
        {
            get => _findAll;
            set { _findAll = value; OnPropertyChanged(nameof(FindAll)); }
        }



        public void doFind(string type)
        {            
            if (type == "Find Next")
            {
                Global.gMainVM.CurEdit.findWord();
            }else if (type == "Find Prev")
            {
                Global.gMainVM.CurEdit.findWord(true);
            }else if (type == "Replace Next")
            {
                Global.gMainVM.CurEdit.findWord(false, true);
            }
            else if (type == "Replace Prev")
            {
                Global.gMainVM.CurEdit.findWord(true, true);
            }



        }

        public DelegateCommand cmdFind { get; set; }

        #endregion

        public LayoutDocumentPane _documentManager;

        public Collection<EditModel> _editors = new Collection<EditModel>();
        public Collection<PrevPosition> _prevPositions = new Collection<PrevPosition>();

        public DockVM()
        {
            Global.gDockVM = this;
            cmdFind = new DelegateCommand(e => { Button bt = e as Button; doFind(bt.Content.ToString()); });


        }

        public void  addDocPane(FileModel fm, bool isFocus = true)
        {
            if ( !(fm.Type == FILE_TYPE.PAT 
                || fm.Type == FILE_TYPE.CPP
                || fm.Type == FILE_TYPE.VEC
                || fm.Type == FILE_TYPE.HEAD) ) return;

            EditModel checkFm = findEdit(fm.Name);
            if (checkFm != null)
            {
                if (isFocus)
                {
                    checkFm._pane.IsActive = true;
                    Global.gMainVM.CurEdit = checkFm;
                }
                return;
            }

            LayoutDocument doc = new LayoutDocument();            
            doc.Title = fm.Name;            
            doc.Closed += CloseEvent;
            doc.IsActiveChanged += SelectChangeEvent;

            DockPanel dp = new DockPanel();
            doc.Content = dp;

            FindView find = new FindView();
            dp.Children.Add(find);
            DockPanel.SetDock(find, Dock.Top);
            find.DataContext = this;            
            find.show(false);

            WindowsFormsHost wfh = new WindowsFormsHost();         
            dp.Children.Add(wfh);
            DockPanel.SetDock(wfh, Dock.Bottom);
            dp.LastChildFill = true;


            EditModel edit = new EditModel(fm, doc);
            edit._findView = find;
            edit._findView._parentEM = edit;
            wfh.Child = edit._sci;
                  
            

            _editors.Add(edit);
            _documentManager.Children.Add(doc);

            if (isFocus)
            {
                doc.IsActive = true;
                edit._sci.Focus();
                Global.gMainVM.CurEdit = edit;
                
            }
        }

        public void closeDocAll()
        {
            foreach (EditModel edit in _editors)
                edit.checkSave();

            _documentManager.Children.Clear();
            _editors.Clear();
        }

        public EditModel findEdit(string name)
        {
            string comp = name.Replace("*", "");
            foreach (EditModel em in _editors)
            {
                if (em._file.Name == comp)
                    return em;
            }
            return null;

        }

        public void closeDoc(string name)
        {
            EditModel em = findEdit(name);
            if (em != null)
            {
                em.checkSave();
                em._file._edit = null;
                if (em == Global.gMainVM.CurEdit)
                    Global.gMainVM.CurEdit = null;
                _editors.Remove(em);
            }
        }

        public void CloseEvent(object send, EventArgs e)
        {
            LayoutDocument ld = send as LayoutDocument;
            closeDoc(ld.Title);
        }

        public void SelectChangeEvent(object send, EventArgs e)
        {
            LayoutDocument ld = send as LayoutDocument;
            EditModel em = findEdit(ld.Title);
            if (em != null) return;
            if (ld.IsActive == false)
            {                
                if (em._findView.Visibility == Visibility.Visible)
                    em._findView.show(false);
            }
            else // actived
            {
                Global.gMainVM.CurEdit = em;
            }
        }
    }
}
