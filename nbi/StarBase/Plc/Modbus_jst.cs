using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using SuperSocket.ClientEngine;
using System.Runtime.InteropServices;
using System.Windows.Threading;
using System.Threading;
using System.Collections.ObjectModel;

namespace StarBase.Plc {

    /*
    ex
     Modebus aaa ;
    if ( type == JST)
      aaa = new Modebus_jst();
     * */
    public class Modbus_jst : Modbus {

        public Modbus_jst() 
        {
            DataReceived += done_DataReceived;

            readPLCFunc = readPLC;
            getDataFunc = getData;
            setOperationFunc = setOperation;
            setRemoteFunc = setRemote;
            setTempFunc = setTemp;                        
            setResetFunc = setReset;    

            setDoorFunc = setDoor;
            setRackFunc = setRack;
            setLampOwnFunc = setLampOwn;
            setBuzzerFunc = setBuzzer;

            setLampFunc = setLamp;
        }

        public string getData(int index, int size = 4) {
            if (_recvData.Length <= index) return "";

            string sub = _recvData.Substring(index, size);
            return sub;
        }

        public void done_DataReceived(object sender, DataEventArgs e) {
            //e.Data, e.Length   

            string data = MConv.ByteToString(e.Data);
            _recvData = data.Substring(0, data.IndexOf("\r\n"));
            _readDone = true;
            if (getData(2, 2) != "OK") {
                //ERROR
                Console.WriteLine("PLC Interface Error!!");
                _recvData = "";
                _retOK = false;
                return;
            }
            _retOK = true;
            //            Console.WriteLine("recv : {0} str : {1}", e.Length, _recvData.Length);
        }

        public bool request(string cmd) {
            //            string cmd = string.Format("I{0:D5}", PLCMap.PLC_WT_STOP_I);
            //            string buf = string.Format($"01BRW01{cmd},1\r\n");
            //            byte[] sb = MConv.StringToByte(buf);
            //            char[] ss = Encoding.UTF8.GetChars(sb);
            //            Console.WriteLine(ss);
            if (_connect == false) return false;

            lock (_lock) {
                string sendCmd = "01";
                sendCmd += cmd;
                sendCmd += "\r\n";  // + 13 10
                byte[] packet = MConv.StringToByte(sendCmd);

                _readDone = false;
                _retOK = false;
                Task t = Task.Run(() => Send(packet, 0, packet.Length));

                waitRecv();
            }
            return _retOK;

        }

        public bool readMulti(string cmd, int count) {

            string send = string.Format($"WRR{count}{cmd}");
            return request(send);


        }

        public bool setOperation(PLC_STAT_TYPE mode) {

            if (mode == SetStat) return true;
            SetStat = mode;

            string cmd = "";
            if (mode == PLC_STAT_TYPE.STOP)
                cmd = string.Format("I{0:D5}", PLCMap_jst.PLC_WT_STOP_I);
            else if (mode == PLC_STAT_TYPE.RUN)
                cmd = string.Format("I{0:D5}", PLCMap_jst.PLC_WT_RUN_I);
            else if (mode == PLC_STAT_TYPE.AMBI)
                cmd = string.Format("I{0:D5}", PLCMap_jst.PLC_WT_AMBI_I);
            string scmd = "BRW01" + cmd + ",1";
            return request(scmd);

        }

        public bool setRemote() // change mode 1
        {
            string cmd = string.Format("BRW01I{0:D5},1", PLCMap_jst.PLC_WT_REMOTE_I);
            return request(cmd);
        }

        public bool setTemp(double temp) {
            if (temp == SetTargetTemp) return true;
            SetTargetTemp = temp;
            int val = Convert.ToInt32(temp * 10.0);
            int nTemp = val & 0xffff;
            string cmd = string.Format("WRW01D{0:D5},{1:X4}", PLCMap_jst.PLC_WT_SV_W, nTemp);
            return request(cmd);
        }

        public bool setReset() {
            string cmd = string.Format("BRW01I{0:D5},1", PLCMap_jst.PLC_WT_RESET_I);
            return request(cmd);
        }

        public bool setDoor(int ctrl) // 0 = open, 1= close
        {
            int adr = PLCMap_jst.PLC_WT_DOOROPEN_I;
            if (ctrl == 1)
                adr = PLCMap_jst.PLC_WT_DOORCLOSE_I;

            string cmd = string.Format("BRW01I{0:D5},1", adr);
            return request(cmd);
        }

