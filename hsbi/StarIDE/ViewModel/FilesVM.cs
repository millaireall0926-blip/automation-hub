using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarBase;
using System.Collections.ObjectModel;
using StarIDE.Model;
using System.Windows.Input;
using Microsoft.WindowsAPICodePack.Dialogs;
using System.IO;
using System.Windows.Media;
using System.Windows;
using System.Xml.Linq;
using System.Reflection;

namespace StarIDE.ViewModel
{
    public class FilesVM : BaseVM
    {
        public Visibility _compVisible = Visibility.Hidden;
        public Visibility CompVisible
        {
            get => _compVisible;
            set { _compVisible = value; OnPropertyChanged(nameof(CompVisible)); }
        }

        #region Dock View
        string _folderName;
        public string FolderName
        {
            get { return _folderName; }
            set { _folderName = value; OnPropertyChanged(nameof(FolderName)); }

        }

        ObservableCollection<FileModel> _fileModels = new ObservableCollection<FileModel>();
        public ObservableCollection<FileModel> FileModels
        {
            get { return _fileModels; }
            set { _fileModels = value; OnPropertyChanged(nameof(FileModels)); }
        }


        ObservableCollection<MakedModel> _makedModels = new ObservableCollection<MakedModel>();
        public ObservableCollection<MakedModel> MakedModels
        {
            get { return _makedModels; }
            set { _makedModels = value; OnPropertyChanged(nameof(MakedModels)); }
        }

        #endregion Dock View

        #region Inside Members
        public string _folderPath;

        public SolidColorBrush _colTRAN = new SolidColorBrush(Colors.Transparent);
        public SolidColorBrush _colNOR = new SolidColorBrush(Colors.White);
        public SolidColorBrush _colOK = new SolidColorBrush(Colors.LightGreen);
        public SolidColorBrush _colERR = new SolidColorBrush(Colors.IndianRed);
        

        #endregion Inside Members

        public FilesVM()
        {
            Global.gFilesVM = this;
        }



        public MakedModel findMakeFile(string name)
        {
            foreach (MakedModel model in MakedModels)
            {
                if (model.Name == name)
                    return model;
            }
            return null;
        }

        public void checkFolder()
        {
            string path = _folderPath;
            string[] folderFiles = Directory.GetFiles(path, "*.*");

            Collection<string> createdfiles = new Collection<string>();

            foreach (string s in folderFiles)
            {
                string file = Global.getLastText(s, "\\");
                string ext = Global.getLastText(file, ".");
                string name = Global.getLastText(file, ".", true);

                MakedModel mm = MakedModels.FirstOrDefault(a => a.Name == name);

                if (!(ext == "mpa" || ext == "pgm")) continue;

                if (mm != null) mm.checkCompiled = true;
                else
                {
                    if (ext == "mpa")                      
                        mm = new MakedModel() { Name = name, Type = MAKED_TYPE.MPA };
                    else if (ext == "pgm")
                        mm = new MakedModel() { Name = name, Type = MAKED_TYPE.PGM };
                    MakedModels.Add(mm);
                    mm.checkCompiled = true;
                    mm.start_Color(true);                    
                }
                
            }

            while (true)
            {
                if (MakedModels.Count == 0) break;

                bool allPass = true;
                MakedModel mm = null;
                for (int i = 0; i < MakedModels.Count; i++)
                {
                    mm = MakedModels[i];
                    if (!mm.checkCompiled)
                    {
                        allPass = false;
                        break;
                    }
                }
                if (allPass)
                {
                    foreach (MakedModel checkmm in MakedModels)
                        checkmm.checkCompiled = false;
                    break;
                }

                MakedModels.Remove(mm);
            }
                        
        }

        public void clearFolder()
        {
            FileModels.Clear();
            MakedModels.Clear();

        }

        public void closeFolder()
        {
            clearFolder();
            FolderName = "";
            Global.gMainVM.Title = "";            
        }

        public int getFileIndex(FileModel fm)
        {
            int index = 0;
            foreach (FileModel model in FileModels)
            {
                if (fm == model)
                    break;
                index++;
            }
            return index;
        }

        public void biuldtemContext(string menue, MakedModel mm)
        {
            if (mm == null) return;
            if (menue == "Delete")
            {
                string path = _folderPath + @"\" + mm.Name + "." + mm.Type.ToString().ToLower();
                if (File.Exists(path)) { File.Delete(path); }
                MakedModels.Remove(mm);
            }else if (menue == "Delete All")
            {
                foreach (MakedModel model in MakedModels)
                {
                    string path = _folderPath + @"\" + model.Name + "." + model.Type.ToString().ToLower();
                    if (File.Exists(path)) { File.Delete(path); }
                }
                MakedModels.Clear();
            }
        }

