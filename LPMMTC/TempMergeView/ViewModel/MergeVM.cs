using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using TempMergeView.Base;

namespace TempMergeView.ViewModel
{
    public class MergeVM : BaseVM
    {
        public DelegateCommand MergeCmd { get; set; }

        public MergeVM()
        {
            MergeCmd = new DelegateCommand(p => { MergeProcess(); });
        }

        private void MergeProcess()
        {
            string GetPath = string.Empty;
            if (Directory.Exists(Global.gMainVM.DataPath))
                GetPath = Global.gMainVM.DataPath.Replace("\\\\", "\\");
            else
                GetPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments); // 기본 폴더 설정

            OpenFileDialog openFileDialog = new OpenFileDialog
            {
                Multiselect = true,
                Filter = "CSV 파일 (*.csv)|*.csv|모든 파일 (*.*)|*.*",
                InitialDirectory = GetPath
            };

            if (openFileDialog.ShowDialog() == true)
            {
                var selectedFiles = openFileDialog.FileNames.ToList();

                // 📌 최소 1개, 최대 12개 제한
                if (selectedFiles.Count != 12)
                {
                    MessageBox.Show($"12개의 파일을 선택하세요. 현재 {selectedFiles.Count}", "파일 선택 오류", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                // 📌 파일명을 분석하여 Box 번호 기준으로 정렬
                selectedFiles = selectedFiles
                    .Where(f => Regex.IsMatch(Path.GetFileName(f), @"^Box(\d+)@")) // 파일명 형식 검사
                    .OrderBy(f => int.Parse(Regex.Match(Path.GetFileName(f), @"^Box(\d+)@").Groups[1].Value)) // Box 번호 기준 정렬
                    .ToList();

                if (selectedFiles.Count == 0)
                {
                    HandyControl.Controls.MessageBox.Show("올바른 Box 파일을 선택하세요 (Box1~Box18).", "파일 형식 오류", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                List<List<string[]>> allFilesData = new List<List<string[]>>();
                int maxRows = 0; // 가장 긴 파일의 행 개수

                foreach (string file in selectedFiles)
                {
                    var lines = File.ReadAllLines(file, Encoding.UTF8)
                                    .Select(line => line.Split(','))
                                    .ToList();

                    allFilesData.Add(lines);
                    maxRows = Math.Max(maxRows, lines.Count); // 최대 행 개수 갱신
                }

                if (maxRows == 0)
                {
                    HandyControl.Controls.MessageBox.Show("선택한 파일이 비어 있습니다.", "오류", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }

                // 📌 가장 긴 파일의 날짜/시간을 기준으로 병합
                List<string> mergedData = new List<string>();

                for (int i = 0; i < maxRows; i++)
                {
                    List<string> rowData = new List<string>();

                    // 가장 긴 파일의 날짜/시간 가져오기
                    string dateTime = allFilesData.FirstOrDefault(file => file.Count > i && file[i].Length > 0)?[i][0] ?? "";
                    rowData.Add(dateTime);

                    // 📌 데이터1, 데이터2, 데이터3 순으로 추가 (없으면 0으로 채움)
                    foreach (var fileData in allFilesData)
                        rowData.Add(fileData.Count > i && fileData[i].Length > 1 ? fileData[i][1] : "0");

                    foreach (var fileData in allFilesData)
                        rowData.Add(fileData.Count > i && fileData[i].Length > 2 ? fileData[i][2] : "0");

                    foreach (var fileData in allFilesData)
                        rowData.Add(fileData.Count > i && fileData[i].Length > 3 ? fileData[i][3] : "0");

                    mergedData.Add(string.Join(",", rowData));
                }

                // 📌 자동 저장할 파일명 (AllTempMerge_년_월_일_시간분초.csv)
                string timestamp = DateTime.Now.ToString("yyyy_MM_dd_HHmmss");
                string fileName = $"TempMerge_{timestamp}.csv";

                // 📌 실행 경로에 자동 저장
                string savePath = Path.Combine(Global.gMainVM.DataPath, fileName);
                File.WriteAllLines(savePath, mergedData, Encoding.UTF8);

                HandyControl.Controls.MessageBox.Show($"CSV 파일 병합 완료!\n저장 위치: {savePath}", "완료", MessageBoxButton.OK, MessageBoxImage.Information);
            }
        }
    }
}
