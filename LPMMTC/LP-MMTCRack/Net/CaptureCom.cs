using LP_MMTCRack.Base;
using LP_MMTCRack.Model;
using LP_MMTCRack.ViewModel;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;

namespace LP_MMTCRack.Net
{
    public class CaptureCom
    {
        public int CaputureCh { get; set; } = 0;
        public string CaputurePortName { get; set; } = string.Empty;
        public string UsbHWPort { get; set; } = string.Empty;
        public string UsbCaptureNameing { get; set; } = string.Empty;
        public bool CaputureUse { get; set; } = false;

        // 예상되는 데이터 길이에 맞춘 배열 (예: 12바이트)
        public byte[] outData = new byte[12];
        public SerialPort CapturePort { get; private set; }

        // DataReceived 이벤트에서 누적되는 데이터를 위한 내부 버퍼 (쓰레드 안전을 위해 lock 사용)
        private List<byte> _recvBufferList = new List<byte>();

        public CaptureCom()
        {
            Global.gCaptureCom = this;
        }

        #region Connection / Disconnection

        public void Connect(int ch)
        {
            try
            {
                ResetBoxData();

                if (CapturePort != null)
                {
                    CapturePort.DataReceived -= Capture_DataReceivedHandler;
                    if (CapturePort.IsOpen)
                        CapturePort.Close();
                    CapturePort.Dispose();
                    CapturePort = null;
                    
                    Thread.Sleep(100);
                }

                CaputureCh = ch;

                if (string.IsNullOrEmpty(CaputurePortName))
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CapturePort {CaputureCh + 1} Com Port Name Empty");
                    return;
                }

                CapturePort = new SerialPort
                {
                    PortName = CaputurePortName,
                    BaudRate = 9600,
                    DataBits = 8,
                    Parity = Parity.None,
                    StopBits = StopBits.One
                };

                // DataReceived 이벤트 핸들러 등록
                CapturePort.DataReceived += Capture_DataReceivedHandler;
                CapturePort.Open();

                // 내부 수신 버퍼 초기화
                lock (_recvBufferList)
                {
                    _recvBufferList.Clear();
                }

                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"CapturePort {CaputureCh + 1} Usb Info Match Connect");
            }
            catch (Exception ex)
            {
                try { CapturePort?.Close(); CapturePort?.Dispose(); } catch { }
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[CapturePort {CaputureCh + 1} Exception] {ex.Message}");
            }
        }

        public void Disconnect()
        {
            try
            {
                ResetBoxData();

                if (CapturePort != null)
                {
                    CapturePort.DataReceived -= Capture_DataReceivedHandler;
                    if (CapturePort.IsOpen)
                        CapturePort.Close();
                    CapturePort.Dispose();
                    CapturePort = null;
                    
                }
				// 내부 수신 버퍼 초기화
				lock (_recvBufferList)
				{
					_recvBufferList.Clear();
				}
			}
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[CapturePort {CaputureCh + 1} Disconnect Exception] {ex.Message}");
            }
        }

        private void ResetBoxData()
        {
            foreach (BoxModel bm in Global.gMainPageVM.Racks[CaputureCh].Boxes)
            {
                bm.BoxPV = 0;
                bm.BoxPVColor = Global.gWarningColors[0];
                bm.BoxPVText = string.Empty;
            }
        }

		#endregion

		#region Write and Async Read Operations

		/// <summary>
		/// 명령(0x13)을 전송한 후, 비동기적으로 데이터를 읽어와 처리하는 메서드.
		/// 포트가 닫혀 있으면 재연결을 시도합니다.
		/// </summary>

		private int _noResponseCount = 0;
		private const int MAX_RECONNECT_ATTEMPTS = 4;
		public async Task WriteAndProcessAsync(CancellationToken cancellationToken)
        {

            if (CapturePort == null || !CapturePort.IsOpen)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"CapturePort {CaputureCh + 1} is not open, attempting reconnect.");
                Connect(CaputureCh);
                if (CapturePort == null || !CapturePort.IsOpen)
                {
                    Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"CapturePort {CaputureCh + 1} failed to open after reconnect.");
                    return;
                }
            }

            byte[] command = new byte[] { 0x13 };
            try
            {
                CapturePort.Write(command, 0, command.Length);
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[CaptureWrite] Write Exception: {ex.Message}");
                return;
            }

            // 센서 응답을 위한 충분한 대기 시간 (300ms)
            await Task.Delay(300, cancellationToken);

            RackModel rm = Global.gMainPageVM.getRack(CaputureCh);
            Array.Clear(outData, 0, outData.Length);

            int bytesRead = await ReadCaptureDataAsync(cancellationToken);
            Console.WriteLine($"<<<<<< Capture Data Read ({bytesRead} bytes): {BitConverter.ToString(outData)}");

            //Read 결과 판단
            if(bytesRead <= 0)
            {
				if (_noResponseCount < 6)
				{
					_noResponseCount++;
				}

				if (_noResponseCount <= MAX_RECONNECT_ATTEMPTS)
                {
                    Disconnect();
                }
                else if(_noResponseCount == MAX_RECONNECT_ATTEMPTS + 1)
                {
					Global.gMainPageVM.addLog(LOG_TYPE.ERROR,
					$"[Capture {CaputureCh + 1}] No Response 5 times. Stop reconnecting. Check sensor/cable.");
				}

                return;
            }

			_noResponseCount = 0;

			ProcessReceivedData(rm);
        }

        /// <summary>
        /// CapturePort의 BaseStream을 이용하여 비동기적으로 데이터를 읽어옵니다.
        /// 내부 버퍼(_recvBufferList)를 사용하여 충분한 데이터가 모이면 outData를 업데이트합니다.
        /// </summary>
        private async Task<int> ReadCaptureDataAsync(CancellationToken cancellationToken)
        {
            if (CapturePort == null || !CapturePort.IsOpen)
                return 0;

            int totalRead = 0;
            int expectedBytes = outData.Length;

            try
            {
                // BaseStream의 ReadAsync를 이용하여 데이터를 읽는 대신,
                // DataReceived 이벤트에서 _recvBufferList가 업데이트되므로
                // 일정 시간 동안 대기 후 내부 버퍼에서 데이터를 복사하는 방식으로 처리합니다.
                int waitTimeMs = 0;
                while (waitTimeMs < 1000) // 최대 1초 대기
                {
                    cancellationToken.ThrowIfCancellationRequested();
                    lock (_recvBufferList)
                    {
                        if (_recvBufferList.Count >= expectedBytes)
                        {
                            // 예상 바이트 수 이상 데이터가 모이면, 앞쪽 expectedBytes 만큼 복사
                            _recvBufferList.CopyTo(0, outData, 0, expectedBytes);
                            totalRead = expectedBytes;
                            // 남은 데이터가 있다면 제거, 아니면 전체 클리어
                            _recvBufferList.RemoveRange(0, expectedBytes);
                            break;
                        }
                    }
                    await Task.Delay(10, cancellationToken);
                    waitTimeMs += 10;
                }
            }
            catch (OperationCanceledException)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, $"[ReadCaptureDataAsync] Operation canceled.");
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[ReadCaptureDataAsync] Exception: {ex.Message}");
            }

            return totalRead;
        }

        /// <summary>
        /// 수신된 outData를 기반으로 온도 데이터를 계산하고, 각 BoxModel에 전달합니다.
        /// </summary>
        private void ProcessReceivedData(RackModel rm)
        {
            if (Global.gMainPageVM.AllBoxCnt == 3)
            {
                if (outData.Length >= 3 && outData[0] == 0x01)
                {
                    rm.Boxes[0].preTempData();
                    rm.Boxes[0].GetBoxPV(ReadDimmTemperature(outData[1], outData[2]));
                }
                if (outData.Length >= 7 && outData[4] == 0x03)
                {
                    rm.Boxes[2].preTempData();
                    rm.Boxes[2].GetBoxPV(ReadDimmTemperature(outData[5], outData[6]));
                }
                if (outData.Length >= 11 && outData[8] == 0x02)
                {
                    rm.Boxes[1].preTempData();
                    rm.Boxes[1].GetBoxPV(ReadDimmTemperature(outData[9], outData[10]));
                }
            }
            else if (Global.gMainPageVM.AllBoxCnt == 2)
            {
                if (outData.Length >= 3 && outData[0] == 0x01)
                {
                    rm.Boxes[0].preTempData();
                    rm.Boxes[0].GetBoxPV(ReadDimmTemperature(outData[1], outData[2]));
                }
                if (outData.Length >= 11 && outData[8] == 0x02)
                {
                    rm.Boxes[1].preTempData();
                    rm.Boxes[1].GetBoxPV(ReadDimmTemperature(outData[9], outData[10]));
                }
            }
        }

        /// <summary>
        /// 두 바이트 데이터를 온도(double)로 계산합니다.
        /// </summary>
        public double ReadDimmTemperature(byte data1, byte data2)
        {
            double temp = 0;
            if ((data1 & 0x10) != 0)
            {
                int code = (data1 << 8) | data2;
                code = (~code) & 0x0FFF;
                temp = -1.0 * (code + 1) * 0.0625;
            }
            else
            {
                int code = (data1 << 8) | data2;
                code = code & 0x0FFF;
                temp = code * 0.0625;
            }
            return temp;
        }

        #endregion

        #region DataReceived Event Handler

        /// <summary>
        /// SerialPort의 DataReceived 이벤트 핸들러.
        /// 수신된 데이터를 내부 버퍼(_recvBufferList)에 누적하며, 비동기 읽기에서 사용할 수 있도록 합니다.
        /// </summary>
        public void Capture_DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int inputLength = CapturePort.BytesToRead;
                if (inputLength > 0)
                {
                    byte[] recvData = new byte[inputLength];
                    CapturePort.Read(recvData, 0, inputLength);

                    lock (_recvBufferList)
                    {
                        _recvBufferList.AddRange(recvData);
                    }

                    //Console.WriteLine($"[Capture_DataReceivedHandler] Data Received: {BitConverter.ToString(recvData)}");
                }
            }
            catch (Exception ex)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, $"[Capture_DataReceivedHandler] Exception: {ex.Message}");
            }
        }

        #endregion
    }
}
