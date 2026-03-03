using HTCRack.Base;
using HTCRack.Model;
using HTCRack.ViewModel;
using SuperSocket.ClientEngine;
using SuperSocket.ProtoBase;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;
using System.Runtime.Serialization.Formatters;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Threading;
using System.Xml;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace HTCRack.Net
{
    
    public class HtcPacket : PackageInfo<string, byte[]>
    {
        public HtcHeader Header;
        public HtcPacket(byte[] head, byte[] body) :base(null, body)
        {
            Header = MConv.ByteToStruct<HtcHeader>((int)PROTOCAL.HEAD_SIZE, head);
        }
    }

    public class HtcFilter : FixedHeaderReceiveFilter<HtcPacket>
    {
        public HtcFilter() : base((int)PROTOCAL.HEAD_SIZE) { }

        byte[] HeadBuf = new byte[(int)PROTOCAL.HEAD_SIZE];

        protected override int GetBodyLengthFromHeader(IBufferStream bufferStream, int length)
        {

            int len = (int)bufferStream.Length;
            byte[] buf = new byte[len];
            try
            {
                bufferStream.Read(buf, 0, (int)bufferStream.Length);                
                len = (int)buf[2] | (int) buf[3] << 8;  //(int)BitConverter.ToChar(buf, 2);
                if (len == 0)                
                    HeadBuf = buf.CloneRange(0, (int)bufferStream.Length);
                
            }
            catch (Exception ex)
            {
                Console.WriteLine("catch {1}: {0}", ex.Message,
                    System.Reflection.MethodBase.GetCurrentMethod().Name);
            }

            return len;
        }

        public override HtcPacket ResolvePackage(IBufferStream bufferStream)
        {
            int len = (int)bufferStream.Length;
            byte[] buf = new byte[len];
            int headLen = (int)PROTOCAL.HEAD_SIZE;
            
            if (len == headLen)
                buf = HeadBuf.CloneRange(0, (int)headLen);
            else 
                bufferStream.Read(buf, 0, (int)bufferStream.Length);
            
            HtcPacket pack;
            if (len - headLen > 0)
                pack = new HtcPacket(
                    buf.CloneRange(0, headLen),
                    buf.CloneRange(headLen, len - headLen));
            else            
                pack = new HtcPacket(
                   buf.CloneRange(0, headLen), null);
            
            return pack;
        }

    }

    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class HtcMessage
    {
        public HtcHeader Header;
        public HtcSub sub;
        public byte[] data;
    }
    
    public delegate void DGRecv(HtcMessage msg);
    public delegate void DGConenct(bool isConect);

    public class HtcClient : EasyClient
    {
        public int _index;
        public string _ip;
        public int _port;

        public bool _isConnect = false;
        public DGRecv funcRecv = null;
        public DGConenct funcConect = null;

        public bool _isCloseSend = false;
        public Thread _sendThread = new Thread(SendFunction);
        public DispatcherTimer _recvTimer = new DispatcherTimer();
        int _recvCnt = 0;
        public int _ackWaitCount = 0;
        public DateTime prevPDOTime = DateTime.Now;

        public bool _isUploading = false;
        public bool _isPushing = false;


        int _timerLimit = 12; 
        public void timer_func(object sender, EventArgs e)
        {
            if (!_isConnect) return;

            if (_recvCnt > _timerLimit )
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"FIX{_index+1} No RecvData. Try Reconnect!");
                _recvCnt = 0;
                Close();
                done_Closed(null, null);
            }
            _recvCnt++;
        }

        public void quit()
        {
    
            _isCloseSend = true;
            _recvTimer.Stop();
            _sendThread.Join();
        }
        static public void SendFunction(object parent)
        {
            HtcClient client = parent as HtcClient;

            while (!client._isCloseSend)
            {

                if (!client._isConnect)
                {
                    Thread.Sleep(1000);
                    continue;
                }

                if (client._sendQueue.Count == 0)
                {
                    if (!client._isUploading)
                        Thread.Sleep(100);
                }
                else
                {
                    byte[] packet = client.popSendQue();
                    client.Send(packet);

                    if (!client._isUploading) Thread.Sleep(10);
                } 
                

            }
        }
        public object _lock = new object();
        public Queue<byte[] > _sendQueue = new Queue<byte[] >();

        public void pushSendQue(REG_TYPE type, HtcMessage msg)
        {
            if (!_isConnect) return;

            int code = 0xc0de;
            int len = 5;
            if (msg.data != null)
            {
                msg.sub.Len = (short)msg.data.Length;
                len += msg.data.Length;
            }
            // if (msg.data == null, sub.len != 0) REQ Byte Data!

            msg.Header = new HtcHeader
            {
                Magic = (short)code,//IPAddress.HostToNetworkOrder((short)code),
                Len =  (short)len,
                Type = (byte)type,
                Res = 0
            };

            


            var packet = MConv.GetCombined(MConv.StructToByte(msg.Header), MConv.StructToByte(msg.sub));
            if (msg.data != null)
                packet = MConv.GetCombined(packet, msg.data);

            if (type != REG_TYPE.PDO)
            {
                if (Global.gMainVM.ShowEngineer == Visibility.Visible && Global.gEnginerVM.CurRack == _index)
                {                    
                    StringBuilder sb = new StringBuilder();                    
                    sb.Append($"T:{msg.Header.Type.ToString("X2")} L:{len.ToString("X2")} -> ");
                    sb.Append($"{msg.sub.Chanel.ToString("X2")} " +
                        $"{msg.sub.Adr.ToString("X2")} " +
                        $"{msg.sub.Func.ToString("X2")} " +
                        $"{msg.sub.Len.ToString("X2")} : ");
                    if (msg.data != null)
                        Array.ForEach(msg.data, num => sb.Append(num.ToString("X2") + ", "));                    
                    Global.gEnginerVM.pushCmdLog(new CommandLog(_index, CMD_TYPE.SEND, sb.ToString()));
                }
            }


            lock (_lock)
            {
                _sendQueue.Enqueue(packet);
            }
        }

        public byte[] popSendQue()
        {
            byte[] data;
            lock (_lock)
            {
                data = _sendQueue.Dequeue();
            }
            return data;
        }

        public HtcClient() {
            Connected += done_Connected;
            Error += done_Error;
            Closed += done_Closed;

            ReceiveBufferSize = 64 * 1024;

            Initialize<HtcPacket>(new HtcFilter(), (packet) => processPacket(packet));

            _sendThread.Start(this);

            _recvTimer.Tick += timer_func;
            _recvTimer.Interval = TimeSpan.FromSeconds(1);
            _recvTimer.Start();


        }

        public void processPacket(HtcPacket pack)
        {
   //         Console.WriteLine($"<- M:{pack.Header.Magic.ToString("X2")} len:{pack.Header.Len.ToString("X2")}" +
   //             $" T:{pack.Header.Type.ToString("X2")} R:{pack.Header.Res.ToString("X2")}");
            
            HtcMessage msg = new HtcMessage();
            msg.Header = MConv.ObjectDeepCopy(pack.Header);
            int code = (int)PROTOCAL.MAGIC_VAL;
            if (msg.Header.Magic != (short)code)
            {
                Console.WriteLine("Header Magic Error");
                Close();
                return;
            }

            if (pack.Body != null)
            {
                byte[] read = pack.Body;
       //         Array.ForEach(read, num => Console.Write(num.ToString("X2") + ", "));
       //         Console.WriteLine("");
       //         Console.WriteLine($"dataLen : {read.Length}");

                if (read.Length > 0)
                {
                    msg.sub = MConv.ByteToStruct<HtcSub>((int)PROTOCAL.SUB_SIZE, read);                    
                    msg.data = new byte[msg.sub.Len];
                    msg.data = read.CloneRange((int)PROTOCAL.SUB_SIZE, msg.sub.Len);
                    
                }
            }
            
            if (funcRecv != null)            
                funcRecv(msg);

           // Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"recv : {_index} cnt={_recvCnt}");
            _recvCnt = 0;
        }

        public void done_Connected(object sender, EventArgs e)
        {
            _isConnect = true;
            _recvCnt = 0;
            if (funcConect != null) funcConect(_isConnect);
            Console.WriteLine("Connected\n");
        }

        public void done_Error(object sender, SuperSocket.ClientEngine.ErrorEventArgs e)
        {
            Close();
        }
        public void done_Closed(object sender, EventArgs e)
        {
            _isConnect = false;
            Console.WriteLine("Connection Closed");
            if (funcConect != null) funcConect(_isConnect);
        }        

        public async void doConnect()
        {
            try
            {
                await ConnectAsync(new IPEndPoint(IPAddress.Parse(_ip), _port));
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }



    }


}
