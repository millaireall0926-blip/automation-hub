using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Forms;
using Microsoft.WindowsAPICodePack.Dialogs;

namespace JsonBone
{
    public class MainVM : BaseVM
    {
        public ObservableCollection<FileInfo> FileList { get; set; } = new ObservableCollection<FileInfo>();
        public ObservableCollection<SpineRowModel> SpineDataList { get; set; } = new ObservableCollection<SpineRowModel>();

        private string _selectedFolderPath;
        public string SelectedFolderPath
        {
            get => _selectedFolderPath;
            set { _selectedFolderPath = value; PC(nameof(SelectedFolderPath)); }
        }

        public ICommand SelectFolderCommand { get; }
        public ICommand ConvertCommand { get; }
        public ICommand CopyCommand { get; }

        public MainVM()
        {
            SelectFolderCommand = new DelegateCommand(OnSelectFolder);
            ConvertCommand = new DelegateCommand(OnConvert);
            CopyCommand = new DelegateCommand(obj => CopyAsMergedTable());
        }

        private readonly string _settingFilePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "user_settings.txt");

        private void OnSelectFolder(object obj)
        {
            // 1. 저장된 마지막 폴더 경로 읽기
            string lastPath = string.Empty;
            if (File.Exists(_settingFilePath))
            {
                lastPath = File.ReadAllText(_settingFilePath);
            }

            // 2. 탐색기 방식의 다이얼로그 생성
            using (var dialog = new CommonOpenFileDialog())
            {
                dialog.IsFolderPicker = true;           // 폴더 선택 모드 활성화
                dialog.Title = "분석할 Spine JSON 폴더를 선택하세요";

                // 마지막 경로가 유효할 때만 시작 위치 지정
                if (!string.IsNullOrEmpty(lastPath) && Directory.Exists(lastPath))
                {
                    dialog.InitialDirectory = lastPath;
                }

                if (dialog.ShowDialog() == CommonFileDialogResult.Ok)
                {
                    string selectedPath = dialog.FileName;
                    SelectedFolderPath = selectedPath;

                    // 3. 선택한 경로 저장
                    File.WriteAllText(_settingFilePath, selectedPath);

                    // 4. 파일 리스트 업데이트
                    UpdateFileList(selectedPath);
                }
            }
        }

