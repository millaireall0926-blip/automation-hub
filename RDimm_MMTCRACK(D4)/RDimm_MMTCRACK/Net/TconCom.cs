using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.ViewModel;
using ScottPlot;
using ScottPlot.Statistics;
using Serilog;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace RDimm_MMTCRACK.Net
{
    public class SendMessage
    {
        public SendData sData;
        public CrcData sCrcData;
    }
    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class SendData
    {
        public byte ch;
        public byte fun;
        public byte sad_h;
        public byte sad_l;
        public byte len_h;
        public byte len_l;
    }
    [Serializable, StructLayout(LayoutKind.Sequential, Pack = 1)]
    public class CrcData
    {
        public byte crc_h;
        public byte crc_l;
    }
    public class TconCom
    {
        public string TconPortName { get; set; } = string.Empty;
        public SerialPort TConPort;
        private List<Byte> _RecvDataList;
        public List<Byte> RecvDataList
        {
            get
            {
                if (_RecvDataList == null)
                    _RecvDataList = new List<byte>();
                return _RecvDataList;
            }
        }
        public Thread _TconWriteThread;
        public Thread _TconReadThread;

        public TconCom()
        {
            Global.gTconCom = this;
            _TconWriteThread = new Thread(TconWriteFunction);
            _TconWriteThread.Start();
            _TconReadThread = new Thread(TconReadFunction);
            _TconReadThread.Start();
        }

        public void TConConnect()
        {
            try
            {
                if (TConPort != null && TConPort.IsOpen)
                {
                    TConPort.Close();
                    TConPort.Dispose();
                    Thread.Sleep(100);  // 잠시 대기
                }

                TConPort = new SerialPort
                {
                    PortName = TconPortName,
                    BaudRate = 38400,
                    DataBits = 8,
                    Parity = Parity.None,
                    Handshake = Handshake.None,
                    StopBits = StopBits.One,
                    Encoding = Encoding.UTF8,
                    DtrEnable = true,
                    WriteTimeout = 500,
                    ReadTimeout = 500
                };

                TConPort.DataReceived += TCONport_DataReceived;
                TConPort.Open();
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "TCONPort Connect");
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "TConPort : " + ex.Message);
                CleanupPort();
            }
        }

        private void CleanupPort()
        {
            if (TConPort != null)
            {
                if (TConPort.IsOpen)
                    TConPort.Close();
                TConPort.Dispose();
            }
        }

        public void TConDisConnet()
        {
            CleanupPort();
        }

        public void TConquit()
        {
            // CancellationToken을 사용하여 쓰레드 안전한 종료
            _TconWriteThread?.Join();
            _TconReadThread?.Join();
        }
        public void makeConsoleMsg(byte[] getPacket)
        {
            Array.ForEach(getPacket, num => sb.Append(num.ToString("X2") + " "));
            //Console.WriteLine(sb);
            sb.Clear();
        }
        public object obj_lock = new object();
        public Queue<byte[]> sendQueue = new Queue<byte[]>();

        StringBuilder sb = new StringBuilder();
        private void SendPacket(byte[] packet)
        {
            if (TConPort == null || !TConPort.IsOpen)
                return;

            lock (sendQueue)
            {
                sendQueue.Enqueue(packet);
            }
            makeConsoleMsg(packet);
        }

        public void TConStatMsg(byte ch)
        {
            var meg = newMsg(ch, 0x03, 0x00, 0x00, 0x00, 0x13);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            SendPacket(packet);
        }

        public void RunStopMsg(byte ch, byte flag)
        {
            var meg = newMsg(ch, 0x06, 0x00, 0x64, 0x00, flag);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            SendPacket(packet);
        }

        public void SetTempMsg(byte ch, string getTempVal)
        {
            byte[] SendSV = MConv.SVStringToByte(getTempVal);
            var meg = newMsg(ch, 0x06, 0x00, 0xC8, SendSV[0], SendSV[1]);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            SendPacket(packet);
        }

        public void ParaWriteMsg(byte ch, byte totalcnt, ArrayList getWriteMsg, int type)
        {
            ArrayList WriteMsg = new ArrayList { ch, (byte)0x10 };
            switch (type)
            {
                case 0:
                    WriteMsg.AddRange(new byte[] { 0x00, 0x64, 0x00, 0x28 });
                    break;
                case 1:
                    WriteMsg.AddRange(new byte[] { 0x00, 0xC8, 0x00, 0x0C });
                    break;
                case 2:
                    WriteMsg.AddRange(new byte[] { 0x02, 0x58, 0x00, 0x1D });
                    break;
            }

            WriteMsg.Add(totalcnt);
            WriteMsg.AddRange(getWriteMsg);
            byte[] SendMeg = (byte[])WriteMsg.ToArray(typeof(byte));
            var GetCrc = BitConverter.GetBytes(MConv.ComputeCrc(SendMeg));
            var packet = MConv.GetCombined(SendMeg, GetCrc);
            SendPacket(packet);
        }

        public void ParaReadMsg(byte ch, int func)
        {
            byte[] packet;
            switch (func)
            {
                case 0:
                    var meg0 = newMsg(ch, 0x03, 0x00, 0x64, 0x00, 0x28);
                    packet = MConv.GetCombined(MConv.StructToByte(meg0.sData), MConv.StructToByte(meg0.sCrcData));
                    break;
                case 1:
                    var meg1 = newMsg(ch, 0x03, 0x00, 0xC8, 0x00, 0x0C);
                    packet = MConv.GetCombined(MConv.StructToByte(meg1.sData), MConv.StructToByte(meg1.sCrcData));
                    break;
                case 2:
                    var meg2 = newMsg(ch, 0x03, 0x02, 0x58, 0x00, 0x1D);
                    packet = MConv.GetCombined(MConv.StructToByte(meg2.sData), MConv.StructToByte(meg2.sCrcData));
                    break;
                default:
                    return;
            }
            SendPacket(packet);
        }
        /*
        public string TconPortName { get; set; } = string.Empty;
        public SerialPort TConPort;
        private List<Byte> _RecvDataList;
        public List<Byte> RecvDataList
        {
            get
            {
                if (_RecvDataList == null)
                    _RecvDataList = new List<byte>();
                return _RecvDataList;
            }
        }
        public Thread _TconWriteThread;
        public Thread _TconReadThread;
        public TconCom()
        {
            Global.gTconCom = this;
            _TconWriteThread = new Thread(TconWriteFunction);
            _TconWriteThread.Start();
            _TconReadThread = new Thread(TconReadFunction);
            _TconReadThread.Start();
        }
        public void TConConnect()
        {
            try
            {
                if (TConPort != null)
                {
                    TConPort.Close();
                    TConPort.Dispose();
                    Thread.Sleep(100);
                    TConPort = null;
                }

                TConPort = new SerialPort();
                TConPort.PortName = TconPortName;
                TConPort.BaudRate = 38400;
                TConPort.DataBits = 8;
                TConPort.Parity = Parity.None;
                TConPort.Handshake = Handshake.None;
                TConPort.StopBits = StopBits.One;
                TConPort.Encoding = Encoding.UTF8;
                TConPort.DtrEnable = true;
                TConPort.WriteTimeout = 500;
                TConPort.ReadTimeout = 500;
                TConPort.DataReceived += new SerialDataReceivedEventHandler(TCONport_DataReceived);
                TConPort.Open();
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "TCONPort Connect");

            }
            catch (Exception ex)
            {
                if (TConPort != null)
                {
                    TConPort.Close();
                    TConPort.Dispose();
                    string msg = "TConPort : " + ex.Message;
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
                }
            }
        }
        public void TConDisConnet()
        {
            if (TConPort != null)
            {
                TConPort.Close();
                TConPort.Dispose();
            }
        }
        public void TConquit()
        {
            if(_TconWriteThread.IsAlive)
                _TconWriteThread.Join();
            if(_TconReadThread.IsAlive)
                _TconReadThread.Join();
        }
        public void makeConsoleMsg(byte[] getPacket)
        {
            Array.ForEach(getPacket, num => sb.Append(num.ToString("X2") + " "));
            //Console.WriteLine(sb);
            sb.Clear();
        }
        public object obj_lock = new object();
        public Queue<byte[]> sendQueue = new Queue<byte[]>();
        
        StringBuilder sb = new StringBuilder();

        public void TConStatMsg(byte ch)
        {
            var meg = newMsg(ch, 0x03, 0x00, 0x00, 0x00, 0x13);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));

            makeConsoleMsg(packet);

            if (TConPort == null)
                return;
            if (!TConPort.IsOpen)
                return;

            lock (sendQueue)
                sendQueue.Enqueue(packet);
        }
        public void RunStopMsg(byte ch, byte flag)
        {
            var meg = newMsg(ch, 0x06, 0x00, 0x64, 0x00, flag);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));

            makeConsoleMsg(packet);

            if (TConPort == null)
                return;
            if (!TConPort.IsOpen)
                return;

            lock (sendQueue)
                sendQueue.Enqueue(packet);
        }
        public void SetTempMsg(byte ch, string getTempVal)
        {
            if (TConPort == null)
                return;
            if (!TConPort.IsOpen)
                return;

            byte[] SendSV = MConv.SVStringToByte(getTempVal);

            var meg = newMsg(ch, 0x06, 0x00, 0xC8, SendSV[0], SendSV[1]);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            
            makeConsoleMsg(packet);

            if (TConPort == null)
                return;
            if (!TConPort.IsOpen)
                return;

            lock (sendQueue)
                sendQueue.Enqueue(packet);
        }
        public void ParaWriteMsg(byte ch, byte totalcnt, ArrayList getWriteMsg, int type)
        {
            ArrayList WriteMsg = new ArrayList();
            WriteMsg.Add(ch);
            WriteMsg.Add((byte)0x10);
            if (type == 0)
            {
                WriteMsg.Add((byte)0x00);
                WriteMsg.Add((byte)0x64);
                WriteMsg.Add((byte)0x00);
                WriteMsg.Add((byte)0x28);
            }
            else if(type == 1)
            {
                WriteMsg.Add((byte)0x00);
                WriteMsg.Add((byte)0xC8);
                WriteMsg.Add((byte)0x00);
                WriteMsg.Add((byte)0x0C);
            }
            else if(type == 2)
            {
                WriteMsg.Add((byte)0x02);
                WriteMsg.Add((byte)0x58);
                WriteMsg.Add((byte)0x00);
                WriteMsg.Add((byte)0x1D);
            }
            WriteMsg.Add((byte)totalcnt);
            WriteMsg.AddRange(getWriteMsg);
            byte[] SendMeg = (byte[])WriteMsg.ToArray(typeof(byte));
            var GetCrc = BitConverter.GetBytes(MConv.ComputeCrc(SendMeg));
            var packet = MConv.GetCombined(SendMeg, GetCrc);

            makeConsoleMsg(packet);

            if (TConPort == null)
                return;
            if (!TConPort.IsOpen)
                return;

            lock (sendQueue)
                sendQueue.Enqueue(packet);
        }
        public void ParaReadMsg(byte ch , int func)
        {
            if (func == 0)
            {
                var meg = newMsg(ch, 0x03, 0x00, 0x64, 0x00, 0x28);
                var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
                
                makeConsoleMsg(packet);

                lock (sendQueue)
                    sendQueue.Enqueue(packet);
            }
            else if(func == 1)
            {
                var meg = newMsg(ch, 0x03, 0x00, 0xC8, 0x00, 0x0C);
                var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));

                makeConsoleMsg(packet);

                if (TConPort == null)
                    return;
                if (!TConPort.IsOpen)
                    return;

                lock (sendQueue)
                    sendQueue.Enqueue(packet);
            }
            else if(func == 2)
            {
                var meg = newMsg(ch, 0x03, 0x02, 0x58, 0x00, 0x1D);
                var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));

                makeConsoleMsg(packet);

                if (TConPort == null)
                    return;
                if (!TConPort.IsOpen)
                    return;

                lock (sendQueue)
                    sendQueue.Enqueue(packet);
            }
        }
        */
        public async void TconWriteFunction()
        {
            byte[] seData;

            while (!Global.gMainVM._isCloseCom)
            {
                if (TConPort == null)
                {
                    await Task.Delay(10);
                    continue;
                }
                if (!TConPort.IsOpen)
                {
                    await Task.Delay(10);
                    continue;
                }

                if (sendQueue.Count > 0)
                {
                    lock (sendQueue)
                    {
                        Global.gEngineerVM.TconWriteCnt = sendQueue.Count;
                        //Console.WriteLine("sendQueue.Count ====>>>> {0}", sendQueue.Count);
                        seData = sendQueue.Dequeue();
                        //Console.WriteLine("sendQueue.Data ====>>>> {0}", seData.ToString());
                        //if (Global.gMainVM.AllRackSTAT == TOTALRACK_STAT.PARAMETER)
                        //    Global.gEngineerVM.addParaLog(seData.ToString());
                    }
                    RecvData_OK = false;
                    try
                    {
                        TConPort.Write(seData, 0, seData.Length);
                    }
                    catch { }
                    await Task.Delay(80);
                    /*
                    while (true)
                    {
                        try
                        {
                            if (!TConPort.IsOpen)
                                break;

                            RecvData_OK = false;
                            TConPort.Write(seData, 0, seData.Length);


                            await Task.Delay(100);

                            if (RecvData_OK || cntRetry > 4)
                            {
                                if (cntRetry > 0)
                                    Console.WriteLine("TConCom Retry = {0}", cntRetry);
                                cntRetry = 0;
                                break;
                            }
                            cntRetry++;
                        }
                        catch
                        {
                            Console.WriteLine("!!! TCON Port DisConnet !!!");
                        }
                    }
                    */
                }
                else
                    await Task.Delay(1);
            }
        }
        public async void TconReadFunction()
        {
            bool result = false;
            while (!Global.gMainVM._isCloseCom)
            {
                if (RecvDataList.Count >= 8)
                {
                    Global.gEngineerVM.TconReadCnt = RecvDataList.Count;
                    //Console.WriteLine("PrewaitRecv = {0}", RecvDataList.Count);
                    //afterwaitRecv = true;
                    if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.PARAMETER)
                        result = waitRecv();
                    else
                        result = waitParaRecv();

                    //if (RecvDataList.Count == 0)
                    //{
                    //Console.WriteLine("PrewaitRecv = {0}", RecvDataList.Count);
                    //break;
                    //}
                    //await Task.Delay(10);
                }
                else
                    await Task.Delay(1);
            }
        }
        public bool RecvData_OK = false;
        public void TCONport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = 0;

            try
            {
                bytesToRead = TConPort.BytesToRead;
            }
            catch (Exception ex)
            {
                //log.addLog(LOG_TYPE.WARNING, "[Exception] TempCtrl : " + ex.Message);
                return;
            }

            byte[] bytesBuffer = new byte[bytesToRead];

            if (bytesToRead > 0)
            {
                try
                {
                    int readBytes = TConPort.Read(bytesBuffer, 0, bytesToRead);

                    RecvDataList.AddRange(bytesBuffer);
                    Array.Clear(bytesBuffer, 0, bytesBuffer.Length);
                    readBytes = 0;
                }
                catch
                {
                    TConPort.DiscardInBuffer();
                    TConPort.DiscardOutBuffer();
                    bytesToRead = 0;
                }
            }
        }
        public bool waitRecv()
        {
            String sRecieveMsg = string.Empty;
            UInt16 crc16 = 0xFFFF;
            byte fun_byte;
            int packet_length;
            int data_length;

            try
            {
                if ((RecvDataList[0] < 1) || (RecvDataList[0] > 18))
                {
                    RecvDataList.Clear();
                    return false;
                }

                fun_byte = RecvDataList[1];

                if ((fun_byte != 03) && (fun_byte != 06) && (fun_byte != 08) && (fun_byte != 16))
                {
                    RecvDataList.Clear();
                    return false;
                }
                if (fun_byte == 0x03) //D-Register의 연속읽기      --> 수신 : 8바이트이상
                {
                    data_length = RecvDataList[2];
                    packet_length = 2 + 1 + data_length + 2;
                    if (packet_length > RecvDataList.Count)
                    {
                        return false;
                    }
                }
                else
                {
                    packet_length = 8; //06 수신데이타 fixed
                }

                for (int i = 0; i < packet_length; i++)
                {
                    sRecieveMsg += RecvDataList[i].ToString("X2").PadLeft(2, '0') + " ";
                    crc16 = MConv.CalculateCRC((Byte)RecvDataList[i], crc16);
                }
                if (crc16 == 0)
                {
                    string funcd = RecvDataList[1].ToString();
                    int Box = (RecvDataList[0] - 1) % 3;
                    int Rack = (int)((RecvDataList[0] - 1) / 3);

                    if (funcd == "3")
                    {
                        int RecvDataLength = RecvDataList[2];

                        if (RecvDataLength >= 38)
                        {
                            //ORG
                            if ((RecvDataList[32] & 0x03) == 0x00 && RecvDataList[32] < 0x04)
                            {
                                if (Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat == CBOX_STAT.PRERUN ||
                                    Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat == CBOX_STAT.RUN ||
                                    Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat == CBOX_STAT.DEFROST)
                                {
                                    if (Global.gMainPageVM.Racks[Rack].CBoxs[Box].Bimetal_Stat_Cnt < Global.BimetalErrorCnt)
                                        Global.gMainPageVM.Racks[Rack].CBoxs[Box].Bimetal_Stat_Cnt++;
                                    else
                                        Global.gMainPageVM.Racks[Rack].CBoxs[Box].BimetalStat(false);
                                }
                                else
                                {
                                    Global.gMainPageVM.Racks[Rack].CBoxs[Box].BimetalStat(false);
                                }
                                //Log Msg 추가
                            }
                            else
                            {
                                Global.gMainPageVM.Racks[Rack].CBoxs[Box].Bimetal_Stat_Cnt = 0;
                                Global.gMainPageVM.Racks[Rack].CBoxs[Box].BimetalStat(true);
                            }
                            //ORG
                            if ((RecvDataList[39] & 0x04) == 0x04 && RecvDataList[39] < 0x08)
                            {
                                if (Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat == CBOX_STAT.PRERUN ||
                                    Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat == CBOX_STAT.RUN ||
                                    Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat == CBOX_STAT.DEFROST)
                                {
                                    if (Global.gMainPageVM.Racks[Rack].CBoxs[Box].Sopen_Stat_Cnt < Global.BimetalErrorCnt)
                                        Global.gMainPageVM.Racks[Rack].CBoxs[Box].Sopen_Stat_Cnt++;
                                    else
                                        Global.gMainPageVM.Racks[Rack].CBoxs[Box].SopenStat(false);
                                }
                                else
                                {
                                    Global.gMainPageVM.Racks[Rack].CBoxs[Box].SopenStat(false);
                                }
                            }
                            else
                            {
                                Global.gMainPageVM.Racks[Rack].CBoxs[Box].Sopen_Stat_Cnt = 0;
                                Global.gMainPageVM.Racks[Rack].CBoxs[Box].SopenStat(true);
                            }
                        }

                        string nowvalue_hex = RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2");
                        string setvalue_hex = RecvDataList[5].ToString("X2") + RecvDataList[6].ToString("X2");
                        string nowstatus = RecvDataList[21].ToString("X2") + RecvDataList[22].ToString("X2");

                        short nowvalue_dec = Convert.ToInt16(nowvalue_hex, 16);
                        short setvalue_dec = Convert.ToInt16(setvalue_hex, 16);

                        double nowvalue_dou = nowvalue_dec / 10.0;
                        double setvalue_dou = setvalue_dec / 10.0;

                        if (nowstatus == "0001")
                            Global.gMainPageVM.Racks[Rack].CBoxs[Box].GetTconStatus(false);
                        //Global.gMainPageVM.Racks[Rack].CBoxs[Box].TconIsRun = false;
                        else
                            Global.gMainPageVM.Racks[Rack].CBoxs[Box].GetTconStatus(true);
                        //Global.gMainPageVM.Racks[Rack].CBoxs[Box].TconIsRun = true;

                        //ORG
                        Global.gMainPageVM.Racks[Rack].CBoxs[Box].InsideBoxTemp = nowvalue_dou; //InsideBoxTemp == GetPV
                        /////TEST 용////
                        //Global.gMainPageVM.Racks[Rack].Boxes[Box].SetGetPV(nowvalue_dou);
                        ///////////////
                        Global.gMainPageVM.Racks[Rack].CBoxs[Box].TargetSV = setvalue_dou; //TargetSV == GetSV

                        RecvDataList.RemoveRange(0, packet_length);

                        //if (Global.gMain.consoleFalg)
                        //Console.WriteLine("RECV <<<<< " + sRecieveMsg);
                        RecvData_OK = true;
                    }
                    else if (funcd == "6")
                    {
                        string DregisterHL = RecvDataList[3].ToString("X2");
                        if (DregisterHL == "64")
                        {
                            string setvalue_hex = RecvDataList[5].ToString("X2");

                            RecvDataList.RemoveRange(0, packet_length);

                            //Console.WriteLine("RECV <<<<< " + sRecieveMsg);
                            RecvData_OK = true;

                        }
                        else if (DregisterHL == "C8")
                        {
                            string setvalue_hex = RecvDataList[4].ToString("X2") + RecvDataList[5].ToString("X2");
                            short setvalue_dec = Convert.ToInt16(setvalue_hex, 16);
                            double setvalue_dou = setvalue_dec / 10.0;

                            Global.gMainPageVM.Racks[Rack].CBoxs[Box].TargetSV = setvalue_dou;

                            RecvDataList.RemoveRange(0, packet_length);

                            //if (Global.gMain.consoleFalg)
                           //Console.WriteLine("RECV <<<<< " + sRecieveMsg);
                            RecvData_OK = true;

                        }
                    }
                    return true;
                }
                else
                {
                    //Console.WriteLine("RECV <<<CRC16 ERROR<< " + sRecieveMsg);
                    RecvDataList.RemoveRange(0, packet_length);
                    TConPort.DiscardInBuffer();
                    TConPort.DiscardOutBuffer();
                }
            }
            catch (Exception e)
            {
                return false;
            }
            return false;
        }
        public bool waitParaRecv()
        {
            String sRecieveMsg = string.Empty;
            UInt16 crc16 = 0xFFFF;
            byte fun_byte;
            int packet_length;
            int data_length;

            try
            {
                if ((RecvDataList[0] < 1) || (RecvDataList[0] > 18))
                {
                    RecvDataList.Clear();
                    return false;
                }

                fun_byte = RecvDataList[1];

                if ((fun_byte != 03) && (fun_byte != 06) && (fun_byte != 08) && (fun_byte != 16))
                {
                    RecvDataList.Clear();
                    return false;
                }
                if (fun_byte == 0x03) //D-Register의 연속읽기      --> 수신 : 8바이트이상
                {
                    data_length = RecvDataList[2];
                    packet_length = 2 + 1 + data_length + 2;
                    if (packet_length > RecvDataList.Count)
                    {
                        return false;
                    }
                }
                else
                {
                    packet_length = 8; //06 수신데이타 fixed
                }

                for (int i = 0; i < packet_length; i++)
                {
                    sRecieveMsg += RecvDataList[i].ToString("X2").PadLeft(2, '0') + " ";
                    crc16 = MConv.CalculateCRC((Byte)RecvDataList[i], crc16);
                }
                if (crc16 == 0)
                {
                    string funcd = RecvDataList[1].ToString();
                    int ch = RecvDataList[0] - 1;

                    if (Global.gEngineerVM.writereadindex  == 0)
                    {
                        if(fun_byte == 0x03 && RecvDataList.Count >= 82)
                        {
                            Global.gEngineerVM.Funcdatas[0].ConvertVal(ch, Convert.ToInt16(RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2"),16).ToString());
                            Global.gEngineerVM.Funcdatas[1].ConvertVal(ch, Convert.ToInt16(RecvDataList[33].ToString("X2") + RecvDataList[34].ToString("X2"),16).ToString());
                            Global.gEngineerVM.Funcdatas[2].ConvertVal(ch, Convert.ToInt16(RecvDataList[77].ToString("X2") + RecvDataList[78].ToString("X2"),16).ToString());
                            Global.gEngineerVM.Funcdatas[3].ConvertVal(ch, (Convert.ToInt16(RecvDataList[79].ToString("X2") + RecvDataList[80].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.Funcdatas[4].ConvertVal(ch, (Convert.ToInt16(RecvDataList[81].ToString("X2") + RecvDataList[82].ToString("X2"),16) / 10.0).ToString());

                            Global.gEngineerVM.addParaLog(sRecieveMsg);
                            RecvDataList.RemoveRange(0, packet_length);
                            RecvData_OK = true;
                        }
                        else if(fun_byte == 0x10)
                        {
                            RecvDataList.RemoveRange(0, packet_length);
                            RecvData_OK = true;
                        }
                    }
                    else if(Global.gEngineerVM.writereadindex == 1)
                    {
                        if (fun_byte == 0x03 && RecvDataList.Count >= 26)
                        {
                            Global.gEngineerVM.SetPointdatas[0].ConvertVal(ch, (Convert.ToInt16(RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.SetPointdatas[1].ConvertVal(ch, (Convert.ToInt16(RecvDataList[5].ToString("X2") + RecvDataList[6].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.SetPointdatas[2].ConvertVal(ch, (Convert.ToInt16(RecvDataList[7].ToString("X2") + RecvDataList[8].ToString("X2"),16) /10.0).ToString());
                            Global.gEngineerVM.SetPointdatas[3].ConvertVal(ch, (Convert.ToInt16(RecvDataList[9].ToString("X2") + RecvDataList[10].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.SetPointdatas[4].ConvertVal(ch, (Convert.ToInt16(RecvDataList[23].ToString("X2") + RecvDataList[24].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.SetPointdatas[5].ConvertVal(ch, (Convert.ToInt16(RecvDataList[25].ToString("X2") + RecvDataList[26].ToString("X2"),16) / 10.0).ToString());

                            Global.gEngineerVM.addParaLog(sRecieveMsg);
                            RecvDataList.RemoveRange(0, packet_length);
                            RecvData_OK = true;
                        }
                        else if (fun_byte == 0x10)
                        {
                            RecvDataList.RemoveRange(0, packet_length);
                            RecvData_OK = true;
                        }
                    }
                    else if(Global.gEngineerVM.writereadindex == 2)
                    {
                        if (fun_byte == 0x03 && RecvDataList.Count >= 60)
                        {
                            Global.gEngineerVM.InOutdatas[0].ConvertVal(ch, Convert.ToInt16(RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2"),16).ToString());
                            Global.gEngineerVM.InOutdatas[1].ConvertVal(ch, (Convert.ToInt16(RecvDataList[7].ToString("X2") + RecvDataList[8].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[2].ConvertVal(ch, (Convert.ToInt16(RecvDataList[9].ToString("X2") + RecvDataList[10].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[3].ConvertVal(ch, (Convert.ToInt16(RecvDataList[23].ToString("X2") + RecvDataList[24].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[4].ConvertVal(ch, (Convert.ToInt16(RecvDataList[25].ToString("X2") + RecvDataList[26].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[5].ConvertVal(ch, (Convert.ToInt16(RecvDataList[27].ToString("X2") + RecvDataList[28].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[6].ConvertVal(ch, (Convert.ToInt16(RecvDataList[33].ToString("X2") + RecvDataList[34].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[7].ConvertVal(ch, (Convert.ToInt16(RecvDataList[35].ToString("X2") + RecvDataList[36].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[8].ConvertVal(ch, (Convert.ToInt16(RecvDataList[37].ToString("X2") + RecvDataList[38].ToString("X2"),16) / 10.0).ToString());
                            Global.gEngineerVM.InOutdatas[9].ConvertVal(ch, Convert.ToInt16(RecvDataList[55].ToString("X2") + RecvDataList[56].ToString("X2"),16).ToString());
                            Global.gEngineerVM.InOutdatas[10].ConvertVal(ch, Convert.ToInt16(RecvDataList[57].ToString("X2") + RecvDataList[58].ToString("X2"),16).ToString());
                            Global.gEngineerVM.InOutdatas[11].ConvertVal(ch, Convert.ToInt16(RecvDataList[59].ToString("X2") + RecvDataList[60].ToString("X2"),16).ToString());

                            Global.gEngineerVM.addParaLog(sRecieveMsg);
                            RecvDataList.RemoveRange(0, packet_length);
                            RecvData_OK = true;
                        }
                        else if (fun_byte == 0x10)
                        {
                            RecvDataList.RemoveRange(0, packet_length);
                            RecvData_OK = true;
                        }
                    }
                }
                else
                {
                    //Console.WriteLine("RECV <<<CRC16 ERROR<< " + sRecieveMsg);
                    RecvDataList.RemoveRange(0, packet_length);
                    TConPort.DiscardInBuffer();
                    TConPort.DiscardOutBuffer();
                }
            }
            catch
            {
                return false;
            }

            return false;
        }
        public SendMessage newMsg(byte ch, byte fun, byte saddr_h, byte saddr_l, byte len_h, byte len_l)
        {
            SendMessage msg = new SendMessage();
            msg.sData = new SendData();
            msg.sData.ch = (byte)ch;
            msg.sData.fun = (byte)fun;
            msg.sData.sad_h = (byte)saddr_h;
            msg.sData.sad_l = (byte)saddr_l;
            msg.sData.len_h = (byte)len_h;
            msg.sData.len_l = (byte)len_l;

            var getcrc = BitConverter.GetBytes(MConv.ComputeCrc(msg));
            msg = totalMsg(msg, getcrc);

            return msg;
        }
        public SendMessage totalMsg(SendMessage meg, byte[] gcrc)
        {
            meg.sCrcData = new CrcData();
            meg.sCrcData.crc_h = gcrc[0];
            meg.sCrcData.crc_l = gcrc[1];
            return meg;
        }
    }
}
