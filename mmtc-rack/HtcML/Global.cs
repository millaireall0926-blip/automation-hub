using HandyControl.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using Tensorflow;

namespace HtcML
{
    public static class Global
    {

        static public XmlDocument gConfigDoc = new XmlDocument();

        public static List<int>[] gBoxInDimmList = new List<int>[3];

        public static int gMaxDimmCount = 16;
        public static bool gIs16Dimm = true;

        static Global()
        {
            for (int i = 0;i < 3 ;i++)            
                gBoxInDimmList[i] = new List<int> ();
            
        }

        public static void loadMap()
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

            var node = gConfigDoc.SelectSingleNode("htc-rack/rack-map");
            gMaxDimmCount = Convert.ToInt32(node.Attributes["dims"].Value);
            if (Global.gMaxDimmCount == 16) gIs16Dimm = true;
            else gIs16Dimm = false;


        }

    }


    public static class VerInfo
    {

        const string Program = "HTC ML";
        const string Date = "2024/02/20";
        const string Ver = "V1.0.0";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }
}
