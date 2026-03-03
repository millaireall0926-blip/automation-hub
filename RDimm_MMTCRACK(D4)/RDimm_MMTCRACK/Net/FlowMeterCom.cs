using RDimm_MMTCRACK.Base;
using RDimm_MMTCRACK.ViewModel;
using Serilog;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RDimm_MMTCRACK.Net
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
                        Global.gMainVM._isCloseFlow = true;
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
                        Global.gMainVM._isCloseFlow = false;

                        // 플로우 상태를 처리하는 비동기 Task 실행
                        Global.gMainVM._FlowStatTask = Task.Run(() => Global.gMainVM.FlowStatFunction(Global.gMainVM._cancellationTokenSource.Token));
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

            try
            {
                bufferLength = FlowMeterPort.BytesToRead;
            }
            catch
            { }

            byte[] buffer = new byte[bufferLength];
            if (!FlowMeterPort.IsOpen)
                return;

            int data = 0;

            try
            {
                data = FlowMeterPort.Read(buffer, 0, bufferLength);
            }
            catch { }
            
            int checksum = 0;

            // prevStartIndex가 dataInput 배열 크기보다 클 경우 더 이상 진행하지 않음
            if (prevStartIndex >= dataInput.Length)
                return;

            for (int i = 0; i < bufferLength; i++)
            {
                // prevStartIndex가 15를 넘지 않도록 체크
                if (prevStartIndex >= dataInput.Length)
                {
                    // 배열의 크기 이상으로 데이터가 들어오면 배열을 초기화하고 종료
                    Array.Clear(dataInput, 0, dataInput.Length);
                    prevStartIndex = 0;
                    break;
                }

                // 데이터 입력
                dataInput[prevStartIndex] = buffer[i];
                prevStartIndex++;

                // 시작 byte가 0x41이 아닌 경우 초기화
                if (dataInput[0] != 0x41)
                {
                    prevStartIndex = 0;
                    break;
                }

                // 유효한 데이터인 경우
                if (dataInput[0] == 0x41 && dataInput[14] == 0x0a)
                {
                    checksum = (dataInput[0] + dataInput[1] + dataInput[2] + dataInput[3] + dataInput[4] + dataInput[5] + dataInput[6] +
                        dataInput[7] + dataInput[8] + dataInput[9] + dataInput[10] + dataInput[11] + dataInput[12]) & 0xff;

                    // 체크섬이 일치하면 플로우 레이트를 설정
                    if (checksum == dataInput[13])
                    {
                        Global.gMainPageVM.Racks[0].SetFlowRate(Convert.ToDouble((dataInput[1] << 8) + dataInput[2]) / 100);
                        Global.gMainPageVM.Racks[1].SetFlowRate(Convert.ToDouble((dataInput[3] << 8) + dataInput[4]) / 100);
                        Global.gMainPageVM.Racks[2].SetFlowRate(Convert.ToDouble((dataInput[5] << 8) + dataInput[6]) / 100);
                        Global.gMainPageVM.Racks[3].SetFlowRate(Convert.ToDouble((dataInput[7] << 8) + dataInput[8]) / 100);
                        Global.gMainPageVM.Racks[4].SetFlowRate(Convert.ToDouble((dataInput[9] << 8) + dataInput[10]) / 100);
                        Global.gMainPageVM.Racks[5].SetFlowRate(Convert.ToDouble((dataInput[11] << 8) + dataInput[12]) / 100);
                    }

                    // 데이터 처리 완료 후 배열 초기화
                    Array.Clear(dataInput, 0, dataInput.Length);
                    prevStartIndex = 0;
                    break;
                }
            }
        }
    }
}
