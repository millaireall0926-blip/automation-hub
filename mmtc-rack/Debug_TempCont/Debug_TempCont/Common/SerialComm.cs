using Debug_TempCont.ViewModel;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Debug_TempCont.Common
{
    public class SerialComm
    {
        const char STX = (char)0x02;
        const char CR = (char)0x0D;
        const char LF = (char)0x0A;

        public bool bWorkThread_rx = false;
        public bool bWorkThread_tx = false;

        string[] nowsetvalue_hex = new string[8];
        short[] nowsetvalue_dec = new short[8];
        double[] nowsetvalue_dou = new double[8];

        Thread thread_tx = null;
        private object LockEvent = new object();

        Calculator cal = null;
        public SerialPort sPort = null;
        ConnetVM cvm = null;
        FuncVM fvm = null;
        TempChVM tcvm = null;

        private List<Byte> _RecvDataList;
        private List<Byte> RecvDataList
        {
            get
            {
                if (_RecvDataList == null)
                    _RecvDataList = new List<byte>();
                return _RecvDataList;
            }
        }

        public SerialComm()
        {
            Global.gSerialComm = this;
            cal = new Calculator();
            sPort = new SerialPort();
            cvm = (ConnetVM)App.Current.Services.GetService(typeof(ConnetVM));
            fvm = (FuncVM)App.Current.Services.GetService(typeof(FuncVM));
            tcvm = (TempChVM)App.Current.Services.GetService(typeof(TempChVM));
        }
        public void Connect_Run()
        {
            if (!sPort.IsOpen)
            {

                sPort.PortName = cvm.Ctm.SelPort;
                sPort.BaudRate = Convert.ToInt32(cvm.Ctm.Selpeed);
                sPort.DataBits = Convert.ToInt32(cvm.Ctm.Seldatalen);
                if (cvm.Ctm.Selparity == "None")
                    sPort.Parity = Parity.None;
                else if (cvm.Ctm.Selparity == "Odd")
                    sPort.Parity = Parity.Odd;
                else if (cvm.Ctm.Selparity == "Even")
                    sPort.Parity = Parity.Even;
                else
                    sPort.Parity = Parity.None;

                sPort.Handshake = Handshake.None;
                if (cvm.Ctm.Selstopbit == "1")
                    sPort.StopBits = StopBits.One;
                else if (cvm.Ctm.Selstopbit == "0")
                    sPort.StopBits = StopBits.Two;
                sPort.ReadTimeout = 500;
                sPort.WriteTimeout = 500;
                sPort.DataReceived += new SerialDataReceivedEventHandler(_Sport_DataReceived);

                sPort.Open();

                if (sPort.IsOpen)
                {
                    cvm.Ctm.Isenabed = "False";
                    cvm.Ctm.Isenabed2 = "True";
                    Global.gPortName = sPort.PortName;
                    Global.gProtocolName = cvm.Ctm.Protocoltxt;
                    Global.gM7Number = cvm.Ctm.M7num;
                    fvm.Fcm.statusBackgroundColor1 = Brushes.GreenYellow;
                    fvm.Fcm.statustxt = "통신 가능";
                    if (bWorkThread_tx == false)
                    {
                        bWorkThread_tx = true;
                        thread_tx = new Thread(new ThreadStart(Thread_Run_Send));
                        thread_tx.Priority = ThreadPriority.Lowest;
                        thread_tx.Start();
                    }
                }
            }
            else
            {

            }
        }
        public void Connect_Stop()
        {
            if (bWorkThread_tx)
            {
                bWorkThread_tx = false;
                thread_tx.Join();
            }
            if (sPort.IsOpen)
            {
                cvm.Ctm.Isenabed = "True";
                cvm.Ctm.Isenabed2 = "False";
                sPort.Close();
                sPort.Dispose();
                fvm.Fcm.statusBackgroundColor1 = Brushes.OrangeRed;
                fvm.Fcm.statustxt = "통신 불가";
            }
        }
        public void _Sport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
            {
                int bufferOffset = 0;
                int bytesToRead = sPort.BytesToRead;
                byte[] bytesBuffer = new byte[bytesToRead];

                while (bytesToRead > 0)
                {
                    try
                    {
                        if ((bufferOffset > -1) && (bytesToRead - bufferOffset > -1))
                        {
                            int readBytes = sPort.Read(bytesBuffer, bufferOffset, bytesToRead - bufferOffset);
                            bytesToRead -= readBytes;
                            bufferOffset += readBytes;
                        }
                        else
                        {
                            bufferOffset = 0;
                            bytesToRead = 0;
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex.ToString());
                        sPort.DiscardInBuffer();
                        sPort.DiscardOutBuffer();
                        bufferOffset = 0;
                        bytesToRead = 0;
                    }
                }

                RecvDataList.AddRange(bytesBuffer);
            }
        }
        public void Thread_Run_Send()
        {
            while (bWorkThread_tx)
            {
                if (!Global.setTempFlag)
                {
                    if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
                    {
                        ArrayList alReturn = new ArrayList();

                        alReturn.Add((byte)Convert.ToByte(cvm.Ctm.M7num));
                        alReturn.Add((byte)0x03);

                        alReturn.Add((byte)0x00);
                        alReturn.Add((byte)0x01);

                        alReturn.Add((byte)0x00);
                        alReturn.Add((byte)0x08);

                        sendMessage(alReturn);
                        alReturn.Clear();

                        waitRecv();
                    }
                    /*
                    else if (Global.gCommType == "PCLINK")
                    {
                        ArrayList alReturn = new ArrayList();
                        string Send_str = TempNumber[i] + "DRR,05,0001,0002,0005,0017,0018";
                        List<int> ascii = cal.convert_ascii(Send_str);
                        alReturn.Add((byte)STX);

                        cal.checksumData(ascii, alReturn);

                        alReturn.Add((byte)CR);
                        alReturn.Add((byte)LF);
                        sendMessage(alReturn);
                        alReturn.Clear();

                        waitRecv(i);
                    }
                    */
                }
                else
                {
                    Global.sendWaitFlag = true;
                    Thread.Sleep(30);
                }
                //Temp Graph Timer Start
                /*   
                if (!firstWrite)
                    {
                        Global.gTempGraphView.Graphtimer.Start();
                        firstWrite = true;
                    }
                */
            }
        }
        public void sendMessage(ArrayList alToSend)
        {
            lock (LockEvent)
            {
                if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
                {
                    if (alToSend.Count > 0)
                    {
                        byte[] bytesToSend = new byte[alToSend.Count + 2]; // the 2 is for the CRC we'll add at the end
                        String sMessageSent = "";
                        UInt16 crc16 = 0xFFFF;
                        for (int i = 0; i < alToSend.Count; i++)
                        {
                            Byte byteFromArray = (Byte)alToSend[i];
                            bytesToSend[i] = byteFromArray;
                            crc16 = cal.CalculateCRC(byteFromArray, crc16);
                            sMessageSent += bytesToSend[i].ToString("X").PadLeft(2, '0') + " ";
                        }

                        bytesToSend[bytesToSend.Count() - 2] = (Byte)(crc16 % 0x100);
                        sMessageSent += bytesToSend[bytesToSend.Count() - 2].ToString("X").PadLeft(2, '0') + " ";

                        bytesToSend[bytesToSend.Count() - 1] = (Byte)(crc16 / 0x100);
                        sMessageSent += bytesToSend[bytesToSend.Count() - 1].ToString("X").PadLeft(2, '0') + " ";

                        try
                        {
                            if (sPort.IsOpen)
                            {
                                sPort.Write(bytesToSend, 0, bytesToSend.Length);
                                Console.WriteLine("SEND : {0} ", sMessageSent);
                            }
                            sMessageSent = "";
                        }
                        catch (Exception ex)
                        {
                            ;
                        }
                    }
                }
                /*
                else if (Global.gCommType == "PCLINK")
                {
                    string Send_txt = string.Empty;

                    if (alToSend.Count > 0)
                    {
                        byte[] bytesToSend = new byte[alToSend.Count];
                        for (int i = 0; i < alToSend.Count; i++)
                        {
                            Send_txt += Convert.ToChar(alToSend[i]);
                            Byte byteFromArray = (Byte)alToSend[i];
                            bytesToSend[i] = byteFromArray;
                        }
                        try
                        {
                            if (sPort.IsOpen)
                            {
                                sPort.Write(bytesToSend, 0, bytesToSend.Length);

                                Console.WriteLine("SEND : {0} ", Send_txt);
                            }
                        }
                        catch
                        {
                            //Strings = log.writeEventLogLine("[Exception] " + ex.Message); 
                            return;
                        }
                    }
                  
                }
                */
            }
        }
        public void waitRecv()
        {
            if (cvm.Ctm.Protocoltxt == "Modbus-RTU")
            {
                Thread.Sleep(100);
                if (RecvDataList.Count >= 3)
                {
                    String sMessageRecieved = "";
                    bool bCRCPasses = false;
                    UInt16 crc16 = 0xFFFF;
                    byte fun_byte;
                    int packet_length;
                    int data_length;

                    /*
                    if ((RecvDataList[0] < 1) || (RecvDataList[0] > 12))
                    {
                        RecvDataList.Clear();
                        return;
                    }
                    */

                    //RTU 첫번째 바이트 : 통신주소
                    fun_byte = RecvDataList[1];

                    if ((fun_byte != 03) && (fun_byte != 06) && (fun_byte != 16))
                    {
                        RecvDataList.Clear();
                        return;
                    }
                    if (fun_byte == 0x03) //D-Register의 연속읽기      --> 수신 : 8바이트이상
                    {
                        data_length = RecvDataList[2];
                        packet_length = 2 + 1 + data_length + 2;
                        if (packet_length > RecvDataList.Count)
                        {
                            //RecvDataList.RemoveRange(0, packet_length);
                            //sPort.DiscardInBuffer();
                            //sPort.DiscardOutBuffer();
                            return;
                        }
                    }
                    else if (fun_byte == 0x06)
                    {
                        if (RecvDataList.Count == 5)
                            packet_length = 5;
                        else
                            packet_length = 8;
                    }
                    else
                    {
                        packet_length = 1;
                        return;
                    }


                    for (int i = 0; i < packet_length; i++)
                    {
                        sMessageRecieved += RecvDataList[i].ToString("X2").PadLeft(2, '0') + " ";
                        crc16 = cal.CalculateCRC((Byte)RecvDataList[i], crc16);
                    }

                    Console.WriteLine("RECV : {0} ", sMessageRecieved);

                    if (crc16 == 0)
                        bCRCPasses = true;

                    if (bCRCPasses == true)
                    {
                        string funcd = RecvDataList[1].ToString();
                        int index = RecvDataList[0] - 1;

                        if (funcd == "3")
                        {
                            int q = 0;
                            int j = 1;
                            int k = 2;


                            for(int i = 0; i < 4; i++)
                            {
                                nowsetvalue_hex[q] = RecvDataList[j+2].ToString("X2") + RecvDataList[k+2].ToString("X2");     //CH Now Temp HEX
                                nowsetvalue_hex[q + 1] = RecvDataList[j+10].ToString("X2") + RecvDataList[k+10].ToString("X2"); //CH Set Temp HEX

                                nowsetvalue_dec[q] = Convert.ToInt16(nowsetvalue_hex[q], 16);
                                nowsetvalue_dec[q + 1] = Convert.ToInt16(nowsetvalue_hex[q + 1], 16);

                                nowsetvalue_dou[q] = nowsetvalue_dec[q] / 10.0;
                                nowsetvalue_dou[q + 1] = nowsetvalue_dec[q + 1] / 10.0;

                                q += 2;j += 2;k += 2;
                            }

                            /*
                            if (bRunStopFlag[index] && modstsL[1] == '1' && bContactFlag[index])
                            {
                                Global.gTempControlCh.UpadateStatus(uiIndex, subindx, "READY");
                                Global.gTempCHViewModel.Topchs[uiIndex].TempCH[subindx].PrevStat = STAT_TYPE.STOP;
                                bRunStopFlag[index] = false;
                            }
                            else if (!bRunStopFlag[index] && modstsL[1] == '0' && bContactFlag[index])
                            {
                                Global.gTempControlCh.UpadateStatus(uiIndex, subindx, "RUN");
                                Global.gTempCHViewModel.Topchs[uiIndex].TempCH[subindx].PrevStat = STAT_TYPE.RUN;
                                bRunStopFlag[index] = true;
                            }
                            */
                            tcvm.UpdateTemp(nowsetvalue_dou);

                            //RecvDataList.RemoveRange(0, packet_length);
                        }
                        else if(funcd == "6")
                        {
                            int getch = 99;
                            switch(RecvDataList[3].ToString("X2"))
                            {
                                case "3B":
                                    getch= 0;
                                    break;
                                case "3C":
                                    getch= 1;
                                    break;
                                case "3D":
                                    getch= 2;
                                    break;
                                case "3F":
                                    getch= 3;
                                    break;
                            }
                            if(getch != 99)
                            {
                                if (RecvDataList[5].ToString("X2") == "01")
                                    tcvm.TempDatas[getch].TxtStatusValue = "RUNING";
                                else if (RecvDataList[5].ToString("X2") == "00")
                                    tcvm.TempDatas[getch].TxtStatusValue = "READY";
                            }
                        }
                    }

                    RecvDataList.RemoveRange(0, packet_length);
                    sPort.DiscardInBuffer();
                    sPort.DiscardOutBuffer();
                }
            }
        }
    }
}
