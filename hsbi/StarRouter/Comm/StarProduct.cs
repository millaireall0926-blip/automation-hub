using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Markup;
using SuperSocket;
using SuperSocket.SocketBase;
using SuperSocket.SocketBase.Protocol;
using StarBase.Net;
using StarBase;
using System.Net;
using System.Security.Cryptography;
using System.Runtime.Remoting.Messaging;
using StarRouter.ViewModel;
using System.Collections;
using System.Xml;
using System.Windows.Interop;
using System.Windows.Media;
using System.Xml.Linq;
using System.ComponentModel;
using SuperSocket.Facility.Protocol;
using SuperSocket.Common;
using System.Windows.Media.Imaging;
using System.Windows;



namespace StarRouter.Comm {

    public class ProductPacket : BinaryRequestInfo {

        public StarHeader header;

        public ProductPacket(byte[] head, byte[] body)
            : base(null, body) {
            header = MConv.ByteToStruct<StarHeader>(StarProtocal.HEADER_SIZE, head);
        }
    }
    public class ProductFilter : FixedHeaderReceiveFilter<ProductPacket> {
        public ProductFilter() : base(StarProtocal.HEADER_SIZE) {

        }

        protected override int GetBodyLengthFromHeader(byte[] header, int offset, int length) {

            return IPAddress.NetworkToHostOrder((BitConverter.ToInt32(header, offset + 8)));

        }

        protected override ProductPacket ResolveRequestInfo(ArraySegment<byte> header, byte[] bodyBuffer, int offset, int length) {


            ProductPacket pack = new ProductPacket(
                header.CloneRange(0, StarProtocal.HEADER_SIZE),
                bodyBuffer.CloneRange(offset, length));

            pack.header.Magic = IPAddress.NetworkToHostOrder(pack.header.Magic);
            pack.header.Mode = IPAddress.NetworkToHostOrder(pack.header.Mode);
            pack.header.Len = IPAddress.NetworkToHostOrder(pack.header.Len);
            pack.header.OrgLen = IPAddress.NetworkToHostOrder(pack.header.OrgLen);

            return pack;
        }

    }

    public class StarProduct : AppSession<StarProduct, ProductPacket> , INotifyPropertyChanged{

        #region VM 용
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        #endregion

        public StarSender _sender = new StarSender();


        string _name = "";
        public string Name {
            get => _name;
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }
        string _ip;
        public string IP { get => _ip; set { _ip = value; OnPropertyChanged(nameof(IP)); } }

        int _port;
        public int Port { get => _port; set { _port = value; OnPropertyChanged(nameof(Port)); } }

        public MSG_TYPE ProductType { get; set; } = MSG_TYPE.DEST_OBSERVER;

        BitmapImage _icon;
        public BitmapImage Icon {
            get => _icon;
            set {
                _icon = value;
                OnPropertyChanged("Icon");
            }
        }

        SolidColorBrush _statBrush;
        public SolidColorBrush StatBrush {
            get => _statBrush;
            set {
                _statBrush = value;
                OnPropertyChanged("StatBrush");
            }
        }

        public MainVM _mvm;        

        public int Index { get; set; }

        public StarProduct() {
            _sender.Owner = OWNER.PRODUCT;
            _sender.sendFunc = Send;
            _mvm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
            
        }

        public void doIdentify(XmlNode node) {
            string name = node.Attributes["me"].Value;
            Name = name;
            ITEM_DEF type = ITEM_DEF.OS;
            switch (Name) { 
                case "StarOS":
                    ProductType = MSG_TYPE.SESSION_OS;
                    type = ITEM_DEF.OS;
                    break;
                case "StarMaint":
                    ProductType = MSG_TYPE.SESSION_MAINT;
                    type = ITEM_DEF.MAINT;
                    break;
                case "StarWeb":
                    ProductType = MSG_TYPE.SESSION_WEB;
                    type = ITEM_DEF.WEB;
                    break;
                default:
                    ProductType = MSG_TYPE.SESSION_OBSERVER;
                    break;
            }

            Application.Current.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal, new Action(() =>
            {
                Icon = _mvm._itemDefs[(int)type].Image;
                StatBrush = _mvm._itemDefs[(int)type].Brush;
            }));
            
        }

