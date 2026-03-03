using SuperSocket.ClientEngine;
using SuperSocket.ProtoBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace StarBase.Net
{
    
    public class HandlerPacket : PackageInfo<string, byte[]>
    {
        public HandlerHeader header;
        public byte[] datas;
        public HandlerPacket(byte[] head, byte[] body) : base(null, body)
        {
            header = MConv.ByteToStruct<HandlerHeader>(Marshal.SizeOf<HandlerHeader>(), head);
        }
    }


    public class HandlerFilter : FixedHeaderReceiveFilter<HandlerPacket>
    {
        public HandlerFilter() : base(HandlerProtocal.HEADER_SIZE)
        {
        }

        protected override int GetBodyLengthFromHeader(IBufferStream bufferStream, int length)
        {

            int len = (int)bufferStream.Length;           

            byte[] buf = new byte[len];
            try
            {
                bufferStream.Read(buf, 0, len);
                len = IPAddress.NetworkToHostOrder(BitConverter.ToInt16(buf, 2)) - HandlerProtocal.HEADER_SIZE;
            }
            catch (Exception ex)
            {
                Console.WriteLine("catch {1}: {0}", ex.Message,
                    System.Reflection.MethodBase.GetCurrentMethod().Name);
            }

            return len;
        }

        public override HandlerPacket ResolvePackage(IBufferStream bufferStream)
        {

            int len = (int)bufferStream.Length;
            byte[] buf = new byte[len];
            bufferStream.Read(buf, 0, len);
            HandlerPacket pack = new HandlerPacket(
                    buf.CloneRange(0, HandlerProtocal.HEADER_SIZE),
                    buf.CloneRange(HandlerProtocal.HEADER_SIZE, len - HandlerProtocal.HEADER_SIZE));

            /*
                        byte[] body = null;
                        if (len - Marshal.SizeOf<HandlerHeader>() > 0)
                        {
                            body = buf.Clone    Range(Marshal.SizeOf<HandlerHeader>(), len - Marshal.SizeOf<HandlerHeader>());
                        }


                        HandlerPacket pack = new HandlerPacket(
                            buf.CloneRange(0, Marshal.SizeOf<HandlerHeader>()),
                            body
                        );

                        pack.datas = new byte[len];
                                          
            */

            pack.datas = new byte[len];
            Array.Copy(buf, pack.datas, len);
            pack.header.Magic = IPAddress.NetworkToHostOrder(pack.header.Magic);
            pack.header.Len = IPAddress.NetworkToHostOrder(pack.header.Len);

            return pack;
        }

    }

    public delegate void processRecvHandler(byte type, byte[] datas);
    public delegate void processLogHandler(int type, string msg);

    public class HandlerClient : EasyClient
    {
        public bool _isConnect = false;
        public event processConenctFunc conFunc;
        public event processRecvHandler recvFunc;
        public event processLogHandler logFunc; 
        public HandlerClient()
        {
            Connected += done_Connected;
            Error += done_Error;
            Closed += done_Closed;

            ReceiveBufferSize = 1024;

            this.Initialize<HandlerPacket>(new HandlerFilter(), (p) => processPacket(p));

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

        public void processPacket(HandlerPacket pack)
        {

            byte[] body = null;

            try
            {
                if (pack.header.Len > 0x5)
                { // data exist            
                    body = pack.Body;
                }

                if (logFunc != null)
                {
                    StringBuilder sb = new StringBuilder();                
                    foreach (byte b in pack.datas)
                        sb.Append($"{b.ToString("X2")} ");
                    logFunc(0, sb.ToString());
                }

                if (recvFunc != null)                                    
                        recvFunc(pack.header.Type, body);                
            }
            catch (Exception ex)
            {
                logFunc(2, ex.ToString());
            }
        }

        public async void send(byte type, byte[] datas = null)
        {
            if (!_isConnect) return;
            int len = 5;
            if (datas != null) 
                len += datas.Length;
            var header = MConv.StructToByte(
                    new HandlerHeader
                    {
                        Magic = IPAddress.HostToNetworkOrder(HandlerProtocal.HDR_MAGIC),
                        Type = type,
                        Len = IPAddress.HostToNetworkOrder((short)len)
                    }
                );
            var packet = header;
            if (datas != null)
                packet = MConv.GetCombined(header, datas);

            if (logFunc != null)
            {
                StringBuilder sb = new StringBuilder();
                foreach (byte b in packet)
                    sb.Append($"{b.ToString("X2")} ");
                logFunc(1, sb.ToString());
            }

            Task t = Task.Run(() => Send(packet));
            await t;

        }
           
    }

}
