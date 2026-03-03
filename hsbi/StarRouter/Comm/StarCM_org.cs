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


namespace StarRouter.Comm {


    public class StarCM : AsyncTcpSession , INotifyPropertyChanged {
        
        #region VM 용
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        #endregion

        public StarSession _session = new StarSession();        
        MainVM _mvm;
        

        #region Connection 
        public string Name { get; set; }
        public string IP { get; set; }
        public int Port { get; set; }

        public bool _isConnected = false;
        public bool IsConnected { get => _isConnected;
            set {
                bool prev = _isConnected;
                _isConnected = value;
                if (_isConnected != prev) _mvm.UpdateFlag_CMList = true;
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
//        public DateTime StartTime { get; set; }


        public Queue<StarProduct> _reqFileProducts = new Queue<StarProduct>();
        
        
        #endregion

        public StarCM() {
            _session.Owner = OWNER.CM;

            _session.closeFunc += Close;
            _session.recvDoneFunc += processRecvData;
            _session.sendFunc += Send;

            
            Connected += done_Connected;
            DataReceived += done_DataReceived;
            Error += done_Error;
            Closed += done_Closed;

            ReceiveBufferSize = 128 * 1024;//(int)MSG_TYPE.CHUNK_SZ * 1024;

            pingSender.PingCompleted += new PingCompletedEventHandler(PingCompletedCallback);
            _mvm = App.Current.Services.GetService(typeof(MainVM)) as MainVM;

            
        }


        #region Ping 
        Ping pingSender = new Ping();
        PingOptions options = new PingOptions(8, true);
        public bool IsPing { get; set; } = false;
        byte[] pingCmd = Encoding.ASCII.GetBytes("is Good? from star");
        public void doPing() {
            pingSender.SendAsync(IP, 2000, pingCmd, options);
        }

        void PingCompletedCallback(object sender, PingCompletedEventArgs e) {
            if (e.Cancelled) {
                IsPing = false; return;
            }
            if (e.Error != null) {
                IsPing = false; return;
            }

            PingReply reply = e.Reply;
            IsPing = (reply.Status == IPStatus.Success);


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
                Console.WriteLine("catch {1}: {0}", ex.Message,
                        System.Reflection.MethodBase.GetCurrentMethod().Name);
                return;
            }

            XmlNode node = xml.DocumentElement.FirstChild;
            string tag = node.Name;
            DateTime dt = DateTime.Now;
            switch (tag) {
                case "req-time":
                    string stime  = $"<star-p path=\"{Name}\">" +
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

                    _session.processSend((int)MSG_TYPE.DEST_DAEMON, stime);
                    break;
            }
        }

        public void processRecvData(int mode, byte[] data, int len) {

            MSG_TYPE dest = (MSG_TYPE)(mode & 0x00ff);
            MSG_TYPE type = (MSG_TYPE)(mode & 0xff00);
            if (type == MSG_TYPE.TYPE_SLOTSTAT) {                
                recvSlotStat(data);                
            }

            TimeLast = DateTime.Now;

            if (dest == MSG_TYPE.DEST_ROUTER) {
                recvRouterCmd(data);
            }
            else if (type == MSG_TYPE.TYPE_REQFILE || type == MSG_TYPE.TYPE_REQDONE) {

                if (_reqFileProducts.Count == 0) return;                
                StarProduct product = _reqFileProducts.Peek();
                
                product._session.processSend(mode, data, len);
                if (type == MSG_TYPE.TYPE_REQDONE)
                    _reqFileProducts.Dequeue();
                
            }            
            else {
                IList<StarProduct> products = _mvm.Products;
                foreach (StarProduct product in products) {
                    if (product.ProductType == MSG_TYPE.SESSION_WEB)
                        continue;

                    product._session.processSend(mode, data, len);
                }
            }

            
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
        public void recvSlotStat(byte[] data) {
            string str = MConv.ByteToString(data);
            XmlDocument xml = new XmlDocument();
            try {
                xml.LoadXml(str);
            }catch (Exception ex) {
                Console.WriteLine("catch {1}: {0}", ex.Message,
                       System.Reflection.MethodBase.GetCurrentMethod().Name);
                return;
            }
            XmlNode node = xml.SelectSingleNode("/star-p/result/slot-stat");
            if (node == null) return;
            BibID = node.Attributes["bid"].Value;
            IsPgm = (Convert.ToInt32(node.Attributes["pgm"].Value) == 1);
            IsDiag = (Convert.ToInt32(node.Attributes["diag"].Value) == 1);
            node = xml.SelectSingleNode("/star-p/result/slot-stat/run-step");
            if (node == null) return;
            LotID = node.Attributes["lotid"].Value;
            IsContact = (Convert.ToInt32(node.Attributes["con"].Value) == 1);
            PgmName = node.Attributes["name"].Value;
            StartTime = node.Attributes["st"].Value;
            // -> to String : DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") 
            // string strStart = MConv.convertTimeText(node.Attributes["st"].Value);
            // if (strStart != "") StartTime = Convert.ToDateTime(strStart);
            StepName = node.Attributes["step"].Value;
            
        }


        public void doConnect() {            

            try {
                Connect(new IPEndPoint(IPAddress.Parse(IP), Port));                
            }
            catch (Exception ex) {
                Console.WriteLine("catch {1}: {0}", ex.Message,
                    System.Reflection.MethodBase.GetCurrentMethod().Name);
            }

           
        }

        public void done_Connected(object sender, EventArgs e) {            
            IsConnected = true;
            // Send Identify 
            _session.processSend((int)MSG_TYPE.DEST_DAEMON, "<star-p client=\"0\"><identify me=\"AP\"/>/star-p>");
            // Send Timer            
        }

        public void done_DataReceived(object sender, DataEventArgs e) {
            Console.WriteLine($"{System.Reflection.MethodBase.GetCurrentMethod().Name} " +
                $"ofs={e.Offset} len={e.Length}");
            _session.processRecv(e.Data, e.Length);
        }

        public void done_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e) {
            Close();
        }
        public void done_Closed(object sender, EventArgs e) {
            IsConnected = false;
            
        }
    }
}
