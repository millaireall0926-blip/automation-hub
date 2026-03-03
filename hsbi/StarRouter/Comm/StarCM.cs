using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarBase.Net;
using StarBase;
using SuperSocket.ClientEngine;
using System.Net.NetworkInformation;
using System.Windows.Markup;
using System.Threading;
using System.Net;
using System.Xml;
using StarRouter.ViewModel;
using System.Collections.ObjectModel;
using System.ComponentModel;
using SuperSocket.ProtoBase;
using System.Runtime.Remoting.Messaging;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows;
using System.Windows.Threading;
using System.Windows.Input;

namespace StarRouter.Comm {
    
    public class CMPacket : PackageInfo<string, byte[]> {
        public StarHeader header;
        
        public CMPacket(byte[] head, byte[] body) : base(null, body) {
            header = MConv.ByteToStruct<StarHeader>(StarProtocal.HEADER_SIZE, head);
        }
    }

    public class CMFilter : FixedHeaderReceiveFilter<CMPacket> 
    {
        public CMFilter() : base(StarProtocal.HEADER_SIZE) 
        {
        }

        protected override int GetBodyLengthFromHeader(IBufferStream bufferStream, int length) {
            
            int len = (int)bufferStream.Length;
                    
            try {
                if (len < StarProtocal.HEADER_SIZE)                
                    throw new Exception("Failed to extract length from header segments.");
                

                byte[] buf = new byte[16];
                bufferStream.Read(buf, 0, 16);

                int magic = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(buf, 0));
                if (magic != StarProtocal.HDR_MAGIC)
                {
                    MainVM mv = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
                    mv.addLog(LOG_TYPE.ERROR, $"MAGIC Code Not Matched : {magic.ToString("X8")}", "CMFilter", "GetBodyLengthFromHeader");
                    return -1;
                }

                int bodyLen = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(buf, 8));