        public void doContextMenu(string menu, FileModel fm)
        {
            if (fm == null) return;
            int pos = getFileIndex(fm);

            if (menu == "New" || menu == "New Folder")
            {
                FileModel model = new FileModel();
                model.Type = FILE_TYPE.ETC;
                string name = "NewOne";
                if (menu == "New Folder")
                {
                    model.Type = FILE_TYPE.FOLDER;
                    name = "NewFold";
                }                

                model.Name = name;                
                model._fmParent = fm._fmParent;                
                model.Path = model.getPath();                

                if (model._fmParent != null)
                {
                    model._fmParent = fm._fmParent;
                    model._fmParent.InnerFiles.Add(model);
                }
                else if (fm.Type == FILE_TYPE.FOLDER)
                {
                    model._fmParent = fm;
                    model.Path = model.getPath();
                    fm.InnerFiles.Add(model);
                }
                else
                    FileModels.Insert(pos, model);

                int index = 1;
                if (menu == "New")
                {
                    while (true)
                    {
                        if (!File.Exists(model.Path)) { File.Create(model.Path).Close(); break; }
                        model.Name = $"{name}{index++}";
                        model.Path = model.getPath();
                    }
                    
                }else if (menu == "New Folder")
                {
                    while (true)
                    {
                        DirectoryInfo di = new DirectoryInfo(model.Path);
                        if (!di.Exists) { di.Create(); break; }
                        model.Name = $"{name}{index++}";
                        model.Path = model.getPath();
                    }
                }
                model._originName = model.Name;
                model.setTextType(true);
            }
            else if (menu == "Copy")
            {
                if (fm.Type == FILE_TYPE.FOLDER)
                {
                    HandyControl.Controls.MessageBox.Show("Folder Copy Not Support This Tool\r\n" +
                        "Edit In Explore", "Folder Copy", MessageBoxButton.OK, MessageBoxImage.Hand);
                    return;
                }

                FileModel model = new FileModel();
                model.Name = fm.Name + "_copy";
                model.Type = fm.Type;
                model._fmParent = fm._fmParent;
                model.Path = model.getPath();

                int index = 1;
                while (true)
                {
                    if (!File.Exists(model.Path)) { File.Create(model.Path).Close(); break; }
                    model.Name = $"{model.Name}{index++}";
                    model.Path = model.getPath();
                }

                if (model._fmParent != null)
                {
                    model._fmParent = fm._fmParent;
                    model._fmParent.InnerFiles.Add(model);
                }               
                else
                    FileModels.Insert(pos, model);

                model._originName = model.Name;
                model.setTextType(true);

            }
            else if (menu == "Delete")
            {
                MessageBoxResult res = HandyControl.Controls.MessageBox.Show($"Do You Wan\'t Delete \"{fm.Name}\"", "Delete", MessageBoxButton.OKCancel, MessageBoxImage.Warning);
                if (res != MessageBoxResult.OK) return;

                if (fm._edit != null)
                {
                    Global.gDockVM.closeDoc(fm.Name);
                }

                if (fm.Type == FILE_TYPE.FOLDER)
                {
                    DirectoryInfo di = new DirectoryInfo(fm.Path);
                    if (di.Exists) di.Delete(true);

                }
                else
                    File.Delete(fm.Path);


                if (fm._fmParent != null)
                {
                    fm._fmParent.InnerFiles.Remove(fm);
                }
                else
                    FileModels.Remove(fm);
            }
            else if (menu == "Rename")
            {
                if (fm.Type == FILE_TYPE.FOLDER)
                {
                    if (fm.InnerFiles.Count > 0)
                    {
                        HandyControl.Controls.MessageBox.Show("Folder Rename Not Support This Tool If Folder has Files\r\n " +
                            "Edit In Explore", "Folder Rename", MessageBoxButton.OK, MessageBoxImage.Hand);
                        return;
                    }
                }
                fm._originName = fm.Name;
                fm.setTextType(true);
            }
            else if (menu == "Open Explorer")
            {
                System.Diagnostics.Process.Start(_folderPath);
            }
        }

        public void loadFolder(string path, ObservableCollection<FileModel> models, FileModel fmParent = null)
        {
            if (path == "" || path == null) return; 
            if (fmParent == null)
            {
                _folderPath = path;
                Global.gMainVM.Title = path;
                FolderName = Global.getLastText(path, "\\");
                clearFolder();
                models = FileModels;
            }

            string[] tmpPath = Directory.GetDirectories(path, "*");

            foreach (string s in tmpPath)
            {
                FileModel model = new FileModel();
                model.Name = Global.getLastText(s, "\\");
                model.Path = s;
                if (model.Name == "") continue;
                model.Type = FILE_TYPE.FOLDER;
                models.Add(model);
                Console.WriteLine(model.Name);
                loadFolder(s, model.InnerFiles, model); // 폴더를 만나면 하위로 계속 탐색을 진행
            }

            string[] folderFiles = Directory.GetFiles(path, "*.*");
            foreach (string s in folderFiles)
            {
                FileModel model = new FileModel();
                model._fmParent = fmParent;
                string file = Global.getLastText(s, "\\");
                string ext = Global.getLastText(file, ".");
                string name = Global.getLastText(file, ".", true);

                if (fmParent == null)
                {
                    if (ext == "mpa")
                    {
                        MakedModels.Add(new MakedModel() { Name = name, Type = MAKED_TYPE.MPA });
                        continue;
                    }
                    else if (ext == "pgm")
                    {
                        MakedModels.Add(new MakedModel() { Name = name, Type = MAKED_TYPE.PGM });
                        continue;
                    }
                }

                model.checkExt(ext);
                if (ext == "") name = file;

                // Path Save After Setting Extention
                model.Path = s;
                model.Name = name;
                models.Add(model);

                if (fmParent == null && (ext == "cpp"))
                    Global.gDockVM.addDocPane(model);

                //                if ((model.Type == FILE_TYPE.HEAD) && (!top)) // Header가 다른 폴더에 있는 경우
                //                    FileModels.Add(model);
            }

            if (fmParent == null) // Folder Open 완료 후 SDEF List Update            
            {
                Global.gAssistVM.updateSdef();
                Global.gMainVM.setRecentModel();
            }
        }
            
    }
}
