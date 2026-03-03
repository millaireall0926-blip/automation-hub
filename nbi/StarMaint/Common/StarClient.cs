using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using SuperSocket.ClientEngine;
using System.Xml;
using System.Windows.Threading;
using System.IO;
using System.Windows;

namespace StarMaint.Common
{

    

    public class SendFileProcess
    {
        public DispatcherTimer timer = new DispatcherTimer();
        public int timerCount = 50; // 10S
        public int timerProgress = 0;
//        public EventHandler tick;

        public bool isRunning = false;
        public delegate void TimeOver();

        public string StarPath;
        public string FileName;
        public int offset;
        public int remind;
        public int length;
        public int progress;
        public byte [] buffer;
        public StarClient Client;

        public event TimeOver timeOver;

        
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
            }catch (FileNotFoundException e)
            {
                MessageBox.Show(e.Message);
                return;
            }
            


            progress = 0;
            offset = 0;
            length = (int)fsr.Length;
            remind = length;
            buffer = br.ReadBytes(length);

            fsr.Close();
            br.Close();
            
            timer.Interval = TimeSpan.FromTicks(1000*100); //100MS Period
            timer.Tick += timer_tick;
            timerProgress = 0;
            isRunning = true;
            timer.Start();
            next();
     
            

        }

        public void timer_tick(object sender, EventArgs e)
        {
            timerProgress++;
            if (timerCount <= timerProgress) 
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
            global::System.Buffer.BlockCopy(buffer, offset, temp, 0, sendByte);
            
            Client.sendFile(StarPath, "", FileName, offset, sendByte, temp);

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
        public string recvMsg;
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

    public class StarClient : AsyncTcpSession
    {
        private byte[] _buffer = new byte[8 * 1024 * 1024];
        private int _length = 0;
        private int _progress = 0;
        private int _orgSum = 0;

        bool _bHeadDone = false;        

        public StarHeader Header = new StarHeader();
        public bool _isConnect = false;

        
        public event processResultFunc deleFunc;
        public event processRecvFileFunc recvFileFunc;

        public object _lock = new object();
        public Queue<StarMessage> _msgQueue = new Queue<StarMessage>();


        public StarClient()
        {
            Connected += done_Connected;
            DataReceived += done_DataReceived;
            Error += done_Error;
            Closed += done_Closed;

            ReceiveBufferSize = 2 * 1024 * 1024;//(int)MSG_TYPE.CHUNK_SZ * 1024;

            Header = new StarHeader();
        }    
        ~StarClient()
        {           
        }

        public void push(StarMessage msg)
        {
            lock (_lock)
            {
                _msgQueue.Enqueue(msg);
            }
        }

        public StarMessage pop()
        {
            StarMessage msg;
            lock(_lock)
            {
                msg = _msgQueue.Dequeue();
            }
            return msg;
        }

        public int getQueSize()
        {
            return _msgQueue.Count;
        }

        public void put(byte[] data, int length)
        {
            lock (this._buffer)
            {
                global::System.Buffer.BlockCopy(data, 0, _buffer, _length, length);
                this._length += length;
            }
//            Console.WriteLine($"PUT : Prog={_progress} L={_length} Arg={length}");
        }

        public byte[] get(int size)
        {
            var temp = new byte[size];

            global::System.Buffer.BlockCopy(_buffer, _progress, temp, 0, size);
            this._progress += size;

  //          Console.WriteLine($"GET : Prog={_progress} L={_length} Org={size} OrgSum={_orgSum}");

            return temp;
        }
        public bool isValid(int size)
        {
//            Console.WriteLine($"{(_bHeadDone?"Body":"Head")} : {_progress}+{size} -> " +
//                $"POG:{_progress + size}, RECV:{_length} = {(_progress + size <= _length).ToString()}");
            if (_progress + size <= _length)
                return true;
            return false;
        }

        public void complete()
        {
            Array.Clear(_buffer, 0, _length);

            _progress = 0;
            _length = 0;            
            _bHeadDone = false;            
        }

        public bool readHeader()
        {
            if (isValid(StarProtocal.HEADER_SIZE))
            {
                Header = MConv.ByteToStruct<StarHeader>(StarProtocal.HEADER_SIZE, get(StarProtocal.HEADER_SIZE));                
                Header.Magic    = IPAddress.NetworkToHostOrder(Header.Magic);
                Header.Mode     = IPAddress.NetworkToHostOrder(Header.Mode);
                Header.Len      = IPAddress.NetworkToHostOrder(Header.Len);
                Header.OrgLen   = IPAddress.NetworkToHostOrder(Header.OrgLen);
                if (Header.Magic != StarProtocal.HDR_MAGIC)
                {
                    Close();
                    return false;
                }
            }
            _bHeadDone = true;
            return true;
        }

        public bool readBody()
        {
            if (!isValid(Header.OrgLen)) return false;

            var paload = get(Header.OrgLen);
            string recv = MConv.ByteToString(paload);//.TrimEnd('\0');
//            Console.WriteLine("RECV <- " + recv);
            MSG_TYPE msgType = (MSG_TYPE) (Header.Mode & 0xff00);
            if (msgType == MSG_TYPE.TYPE_REQFILE || msgType == MSG_TYPE.TYPE_REQDONE)
            {  //result 처리
                push(new StarMessage { type = msgType, command = RESULT_CMD.CMD_UNKNOWN, recvMsg = recv });
                //recvFileFunc(recv);
            }
            else
            {
                string recvCheck = recv.Substring(0, recv.IndexOf("</star-p>") + "</star-p>".Length);
                XmlDocument xml = new XmlDocument();
                xml.LoadXml(recvCheck);

                XmlNode node = xml.SelectSingleNode("/star-p");
                XmlNode nodeCmd = node.SelectSingleNode("result");
                if (nodeCmd == null) return true;
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
                else if (cmd == "CTRL-CM") nCmdType = RESULT_CMD.CMD_CM_CTRL;
                else if (cmd == "REQ-FILE") nCmdType = RESULT_CMD.CMD_REQ_FILE;
                else
                {
                    Console.WriteLine("UnKnown Cmd : {0}", cmd);
                    return true;
                }
                /// -- User Thread 
                push(new StarMessage { type = msgType, command = nCmdType, xmlNode = node });

                /// -- CLR Worker Thread
//                deleFunc(nCmdType, node);
            }
            

            return true;
        }

        
        public void done_DataReceived(object sender, DataEventArgs e)
        {
            put(e.Data, e.Length);
            //Console.WriteLine($"Recv : {e.Length}");
            try
            {
                while (true)
                {
                    
                    if (_bHeadDone)
                    {
                        
                         readBody();

                        PACKET_PROC Ret = PACKET_PROC.DONE;

                        if (_orgSum > _progress)        // Read More. Wait Packet
                            Ret = PACKET_PROC.WAIT;
                        else if (_orgSum == _progress)  // Read Done
                            Ret = PACKET_PROC.DONE;
                        else if (_orgSum < _progress)    // Packet Remind. Process More
                            Ret = PACKET_PROC.MORE;
              //          Console .WriteLine($"Judge : I={_progress} L={_length} ORG={_orgSum} diff { _length - _orgSum} RET={Ret.ToString()}");                        
                        
                        // Packet Multi로 들어온 경우
                        if (Ret == PACKET_PROC.DONE)
                        {
                            if (_progress < _length)
                            {
                                Array.Clear(_buffer, 0, _progress);
                                global::System.Buffer.BlockCopy(_buffer, _progress, _buffer, 0, _length - _progress);
                                _length -= _progress;
                                _progress = 0;

//                                Console.WriteLine($"Done -> More {_progress} {_length}");
                                Ret = PACKET_PROC.MORE;
                            }
                        }

                        if (Ret == PACKET_PROC.DONE) 
                        {
                            Array.Clear(_buffer, 0, _length);
                            _progress = 0;
                            _length = 0;
                            _orgSum = 0;
                            _bHeadDone = false;
                            break; 
                        }else if (Ret == PACKET_PROC.MORE) 
                        {
                            _orgSum = 0;  
                            _bHeadDone = false; 
                            continue; 
                        }
                        else if (Ret == PACKET_PROC.WAIT) { break; }
                    }
                    else
                    {
                        bool ret = readHeader();                       
                        _orgSum += (Header.OrgLen + StarProtocal.HEADER_SIZE);


                        if (ret == false)
                        {
                            Console.WriteLine("Packet Head Read Error. Clear State");
                            complete();
                            break;
                        }
                    }
                    
                }
            }
            catch (Exception ex)
            {
                Array.Clear(_buffer, 0, _length);
                _progress = 0;
                _length = 0;
                _orgSum = 0;
                _bHeadDone = false;
                Console.WriteLine("Net Error: {0}", ex.Message);
            }

        }

        public void done_Connected(object sender, EventArgs e)
        {
            _isConnect = true;
            Console.WriteLine("Connected\n");
        }

        public void done_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {            
            Close();
        }
        public void done_Closed(object sender, EventArgs e)
        {
            _isConnect = false;         
        }

        public void doConnect()
        {
            try
            {
                Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), StarProtocal.ROUTER_PORT));
            }catch (Exception ex)
            { 
                Console.WriteLine(ex.ToString()); 
            }
        }

        public void send(int mode, byte[] payload, int len)
        {
            if (_isConnect == false) return;
            int intMode = (int)mode;
            var header = MConv.StructToByte(
                    new StarHeader
                    {
                        Magic   = IPAddress.HostToNetworkOrder(StarProtocal.HDR_MAGIC),
                        Mode    = IPAddress.HostToNetworkOrder(intMode),
                        Len     = IPAddress.HostToNetworkOrder(len),
                        OrgLen  = IPAddress.HostToNetworkOrder(len),
                    }
                );

            var packet = header;
            if (payload != null)
                packet = MConv.GetCombined(header, payload);

            Send(packet, 0, packet.Length);
        }

        public void send(int mode, string payload, int len)
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

        public bool sendIdentify(string name)
        {
            string msg = string.Format("<identify me=\"{0}\" list=\"1\"/>", name);
            sendCmd((int)MSG_TYPE.DEST_ROUTER,  "", "", msg);
            return true;
        }

        public bool sendReqFile(string star_path, string source, string target)
        {	        
	        if (!IsConnected) return false;
	        string xml = string.Format("<req-file source=\"{0}\" target=\"{1}\"/>",  source, target);                
	        return sendCmd((int)MSG_TYPE.DEST_DAEMON | (int)MSG_TYPE.TYPE_REQFILE, star_path, "", xml);
        }

        public bool sendResponse(string star_path, string cmd, bool ret)
        {
            if (!IsConnected) return false;
            string xml = string.Format("<result seq=\"0\" cmd=\"{0}\" return=\"{1}\" type=\"xml\" />", cmd, (ret? "OK" : "FAIL"));

            return sendCmd((int)MSG_TYPE.TYPE_RESULT, star_path, "", xml);
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