                if (bodyLen <= 0 || bodyLen > (256 * 1024))
                {
                    MainVM mv = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
                    mv.addLog(LOG_TYPE.ERROR, $"MaxBody Len 64K But : {bodyLen}", "CMFilter", "GetBodyLengthFromHeader");
                    return -1; // 패킷 폐기/연결 종료 신호
                }
                return bodyLen;
            }
            catch (Exception ex) {
                //Console.WriteLine("catch {1}: {0}", ex.Message,
                //    System.Reflection.MethodBase.GetCurrentMethod().Name);
                MainVM mv =  App.Current.Services.GetService(typeof(MainVM)) as MainVM ;
                mv.addLog(LOG_TYPE.ERROR, $"bufferStream.Length {len} & "+ ex.Message, "CMFilter", System.Reflection.MethodBase.GetCurrentMethod().Name);
                return -1; 
            }

            
        }

        public override CMPacket ResolvePackage(IBufferStream bufferStream) {

            int len = (int)bufferStream.Length;
            try
            {
                if (len < StarProtocal.HEADER_SIZE)
                {
                    MainVM mv = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
                    mv.addLog(LOG_TYPE.ERROR, $"Invalid packet length: {len}", "CMFilter", "ResolvePackage");
                    return null; // 🔥 SuperSocket이 패킷을 무시하도록 함
                }                

                var headerBytes = new byte[StarProtocal.HEADER_SIZE];
                bufferStream.Read(headerBytes, 0, StarProtocal.HEADER_SIZE);
                int bodyLen = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(headerBytes, 8));



                if (bodyLen <= 0 || bufferStream.Length != StarProtocal.HEADER_SIZE + bodyLen)
                //if (bodyLen < 0 || bufferStream.Length < bodyLen)
                {
                    MainVM mv = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
                    mv.addLog(LOG_TYPE.ERROR, $"Remind packet length: {len} headers {bodyLen}", "CMFilter", "ResolvePackage");
                    return null; // 🔥 SuperSocket이 패킷을 무시하도록 함
                }

                var bodyBytes = new byte[bodyLen];
                bufferStream.Read(bodyBytes, 0, bodyLen);

                var pack = new CMPacket(headerBytes, bodyBytes);

                pack.header.Magic = IPAddress.NetworkToHostOrder(pack.header.Magic);
                pack.header.Mode = IPAddress.NetworkToHostOrder(pack.header.Mode);
                pack.header.Len = IPAddress.NetworkToHostOrder(pack.header.Len);
                pack.header.OrgLen = IPAddress.NetworkToHostOrder(pack.header.OrgLen);

                return pack;

            }
            catch (Exception ex)
            {
                MainVM mv = App.Current.Services.GetService(typeof(MainVM)) as MainVM;
                mv.addLog(LOG_TYPE.ERROR, $"bufferStream.Length {len} & " + ex.Message, "CMFilter", System.Reflection.MethodBase.GetCurrentMethod().Name);
                return null;
            }
        }
    }


    public class SendFileTimer {
        public DispatcherTimer timer;
        public int timerCount = 5; 
        public int timerProgress = 0;

        public delegate void TimeOver();
        public TimeOver timeOver = null;

        public SendFileTimer() {
            timer = new DispatcherTimer();
        }

        public bool isActive { get => timer.IsEnabled; }

        public void start() { 
            timer.Start();
        }

        public void tick() {
            if (timerProgress >= timerCount) {
                done();
                if (timeOver != null) 
                    timeOver();                
            }
            timerProgress++;            
        }

        public void reset() {
            timerProgress = 0;
        }
        public void done() {
            timerProgress = 0;
            timer.Stop(); 
        }
    }


    public class StarCM : EasyClient, INotifyPropertyChanged {

        
        #region VM 용
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        #endregion

        public StarSender _sender = new StarSender();        
        MainVM _mvm;

        SendFileTimer sendFileTimer = new SendFileTimer();

        

        //EasyClient EasyClient;

        #region Connection 
        string _name = "";
        public string Name {
            get=>_name; 
            set { _name = value; OnPropertyChanged(nameof(Name)); }
        }
        public string _ip;
        public string IP { get => _ip; set { _ip = value; OnPropertyChanged(nameof(IP)); } }
        public int _port;
        public int Port { get => _port; set { _port = value; OnPropertyChanged(nameof(Port)); } }

        public bool _isConnected = false;
        public bool IsConnected { get => _isConnected;
            set {

                _isConnected = value;

               _mvm.UpdateFlag_CMList = true;
               Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
               {
                   Icon = _mvm._itemDefs[_isConnected ? (int)ITEM_DEF.CONNECT : (int)ITEM_DEF.NODEAMON].Image;
                   StatBrush = _mvm._itemDefs[_isConnected ? (int)ITEM_DEF.CONNECT : (int)ITEM_DEF.NODEAMON].Brush;
               }));


            }
        }
        public DateTime TimeLast { get; set; } = DateTime.Now;

        #endregion

        #region CM Info Data
        public int CMStat { get; set; }

        bool _isPgm = false;
        public bool IsPgm { 
            get => _isPgm; 
            set {
                bool prev = _isPgm;
                _isPgm = value;
//                if (_isPgm != prev) _mvm.UpdateFlag_CMList = true;
            } 
        }
        bool _isDiag = false;
        public bool IsDiag { 
            get => _isDiag;
            set {
                bool prev = _isDiag;
                _isDiag = value;
                if (_isDiag != prev) _mvm.UpdateFlag_CMList = true;
            }
        }
        public bool IsContact  { get; set; }

        string _bibID = "";
        public string BibID {
            get => _bibID;
            set {
                string prev = _bibID;
                _bibID = value;
                if (_bibID != prev) _mvm.UpdateFlag_CMList = true;
            }
        }        
        public string PgmName { get; set; }
        public string LotID { get; set; }
        public string FlowName { get; set; }
        public string StepName { get; set; }
        public string StartTime { get; set; }


        BitmapImage _icon;
        public BitmapImage Icon {
            get => _icon;
            set {
                _icon = value;
                OnPropertyChanged("Icon");
                CommandManager.InvalidateRequerySuggested();
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

        //        public DateTime StartTime { get; set; }        
        

        public Queue<StarProduct> _reqFileProducts = new Queue<StarProduct>();        
        #endregion
        public StarCM() {
            _sender.Owner = OWNER.CM;            
            _sender.sendFunc += send;
            

            Connected += done_Connected;            
            Error += done_Error;
            Closed += done_Closed;

            ReceiveBufferSize = 128 * 1024;//(int)MSG_TYPE.CHUNK_SZ * 1024;

            this.Initialize<CMPacket>(new CMFilter(), (packet) => processPacket(packet));
            pingSender.PingCompleted += new PingCompletedEventHandler(PingCompletedCallback);
            _mvm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;

            Icon = _mvm._itemDefs[(int)ITEM_DEF.DISCON].Image;
            StatBrush = _mvm._itemDefs[(int)ITEM_DEF.DISCON].Brush;
            sendFileTimer.timeOver += sendFileTimeOut;

         
        }

        public void sendFileTimeOut() {
            if (_reqFileProducts.Count != 0)
                _reqFileProducts.Dequeue();
            // error log
        }


        public void processPacket(CMPacket packetData) {


            if (packetData == null)
            {                
                _mvm.addLog(LOG_TYPE.ERROR, "Received null packet, ignoring.", "StarCM", "processPacket");
                return; // 🔥 패킷이 null이면 바로 리턴하여 처리 중단
            }

            MSG_TYPE dest = (MSG_TYPE)(packetData.header.Mode & 0x00ff);
            MSG_TYPE type = (MSG_TYPE)(packetData.header.Mode & 0xff00);

//            string recv = MConv.ByteToString(packetData.Body);
//            Console.WriteLine($"recv {recv}");
            if (type == MSG_TYPE.TYPE_SLOTSTAT) {
                if (recvSlotStat(packetData.Body) == false) return;
            }

            TimeLast = DateTime.Now;

            if (dest == MSG_TYPE.DEST_ROUTER) {
                recvRouterCmd(packetData.Body);
            }
            else if (type == MSG_TYPE.TYPE_REQFILE || type == MSG_TYPE.TYPE_REQDONE) {

                if (_reqFileProducts.Count == 0) return;
                StarProduct product = _reqFileProducts.Peek();
                if (!sendFileTimer.isActive)
                    sendFileTimer.start();

                product._sender.processSend(
                    packetData.header.Mode,
                    packetData.Body,
                    packetData.header.Len);
                if (type == MSG_TYPE.TYPE_REQDONE) {
                    _reqFileProducts.Dequeue();
                    sendFileTimer.done();
                }
                _mvm.addLog(LOG_TYPE.DEBUG, $"[CM -> Product][Send File] {type.ToString()} ", Name);
            }
            else {
                IList<StarProduct> products = _mvm.Products;
                foreach (StarProduct product in products) {
                    if (product.ProductType == MSG_TYPE.SESSION_WEB)
                        continue;

                    product._sender.processSend(
                        packetData.header.Mode,
                        packetData.Body,
                        packetData.header.Len);

                    _mvm.addLog(LOG_TYPE.DEBUG, $"[CM -> Product] {MConv.ByteToString(packetData.Body)} ", Name);
                }
            }

        }


        public void send(byte[] data, int offset, int length) {
            try
            {
                Send(data);
            }
            catch (Exception e)
            {
                _mvm.addLog(LOG_TYPE.ERROR, $"[CM -> Product] SendFail len={data.Length} ex={e.GetType().Name} msg={e.Message}", Name);
            }
        }

        #region Ping 
        Ping pingSender = new Ping();
        PingOptions options = new PingOptions(8, true);

        public bool IsPing { get; set; } = false;
        byte[] pingCmd = Encoding.ASCII.GetBytes("is Good? from star");
        public void doPing() {
            pingSender.SendAsync(IP, 2000, pingCmd, options);
        }

        int _pingConCheckCount = 0;
        void PingCompletedCallback(object sender, PingCompletedEventArgs e) {

            bool prev = IsPing;
            if (e.Cancelled) {
                IsPing = false; return;
            }
            if (e.Error != null) {
                IsPing = false; return;
            }

            PingReply reply = e.Reply;
            IsPing = (reply.Status == IPStatus.Success);
            if (IsPing) _pingConCheckCount = 0;

            if (IsConnected && !IsPing)
            {
                if (_pingConCheckCount > 3)
                    IsConnected = false;
                _pingConCheckCount++;
            }// 비정상 종료시의 Connection 처리
            if (prev != IsPing) {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                {
                    Icon = _mvm._itemDefs[IsPing ? (int)ITEM_DEF.NODEAMON : (int)ITEM_DEF.DISCON].Image;
                    StatBrush = _mvm._itemDefs[IsPing ? (int)ITEM_DEF.NODEAMON : (int)ITEM_DEF.DISCON].Brush;
                }));
            }


        }
        #endregion
        // -- Send File
        // <star-p client=\"%d\" path=\"%s\">
        //  <send-file dir=\"%s\" name=\"%s\" ofs=\"%ld\" size=\"%ld\" compress=\"%s\" org_sz=\"%ld\">", 

        // -- Result
        // <star-p path=\"%s\">
        //   <result seq=\"%d\" cmd=\"%s\" return=\"%s\" type=\"xml\">%s</result></star-p>

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
            DateTime dt = DateTime.Now;
            string sendMsg = "";
            switch (tag) {
                case "req-time":
                    sendMsg = $"<star-p path=\"{Name}\">" +
                        $"<set-time date=\"" +
                        $"{dt.Year.ToString().PadLeft(4,'0')}" +
                        $"{dt.Month.ToString().PadLeft(2, '0')}" +
                        $"{dt.Day.ToString().PadLeft(2, '0')}" +
                        $"T" +
                        $"{dt.Hour.ToString().PadLeft(2, '0')}" +
                        $"{dt.Minute.ToString().PadLeft(2, '0')}" +
                        $"{dt.Second.ToString().PadLeft(2, '0')}\" " +
                        $"/></star-p>";


                    //<star-p path=\"%s\"><set-time date=\"%sZ\"/></star-p>

                    _sender.processSend((int)MSG_TYPE.DEST_DAEMON, sendMsg);
                    break;
            }

