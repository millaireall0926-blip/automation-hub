using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Documents;
using HandyControl.Controls;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;
using StarBase;
using StarResult.Model;

namespace StarResult.ViewModel
{


    public class GraphVM : BaseVM
    {

        enum CHAMBER_PRINT
        {
            ALL,
            STEP
        }

        enum POWER_CH : int
        {
            PS1_V = 0,
            PS1_C = 1,

            PS2_V,
            PS2_C,

            PS3_V,
            PS3_C,

            PS4_V,
            PS4_C,

            PS5_V,
            PS5_C,

            IO_V,
            CLK_V,
            DRV_V,
            SCAN_V,

            COUNT
        }


        bool _isTmepAllStep = true;
        public bool IsTempAllStep
        {
            get { return _isTmepAllStep;}
            set { _isTmepAllStep = value; OnPropertyChanged(nameof(IsTempAllStep)); } 
        }

        const int MaxCnt = 86400 *7; // 1 day 86400
        const int MaxTempCnt = 86400 * 7; // 1 weeks data

        public WpfPlot xTempGraph;
        public WpfPlot xPowerGraph;

        public double[][] _pwrDatas;// = new double[864_00]; // 1day Data
        SignalPlot[] _pwrSigs;

        public double[] _tempData;
        SignalPlot _tempSig;

        public int _pDataIndex = 0;
        public int _tDataIndex = 0;

        string _fileName = "";


        

        public GraphVM()
        {
            initData();

        }

        public void initTemp()
        {
            xTempGraph.Plot.Title("Chamber");
            xTempGraph.Plot.YLabel("℃");


            xTempGraph.Plot.Style(ScottPlot.Style.Hazel);
            xTempGraph.Plot.Palette = Palette.Amber;

            _tempSig = xTempGraph.Plot.AddSignal(_tempData);
            _tempSig.FillBelow(System.Drawing.Color.RosyBrown, System.Drawing.Color.IndianRed);
            _tempSig.LineWidth = 5;
            _tempSig.MarkerSize = 0;

               xTempGraph.Refresh();

        }

        public void initPower()
        {
            xPowerGraph.Plot.Title("Power");
            xPowerGraph.Plot.YLabel("Level/Current");


            xPowerGraph.Plot.Style(ScottPlot.Style.Gray2);
            xPowerGraph.Plot.Palette = Palette.DarkPastel;

            int i = 0;
            foreach (POWER_CH ch in Enum.GetValues(typeof(POWER_CH)))
            {
                if (ch == POWER_CH.COUNT) break;
                _pwrSigs[i] = xPowerGraph.Plot.AddSignal(_pwrDatas[i], label: ch.ToString());
                _pwrSigs[i].LineWidth = 0;
                _pwrSigs[i].MarkerSize = 0;
                i++;
            }

            for (i = (int)POWER_CH.PS1_V; i <= (int)POWER_CH.PS4_C; i++)
            {
                _pwrSigs[i].LineWidth = 2; _pwrSigs[i].MarkerSize = 4;
            }


            Legend ld = xPowerGraph.Plot.Legend();
            ld.FillColor = System.Drawing.Color.DarkBlue;
            ld.FontColor = System.Drawing.Color.LightGray;
            ld.Location = Alignment.LowerRight;
            ld.IsVisible = false;

            xPowerGraph.Refresh();
        }

        public void initData()
        {
            _pwrDatas = new double[(int)POWER_CH.COUNT][];
            for (int i = 0; i < (int)POWER_CH.COUNT; i++)
            {
                _pwrDatas[i] = new double[MaxCnt];
            }

            _tempData = new double[MaxTempCnt];

            _pwrSigs = new SignalPlot[(int)POWER_CH.COUNT];
        }

        public void loadFile(string name)
        {
            _fileName = name;
            if (IsTempAllStep)
                selectTempAll();

        }

