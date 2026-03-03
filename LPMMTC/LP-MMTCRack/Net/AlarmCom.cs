using LP_MMTCRack.Model;
using LP_MMTCRack.ViewModel;
using Serilog;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LP_MMTCRack.Net
{
    public class AlarmCom
    {
        public string AlarmPortName { get; set; } = string.Empty;
        public bool AlarmPortFlag { get; set; } = false;
        public bool AlarmPortUse { get; set; } = false;
        public bool AllRackStatus { get; set; } = false;
        public SerialPort AlarmPort;
        private List<Byte> _RecvAlarmDataList;
        private List<Byte> RecvAlarmDataList
        {
            get
            {
                if (_RecvAlarmDataList == null)
                    _RecvAlarmDataList = new List<byte>();
                return _RecvAlarmDataList;
            }
        }
        public bool AlarmBDFlag = false;
        public AlarmCom()
        {
            Global.gAlarmCom = this;
        }
        public void AlarmConnect()
        {

            try
            {
                if(AlarmPort != null)
                {
                    AlarmPort.Close();
                    AlarmPort.Dispose();
                    Thread.Sleep(100);
                }

                AlarmPort = new SerialPort();
                AlarmPort.PortName = AlarmPortName;
                AlarmPort.BaudRate = 115200;
                AlarmPort.DataBits = 8;
                AlarmPort.Parity = Parity.None;
                AlarmPort.Handshake = Handshake.None;
                AlarmPort.StopBits = StopBits.One;
                AlarmPort.Encoding = Encoding.UTF8;
                AlarmPort.DataReceived += AlarmPort_DataReceived;
                AlarmPort.Open();
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "AlarmPort Connect");

            }
            catch (Exception ex)
            {
                AlarmPort.Close();
                AlarmPort.Dispose();
                string msg = "AlarmPort : " + ex.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
            AlarmWrite("S");
            AllRackStatus = false;
        }
        public void AlarmDisConnect()
        {
            if (AlarmPort != null)
            {
                AlarmPort.Close();
                AlarmPort.Dispose();
            }
        }
        public void AlarmWrite(string msg)
        {
            try
            {
                if ((AlarmPort != null) && (AlarmPort.IsOpen))
                {
                    AlarmPort.Write(msg);
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "==> 경광등 통신 연결을 확인해 주세요.");
                return;
            }
        }
        public async void AlarmPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bufferOffset = 0;
            int bytesToRead;

            try
            {
                bytesToRead = AlarmPort.BytesToRead;
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Exception] Alarm invoke" + ex.Message);
                return;
            }
            byte[] bytesBuffer = new byte[bytesToRead];

            if (bytesToRead > 0)
            {
                try
                {
                    int readBytes = AlarmPort.Read(bytesBuffer, bufferOffset, bytesToRead - bufferOffset);
                    bytesToRead -= readBytes;
                    bufferOffset += readBytes;
                }
                catch (Exception ex)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[Exception] Alarm" + ex.Message);
                    return;
                }
            }
            RecvAlarmDataList.AddRange(bytesBuffer);
            if (RecvAlarmDataList.Count >= 7)
            {
                if (RecvAlarmDataList[0] != 0x02 && RecvAlarmDataList[6] != 0x03)
                {
                    RecvAlarmDataList.Clear();
                }
                else
                {
                    if (RecvAlarmDataList[2] == 1)
                    {
                        //todo AlarmWarning_panel.SendToBack();
                        AlarmPort.DiscardInBuffer();
                        RecvAlarmDataList.RemoveRange(0, 7);
                        AlarmBDFlag = false;
                    }
                    else if (RecvAlarmDataList[2] == 2)
                    {
                        AlarmPort.DiscardInBuffer();
                        RecvAlarmDataList.RemoveRange(0, 7);
                        if (!AlarmBDFlag)
                        {
                            // 워터 리크 처리방법?
                            ////////////////////////////////////////////////////////////
                            //STOP 버튼 클릭
                            foreach (RackModel rm in Global.gMainPageVM.Racks)
                            {
                                await rm.RackstopTestAsync();
                            }
                            ////////////////////////////////////////////////////////////
                            AlarmBDFlag = true;
                        }
                    }
                }
            }
        }
    }
}
