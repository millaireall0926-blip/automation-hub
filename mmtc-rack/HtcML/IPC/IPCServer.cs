using HtcML.Base;
using OneOf.Types;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace HtcML.IPC
{

    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct StarIPCHead
    {
        public ushort Idntify;
        public int RackNo;
        public int BoxNo;
        public int DataLen;
    }

    public struct IPCMessage
    {
        public int RackNo;
        public int BoxNo;
        public float[] Temps;
    }


    public delegate void NotiCon(bool isCon);

    public class IPCServer
    {
       
        public NamedPipeServerStream serverPipeStream;
        public bool _isQuit = false;

        public object _lock = new object();
        public Queue<IPCMessage> _recvQueue = new Queue<IPCMessage>();

        public NotiCon notifyCon = null;
        

        public IPCServer() {
                                         
        }

        public void startServer()
        {
            serverPipeStream = new NamedPipeServerStream("star-pipe",
                PipeDirection.InOut,
                NamedPipeServerStream.MaxAllowedServerInstances,
                PipeTransmissionMode.Message,
                PipeOptions.Asynchronous);
            
            serverPipeStream.BeginWaitForConnection(new AsyncCallback(PipeConnected), null);
        }

        public void stopServer()
        {
            if (serverPipeStream == null) return;
            if (serverPipeStream.IsConnected)
                serverPipeStream.WaitForPipeDrain();
            serverPipeStream.Close();
            serverPipeStream.Dispose();
            serverPipeStream = null;

            if (notifyCon != null)
                notifyCon(false);
            Console.WriteLine("DisConnect");
        }



        public void PipeConnected(IAsyncResult ar)
        {
            if (serverPipeStream == null) return;
            serverPipeStream.EndWaitForConnection(ar);
            startReadAsync();
            if (notifyCon != null)
                notifyCon(true);
            Console.WriteLine("Connected");
        }

        public void resetServer()
        {
            Console.WriteLine("Disconnected Reset");
            stopServer();
            startServer();
        }

        public void startReadAsync()
        {
            int headSZ = Marshal.SizeOf<StarIPCHead>();
            byte[] rbuf = new byte[headSZ];
            // Header Read
            serverPipeStream.ReadAsync(rbuf, 0, rbuf.Length).ContinueWith(t =>
            {
                if (t.Result == 0)
                {
                    resetServer();
                }
                else
                {

                    StarIPCHead head = MConv.ByteToStruct<StarIPCHead>(headSZ, rbuf);
                    Console.WriteLine($"head Rack{head.RackNo} Box{head.BoxNo} len {head.DataLen}");


                    // Body Read
                    byte[] dataBytes = new byte[head.DataLen];
                    serverPipeStream.ReadAsync(dataBytes, 0, head.DataLen).ContinueWith(t2 =>
                    {
                        if (t2.Result == 0)
                        {
                            resetServer();
                        }
                        else
                        {
                            float[] dataFloats = new float[head.DataLen / 4];
                            for (int i = 0; i < dataFloats.Length; i++)
                            {
                                dataFloats[i] = BitConverter.ToSingle(dataBytes, i * 4);
                                Console.WriteLine($"recv{i} {dataFloats[i]}");
                            }
                            IPCMessage msg = new IPCMessage();
                            msg.RackNo = head.RackNo;
                            msg.BoxNo = head.BoxNo;
                            msg.Temps = dataFloats;
                            pushRecvQue(msg);
                            startReadAsync();
                        }
                    });
                }

            });
        }



        public Task writeByte(int rackNo, int boxNo, float[] vals)
        {
            StarIPCHead head = new StarIPCHead();
            head.Idntify = (ushort)0xc0de;
            head.RackNo = rackNo;
            head.BoxNo = boxNo;
            head.DataLen = vals.Length * 4;

            byte[] headBytes = MConv.StructToByte(head);
            byte[] dataBytes = new byte[head.DataLen];
            for (int i = 0; i < vals.Length; i++)
            {
                byte[] val = System.BitConverter.GetBytes(vals[i]);
                Buffer.BlockCopy(val, 0, dataBytes, i * 4, 4);
            }
            byte[] packet = MConv.GetCombined(headBytes, dataBytes);

            return serverPipeStream.WriteAsync(packet, 0, packet.Length);
        }

        public int getQueSize()
        {
            return _recvQueue.Count();
        }


        public void pushRecvQue(IPCMessage msg)        {
            lock(_lock)
            {
                _recvQueue.Enqueue(msg);
            }
        }

        public IPCMessage popRecvQue()
        {
            IPCMessage ret;
            lock(_lock)
            {
                ret = _recvQueue.Dequeue();
            }
            return ret;
        }


    }
}
