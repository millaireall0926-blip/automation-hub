using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.ViewModel;
using ScottPlot;
using ScottPlot.Statistics;
using Serilog;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Threading;

namespace LP_MMTCRack.Net
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
        public void TConDisConnet(bool getval)
        {
            if (getval)
            {
                if (TConPort != null)
                {
                    try
                    {
                        // 디버깅 로그
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"TConPort.IsOpen={TConPort.IsOpen}");

                        TConquit();
                        // 이벤트 핸들러 제거
                        TConPort.DataReceived -= TCONport_DataReceived;

                        // 포트 닫기
                        if (TConPort.IsOpen)
                        {
                            TConPort.DiscardInBuffer();  // 입력 버퍼 비우기
                            TConPort.DiscardOutBuffer(); // 출력 버퍼 비우기
                            TConPort.Close();            // 포트 닫기
                            Global.gMainPageVM.addLog(LOG_TYPE.INFO, "TConPort closed successfully.");
                        }
                    }
                    catch (Exception ex)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"Error closing TConPort: {ex.Message}");
                    }
                    finally
                    {
                        // 리소스 정리
                        TConPort.Dispose();
                        TConPort = null;
                    }
                    /*
                    TConPort.DataReceived -= TCONport_DataReceived; // 이벤트 핸들러 제거
                    if (TConPort.IsOpen)
                    {
                        TConPort.DiscardInBuffer();
                        TConPort.DisㅁcardOutBuffer();
                        TConPort.Close();
                    }
                    TConPort.Dispose();
                    TConPort = null;
                    */
                }
            }
        }
        public void TConquit()
        {
            try
            {
                if (_TconWriteThread?.IsAlive == true)
                {
                    if (!_TconWriteThread.Join(5000))
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "TconWrite thread did not stop gracefully");
                        _TconWriteThread.Abort();
                    }
                }
                if (_TconReadThread.IsAlive)
                    _TconReadThread.Join();
            }
            catch (Exception ex)
    {
        Global.gMainPageVM.addLog(LOG_TYPE.ERROR, 
            $"TConquit error: {ex.Message}");
    }
        }
        public void makeConsoleMsg(byte[] getPacket)
        {
            Array.ForEach(getPacket, num => sb.Append(num.ToString("X2") + " "));
            //
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
        public void ParaReadMsg(byte ch , int func)
        {
			if (TConPort == null || !TConPort.IsOpen) return;

			string groupName = string.Empty;

			switch (func)
			{
				case 0: groupName = "PROCESS"; break;
				case 1: groupName = "FUNCTION"; break;
				case 2: groupName = "SET POINT"; break;
				case 3: groupName = "ALARM"; break;
				case 4: groupName = "PID"; break;
				case 5: groupName = "IN/OUT"; break;
				default: groupName = "PROCESS"; break;
			}

            var targetParams = Global.gEngineerVM.AllParameters.Where(p => p.Group == groupName).OrderBy(p => p.Address).ToList();

            if (targetParams.Count == 0) return;

            ushort startAddr = targetParams.First().Address;
            ushort endAddr = targetParams.Last().Address;
            int totalReadCount = (endAddr - startAddr) + 1;

            int maxRead = 32;
            int currentOffset = 0;

            while(currentOffset < totalReadCount)
            {
                ushort currentStartAddr = (ushort)(startAddr + currentOffset - 1);
                int currentCount = Math.Min(maxRead, totalReadCount - currentOffset);

				Global.gEngineerVM.LastReadStartAddress = (ushort)(startAddr + currentOffset);

				byte addrHi = (byte)((currentStartAddr >> 8) & 0xff);
				byte addrLo = (byte)(currentStartAddr & 0xFF);
				byte countHi = (byte)((currentCount >> 8) & 0xFF);
				byte countLo = (byte)(currentCount & 0xFF);

				var meg = newMsg(ch, 0x03, addrHi, addrLo, countHi, countLo);
				var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));

				if (TConPort != null && TConPort.IsOpen)
				{
					makeConsoleMsg(packet);
					lock (sendQueue)
					{
						sendQueue.Enqueue(packet);
					}
				}

				// 다음 읽을 위치로 이동
				currentOffset += currentCount;

				if (currentOffset < totalReadCount) Thread.Sleep(1000);
			}
		}
        public void SingleWriteMsg(byte ch, TconParameter param)
        {
            if(TConPort == null || !TConPort.IsOpen || param == null || !param.CanWrite) return;

            try
            {
                if (!double.TryParse(param.EditValue, out double val)) return;
                short rawVal = (short)Math.Round(val / param.Scale);

                ushort modbosAddr = (ushort)(param.Address - 1);
                byte addrHi = (byte)((modbosAddr >> 8) & 0xFF);
                byte addrLo = (byte)(modbosAddr & 0xFF);
                byte valHi = (byte)((rawVal >> 8) & 0xFF);
                byte valLo = (byte)(rawVal & 0xFF);

                var meg = newMsg(ch, 0x06, addrHi, addrLo, valHi, valLo);
				var packet = MConv.GetCombined(MConv.StructToByte(meg.sData), MConv.StructToByte(meg.sCrcData));

				lock (sendQueue) sendQueue.Enqueue(packet);

                makeConsoleMsg(packet);

				Global.gEngineerVM.addParaLog($"[WRITE CH {ch}] {param.Symbol} -> {param.EditValue} (Raw: {rawVal})");
			}
			catch (Exception ex)
			{
				Global.gEngineerVM.addParaLog($"Write Error ({param.Symbol}): {ex.Message}");
			}
		}
        public async void TconWriteFunction()
        {
            byte[] seData;

            while (!Global.gMainVM.IsCloseCom)
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
                if(sendQueue.Count > 0)
                {
                    if(Global.gMainVM.ShowEngineer == Visibility.Visible)
                        Global.gEngineerVM.TconWriteCnt = sendQueue.Count;

                    lock (sendQueue)
                    {
                        //Console.WriteLine("sendQueue.Count ====>>>> {0}", sendQueue.Count);
                        if (sendQueue.Count == 0)
                            return;

                        seData = sendQueue.Dequeue();

                        Debug.WriteLine("Tcon Data Write: " + BitConverter.ToString(seData));
                    }
                    RecvData_OK = false;
                    try
                    {
                        TConPort.Write(seData, 0, seData.Length);
                    }
                    catch { }
                    await Task.Delay(80);
                }
                else
                    await Task.Delay(1);
            }
        }
        public async void TconReadFunction()
        {
			bool result = false;
			while (!Global.gMainVM.IsCloseCom)
			{
				int currentCount = 0;

				// 1. 길이를 확인할 때도 아주 짧게 lock을 걸어 안전하게 복사해옴
				lock (RecvDataList)
				{
					currentCount = RecvDataList.Count;
				}

				if (currentCount >= 8) // 최소 패킷 단위(8바이트)가 쌓였을 때만 파싱 시도
				{
					if (Global.gMainVM.ShowEngineer == Visibility.Visible)
						Global.gEngineerVM.TconReadCnt = currentCount;

					if (Global.gMainVM.AllRackSTAT != TOTALRACK_STAT.PARAMETER)
						result = waitRecv();
					else
						result = waitParaRecv();
				}
				else
				{
					// 2. CPU 점유율을 낮추기 위해 Delay 시간을 10ms로 상향 (양산 권장)
					await Task.Delay(10);
				}
			}
		}
        public bool RecvData_OK = false;
        public void TCONport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
			try
			{
				int bytesToRead = TConPort.BytesToRead;
				if (bytesToRead > 0)
				{
					byte[] bytesBuffer = new byte[bytesToRead];
					int readBytes = TConPort.Read(bytesBuffer, 0, bytesToRead);

					// lock을 걸어 멀티스레드 충돌 방지 (매우 중요!)
					lock (RecvDataList)
					{
						RecvDataList.AddRange(bytesBuffer);
					}
				}
			}
			catch (Exception ex)
			{
				// 포트 읽기 중 발생한 에러는 대부분 포트 닫힘이나 케이블 빠짐입니다. 
				// 여기서 버퍼를 날리지 말고 로그만 남깁니다.
				Global.gMainPageVM.addLog(LOG_TYPE.WARNING, "[TCon Recv Error] " + ex.Message);
			}
		}
		public bool waitRecv()
		{
			String sRecieveMsg = string.Empty;
			UInt16 crc16 = 0xFFFF;
			byte fun_byte;
			int packet_length;
			int data_length;

			// 1. 추출한 정상 패킷을 담을 지역 변수 (Lock 밖에서 쓰기 위함)
			byte[] validPacket = null;

			try
			{
				// ==============================================================
				// [임계 영역 시작] 데이터 버퍼 접근: 가장 빠르고 짧게 처리
				// ==============================================================
				lock (RecvDataList)
				{
					if (RecvDataList.Count < 3) return false;

					while (RecvDataList.Count >= 3)
					{
						byte station = RecvDataList[0];
						byte func = RecvDataList[1];

						bool validStation = (station >= 1 && station <= 18);
						bool validFunc = (func == 0x03 || func == 0x06 || func == 0x08 || func == 0x16);

						if (validStation && validFunc) break;
						else RecvDataList.RemoveAt(0);
					}

					if (RecvDataList.Count < 3) return false;

					fun_byte = RecvDataList[1];

					if (fun_byte == 0x03)
					{
						data_length = RecvDataList[2];
						packet_length = 3 + data_length + 2;

						if (packet_length > 255)
						{
							RecvDataList.RemoveAt(0);
							return false;
						}
					}
					else
					{
						packet_length = 8;
					}

					if (packet_length > RecvDataList.Count) return false;

					// CRC 검사
					for (int i = 0; i < packet_length; i++)
					{
						sRecieveMsg += RecvDataList[i].ToString("X2").PadLeft(2, '0') + " ";
						crc16 = MConv.CalculateCRC(RecvDataList[i], crc16);
					}

					// CRC가 정상이면 패킷을 복사(추출)하고 버퍼에서 지움
					if (crc16 == 0)
					{
						// 정상 패킷을 지역 배열에 복사
						validPacket = new byte[packet_length];
						RecvDataList.CopyTo(0, validPacket, 0, packet_length);

						// 버퍼에서는 깔끔하게 삭제
						RecvDataList.RemoveRange(0, packet_length);
					}
					else
					{
						// CRC 에러 시 1바이트 밀어내기
						Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[TCON] CRC Error: {sRecieveMsg}");
						RecvDataList.RemoveAt(0);
					}
				} // lock (RecvDataList) 종료! 
				  // ==============================================================
				  // [임계 영역 끝] 이제 수신 이벤트(DataReceived)는 자유롭게 데이터 적재 가능!
				  // ==============================================================


				// ==============================================================
				// 2. Lock 밖에서 안전하게 파싱 및 UI(ViewModel) 업데이트 수행
				// ==============================================================
				if (validPacket != null)
				{
					int getcnt = Global.gMainPageVM.AllBoxCnt;
					if (getcnt <= 0) getcnt = 1;
					int Box = (validPacket[0] - 1) % getcnt;
					int Rack = (int)((validPacket[0] - 1) / getcnt);
					string funcd = validPacket[1].ToString();

					if (funcd == "3")
					{
						int RecvDataLength = validPacket[2];
						if (RecvDataLength >= 38)
						{
							// 바이메탈 상태 처리
							if ((validPacket[32] & 0x03) == 0x00 && validPacket[32] < 0x04)
							{
								var boxStat = Global.gMainPageVM.Racks[Rack].Boxes[Box].BoxStat;
								if (boxStat == Box_STAT.PRERUN || boxStat == Box_STAT.RUN || boxStat == Box_STAT.DEFROST)
								{
									if (Global.gMainPageVM.Racks[Rack].Boxes[Box].Bimetal_Stat_Cnt < Global.BimetalErrorCnt)
										Global.gMainPageVM.Racks[Rack].Boxes[Box].Bimetal_Stat_Cnt++;
									else
										Global.gMainPageVM.Racks[Rack].Boxes[Box].Bimetal_Stat = false;
								}
								else
								{
									Global.gMainPageVM.Racks[Rack].Boxes[Box].Bimetal_Stat = false;
								}
							}
							else
							{
								Global.gMainPageVM.Racks[Rack].Boxes[Box].Bimetal_Stat_Cnt = 0;
							}

							// Sopen 상태 처리
							if ((validPacket[39] & 0x04) == 0x04 && validPacket[39] < 0x08)
							{
								var boxStat = Global.gMainPageVM.Racks[Rack].Boxes[Box].BoxStat;
								if (boxStat == Box_STAT.PRERUN || boxStat == Box_STAT.RUN || boxStat == Box_STAT.DEFROST)
								{
									if (Global.gMainPageVM.Racks[Rack].Boxes[Box].Sopen_Stat_Cnt < Global.BimetalErrorCnt)
										Global.gMainPageVM.Racks[Rack].Boxes[Box].Sopen_Stat_Cnt++;
									else
										Global.gMainPageVM.Racks[Rack].Boxes[Box].Sopen_Stat = false;
								}
								else
								{
									Global.gMainPageVM.Racks[Rack].Boxes[Box].Sopen_Stat = false;
								}
							}
							else
							{
								Global.gMainPageVM.Racks[Rack].Boxes[Box].Sopen_Stat_Cnt = 0;
							}

							// 온도값 파싱
							string nowvalue_hex = validPacket[3].ToString("X2") + validPacket[4].ToString("X2");
							string setvalue_hex = validPacket[5].ToString("X2") + validPacket[6].ToString("X2");
							string nowstatus = validPacket[21].ToString("X2") + validPacket[22].ToString("X2");

							short nowvalue_dec = Convert.ToInt16(nowvalue_hex, 16);
							short setvalue_dec = Convert.ToInt16(setvalue_hex, 16);

							double nowvalue_dou = nowvalue_dec / 10.0;
							double setvalue_dou = setvalue_dec / 10.0;

							if (nowstatus == "0001")
								Global.gMainPageVM.Racks[Rack].Boxes[Box].GetTconStatus(false);
							else
								Global.gMainPageVM.Racks[Rack].Boxes[Box].GetTconStatus(true);

							Global.gMainPageVM.Racks[Rack].Boxes[Box].GetpvValue(nowvalue_dou);
							Global.gMainPageVM.Racks[Rack].Boxes[Box].GetsvValue(setvalue_dou);

							Debug.WriteLine("RECV <<<<< " + sRecieveMsg);
						}
					}
					else if (funcd == "6")
					{
						string DregisterHL = validPacket[3].ToString("X2");
						if (DregisterHL == "C8")
						{
							string setvalue_hex = validPacket[4].ToString("X2") + validPacket[5].ToString("X2");
							short setvalue_dec = Convert.ToInt16(setvalue_hex, 16);
							double setvalue_dou = setvalue_dec / 10.0;

							Global.gMainPageVM.Racks[Rack].Boxes[Box].GetSV = setvalue_dou;
						}
					}

					RecvData_OK = true;
					return true;
				}
			}
			catch (Exception ex)
			{
				Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[TCON] Parse Exception: {ex.Message}");
				// 에러 시 안전하게 1바이트 밀어내기 (Lock 필수)
				lock (RecvDataList)
				{
					if (RecvDataList.Count > 0) RecvDataList.RemoveAt(0);
				}
				return false;
			}

			return false;
		}
		public bool waitParaRecv()
        {
			String sReceiveMsg = string.Empty;
			UInt16 crc16 = 0xFFFF;
			byte fun_byte;
			int packet_length;
			int data_length;

			// 추출한 정상 패킷을 담을 지역 변수
			byte[] validPacket = null;

			try
			{
				// ==============================================================
				// [임계 영역 시작] 데이터 버퍼 접근: 가장 빠르고 짧게 처리
				// ==============================================================
				lock (RecvDataList)
				{
					if (RecvDataList.Count < 3) return false;

					while (RecvDataList.Count >= 3)
					{
						byte station = RecvDataList[0];
						byte func = RecvDataList[1];

						bool validStation = (station >= 1 && station <= 18);
						bool validFunc = (func == 0x03 || func == 0x06 || func == 0x08 || func == 0x16);

						if (validStation && validFunc) break;
						else RecvDataList.RemoveAt(0);
					}

					if (RecvDataList.Count < 3) return false;

					fun_byte = RecvDataList[1];

					if (fun_byte == 0x03)
					{
						data_length = RecvDataList[2];
						packet_length = 3 + data_length + 2;

						if (packet_length > 255)
						{
							RecvDataList.RemoveAt(0);
							return false;
						}
					}
					else
					{
						packet_length = 8;
					}

					if (packet_length > RecvDataList.Count) return false;

					// CRC 검사
					for (int i = 0; i < packet_length; i++)
					{
						sReceiveMsg += RecvDataList[i].ToString("X2").PadLeft(2, '0') + " ";
						crc16 = MConv.CalculateCRC(RecvDataList[i], crc16);
					}

					// 정상 패킷 추출 및 버퍼 정리
					if (crc16 == 0)
					{
						validPacket = new byte[packet_length];
						RecvDataList.CopyTo(0, validPacket, 0, packet_length);
						RecvDataList.RemoveRange(0, packet_length);
					}
					else
					{
						Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[TCON Para] CRC Error: {sReceiveMsg}");
						RecvDataList.RemoveAt(0);
					}
				} // lock 종료!

				// ==============================================================
				// [임계 영역 끝] 
				// ==============================================================

				// ==============================================================
				// 2. Lock 밖에서 안전하게 파싱 및 UI(ViewModel) 업데이트 수행
				// ==============================================================
				if (validPacket != null)
				{
					int receivedCh = validPacket[0];
					var vm = Global.gEngineerVM;
					fun_byte = validPacket[1]; // fun_byte 다시 확인

					if (fun_byte == 0x03)
					{
						int startAddr = vm.LastReadStartAddress;
						int regCount = validPacket[2] / 2;

						foreach (var param in vm.CurrentParams)
						{
							if (param.Address >= startAddr && param.Address < startAddr + regCount)
							{
								int offset = param.Address - startAddr;
								int dataIdx = 3 + (offset * 2);

								if (dataIdx + 1 < validPacket.Length - 2)
								{
									short rawVal = (short)((validPacket[dataIdx] << 8) | validPacket[dataIdx + 1]);

									if (param.Address == 10 || param.Address == 14 || param.Address == 19)
									{
										param.Value = rawVal.ToString("X4");
									}
									else
									{
										double calcVal = rawVal * param.Scale;
										param.Value = calcVal.ToString("F1");
									}

									if (param.CanWrite) param.EditValue = param.Value;
									else param.EditValue = "-";
								}
							}
						}
						Debug.WriteLine(sReceiveMsg);
						vm.addParaLog($"[RECV CH {receivedCh}] " + sReceiveMsg);
					}
					else if (fun_byte == 0x06)
					{
						vm.addParaLog($"[WRITE SUCCESS CH {receivedCh}] " + sReceiveMsg);
					}
					else
					{
						vm.addParaLog($"[WRITE OK CH {receivedCh}] " + sReceiveMsg);
					}

					RecvData_OK = true;
					return true;
				}
			}
			catch (Exception ex)
			{
				Global.gEngineerVM.addParaLog("Recv Error: " + ex.Message);
				lock (RecvDataList)
				{
					if (RecvDataList.Count > 0) RecvDataList.RemoveAt(0);
				}
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