        private void UpdateFileList(string path)
        {
            FileList.Clear();
            DirectoryInfo dir = new DirectoryInfo(path);
            if (dir.Exists)
            {
                // 1. .json 파일들만 가져옵니다.
                var allFiles = dir.GetFiles("*.json");

                // 2. 커스텀 정렬 로직 적용
                var sortedFiles = allFiles.OrderBy(f => {
                    string name = Path.GetFileNameWithoutExtension(f.Name);

                    // 정렬할 때만 "__on__" 이나 공백을 제거한 이름을 기준으로 삼습니다.
                    string sortKey = name.Replace("__on__", "").Trim();

                    return sortKey;
                }).ThenBy(f => f.Name); // 이름이 같으면 원본 이름순으로 한 번 더 정렬

                foreach (FileInfo file in sortedFiles)
                {
                    FileList.Add(file);
                }
            }
        }
        private void OnConvert(object obj)
        {
            var selectedItems = obj as System.Collections.IList;
            if (selectedItems == null || selectedItems.Count == 0) return;

            SpineDataList.Clear();
            int fileCounter = 1; // 순번 카운터

            foreach (var item in selectedItems)
            {
                FileInfo file = item as FileInfo;
                if (file == null) continue;

                try
                {
                    // 1. 확장자를 뺀 순수 파일명 추출
                    string pureFileName = Path.GetFileNameWithoutExtension(file.Name);

                    // [핵심 수정] 무조건 이미지를 찾는 게 아니라, "__on__"이 포함된 파일만 찾습니다.
                    string imagePathToSet = string.Empty;
                    if (file.Name.Contains("__on__"))
                    {
                        string pngPath = Path.Combine(file.DirectoryName, pureFileName + ".png");
                        if (File.Exists(pngPath))
                        {
                            imagePathToSet = pngPath;
                        }
                    }

                    string jsonText = File.ReadAllText(file.FullName);
                    JObject json = JObject.Parse(jsonText);

                    // 1. "num" 슬롯 추출
                    var numSlots = new List<string>();
                    JArray slotsArray = json["slots"] as JArray;
                    if (slotsArray != null)
                    {
                        foreach (var slot in slotsArray)
                        {
                            string sName = slot["name"]?.ToString();
                            if (!string.IsNullOrEmpty(sName) && sName.IndexOf("num", StringComparison.OrdinalIgnoreCase) >= 0)
                                numSlots.Add(sName);
                        }
                    }

                    // 2. 애니메이션 추출
                    var animNames = new List<string>();
                    JObject animations = json["animations"] as JObject;
                    if (animations != null)
                    {
                        foreach (var anim in animations.Properties()) animNames.Add(anim.Name);
                    }

                    // 3. 행 개수 결정
                    int rowCount = Math.Max(animNames.Count, numSlots.Count);
                    if (rowCount == 0) rowCount = 1;

                    for (int i = 0; i < rowCount; i++)
                    {
                        SpineDataList.Add(new SpineRowModel
                        {
                            // 첫 번째 줄일 때만 찾은 이미지 경로를 넣어줍니다. (__on__이 없으면 어차피 빈 값)
                            ImagePath = (i == 0) ? imagePathToSet : string.Empty,
                            No = (i == 0) ? fileCounter.ToString() : string.Empty,
                            // 미리 구해둔 pureFileName 재사용
                            FileName = (i == 0) ? pureFileName : string.Empty,
                            AnimationName = (i < animNames.Count) ? animNames[i] : string.Empty,
                            Slot = (i < numSlots.Count) ? numSlots[i] : string.Empty,

                            Duration = string.Empty,
                            Skin = string.Empty,
                            Description = string.Empty,
                            IsLoop = false
                        });
                    }
                    fileCounter++;
                }
                catch (Exception ex)
                {
                    System.Windows.MessageBox.Show(file.Name + " 분석 실패: " + ex.Message);
                }
            }
        }
        /*
        private void OnConvert(object obj)
        {
            var selectedItems = obj as System.Collections.IList;
            if (selectedItems == null || selectedItems.Count == 0) return;

            SpineDataList.Clear();
            int fileCounter = 1; // 순번 카운터

            foreach (var item in selectedItems)
            {
                FileInfo file = item as FileInfo;
                if (file == null) continue;

                try
                {
                    string jsonText = File.ReadAllText(file.FullName);
                    JObject json = JObject.Parse(jsonText);

                    // 1. "num" 슬롯 추출
                    var numSlots = new List<string>();
                    JArray slotsArray = json["slots"] as JArray;
                    if (slotsArray != null)
                    {
                        foreach (var slot in slotsArray)
                        {
                            string sName = slot["name"]?.ToString();
                            if (!string.IsNullOrEmpty(sName) && sName.IndexOf("num", StringComparison.OrdinalIgnoreCase) >= 0)
                                numSlots.Add(sName);
                        }
                    }

                    // 2. 애니메이션 추출
                    var animNames = new List<string>();
                    JObject animations = json["animations"] as JObject;
                    if (animations != null)
                    {
                        foreach (var anim in animations.Properties()) animNames.Add(anim.Name);
                    }

                    // 3. 행 개수 결정
                    int rowCount = Math.Max(animNames.Count, numSlots.Count);
                    if (rowCount == 0) rowCount = 1;

                    for (int i = 0; i < rowCount; i++)
                    {
                        SpineDataList.Add(new SpineRowModel
                        {
                            ImagePath = string.Empty,
                            No = (i == 0) ? fileCounter.ToString() : string.Empty,
                            // 확장자 없이 이름만 넣기
                            FileName = (i == 0) ? Path.GetFileNameWithoutExtension(file.Name) : string.Empty,
                            AnimationName = (i < animNames.Count) ? animNames[i] : string.Empty,
                            Slot = (i < numSlots.Count) ? numSlots[i] : string.Empty,

                            Duration = string.Empty,
                            Skin = string.Empty,
                            Description = string.Empty,
                            IsLoop = false
                        });
                    }
                    fileCounter++;
                }
                catch (Exception ex)
                {
                    System.Windows.MessageBox.Show(file.Name + " 분석 실패: " + ex.Message);
                }
            }
        }
        */
        public void CopyAsMergedTable()
        {
            if (SpineDataList.Count == 0) return;

            // 포커스 해제 (텍스트 입력 중일 때 확정)
            System.Windows.Input.Keyboard.ClearFocus();

            var mainWindow = System.Windows.Application.Current.Windows.OfType<MainWindow>().FirstOrDefault();
            var grid = mainWindow?.FindName("MainDataGrid") as System.Windows.Controls.DataGrid;
            if (grid == null) return;

            StringBuilder html = new StringBuilder();
            html.Append("<table border='1' style='border-collapse:collapse; font-family:Arial, sans-serif;'>");
            html.Append("<thead><tr style='background-color:#F4F5F7;'>");

            // 1. 헤더에 직접 박아둔 TextBox에서 Text를 읽어옵니다. (100% 확실한 방법)
            foreach (var column in grid.Columns)
            {
                string headerTitle = string.Empty;

                // 컬럼의 헤더가 TextBox 컨트롤이라면, 유저가 입력한 글자를 바로 가져옵니다.
                if (column.Header is System.Windows.Controls.TextBox tb)
                {
                    headerTitle = tb.Text;
                }
                else
                {
                    headerTitle = column.Header?.ToString() ?? "";
                }

                html.Append($"<th style='padding:8px; border:1px solid #ccc; font-weight:bold;'>{headerTitle}</th>");
            }
            html.Append("</tr></thead><tbody>");

            // 2. 데이터 그룹화 및 병합 복사 진행
            List<SpineRowModel> tempGroup = new List<SpineRowModel>();
            for (int i = 0; i < SpineDataList.Count; i++)
            {
                if (!string.IsNullOrEmpty(SpineDataList[i].FileName) && i != 0)
                {
                    ProcessGroup(html, tempGroup);
                    tempGroup.Clear();
                }
                tempGroup.Add(SpineDataList[i]);
            }
            if (tempGroup.Count > 0) ProcessGroup(html, tempGroup);

            html.Append("</tbody></table>");

            System.Windows.Clipboard.SetText(WrapHtmlForClipboard(html.ToString()), System.Windows.TextDataFormat.Html);
        }
        // 1. 병합 코드를 만드는 함수 (여기가 핵심입니다!)
        private void ProcessGroup(StringBuilder sb, List<SpineRowModel> group)
        {
            if (group.Count == 0) return;
            int rowSpan = group.Count;
            for (int i = 0; i < rowSpan; i++)
            {
                sb.Append("<tr>");
                if (i == 0)
                {
                    string imgTag = "";
                    if (!string.IsNullOrEmpty(group[i].ImagePath))
                    {
                        string base64Image = GetBase64Image(group[i].ImagePath);
                        if (!string.IsNullOrEmpty(base64Image))
                        {
                            // [핵심 수정] 엑셀이 무시할 수 없도록 width 속성을 직접 박아버립니다.
                            imgTag = $"<img src='{base64Image}' width='80' style='display:block; margin:auto;' />";
                        }
                    }

                    // GIF 컬럼
                    sb.Append($"<td rowspan='{rowSpan}' style='width:100px; text-align:center; vertical-align:middle; background:#f9f9f9; border:1px solid #ccc;'>{imgTag}</td>");

                    sb.Append($"<td rowspan='{rowSpan}' style='text-align:center; vertical-align:middle; border:1px solid #ccc;'>{group[i].No}</td>");
                    sb.Append($"<td rowspan='{rowSpan}' style='vertical-align:middle; border:1px solid #ccc;'>{group[i].FileName}</td>");
                }

                sb.Append($"<td style='border:1px solid #ccc; vertical-align:middle;'>{group[i].AnimationName}</td>");
                sb.Append($"<td style='border:1px solid #ccc; vertical-align:middle;'>{group[i].Duration}</td>");

                string checkBoxSymbol = group[i].IsLoop ? "&#9745;" : "&#9744;";
                sb.Append($"<td style='text-align:center; font-size:16px; border:1px solid #ccc; vertical-align:middle;'>{checkBoxSymbol}</td>");

                sb.Append($"<td style='border:1px solid #ccc; vertical-align:middle;'>{group[i].Slot}</td>");
                sb.Append($"<td style='border:1px solid #ccc; vertical-align:middle;'>{group[i].Skin}</td>");
                sb.Append($"<td style='border:1px solid #ccc; vertical-align:middle;'>{group[i].Description}</td>");
                sb.Append("</tr>");
            }
        }