//            _mvm.addLog(LOG_TYPE.DEBUG, $"<Router -> CM> {sendMsg} ", Name);
        }

        /*
        <star-p path = "Slot02">
         <result seq="1" cmd="slot-stat" return="OK" type="xml">
          <slot-stat bid = "0x1234" pgm="0" diag="0" >
            <run-step flow = "" lotid="" con="0" path="" name="" st="" step="" />
          </slot-stat>
         </result>
        </star-p>
        */
        public bool recvSlotStat(byte[] data) {
            string str = MConv.ByteToString(data);
            XmlDocument xml = new XmlDocument();
            try {
                xml.LoadXml(str);
            }catch (Exception ex) {
                _mvm.addLog(LOG_TYPE.ERROR, ex.Message, Name, System.Reflection.MethodBase.GetCurrentMethod().Name);
                return false;
            }
            XmlNode node = xml.SelectSingleNode("/star-p/result/slot-stat");
            if (node == null) return false;
            BibID = node.Attributes["bid"].Value;
            IsPgm = (Convert.ToInt32(node.Attributes["pgm"].Value) == 1);
            IsDiag = (Convert.ToInt32(node.Attributes["diag"].Value) == 1);
            node = xml.SelectSingleNode("/star-p/result/slot-stat/run-step");
            if (node == null) return false;
            LotID = node.Attributes["lotid"].Value;
            IsContact = (Convert.ToInt32(node.Attributes["con"].Value) == 1);
            PgmName = node.Attributes["name"].Value;
            StartTime = node.Attributes["st"].Value;
            // -> to String : DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") 
            // string strStart = MConv.convertTimeText(node.Attributes["st"].Value);
            // if (strStart != "") StartTime = Convert.ToDateTime(strStart);
            StepName = node.Attributes["step"].Value;
            return true;
        }


        public async void doConnect() {            

            try {
                await ConnectAsync(new IPEndPoint(IPAddress.Parse(IP), Port));                
            }
            catch (Exception ex) {
                _mvm.addLog(LOG_TYPE.ERROR, ex.Message, Name, System.Reflection.MethodBase.GetCurrentMethod().Name);
            }
        
        }

        public void done_Connected(object sender, EventArgs e) {            
            IsConnected = true;
            // Send Identify 
            _sender.processSend((int)MSG_TYPE.DEST_DAEMON, "<star-p client=\"0\"><identify me=\"router\"/></star-p>");
            // Send Timer            
        }

        public void done_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e) {
            _mvm.addLog(LOG_TYPE.ERROR, $"[SessError]  ex={e.Exception.GetType().Name} msg={e.Exception.Message}", Name);
            Close();
        }
        public void done_Closed(object sender, EventArgs e) {
            IsConnected = false;
            IsPing = false;
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
            {
                Icon = _mvm._itemDefs[(int)ITEM_DEF.DISCON].Image;
                StatBrush = _mvm._itemDefs[(int)ITEM_DEF.DISCON].Brush;
            }));
        }


        public void modifyItem(string name, string ip, int port) {
            string prev = Name;

            if (IsConnected) {
                string msg = $"<star-p><map name=\"{name}\"/><star-p>";
                _sender.processSend((int)MSG_TYPE.DEST_DAEMON, msg);
            }

            Config conf = App.Current.Services.GetService(typeof(Config)) as Config;

            conf.setCMList(prev, Name, ip, port);

            Name = name;
            IP = ip;
            Port = port;    

            Close();
        }
    }
}
