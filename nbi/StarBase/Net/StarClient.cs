using System;
using System.Collections.Generic;
using System.Net;
using System.Threading.Tasks;
using SuperSocket.ClientEngine;
using System.Xml;
using System.IO;
using System.Windows.Threading;
using SuperSocket.ProtoBase;
using System.Linq;
using System.Collections.Concurrent;

namespace StarBase.Net {

    public class StarPacket : PackageInfo<string, byte[]> {
        public StarHeader header;
        public StarPacket(byte[] head, byte[] body) : base(null, body) {
            header = MConv.ByteToStruct<StarHeader>(StarProtocal.HEADER_SIZE, head);
        }
    }

    public class StarFilter : FixedHeaderReceiveFilter<StarPacket> {
        public StarFilter() : base(StarProtocal.HEADER_SIZE) {
        }

        protected override int GetBodyLengthFromHeader(IBufferStream bufferStream, int length)
        {

            int len = (int)bufferStream.Length;

            try
            {
                if (len < StarProtocal.HEADER_SIZE)
                    throw new Exception("Failed to extract length from header segments.");

                byte[] buf = new byte[16];
                bufferStream.Read(buf, 0, 16);

                int magic = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(buf, 0));
                if (magic != StarProtocal.HDR_MAGIC) return -1;

                int bodyLen = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(buf, 8));

                if (bodyLen <= 0 || bodyLen > (256 * 1024)) return -1; // 패킷 폐기/연결 종료 신호