        // 2. 이미지를 Base64(문자열)로 변환해주는 마법의 도우미 함수 추가
        private string GetBase64Image(string imagePath)
        {
            if (string.IsNullOrEmpty(imagePath) || !File.Exists(imagePath))
                return string.Empty;

            try
            {
                byte[] imageBytes = File.ReadAllBytes(imagePath);
                string base64String = Convert.ToBase64String(imageBytes);
                // HTML이 인식할 수 있도록 접두어 추가
                return $"data:image/png;base64,{base64String}";
            }
            catch
            {
                return string.Empty;
            }
        }
        /*
        // HTML rowspan을 적용하여 실제 병합 코드를 만드는 함수
        private void ProcessGroup(StringBuilder sb, List<SpineRowModel> group)
        {
            if (group.Count == 0) return;
            int rowSpan = group.Count;
            for (int i = 0; i < rowSpan; i++)
            {
                sb.Append("<tr>");
                if (i == 0)
                {
                    sb.Append($"<td rowspan='{rowSpan}' style='width:100px; height:60px; background:#f9f9f9; border:1px solid #ccc;'></td>"); // GIF
                    sb.Append($"<td rowspan='{rowSpan}' style='text-align:center; border:1px solid #ccc;'>{group[i].No}</td>");               // No
                    sb.Append($"<td rowspan='{rowSpan}' style='border:1px solid #ccc;'>{group[i].FileName}</td>");                                   // jason
                }
                sb.Append($"<td style='border:1px solid #ccc;'>{group[i].AnimationName}</td>");
                sb.Append($"<td style='border:1px solid #ccc;'>{group[i].Duration}</td>");

                // 체크박스 기호 적용
                string checkBoxSymbol = group[i].IsLoop ? "&#9745;" : "&#9744;";
                sb.Append($"<td style='text-align:center; font-size:16px; border:1px solid #ccc;'>{checkBoxSymbol}</td>");

                sb.Append($"<td style='border:1px solid #ccc;'>{group[i].Slot}</td>");
                sb.Append($"<td style='border:1px solid #ccc;'>{group[i].Skin}</td>");
                sb.Append($"<td style='border:1px solid #ccc;'>{group[i].Description}</td>");
                sb.Append("</tr>");
            }
        }
        */
        private string WrapHtmlForClipboard(string html)
        {
            string header = "Version:0.9\r\nStartHTML:0000000105\r\nEndHTML:0000000000\r\nStartFragment:0000000141\r\nEndFragment:0000000000\r\n";
            string start = "<html><body>";
            string end = "</body></html>";

            string finalHtml = start + html + end;
            string result = header.Replace("0000000000", (header.Length + finalHtml.Length).ToString("D10")) + finalHtml;
            return result;
        }
    }

    public class SpineRowModel : BaseVM
    {
        private string _no = "";
        public string No { get => _no; set { _no = value; PC(nameof(No)); } }

        private string _imagePath = "";
        public string ImagePath { get => _imagePath; set { _imagePath = value; PC(nameof(ImagePath)); } }

        private string _fileName = "";
        public string FileName { get => _fileName; set { _fileName = value; PC(nameof(FileName)); } }

        private string _animationName = "";
        public string AnimationName { get => _animationName; set { _animationName = value; PC(nameof(AnimationName)); } }

        private string _duration = "";
        public string Duration { get => _duration; set { _duration = value; PC(nameof(Duration)); } }

        private bool _isLoop;
        public bool IsLoop
        {
            get => _isLoop;
            set { _isLoop = value; PC(nameof(IsLoop)); }
        }

        private string _slot = "";
        public string Slot { get => _slot; set { _slot = value; PC(nameof(Slot)); } }

        private string _skin = "";
        public string Skin { get => _skin; set { _skin = value; PC(nameof(Skin)); } }

        private string _description = "";
        public string Description { get => _description; set { _description = value; PC(nameof(Description)); } }
    }
}