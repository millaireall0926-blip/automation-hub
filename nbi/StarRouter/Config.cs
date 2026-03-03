
using HandyControl.Controls;
using StarRouter.Comm;
using StarRouter.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Xml;
using System.Xml.Linq;


namespace StarRouter {
    public class Config {


        XmlDocument _doc = new XmlDocument();
        public Config() {
            
        }

        public string LoggerPath { get; set; } = "";
        public bool IsShowLogo { get; set; } = true;
        public bool loadDoc() {

            string path = "slotmap.xml";
            if (!File.Exists(path)) {
                path = @"StarRouter\" + path;                
                if (!File.Exists(path))                
                    return false;
            }

            try {
                _doc.Load(path);
            }
            catch (Exception ex) {
                Console.WriteLine("catch {1}: {0}", ex.Message,
                    System.Reflection.MethodBase.GetCurrentMethod().Name);
                return false;
            }

            var node = _doc.SelectSingleNode("/router");
            if (node != null)
            {
                if (node.Attributes["logo"] != null)
                {
                    if (Convert.ToInt32(node.Attributes["logo"].Value) == 0)
                        IsShowLogo = false;
                }
            }

            node = _doc.SelectSingleNode("/router/setting/log");
            if (node != null) {
                LoggerPath = node.Attributes["path"].Value;
            }

            loadCMList();
            loadPLCData();
            return true;
        }

        public bool loadCMList() {
            var nodes = _doc.SelectNodes("/router/cm-list/cm");
            if (nodes == null) return false;
            MainVM vm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
            vm.CMs.Clear();
            foreach (XmlNode node in nodes) {
                StarCM cm = new StarCM();
                cm.Name = node.Attributes["id"].Value;
                cm.IP = node.Attributes["ip"].Value;
                cm.Port = Convert.ToInt32(node.Attributes["port"].Value);

                vm.CMs.Add(cm);
            }

            return true;
        }

        public void loadPLCData() {
            XmlNode node = _doc.SelectSingleNode("/router/plc-config");
            MainVM vm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
            vm.PlcData = node.OuterXml;
        }

        public void setCMList(string prev, string name, string ip, int port) {
            //XmlNode nodeCM = _doc.SelectSingleNode($"/router/cm-list/cm[@id='{prev}']");
            XmlNode node = _doc.SelectSingleNode("/router/cm-list");
            if (node == null) return;
            XmlNode nodeCM = node.SelectSingleNode($"//cm[@id='{prev}']");
            if (nodeCM == null) return;
            
            nodeCM.Attributes["id"].Value = name;
            nodeCM.Attributes["ip"].Value = ip;
            nodeCM.Attributes["port"].Value = port.ToString();

            _doc.Save("slotmap.xml");
        }

        public string Version = "Ver 1.0.4";
        /*
         * 
         * 
            2025/09/09
         * Version = "Ver 1.0.4";
         *   StarCM CMFilter 예외처리.
         *   *   StarCM GetBodyLengthFromHeader  Magic Code 값 으로 Packet문제 보강.
         * 
         * 
         * *   2025/02/11
         * Version = "Ver 1.0.3";
         *   StarCM ProcessPacket의 첫번째 recvSlotStat 예외처리. 
         * 
         *   2025/02/06
         * * ver 1.0.2 
         *   StarCM GetBodyLengthFromHeader 중 이상동작이 생기는 경우가 있음.   
         *   예외처리 강화
         * 
         * ver 1.0.1 
         *   cm 비정상 종료시 (power off등) 
         *   ping fail시 cm IsConnect도 fail로 해야  Connect List check후 정상적으로 cm list 전송
         * 
         * ver 1.0.0  StarRouter WPF Ver 1차 완료
         * 
         */

    }


}
