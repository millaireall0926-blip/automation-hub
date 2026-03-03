using HTCRack_CXL.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HTCRack_CXL.View
{
    /// <summary>
    /// TempPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class TempPage : Page
    {
        Color[] GraphColors = new Color[] {
                Colors.Coral,
                Colors.DarkKhaki,
                Colors.Violet,
                Colors.DarkSlateBlue,
        };
        Color[] GraphColors1 = new Color[] {
                Colors.MediumPurple,
                Colors.DodgerBlue,
                Colors.LimeGreen,
                Colors.Orchid
        };
        public TempPage()
        {
            InitializeComponent();
            int TotalCh = 4;

            for (int i = 0; i < TotalCh; i++)
            {
                Global.gTempVM.TempDatas.Add(new TempDataModel());
                Global.gTempVM.TempDatas[i].Name = $"CH{i + 1}";
                Global.gTempVM.TempDatas[i].RackName = $"FIX{i + 1}";
                Global.gTempVM.TempDatas[i].ChannelColor = new SolidColorBrush(GraphColors[i]);
                Global.gTempVM.TempDatas[i].RackColor = new SolidColorBrush(GraphColors1[i]);
                //Console.WriteLine($"Rack{i} =  {Global.gGraphVM.TempDatas[i].ChannelColor}");
                Global.gTempVM.TempDatas[i].boxindex = i + 1;
            }

            Global.gTempVM.TempGraph = xDimGraph;
            Global.gTempVM.InitGraph();

            Rect initRect = Global.gRackMagins[0];
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                initRect = Global.gRackMaginsCol1[0];
            }

            xRect_Select.Margin = new Thickness(initRect.X,
                                                initRect.Y,
                                                initRect.Width,
                                                initRect.Height);

            Global.gTempVM.SetBox();
        }

        private void ToggleDimmAll_Click(object sender, RoutedEventArgs e)
        {
            ToggleButton tb = sender as ToggleButton;

            for (int i = 0; i < 4; i++)
                Global.gTempVM.TempDatas[i].Show = (bool)tb.IsChecked;
        }

        private void Image_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point pos = e.GetPosition(sender as Image);
#if TEST_MODE
            Console.WriteLine($" x {pos.X} , y {pos.Y}");
#endif

            int index = 0;

            Rect[] rects = Global.gRackRects;
            Rect[] margins = Global.gRackMagins;
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                rects = Global.gRackRectsCol1;
                margins = Global.gRackMaginsCol1;
            }

            foreach (Rect rect in rects)
            {
                if (rect.Contains(pos))
                {
                    if (index >= Global.gMainPageVM.Racks.Count) return;
                    xRect_Select.Margin = new Thickness(margins[index].X,
                                                        margins[index].Y,
                                                        margins[index].Width,
                                                        margins[index].Height);
                    Global.gTempVM.CurRack = index;
                    Global.gTempVM.SetBox();

                    break;
                }
                index++;
            }
        }
    }
}
