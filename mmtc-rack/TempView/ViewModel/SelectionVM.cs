using HandyControl.Controls;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
using TempView.Base;


namespace TempView.ViewModel
{
    public enum FILTER : int
    {
        NONE = 0,
        NAME = 1,
        TIME = 2
    };

    public class DateFile
    {
        public DateFile()
        {
            files = new Collection<string>();
        }

        public DateTime dateTime;
        public Collection<string> files;
    }
    public class SelectionVM : BaseVM
    {
        public StackPanel xSP_File;

        public string FindName { get; set; } = "";
        public DelegateCommand FindCmd { get; set; }
        public DateTime SelFrom { get; set; } = DateTime.Now;
        public DateTime SelTo { get; set; } = DateTime.Now;

        public Button _prevSelect = null;
        public bool _prevWater = false;
        public SolidColorBrush _selectColor = new SolidColorBrush(Colors.CadetBlue);
        public SolidColorBrush _dismissColor = new SolidColorBrush(Colors.White);
        public SolidColorBrush _dismissWaterColor = new SolidColorBrush(Colors.LightGray);

        public SelectionVM() 
        {
            FindCmd = new DelegateCommand(p => { findFile((FILTER)Convert.ToInt32(p)); });
            Global.gSelectVM = this;
        }

        public void clearFileList()
        {
            xSP_File.Children.Clear();
        }

        public void findFile(FILTER filter = FILTER.NONE)
        {
            clearFileList();

            string[] datas;
            try
            {
                string path = Global.gMainVM.DataPath;
                datas = Directory.GetFiles(path, "*" + ".csv", SearchOption.TopDirectoryOnly);
            }
            catch (Exception ex)
            {
                Growl.InfoGlobal(ex.Message);
                return;
            }

            List<string> normals = new List<string>();
            List<string> waters = new List<string>();
            List<string> sorts = new List<string>();
            foreach (var name in datas)
            {
                if (name.IndexOf("_Water") < 0)
                    normals.Add(name);
                else
                    waters.Add(name);
            }
            sorts.AddRange(normals);
            sorts.AddRange(waters);

            Collection<DateFile> dateFiles = new Collection<DateFile>();
            foreach (string data in sorts)
            {
                var info = new FileInfo(data);
                if (!info.Exists) continue;

                // --- 핵심 변경 사항 1: 파일 이름에서 날짜 파싱 ---
                DateTime fileDate;
                if (!TryParseDateFromFileName(info.Name, out fileDate))
                {
                    // 파싱 실패 시, 기존 방식대로 파일의 수정 날짜를 사용 (폴백)
                    fileDate = info.LastWriteTime;
                }
                // ------------------------------------------------

                if (filter == FILTER.NAME)
                {
                    if (FindName != "")
                        if (data.IndexOf(FindName) == -1)
                            continue;
                }
                if (filter == FILTER.TIME)
                {
                    // --- 핵심 변경 사항 2: 필터링 기준을 파싱된 날짜로 변경 ---
                    int from = DateTime.Compare(fileDate, SelFrom);
                    int to = DateTime.Compare(SelTo.AddDays(1), fileDate);
                    if (!(from >= 0 && to >= 0)) continue;
                }

                // --- 핵심 변경 사항 3: 그룹화 기준을 파싱된 날짜로 변경 ---
                DateFile df = dateFiles.FirstOrDefault(a => a.dateTime.Date == fileDate.Date);
                if (df == null)
                {
                    df = new DateFile();
                    df.dateTime = fileDate; // 그룹의 대표 날짜를 파싱된 날짜로 설정
                    dateFiles.Add(df);
                }

                int st = data.IndexOf(Global.gMainVM.DataPath) + Global.gMainVM.DataPath.Length + 1;
                int len = data.Length - (st + 4); // .csv는 4글자
                if (len <= 0) continue;
                string file = data.Substring(st, len);

                df.files.Add(file);
            }

            var list2 = new List<DateFile>();
            list2.AddRange(dateFiles);
            // 이 정렬 로직은 df.dateTime을 기준으로 하므로, 위에서 값을 잘 설정했다면 그대로 작동합니다.
            list2.Sort((a, b) => b.dateTime.CompareTo(a.dateTime));
            Console.WriteLine("[{0}]", string.Join(", ", list2));

            if (dateFiles.Count == 0) return;

            bool expand = true;
            foreach (DateFile df in list2)
            {
                // --- UI 생성 부분은 변경할 필요 없음 ---
                var newExpander = new Expander
                {
                    Header = df.dateTime.Date.ToString("yyyy-MM-dd"), // 그룹 헤더도 자연스럽게 파싱된 날짜로 표시됨
                    Margin = new Thickness(5),
                    Background = new SolidColorBrush(Colors.LightSkyBlue),
                    FontWeight = FontWeights.Bold
                };
                var newStackPanel = new StackPanel { };

                foreach (string file in df.files)
                {
                    var newButton = new Button
                    {
                        Content = file,
                        HorizontalAlignment = HorizontalAlignment.Stretch,
                        Margin = new Thickness(3, 1, 3, 0),
                        Background =
                            (file.IndexOf("_Water") < 0)
                                ? new SolidColorBrush(Colors.White)
                                : new SolidColorBrush(Colors.LightGray)
                    };
                    newButton.Click += (s, e) => {
                        Button bt = (Button)s;
                        openFile(bt.Content.ToString());
                        if (_prevSelect != null)
                            _prevSelect.Background = (_prevWater)
                                ? _dismissWaterColor : _dismissColor;

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

        private bool TryParseDateFromFileName(string fileName, out DateTime parsedDate)
        {
            // 정규식을 사용하여 _YYYY_MM_DD_ 패턴을 찾습니다.
            var match = Regex.Match(fileName, @"_(\d{4}_\d{2}_\d{2})_");

            if (match.Success)
            {
                // "2025_07_29" 같은 형식의 문자열을 추출합니다.
                string dateString = match.Groups[1].Value;

                // "yyyy_MM_dd" 형식에 맞춰 DateTime으로 파싱합니다.
                if (DateTime.TryParseExact(dateString, "yyyy_MM_dd", CultureInfo.InvariantCulture, DateTimeStyles.None, out parsedDate))
                {
                    return true;
                }
            }

            // 패턴을 찾지 못하거나 파싱에 실패하면 false를 반환합니다.
            parsedDate = default;
            return false;
        }

        public void openFile(string lot)
        {
            Global.gMainVM.selectFile(lot);
        }



    }
}
