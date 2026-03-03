using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
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
    /// BoardPage.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class BoardPage : Page
    {
        public BoardPage()
        {
            InitializeComponent();
            Rect initRect = Global.gRackMagins[0];
            if (Global.gMainPageVM.LayoutColumnCount == 1)
            {
                initRect = Global.gRackMaginsCol1[0];
            }

            xRect_Select.Margin = new Thickness(initRect.X,
                                                initRect.Y,
                                                initRect.Width,
                                                initRect.Height);
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
                    if (Global.gBoardVM.CurRack != index)
                    {
                        Global.gBoardVM.CurRack = index;
                        Global.gBoardVM.setRack(isRead: false);
                    }
                    break;
                }
                index++;
            }

        }
    }
}
