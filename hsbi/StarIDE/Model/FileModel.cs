using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using System.Xml.Linq;
using StarBase;
using static System.Net.Mime.MediaTypeNames;

namespace StarIDE.Model
{
    public enum FILE_TYPE : int
    {
        CPP = 0,
        PAT = 1,
        HEAD = 2,
        VEC,
        ETC,
        FOLDER
    }

    public enum MAKED_TYPE : int
    {
        PGM = 0,
        MPA = 1

    }

    public class FileModel : BaseVM
    {
        public FILE_TYPE _type;
        public FILE_TYPE Type
        {
            get { return _type; }
            set { _type = value; OnPropertyChanged(nameof(Type)); }
        }

        public string _extension = ""; // ETC File 용

        public string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }

        SolidColorBrush _color;
        public SolidColorBrush Color
        {
            get { return _color; }
            set { _color = value; OnPropertyChanged(nameof(Color)); }
        }

        public FileModel _fmParent = null;


        public ObservableCollection<FileModel> _innerFiles = new ObservableCollection<FileModel>();
        public ObservableCollection<FileModel> InnerFiles
        {
            get { return _innerFiles; }
            set { _innerFiles = value; OnPropertyChanged(nameof(InnerFiles)); }
        }


        #region File Modify Rename


        public Visibility _isTextBlock = Visibility.Visible;
        public Visibility IsTextBlock
        {
            get { return _isTextBlock; }
            set { _isTextBlock = value; OnPropertyChanged(nameof(IsTextBlock)); }
        }

        public Visibility _isTextBox = Visibility.Hidden;
        public Visibility IsTextBox
        {
            get { return _isTextBox; }
            set {
                _isTextBox = value;
                OnPropertyChanged(nameof(IsTextBox));
            }
        }

        public void checkExt(string ext)
        {
            switch (ext)
            {
                case "pat": Type = FILE_TYPE.PAT; break;
                case "cpp":
                case "c": Type = FILE_TYPE.CPP; break;
                case "h": Type = FILE_TYPE.HEAD; break;
                case "vec": Type = FILE_TYPE.VEC; break;
                default:
                    Type = FILE_TYPE.ETC;
                    break;
            }
            _extension = ext;
        }

        public void getParentPath(FileModel fm, ref string path)
        {
            path = @"\" + fm._fmParent.Name + @"\" + path;
            if (fm._fmParent._fmParent != null)
                getParentPath(fm, ref path);
        }

        public string getPath()
        {
            string path;
            if (_fmParent != null)
            {
                path = Name;
                getParentPath(this, ref path);
                path = Global.gFilesVM._folderPath + path;
            }
            else
            {
                path = Global.gFilesVM._folderPath + @"\" + Name;
            }
            return path;
        }

        public void setTextType(bool isModify)
        {
            if (isModify)
            {
                _originPath = Path;
                IsTextBlock = Visibility.Collapsed;
                IsTextBox = Visibility.Visible;
            } else
            {
                if (IsTextBox == Visibility.Visible) // File Change
                {
                    Path = getPath();

                    if (Name.IndexOf(".") != -1)
                    {
                        string ext = Global.getLastText(Name, ".");
                        checkExt(ext);
                        Name = Global.getLastText(Name, ".", true);
                        //                        Path = _folder

                    } else if (_extension != "")
                        Path += "." + _extension;

                }
                IsTextBlock = Visibility.Visible;
                IsTextBox = Visibility.Collapsed;
            }

        }

