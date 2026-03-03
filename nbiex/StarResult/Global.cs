using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Xml;
using HandyControl.Controls;
using StarResult.View;
using StarResult.ViewModel;

namespace StarResult
{

    public static class Global
    {
        public static ResultSectionVM gResSelVM;
        public static GraphVM gGraphVM;
        public static MainVM gMainVM;
        public static InfoVM gInfoVM;
        public static BIBVM gBIBVM;

        public static MainWindow gMainView;
        public static BIBView gBIBView;


        public static string gResultPath = @"d:\data";

        public static Dictionary<int, string> BidFuncMap = new Dictionary<int, string>();
        public static Dictionary<int, string> BidDevMap = new Dictionary<int, string>();
        public static Dictionary<int, string> BidBallMap = new Dictionary<int, string>();

        public static Dictionary<int, SolidColorBrush> BinColorDic = new Dictionary<int, SolidColorBrush>();

        static Global()
        {
            gGraphVM = new GraphVM();  // gGraphVM 은 여기서 생성
            loadBibMap();
            loadBinColor();
            loadConfig();
        }

        public static string gConfigPath = "StarResult.xml";

        public static void loadConfig()
        {
            XmlDocument doc = new XmlDocument();
            string path = gConfigPath;
            if (!File.Exists(path))
            {
                path = @"StarResult\" + path;
                if (File.Exists(path))
                    gConfigPath = path;
                else
                    return;
            }

            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }
            XmlNode node = doc.SelectSingleNode("/starResult/data");
            gResultPath =  node.Attributes["path"].Value;
        }