        public void sendTempData(XmlNode node) {
            _mvm._webData.State = node.Attributes["st"].Value;
            _mvm._webData.PV = node.Attributes["pv"].Value;
        }
        
        public void recvRouterCmd(byte[] data) {
            string str = MConv.ByteToString(data);
            XmlDocument xml = new XmlDocument();
            try {
                xml.LoadXml(str);
            }
            catch (Exception ex) {
                _mvm.addLog(LOG_TYPE.ERROR, ex.Message, Name, System.Reflection.MethodBase.GetCurrentMethod().Name);
                return;
            }
            XmlNode node = xml.DocumentElement.FirstChild;
            string tag = node.Name;
            switch (tag) {
                case "identify":
                    doIdentify(node);
                    break;
                case "cu-list":
                    sendCMList();
                    break;
                case "plc-list":
                    sendPLCList();
                    break;
                case "temp-data":
                    sendTempData(node);
                    break;
                case "web-res":
                    break;
            }
        }

        public void processPacket(ProductPacket packetData) {
            MSG_TYPE dest = (MSG_TYPE)(packetData.header.Mode & 0x00ff);
            MSG_TYPE type = (MSG_TYPE)(packetData.header.Mode & 0xff00);
            if (dest == MSG_TYPE.SESSION_ROUTER) {
                // Call Server Function
                recvRouterCmd(packetData.Body);
                return;
            }

            string recv = MConv.ByteToString(packetData.Body);//.TrimEnd('\0');            

            if (recv.Substring(0, 5) != "<star") return;

            string path = recv.Substring(14, 6);
            // Find Path CM
            IList<StarCM> cms = _mvm.CMs;
            StarCM cm = cms.FirstOrDefault(e => e.Name == path);
            if (cm == null) return;
            if (!cm.IsConnected) return;

            if (type == MSG_TYPE.TYPE_REQFILE) {
                cm._reqFileProducts.Enqueue(this);
            }
             
            cm._sender.processSend(
                packetData.header.Mode,
                packetData.Body,
                packetData.header.Len);

            if (type == MSG_TYPE.TYPE_SENDFILE) {
                _mvm.addLog(LOG_TYPE.DEBUG, $"[Product -> CM][Send File] {type.ToString()} ", Name);
            }else
                _mvm.addLog(LOG_TYPE.DEBUG, $"[Product -> CM] {MConv.ByteToString(packetData.Body)} ", Name);
        }


        // No Use in Product (Server Session)
        public void processRecvData(int mode, byte[] data, int len) {
            
        }

        public void sendCMList(bool Auto = false) {
            StringBuilder msg = new StringBuilder();
            msg.Append("<cu-list>");
            IList cms = _mvm.CMs;

            foreach (StarCM cm in cms) {
                
                msg.Append($"<cu id=\"{cm.Name}\" ip=\"{cm.IP}\" ");
                msg.Append($"pgm=\"{(cm.IsPgm ? 1 : 0)}\" ");
                msg.Append($"online=\"{(cm.IsConnected ? 1 : 0)}\" ");
                msg.Append($"bid=\"{cm.BibID}\" stat=\"{cm.CMStat}\" ");
                msg.Append($"diag=\"{(cm.IsDiag?1:0)}\" />\n");
            }
            msg.Append("</cu-list>");

            _sender.writeResult(Index, RESULT.OK,
                Auto ? "" : "cu-list", msg.ToString());
        }

        public void sendPLCList() {
            
            _sender.writeResult(Index, RESULT.OK, "plc-config",_mvm.PlcData);
        }

        

        protected override void OnSessionStarted() {
            base.OnSessionStarted();
        }

        protected override void HandleException(Exception e) {
            this.Send("오류 : {0}", e.Message);
        }

        protected override void OnSessionClosed(CloseReason reason) {
            base.OnSessionClosed(reason);
        }

    }
}
