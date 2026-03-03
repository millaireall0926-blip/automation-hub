using HTCRack_CXL.Model;
using HTCRack_CXL.ViewModel;
using SuperSocket.ClientEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace HTCRack_CXL.Net
{
    public enum DATA_TYPE : byte
    {
        BIT = 0,
        BYTE,
        WORD,
        DWORD,
        LWORD,
        SEQ = 0x14
    }
    public class ModbusLS : AsyncTcpSession
    {
        public bool _isConnect = false;
        public string _ip = "";
        public int _port = 2004;

        public bool _readDone = false;
        public bool _isNetErr = false;
        public bool _isProtocalErr = false;
        public byte[] _readData = new byte[1024];
        public int _dataCnt = 0;
        public int _byteCnt = 0;
        public object _lock = new object();

        public bool _mainAlarm = false;
        public bool _emergency = false;

        public bool requestRX(string[] adrs)
        {
            if (!_isConnect) return false;

            lock (_lock)
            {
                _readDone = false;
                _isNetErr = false;
                Task t = Task.Run(() => sendRegR(DATA_TYPE.BIT, adrs));
                waitRecv();
            }
            if (!_readDone) return false;

            return !_isNetErr;
        }

        public bool requestWX(string[] adrs, byte[][] datas)
        {
            if (!_isConnect) return false;

            lock (_lock)
            {
                _readDone = false;
                _isNetErr = false;
                Task t = Task.Run(() => sendRegW(DATA_TYPE.BIT, adrs, datas));
                waitRecv();
            }
            if (!_readDone) return false;

            return !_isNetErr;
        }

        public ModbusLS()
        {
            _isConnect = false;
            Connected += doneConnect;
            Closed += doneClose;
            Error += doneError;
            DataReceived += doneRecv;
            ReceiveBufferSize = 1024;
        }

        //int MAKE_WORD(int _a, int _b)
        //{
        //    return ((_a & 0xff) << 8) | (_b & 0xff);
        //}
        int MAKE_WORD(int _a, int _b)
        {
            return ((_a & 0xff)) | ((_b & 0xff) << 8);
        }


        public List<byte> makeHeader(int length)
        {
            List<byte> header = new List<byte>();

            //Company ID
            header.AddRange(Encoding.ASCII.GetBytes("LSIS-XGT"));
            header.AddRange(new byte[] { 0x00, 0x00 });  // null char add in Company ID

            //PLC Info 
            header.AddRange(new byte[] { 0x00, 0x00 });

            //PLC Cpu - XGK : 0xA0 , XGR : 0xA8, XGB : 0xB4 
            header.Add(0xB4);

            //Frame - Client -> Plc : 0x33 , plc -> Client : 0x11
            header.Add(0x33);

            //Invoke ID, Frame Sequnce
            header.AddRange(new byte[] { 0x00, 0x00 });

            //Length 
            header.AddRange(BitConverter.GetBytes((ushort)length));

            //FNET Position
            header.Add(0x00);

            ushort bcc = 0;
            for (int i = 0; i < header.Count; i++)
            {
                bcc += header[i];
            }

            // Reserved BCC Checksum
            header.Add((byte)(bcc));

            return header;
        }


        public void sendRegR(DATA_TYPE type, string[] adrs)  // DataType = Byte
        {
            if (!_isConnect) return;

            List<byte> frame = new List<byte>();
            // Command Write
            frame.AddRange(new byte[] { 0x54, 0x00 });

            // DataType : bool = 0000, byte 0100, word 2000, uint 0300, long long, 0400            
            frame.AddRange(new byte[] { (byte)type, 0x00 });

            // Reserved
            frame.AddRange(new byte[] { 0x00, 0x00 });

            // block count
            frame.AddRange(BitConverter.GetBytes((ushort)(adrs.Length)));

            foreach (string adr in adrs)
            {
                // adr char length
                frame.AddRange(BitConverter.GetBytes((ushort)adr.Length));
                // adr Ascii 
                // MX : Bit, MB : Byte, MW : Word
                // MD : DWord(32 bit), ML : LWord(64 bit)
                frame.AddRange(Encoding.ASCII.GetBytes(adr));
            }



            var sendList = makeHeader(frame.Count);
            sendList.AddRange(frame);
            byte[] sendBuf = sendList.ToArray();
            Send(sendBuf, 0, sendBuf.Length);

        }

        public void sendRegW(DATA_TYPE type, string[] adrs, byte[][] datas) // DataType = Byte
        {
            List<byte> frame = new List<byte>();
            // Command Write
            frame.AddRange(new byte[] { 0x58, 0x00 });

            // DataType : bool = 0000, byte 0100, word 2000, uint 0300, long long, 0400
            frame.AddRange(new byte[] { (byte)type, 0x00 });

            // Reserved
            frame.AddRange(new byte[] { 0x00, 0x00 });

            // block count ( byte Base)
            frame.AddRange(BitConverter.GetBytes((ushort)(adrs.Length)));


            foreach (string adr in adrs)
            {
                // adr char length
                frame.AddRange(BitConverter.GetBytes((ushort)adr.Length));
                // adr Ascii                 
                // MX : Bit, MB : Byte, MW : Word
                // MD : DWord(32 bit), ML : LWord(64 bit)
                frame.AddRange(Encoding.ASCII.GetBytes(adr));
            }

            foreach (byte[] data in datas)
            {
                // data byte length
                frame.AddRange(BitConverter.GetBytes((ushort)(data.Length)));
                frame.AddRange(data);
            }


            var sendList = makeHeader(frame.Count);
            sendList.AddRange(frame);
            byte[] sendBuf = sendList.ToArray();
            Send(sendBuf, 0, sendBuf.Length);
        }


        // MX100 ~ MX103
        public void getPC_Ctrl()
        {
            bool ret = requestRX(new string[] { "%MX100", "%MX101", "%MX102", "%MX103" });
            if (!ret) { Close(); return; }
            if (_readData.Length < 4) return;
            byte PC_CONTROL_SLOT_ALL_AUTO = _readData[0];
            byte PC_CONTROL_SLOT_1_AUTO = _readData[1];
            byte PC_CONTROL_SLOT_2_AUTO = _readData[2];
            byte PC_CONTROL_SLOT_3_AUTO = _readData[3];
        }

        public bool setSlotPower(int index, bool enable)
        {
            if (index >= Global.gConfigVM.PlcRackCount) return false;

            index += 1;
            //string acAdr = $"%MX1{index}1";
            string v48Adr = $"%MX1{index}2";
            string v12Adr = $"%MX1{index}3";
            byte value = (byte)(enable ? 1 : 0);

            bool ret = requestWX(new string[] { v48Adr }, new byte[][] { new byte[] { value } });
            Thread.Sleep(500);
            ret &= requestWX(new string[] { v12Adr }, new byte[][] { new byte[] { value } });
            //Thread.Sleep(500);
            //ret &= requestWX(new string[] { acAdr }, new byte[][] { new byte[] { value } });

            return ret;
        }

        public void processPowerStat(int index, int sum)
        {
            if (index >= Global.gConfigVM.PlcRackCount) return;

            RackModel rm = Global.gMainPageVM.getRack(index);
            if (rm == null) return;
            if (sum == 0) rm.PowerStat = POWER_STAT.POWER_OFF;
            else if (sum == 2) rm.PowerStat = POWER_STAT.POWER_ON;
            else rm.PowerStat = POWER_STAT.MIXED;
        }

        public void getSlotPower(int index)
        {
            if (index >= Global.gConfigVM.PlcRackCount) return;

            bool ret = requestRX(new string[] { $"%MX1{index}2", $"%MX1{index}3" });
            if (!ret) { Close(); return; }
            if (_readData.Length < 2) return;
            int sum = _readData[0] + _readData[1];

            processPowerStat(index, sum);

        }

        public void getPLCAll()  // POWER, ALARM, EMEGENCY
        {
            List<string> reqList = new List<string>();
            int start = 0;
            int end = Global.gConfigVM.PlcRackCount / 2;

            for (int i = start; i < end; i++)
            {
                int racki = i + 1;

                for (int j = 0; j < 3; j++)
                {
                    int regi = j + 1;
                    reqList.Add($"%MX1{racki}{regi}");
                }
            }
            bool ret = requestRX(reqList.ToArray());
            if (!ret) { Close(); return; }
            for (int i = start; i < end; i++)
            {
                int pos = i * 3;

                if (_readData[pos] == 0)
                    if (!(_emergency | _mainAlarm))
                        setSlotAC(i, true);

                int sum = _readData[pos + 1] + _readData[pos + 2];



                processPowerStat(i, sum);
            }

            reqList.Clear();
            start = end;
            end = Global.gConfigVM.PlcRackCount;
            for (int i = start; i < end; i++)
            {
                int racki = i + 1;

                for (int j = 0; j < 3; j++)
                {
                    int regi = j + 1;
                    reqList.Add($"%MX1{racki}{regi}");
                }
            }

            ret = requestRX(reqList.ToArray());
            if (!ret) { Close(); return; }


            for (int i = 0; i < end - start; i++)
            {
                int pos = i * 3;
                if (_readData[pos] == 0)
                    if (!(_emergency | _mainAlarm))
                        setSlotAC(i + start, true);

                int sum = _readData[pos + 1] + _readData[pos + 2];
                processPowerStat(i + start, sum);
            }

            reqList.Clear();
            reqList.Add("%MX000");
            reqList.Add("%MX011");
            ret = requestRX(reqList.ToArray());
            if (!ret) { Close(); return; }

            int index = 0;//start * 3;

            bool prevErr = (_emergency | _mainAlarm);
            _emergency = (_readData[index++] == 1) ? true : false;
            _mainAlarm = (_readData[index++] == 1) ? true : false;
            bool curErr = (_emergency | _mainAlarm);

            if (curErr)
            {
                if (!prevErr) // error 발생
                {
                    string msg = "";
                    if (_emergency) msg += $"Emergency  ";
                    if (_mainAlarm) msg += $"Alarm  ";
                    msg += "Occured";

                    for (int i = 0; i < Global.gConfigVM.PlcRackCount; i++)
                    {
                        if (i >= Global.gMainPageVM.Racks.Count) break;

                        RackModel rm = Global.gMainPageVM.Racks[i];
                        rm.showAlram(ALRAM_TYPE.ERROR, "", msg, false);

                        setSlotPower(rm.RackIndex, false);
                        setSlotAC(rm.RackIndex, false);
                        if (!rm._client.IsConnected) continue;
                        rm.RackStat = RACK_STAT.ERROR;

                    }

                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, "[PLC] : " + msg);


                    getAlarmDetail();
                }
            }
            if (!curErr)      // error 해제
            {
                if (prevErr)
                {
                    string msg = $"PLC Error State Clear";
                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                    for (int i = 0; i < Global.gConfigVM.PlcRackCount; i++)
                    {
                        if (i >= Global.gMainPageVM.Racks.Count) break;
                        RackModel rm = Global.gMainPageVM.Racks[i];
                        rm.ShowBox = System.Windows.Visibility.Hidden;
                    }
                }
            }
        }

        string[] AlarmErrorList =
        {
            "Leak Detect",
            "Leak Burn Detect",
            "DC 48V Power Error",
            "DC 12V Power Error",
            "Fire Occured",
            "Leak Occured",
            "Buzzer On"
        };

        public void getAlarmDetail()
        {
            bool ret = requestRX(new string[] { $"%MX005", $"%MX006", $"%MX007",
                                                $"%MX008", $"%MX013", $"%MX015",
                                                $"%MX016"});
            if (!ret) { Close(); return; }
            if (_readData.Length < 7) return;

            for (int i = 0; i < 7; i++)
            {
                byte compVal = 0;
                if (i == 2 || i == 3) compVal = 1;
                bool err = (_readData[i] != compVal);
                
                if (err)
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[PLC] : Alarm Detail {AlarmErrorList[i]}");

            }
        }

        public void clearLeakError()
        {
            string adr = $"%PX027";
            byte value = 1;
            requestWX(new string[] { adr }, new byte[][] { new byte[] { value } });
        }

        public void setPC_Ctrl(int sel /* 0=all, 1=slot1 ... */ , bool enable)
        {
            string adr = $"%MX10{sel}";
            byte value = (byte)(enable ? 1 : 0);
            requestWX(new string[] { adr }, new byte[][] { new byte[] { value } });
        }

        public void setSlotAC(int index, bool enable)
        {
            if (index >= Global.gConfigVM.PlcRackCount) return;
            index += 1;
            string adr = $"%MX1{index}1";
            byte value = (byte)(enable ? 1 : 0);
            requestWX(new string[] { adr }, new byte[][] { new byte[] { value } });
        }

        public void setSlotMC(int index, bool enable)
        {
            if (index >= Global.gConfigVM.PlcRackCount) return;
            index += 1;
            string adr = $"%MX1{index}0";
            byte value = (byte)(enable ? 1 : 0);
            requestWX(new string[] { adr }, new byte[][] { new byte[] { value } });
        }

        public bool getSlotMC(int index)
        {
            if (index >= Global.gConfigVM.PlcRackCount) return false;
            bool ret = requestRX(new string[] { $"%MX11{index}" });
            if (!ret) { Close(); return false; }
            if (_readData.Length < 1) return false;
            return _readData[0] == 1 ? true : false;
        }


        public void doConnect()
        {
            try
            {
                if (_ip == "") return;
                Connect(new IPEndPoint(IPAddress.Parse(_ip), _port));
            }
            catch (Exception ex)
            {
#if TEST_MODE
                Console.WriteLine("PLC Connect Error!!");
#endif
            }
        }

        public void doneConnect(object sender, EventArgs e)
        {
            _isConnect = true;
#if TEST_MODE
            Console.WriteLine("LS Modbus Connected!");
#endif
        }

        public bool waitRecv()
        {
            int waitCnt = 0;
            while (!_readDone)
            {
                Thread.Sleep(1);
                waitCnt++;
                if (waitCnt >= 1000) // wait 5S
                    break;
                //              Console.WriteLine($"wait : {waitCnt}");
            }


            return _readDone;
        }

        public void doneRecv(object sender, DataEventArgs e)
        {
            if (e.Data.Length < 27)
                return; // error
            _isProtocalErr = false;

            short errVal = BitConverter.ToInt16(e.Data, 26);
            if (errVal != 0)
            {
                _isProtocalErr = true;
                return;
            }

            if (e.Data[20] == 0x59) goto RECV_DONE; // Response of Write  

            // index 28
            int readCount = BitConverter.ToInt16(e.Data, 28);
            int idxRbuf = 30;
            int idxWbuf = 0;
            for (int i = 0; i < readCount; i++)
            {
                short byteCnt = BitConverter.ToInt16(e.Data, idxRbuf);//(ushort)(MAKE_WORD( e.Data[idxRbuf++], e.Data[idxRbuf++]));
                idxRbuf += 2;
                Array.Copy(e.Data, idxRbuf, _readData, idxWbuf, byteCnt);
                idxRbuf += byteCnt;
                idxWbuf += byteCnt;
            }

            _dataCnt = readCount;
            _byteCnt = idxWbuf;

        RECV_DONE:
            _readDone = true;
        }

        public void doneError(object sender, EventArgs e)
        {
            Close();
        }

        public void doneClose(object sender, EventArgs e)
        {
            _isConnect = false;
            
            for (int i = 0; i < Global.gConfigVM.PlcRackCount; i++)
            {
                if (i >= Global.gMainPageVM.Racks.Count) break;
                RackModel rm = Global.gMainPageVM.Racks[i];
                rm.PowerStat = Model.POWER_STAT.PLC_OFF;
            }
        }

    }
}
