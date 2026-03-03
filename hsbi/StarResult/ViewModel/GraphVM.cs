using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data.SqlTypes;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Media;
using HandyControl.Controls;
using ScottPlot;
using ScottPlot.Plottable;
using ScottPlot.Renderable;
using StarBase;
using StarResult.Model;
using StarResult.View;

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

        Color[] GraphColors = new Color[] {
            Colors.Coral,
            Colors.Tomato,
            Colors.OrangeRed,
            Colors.DarkOrange,
            Colors.Gold,
            Colors.DarkKhaki,
            Colors.Thistle,
            Colors.Plum,
            Colors.Violet,
            Colors.Orchid,


            Colors.DeepSkyBlue,
            Colors.DodgerBlue,
            Colors.RoyalBlue,
            Colors.Blue,
            Colors.GreenYellow,
            Colors.LawnGreen,
            Colors.Lime,
            Colors.LimeGreen,
            Colors.PaleGreen,
            Colors.SpringGreen,

            };

        public static int DPS_SIZE = 20;
        public static int HALF_SIZE = DPS_SIZE / 2;
        public SolidColorBrush[] Brushs { get; set; }

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
        public DPSModel[] DPS1s { get; set; }
        public DPSModel[] DPS2s { get; set; }

        SignalPlot[] _pwrSigs;

        public double[] _tempData;
        SignalPlot _tempSig;

        public int _pDataIndex = 0;
        public int _tDataIndex = 0;

        string _fileName = "";

        const int TEMP_POS = 42;


        

        public GraphVM()
        {
            DPS1s = new DPSModel[HALF_SIZE];
            DPS2s = new DPSModel[HALF_SIZE];

            Brushs = new SolidColorBrush[DPS_SIZE * 2];
            for (int i = 0; i < DPS_SIZE; i++)
            {
                Color col = GraphColors[i];
                if (col.R < 250)
                    col.R += 5;
                if (col.G < 250)
                    col.G += 5;
                if (col.B < 250)
                    col.B += 5;

                Brushs[i * 2] = new SolidColorBrush(GraphColors[i]);
                Brushs[i * 2 + 1] = new SolidColorBrush(col);
            }

            

        }

        public void initTemp()
        {
            _tempData = new double[MaxTempCnt];

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

        public DPSModel makeModel(int index)
        {
            var model = new DPSModel();

            model.Name = index.ToString();
            model.ColorV = Brushs[index * 2];
            model.ColorC = Brushs[index * 2 + 1];

            return model;
        }

        public DPSModel getModel(int index)
        {
            if (index >= DPS_SIZE) return null;

            if (index >= HALF_SIZE)
                return DPS2s[index % HALF_SIZE];

            return DPS1s[index % HALF_SIZE];
        }

        public DPSModel[] getModels(int index, out int modelIndex)
        {
            DPSModel[] models = DPS1s;
            if (index >= HALF_SIZE) models = DPS2s;
            modelIndex = index % HALF_SIZE;
            return models;
        }



        public void initPower()
        {
            xPowerGraph.Plot.Title("Power");
            xPowerGraph.Plot.YLabel("Level/Current");

            for (int i = 0; i < DPS_SIZE; i++)
            {
                var models = getModels(i, out int index);
                models[index] = makeModel(i);

            }
            DPS1s[0].Show = true;
            DPS2s[0].Show = true;


            xPowerGraph.Plot.Style(ScottPlot.Style.Gray2);
            xPowerGraph.Plot.Palette = Palette.DarkPastel;

            _pwrSigs = new SignalPlot[DPS_SIZE * 2];
            for (int i = 0; i < DPS_SIZE; i++)
            {
                DPSModel[] models = getModels(i, out int index);


                _pwrSigs[i * 2] = xPowerGraph.Plot.AddSignal(models[index]._graphDataV, label: String.Format($"DPS{i}V"));
                _pwrSigs[i * 2].LineWidth = 3;
                _pwrSigs[i * 2].MarkerSize = 0;
                _pwrSigs[i * 2].MaxRenderIndex = 0;

                Color col = Brushs[i * 2].Color;
                _pwrSigs[i * 2].Color = System.Drawing.Color.FromArgb(
                    col.A, col.R, col.G, col.B);

                _pwrSigs[(i * 2) + 1] = xPowerGraph.Plot.AddSignal(models[index]._graphDataC, label: String.Format($"DPS{i}C"));
                _pwrSigs[(i * 2) + 1].LineWidth = 2;
                _pwrSigs[(i * 2) + 1].MarkerSize = 3;
                _pwrSigs[(i * 2) + 1].MaxRenderIndex = 0;
                col = Brushs[i * 2 + 1].Color;
                _pwrSigs[(i * 2) + 1].Color = System.Drawing.Color.FromArgb(
                    col.A, col.R, col.G, col.B);
            }


            Legend ld = xPowerGraph.Plot.Legend();
            ld.FillColor = System.Drawing.Color.DarkBlue;
            ld.FontColor = System.Drawing.Color.LightGray;
            ld.Location = Alignment.LowerRight;
            ld.IsVisible = false;

            xPowerGraph.Refresh();
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

                _tempData[_tDataIndex] = Convert.ToDouble(word[TEMP_POS]);
                _tDataIndex++;
                if (_tDataIndex >= MaxTempCnt) _tDataIndex = 0;
            }

            if (_tDataIndex != 0) _tDataIndex--;
            _tempSig.MaxRenderIndex = _tDataIndex;
            xTempGraph.Plot.AxisAuto();
            xTempGraph.Refresh();
        }




        public void setCheckBox()
        {
            for (int i = 0; i < DPS_SIZE; i++)
            {
                var models = getModels(i, out int index);
                _pwrSigs[i * 2].IsVisible = models[index].Show;
                _pwrSigs[(i * 2) + 1].IsVisible = models[index].Show;
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
                    if (word.Length < 40) continue;
                    if (word.Length > 50) // Setting Value
                    {
                        continue;
                    }
                    int wordPos;
                    int graphPos;

                    for (int i = 2; i<TEMP_POS; i+=2)
                    {
                        int dmIndex = (i / 2) - 1;
                        DPSModel dm = getModel(dmIndex);

                        dm._graphDataV[_pDataIndex] = Convert.ToDouble(word[i]);
                        dm._graphDataC[_pDataIndex] = Convert.ToDouble(word[i+1]);
                    }

//                    for (int i = 0; i<5; i++) // level
//                    {
//                        wordPos = (i * 4) + 4;
//                        graphPos = i * 2; 
//
//
//                        _pwrDatas[graphPos][_pDataIndex] = Convert.ToDouble(word[wordPos]);                        
//                    }
//
//                    for (int i = 0; i < 5; i++) // current
//                    {
//                        wordPos = (i * 4) + 6;
//                        graphPos = (i * 2) +1;
//                        _pwrDatas[graphPos][_pDataIndex] = Convert.ToDouble(word[wordPos]);
//                    }

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

                    

                    if (IsTempAllStep == false)
                    {
                        _tempData[_tDataIndex] = Convert.ToDouble(word[TEMP_POS]);
                        _tDataIndex++;
                        if (_tDataIndex >= MaxCnt) _tDataIndex = 0;
                    }

                    _pDataIndex++;
                    if (_pDataIndex >= MaxCnt) 
                        _pDataIndex = 0;
                }
            }

            if (_pDataIndex != 0) _pDataIndex--;

            for (int i = 0; i < DPS_SIZE*2; i++)
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