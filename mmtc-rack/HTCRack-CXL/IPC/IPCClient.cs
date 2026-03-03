using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Markup;
using System.Windows.Media;
using System.Xml.Linq;

namespace HTCRack_CXL.IPC
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
    public delegate void NotiRecv(int rack, int box, float[] sv);

    public class IPCClient
    {
        public NamedPipeClientStream clientPipeStream;
        PipeStream pipeStream;

        public NotiRecv notiRecv = null;
        public NotiCon notiCon = null;


        public object _lock = new object();
        public Queue<IPCMessage> _sendQueue = new Queue<IPCMessage>();
        public Thread _sendThread;
        public bool _quitSend = false;

        public IPCClient()
        {

        }

        public void startThread()
        {
            _quitSend = false;
            _sendThread = new Thread(FuncSendQue);
            _sendThread.Start(this);
        }

        public void stopThread()
        {
            if (_sendThread == null) return;

            _quitSend = true;
            _sendThread.Join();
            _sendThread = null;
        }
        public static void FuncSendQue(object obj)
        {
            IPCClient client = obj as IPCClient;
            while (!client._quitSend)
            {
                if (client.clientPipeStream == null)
                {
                    client.connect();

                }
                else if (client._sendQueue.Count > 0)
                {
                    var msg = client.popSenQue();
                    client.writeByte(msg.RackNo, msg.BoxNo, msg.Temps);

                }
                else
                    Thread.Sleep(10);

            }

        }

        public void pushSendQue(IPCMessage msg)
        {
            lock (_lock) { _sendQueue.Enqueue(msg); }
        }

        public IPCMessage popSenQue()
        {
            IPCMessage msg;
            lock (_lock)
            {
                msg = _sendQueue.Dequeue();
            }
            return msg;
        }

        public void start()
        {
            startThread();
        }

        public void stop()
        {
            close();
            stopThread();
        }

        public void connect()
        {
            clientPipeStream = new NamedPipeClientStream(".", "star-pipe",
                PipeDirection.InOut,
                PipeOptions.Asynchronous);
            pipeStream = clientPipeStream;

            try
            {
                clientPipeStream.Connect(500);
#if TEST_MODE
                Console.WriteLine("IPC Connected!!");
#endif
                if (notiCon != null) notiCon(true);
                startReadAsync();
            }
            catch (Exception ex)
            {
#if TEST_MODE
                Console.WriteLine(ex.ToString());
#endif
                close();
            }

        }

        public void close()
        {
            if (clientPipeStream == null) goto Done;
            if (clientPipeStream.IsConnected)
                clientPipeStream.WaitForPipeDrain();
            clientPipeStream.Close();
            clientPipeStream.Dispose();
            clientPipeStream = null;

        Done:
            if (notiCon != null)
                notiCon(false);
        }

        public void reset()
        {
#if TEST_MODE
            Console.WriteLine("Reset IPC");
#endif
            close();

        }

        public byte[] StructToByte(object obj)
        {
            int size = Marshal.SizeOf(obj);
            var returnArray = new byte[size];

            var ptr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(obj, ptr, true);
            Marshal.Copy(ptr, returnArray, 0, size);

            Marshal.FreeHGlobal(ptr);

            return returnArray;
        }
        public static T ByteToStruct<T>(int size, byte[] buffer)
        {
            var ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, 0, ptr, size);

            T obj = (T)Marshal.PtrToStructure(ptr, typeof(T));

            Marshal.FreeHGlobal(ptr);

            return obj;
        }

        public byte[] GetCombined(byte[] A, byte[] B)
        {
            var temp = new byte[A.Length + B.Length];

            Buffer.BlockCopy(A, 0, temp, 0, A.Length);
            Buffer.BlockCopy(B, 0, temp, A.Length, B.Length);

            return temp;
        }

        public byte[] ByteToArray<T>(List<T> values)
        {
            int v = sizeof(float);
            int size = v;
            var buffer = new byte[size];
            return buffer;

        }

        public Task writeByte(int rackNo, int boxNo, float[] vals)
        {
            StarIPCHead head = new StarIPCHead();
            head.Idntify = (ushort)0xc0de;
            head.RackNo = rackNo;
            head.BoxNo = boxNo;
            head.DataLen = vals.Length * 4;

            byte[] headBytes = StructToByte(head);
            byte[] dataBytes = new byte[head.DataLen];
            for (int i = 0; i < vals.Length; i++)
            {
                byte[] val = System.BitConverter.GetBytes(vals[i]);
                Buffer.BlockCopy(val, 0, dataBytes, i * 4, 4);
            }
            byte[] packet = GetCombined(headBytes, dataBytes);

            Task ret = pipeStream.WriteAsync(packet, 0, packet.Length);
            return ret;
        }




        public void startReadAsync()
        {
            int headSZ = Marshal.SizeOf<StarIPCHead>();
            byte[] rbuf = new byte[headSZ];
            // Header Read

            pipeStream.ReadAsync(rbuf, 0, rbuf.Length).ContinueWith(t =>
            {
                if (t.Result == 0)
                {
                    reset();
                }
                else
                {

                    StarIPCHead head = ByteToStruct<StarIPCHead>(headSZ, rbuf);

                    // Body Read
                    byte[] dataBytes = new byte[head.DataLen];
                    pipeStream.ReadAsync(dataBytes, 0, head.DataLen).ContinueWith(t2 =>
                    {
                        if (t2.Result == 0)
                        {
                            reset();
                        }
                        else
                        {
                            // Call Recv 
                            float[] dataFloats = new float[head.DataLen / 4];
                            for (int i = 0; i < dataFloats.Length; i++)
                            {
                                dataFloats[i] = BitConverter.ToSingle(dataBytes, i * 4);
#if TEST_MODE
                                Console.WriteLine($"Box{head.BoxNo} {dataFloats[i]}");
#endif
                            }
                            if (notiRecv != null) notiRecv(head.RackNo, head.BoxNo, dataFloats);
                            startReadAsync();
                        }
                    });
                }

            });


        }


    }
}
