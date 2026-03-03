using HTCRack.Model;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Drawing.Drawing2D;
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

namespace HTCRack.View.Component
{
    /// <summary>
    /// RegFormUC.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class RegFormUC : UserControl
    {
        public RegFormUC()
        {
            InitializeComponent();
        }

        public IEnumerable ItemsSource
        {
            get { return (IEnumerable)GetValue(ItemsSourceProperty); }
            set { SetValue(ItemsSourceProperty, value); }
        }

        public static readonly DependencyProperty ItemsSourceProperty =
            DependencyProperty.Register("ItemsSource", typeof(IEnumerable), typeof(RegFormUC), new PropertyMetadata(null));


        [Category("RegType"), Description("RegType")]
        public string RegType
        {
            get { return xType.Text; }
            set { xType.Text = value; }
        }

        [Category("ValueType"), Description("ValueType")]
        public string ValueType
        {
            get { return xValueHead.Text; }
            set { xValueHead.Text = value; }
        }


    }
}
