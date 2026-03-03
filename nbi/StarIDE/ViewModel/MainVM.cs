using StarBase;
using System;
using System.Collections.ObjectModel;
using StarIDE.Model;
using System.Windows.Input;
using Microsoft.WindowsAPICodePack.Dialogs;
using System.IO;
using System.Diagnostics;
using System.Windows;
using System.Threading;
using System.Windows.Threading;
using ThreadState = System.Threading.ThreadState;
using System.Xml.Linq;
using System.Xml;
using System.Windows.Controls;
using HandyControl.Controls;
using System.Runtime.CompilerServices;
using System.Linq;
using System.Drawing;
using System.Windows.Media;
using System.Threading.Tasks;

namespace StarIDE.ViewModel
{
    public class MainVM : BaseVM
    {

        #region Main View
        public EditModel _curEdit = null;
        public EditModel CurEdit { get { return _curEdit; } set { _curEdit = value;  } }

        public string _title;
        public string Title
        {
            get { return _title; }
            set { 
                string path = value;
                _title = VerInfo.getVer();
                if (Global.gAPIHeader == "STAR_TEST.h")
                    _title += " [TARGET : NBI]  ";
                else
                    _title += " [TARGET : HSBI]  ";
                _title += path;
                OnPropertyChanged(nameof(Title));

                if (Global.gAPIHeader == "STAR_TEST.h")
                    TitleColor = _colorNBI;
                else
                    TitleColor = _colorHSBI;


            }
        }


        public SolidColorBrush _colorNBI = new SolidColorBrush(
            System.Windows.Media.Color.FromArgb(0xdd, 0x39, 0x5c, 0x96));

        public SolidColorBrush _colorHSBI = new SolidColorBrush(
            System.Windows.Media.Color.FromArgb(0xdd, 0x15, 0x1d, 0x29));
      
        
        public SolidColorBrush _titleColor;
        public SolidColorBrush TitleColor
        {
            get => _titleColor;
            set  { _titleColor = value; OnPropertyChanged(nameof(TitleColor)); }

        }
        #endregion Main View

        #region Recent File
        public SideMenuItem xRecent0;
        public SideMenuItem xRecent1;
        public SideMenuItem xRecent2;
        public ObservableCollection<RecentModel> _recentModels = new ObservableCollection<RecentModel>();
        public ObservableCollection<RecentModel> RecentModels
        {
            get => _recentModels;
            set { _recentModels = value; OnPropertyChanged(nameof(RecentModels));}
        }

        public void loadRecent()
        {
            XmlDocument xml = Global.loadConfig();
            if (xml == null) return;
            XmlNodeList list = xml.SelectNodes("/StarIDE/recent/folder");
            foreach (XmlNode node in list)
            {
                if (node.Attributes["name"].Value == "") break;

                RecentModel model = new RecentModel();                
                model.Name = node.Attributes["name"].Value;
                model.Path = node.Attributes["path"].Value;
                RecentModels.Add(model);
            }
         
        }

        public void setRecentModel()
        {
            string name = Global.gFilesVM.FolderName;
            string path = Global.gFilesVM._folderPath;

            RecentModel rm = RecentModels.FirstOrDefault(a => a.Path == path);
            if (rm != null)
                RecentModels.Remove(rm);

            RecentModels.Insert(0, new RecentModel() { Name = name, Path = path});

            if (RecentModels.Count > 3)
                RecentModels.RemoveAt(RecentModels.Count - 1);
            updateRecent();

        }

        public void updateRecent() // side menu data binding 이 안됌..
        {
            updateRecentItem(xRecent0, 0);
            updateRecentItem(xRecent1, 1);
            updateRecentItem(xRecent2, 2);
        }

        public void updateRecentItem(SideMenuItem item, int idx) // side menu data binding 이 안됌..
        {            
           if (RecentModels.Count <= idx)
            {
                item.Visibility = Visibility.Collapsed;
                return;
            }          
           
            RecentModel recentModel = RecentModels[idx];
            item.Header = recentModel.Name;
            item.ToolTip = recentModel.Path;
            item.Visibility = Visibility.Visible;
        }
            

        public void saveRecent()
        {
            XmlDocument xml = Global.loadConfig();
            if (xml == null) return;
            XmlNodeList list = xml.SelectNodes("/StarIDE/recent/folder");
            int index = 0;
            foreach (XmlNode node in list)
            {
                if (RecentModels.Count <= index) break;
                RecentModel model = RecentModels[index];
                node.Attributes["name"].Value = model.Name;
                node.Attributes["path"].Value = model.Path;
                index++;

            }

            xml.Save(Global.gConfigPath);
        }


        #endregion Recent File


