using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.ViewModel;
using Microsoft.Win32;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LP_MMTCRack.Net
{
    public class CaptureCom
    {
        public int CaputureCh { get; set; } = 0;
        public string CaputurePortName { get; set; } = string.Empty;

        public string UsbHWPort {  get; set; } = string.Empty;
        public string UsbCaptureNameing { get; set; } = string.Empty;
        public bool CaputureUse { get; set; } = false;
        public double[] i2c_temperatureResult = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        public SerialPort CapturePort;

        public CaptureCom()
        {
            Global.gCaptureCom = this;
        }

        /*
        public async void CaptureConnect(int ch)
        {
            try
            {
                foreach (BoxModel bm in Global.gMainPageVM.Racks[CaputureCh].Boxes)
                {
                    bm.BoxPV = 0;
                    bm.BoxPVColor = Global.gWarningColors[0];
                    bm.BoxPVText = string.Empty;
                }

                if (CapturePort != null)
                {
                    CapturePort.Close();
                    CapturePort.Dispose();
                    Thread.Sleep(100);
                }

                CaputureCh = ch;
                if (CaputurePortName == string.Empty)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CapturePort {CaputureCh + 1} Com Port Name Emtpy");
                    return;
                }
                //bool MatchDev = false;
                //Task.Run(() => { MatchDev = MConv.GetComPortDetails(CaputurePortName, UsbCaptureNameing); });

                var MatchDev = await Task.Run(() => MConv.GetComPortDetails(CaputurePortName, UsbCaptureNameing , UsbHWPort));

                if (MatchDev.Item1)
                {
                    CapturePort = new SerialPort();
                    //CapturePort.PortName = CaputurePortName;
                    if (CaputurePortName != MatchDev.Item2)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"CapturePort {CaputureCh + 1} {CaputurePortName} -> {MatchDev.Item2} Com Port Chage ");
                        Global.gConfigVM.CapPortName[ch] = MatchDev.Item2;

                        Global.gConfigVM.saveitems();
                    }

                    CapturePort.PortName = CaputurePortName;
                    CapturePort.BaudRate = 9600;
                    CapturePort.DataBits = 8;
                    CapturePort.Parity = Parity.None;
                    CapturePort.StopBits = StopBits.One;
                    CapturePort.Open();

                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"CapturePort {CaputureCh + 1} Usb Info Match Connect");
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"CapturePort {CaputureCh + 1} Usb Info Mismatch. Check Com Port");
            }
            catch(Exception ex)
            {
                CapturePort.Close();
                CapturePort.Dispose();
                string msg = "[CapturePort" + (CaputureCh + 1) + "exception] " + ex.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }
        */

        public void CaptureConnect(int ch)
        {
            try
            {
                foreach (BoxModel bm in Global.gMainPageVM.Racks[CaputureCh].Boxes)
                {
                    bm.BoxPV = 0;
                    bm.BoxPVColor = Global.gWarningColors[0];
                    bm.BoxPVText = string.Empty;
                }

                if (CapturePort != null)
                {
                    CapturePort.Close();
                    CapturePort.Dispose();
                    Thread.Sleep(100);
                }

                CaputureCh = ch;

                if (CaputurePortName == string.Empty)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CapturePort {CaputureCh + 1} Com Port Name Emtpy");
                    return;
                }

                CapturePort = new SerialPort();


                CapturePort.PortName = CaputurePortName;
                CapturePort.BaudRate = 9600;
                CapturePort.DataBits = 8;
                CapturePort.Parity = Parity.None;
                CapturePort.StopBits = StopBits.One;
                CapturePort.Open();

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"CapturePort {CaputureCh + 1} Usb Info Match Connect");

                /*
                //var MatchDev = MConv.GetComPortDetails(UsbCaptureNameing, UsbHWPort);
                var CheckPortStatus = MConv.CheckPortAlive(CaputurePortName, "USB\\VID_10C4&PID_EA60\\" + UsbHWPort);

                
                if (CheckPortStatus.Item1)
                {
                    if (CaputurePortName != CheckPortStatus.Item2)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {CaputureCh + 1} {CaputurePortName} -> {CheckPortStatus.Item2} Com Port Chage ");
                        Global.gConfigVM.CapPortName[ch] = CheckPortStatus.Item2;

                        Global.gConfigVM.saveitems();
                    }
                    CapturePort.PortName = CaputurePortName;
                    CapturePort.BaudRate = 9600;
                    CapturePort.DataBits = 8;
                    CapturePort.Parity = Parity.None;
                    CapturePort.StopBits = StopBits.One;
                    CapturePort.Open();

                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"CapturePort {CaputureCh + 1} Usb Info Match Connect");
                }
                else
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"CapturePort {CaputureCh + 1} Usb Info Mismatch. Check Com Port");
                */
                /*
                CapturePort.PortName = CaputurePortName;
                CapturePort.BaudRate = 9600;
                CapturePort.DataBits = 8;
                CapturePort.Parity = Parity.None;
                CapturePort.StopBits = StopBits.One;
                CapturePort.Open();

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "CapturePort " + (CaputureCh + 1) + " Connect");
                */
            }
            catch (Exception ex)
            {
                CapturePort.Close();
                CapturePort.Dispose();
                string msg = "[CapturePort" + (CaputureCh + 1) + "exception] " + ex.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }

        public void CaptureDisConnect()
        {
            if (CapturePort != null)
            {
                foreach (BoxModel bm in Global.gMainPageVM.Racks[CaputureCh].Boxes)
                {
                    bm.BoxPV = 0;
                    bm.BoxPVColor = Global.gWarningColors[0];
                    bm.BoxPVText = string.Empty;
                }

                CapturePort.Close();
                CapturePort.Dispose();
            }
        }
        //public byte[] outData = new byte[3]; // 한번에 Read 하는 데이터에 따라 변경
        public byte[] outData = new byte[12];
        public async void CaptureWrite()
        {
            string RecvData = string.Empty;
            byte[] data = new byte[1];

            data[0] = (byte)0x13;

            if (!CapturePort.IsOpen)
                return;

            try
            {
                CapturePort.Write(data, 0, data.Length);
            }
            catch
            { }
            //Console.WriteLine($"Rack{CaputureCh + 1} SEND : {data[0]}");
            await Task.Delay(300);

            RackModel rm = Global.gMainPageVM.getRack(CaputureCh);

            Array.Clear(outData, 0, outData.Length);
            readCaptureData();

            Console.WriteLine("Temp Read: " + BitConverter.ToString(outData));

            if (Global.gMainPageVM.AllBoxCnt == 3)
            {

                if (outData[0] == 0x01)
                {
                    rm.Boxes[0].preTempData();
                    rm.Boxes[0].GetBoxPV(ReadDimmTemperature(outData[1], outData[2]));
                }
                if (outData[4] == 0x03)
                {
                    rm.Boxes[2].preTempData();
                    rm.Boxes[2].GetBoxPV(ReadDimmTemperature(outData[5], outData[6]));
                }
                if (outData[8] == 0x02)
                {
                    rm.Boxes[1].preTempData();
                    rm.Boxes[1].GetBoxPV(ReadDimmTemperature(outData[9], outData[10]));
                }
            }
            else if(Global.gMainPageVM.AllBoxCnt == 2)
            {
                if (outData[0] == 0x01)
                {
                    rm.Boxes[0].preTempData();
                    rm.Boxes[0].GetBoxPV(ReadDimmTemperature(outData[1], outData[2]));
                }
                if (outData[8] == 0x02)
                {
                    rm.Boxes[1].preTempData();
                    rm.Boxes[1].GetBoxPV(ReadDimmTemperature(outData[9], outData[10]));
                }
            }

        }
        public void readCaptureData()
        {
            int rxLen = 0;
            int count = 0;

            if (!CapturePort.IsOpen)
                return;

            try
            {
                while (true)
                {
                    if (CapturePort.IsOpen)
                        rxLen = CapturePort.BytesToRead;

                    if (rxLen > 0) break;

                    count++;

                    if (count > 50) break;
                }
                if (11 < rxLen)
                    rxLen = 12;

                if (CapturePort.IsOpen)
                    CapturePort.Read(outData, 0, rxLen);
            }
            catch { }


        }
        public double ReadDimmTemperature(byte data_1, byte data_2)
        {
            double temp = 0;

            if ((data_1 & 0x10) != 0)
            {
                // minus
                int code = (data_1 << 8) | data_2;
                code = (~code) & 0x0FFF;
                double data = code + 1.0;
                temp = -1.0 * data * 0.0625;
            }
            else
            {
                int code = (data_1 << 8) | data_2;
                code = code & 0x0FFF;
                temp = code * 0.0625;
                // plus
                //temp = ((data_2 << 8) + data_1) * 0.0625;
            }
            return temp;
        }
        public void Capture_DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            int rxByteCnt = 0;
            try
            {
                int inputByteLength = CapturePort.BytesToRead;
                byte[] recvData = new byte[inputByteLength];

                if(rxByteCnt > 0)
                {
                    rxByteCnt = 0;
                    Array.Clear(outData, 0x0, outData.Length);
                }
                CapturePort.Read(recvData, 0, inputByteLength);

                //recvData 여기서 처리 Paring Data
            }
            catch (Exception ex)
            {

            }
        }
    }
}
