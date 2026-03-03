using MVVM_Loader.Common;
using MVVM_Loader.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM_Loader.ViewModel
{
    public class Item3ViewModel:BaseVM
    {
        private ObservableCollection<Item3Model> _item3Datas;
        public ObservableCollection<Item3Model> Item3Datas
        {
            get => _item3Datas;
            set
            {
                _item3Datas = value;
                OnPropertyChanged(nameof(Item3Datas));
            }
        }
        public Item3ViewModel()
        {
            //item1m = new Item1Model();
            Item3Datas = new ObservableCollection<Item3Model>
            {
                new Item3Model(12,"COM.C","통신 중지 시 제어상태 설정","0", "", "", "", "Hidden"),
                new Item3Model(13,"EXT.IO","외부 입.출력 설정","0000","","","","Hidden"),
                new Item3Model(14,"CH.MD","채널모드 설정","2", "2", "2", "2", "Visible"),
                new Item3Model(15,"RUN.C","정전복귀 설정","1","","","","Hidden"),
                new Item3Model(16,"ALL.STP","전 채널 정지","0","","","","Hidden"),
                new Item3Model(17,"RUN","운전 / 정지","1", "1", "1", "1", "Visible"),
                new Item3Model(18,"MAN.C","수동제어 모드","0", "0", "0", "0", "Visible"),
                new Item3Model(19,"MVn.O","MVn 출력(수동)","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(20,"EnPO","입력 단선시 출력","0","","","","Hidden"),
                new Item3Model(21,"FAST","Fast mode","0", "0", "0", "0", "Visible"),
                new Item3Model(22,"DI.F","접점입력 종류","0", "0", "0", "0", "Visible"),
                new Item3Model(23,"SV.n","목표 SV 선택","0", "0", "0", "0", "Visible"),
                new Item3Model(24,"SV0","목표설정 SV0","0", "0", "0", "0", "Visible"),
                new Item3Model(25,"SV1","목표설정 SV1","0", "0", "0", "0", "Visible"),
                new Item3Model(26,"SV2","목표설정 SV2","0", "0", "0", "0", "Visible"),
                new Item3Model(27,"SV3","목표설정 SV3","0", "0", "0", "0", "Visible"),
                new Item3Model(28,"AT.M","오토튜닝 종류","0", "0", "0", "0", "Visible"),
                new Item3Model(29,"AL.LCH","경보래치 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(30,"AL.RST","래치모드시 경보해제","0", "0", "0", "0", "Visible"),
                new Item3Model(31,"RLY.CTL","경보출력 제어(AxTY N0.21)","0000", "0000", "0000", "0000", "Visible"),
                new Item3Model(32,"AL.STP","경보시 운전정지","0", "0", "0", "0", "Visible"),
                new Item3Model(33,"A1TY","경보1 종류 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(34,"A2TY","경보2 종류 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(35,"A3TY","경보3 종류 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(36,"A4TY","경보4 종류 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(37,"A1DB","경보1 데드밴드","2", "2", "2", "2", "Visible"),
                new Item3Model(38,"A2DB","경보2 데드밴드","2", "2", "2", "2", "Visible"),
                new Item3Model(39,"A3DB","경보3 데드밴드","2", "2", "2", "2", "Visible"),
                new Item3Model(40,"A4DB","경보4 데드밴드","2", "2", "2", "2", "Visible"),
                new Item3Model(41,"ALM1","경보1 값 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(42,"ALM2","경보2 값 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(43,"ALM3","경보3 값 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(44,"ALM4","경보4 값 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(45,"A1Pn","경보1 출력포트","1", "1", "1", "1", "Visible"),
                new Item3Model(46,"A2Pn","경보2 출력포트","2", "2", "2", "2", "Visible"),
                new Item3Model(47,"A3Pn","경보3 출력포트","3", "3", "3", "3", "Visible"),
                new Item3Model(48,"A4Pn","경보4 출력포트","4", "4", "4", "4", "Visible"),
                new Item3Model(49,"LBA","LBA 시간 값 설정","480", "480", "480", "480", "Visible"),
                new Item3Model(50,"LBW","LBW 변화 폭 설정","2", "2", "2", "2", "Visible"),
                new Item3Model(51,"HBA","히터단선경보 출력의 전류설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(52,"HOC","히터 과전류 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(53,"HDB","히터단선 경보출력의 히스테리시스 설정","1.0", "1.0", "1.0", "1.0", "Visible"),
                new Item3Model(54,"HBS","히터단선 시작점","20", "20", "20", "20", "Visible"),
                new Item3Model(55,"ABS","CT Auto Bias Set","1", "1", "1", "1", "Visible"),
                new Item3Model(56,"H.HLD","히터전류값 홀드","1", "1", "1", "1", "Visible"),
                new Item3Model(57,"P","비례대 설정","20.0", "20.0", "20.0", "20.0", "Visible"),
                new Item3Model(58,"I","적분시간 설정","240", "240", "240", "240", "Visible"),
                new Item3Model(59,"D","미분시간 설정","60", "60", "60", "60", "Visible"),
                new Item3Model(60,"M.R","수동리셋 설정","50.0", "50.0", "50.0", "50.0", "Visible"),
                new Item3Model(61,"HC.DB","가열냉각 불감대","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(62,"STB.C","제어 안정도","1", "1", "1", "1", "Visible"),
                new Item3Model(63,"RIFT","램프제어 적분보상","10", "10", "10", "10", "Visible"),
                new Item3Model(64,"RTMU","RAMP 제어시 시간 단위 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(65,"UP.RT","RAMP 제어시 초기 상승온도 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(66,"DN.RT","RAMP 제어시 초기 하강온도 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(67,"RET","전송출력 종류","0", "0", "0", "0", "Visible"),
                new Item3Model(68,"RET.L","전송출력 하한","0", "0", "0", "0", "Visible"),
                new Item3Model(69,"RET.H","전송출력 상한","100", "100", "100", "100", "Visible"),
                new Item3Model(70,"CT","제어출력 주기 설정","2", "2", "2", "2", "Visible"),
                new Item3Model(71,"CTC","냉각측 제어출력 주기 설정","2", "2", "2", "2", "Visible"),
                new Item3Model(72,"GCO","냉각게인 설정","1.0", "1.0", "1.0", "1.0", "Visible"),
                new Item3Model(73,"HYS.T","히스테리시스 동작","0", "0", "0", "0", "Visible"),
                new Item3Model(74,"HYS","히스테리시스 설정","2", "2", "2", "2", "Visible"),
                new Item3Model(75,"PO","입력 단선시 출력량 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(76,"POC","입력 단선시 냉각측 출력량 설정","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(77,"OL-L","출력량 하한제한","0.0", "0.0", "0.0", "0.0", "Visible"),
                new Item3Model(78,"OL-H","출력량 상한제한","100.0", "100.0", "100.0", "100.0", "Visible"),
                new Item3Model(79,"DP-P","소수점 선택","1", "1", "1", "1", "Visible"),
                new Item3Model(80,"FILT","입력 필터 설정","0", "0", "0", "0", "Visible"),
                new Item3Model(81,"BIAS.L","입력 하한 보정(INP 1~35)","0", "0", "0", "0", "Visible"),
                new Item3Model(82,"BIAS.H","입력 상한 보정(INP 30~35)","0", "0", "0", "0", "Visible"),
                new Item3Model(83,"EN.BS","입력 구간보정기능 ON/OFF","0", "0", "0", "0", "Visible"),
                new Item3Model(84,"BS.P1","입력 구간 설정 1","100", "100", "100", "100", "Visible"),
                new Item3Model(85,"BS.P2","입력 구간 설정 2","100", "100", "100", "100", "Visible"),
                new Item3Model(86,"BS.P3","입력 구간 설정 3","100", "100", "100", "100", "Visible"),
                new Item3Model(87,"BS0","입력 바이어스 0","0", "0", "0", "0", "Visible"),
                new Item3Model(88,"BS1","입력 바이어스 1","0", "0", "0", "0", "Visible"),
                new Item3Model(89,"BS2","입력 바이어스 2","0", "0", "0", "0", "Visible"),
                new Item3Model(90,"BS3","입력 바이어스 3","0", "0", "0", "0", "Visible"),
                new Item3Model(91,"BS4","입력 바이어스 4","0", "0", "0", "0", "Visible")
            };
            //Global.gItem3ViewModel = this;
        }
    }
}