        public void openFolder(string path)
        {
            excuteMenu("CLOSE PROJECT");
            try
            {
                Global.gFilesVM.loadFolder(path, null);
            }
            catch
            {
                HandyControl.Controls.MessageBox.Show("Folder Open Fail.", "Folder Open", MessageBoxButton.OK, MessageBoxImage.Error);
                Global.gFilesVM.closeFolder();                
            }
        }

        public void newFolder(string path)
        {
            string pat = @"sample\sample_pattern.pat";
            string head = @"sample\sample_def.h";
            string main = @"sample\sample_main.cpp";

            if (!File.Exists(main))
            {
                main = @"StarIDE\" + main;
                head = @"StarIDE\" + head;
                pat = @"StarIDE\" + pat;                
            }

            if (!File.Exists(main))
            {
                HandyControl.Controls.MessageBox.Show("Creating Folder / File Fail!\r\n"
                   , "Create", MessageBoxButton.OK, MessageBoxImage.Hand);
                return;
            }

            File.Copy(main, path + @"\sample_main.cpp");
            File.Copy(head, path + @"\sample_def.h");
            File.Copy(pat, path + @"\sample_pattern.pat");

            openFolder(path);

        }

        public void excuteMenu(string menu)
        {
            if (menu == "OPEN PROJECT")
            {
                CommonOpenFileDialog cofd = new CommonOpenFileDialog();
                cofd.IsFolderPicker = true;                
                if (cofd.ShowDialog() == CommonFileDialogResult.Ok)                                  
                    openFolder(cofd.FileName);                                    
            }
            else if (menu == "REOPEN PROJECT")
            {                               
                openFolder(Global.gFilesVM._folderPath);                
            }
            else if (menu == "NEW PROJECT")
            {
                System.Windows.Forms.FolderBrowserDialog folderDlg = new System.Windows.Forms.FolderBrowserDialog();
                folderDlg.ShowNewFolderButton = true;
                var res = folderDlg.ShowDialog();
                if (res.ToString() == "OK")
                    newFolder(folderDlg.SelectedPath);
                
            }
            else if (menu == "CLOSE PROJECT")
            {
                Global.gDockVM.closeOutput();
                Global.gDockVM.closeDocAll();
                Global.gDockVM._prevPositions.Clear();
                Global.gFilesVM.closeFolder();

            }
            else if (menu == "COMPILE FILE")
            {
                //in Current Pane File 
                if (CurEdit == null) return;

                Global.gDockVM.clearReport();

                if (CurEdit._file.Type == FILE_TYPE.CPP)
                {
                    Task.Run(() => compileCPP(CurEdit._file));

                }
                else if (CurEdit._file.Type == FILE_TYPE.PAT)
                {
                    Task.Run(() => compilePAT(CurEdit._file));
                }
                Global.gFilesVM.checkFolder();
            }
            else if (menu == "COMPILE ALL")
            {
                if (Global.gFilesVM.FileModels.Count == 0 ) return;
                if (_compThread.ThreadState == ThreadState.Unstarted)
                    _compThread.Start(this);
                else if (_compThread.ThreadState == ThreadState.Stopped)
                {
                    _compThread = new Thread(threadCompileAll);
                    _compThread.Start(this);
                }
                else if (_compThread.ThreadState == ThreadState.Running)
                {
                    HandyControl.Controls.MessageBox.Show("Compiing Now. Please Wait\r\n"
                        , "Compile", MessageBoxButton.OK, MessageBoxImage.Hand);
                }

            }
            else if (menu == "SAVE FILE")
            {
                if (CurEdit != null)
                    CurEdit.save();
            }
            else if (menu == "SAVE ALL")
            {
                foreach (EditModel em in Global.gDockVM._editors)
                    em.save();
            }
        }

        Thread _compThread = new Thread(threadCompileAll);


        public static async void threadCompileAll(object obj)
        {
            MainVM vm = obj as MainVM;
            Global.gFilesVM.CompVisible = Visibility.Visible;

            await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gDockVM.clearReport()));