        public static void saveConfig()
        {
            XmlDocument doc = new XmlDocument();
            string path = gConfigPath;
            if (!File.Exists(path))
            {
                path = @"StarResult\" + path;
                if (File.Exists(path))
                    gConfigPath = path;
                else
                    return;
            }

            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }
            XmlNode node = doc.SelectSingleNode("/starResult/data");
            node.Attributes["path"].Value = gResultPath;
            doc.Save(gConfigPath);
        }

        public static void loadLayout(int num)
        {
            XmlDocument doc = new XmlDocument();
            string path = gConfigPath;
            if (!File.Exists(path))
            {
                path = @"StarResult\" + path;
                if (File.Exists(path))
                    gConfigPath = path;
                else
                    return;
            }

            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }
            XmlNode node = doc.SelectSingleNode("/starResult/pos");
            if (node == null) return;
            XmlNode group = node.SelectSingleNode($"//group[@no='{num}']");
            if (group == null) return;  
            var posList = group.ChildNodes;

            Grid dragItem = null;
            Image dragImg = null;

            foreach (XmlNode n in posList)
            {
                switch (n.Name)
                {
                    case "pwr":
                        dragItem = gMainView.xGrid_PWR; dragImg = gMainView.xImg_PWR;
                        break;
                    case "temp":
                        dragItem = gMainView.xGrid_Temp; dragImg = gMainView.xImg_Temp;
                        break;
                    case "info":
                        dragItem = gMainView.xGrid_Info; dragImg = gMainView.xImg_Info;
                        break;
                    case "log":
                        dragItem = gMainView.xGrid_Log; dragImg = gMainView.xImg_Log;
                        break;
                    case "step":
                        dragItem = gMainView.xGrid_Step; dragImg = gMainView.xImg_Step;
                        break;
                    case "bib":
                        dragItem = gMainView.xGrid_BIB; dragImg = gMainView.xImg_BIB;
                        break;
                    case "detail":
                        dragItem = gMainView.xGrid_Detail; dragImg = gMainView.xImg_Detail;
                        break;
                }

                if (dragItem == null) continue;
                dragItem.Width = Convert.ToDouble(n.Attributes["w"].Value);
                dragItem.Height = Convert.ToDouble(n.Attributes["h"].Value);

                if (dragItem.Height > 0)
                    dragImg.Margin = new Thickness(dragItem.Width - 20, dragItem.Height - 20, 0, 0);
            }

        }

        public static void saveLayout(int num)
        {
            XmlDocument doc = new XmlDocument();
            string path = gConfigPath;
            if (!File.Exists(path))
            {
                path = @"StarResult\" + path;
                if (File.Exists(path))
                    gConfigPath = path;
                else
                    return;
            }

            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }
            XmlNode node = doc.SelectSingleNode("/starResult/pos");
            if (node == null) return;
            XmlNode group = node.SelectSingleNode($"//group[@no='{num}']");
            if (group == null) return;
            var posList = group.ChildNodes;

            Grid dragItem = null;

            foreach (XmlNode n in posList)
            {
                switch (n.Name)
                {
                    case "pwr":
                        dragItem = gMainView.xGrid_PWR; 
                        break;
                    case "temp":
                        dragItem = gMainView.xGrid_Temp;
                        break;
                    case "info":
                        dragItem = gMainView.xGrid_Info;
                        break;
                    case "log":
                        dragItem = gMainView.xGrid_Log; 
                        break;
                    case "step":
                        dragItem = gMainView.xGrid_Step;
                        break;
                    case "bib":
                        dragItem = gMainView.xGrid_BIB; 
                        break;
                    case "detail":
                        dragItem = gMainView.xGrid_Detail; 
                        break;
                }

                if (dragItem == null) continue;
                n.Attributes["w"].Value = dragItem.Width.ToString();
                n.Attributes["h"].Value = dragItem.Height.ToString();
               
            }

            doc.Save(gConfigPath);

        }

        public static void loadBibMap()
        {
            XmlDocument doc = new XmlDocument();

            string path = "bibmap.xml";
            if (!File.Exists(path)) {
                string parent = System.IO.Directory.GetParent(System.Environment.CurrentDirectory).ToString();
                path = parent + @"\" + path;
                if (!File.Exists(path)) return;
            }

            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }

            XmlNodeList list = doc.SelectNodes("/bib-id/function/item");
            foreach (XmlNode node in list)
            { 
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidFuncMap.Add(key, node.Attributes["text"].Value);
            }
            list = doc.SelectNodes("/bib-id/device/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidDevMap.Add(key, node.Attributes["text"].Value);
            }
            list = doc.SelectNodes("/bib-id/ball/item");
            foreach (XmlNode node in list)
            {
                int key = Convert.ToInt32(node.Attributes["value"].Value);
                BidBallMap.Add(key, node.Attributes["text"].Value);
            }
        }

        public static void loadBinColor() {

            string path = "binColor.xml";
            if (!File.Exists(path)) {
                string parent = System.IO.Directory.GetParent(System.Environment.CurrentDirectory).ToString();
                path = parent + @"\" + path;
                if (!File.Exists(path)) return;
            }

            XmlDocument doc = new XmlDocument();
            try {
                doc.Load(path);
            }
            catch (Exception ex) {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }

            XmlNodeList list = doc.SelectNodes("/binColor/item");
            foreach (XmlNode node in list) {
                int bin = Convert.ToInt32(node.Attributes["bin"].Value);
                string sCol = node.Attributes["color"].Value;
                SolidColorBrush br  =  new BrushConverter().ConvertFromString(sCol) as SolidColorBrush;
                BinColorDic.Add(bin, br);
            }
        }
    }

    public static class VerInfo
    {
        const string Program = "Star Result";
        const string Date = "2024/01/08";
        const string Ver = "V1.0.4";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }
    /*
     * * V1.0.4
     *  mod : Select File Color Show
     * 
     * * V1.0.3
     *  mod : Search by Time  To Date + addDays(1)
     *  add : bin color apply
     * 
     * * V1.0.2
     *  add : bib r/c 추가
     * 
     * V1.0.1
     *  fix : Graph 출력 Data / Graph Chanel 안맞는것 수정.
     * 
     */

}
