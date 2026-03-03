using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using HandyControl.Controls;
using StarBase;
using StarResult.Model;

namespace StarResult.ViewModel
{
    public class MainVM : BaseVM
    {

        public ObservableCollection<StepModel> _stepModels = new ObservableCollection<StepModel>();
        public ObservableCollection<StepModel> StepModels
        {
            get { return _stepModels; }
            set { _stepModels = value; OnPropertyChanged(nameof(StepModels)); }
        }

        public string _dataPath = Global.gResultPath;
        public string DataPath
        {
            get { return _dataPath; }
            set { 
                _dataPath = value; 
                Global.gResultPath = _dataPath; 
                OnPropertyChanged(nameof(DataPath)); 
            }
        }

        int _TextMode = 1;
        public int TextMode
        {
            // 0 : NO TEXT                      
            // 1: Num
            // 2: IO        
            // 3: FAILBIT          
            // 4: CHIPID            
            // 5: BIN
            get { return _TextMode; }
            set
            {
                _TextMode = value;
                OnPropertyChanged(nameof(TextMode));
                if (Global.gBIBVM != null) {
                    Global.gBIBVM.setDutText(TextMode);
                    if (_curStep != null) {                        
                        Global.gBIBVM.setStep(_curStep);
                        if (Global.gBIBVM.MultiChipWin.IsEnabled)
                            Global.gBIBVM.MultiChipWin.Hide();
                    }
                }
            }
        }

        string _totalText = "STEP";
        public string TotalText
        {
            get => _totalText;
            set
            {
                _totalText = value;
                OnPropertyChanged("TotalText");
            }
        }

        bool _total = false;
        public bool Total
        {
            get => _total;
            set
            {
                _total = value;
                if (value == true) TotalText = "TOTAL";
                else TotalText = "STEP";
                if (Global.gBIBVM != null) { 
                    if (_curStep != null)
                    {
                        Global.gBIBVM.setStep(_curStep);
                        if (Global.gBIBVM.MultiChipWin.IsEnabled)
                            Global.gBIBVM.MultiChipWin.Hide();
                    }
                }
                OnPropertyChanged("Total");
            }
        }

        ObservableCollection<SlotLog> _slotLogs = new ObservableCollection<SlotLog>();
        public ObservableCollection<SlotLog> SlotLogs
        {
            get { return _slotLogs; }
            set { _slotLogs = value; OnPropertyChanged("SlotLogs"); }
        }

        #region Layout 관련
        bool _layoutMode = true; // save, load
        public bool LayoutMode
        {
            get => _layoutMode;
            set
            {
                _layoutMode = value;
                OnPropertyChanged("LayoutMode");
            }
        }

        int _layoutNum = 0; // save, load
        public int LayoutNum
        {
            get => _layoutNum;
            set
            {
                _layoutNum = value;
                OnPropertyChanged("LayoutNum");
            }
        }



        #endregion
        public Visibility ShowLogo { get; set; } = Visibility.Visible;


        public MainVM()
        {
            Global.gMainVM = this;
            //      DateTime fromdate = new DateTime(1970, 1, 1, 9, 0, 0);
            //      TimeSpan ts = DateTime.Now.Subtract(fromdate);
            //      int value = (int)ts.TotalSeconds;
            //      Console.WriteLine($"{value.ToString("X")}");
            if (Global.gShowLogo) ShowLogo = Visibility.Visible;
            else ShowLogo = Visibility.Hidden;
        }

        public string convertTimeText(string text)
        {
            if (text.Length != 15) return text;

            string time;
            string tmp = text;
            time = tmp.Substring(0, 4);
            time += ("/" + tmp.Substring(4, 2));
            time += ("/" + tmp.Substring(6, 2));
            time += (" " + tmp.Substring(9, 2));
            time += (":" + tmp.Substring(11, 2));
            time += (":" + tmp.Substring(13, 2));

            return time;
        }
        public string getBidText(int bid)
        {
            string sbid = "";
            int nSerial = bid & 0xffff;
            int nBall = (bid >> 16) & 0xf;
            int nDevFam = (bid >> 20) & 0x1f;
            int nFunc = (bid >> 25) & 0x7;

            if (Global.BidFuncMap.ContainsKey(nFunc))
                sbid += Global.BidFuncMap[nFunc];
            else
                sbid += "---"; sbid += " ";
            if (Global.BidDevMap.ContainsKey(nDevFam))
                sbid += Global.BidDevMap[nDevFam];
            else
                sbid += "---"; sbid += " ";
            if (Global.BidBallMap.ContainsKey(nBall))
                sbid += Global.BidBallMap[nBall];
            else
                sbid += "---"; sbid += " ";
            sbid += string.Format(": {0}", Convert.ToString(nSerial, 16));

            return sbid;
        }


