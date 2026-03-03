using LP_MMTCRack.Base;
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
    public class FlowMeterCom
    {
        public string FlowMeterName { get; set; } = string.Empty;
        public bool isFlowPortOpened { get; set; } = false;
        public bool GetFlowRecv { get; set; } = true;
        public SerialPort FlowMeterPort;
        public FlowMeterCom()
        {
            Global.gFlowMeterCom = this;
        }
        public void FlowMeterConnect()
        {
            try
            {
                if(FlowMeterPort != null)
                {
                    FlowMeterPort.Close();
                    FlowMeterPort.Dispose();
                    Thread.Sleep(100);
                }

                FlowMeterPort = new SerialPort();
                FlowMeterPort.PortName = FlowMeterName;
                FlowMeterPort.BaudRate = 9600;
                FlowMeterPort.DataBits = 8;
                FlowMeterPort.StopBits = StopBits.One;
                FlowMeterPort.Parity = Parity.None;
                FlowMeterPort.DataReceived += new SerialDataReceivedEventHandler(GetFlowMeter_Received);
                FlowMeterPort.Open();
                isFlowPortOpened = true;
                Global.gMainPageVM.addLog(LOG_TYPE.INFO, "FlowPort Connect");
            }
            catch (Exception ex)
            {
                isFlowPortOpened = false;
                FlowMeterPort.Close();
                FlowMeterPort.Dispose();
                string msg = "[FlowMeterPort exception] " + ex.Message;
                Global.gMainPageVM.addLog(LOG_TYPE.WARNING, msg);
            }
        }
        public void FlowMeterDiConnect()
        {
            if (FlowMeterPort != null)
            {
                FlowMeterPort.Close();
                FlowMeterPort.Dispose();
            }
        }
        public void FlowMeterWrite()
        {
            try
            {
                if (FlowMeterPort == null)
                    return;
                if( FlowMeterPort.IsOpen)
                {
                    if(isFlowPortOpened)
                    {
                        byte[] txData = { 0x41, 0x41, 0x0a };
                        FlowMeterPort.Write(txData, 0, txData.Length);
                    }
                }
                else
                {
                    if(isFlowPortOpened)
                    {
                        isFlowPortOpened = false;
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "유량계 연결이 종료되었습니다.!");
                        Global.gMainVM.IsCloseFlow = true;
                        FlowMeterPort.Close();
                    }
                    try
                    {
                        if(FlowMeterName != "")
                            FlowMeterPort.Open();
                    }
                    catch
                    {

                    }
                    if(FlowMeterPort.IsOpen)
                    {
                        isFlowPortOpened = true;
                        Global.gMainPageVM.addLog(LOG_TYPE.INFO, "유량계가 연결되었습니다.!");
                        Global.gMainVM.IsCloseFlow = false;
                        // 플로우 상태를 처리하는 비동기 Task 실행
                        Global.gMainVM._flowStatTask = Task.Run(() => Global.gMainVM.FlowStatFunctionAsync(Global.gMainVM._cancellationTokenSource.Token));
                    }
                }
            }
            catch
            {

            }
        }
        int prevStartIndex = 0;
        byte[] dataInput = new byte[15];
        private void GetFlowMeter_Received(object sender, SerialDataReceivedEventArgs e)
        {
            int bufferLength = 0;
            Thread.Sleep(100);
            try
            {
                bufferLength = FlowMeterPort.BytesToRead;
            }
            catch
            {
                // BytesToRead 오류 발생 시 처리 (필요하면 로그 추가)
            }

            byte[] buffer = new byte[bufferLength];

            try
            {
                // 실제 읽은 바이트 수는 data 변수에 저장됨.
                int data = FlowMeterPort.Read(buffer, 0, bufferLength);
            }
            catch
            {
                // Read 실패 시 처리
            }

            int checksum = 0;

            // 만약 prevStartIndex가 이미 dataInput 배열 크기를 초과했다면, 리셋
            if (prevStartIndex >= dataInput.Length)
            {
                prevStartIndex = 0;
                return;
            }

            // 수신된 버퍼를 dataInput 배열에 누적 (인덱스 초과 검사 포함)
            for (int i = 0; i < bufferLength; i++)
            {
                // 만약 배열의 끝에 도달했다면, 데이터를 처리할 수 없으므로 리셋 후 종료
                if (prevStartIndex >= dataInput.Length)
                {
                    // 배열 범위를 초과하였으므로 버퍼를 리셋합니다.
                    Array.Clear(dataInput, 0, dataInput.Length);
                    prevStartIndex = 0;
                    break;
                }

                dataInput[prevStartIndex] = buffer[i];
                prevStartIndex++;

                // 헤더 검사: 첫 바이트가 0x41이 아니면 버퍼 리셋
                if (dataInput[0] != 0x41)
                {
                    Array.Clear(dataInput, 0, dataInput.Length);
                    prevStartIndex = 0;
                    break;
                }

                // 충분한 데이터가 모였는지 확인 (최소 15바이트 필요)
                if (prevStartIndex >= 15)
                {
                    // 종료 바이트 검사: dataInput[14]가 0x0A인지 확인
                    if (dataInput[14] == 0x0A)
                    {
                        // 체크섬 계산 (dataInput[0] ~ dataInput[12]의 합을 & 0xff)
                        checksum = (dataInput[0] + dataInput[1] + dataInput[2] + dataInput[3] + dataInput[4] +
                                    dataInput[5] + dataInput[6] + dataInput[7] + dataInput[8] +
                                    dataInput[9] + dataInput[10] + dataInput[11] + dataInput[12]) & 0xff;

                        // 체크섬이 일치하면 데이터 처리
                        if (checksum == dataInput[13])
                        {
                            Global.gMainPageVM.Racks[0].SetFlowRate(Convert.ToDouble((dataInput[1] << 8) + dataInput[2]) / 100);
                            Global.gMainPageVM.Racks[1].SetFlowRate(Convert.ToDouble((dataInput[3] << 8) + dataInput[4]) / 100);
                            Global.gMainPageVM.Racks[2].SetFlowRate(Convert.ToDouble((dataInput[5] << 8) + dataInput[6]) / 100);
                            Global.gMainPageVM.Racks[3].SetFlowRate(Convert.ToDouble((dataInput[7] << 8) + dataInput[8]) / 100);
                            Global.gMainPageVM.Racks[4].SetFlowRate(Convert.ToDouble((dataInput[9] << 8) + dataInput[10]) / 100);
                            Global.gMainPageVM.Racks[5].SetFlowRate(Convert.ToDouble((dataInput[11] << 8) + dataInput[12]) / 100);
                        }

                        // 데이터 처리 후 버퍼 리셋
                        Array.Clear(dataInput, 0, dataInput.Length);
                        prevStartIndex = 0;
                        break;
                    }
                }
            }
        }

        /*
        private void GetFlowMeter_Received(object sender, SerialDataReceivedEventArgs e)
        {
            int bufferLength = 0;
            Thread.Sleep(100);
            try
            {
                bufferLength = FlowMeterPort.BytesToRead;
            }
            catch
            { }
            byte[] buffer = new byte[bufferLength];

            int data = 0;

            try
            {
                data = FlowMeterPort.Read(buffer, 0, bufferLength);
            }
            catch { }
                
            int checksum = 0;

            if (prevStartIndex > 15)
                return;

            for (int i = 0; i < bufferLength; i++)
            {
                dataInput[prevStartIndex] = buffer[i];
                prevStartIndex++;

                if (dataInput[0] != 0x41)
                {
                    prevStartIndex = 0;
                    break;
                }

                if (dataInput[0] == 0x41 && dataInput[14] == 0x0a)
                {
                    checksum = (dataInput[0] + dataInput[1] + dataInput[2] + dataInput[3] + dataInput[4] + dataInput[5] + dataInput[6] +
                        dataInput[7] + dataInput[8] + dataInput[9] + dataInput[10] + dataInput[11] + dataInput[12]) & 0xff;

                    if (checksum == dataInput[13])
                    {
                        Global.gMainPageVM.Racks[0].SetFlowRate( Convert.ToDouble((dataInput[1] << 8) + dataInput[2]) / 100);
                        Global.gMainPageVM.Racks[1].SetFlowRate( Convert.ToDouble((dataInput[3] << 8) + dataInput[4]) / 100);
                        Global.gMainPageVM.Racks[2].SetFlowRate( Convert.ToDouble((dataInput[5] << 8) + dataInput[6]) / 100);
                        Global.gMainPageVM.Racks[3].SetFlowRate( Convert.ToDouble((dataInput[7] << 8) + dataInput[8]) / 100);
                        Global.gMainPageVM.Racks[4].SetFlowRate( Convert.ToDouble((dataInput[9] << 8) + dataInput[10]) / 100);
                        Global.gMainPageVM.Racks[5].SetFlowRate( Convert.ToDouble((dataInput[11] << 8) + dataInput[12]) / 100);
    }

    Array.Clear(dataInput, 0, dataInput.Length);
                    prevStartIndex = 0;
                    break;
                }
            }
        }
        */
    }
}
