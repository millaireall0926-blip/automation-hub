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

        //public static List<string> gSVList = new List<string>();
        //public static List<string> gPVList = new List<string>();

        // Dimms Show In Box
        //public static List<int>[] gBoxInDimmList = new List<int>[3];

        public static int gMaxDimmCount = 18;
        static Global()
        {
            //for (int i = 0; i < 3; i++) 
            //    gBoxInDimmList[i] = new List<int>();


            loadConfig();
        }

        public static void loadConfig()
        {            
            string path = "RDimmRack.xml";
            if (!File.Exists(path))
            {
                path = @"..\" + path;
                if (!File.Exists(path))
                {
                    gConfigDoc = null;
                    Growl.ClearGlobal();
                    Growl.InfoGlobal("Not Found RDimmRack.xml");
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
            var columnNode = Global.gConfigDoc.SelectSingleNode("/lpcam-rack/rack-map");
            Global.gMaxDimmCount = Convert.ToInt32(columnNode.Attributes["dims"].Value);

            XmlNode node = gConfigDoc.SelectSingleNode("/lpcam-rack/log/temp");
            if (node != null) {
                gTempPath = node.Attributes["path"].Value;
                
            }
            
            //node = gConfigDoc.SelectSingleNode("htc-rack/rack-map");
            //gMaxDimmCount = Convert.ToInt32(node.Attributes["dims"].Value);
            /*
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
            */
        }
    }


    public static class VerInfo
    {
        const string Program = "Temp View Exclusive use RDIMM";
        const string Date = "2024/07/25";
        const string Ver = "V1.0.0";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }

    /*
     * 2024/01/24
     *    Guid 추가
     * 
        V1.0.0
           Box 별 Show Hide
    */

}
