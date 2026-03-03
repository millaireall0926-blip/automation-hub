using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media;
using StarLauncher.Common;
using System.Drawing;
using System.Windows.Interop;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;

namespace StarLauncher.Model
{
    public class ToolModel : BaseVM
    {
        public string _title;
        public string Title
        {
            get { return _title; }
            set { _title = value; OnPropertyChanged(nameof(Title)); }
        }

        public string _path;
        public string Path
        { 
            get { return _path; } 
            set { _path = value;
                this.Icon = GetIcon();
                OnPropertyChanged(nameof(Path)); }
        }

        public ImageSource _icon;
        public ImageSource Icon
        {
            get { return _icon; }
            set { _icon = value; OnPropertyChanged(nameof(Icon));}
        }

        public string _tip;
        public string Tip
        {
            get { return _tip; }
            set { _tip = value; OnPropertyChanged(nameof(Tip)); }
        }

        public ICommand _command;
        public ICommand Command
        {
            get => _command;
            set => _command = value;
        }

        private ImageSource GetIcon()
        {
            try
            {
                var icon = System.Drawing.Icon.ExtractAssociatedIcon(this.Path);
                var bitmap = icon.ToBitmap();
                var hBitmap = bitmap.GetHbitmap();

                var wpfBitmap =
                    Imaging.CreateBitmapSourceFromHBitmap(
                        hBitmap,
                        IntPtr.Zero,
                        Int32Rect.Empty,
                        BitmapSizeOptions.FromEmptyOptions());

                if (!DeleteObject(hBitmap))
                    throw new System.ComponentModel.Win32Exception();

                return wpfBitmap;
            }
            catch
            {
                return (DrawingImage)Application.Current.TryFindResource("NoneIcon");
            }
        }

        public ToolModel(string path)
        {            
            Path = path;

            this.Command = new DelegateCommand(p =>
            {
                if (File.Exists(path))
                    Process.Start(path);
            });
        }

        [DllImport("gdi32.dll", SetLastError = true)]
        private static extern bool DeleteObject(IntPtr hObject);
    }

}