        public void loadStep(XmlDocument doc)
        {
            StepModels.Clear();

            StepModel lastStep = new StepModel();
            XmlNode node = doc.SelectSingleNode("/test-result");
            XmlNodeList list = node.SelectNodes("step");

            int passCnt = 0;
            int totalCnt = 0;
            #region  Step Loading
            foreach (XmlNode line in list)
            {
                StepModel step = new StepModel()
                {
                    Name = line.Attributes["id"].Value,
                    Start = line.Attributes["start"].Value,
                    End = ""
                };

                if (line.Attributes["end"] != null)
                    step.End = line.Attributes["end"].Value;
                if (Convert.ToInt32(line.Attributes["con"].Value) == 1)
                    step.IsContact = true;

                step.Start = convertTimeText(step.Start);
                step.End = convertTimeText(step.End);

                StepModels.Add(step);
                passCnt = 0;
                int testCnt = Global.gBIBVM._layout._dutCnt;  // Mount

                XmlNode pfNode = line.SelectSingleNode("con");
                if (pfNode != null)
                {
                    int idx = 0;
                    string[] buf = pfNode.InnerText.Split(',');
                    foreach (string str in buf)
                    {
                        if (str == "") break;
                        int pf = Convert.ToInt32(str, 16);
                        for (int i = 0; i < 32; i++)
                        {
                            bool isPass = (pf & (1 << i)) == 0 ? false : true;
                            if (isPass) passCnt++;
                            Global.gBIBVM.DutModels[idx].ContactDic[step.Name] = isPass;
                            idx++;
                            if (idx >= Global.gBIBVM.DutModels.Count) break;
                        }
                        if (idx >= Global.gBIBVM.DutModels.Count) break;
                    }
                    Global.gInfoVM.MountCnt = passCnt.ToString();
                }
                if (!step.IsContact)
                    testCnt = passCnt;

                passCnt = 0;
                pfNode = line.SelectSingleNode("cur");
                if (pfNode != null)
                {
                    int idx = 0;
                    string[] buf = pfNode.InnerText.Split(',');
                    foreach (string str in buf)
                    {
                        if (str == "") break;
                        int pf = Convert.ToInt32(str, 16);
                        for (int i = 0; i < 32; i++)
                        {
                            bool isPass = (pf & (1 << i)) == 0 ? false : true;
                            if (isPass) passCnt++;
                            Global.gBIBVM.DutModels[idx].CurrentDic[step.Name] = isPass;
                            idx++;
                            if (idx >= Global.gBIBVM.DutModels.Count) break;
                        }
                        if (idx >= Global.gBIBVM.DutModels.Count) break;
                    }
                }

                totalCnt = 0;
                pfNode = line.SelectSingleNode("total");
                if (pfNode != null)
                {
                    int idx = 0;
                    string[] buf = pfNode.InnerText.Split(',');
                    foreach (string str in buf)
                    {
                        if (str == "") break;
                        int pf = Convert.ToInt32(str, 16);
                        for (int i = 0; i < 32; i++)
                        {
                            bool isPass = (pf & (1 << i)) == 0 ? false : true;
                            if (isPass) totalCnt++;
                            Global.gBIBVM.DutModels[idx].TotalDic[step.Name] = isPass;
                            idx++;
                            if (idx >= Global.gBIBVM.DutModels.Count) break;
                        }
                        if (idx >= Global.gBIBVM.DutModels.Count) break;
                    }
                }
                step.Yeild = String.Format($"{passCnt}/{testCnt} T:{totalCnt}");


                lastStep = step;

                var optionNode = line.SelectSingleNode("fbit");
                if (optionNode != null)
                {
                    var dlist = optionNode.SelectNodes("d");
                    foreach (XmlNode d in dlist)
                    {
                        int no = Convert.ToInt32(d.Attributes["no"].Value);
                        int fb = Convert.ToInt32(d.Attributes["fb"].Value, 16);
                        //Global.gBIBVM.DutModels[idx].FBit = d.Attributes["fb"].Value;

                        Global.gBIBVM.DutModels[no]._fBit[step.Name] = fb;
                    }
                }

                optionNode = line.SelectSingleNode("chipid");
                if (optionNode != null)
                {
                    var dlist = optionNode.SelectNodes("d");
                    foreach (XmlNode d in dlist)
                    {
                        int idx = Convert.ToInt32(d.Attributes["no"].Value);
                        Global.gBIBVM.DutModels[idx].ChipID = d.InnerText;
                    }
                }

                optionNode = line.SelectSingleNode("sub");
                if (optionNode != null)
                {
                    string type = optionNode.Attributes["type"].Value;
                    string lot = optionNode.Attributes["lot"].Value;
                    string id = optionNode.Attributes["id"].Value;

                    step._subType = StepModel.findSubType(type);
                    
                    if (step._subType == SUB_TYPE.DATA || step._subType == SUB_TYPE.MEAS_DATA)
                        step._subFile = Global.gResultPath + "\\data\\" + Global.gInfoVM.LotID +"_" + id + "_" + _selSlotName;                    
                    else if (step._subType == SUB_TYPE.CYCLE)
                        step._subFile = Global.gResultPath + "\\sub\\" + Global.gInfoVM.LotID + "_" + id + "_" + _selSlotName + ".csv";
                    else 
                        step._subFile = Global.gResultPath + "\\sub\\" + Global.gInfoVM.LotID + "_" + id + "_" + _selSlotName + ".xml";

                }
            }

            Global.gInfoVM.End = lastStep.End;

            Global.gInfoVM.TotalCnt = Global.gBIBVM._layout._dutCnt.ToString();
            Global.gInfoVM.PassCnt = totalCnt.ToString();
            Global.gInfoVM.FailCnt = (Convert.ToInt32(Global.gInfoVM.MountCnt) - totalCnt).ToString();
            
            selectStep(lastStep);

            #endregion
        }


