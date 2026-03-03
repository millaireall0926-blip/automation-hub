using HandyControl.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using TempView.ViewModel;

namespace TempView
{
    public static class Global
    {

        public static MainVM gMainVM;
        public static MainWindow gMainView;
        public static SelectionVM gSelectVM;
        public static GraphVM gGraphVM;

        public static string gTempPath = "";
        static public XmlDocument gConfigDoc = new XmlDocument();

        public static List<string> gSVList = new List<string>();
        public static List<string> gPVList = new List<string>();

        // Dimms Show In Box
        public static List<int>[] gBoxInDimmList = new List<int>[3];

        public static int gMaxDimmCount = 16;
        static Global()
        {
            for (int i = 0; i < 3; i++) 
                gBoxInDimmList[i] = new List<int>();


            loadConfig();
        }

        public static void loadConfig()
        {            
            string path = "HTCRack.xml";
            if (!File.Exists(path))
            {
                path = @"..\" + path;
                if (!File.Exists(path))
                {
                    gConfigDoc = null;
                    Growl.ClearGlobal();
                    Growl.InfoGlobal("Not Found HTCRack.xml");
                    return;
                }
            }

            try
            {
                gConfigDoc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return;
            }

            XmlNode node = gConfigDoc.SelectSingleNode("/htc-rack/log/temp");
            if (node != null) {
                gTempPath = node.Attributes["path"].Value;
                
            }

            node = gConfigDoc.SelectSingleNode("htc-rack/rack-map");
            gMaxDimmCount = Convert.ToInt32(node.Attributes["dims"].Value);

            node = gConfigDoc.SelectSingleNode("/htc-rack/rack-map/rack");
            int boxIdx = 0;
            foreach (XmlNode boxNode in node.ChildNodes)
            {

                foreach (XmlNode subNode  in  boxNode.ChildNodes)
                {

                    if (subNode.Name != "sub") continue;
                    var svNode = subNode.SelectSingleNode("sv");
                    string val = svNode.InnerText;
                    string[] buf = val.Split(new char[] { ',' });
                    foreach (string s in buf)
                        gSVList.Add((Convert.ToInt32(s) + 1).ToString());

                    var pvNode = subNode.SelectSingleNode("pv");
                    val = pvNode.InnerText;
                    buf = val.Split(new char[] { ',' });
                    foreach (string s in buf)
                        gPVList.Add((Convert.ToInt32(s) + 1).ToString());


                    var dimNode = subNode.SelectSingleNode("dim");
                    val = dimNode.InnerText;
                    buf = val.Split(new char[] { ',' });
                    foreach (string s in buf)
                        gBoxInDimmList[boxIdx].Add((Convert.ToInt32(s)));

                }

                boxIdx++;
            }

        }
    }


    public static class VerInfo
    {
        const string Program = "Temp Viewr";
        const string Date = "2025/08/04";
        const string Ver = "V1.1.2";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }

    /*
     * * ** * "2025/08/04";
     *          "V1.1.2";
     *          Graph 데이터 양 늘림 60일치까지 표현하게
     *          
     * ** * "2025/08/04";
     *          "V1.1.1";
     *          x측 표현 누적 시간으로 변경
     *          
     *          
     * ** * "2025/06/20";
     *        "V1.1.0";
     *          Water 도 포함.
     *
     ** * 2024/10/09
     *     x 축 label 시간으로
     *
     * * 2024/07/08
     *    Graph Buffer 1_000_000 -> 1_600_000
     * 
     * 2024/01/24
     *    Guid 추가
     * 
        V1.0.0
           Box 별 Show Hide
    */

}
