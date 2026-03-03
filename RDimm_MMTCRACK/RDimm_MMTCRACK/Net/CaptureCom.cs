using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.ViewModel;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RDimm_MMTCRACK.Net
{
    public class CaptureCom
    {
        public int CaputureCh { get; set; } = 0;
        public string CaputurePortName { get; set; } = string.Empty;
        public bool CaputureUse { get; set; } = false;
        public double[] i2c_temperatureResult = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        public SerialPort CapturePort;

        public CaptureCom()
        {
            Global.gCaptureCom = this;
        }
        public void CaptureConnect(int ch)
        {
            try
            {
                foreach (CBoxModel bm in Global.gMainPageVM.Racks[CaputureCh].CBoxs)
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
                CapturePort = new SerialPort();
                CapturePort.PortName = CaputurePortName;
                CapturePort.BaudRate = 9600;
                CapturePort.DataBits = 8;
                CapturePort.Parity = Parity.None;
                CapturePort.StopBits = StopBits.One;
                CapturePort.Open();
                
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "CapturePort " + (CaputureCh + 1) + " Connect");
            }
            catch(Exception ex)
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
                CapturePort.Close();
                CapturePort.Dispose();
            }
        }
        //public byte[] outData = new byte[3]; // 한번에 Read 하는 데이터에 따라 변경
        public byte[] outData = new byte[17];
        public double[] RackDimmTemp = new double[32];
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
            //Console.WriteLine("SEND : {0}", data[0]);
            await Task.Delay(300);

            RackModel rm = Global.gMainPageVM.getRack(CaputureCh);

            Array.Clear(outData, 0, outData.Length);
            readCaptureData();

            for (int i = 0; i < outData.Length; i++)
            {
                RecvData += outData[i];
                RecvData += " , ";
            }

            Array.Clear(RackDimmTemp, 0, RackDimmTemp.Length); //RackDimmTemp 초기화

            RackDimmTemp[22] = ReadDimmTemperature(outData[2], outData[1], 22);
            RackDimmTemp[23] = ReadDimmTemperature(outData[4], outData[3], 23);
            RackDimmTemp[20] = ReadDimmTemperature(outData[6], outData[5], 20);
            RackDimmTemp[21] = ReadDimmTemperature(outData[8], outData[7], 21);
            RackDimmTemp[18] = ReadDimmTemperature(outData[10], outData[9], 18);
            RackDimmTemp[19] = ReadDimmTemperature(outData[12], outData[11], 19);
            RackDimmTemp[16] = ReadDimmTemperature(outData[14], outData[13], 16);
            RackDimmTemp[17] = ReadDimmTemperature(outData[16], outData[15], 17);


            Array.Clear(outData, 0, outData.Length);
            readCaptureData();

            for (int i = 0; i < outData.Length; i++)
            {
                RecvData += outData[i];
                RecvData += " , ";
            }


            RackDimmTemp[24] = ReadDimmTemperature(outData[2], outData[1], 24);
            RackDimmTemp[25] = ReadDimmTemperature(outData[4], outData[3], 25);
            RackDimmTemp[26] = ReadDimmTemperature(outData[6], outData[5], 26);
            RackDimmTemp[27] = ReadDimmTemperature(outData[8], outData[7], 27);
            RackDimmTemp[28] = ReadDimmTemperature(outData[10], outData[9], 28);
            RackDimmTemp[29] = ReadDimmTemperature(outData[12], outData[11], 29);
            RackDimmTemp[30] = ReadDimmTemperature(outData[14], outData[13], 30);
            RackDimmTemp[31] = ReadDimmTemperature(outData[16], outData[15], 31);

            await Task.Delay(10);

            Array.Clear(outData, 0, outData.Length);
            readCaptureData();

            for (int i = 0; i < outData.Length; i++)
            {
                RecvData += outData[i];
                RecvData += " , ";
            }


            RackDimmTemp[6] = ReadDimmTemperature(outData[2], outData[1], 6);
            RackDimmTemp[7] = ReadDimmTemperature(outData[4], outData[3], 7);
            RackDimmTemp[4] = ReadDimmTemperature(outData[6], outData[5], 4);
            RackDimmTemp[5] = ReadDimmTemperature(outData[8], outData[7], 5);
            RackDimmTemp[2] = ReadDimmTemperature(outData[10], outData[9], 2);
            RackDimmTemp[3] = ReadDimmTemperature(outData[12], outData[11], 3);
            RackDimmTemp[0] = ReadDimmTemperature(outData[14], outData[13], 0);
            RackDimmTemp[1] = ReadDimmTemperature(outData[16], outData[15], 1);

            await Task.Delay(10);

            Array.Clear(outData, 0, outData.Length);
            readCaptureData();

            for (int i = 0; i < outData.Length; i++)
            {
                RecvData += outData[i];
                RecvData += " , ";
            }


            RackDimmTemp[8] = ReadDimmTemperature(outData[2], outData[1], 8);
            RackDimmTemp[9] = ReadDimmTemperature(outData[4], outData[3], 9);
            RackDimmTemp[10] = ReadDimmTemperature(outData[6], outData[5], 10);
            RackDimmTemp[11] = ReadDimmTemperature(outData[8], outData[7], 11);
            RackDimmTemp[12] = ReadDimmTemperature(outData[10], outData[9], 12);
            RackDimmTemp[13] = ReadDimmTemperature(outData[12], outData[11], 13);
            RackDimmTemp[14] = ReadDimmTemperature(outData[14], outData[13], 14);
            RackDimmTemp[15] = ReadDimmTemperature(outData[16], outData[15], 15);

            await Task.Delay(500);

            Global.gMainPageVM.Racks[CaputureCh].PreprocessTest();
            //Console.WriteLine("RECV : {0}", RecvData);

            //Console.WriteLine(aaa);
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
                if (17 < rxLen)
                    rxLen = 17;

                if (CapturePort.IsOpen)
                    CapturePort.Read(outData, 0, rxLen);
            }
            catch { }


        }

        public int[] emtpyCnt = new int[32];
        public double ReadDimmTemperature(byte data_1, byte data_2, int dimch)
        {
            /*
            int d0, d1, d2;
            int merge;
            double temp = 0;



            //-0.25 : xxx1 1111 1111 11xx --> 0x1F 0xFC  0.1875
            //-1.00 : xxx1 1111 1111 00xx --> 0x1F 0xF0  0.9374
            //-2.75 : xxx1 1111 1101 01xx --> 0xfF 0xD4


            d0 = data_1 & 0x10;     //부호 익스트렉션
            d1 = data_1 & 0xF;      //상위 4bit data 익스트렉션
            d2 = data_2 & 0xFF;     //하위 8bit data 익스트렉션

            if (d0 == 0)
            {
                //양수
                merge = (d1 * 256) + d2;
                temp = merge * 0.0625;

                //온도데이타 버림처리한다.
                temp = (double)(Math.Truncate(temp * 10) / 10);
            }
            else
            {
                //음수
                merge = (int)((0xFFFFF000 + d1 * 256 + d2));
                temp = merge * 0.0625;
            }
            */
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
                // plus
                int code = (data_1 << 8) | data_2;
                code = code & 0x0FFF;
                temp = code * 0.0625;
                //temp = ((data_1 << 8) + data_2) * 0.0625;
            }
            temp = Math.Round(temp, 1);
            //Console.WriteLine($"{data_1} {data_2}  TEMP = {temp}");

            RackModel rm = Global.gMainPageVM.getRack(CaputureCh);

            if(rm._prevDimmTemps[dimch] != 0)
            {
                if ((temp < (-20.0)) || (temp > (125.0))) // -20도 , 125 넘어가는 값은 0으로 처리
                {
                    temp = 0;
                }
                else
                {
                    if (temp == 0)
                        emtpyCnt[dimch]++;
                    else
                        emtpyCnt[dimch] = 0;

                    if (emtpyCnt[dimch] < 5)
                    {
                        double diff = temp - rm._prevDimmTemps[dimch];

                        if (Math.Abs(diff) > Global.gConfigVM.NoiseTempDiff)
                            temp = rm._prevDimmTemps[dimch];
                    }
                }
            }
            else if ((temp < -20.0) || (temp > 125.0)) // -20도 , 125 넘어가는 값은 0으로 처리
            {
                temp = 0;
            }

            rm._prevDimmTemps[dimch] = temp;

            //막아놈 10.14
            //if ((temp <= (-10.0)) || (temp >= (99.0)))
            //{
            //    return 0;
            //}

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