            foreach (FileModel fm in Global.gFilesVM.FileModels)
            {
                if (fm.Type == FILE_TYPE.CPP)
                    await vm.compileCPP(fm);
                else if (fm.Type == FILE_TYPE.PAT)
                    await vm.compilePAT(fm);
                else
                    continue;

                await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gFilesVM.checkFolder()));
            }

            await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() =>
                    {
                        int failCPPCnt = 0;
                        int passCPPCnt = 0;
                        int failPatCnt = 0;
                        int passPatCnt = 0;

                        foreach (FileModel fm in Global.gFilesVM.FileModels)
                        {
                            if (fm.Type == FILE_TYPE.CPP)
                            {
                                if (fm._isCompile) passCPPCnt++;
                                else failCPPCnt++;
                            }
                            else if (fm.Type == FILE_TYPE.PAT)
                            {
                                if (fm._isCompile) passPatCnt++;
                                else failPatCnt++;
                            }                            
                        }

                        Global.gDockVM.addReportResultLast(
                            (failCPPCnt == 0) ? REPORT_TYPE.OK : REPORT_TYPE.ERR,
                            $"-- [PGM] Compiled : {passCPPCnt} ,   Fail : {failCPPCnt} --");

                        Global.gDockVM.addReportResultLast(
                            (failPatCnt == 0) ? REPORT_TYPE.OK : REPORT_TYPE.ERR,
                            $"-- [MPA] Compiled : {passPatCnt} ,   Fail : {failPatCnt} --");
                        Global.gFilesVM.CompVisible = Visibility.Hidden;
                    }));

        }


        bool _isProcCompCpp = false;
        public async Task compileCPP(FileModel fm)
        {
            if (_isProcCompCpp)
            {
                await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => {
                        Global.gDockVM.addReportResult(
                            REPORT_TYPE.WARNING, $">>>>>>>>> Please Wait", -1, fm);
                        Global.gDockVM.addReportResult(
                            REPORT_TYPE.WARNING, $">>>>>>>>> Now Processing Compile {fm.Name}.cpp -> {Global.gFilesVM.FolderName}.pgm", -1, fm);
                        }
                    ));
                return;
            }

            await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gDockVM.addReportResult(
                        REPORT_TYPE.INFO, $">>>>>>>>> Start Compile {fm.Name}.cpp -> {Global.gFilesVM.FolderName}.pgm", -1, fm)));


            _isProcCompCpp = true;
            string targetFile = $"{Global.gFilesVM.FolderName}.pgm";
            delFile(targetFile);

            if (fm._edit != null)
                await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => fm._edit.save()));
            fm._isCompile = false;            
            createMakeFile(fm);

            if (fm.Path.IndexOf(":") == -1) return;

            string dir = Global.getLastText(fm._wslPath, "/", true);
            string syncCmd = "rsync " + dir;
            wslCommand(syncCmd);

            string cleanCmd = "make -C " + dir + " clean";
            string makeCmd = "make -C " + dir;

            string pgmCompMsg = "";
            await Task.Run(() =>
            {
                pgmCompMsg = wslCommand(makeCmd, checkFile:targetFile);
            });

            //            wslCommand(cleanCmd);
            await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    
                    fm._isCompile = Global.gDockVM.addReportCPP(fm, pgmCompMsg);                    
                    fm.start_Color(fm._isCompile);
                    MakedModel mm = Global.gFilesVM.findMakeFile(Global.gFilesVM.FolderName);
                    if (mm != null) mm.start_Color(fm._isCompile);

                    if (Global.gDeleteMake) 
                        delFile("makefile");                    
                }));

            _isProcCompCpp = false;

        }

        public async Task compilePAT(FileModel fm)
        {
            if (fm._edit != null)
            {
                await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => fm._edit.save()));

            }
            string dir = Global.getLastText(fm._wslPath, "/", true);
            string syncCmd = "rsync " + dir;
            wslCommand(syncCmd);

            fm._isCompile= false;
            string patCmd = Global.gPGCPath + "pgc " + fm._wslPath;
            string str = wslCommand(patCmd);

            await Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() =>
                {
                    fm._isCompile = Global.gDockVM.addReportPAT(fm, str);
                    fm.start_Color(fm._isCompile);
                    MakedModel mm = Global.gFilesVM.findMakeFile(fm.Name);
                    if (mm != null) mm.start_Color(fm._isCompile);
                }));
        }


        public string wslCommand(string cmd, string checkFile = "")
        {
            string ret = "";
            using (var proc = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = @"cmd.exe",
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardInput = true,
                    RedirectStandardError = true,
                    CreateNoWindow = true,

                }
            })
            {
                proc.Start();
                proc.StandardInput.WriteLine("wsl " + cmd);
                System.Threading.Thread.Sleep(100); // give some time for command to execute
                //await Task.Delay(100);
                if (checkFile != "")
                {
                    int checkCount = 0; // -> 10s 
                    string file = Global.gFilesVM._folderPath + @"\" + checkFile;
                    while (true) {
                        try
                        {
                            if (File.Exists(file))
                            {
                                break;
                            }
                        }
                        catch (Exception ex)
                        {
                            break;
                        }
                        if (checkCount > 36) break;
                        System.Threading.Thread.Sleep(100);
                        checkCount++;
                    }
                }


                proc.StandardInput.Flush();
                proc.StandardInput.Close();
                proc.WaitForExit(1000); // wait up to 5 seconds for command to execute




                //Console.WriteLine( $"peek : {proc.StandardOutput.Peek()}");
//                ret += proc.StandardOutput.ReadToEnd();
                while (proc.StandardOutput.Peek() > 0)
                {
                    string read = $"{proc.StandardOutput.ReadLine()}";                    
                    ret += read + "\n";
                }

                ret += proc.StandardError.ReadToEnd();
                
           //     Console.WriteLine(ret);
           //     Console.WriteLine("---------------------------------------------");
            }
            return ret;
        }

        public bool createMakeFile(FileModel fm)
        {
            string pkg = Global.gFilesVM.FolderName;
            if (pkg == "") pkg = "test";
            string lib = "starapi.a";
            if (Global.gAPIHeader == "STAR_HSBI.h")
                lib = "hsbiapi.a";

            string text = $"SOURCE={fm.Name}\n" +
                        $"FOLDER={pkg}\n" +
                        $"CC={Global.gCompiler}\n" +
                        $"STARLIB = $(LIBPATH){lib}\n" +
                        $"CFLAGS = -fno-operator-names \n" +  // -g 옵션 
                        $"LIBS = -L{Global.gEAPIPath} -Wl,--no-as-needed -rdynamic -ldl -lEAPI_Library -lI2CReset -pthread\n" +
                        $"LIBPATH = {Global.gAPIPath}\n" +
                        $"HEADPATH = {Global.gHeadPath}\n" +
                        $"BIN = $(FOLDER).pgm\n\n" +
                        $"all: $(PATBIN) $(BIN)\n" +
                        $"	@echo Compiled PGM:$(FOLDER)\n\n" +
                        $".SILENT: \n" +
                        $"clean:	\n" +
                        $"	rm -fr $(BIN) *.o\n\n" +
                        $"$(BIN) : $(SOURCE).cpp\n" +
                        $"	$(CC) $(CFLAGS) -o $@ $< -I$(HEADPATH) -L./ $(STARLIB) $(LIBS)\n";
            /*
             *             string text = $"SOURCE={fm.Name}\n" +
                        $"FOLDER={pkg}\n" +
                        $"CC={Global.gCompiler}\n" +
                        $"STARLIB = $(LIBPATH)starapi.a\n" +
                        $"CFLAGS = -fno-operator-names \n" +
                        $"LIBS = -L/star/eapi -Wl,--no-as-needed -rdynamic -ldl -lEAPI_Library -lI2CReset -pthread\n" +
                        $"LIBPATH = /star/StarAPI/\n" +
                        $"BIN = $(FOLDER).pgm\n\n" +
                        $"all: $(PATBIN) $(BIN)\n" +
                        $"	@echo Compiled PGM:$(FOLDER)\n\n" +
                        $".SILENT: \n" +
                        $"clean:	\n" +
                        $"	rm -fr $(BIN) *.o\n\n" +
                        $"$(BIN) : $(SOURCE).cpp\n" +
                        $"	$(CC) $(CFLAGS) -o $@ $< -I$(LIBPATH) -L./ $(STARLIB) $(LIBS)\
            */

            string makePath = Global.gFilesVM._folderPath + @"\makefile";
            File.WriteAllText( makePath, text);

            return true;
        }

        public void delFile(string name)
        {
            string file = Global.gFilesVM._folderPath + @"\" + name;
            if (System.IO.File.Exists(file))
            {
                try
                {
                    System.IO.File.Delete(file);
                }
                catch (System.IO.IOException e)
                {
                    // handle exception
                }
            }
        }

        public Visibility ShowLogo { get; set; } = Visibility.Visible;
        public MainVM()
        {
            Global.gMainVM = this;
            initCommand();
            Title = "";

            loadRecent();
            if (Global.gShowLogo) ShowLogo = Visibility.Visible;
            else ShowLogo = Visibility.Hidden;

        }


        #region MVVM Command Set
        public static CommandBindingCollection Commands = new CommandBindingCollection();
        /*
         * scintilla SuppressKeyPress = true 가 RoutedEvent 먹어버리는듯..
         *  
        public static RoutedCommand CmdMenu = new RoutedCommand("CmdMenu", typeof(ApplicationCommands));
        void canExcute_CmdMenu(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        void Excute_CmdMenu(object sender, ExecutedRoutedEventArgs e)
        {
            if (e.Parameter.ToString() == "OPEN PROJECT")
            {
            }
        }
        */
            void initCommand()
        {
            //         Commands.Add(new CommandBinding(CmdMenu, Excute_CmdMenu, canExcute_CmdMenu));
        }
        #endregion MVVM Command Set


    }
}