        public string _originPath = ""; // File Path 변경 Check용
        public string _originName = ""; // file error시 복구용
        public void doRename()
        {

            setTextType(false);
            string newone = Path;
            if (_originPath != newone)
            {
                if (Type != FILE_TYPE.FOLDER)
                {
                    if (File.Exists(newone))
                    {
                        HandyControl.Controls.MessageBox.Show($"Already \'{Name}\' File Exist\r\n" +
                        "Change File Name", "Rename", MessageBoxButton.OK, MessageBoxImage.Hand);
                        Name = _originName;
                        Path = _originPath;
                        setTextType(true);
                        return;
                    }

                    File.Move(_originPath, newone);
                    if (_edit != null)
                        _edit._pane.Title = Name;
                }
                else
                {
                    DirectoryInfo di = new DirectoryInfo(newone);
                    if (di.Exists)
                    {
                        HandyControl.Controls.MessageBox.Show($"Already \'{Name}\' Folder Exist\r\n" +
                        "Change Folder Name", "Rename", MessageBoxButton.OK, MessageBoxImage.Hand);
                        Name = _originName;
                        Path = _originPath;
                        setTextType(true);
                        return;
                    }
                    di = new DirectoryInfo(_originPath);
                    di.MoveTo(newone);
                }
            }
        }


        public DelegateCommand ReNameCommand { get; set; }

        #endregion

        public string _path;
        public string Path
        {
            get { return _path; }
            set
            {
                _path = value;

                if (Type == FILE_TYPE.PAT || Type == FILE_TYPE.CPP)
                {
                    if (_path.IndexOf(":") == -1)
                        _wslPath = "";
                    else
                    {
                        string[] dir = _path.Split(':');
                        dir[0] = dir[0].ToLower();
                        _wslPath = "/mnt/" + dir[0] + dir[1];
                        _wslPath = _wslPath.Replace(@"\", "/");

                    }
                }
            }
        }

        public bool _isCompile = false;
        public EditModel _edit = null;
        public string _wslPath;

        public DispatcherTimer _timerCol = new DispatcherTimer();

        public FileModel()
        {
            Color = Global.gFilesVM._colNOR;
            _timerCol.Interval = TimeSpan.FromSeconds(4);
            _timerCol.Tick += tick_Color;

            ReNameCommand = new DelegateCommand(p => doRename());
        }

        public void start_Color(bool isOK) // 
        {
            if (isOK)
            {
                _timerCol.Interval = TimeSpan.FromSeconds(4);
                Color = Global.gFilesVM._colOK;
            }
            else
            {
                _timerCol.Interval = TimeSpan.FromSeconds(6);
                Color = Global.gFilesVM._colERR;
            }
            _timerCol.Start();
        }

        void tick_Color(object sender, EventArgs e)
        {
            _timerCol.Stop();
            Color = Global.gFilesVM._colNOR;
        }

    }

    public class MakedModel : BaseVM
    {
        public MAKED_TYPE _type;
        public MAKED_TYPE Type
        {
            get { return _type; }
            set { _type = value; OnPropertyChanged(nameof(Type)); }
        }

        public string _name;
        public string Name
        {
            get { return _name; }
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }

        SolidColorBrush _color;
        public SolidColorBrush Color
        {
            get { return _color; }
            set { _color = value; OnPropertyChanged(nameof(Color)); }
        }

        public bool checkCompiled = false; // Check false = removed

        public DispatcherTimer _timerCol = new DispatcherTimer();

        public MakedModel()
        {
            Color = Global.gFilesVM._colTRAN;
            _timerCol.Interval = TimeSpan.FromSeconds(4);
            _timerCol.Tick += tick_Color;
        }

        public void start_Color(bool isOK) // 
        {
            if (isOK)
            {
                _timerCol.Interval = TimeSpan.FromSeconds(4);
                Color = Global.gFilesVM._colOK;
            }
            else
            {
                _timerCol.Interval = TimeSpan.FromSeconds(6);
                Color = Global.gFilesVM._colERR;
            }
            _timerCol.Start();
        }

        void tick_Color(object sender, EventArgs e)
        {
            _timerCol.Stop();
            Color = Global.gFilesVM._colTRAN;
        }
    }

    public class RecentModel : BaseVM
    {
        public string _path;
        public string Path
        {
            get => _path;
            set { _path = value; OnPropertyChanged(nameof(Path));}
        }
        public string _name;
        public string Name
        {
            get => _name;
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }



        public RecentModel()
        {
            
        }

    }

}
