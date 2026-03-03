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
        private readonly object _recvDataLock = new object();
        private List<Byte> _RecvDataList;
        public List<Byte> RecvDataList
        {
            get
            {
                lock (_recvDataLock)
                {
                    if (_RecvDataList == null)
                        _RecvDataList = new List<byte>();
                    return _RecvDataList;
                }
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
        private void ConfigurePort(SerialPort port)
        {
            port.PortName = TconPortName;
            port.BaudRate = 38400;
            port.DataBits = 8;
            port.Parity = Parity.None;
            port.Handshake = Handshake.None;
            port.StopBits = StopBits.One;
            port.Encoding = Encoding.UTF8;
            port.DtrEnable = true;
            port.WriteTimeout = 500;
            port.ReadTimeout = 500;
            port.DataReceived += new SerialDataReceivedEventHandler(TCONport_DataReceived);
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
                ConfigurePort(TConPort);
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
        public void makeConsoleMsg(byte[] packet)
        {
            sb.Clear();
            foreach (byte b in packet)
            {
                sb.Append(b.ToString("X2")).Append(" ");
            }
            //Console.WriteLine(sb.ToString());
        }
        public object obj_lock = new object();
        public Queue<byte[]> sendQueue = new Queue<byte[]>();
        
        StringBuilder sb = new StringBuilder();

        public void TConStatMsg(byte ch)
        {
            var meg = CreateMessage(ch, 0x03, 0x00, 0x00, 0x00, 0x13);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            SendPacket(packet);
        }
        public void RunStopMsg(byte ch, byte flag)
        {
            var meg = CreateMessage(ch, 0x06, 0x00, 0x64, 0x00, flag);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            SendPacket(packet);
        }
        public void SetTempMsg(byte ch, string getTempVal)
        {
            byte[] SendSV = MConv.SVStringToByte(getTempVal);
            var meg = CreateMessage(ch, 0x06, 0x00, 0xC8, SendSV[0], SendSV[1]);
            var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));
            SendPacket(packet);
        }
        private void SendPacket(byte[] packet)
        {
            if (TConPort == null || !TConPort.IsOpen)
                return;

            makeConsoleMsg(packet);
            lock (sendQueue)
                sendQueue.Enqueue(packet);
        }

        private byte[] CreateParameterWriteMessage(byte ch, byte totalcnt, ArrayList getWriteMsg, int type)
        {
            var message = new List<byte>
            {
                ch,
                0x10
            };

            switch (type)
            {
                case 0:
                    message.AddRange(new byte[] { 0x00, 0x64, 0x00, 0x28 });
                    break;
                case 1:
                    message.AddRange(new byte[] { 0x00, 0xC8, 0x00, 0x0C });
                    break;
                case 2:
                    message.AddRange(new byte[] { 0x02, 0x58, 0x00, 0x1D });
                    break;
            }

            message.Add(totalcnt);
            message.AddRange(getWriteMsg.Cast<byte>());

            var messageBytes = message.ToArray();
            var crcBytes = BitConverter.GetBytes(MConv.ComputeCrc(messageBytes));
            return MConv.GetCombined(messageBytes, crcBytes);
        }

        public void ParaWriteMsg(byte ch, byte totalcnt, ArrayList getWriteMsg, int type)
        {
            var packet = CreateParameterWriteMessage(ch, totalcnt, getWriteMsg, type);
            SendPacket(packet);
        }

        private byte[] CreateParameterReadMessage(byte ch, int func)
        {
            SendMessage message;
            switch (func)
            {
                case 0:
                    message = CreateMessage(ch, 0x03, 0x00, 0x64, 0x00, 0x28);
                    break;
                case 1:
                    message = CreateMessage(ch, 0x03, 0x00, 0xC8, 0x00, 0x0C);
                    break;
                case 2:
                    message = CreateMessage(ch, 0x03, 0x02, 0x58, 0x00, 0x1D);
                    break;
                default:
                    return null;
            }

            return MConv.GetCombined(MConv.StructToByte(message.sData), MConv.StructToByte(message.sCrcData));
        }

        public void ParaReadMsg(byte ch, int func)
        {
            var packet = CreateParameterReadMessage(ch, func);
            if (packet != null)
            {
                SendPacket(packet);
            }
        }
        public async void TconWriteFunction()
        {
            while (!Global.gMainVM._isCloseCom)
            {
                try
                {
                    if (TConPort == null || !TConPort.IsOpen)
                    {
                        await Task.Delay(10);
                        continue;
                    }

                    byte[] seData = null;
                    lock (sendQueue)
                    {
                        if (sendQueue.Count > 0)
                        {
                            Global.gEngineerVM.TconWriteCnt = sendQueue.Count;
                            seData = sendQueue.Dequeue();
                        }
                    }

                    if (seData != null)
                    {
                        RecvData_OK = false;
                        TConPort.Write(seData, 0, seData.Length);
                        await Task.Delay(80);
                    }
                    else
                    {
                        await Task.Delay(1);
                    }
                }
                catch (InvalidOperationException ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"TConPort Write Error: {ex.Message}");
                    await Task.Delay(100);
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"TConPort Unexpected Error: {ex.Message}");
                    await Task.Delay(100);
                }
            }
        }
        public async void TconReadFunction()
        {
            while (!Global.gMainVM._isCloseCom)
            {
                try
                {
                    if (RecvDataList.Count >= 8)
                    {
                        Global.gEngineerVM.TconReadCnt = RecvDataList.Count;
                        bool success = Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.PARAMETER ? 
                            waitRecv() : waitParaRecv();

                        //if (!success)
                        //{
                            // 데이터 처리 실패 시 로그 추가
                        //    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Failed to process received data");
                            await Task.Delay(10);
                        //}
                    }
                    else
                    {
                        await Task.Delay(1);
                    }
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"TconReadFunction Error: {ex.Message}");
                    await Task.Delay(100);
                }
            }
        }
        public bool RecvData_OK = false;
        public void TCONport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int bytesToRead = TConPort.BytesToRead;
                if (bytesToRead <= 0)
                    return;

                byte[] bytesBuffer = new byte[bytesToRead];
                TConPort.Read(bytesBuffer, 0, bytesToRead);
                
                lock (_recvDataLock)
                {
                    RecvDataList.AddRange(bytesBuffer);
                }
            }
            catch (InvalidOperationException ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"TConPort Read Error: {ex.Message}");
                TConPort.DiscardInBuffer();
                TConPort.DiscardOutBuffer();
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"TConPort Unexpected Error: {ex.Message}");
            }
        }
        private bool ValidatePacket(List<byte> data, out byte fun_byte, out int packet_length)
        {
            fun_byte = 0;
            packet_length = 0;

            try
            {
                lock (_recvDataLock)
                {
                    if (data.Count < 8)
                        return false;

                    if ((data[0] < 1) || (data[0] > 18))
                    {
                        data.Clear();
                        return false;
                    }

                    fun_byte = data[1];
                    if ((fun_byte != 0x03) && (fun_byte != 0x06) && (fun_byte != 0x08) && (fun_byte != 0x16))
                    {
                        data.Clear();
                        return false;
                    }

                    if (fun_byte == 0x03)
                    {
                        int data_length = data[2];
                        packet_length = 2 + 1 + data_length + 2;
                        if (packet_length > data.Count)
                            return false;
                    }
                    else
                    {
                        packet_length = 8;
                    }

                    return true;
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"Packet validation error: {ex.Message}");
                return false;
            }
        }

        private bool CalculateCRC(List<byte> data, int packet_length, out string sRecieveMsg, out UInt16 crc16)
        {
            sRecieveMsg = string.Empty;
            crc16 = 0xFFFF;

            try
            {
                lock (_recvDataLock)
                {
                    for (int i = 0; i < packet_length; i++)
                    {
                        sRecieveMsg += data[i].ToString("X2").PadLeft(2, '0') + " ";
                        crc16 = MConv.CalculateCRC(data[i], crc16);
                    }
                    return true;
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CRC calculation error: {ex.Message}");
                return false;
            }
        }

        public bool waitRecv()
        {
            try
            {
                if (!ValidatePacket(RecvDataList, out byte fun_byte, out int packet_length))
                    return false;

                if (!CalculateCRC(RecvDataList, packet_length, out string sRecieveMsg, out UInt16 crc16))
                    return false;

                if (crc16 == 0)
                {
                    string funcd = RecvDataList[1].ToString();
                    int Box = (RecvDataList[0] - 1) % 3;
                    int Rack = (int)((RecvDataList[0] - 1) / 3);

                    if (funcd == "3")
                    {
                        ProcessStatusData(Rack, Box);
                    }
                    else if (funcd == "6")
                    {
                        ProcessControlData(Rack, Box);
                    }

                    RecvDataList.RemoveRange(0, packet_length);
                    //Console.WriteLine("RECV <<<<< " + sRecieveMsg);
                    RecvData_OK = true;
                    return true;
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CRC16 Error: {sRecieveMsg}");
                    RecvDataList.RemoveRange(0, packet_length);
                    TConPort.DiscardInBuffer();
                    TConPort.DiscardOutBuffer();
                    return false;
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"waitRecv error: {ex.Message}");
                return false;
            }
        }

        private void ProcessStatusData(int Rack, int Box)
        {
            if (RecvDataList.Count >= 38)
            {
                ProcessBimetalStatus(Rack, Box);
                ProcessSopenStatus(Rack, Box);

                string nowvalue_hex = RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2");
                string setvalue_hex = RecvDataList[5].ToString("X2") + RecvDataList[6].ToString("X2");
                string nowstatus = RecvDataList[21].ToString("X2") + RecvDataList[22].ToString("X2");

                short nowvalue_dec = Convert.ToInt16(nowvalue_hex, 16);
                short setvalue_dec = Convert.ToInt16(setvalue_hex, 16);

                double nowvalue_dou = nowvalue_dec / 10.0;
                double setvalue_dou = setvalue_dec / 10.0;

                Global.gMainPageVM.Racks[Rack].CBoxs[Box].GetTconStatus(nowstatus != "0001");
                Global.gMainPageVM.Racks[Rack].CBoxs[Box].InsideBoxTemp = nowvalue_dou;
                Global.gMainPageVM.Racks[Rack].CBoxs[Box].TargetSV = setvalue_dou;
            }
        }

        private void ProcessControlData(int Rack, int Box)
        {
            string DregisterHL = RecvDataList[3].ToString("X2");
            if (DregisterHL == "C8")
            {
                string setvalue_hex = RecvDataList[4].ToString("X2") + RecvDataList[5].ToString("X2");
                short setvalue_dec = Convert.ToInt16(setvalue_hex, 16);
                double setvalue_dou = setvalue_dec / 10.0;
                Global.gMainPageVM.Racks[Rack].CBoxs[Box].TargetSV = setvalue_dou;
            }
        }

        private void ProcessBimetalStatus(int Rack, int Box)
        {
            if ((RecvDataList[32] & 0x03) == 0x00 && RecvDataList[32] < 0x04)
            {
                if (IsRunningState(Rack, Box))
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
            }
            else
            {
                Global.gMainPageVM.Racks[Rack].CBoxs[Box].Bimetal_Stat_Cnt = 0;
                Global.gMainPageVM.Racks[Rack].CBoxs[Box].BimetalStat(true);
            }
        }

        private void ProcessSopenStatus(int Rack, int Box)
        {
            if ((RecvDataList[39] & 0x04) == 0x04 && RecvDataList[39] < 0x08)
            {
                if (IsRunningState(Rack, Box))
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

        private bool IsRunningState(int Rack, int Box)
        {
            var state = Global.gMainPageVM.Racks[Rack].CBoxs[Box].CBoxStat;
            return state == CBOX_STAT.PRERUN || state == CBOX_STAT.RUN || state == CBOX_STAT.DEFROST;
        }

        public bool waitParaRecv()
        {
            try
            {
                if (!ValidatePacket(RecvDataList, out byte fun_byte, out int packet_length))
                    return false;

                if (!CalculateCRC(RecvDataList, packet_length, out string sRecieveMsg, out UInt16 crc16))
                    return false;

                if (crc16 == 0)
                {
                    int ch = RecvDataList[0] - 1;
                    ProcessParameterData(ch, fun_byte, sRecieveMsg, packet_length);
                    return true;
                }
                else
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CRC16 Error: {sRecieveMsg}");
                    RecvDataList.RemoveRange(0, packet_length);
                    TConPort.DiscardInBuffer();
                    TConPort.DiscardOutBuffer();
                    return false;
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"waitParaRecv error: {ex.Message}");
                return false;
            }
        }

        private void ProcessParameterData(int ch, byte fun_byte, string sRecieveMsg, int packet_length)
        {
            if (fun_byte == 0x10)
            {
                RecvDataList.RemoveRange(0, packet_length);
                RecvData_OK = true;
                return;
            }

            if (fun_byte != 0x03)
                return;

            switch (Global.gEngineerVM.writereadindex)
            {
                case 0:
                    ProcessFunctionData(ch, sRecieveMsg, packet_length);
                    break;
                case 1:
                    ProcessSetPointData(ch, sRecieveMsg, packet_length);
                    break;
                case 2:
                    ProcessInOutData(ch, sRecieveMsg, packet_length);
                    break;
            }
        }

        private void ProcessFunctionData(int ch, string sRecieveMsg, int packet_length)
        {
            if (RecvDataList.Count < 82)
                return;

            var data = Global.gEngineerVM.Funcdatas;
            data[0].ConvertVal(ch, Convert.ToInt16(RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2"), 16).ToString());
            data[1].ConvertVal(ch, Convert.ToInt16(RecvDataList[33].ToString("X2") + RecvDataList[34].ToString("X2"), 16).ToString());
            data[2].ConvertVal(ch, Convert.ToInt16(RecvDataList[77].ToString("X2") + RecvDataList[78].ToString("X2"), 16).ToString());
            data[3].ConvertVal(ch, (Convert.ToInt16(RecvDataList[79].ToString("X2") + RecvDataList[80].ToString("X2"), 16) / 10.0).ToString());
            data[4].ConvertVal(ch, (Convert.ToInt16(RecvDataList[81].ToString("X2") + RecvDataList[82].ToString("X2"), 16) / 10.0).ToString());

            Global.gEngineerVM.addParaLog(sRecieveMsg);
            RecvDataList.RemoveRange(0, packet_length);
            RecvData_OK = true;
        }

        private void ProcessSetPointData(int ch, string sRecieveMsg, int packet_length)
        {
            if (RecvDataList.Count < 26)
                return;

            var data = Global.gEngineerVM.SetPointdatas;
            data[0].ConvertVal(ch, (Convert.ToInt16(RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2"), 16) / 10.0).ToString());
            data[1].ConvertVal(ch, (Convert.ToInt16(RecvDataList[5].ToString("X2") + RecvDataList[6].ToString("X2"), 16) / 10.0).ToString());
            data[2].ConvertVal(ch, (Convert.ToInt16(RecvDataList[7].ToString("X2") + RecvDataList[8].ToString("X2"), 16) / 10.0).ToString());
            data[3].ConvertVal(ch, (Convert.ToInt16(RecvDataList[9].ToString("X2") + RecvDataList[10].ToString("X2"), 16) / 10.0).ToString());
            data[4].ConvertVal(ch, (Convert.ToInt16(RecvDataList[23].ToString("X2") + RecvDataList[24].ToString("X2"), 16) / 10.0).ToString());
            data[5].ConvertVal(ch, (Convert.ToInt16(RecvDataList[25].ToString("X2") + RecvDataList[26].ToString("X2"), 16) / 10.0).ToString());

            Global.gEngineerVM.addParaLog(sRecieveMsg);
            RecvDataList.RemoveRange(0, packet_length);
            RecvData_OK = true;
        }

        private void ProcessInOutData(int ch, string sRecieveMsg, int packet_length)
        {
            if (RecvDataList.Count < 60)
                return;

            var data = Global.gEngineerVM.InOutdatas;
            data[0].ConvertVal(ch, Convert.ToInt16(RecvDataList[3].ToString("X2") + RecvDataList[4].ToString("X2"), 16).ToString());
            data[1].ConvertVal(ch, (Convert.ToInt16(RecvDataList[7].ToString("X2") + RecvDataList[8].ToString("X2"), 16) / 10.0).ToString());
            data[2].ConvertVal(ch, (Convert.ToInt16(RecvDataList[9].ToString("X2") + RecvDataList[10].ToString("X2"), 16) / 10.0).ToString());
            data[3].ConvertVal(ch, (Convert.ToInt16(RecvDataList[23].ToString("X2") + RecvDataList[24].ToString("X2"), 16) / 10.0).ToString());
            data[4].ConvertVal(ch, (Convert.ToInt16(RecvDataList[25].ToString("X2") + RecvDataList[26].ToString("X2"), 16) / 10.0).ToString());
            data[5].ConvertVal(ch, (Convert.ToInt16(RecvDataList[27].ToString("X2") + RecvDataList[28].ToString("X2"), 16) / 10.0).ToString());
            data[6].ConvertVal(ch, (Convert.ToInt16(RecvDataList[33].ToString("X2") + RecvDataList[34].ToString("X2"), 16) / 10.0).ToString());
            data[7].ConvertVal(ch, (Convert.ToInt16(RecvDataList[35].ToString("X2") + RecvDataList[36].ToString("X2"), 16) / 10.0).ToString());
            data[8].ConvertVal(ch, (Convert.ToInt16(RecvDataList[37].ToString("X2") + RecvDataList[38].ToString("X2"), 16) / 10.0).ToString());
            data[9].ConvertVal(ch, Convert.ToInt16(RecvDataList[55].ToString("X2") + RecvDataList[56].ToString("X2"), 16).ToString());
            data[10].ConvertVal(ch, Convert.ToInt16(RecvDataList[57].ToString("X2") + RecvDataList[58].ToString("X2"), 16).ToString());
            data[11].ConvertVal(ch, Convert.ToInt16(RecvDataList[59].ToString("X2") + RecvDataList[60].ToString("X2"), 16).ToString());

            Global.gEngineerVM.addParaLog(sRecieveMsg);
            RecvDataList.RemoveRange(0, packet_length);
            RecvData_OK = true;
        }

        public SendMessage CreateMessage(byte ch, byte fun, byte saddr_h, byte saddr_l, byte len_h, byte len_l)
        {
            var msg = new SendMessage
            {
                sData = new SendData
                {
                    ch = ch,
                    fun = fun,
                    sad_h = saddr_h,
                    sad_l = saddr_l,
                    len_h = len_h,
                    len_l = len_l
                }
            };

            var crcBytes = BitConverter.GetBytes(MConv.ComputeCrc(msg));
            return AddCrcToMessage(msg, crcBytes);
        }

        private SendMessage AddCrcToMessage(SendMessage msg, byte[] crcBytes)
        {
            msg.sCrcData = new CrcData
            {
                crc_h = crcBytes[0],
                crc_l = crcBytes[1]
            };
            return msg;
        }
    }
}