        public string _selSlotName = "";

        public void selectFile(string lot)
        {
            string resultFile =  Global.gResultPath + "\\result\\" + lot + ".xml";

            int pos = lot.IndexOf("_Slot");
            if (pos > -1) _selSlotName = lot.Substring(pos+ 1);
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load(resultFile);
            }
            catch (Exception ex) {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return; 
            }

            string powerFile = Global.gResultPath + "\\power\\" + lot + ".power";
            Global.gGraphVM.loadFile(powerFile);

            string logFile = Global.gResultPath + "\\slot\\" + lot + ".log";
            loadLog(logFile);

            XmlNode node = doc.SelectSingleNode("/test-result/info");
            if (node != null)
            {
                string sbid = node.Attributes["bid"].Value;
                if (sbid != "")
                    Global.gInfoVM.BID = getBidText(Convert.ToInt32(sbid, 16));
                else
                    Global.gInfoVM.BID = sbid;

                Global.gInfoVM.Start = convertTimeText(node.Attributes["date"].Value);
                Global.gInfoVM.Pgm = node.Attributes["pgm"].Value;
                Global.gInfoVM.LotID = node.Attributes["lot"].Value;
            }

            Global.gBIBVM.loadBIB(doc);                        
            
            loadStep(doc);
            Global.gInfoVM.updateYeild();

        }

        public StepModel _curStep;

        public void selectStep(StepModel step)
        {
            _curStep = step;
            Global.gBIBVM.setStep(step);
            Global.gGraphVM.setStep(step);

        }

        public void loadLog(string fileName)
        {
            SlotLogs.Clear();

            Collection<string> lines = new Collection<string>();
            try
            {
                FileStream logFileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                StreamReader logFileReader = new StreamReader(logFileStream);
                while (!logFileReader.EndOfStream)
                {
                    lines.Add(logFileReader.ReadLine());
                }
                logFileReader.Close();
                logFileStream.Close();

                //                lines = File.ReadAllLines(_fileName, Encoding.UTF8);
            }
            catch (Exception ex)
            {
                //MessageBox.Show(ex.Message);
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);

                return;
            }

            foreach (string line in lines)
            {
                char [] strOrg = line.ToArray();
                int idx = 0;
                string time = "";
                string step = "";
                string msg ="";
                foreach(char c in strOrg)
                {
                    if (idx >= 2)
                        msg += c;                 
                    else
                    {
                        if (c == '[')
                            continue;
                        else if (c == ']')
                        {
                            idx++;
                            continue;                            
                        }
                        if (idx == 0) time += c;
                        if (idx == 1) step += c;
                    }                   

                }
                SlotLogs.Add(new SlotLog(time, step, msg));
            }

        }
    }
}
