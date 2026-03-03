using SuperSocket.SocketBase.Protocol;
using SuperSocket.SocketBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StarBase.Net;
using StarBase;
using System.Net;
using System.Windows.Input;
using System.Windows.Markup;


namespace StarRouter.Comm {

    public delegate void DelageteSend(byte[] data, int offset,  int len);

    public enum RESULT : int 
    {
        OK = 1,
        FAIL = 0
    }

    public enum OWNER : int {
        CM = 0,
        PRODUCT =1
    }
    public class StarSender {

        public DelageteSend sendFunc;

        public OWNER Owner = OWNER.CM;

        public void processSend(int mode, byte[] data, int len) {
            var header = MConv.StructToByte(
            new StarHeader {
                Magic = IPAddress.HostToNetworkOrder(StarProtocal.HDR_MAGIC),
                Mode = IPAddress.HostToNetworkOrder(mode),
                Len = IPAddress.HostToNetworkOrder(len),
                OrgLen = IPAddress.HostToNetworkOrder(len),
            });

            string bodyMsg = "";
            if (data != null && data.Length > 0)
            {
                try
                {
                    // 1. 일단 전체를 문자열로 변환
                    bodyMsg = System.Text.Encoding.Default.GetString(data);

                    // 2. ★ 1000자가 넘으면 자르기 (Truncate)
                    if (bodyMsg.Length > 1000)
                    {
                        bodyMsg = bodyMsg.Substring(0, 1000) + " ... [Truncated]";
                    }
                }
                catch
                {
                    bodyMsg = "[Binary Data Decode Error]";
                }
            }
            else
            {
                bodyMsg = "[No Body]";
            }

            Console.WriteLine($"[SEND][{Owner.ToString()}] Mode:0x{mode:X4} Len:{len}");
            Console.WriteLine($"└─> Body: {bodyMsg}");
            Console.WriteLine("[/SEND]");

            byte[] packet = header;
            if (data != null)
                packet = MConv.GetCombined(header, data);
            sendFunc(packet, 0, packet.Length);
        }

        public void processSend(int mode, string data) 
        {
            var buf = MConv.StringToByte(data);
            processSend(mode, buf, buf.Length);
        }


        public void writeResult(int index, RESULT status, string cmd, string xml ="") {
            
            string rstr = status.ToString();
            string msg = $"<star-p client=\"{index}\" >" +
                $"<result cmd=\"{cmd}\" return=\"{status.ToString()}\" type=\"xml\">";
            msg += xml;
            msg += "</result></star-p>";
            processSend((int)MSG_TYPE.TYPE_RESULT, msg);
        }
    }
       
}
