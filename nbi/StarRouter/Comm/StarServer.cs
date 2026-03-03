using StarRouter.ViewModel;
using SuperSocket.SocketBase;
using SuperSocket.SocketBase.Config;
using SuperSocket.SocketBase.Protocol;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition.Primitives;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace StarRouter.Comm {
    public class StarServer : AppServer<StarProduct, ProductPacket> {

        MainVM mainVM;
        public StarServer()
            : base(new DefaultReceiveFilterFactory<ProductFilter, ProductPacket>()) {
            
        }

        public void serverStart() {
            mainVM = (MainVM)App.Current.Services.GetService(typeof(MainVM));

            ServerConfig serverConfig = new ServerConfig {
                Ip = "Any",
                Port = 15267,
                MaxConnectionNumber = 8,
                Mode = SocketMode.Tcp,
                MaxRequestLength = 128 * 1024,
                ReceiveBufferSize = 128*1024,
                SendBufferSize = 128 * 1024
            };

            Setup(serverConfig);
            Start();

            NewRequestReceived += new RequestHandler<StarProduct, ProductPacket> (StarServer_NewRequestReceived);
            
        }

        private void StarServer_NewRequestReceived(StarProduct product, ProductPacket packetData) {
            //Console.WriteLine($"[RECV][PRODUCT] Len={packetData.Body.Length}");
            Task.Run(() => product.processPacket(packetData));
        }

        
        


        protected override void OnNewSessionConnected(StarProduct product) {
            //세션으로부터 받을 메시지용 이벤트
            product.IP = product.RemoteEndPoint.Address.ToString();
            product.Port = product.RemoteEndPoint.Port;

            int newIndex = 1;
            while (true) {
                if (mainVM.Products.FirstOrDefault(e => e.Index == newIndex) == null)
                {
                    product.Index = newIndex;
                    break;
                }
                newIndex++;
            }

            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, 
                new Action(() => mainVM.Products.Add(product)
            ));

            base.OnNewSessionConnected(product);

            product.sendCMList();
            product.sendPLCList();

        }

        protected override void OnSessionClosed(StarProduct product, CloseReason reason) {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                new Action(() => mainVM.Products.Remove(product)
            ));
            base.OnSessionClosed(product, reason);
        }

               


    }
}
