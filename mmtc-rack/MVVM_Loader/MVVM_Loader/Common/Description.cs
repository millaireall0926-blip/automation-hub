using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM_Loader.Common
{
    public class Description
    {
        public Dictionary<string, string> infoDic;
        public Description()
        {
            Global.gDescription = this;
            initDic();
        }
        private void initDic()
        {
            infoDic = new Dictionary<string, string>();

            infoDic.Add("입력 종류 설정", " 01 ~ 36\r\n 01 : [K] -200 ~ 1370 °C\r\n 02 : [K] -199.9 ~ 999.9 °C\r\n 주의) 입력 종류를 변경하면 모든 파라메터가 초기화 됩니다.\r\n");
            infoDic.Add("입력 단위 설정", " 0 : 섭씨 [℃]\r\n 1: 화씨 [℉]\r\n");
            infoDic.Add("입력 하한 설정", " EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("입력 상한 설정", " EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("프리스케일 하한", " -1999 ~ (SL-H)-1\r\n");
            infoDic.Add("프리스케일 상한", " (SL-L)+1 ~ 9999\r\n");
            infoDic.Add("출력 포트", " 포트고정[---]: (00) OFF, (01) OUT1, (02) OUT2, (03) RET, (04) ALM\r\n 포트변경[CH1]: (10)OFF, (11)OUT1, (12)OUT2, (13)RET, (14)ALM(P1)\r\n 포트변경[CH2]: (20)OFF, (21)OUT1, (22)OUT2, (23)RET, (24)ALM(P1)\r\n 포트변경[CH3]: (30)OFF, (31)OUT1, (32)OUT2, (33)RET, (34)ALM(P1)\r\n 포트변경[CH4]: (40)OFF, (41)OUT1, (42)OUT2, (43)RET, (44)ALM(P1)\r\n");
            infoDic.Add("출력 동작 선택", " 0 : 역(가열)동작 제어 모드\r\n 1 : 정(냉각)동작 제어 모드\r\n 2 : 가열냉각 동시제어 모드\r\n");
            infoDic.Add("출력1 종류 선택(가열 제어출력)", " 1 : SSR ( Voltage-Pulse ) 모드\r\n 2 : SCR ( 4 ~ 20mA ) 모드\r\n 3 : RELAY 제어 모드\r\n");
            infoDic.Add("출력2 종류 선택(냉각 제어출력)", " 1 : SSR ( Voltage-Pulse ) 모드\r\n 2 : SCR ( 4 ~ 20mA ) 모드\r\n 3 : RELAY 제어 모드\r\n");
            infoDic.Add("전류출력(SCR) 선택시 전류 범위", " Bit3: CH4, Bit2: CH3, Bit1: CH2, Bit0: CH1\r\n Bit3 ~ 0 ［SET(1) 0 ~ 20mA Output］\rn");
            infoDic.Add("통신 중지 시 제어상태 설정", " 0 : OFF\r\n 1 ~ 3600 sec\r\n");
            infoDic.Add("외부 입.출력 설정", " HI/Byte : FFH(COM), F0H(I/O), 00H(OFF)\r\n Lo/Byte : b7~4(Output Type), b3~0(I/O)\r\n b 7 ~ 4 : Alarm(1) / Control Output(0)\r\n b 3 ~ 0 : Input(1) / Output(0)\r\n");

            infoDic.Add("채널모드 설정", " 0 : 비 활성 모드\r\n 1 : 모니터링 모드\r\n 2 : 운전 모드\r\n");
            infoDic.Add("정전복귀 설정", " 0 : 정전후 무조건 정지\r\n 1 : 운전상태 복귀\r\n");
            infoDic.Add("전 채널 정지", " 0 : 해제 (Release)\r\n 1 : 정지 (ALL-Stop) \r\n");
            infoDic.Add("운전 / 정지", "  0 : 정지 (STOP)\r\n 1 : 운전 (RUN) \r\n");
            infoDic.Add("수동제어 모드", " 0 : 자동제어 (PID or ON/OFF)\r\n 1 : 수동제어 (수동 출력제어)\r\n");
            infoDic.Add("MVn 출력(수동)", " 가열 또는 냉각 제어 : 0.0 ~ 100.0 [%]\r\n 가열 냉각 동시제어 : -100.0 ~ 100.0 [%]\r\n");
            infoDic.Add("입력 단선시 출력", " 0 : OFF \r\n 1 : ON \r\n");
            infoDic.Add("Fast mode", " 0 : OFF \r\n 1 : ON \r\n");
            infoDic.Add("접점입력 종류", " 0 : OFF \r\n 1 : 목표 SV 선택 모드 \r\n 2 : 운전 제어 모드 \r\n 3 : 정제 제어 모드 \r\n 4 : 자동/수동 제어 모드 \r\n 5 : 경보 리셋 모드 \r\n");
            infoDic.Add("목표 SV 선택", " 0 ~ 3\r\n");
            infoDic.Add("목표설정 SV0", " EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("목표설정 SV1", " EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("목표설정 SV2", " EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("목표설정 SV3", " EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("오토튜닝 종류", " 0 : 표준 \r\n 1 : Low(10%) Tuning \r\n");
            infoDic.Add("경보래치 설정", " 0 : OFF \r\n 1 : CH1 Latch Mode \r\n 2 : CH2 Latch Mode \r\n 3 : Ch3 Latch Mode \r\n 4 : CH4 Latch Mode \r\n 5 : ALL-Latch Mdoe \r\n");
            infoDic.Add("래치모드시 경보해제", " 0 : OFF \r\n 1 : 경보해제 \r\n");
            infoDic.Add("경보출력 제어(AxTY N0.21)", " bit0: 경보2 출력 직접제어 (0:OFF,1:ON)\r\n bit1: 경보3 출력 직접제어 (0:OFF,1:ON)\r\n bit2: 경보4 출력 직접제어 (0:OFF,1:ON)\r\n bit3: Run/Stop 제어 신호를 AL2로 출력\r\n bit4: Run/Stop 제어 신호를 반전시킴\r\n");
            infoDic.Add("경보시 운전정지", " 0 : OFF\r\n 1 : 경보 발생시 운전 정지\r\n");
            infoDic.Add("경보1 종류 설정", " 1 : OFF                 7 : 하한편차(역접)         13 : 상한편차(정접,홀드)        19 : 상한절대(역접,홀드)\r\n 2 : 상한절대(정접)   8 : 상.하한편차             14 : 하한편차(정접,홀드)        20 : 하한절대(역접,홀드)\r\n 3 : 하한절대(정접)   9 : 상.하한편차 범위내    15 : 상한편차(역접,홀드)        21 : 히터단선경보(HBA) OFF시 \r\n 4 : 상한편차(정접)   10 : 상한절대(역접)        16 : 하한편차(역접,홀드)             제어루프단선경보(LBA)동작\r\n 5 : 하한편차(정접)   11 : 상한절대(정접,홀드)  17 : 상.하한편차(홀드)          22 : HBA, HOC, LBA 역 접점 출력 \r\n 6 : 상한편차(역접)   12 : 하한절대(정접,홀드)  18 : 상.하한편차범위내(홀드)      \r\n");
            infoDic.Add("경보2 종류 설정", " 설정코드1 ~ 20 까지 경보1 코드와  동일하고, 21번은 경보출력 직접 제  어모드임 ( RLY.CTL 로 제어가능! )\r\n\r\n");
            infoDic.Add("경보3 종류 설정", " 설정코드1 ~ 20 까지 경보1 코드와  동일하고, 21번은 경보출력 직접 제  어모드임 ( RLY.CTL 로 제어가능! )\r\n\r\n");
            infoDic.Add("경보4 종류 설정", " 설정코드1 ~ 20 까지 경보1 코드와  동일하고, 21번은 경보출력 직접 제  어모드임 ( RLY.CTL 로 제어가능! )\r\n\r\n");
            infoDic.Add("경보1 데드밴드", " EUS ( 0 ~ 10 % )\r\n");
            infoDic.Add("경보2 데드밴드", " EUS ( 0 ~ 10 % )\r\n");
            infoDic.Add("경보3 데드밴드", " EUS ( 0 ~ 10 % )\r\n");
            infoDic.Add("경보4 데드밴드", " EUS ( 0 ~ 10 % )\r\n");
            infoDic.Add("경보1 값 설정", " EUS ( -100 ~ 100 % )\r\n");
            infoDic.Add("경보2 값 설정", " EUS ( -100 ~ 100 % )\r\n");
            infoDic.Add("경보3 값 설정", " EUS ( -100 ~ 100 % )\r\n");
            infoDic.Add("경보4 값 설정", " EUS ( -100 ~ 100 % )\r\n");
            infoDic.Add("경보1 출력포트", " 경보가 출력될 포트를 선택한다\r\n 0 : OFF \r\n 1 : ALARM1 OUTPUT PORT \r\n 2 : ALARM2 OUTPUT PORT \r\n 3 : ALARM3 OUTPUT PORT \r\n 4 : ALARM4 OUTPUT PORT \r\n");
            infoDic.Add("경보2 출력포트", " 경보가 출력될 포트를 선택한다\r\n 0 : OFF \r\n 1 : ALARM1 OUTPUT PORT \r\n 2 : ALARM2 OUTPUT PORT \r\n 3 : ALARM3 OUTPUT PORT \r\n 4 : ALARM4 OUTPUT PORT \r\n");
            infoDic.Add("경보3 출력포트", " 경보가 출력될 포트를 선택한다\r\n 0 : OFF \r\n 1 : ALARM1 OUTPUT PORT \r\n 2 : ALARM2 OUTPUT PORT \r\n 3 : ALARM3 OUTPUT PORT \r\n 4 : ALARM4 OUTPUT PORT \r\n");
            infoDic.Add("경보4 출력포트", " 경보가 출력될 포트를 선택한다\r\n 0 : OFF \r\n 1 : ALARM1 OUTPUT PORT \r\n 2 : ALARM2 OUTPUT PORT \r\n 3 : ALARM3 OUTPUT PORT \r\n 4 : ALARM4 OUTPUT PORT \r\n");
            infoDic.Add("LBA 시간 값 설정", " 0 : OFF \r\n 1 ~ 7200 sec \r\n");
            infoDic.Add("LBW 변화 폭 설정", " EUS ( 0 ~ 100 % )\r\n");
            infoDic.Add("히터단선경보 출력의 전류설정", " 0 : OFF\r\n 0.1 ~ 38.0 A \r\n");
            infoDic.Add("히터 과전류 설정", " 0 : OFF\r\n 0.1 ~ 38.0 A \r\n");
            infoDic.Add("히터단선 경보출력의 히스테리시스 설정", " 0.0 ~ 30.0 A\r\n");
            infoDic.Add("히터단선 시작점", " 0 ~ 100 % \r\n");
            infoDic.Add("CT Auto Bias Set", " 0 : OFF \r\n 1 : ON \r\n");
            infoDic.Add("히터전류값 홀드", " 0 : OFF \r\n 1 : ON \r\n");
            infoDic.Add("비례대 설정", " 0 : ON/OFF 제어 \r\n 0.1 ~ 999.9 ℃ : PID 제어 \r\n");
            infoDic.Add("적분시간 설정", " 0 : OFF \r\n 1 ~ 3600 sec \r\n");
            infoDic.Add("미분시간 설정", " 0 : OFF \r\n 1 ~ 3600 sec \r\n");
            infoDic.Add("수동리셋 설정", " -5.0 ~ 105.0 % \r\n");
            infoDic.Add("가열냉각 불감대", " -100.0 ~ 100.0 % \r\n");
            infoDic.Add("제어 안정도", " 오토튜닝(A.T) 후, 수렴 시 제어 안정도를 개선함 \r\n 온도 헌팅 시 값을 증가 (1~10) 시키면 안정도가 좋아짐");
            infoDic.Add("램프제어 적분보상", " 0 ~ 100 % \r\n");
            infoDic.Add("RAMP 제어시 시간 단위 설정", " 0 : MINUTE \r\n 1 : HOUR \r\n");
            infoDic.Add("RAMP 제어시 초기 상승온도 설정", " 0 : OFF \r\n EUS ( 0 ~ 100 % ) \r\n");
            infoDic.Add("RAMP 제어시 초기 하강온도 설정", " 0 : OFF \r\n EUS ( 0 ~ 100 % ) \r\n");
            infoDic.Add("전송출력 종류", " 0 : PV \r\n 1 : SV \r\n 2 : MV \r\n");
            infoDic.Add("전송출력 하한", " TC or RTD : FR-L ~ FR-H \r\n 전류,전압 : SL-L ~ SL-H \r\n 단, RET.L < RET.H만 설정가능 \r\n");
            infoDic.Add("전송출력 상한", " TC or RTD : FR-L ~ FR-H \r\n 전류,전압 : SL-L ~ SL-H \r\n 단, RET.L < RET.H만 설정가능 \r\n");
            infoDic.Add("제어출력 주기 설정", " 1 ~ 100 sec \r\n");
            infoDic.Add("냉각측 제어출력 주기 설정", " 1 ~ 100 sec \r\n");
            infoDic.Add("냉각게인 설정", " 0.1 ~ 10.0 \r\n");
            infoDic.Add("히스테리시스 동작", " 0 : NORMAL \r\n 1 : HALF \r\n");
            infoDic.Add("히스테리시스 설정", " EUS ( 0 ~ 10 % ) \r\n");
            infoDic.Add("입력 단선시 출력량 설정", " -5.0 ~ 105.0 % \r\n");
            infoDic.Add("입력 단선시 냉각측 출력량 설정", " -5.0 ~ 105.0 % \r\n");
            infoDic.Add("출력량 하한제한", " -5.0 ~ OL-H % \r\n");
            infoDic.Add("출력량 상한제한", " OL-L ~ 105.0 % \r\n");
            infoDic.Add("소수점 선택", " INP.30 ~ 36 일때 \r\n 소수점 위치 선택정보 *0/1/2/3) \r\n");
            infoDic.Add("입력 필터 설정", " 0 : OFF\r\n 1 ~ 120 sec \r\n");
            infoDic.Add("입력 하한 보정(INP 1~35)", " EUS ( -100 ~ 100 % )\r\n");
            infoDic.Add("입력 상한 보정(INP 30~35)", " EUS ( -100 ~ 100 % )\r\n");
            infoDic.Add("입력 구간보정기능 ON/OFF", " 0 : OFF \r\n 1 : ON \r\n");
            infoDic.Add("입력 구간 설정 1", " (SLL)\t(SLH)\r\n FRL ≤ P1 ≤ P2 ≤ P3 ≤ FRH\r\n 설정범위 : EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("입력 구간 설정 2", " (SLL)\t(SLH)\r\n FRL ≤ P1 ≤ P2 ≤ P3 ≤ FRH\r\n 설정범위 : EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("입력 구간 설정 3", " (SLL)\t(SLH)\r\n FRL ≤ P1 ≤ P2 ≤ P3 ≤ FRH\r\n 설정범위 : EU ( 0 ~ 100 % )\r\n");
            infoDic.Add("입력 바이어스 0", " (SLL)\t(SLH)\r\n FRL――P1――P2――P3――FR\r\n H  BS0\tBS1\tBS2\tBS3\tBS4\r\n \r\n 설정범위 : EUS ( ±30 % )\r\n");
            infoDic.Add("입력 바이어스 1", " (SLL)\t(SLH)\r\n FRL――P1――P2――P3――FR\r\n H  BS0\tBS1\tBS2\tBS3\tBS4\r\n \r\n 설정범위 : EUS ( ±30 % )\r\n");
            infoDic.Add("입력 바이어스 2", " (SLL)\t(SLH)\r\n FRL――P1――P2――P3――FR\r\n H  BS0\tBS1\tBS2\tBS3\tBS4\r\n \r\n 설정범위 : EUS ( ±30 % )\r\n");
            infoDic.Add("입력 바이어스 3", " (SLL)\t(SLH)\r\n FRL――P1――P2――P3――FR\r\n H  BS0\tBS1\tBS2\tBS3\tBS4\r\n \r\n 설정범위 : EUS ( ±30 % )\r\n");
            infoDic.Add("입력 바이어스 4", " (SLL)\t(SLH)\r\n FRL――P1――P2――P3――FR\r\n H  BS0\tBS1\tBS2\tBS3\tBS4\r\n \r\n 설정범위 : EUS ( ±30 % )\r\n");
        }
    }
}
