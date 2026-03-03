using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.Model;
using RDimm_MMTCRACK.View.Component;
using RDimm_MMTCRACK.ViewModel;
using ScottPlot.Statistics;
using Serilog;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace RDimm_MMTCRACK.Net
{
    public class SiteCom
    {
        public int SiteCh { get; set; } = 0;
        public string SitePortName { get; set; } = string.Empty;
        public string MotorValue { get; set; } = string.Empty;
        public bool SitePortUse { get; set; } = false;
        public string MotorFinalStep { get; set; } = string.Empty;
        public bool MotorPositon { get; set; } = false;
        public bool GetSiteRecv { get; set; } = true;
        public bool IsMotorMove { get; set; } = false;
        public bool IsMtrValueUpdated { get; set; } = false;
        public bool IsGetSiteRecv { get; set; } = false;
        public bool IsMotorDown { get; set; } = false;
        
        public bool Board_SensingOK { get; set; } = false;
        public bool Board_MotorPowerOn { get; set; } = false;
        public bool isMotorStatus { get; set; } = true;
        public SerialPort SitePort;
        public SiteCom()
        {
            Global.gSiteCom = this;
        }
        public void SiteConnect(int ch)
        {
            try
            {

                Global.gMainPageVM.Racks[SiteCh].MotorStatText = string.Empty;// 비우고 다시 받을지
                Global.gMainPageVM.Racks[SiteCh].MotorStatColor = Global.gMotorStatColors[1];
                Global.gEngineerVM.ManualMotor[SiteCh].eMotorStatColor = Global.gMotorStatColors[1];
                foreach (CBoxModel bm in Global.gMainPageVM.Racks[SiteCh].CBoxs)
                {
                    bm.HumidityValue = 0;
                }

                if (SitePort != null)
                {
                    SitePort.Close();
                    SitePort.Dispose();
                    Thread.Sleep(100);
                }

                SiteCh = ch;
                SitePort = new SerialPort();
                SitePort.PortName = SitePortName;
                SitePort.BaudRate = 38400; // 9600;
                SitePort.DataBits = 8;
                SitePort.Parity = Parity.None; // Parity.None;
                SitePort.Handshake = Handshake.None;
                SitePort.StopBits = StopBits.One;

                SitePort.DataReceived += new SerialDataReceivedEventHandler(SiteBoard_DataReceivedHandler);
                SitePort.Open();
                lock (sendSiteQueue)
                {
                    sendSiteQueue.Clear();
                }
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"SitePort {SiteCh + 1} Connect");
            }
            catch (Exception e)
            {
                SitePort.Close();
                SitePort.Dispose();
                string msg = "[SitePort" + (SiteCh + 1) + "exception] " + e.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }

        public void SiteDisConnect()
        {
            if (SitePort != null)
            {
                SitePort.Close();
                SitePort.Dispose();
            }
        }
        public Queue<byte[]> sendSiteQueue = new Queue<byte[]>();
        public void SiteWirteMsg(string val)
        {
            int rotattion = 0;
            int sendlen = MConv.CheckSendLength(val);
            byte[] txData = new byte[sendlen];

            try
            {
                string str_count = val.Substring(1);
                if (str_count == "")
                {
                    str_count = "0";
                }

                rotattion = Int32.Parse(str_count);
            }
            catch
            {
                rotattion = 0;
            }
            txData = MConv.MakeSiteMeg(val, rotattion, sendlen);

            if (SitePort == null)
                return;
            if (!SitePort.IsOpen)
                return;

            lock (sendSiteQueue)
            {
                sendSiteQueue.Enqueue(txData);
            }

            PreMotorSendMsg();
        }
        public void SiteHeightWriteMsg(int val)
        {
            byte[] txData = new byte[3];

            if (val == 17)
            {
                txData[0] = 0x11;
                txData[1] = 0x11;
                txData[2] = 0x0a;
            }

            if (SitePort == null)
                return;
            if (!SitePort.IsOpen)
                return;

            lock (sendSiteQueue)
            {
                sendSiteQueue.Enqueue(txData);
            }
            PreMotorSendMsg();
        }
        public object site_lock = new object();
        private void PreMotorSendMsg()
        {
            lock (site_lock)
            {
                MotorSendMessage();
            }
        }
        private void MotorSendMessage()
        {
            if (SitePort == null)
                return;
            if (!SitePort.IsOpen)
                return;

            try
            {
                int SiteRetry = 0;
                byte[] siteData;

                lock (sendSiteQueue)
                {
                    //Console.WriteLine("sendSiteQueue.Count ====>>>> {0}", sendSiteQueue.Count);
                    siteData = sendSiteQueue.Dequeue();
                }


                GetSiteRecv = true;

                SitePort.Write(siteData, 0, siteData.Length);

                if (IsMtrValueUpdated)
                    IsMtrValueUpdated = false;
                while (SiteRetry <= 100)
                {
                    if (GetSiteRecv == false)// || SiteRetry > 4)
                    {
                        //if (SiteRetry > 0)
                        //Console.WriteLine("SiteCom Write & Recv OK ");
                        break;
                    }
                    Thread.Sleep(100);
                    SiteRetry++;
                }
                if (GetSiteRecv)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] Write & Read Com Fail ");
                }

            }
            catch (Exception ex)
            {
                //string msg = $"Motor Write Fail, Board" + (SiteCh + 1) + " " + ex.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "Motor Write Fail, Board" + (SiteCh + 1) + " " + ex.Message);
            }
        }

        public byte[] recvResult = new byte[20];
        public void SiteBoard_DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            //Thread.Sleep(50);
            int rotattion = 0;
            int boardnum = SiteCh;
            int rxByteCnt = 0;
            try
            {
                int inputByteLength = SitePort.BytesToRead;
                byte[] recvData = new byte[inputByteLength];

                if (rxByteCnt >= 18)
                {
                    rxByteCnt = 0;
                    Array.Clear(recvResult, 0x0, recvResult.Length);
                }

                SitePort.Read(recvData, 0, inputByteLength);

                if (inputByteLength < 19)
                {
                    for (int i = 0; i <= inputByteLength - 1; i++)
                    {
                        if ((recvData[i] != 0x00) || (recvResult[0] + recvResult[1] + recvResult[2] >= 0x100))
                        {
                            recvResult[rxByteCnt] = recvData[i];
                            rxByteCnt++;
                        }
                        if ((recvData[0] == 0x41) && (i < 8))
                        {
                            continue;
                        }
                        if (recvData[i] == 0x0a)
                        {
                            if (recvData[0] == 0x11)
                            {
                                if (!IsMtrValueUpdated)
                                {


                                    string msg = "";
                                    UInt16 repeat_count = 0;

                                    /*
                                    for (int k = 0; k <= i; k++)
                                    {
                                        msg = msg + " " + recvData[k].ToString("X2");
                                    }
                                    */
                                    repeat_count = (UInt16)((recvData[1] << 8) | (recvData[2]));
                                    msg = Convert.ToString(repeat_count, 10);

                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Rx : " + repeat_count.ToString());
                                    Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] Rx : " + repeat_count.ToString());
                                    GetSiteRecv = false;
                                    IsMtrValueUpdated = true;
                                }
                            }

                            if (recvData[0] != 0x0a && recvData[1] != 0x0a)
                            {
                                if (recvData[1] != recvResult[1])
                                {
                                    for (int j = 4; j >= 2; j--)
                                    {
                                        recvResult[j] = recvResult[j - 1];
                                        if (j == 2)
                                        {
                                            recvResult[1] = 0x00;
                                        }
                                    }
                                }
                                if (recvResult[0] == 0x51) // "P" InterLock Open
                                {
                                    GetSiteRecv = false;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, " [S" + (SiteCh + 1) + "] InterLock Open ");
                                    Global.gEngineerVM.addMotorLog(" [S" + (SiteCh + 1) + "] InterLock Open ");
                                    Board_MotorPowerOn = true;
                                }
                                else if (recvResult[0] == 0x52) // "R"  InterLock Close
                                {
                                    GetSiteRecv = false;
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, " [S" + (SiteCh + 1) + "] InterLock Close");
                                    Global.gEngineerVM.addMotorLog(" [S" + (SiteCh + 1) + "] InterLock Close");
                                }

                                if (recvResult[0] == 0x48) //모터상승
                                {
                                    GetSiteRecv = false;
                                    IsMotorDown = false;

                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    if (rm == null) return;

                                    rm.MotorStatText = "OPEN";
                                    //rm.MotorStatColor = Global.gMotorStatColors[1];

                                    //회전수
                                    rotattion = (recvResult[1] << 8) & (recvResult[2]);
                                    //Global.gMain.Txt_display[boardnum].Text = rotattion.ToString();
                                }
                                else if (recvResult[0] == 0x4c) //모터하강
                                {
                                    GetSiteRecv = false;
                                    IsMotorDown = true;

                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    if (rm == null) return;

                                    rm.MotorStatText = "CLOSE";
                                    //rm.MotorStatColor = Global.gMotorStatColors[0];

                                    //회전수
                                    rotattion = (recvResult[1] << 8) & (recvResult[2]);
                                    //Global.gMain.Txt_display[boardnum].Text = rotattion.ToString();
                                }
                                else if (recvResult[0] == 0x49) //Sense Info
                                {
                                    if ((recvResult[1] & 0x01) == 0x01) //상승
                                    {
                                        GetSiteRecv = false;
                                        //Console.WriteLine("Sense Info Data Open in");
                                        IsMotorDown = false;

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.MotorStatText = "OPEN";
                                        //rm.MotorStatColor = Global.gMotorStatColors[1];
                                    }
                                    if (((~recvResult[1]) & 0x01) == 0x01) //하강
                                    {
                                        GetSiteRecv = false;
                                        //Console.WriteLine("Sense Info Data Close in");
                                        IsMotorDown = false;

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.MotorStatText = "CLOSE";
                                        //rm.MotorStatColor = Global.gMotorStatColors[0];
                                    }
                                    if ((~recvResult[1] & 0x02) == 0x02) // Slide Open status
                                    {
                                        GetSiteRecv = false;
                                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Slide Open");
                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.GetSlideOpen(true);
                                    }
                                    else if ((recvResult[1] & 0x02) == 0x02) //Slide Close Status
                                    {
                                        GetSiteRecv = false;
                                        //Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] Slide Close");
                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        rm.GetSlideOpen(false);
                                    }
                                }
                                else if (recvResult[0] == 0x56) // FW Version
                                {
                                    GetSiteRecv = false;
                                    RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                    Global.gEngineerVM.ManualMotor[rm.RackIndex].GetSiteVer = recvResult[1].ToString();
                                    Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] SiteBoard FW Version = " + recvResult[1].ToString());
                                    Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] SiteBoard FW Version = " + recvResult[1].ToString());
                                }

                                if ((recvResult[0] == 0x31) || (recvResult[0] == 0x33)) //MCU Statue - InterLock On
                                {
                                    // 230526 Siteboard 적용건으로 인한 수정
                                    //1번 바이트 - Board Address
                                    if (recvResult[1] == Convert.ToByte((SiteCh + 1).ToString("X")))
                                    {
                                        //2번, 3번 Sense
                                        int sense1 = (recvResult[2] & 0xF0) >> 4;
                                        int sense2 = (recvResult[2] & 0x0F);
                                        int sense3 = (recvResult[3] & 0xF0) >> 4;

                                        //L R 위치 변경
                                        //YHS 20231031
                                        string msg = "[S" + (SiteCh + 1).ToString() + "] ";
                                        if (sense1 == 1) { msg += "L(ON) "; } else { msg += "L(OFF) "; }
                                        if (sense2 == 1) { msg += "C(ON) "; } else { msg += "C(OFF) "; }
                                        if (sense3 == 1) { msg += "R(ON) "; } else { msg += "R(OFF) "; }

                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                                        Global.gEngineerVM.addMotorLog(msg);

                                        if ((sense1 == 1) && (sense2 == 1) && (sense3 == 1))
                                        {
                                            Board_SensingOK = true;
                                        }
                                        GetSiteRecv = false;
                                    }
                                    else
                                    {
                                        string msg = "[S" + (SiteCh + 1) + "] Interlock On Cmd SiteBoard Address Error.";
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, msg);
                                    }
                                }
                                else if (recvResult[0] == 0x32) //MCU Motor Reponse - Interlock Off
                                {
                                    // 230524 사이트보드 적용 수정 
                                    if (recvResult[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
                                    {
                                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "[S" + (SiteCh + 1) + "] MotorPower is On");
                                        Global.gEngineerVM.addMotorLog("[S" + (SiteCh + 1) + "] MotorPower is On");
                                        Board_MotorPowerOn = true;
                                        GetSiteRecv = false;
                                    }
                                }
                                else if (recvResult[0] == 0x41) //습도체크 humidity check
                                {
                                    // 230524 사이트보드 적용 수정 
                                    if (recvResult[1] == uint.Parse((SiteCh + 1).ToString("X"), System.Globalization.NumberStyles.AllowHexSpecifier))
                                    {
                                        //boardnum = 1;
                                        //Console.WriteLine("humidity Data in");
                                        int[] humidity = new int[3];
                                        humidity[0] = recvResult[2];
                                        humidity[1] = recvResult[3];
                                        humidity[2] = recvResult[4];

                                        int[] temperature = new int[3];
                                        temperature[0] = recvResult[5];
                                        temperature[1] = recvResult[6];
                                        temperature[2] = recvResult[7];

                                        if ((temperature[0] & 0x80) > 0)
                                            temperature[0] = -temperature[0];
                                        if ((temperature[1] & 0x80) > 0)
                                            temperature[1] = -temperature[1];
                                        if ((temperature[2] & 0x80) > 0)
                                            temperature[2] = -temperature[2];

                                        RackModel rm = Global.gMainPageVM.getRack(SiteCh);
                                        if (rm == null) return;

                                        for (int ch = 0; ch < rm.CBoxs.Count; ch++)
                                        {
                                            rm.CBoxs[ch].SetHumidityValue(humidity[ch]);
                                            rm.CBoxs[ch].Hum_TempSenorVal = temperature[ch];
                                        }

                                        GetSiteRecv = false;
                                    }
                                }
                            }
                            rxByteCnt = 0;

                            Array.Clear(recvResult, 0x0, recvResult.Length);
                        }
                    }
                }
                else
                {
                    rxByteCnt = 0;
                    Array.Clear(recvResult, 0x0, recvResult.Length);
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[S" + (SiteCh + 1) + "] ex : " + ex.Message);
            }
        }
    }
}