                return bodyLen;
            }
            catch (Exception ex)
            {
                return -1;
            }
        }

        public override StarPacket ResolvePackage(IBufferStream bufferStream)
        {

            try
            {
                int len = (int)bufferStream.Length;
                if (len < StarProtocal.HEADER_SIZE)
                {
                    return null; // 🔥 SuperSocket이 패킷을 무시하도록 함
                }

                var headerBytes = new byte[StarProtocal.HEADER_SIZE];
                bufferStream.Read(headerBytes, 0, StarProtocal.HEADER_SIZE);
                int bodyLen = IPAddress.NetworkToHostOrder(BitConverter.ToInt32(headerBytes, 8));



                if (bodyLen <= 0 || bufferStream.Length != StarProtocal.HEADER_SIZE + bodyLen)
                //if (bodyLen < 0 || bufferStream.Length < bodyLen)
                {
                    return null; // 🔥 SuperSocket이 패킷을 무시하도록 함
                }

                var bodyBytes = new byte[bodyLen];
                bufferStream.Read(bodyBytes, 0, bodyLen);

                var pack = new StarPacket(headerBytes, bodyBytes);

                pack.header.Magic = IPAddress.NetworkToHostOrder(pack.header.Magic);
                pack.header.Mode = IPAddress.NetworkToHostOrder(pack.header.Mode);
                pack.header.Len = IPAddress.NetworkToHostOrder(pack.header.Len);
                pack.header.OrgLen = IPAddress.NetworkToHostOrder(pack.header.OrgLen);

                return pack;
            }
            catch (Exception ex)
            {
                return null;
            }
        }
    }

    public class SendFileProcess
    {
        public DispatcherTimer timer = new DispatcherTimer();
        public int timerCount = 5; // 5S
        public int timerProgress = 0;
//        public EventHandler tick;

        public bool isRunning = false;
        public delegate void TimeOver();

        public string StarPath = "";
        public string FileName = "";
        public int offset;
        public int remind;
        public int length;
        public int progress;
        public byte [] buffer;
        public StarClient Client;

        public event TimeOver timeOver;

        public SendFileProcess()
        {
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += timer_tick;
        }

        
        public void start(string id, string target, string name)
        {
            StarPath = id;
            FileName = target;

            FileStream fsr;
            BinaryReader br;
            try
            {
                fsr = new FileStream(name, FileMode.Open, FileAccess.Read);
                br = new BinaryReader(fsr);
            }catch 
            {                
                return;
            }
            


            progress = 0;
            offset = 0;
            length = (int)fsr.Length;
            remind = length;
            buffer = br.ReadBytes(length);

            fsr.Close();
            br.Close();
            
            
            timerProgress = 0;
            isRunning = true;
            timer.Start();
            next();
     
            

        }

        public void timer_tick(object sender, EventArgs e)
        {
            timerProgress++;
            Console.WriteLine("timer_tick {0}", timerProgress);

            if (timerCount < timerProgress) 
            { 
                stop();                
                if (timeOver != null)
                    timeOver();
            }
        }

        public void stop()
        {
            timer.Stop();
            isRunning = false;
        }

        public void resetTickCount()
        {            
            timerProgress = 0;                                    
        }

        public int next()
        {
            resetTickCount();
            int sendByte = (remind > (int)MSG_TYPE.CHUNK_SZ) ? (int)MSG_TYPE.CHUNK_SZ : remind;
            var temp = new byte[sendByte];
            Buffer.BlockCopy(buffer, offset, temp, 0, sendByte);

#pragma warning disable CS8602 // null 가능 참조에 대한 역참조입니다.
            Client.sendFile(StarPath, "", FileName, offset, sendByte, temp);
#pragma warning restore CS8602 // null 가능 참조에 대한 역참조입니다.

            offset += sendByte;
            remind -= sendByte;
            if (remind == 0 )stop();

            return remind;
        }
    }

    public class StarMessage
    {
        public MSG_TYPE type;
        public RESULT_CMD command;
        public XmlNode xmlNode;
        public string recvMsg = "";
        public byte[] buffer;
    }

    public enum PACKET_PROC : int
    {
        DONE = 0,
        MORE = 1,
        WAIT = 2
    }

    // QUE 사용 안할 경우
    public delegate void processResultFunc(RESULT_CMD type, XmlNode node);
    public delegate void processRecvFileFunc(string xmlTest);
    public delegate void processConenctFunc(bool isConect);

    public class StarClient : EasyClient {
        
        bool _bHeadDone = false;        

        //public StarHeader Header = new StarHeader();
        public bool _isConnect = false;

        // QUE 처리로 굳이 사용이 필요없음..
        public event processResultFunc deleFunc;
        public event processRecvFileFunc recvFileFunc;
        public event processConenctFunc conFunc;

        public object _lock = new object();
        //public Queue<StarMessage> _recvQueue = new Queue<StarMessage>();
        public ConcurrentQueue<StarMessage> _recvQueue = new ConcurrentQueue<StarMessage>();


        public StarClient()
        {
            Connected += done_Connected;            
            Error += done_Error;
            Closed += done_Closed;

            ReceiveBufferSize = 2 * 1024 * 1024;//(int)MSG_TYPE.CHUNK_SZ * 1024;

            //Header = new StarHeader();

            this.Initialize<StarPacket>(new StarFilter(), (packet) => processPacket(packet));
        }    
        ~StarClient()
        {           
        }

        public void pushRQ(StarMessage msg)
        {
            //lock (_lock)
            //{
                _recvQueue.Enqueue(msg);
            //}
        }

        public StarMessage popRQ()
        {
            StarMessage msg;            
            _recvQueue.TryDequeue(out msg);            
            return msg;
        }

        public int getRQSize()
        {
            return _recvQueue.Count;
        }



        public void processPacket(StarPacket pack)
        {
            if (pack == null) return;

            try
            {
                Console.WriteLine($"[PACKET HEADER] Mode: 0x{pack.header.Mode:X4}, Len: {pack.header.Len}");

                if (pack.Body != null && pack.Body.Length > 0)
                {
                    // Encoding.Default를 사용해 바이트를 문자열로 변환
                    string debugBody = System.Text.Encoding.Default.GetString(pack.Body);

                    Console.WriteLine($"[PACKET BODY]: {debugBody}");
                }
                else
                {
                    Console.WriteLine("[PACKET BODY]: (Empty)");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[DEBUG ERROR] {ex.Message}");
            }

            MSG_TYPE msgType = (MSG_TYPE) (pack.header.Mode & 0xff00);
            string recv = MConv.ByteToString(pack.Body);
            if (msgType == MSG_TYPE.TYPE_REQFILE || msgType == MSG_TYPE.TYPE_REQDONE)
            {  //result 처리
                pushRQ(new StarMessage { type = msgType, command = RESULT_CMD.CMD_UNKNOWN, recvMsg = recv });
                //recvFileFunc(recv);
            }
            else
            {
                XmlDocument xml = new XmlDocument();

                try
                {
                    string recvCheck = recv.Substring(0, recv.IndexOf("</star-p>") + "</star-p>".Length);
                    xml.LoadXml(recvCheck);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("processPacket : XmlParsing :{0}", ex.Message);
                    return;
                }

                XmlNode node = xml.SelectSingleNode("/star-p");
                XmlNode nodeCmd = node?.SelectSingleNode("result");
                if (nodeCmd == null) return;
                string cmd = nodeCmd.Attributes["cmd"].Value.ToUpper();

                RESULT_CMD nCmdType = RESULT_CMD.CMD_UNKNOWN;

                if (cmd == "SEND-FILE") nCmdType = RESULT_CMD.CMD_SEND_FILE;
                else if (cmd == "CU-LIST-AUTO") nCmdType = RESULT_CMD.CMD_CU_LIST_AUTO;
                else if (cmd == "CU-LIST") nCmdType = RESULT_CMD.CMD_CU_LIST;
                else if (cmd == "VZONE-LIST") nCmdType = RESULT_CMD.CMD_VZONE_LIST;
                else if (cmd == "PLC-CONFIG") nCmdType = RESULT_CMD.CMD_PLC_CONFIG;
                else if (cmd == "VZONE-SET") nCmdType = RESULT_CMD.CMD_VZONE_SET;
                else if (cmd == "SLOT-STAT") nCmdType = RESULT_CMD.CMD_STATUS;
                else if (cmd == "PROCESS") nCmdType = RESULT_CMD.CMD_PROC;
                else if (cmd == "PGM") nCmdType = RESULT_CMD.CMD_PGM;
                else if (cmd == "DIAG") nCmdType = RESULT_CMD.CMD_DIAG;
                else if (cmd == "ROM") nCmdType = RESULT_CMD.CMD_ROM;
                else if (cmd == "CTRL-CM") nCmdType = RESULT_CMD.CMD_CM_CTRL;                
                else if (cmd == "REQ-FILE") nCmdType = RESULT_CMD.CMD_REQ_FILE;
                else
                {
                    Console.WriteLine("UnKnown Cmd : {0}", cmd);
                    return;
                }
                /// -- User Thread 
                pushRQ(new StarMessage { type = msgType, command = nCmdType, xmlNode = node });

                /// -- CLR Worker Thread
//                deleFunc(nCmdType, node);
            }
                        
        }



        public void done_Connected(object sender, EventArgs e)
        {
            _isConnect = true;
            Console.WriteLine("Connected\n");
            if (conFunc != null) conFunc(_isConnect);
        }

        public void done_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {            
            Close();
        }
        public void done_Closed(object sender, EventArgs e)
        {
            _isConnect = false;
            if (conFunc != null) conFunc(_isConnect);
        }

        public async void doConnect(string ip, int port)
        {
            try
            {
                await ConnectAsync(new IPEndPoint(IPAddress.Parse(ip), port));
            }
            catch (Exception ex)
            { 
                Console.WriteLine(ex.ToString()); 
            }
        }

        public async void send(int mode, byte[] payload, int len)
        {
            if (_isConnect == false) return;

            try
            {
                // 1. 헤더 정보 출력
                Console.Write($"[SEND FINAL] Mode: 0x{mode:X4}, Len: {len} ");

                // 2. 내용 출력 (바이트를 문자열로 바꿔서 보여줌)
                if (payload != null && payload.Length > 0)
                {
                    // 한글 깨짐 방지 등을 위해 Default 인코딩 사용
                    string msg = System.Text.Encoding.Default.GetString(payload);

                    // 내용이 너무 길면(파일 전송 등) 콘솔 도배되니까 200자만 찍기
                    if (msg.Length > 200)
                        Console.WriteLine($": {msg.Substring(0, 200)} ... (Truncated)");
                    else
                        Console.WriteLine($": {msg}");
                }
                else
                {
                    Console.WriteLine(": (No Body)");
                }
            }
            catch (Exception)
            {
                Console.WriteLine(": (Log Error)");
            }

            var header = MConv.StructToByte(
                    new StarHeader
                    {
                        Magic   = IPAddress.HostToNetworkOrder(StarProtocal.HDR_MAGIC),
                        Mode    = IPAddress.HostToNetworkOrder(mode),
                        Len     = IPAddress.HostToNetworkOrder(len),
                        OrgLen  = IPAddress.HostToNetworkOrder(len),
                    }
                );

            var packet = header;
            if (payload != null)
                packet = MConv.GetCombined(header, payload);

            Task t = Task.Run(() => Send(packet));
            await t;
        }

        public  void send(int mode, string payload, int len)
        {
            //            Console.WriteLine("SEND -> " + payload);
            var buf = MConv.StringToByte(payload);
            send(mode, buf, buf.Length);           
        }


        public bool sendCmd(int mode, string id, string cmd, string xml)
        {
            string buf = "";
            MSG_TYPE dest = (MSG_TYPE)(mode & 0xff);

            switch (dest)
            {
                case MSG_TYPE.DEST_ROUTER:
                    buf = string.Format("<star-p>{0}</star-p>", xml);
                    break;
                case MSG_TYPE.TYPE_RESULT:
                case MSG_TYPE.DEST_DAEMON:
                    buf = string.Format("<star-p path=\"{0}\">{1}</star-p>", id, xml);
                    break;
                case MSG_TYPE.DEST_PGM:
                    buf = string.Format("<star-p path=\"{0}\"><pgm cmd=\"{1}\">{2}</pgm></star-p>", id, cmd, xml);
                    break;
                case MSG_TYPE.DEST_DIAG:
                    buf = string.Format("<star-p path=\"{0}\"><diag cmd=\"{1}\">{2}</diag></star-p>", id, cmd, xml);
                    break;
                
            }

            send(mode, buf, buf.Length);

            return true;
        }


        public bool sendPgmStop(string star_path)
        {
            string xml = string.Format("<process cmd=\"kill\" id=\"pgm\" />");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }


        public bool sendPgmRun(string star_path, string name)
        {
            string xml = string.Format($"<process cmd=\"run\" id=\"pgm\" exec=\"{name}\" />");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendDiagRun(string star_path, string name)
        {
            string xml = string.Format($"<process cmd=\"run\" id=\"diag\" exec=\"{name}\" />");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendDiagStop(string star_path)
        {
            string xml = string.Format("<process cmd=\"kill\" id=\"diag\" />");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendAuthority(string star_path, string name)
        {
            string xml = string.Format($"<ctrl-cm cmd=\"sh sudo chmod 777 {name}\"/>");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendPGMRemove(string star_path)
        {
            string xml = string.Format("<ctrl-cm cmd=\"sh sudo rm /star/pgm/*\"/>");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendSlotStat(string star_path)
        {
            string xml = string.Format("<slot-stat/>");            
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendIdentify(string name)
        {
            string msg = string.Format("<identify me=\"{0}\" list=\"1\"/>", name);
            sendCmd((int)MSG_TYPE.DEST_ROUTER,  "", "", msg);
            return true;
        }

        public bool sendTempData(string pv, string st) 
        {
            string msg = string.Format("<temp-data pv=\"{0}\" st=\"{0}\"/>",  pv, st);
            sendCmd((int)MSG_TYPE.DEST_ROUTER, "", "", msg);
            return true;
        }


        public bool sendDelFile(string star_path, string source)
        {
            string xml = string.Format($"<del-file source=\"{source}\" />");
            return sendCmd((int)MSG_TYPE.DEST_DAEMON, star_path, "", xml);
        }

        public bool sendReqFile(string star_path, string source, string target)
        {
            string xml = string.Format($"<req-file source=\"{source}\" target=\"{target}\"/>");
            return sendCmd((int) MSG_TYPE.DEST_DAEMON | (int) MSG_TYPE.TYPE_REQFILE, star_path, "", xml);
        }


        public bool sendResponse(string star_path, string cmd, bool ret)
        {
            if (!IsConnected) return false;
            string xml = string.Format("<result seq=\"0\" cmd=\"{0}\" return=\"{1}\" type=\"xml\" />", cmd, (ret? "OK" : "FAIL"));

            return sendCmd((int)MSG_TYPE.TYPE_REQFILE, star_path, "", xml);
        }

        public bool sendFile(string star_path, string dir, string name, int ofs, int size, byte[] data)        
        {
            if (!IsConnected) return false;
//            string xml = string.Format("<send-file dir=\"{0}\" name=\"{1}\" ofs=\"{2}\" size=\"{3}\">{4}</send-file>", 
//                dir, name, ofs, size, MConv.ByteToString(data));
             string sXml = string.Format("<star-p path=\"{0}\"><send-file dir=\"{1}\" name=\"{2}\" ofs=\"{3}\" size=\"{4}\">",
                 star_path, dir, name, ofs, size);
            
            var buf = MConv.GetCombined(MConv.StringToByte(sXml), data);            
            buf = MConv.GetCombined(buf, MConv.StringToByte("</send-file></star-p>"));
            send((int)MSG_TYPE.DEST_DAEMON | (int)MSG_TYPE.TYPE_SENDFILE, buf, buf.Length);


            return true;
        }


    }
}