        public void selectTempAll()
        {
            Collection<string> lines = new Collection<string>();
            if (IsTempAllStep == false) return;

            _tDataIndex = 0;

            try
            {
                FileStream logFileStream = new FileStream(_fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
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
                string str = line.Trim();
                string[] word = str.Split(new char[] { '\t' });
                if (word.Length < 37) continue;

                _tempData[_tDataIndex] = Convert.ToDouble(word[36]);
                _tDataIndex++;
                if (_tDataIndex >= MaxTempCnt) _tDataIndex = 0;
            }

            if (_tDataIndex != 0) _tDataIndex--;
            _tempSig.MaxRenderIndex = _tDataIndex;
            xTempGraph.Plot.AxisAuto();
            xTempGraph.Refresh();
        }




        public void setCheckBox(string name, bool bCheck)
        {
            if (_pwrSigs == null) return;            
            if (name == "xCB_show") xPowerGraph.Plot.Legend().IsVisible = bCheck;

            switch (name)
            {
                case "xCB_vps1":            
                    _pwrSigs[(int)POWER_CH.PS1_V].IsVisible = bCheck;
                    break;
                case "xCB_vps2":
                    _pwrSigs[(int)POWER_CH.PS2_V].IsVisible = bCheck;                    
                    break;
                case "xCB_vps3":            
                    _pwrSigs[(int)POWER_CH.PS3_V].IsVisible = bCheck;
                    break;
                case "xCB_vps4":            
                    _pwrSigs[(int)POWER_CH.PS4_V].IsVisible = bCheck;
                    break;
                case "xCB_vps5":            
                    _pwrSigs[(int)POWER_CH.PS5_V].IsVisible = bCheck;
                    break;
                case "xCB_io":                    
                    _pwrSigs[(int)POWER_CH.IO_V].IsVisible = bCheck;
                    break;
                case "xCB_clk":
                    _pwrSigs[(int)POWER_CH.CLK_V].IsVisible = bCheck;
                    break;
                case "xCB_drv":                    
                    _pwrSigs[(int)POWER_CH.DRV_V].IsVisible = bCheck;
                    break;
                case "xCB_scan":                    
                    _pwrSigs[(int)POWER_CH.SCAN_V].IsVisible = bCheck;
                    break;
                case "xCB_cps1":
                    _pwrSigs[(int)POWER_CH.PS1_C].IsVisible = bCheck;
                    break;
                case "xCB_cps2":
                    _pwrSigs[(int)POWER_CH.PS2_C].IsVisible = bCheck;
                    break;
                case "xCB_cps3":
                    _pwrSigs[(int)POWER_CH.PS3_C].IsVisible = bCheck;
                    break;
                case "xCB_cps4":
                    _pwrSigs[(int)POWER_CH.PS4_C].IsVisible = bCheck;
                    break;
                case "xCB_cps5":
                    _pwrSigs[(int)POWER_CH.PS5_C].IsVisible = bCheck;
                break;
            }
            xPowerGraph.Refresh();
        }

        

        public void setTempMode() // All, Step
        {
            if (_fileName.Length <= 0) return;

            if (IsTempAllStep == true)
                selectTempAll();
            else
                setStep(Global.gMainVM._curStep);
        }

        public void setStep(StepModel step)
        {
            _pDataIndex = 0;
            if (IsTempAllStep == false) _tDataIndex = 0;

            //string[] lines;
            Collection<string> lines = new Collection<string>();
            try
            {
                FileStream logFileStream = new FileStream(_fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
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
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }
            bool bFind = false;
            string name = step.Name;
            foreach (var line in lines)
            {
                string stFind = "start step \'" + name;
                string endFind = "end step \'" + name;

                if (!bFind)
                {
                    if (line.Contains(stFind))
                    {
                        bFind = true;
                        continue;
                    }
                }
                else
                {
                    if (line.Contains(endFind)) break;
                    string[] word = line.Split(new char[] { '\t' });
                    if (word.Length < 37) continue;
                    int wordPos;
                    int graphPos;
                    for (int i = 0; i<5; i++) // level
                    {
                        wordPos = (i * 4) + 4;
                        graphPos = i * 2; 
                        _pwrDatas[graphPos][_pDataIndex] = Convert.ToDouble(word[wordPos]);                        
                    }

                    for (int i = 0; i < 5; i++) // current
                    {
                        wordPos = (i * 4) + 6;
                        graphPos = (i * 2) +1;
                        _pwrDatas[graphPos][_pDataIndex] = Convert.ToDouble(word[wordPos]);
                    }

                    /*  i   val chan  type
                        23  1.8 io    VIH
                        24  1.8 clk   VIH
                        25  1.8 drv   VIH
                        26  1.8 scan  VIH
                        27  0   io    VIL
                        28  0   clk   VIL
                        29  0   drv   VIL
                        30  0   scan  VIL
                        31  0.9 io    VTH
                        32  0.9 clk   VTH
                        33  0.9 drv   VTH
                        34  0.9 scan  VTH
                    */

                    for (int i = 0; i<4; i++)
                    {
                        wordPos = i + 23;  
                        graphPos = 10 + i;
                        _pwrDatas[graphPos][_pDataIndex] = Convert.ToDouble(word[wordPos]); wordPos++;
                    }

                    if (IsTempAllStep == false)
                    {
                        _tempData[_tDataIndex] = Convert.ToDouble(word[35]);
                        _tDataIndex++;
                        if (_tDataIndex >= MaxCnt) _tDataIndex = 0;
                    }

                    _pDataIndex++;
                    if (_pDataIndex >= MaxCnt) 
                        _pDataIndex = 0;
                }
            }

            if (_pDataIndex != 0) _pDataIndex--;

            for (int i = 0; i < (int)POWER_CH.COUNT; i++)
                _pwrSigs[i].MaxRenderIndex = _pDataIndex;
            xPowerGraph.Plot.AxisAuto();
            xPowerGraph.Refresh();

            if (IsTempAllStep == false)
            {
                if (_tDataIndex != 0) _tDataIndex--;
                _tempSig.MaxRenderIndex = _tDataIndex;
                xTempGraph.Plot.AxisAuto();
                xTempGraph.Refresh();
            }
        }


    }


}