        public bool setRack(int ctrl) // 0 = remove, 1 insert
        {
            int adr = PLCMap_jst.PLC_WT_RACKOUT_I;
            if (ctrl == 1)
                adr = PLCMap_jst.PLC_WT_RACKIN_I;

            string cmd = string.Format("BRW01I{0:D5},1", adr);
            return request(cmd);
        }

        public bool setLampOwn(int own) // 0 = OS, 1 = PLC
        {
            string cmd = string.Format("BRW01I{0:D5},{1}", PLCMap_jst.PLC_RW_LAMP_CTRL_I, own);
            return request(cmd);
        }

        public bool setBuzzer(int buzzer) // 0 = OS, 1 = PLC
{
            string cmd = string.Format("BRW01I{0:D5},{1}", PLCMap_jst.PLC_RW_BUZZER_I, buzzer);
            return request(cmd);
        }

        public bool setLamp(bool on, LAMP_TYPE type, bool blink) {
            int adr = PLCMap_jst.PLC_RW_LAMP_R_I;

            if (type == LAMP_TYPE.R) {
                adr = PLCMap_jst.PLC_RW_LAMP_R_I;
            }
            else if (type == LAMP_TYPE.Y) {
                adr = PLCMap_jst.PLC_RW_LAMP_Y_I;
            }
            else if (type == LAMP_TYPE.G) {
                adr = PLCMap_jst.PLC_RW_LAMP_G_I;
            }
            else if (type == LAMP_TYPE.ALL) {
                adr = PLCMap_jst.PLC_RW_LAMP_ALL_I;
                blink = false;
            }
            else
                return false;

            int value = 0;
            if (on) value = 1;
            if (blink) adr += 5;
            string cmd = string.Format("BRW01I{0:D5},{1}", adr, value);
            return request(cmd);
        }



        public void readPLC() {
            String cmd = "";
            int wCnt = 0;

            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_PV_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SV_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_STAT_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_LAMP1_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_LAMP2_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SYS_ALARM1_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SYS_ALARM2_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SYS_ALARM3_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SYS_ALARM4_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SYS_ALARM5_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_SYS_ALARM6_W); wCnt++;
            cmd += string.Format("D{0:D5},", PLCMap_jst.PLC_RD_POWER_ALARM_W); wCnt++;
            readMulti(cmd, wCnt);

            if (_recvData == "") return;
            if (_recvData.Length < 50) return;
            int rcnt = 4;
            string str = getData(rcnt);
            int val = Convert.ToInt32(str, 16);
            if ((val & (0xf000)) != 0)
                val = (0xffff << 16) | val;

            CurTemp = Convert.ToDouble(val) / 10.0; // CurTemp


            rcnt += 4;
            str = getData(rcnt);
            val = Convert.ToInt32(str, 16);
            if ((val & (0xf000)) != 0)
                val = (0xffff << 16) | val;
            TargetTemp = Convert.ToDouble(val) / 10.0; // TargetTemp
            SetTargetTemp = TargetTemp;

            rcnt += 4;
            str = getData(rcnt);
            val = Convert.ToInt32(str, 16);
            Remote = (val >> 5) & 1;
            Stat = (PLC_STAT_TYPE)(val & 0x7);
            SetStat = Stat;
            Rack = (val >> 6) & 1;
            if ((val & 0x10) != 0) Door = 1; // close
            else if ((val & 0x8) != 0) Door = 0; // open
            else Door = 2;  // noInfo

            rcnt += 4;
            int lamp1 = Convert.ToInt32(getData(rcnt), 16);
            rcnt += 4;
            int lamp2 = Convert.ToInt32(getData(rcnt), 16);
            LampStat = 0;
            LampStat |= (lamp1 >> 10) & 0x7;           // R Y G
            LampStat |= ((lamp1 >> 15) & 0x1) << 4;    // RB
            LampStat |= ((lamp1 >> 14) & 0x1) << 8;    // buzzer
            LampStat |= (lamp2 & 0x3) << 5;            // YB GB
            LampStat |= ((lamp1 >> 13) & 0x1) << 12;   // Lamp All 

            LampOwn = (lamp1 >> 9) & 0x1;  // 1=PLC, 0=OS Ctrl

            for (int i = 0; i < PLCMap_jst.AlramCount; i++) {
                rcnt += 4;
                _plcAlram[i] = Convert.ToInt32(getData(rcnt), 16); // ALARM read       
            }

        }

    }
   
}
