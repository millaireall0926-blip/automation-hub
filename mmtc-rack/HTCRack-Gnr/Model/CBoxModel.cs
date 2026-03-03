using HTCRack.Base;
using HTCRack.IPC;
using HTCRack.Net;
using HTCRack.ViewModel;
using PropertyChanged;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Media;

namespace HTCRack.Model
{
    public enum BOX_STAT
    {
        STOP = 0,
        RUN,
        MIX,
        TUNE, 
        ERR
    }

    public enum TCON_STAT : int
    {
        COOLING = 1,
        HEATING = 2,
        TUNNING = 4,
        MANUAL = 8,
        RUN = 0x10,
        ADJUST_ERROR = 0x20,
        PLUS_BURNOUT = 0x40,    // Real Error    
        MINUS_BURNOUT= 0x80,    // Real Error
        PLUS_OVER = 0x100,      // Real Error
        MINUS_OVER = 0x200,     // Real Error
        AT_ERROR = 0x400,        
        COMM_ERROR = 0x800,
        RJC_ERROR = 0x1000,
        EEPROM_ERROR = 0x2000,
        FIRMWARE_ERROR = 0x4000,
        SYSTEM_ERROR = 0x8000
    }

    public enum DIM_TYPE
    {
        LOW = 0,
        HIGH,
        LH,
        HL,
        AVG,
        DIMM_MAX,
        BOX_MAX,
        EACH
    }

    public enum SVC  //SV Ctrl
    {
        CLEAR = 0,
        TCON = 0x1,
        STABLE = 0x2,
        ACCELERATION = 0x4,
        NEXT = 0x8,

        DOWN = 0x10,
        UP = 0x20
    }

    public enum TRADING
    {
        RISE,
        FALL
    }


    public class CBoxSub
    {
        [DoNotNotify]
        public int SSRValue { get; set; } = 0;

        [DoNotNotify]
        public List<int> SVCtrlList { get; set; } = new List<int>();        
        [DoNotNotify]
        public List<int> PVCtrlList { get; set; } = new List<int>();
        [DoNotNotify]
        public List<int> DimCtrlList { get; set; } = new List<int>();
        [DoNotNotify]
        public List<int> FanCtrlList { get; set; } = new List<int>();

        [DoNotNotify]
        public int FPIndex { get; set; } = 0;

        public DIM_TYPE _dimType = DIM_TYPE.LOW;
        [DoNotNotify]
        public double SubInsideTemp { get; set; }
        [DoNotNotify]
        public double SubDimTempAvg { get; set; }

        [DoNotNotify]
        public double TargetSV { get; set; }
        [DoNotNotify]
        public double ReadPV { get; set; }


        public Collection<double> SampleLongTerm = new Collection<double>();
        
        public Collection<double> SampleShortTerm = new Collection<double>();

        public int Level = 0;
        public int Level1Check = 0;
        public double OrgSV = 0;

        public double BSVAlue = 0;

        public DateTime ActionTime = DateTime.Now;

        public int SVCtrl;

        public TRADING PrevTrading = TRADING.RISE;
    }

    public class CBoxModel : BaseVM
    {
        public string Name { get; set; }
        [DoNotNotify]
        public int BoxIndex { get; set; }

        public int DimColumns { get; set; }

        public string BoxTemp { get; set; }
        [DoNotNotify]        
        public double InsideBoxTemp { get; set; } = 0;
        [DoNotNotify]
        public double PrevInsideBoxTemp { get; set; } = 0;
        [DoNotNotify]
        public double InsideBoxTempCheckCount { get; set; } = 0;

        [DoNotNotify]
        public double DimTempAvg { get; set; }


        [DoNotNotify]
        public List<CBoxSub> BoxSubs { get; set; } = new List<CBoxSub>();
        [DoNotNotify]
        public ObservableCollection<DimModel> DimModels { get; set; } = new ObservableCollection<DimModel>();
        public SolidColorBrush BoxColor { get; set; }

        // sv-ofs 기울기로 사용할때
        [DoNotNotify]
        public double ExtSVScope { get; set; } = 0;
        [DoNotNotify]
        public double ExtSVOffet { get; set; } = 0;
        [DoNotNotify]
        public double ExtSVValue { get; set; } = 0;
        [DoNotNotify]

        public DIM_TYPE[] EachTempHLs = Enumerable.Repeat(DIM_TYPE.LOW, 22).ToArray();//new int[10];
        public double[] EachTempOffsets = Enumerable.Repeat<double>(0, 22).ToArray();
        public double[] MinBefore10m    = Enumerable.Repeat<double>(0, 22).ToArray();
        public double[] MaxBefore10m    = Enumerable.Repeat<double>(0, 22).ToArray();

        public List<double> MaxInBox = new List<double>();
        public int NoDimmCount = 0;
        public double MaxInBoxValue = 0;


        [DoNotNotify]
        public RackModel MasterRack { get; set; }

        public int LongTermCount = 20;
        public int ShortTermCount = 10;



        public BOX_STAT _boxStat = BOX_STAT.STOP;

        [DoNotNotify]
        public BOX_STAT BoxStat {
            get => _boxStat;
            set {
                BOX_STAT prevStat = _boxStat;
                _boxStat = value;
                BoxColor = Global.gCBoxColors[(int)value];
                if (_boxStat == BOX_STAT.STOP && BoxTemp != "")
                {
                    BoxTemp = "";
                    foreach (var dm in DimModels)
                    {
                        dm.DimTemp = 0;
                        dm.Stat = DIMM_STAT.STOP;
                    }

                } else if ((prevStat == BOX_STAT.STOP || prevStat == BOX_STAT.MIX) &&
                    _boxStat == BOX_STAT.RUN) {
                    foreach (var dm in DimModels)
                    {
                        dm.DimTemp = 0;
                        dm.Stat = DIMM_STAT.RUN;
                        MasterRack.TimeStart = DateTime.Now;
                    }
                }
            }
        }

        // 1. start time to stable time wait        
        public void processBoxST()
        {
            IList<RegModel> tcons = (IList<RegModel>)MasterRack.TConRegs;

            string warnMsg = "";
            int runSum = 0;
            int tuneSum = 0;
            int errSum = 0;
            int svCount = 0;

            foreach (CBoxSub sub in BoxSubs)
            {
                foreach (int idx in sub.SVCtrlList)
                {
                    int chStat = (int)tcons[RegMap.TCon_GetCHSTS + idx].ReadValue;
                    if ((chStat & (int)TCON_STAT.RUN) != 0)
                        runSum++;
                    if ((chStat & (int)TCON_STAT.TUNNING) != 0)
                        tuneSum++;
                    //0619 Error ChStat 수정
                    if ((chStat & ((int)TCON_STAT.PLUS_BURNOUT
                                    | (int)TCON_STAT.MINUS_BURNOUT
                                    | (int)TCON_STAT.PLUS_OVER
                                    | (int)TCON_STAT.MINUS_OVER)) != 0)
                    {
                        if ((chStat & (int)TCON_STAT.PLUS_BURNOUT) != 0)
                            warnMsg += TCON_STAT.PLUS_BURNOUT.ToString();
                        if ((chStat & (int)TCON_STAT.MINUS_BURNOUT) != 0)
                            warnMsg += TCON_STAT.MINUS_BURNOUT.ToString();
                        if ((chStat & (int)TCON_STAT.PLUS_OVER) != 0)
                            warnMsg += TCON_STAT.PLUS_OVER.ToString();
                        if ((chStat & (int)TCON_STAT.MINUS_OVER) != 0)
                            warnMsg += TCON_STAT.MINUS_OVER.ToString();
                        errSum++;
                    }
                }
                svCount += sub.SVCtrlList.Count;
            }

            if (errSum > 0)
            {
                Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{MasterRack.Name}][{Name}] {warnMsg}");
                BoxStat = BOX_STAT.ERR;
                return;
            }

            if (runSum == 0) BoxStat = BOX_STAT.STOP;
            else if (runSum == svCount) BoxStat = BOX_STAT.RUN;
            else BoxStat = BOX_STAT.MIX;

            if (tuneSum > 0) BoxStat = BOX_STAT.TUNE;
        }

        public double calcDimTemp(int data)
        {
            int value = data & 0xfff;
            double temp = 0;
            if (((int)data & 0x1000) != 0)
                temp = Math.Round((0xFFFFF000 + value) * 0.0625, 1);
            else
                temp = Math.Round(value * 0.0625, 1);
            return temp;
        }

        public void processBoxDimmInStop()
        {
            IList<RegModel> dims = (IList<RegModel>)MasterRack.DimRegs;
            IList<DimDataModel> dimTemps = (IList<DimDataModel>)MasterRack.DimDatas;
            int uiIdx = 0;

            foreach (CBoxSub sub in BoxSubs)
            {                
                for (int i = 0; i < sub.DimCtrlList.Count; i++)
                {
                    int index = sub.DimCtrlList[i];
                    DimModel uiDim = DimModels[uiIdx];
                    if (index >= Global.gMainPageVM.MaxFixDimmCount) continue;

                    DIM_TYPE type = sub._dimType;
                    if (type == DIM_TYPE.EACH)
                    {
                        int eachidx = (int)MasterRack.SetSV / 5;
                        type = EachTempHLs[eachidx];                        
                    }

                    uiDim.DimTempLow = dimTemps[index].TempLow = dims[index * 2].ReadValue;
                    uiDim.DimTempHigh = dimTemps[index].TempHigh = dims[index * 2 + 1].ReadValue;


                    double temp = 0;
                    if (type == DIM_TYPE.HIGH)
                        temp = dims[index * 2 + 1].ReadValue;
                    else if (type == DIM_TYPE.LOW)
                        temp = dims[index * 2].ReadValue;
                    else if (type == DIM_TYPE.LH)
                    {
                        if (i >= (sub.DimCtrlList.Count / 2))
                            temp = dims[index * 2 + 1].ReadValue;
                        else
                            temp = dims[index * 2].ReadValue;
                    }
                    else if (type == DIM_TYPE.HL)
                    {
                        if (i >= (sub.DimCtrlList.Count / 2))
                            temp = dims[index * 2].ReadValue;
                        else
                            temp = dims[index * 2 + 1].ReadValue;
                    }
                    else if (type == DIM_TYPE.AVG)
                    {
                        temp += dims[index * 2].ReadValue;
                        temp += dims[index * 2 + 1].ReadValue;
                        temp /= 2;
                    }
                    else if (type == DIM_TYPE.BOX_MAX || type == DIM_TYPE.DIMM_MAX)
                    {
                        if (dims[index * 2].ReadValue > dims[index * 2 + 1].ReadValue)
                            temp = dims[index * 2].ReadValue;
                        else
                            temp = dims[index * 2 + 1].ReadValue;
                    }


                    uiDim.DimTemp = temp;
                    uiIdx++;
                }
                
            }
        }




        public void processBoxDimm()
        {
            IList<RegModel> dims = (IList<RegModel>)MasterRack.DimRegs;
            IList<DimDataModel> dimTemps = (IList<DimDataModel>)MasterRack.DimDatas;

            DateTime afterStable = MasterRack.TimeStart.AddMinutes(Global.gConfigVM.StableTime);

            double dimAvg = 0;
            int mountCnt = 0;
            double pvAvg = 0;
            int pvCnt = 0;
            bool isError = false;
            string warnMsg = "";
            int uiIdx = 0;

            foreach (CBoxSub sub in BoxSubs)
            {
                int subMountCnt = 0;
                double subDimAvg = 0;
                for (int i = 0; i < sub.DimCtrlList.Count; i++)
                {
                    int index = sub.DimCtrlList[i];
                    DimModel uiDim = DimModels[uiIdx];
                    if (index >= Global.gMainPageVM.MaxFixDimmCount) continue;

                    DIM_TYPE type = sub._dimType;
                    if (type == DIM_TYPE.EACH)
                    {
                        int eachidx = (int)MasterRack.SetSV / 5;
                        type = EachTempHLs[eachidx];
                        //type = (DIM_TYPE)Enum.Parse(typeof(DIM_TYPE), EachTempHLs[eachidx]
                    }

                    uiDim.DimTempLow = dimTemps[index].TempLow = dims[index * 2].ReadValue;
                    uiDim.DimTempHigh = dimTemps[index].TempHigh = dims[index * 2 + 1].ReadValue;


                    double temp = 0;
                    if (type == DIM_TYPE.HIGH)
                        temp = dims[index * 2 + 1].ReadValue;
                    else if (type == DIM_TYPE.LOW)
                        temp = dims[index * 2].ReadValue;
                    else if (type == DIM_TYPE.LH)
                    {
                        if (i >= (sub.DimCtrlList.Count / 2))
                            temp = dims[index * 2 + 1].ReadValue;
                        else
                            temp = dims[index * 2].ReadValue;
                    }
                    else if (type == DIM_TYPE.HL)
                    {
                        if (i >= (sub.DimCtrlList.Count / 2))
                            temp = dims[index * 2].ReadValue;
                        else
                            temp = dims[index * 2 + 1].ReadValue;
                    }
                    else if (type == DIM_TYPE.AVG)
                    {
                        temp += dims[index * 2].ReadValue;
                        temp += dims[index * 2 + 1].ReadValue;
                        temp /= 2;
                    }else if (type == DIM_TYPE.BOX_MAX || type == DIM_TYPE.DIMM_MAX)
                    {
                        if (dims[index * 2].ReadValue > dims[index * 2 + 1].ReadValue)
                            temp = dims[index * 2].ReadValue;
                        else
                            temp = dims[index * 2+1].ReadValue;
                    }
                    


                    uiDim.DimTemp = temp;
                    dimTemps[index].Temp = temp;

                    
                    MaxInBox.Add(uiDim.DimTempLow);
                    MaxInBox.Add(uiDim.DimTempHigh);


                    if (temp != 0)
                    {
                        dimAvg += temp;
                        subDimAvg += temp;
                        subMountCnt++;
                        mountCnt++;
                    }

                    if (DateTime.Now > afterStable)
                    {                        
                        uiDim.doDimStat();
                        if (uiDim.Stat == DIMM_STAT.ALRAM)
                        {
                            warnMsg += $"Dim{uiIdx + 1} ";
                        }
                        else if (uiDim.Stat == DIMM_STAT.ERROR)
                        {
                            isError = true;
                            Global.gMainPageVM.addLog(LOG_TYPE.ERROR, $"[Prog][{MasterRack.Name}][{Name}] : Dimm{uiIdx}(Total Index{index}) " +
                                $"Temp={uiDim.DimTemp}) " +
                                $" Limit={Global.gConfigVM.DimmLimit}" +
                                $" Max={Global.gConfigVM.MaxDimm}" +
                                $" Min={Global.gConfigVM.MinDimm}");
                        }
                    }
                    uiIdx++;
                }

                double prevDimAvg = sub.SubDimTempAvg;
                if (subMountCnt > 0)
                {
                    MaxInBoxValue = MaxInBox.Max();
                    if (sub._dimType == DIM_TYPE.BOX_MAX)
                        sub.SubDimTempAvg = MaxInBoxValue;
                    else
                        sub.SubDimTempAvg = Math.Round(subDimAvg / subMountCnt, 1);                    
                }
                else
                {
                    sub.SubDimTempAvg = 0;
                    MaxInBoxValue = 0;
                }
                MaxInBox.Clear();


                sub.SampleShortTerm.Add(sub.SubDimTempAvg - prevDimAvg);
                if (sub.SampleShortTerm.Count > ShortTermCount)
                    sub.SampleShortTerm.RemoveAt(0);

                sub.SampleLongTerm.Add(sub.SubDimTempAvg);
                if (sub.SampleLongTerm.Count > LongTermCount)
                    sub.SampleLongTerm.RemoveAt(0);


                if (prevDimAvg == 0 && sub.SubDimTempAvg != 0)  // 모든 dimm이 0 이였다가 다시 dimm 온도가 나오면 
                {                                               // 기존 Ctrl Context를 초기화
                    SVST(sub, SVC.STABLE);
                    sub.SampleLongTerm.Clear();
                    sub.SampleShortTerm.Clear();
                }

                // Tcon의 Box 안쪽 Temp
                IList<RegModel> tcons = (IList<RegModel>)MasterRack.TConRegs;
                double subBoxAvg = 0;

                for (int i = 0; i < sub.PVCtrlList.Count; i++)
                {
                    int index = sub.PVCtrlList[i] + RegMap.TCon_GetPV;
                    if (index >= tcons.Count)
                        continue;
                    subBoxAvg += tcons[index].ReadValue;
                    pvAvg += tcons[index].ReadValue;
                }

                if (sub.PVCtrlList.Count > 0) subBoxAvg = Math.Round(subBoxAvg / sub.PVCtrlList.Count, 1);
                sub.SubInsideTemp = subBoxAvg;  // TCON PV Temp 평균
                pvCnt += sub.PVCtrlList.Count;



                if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.ML)
                {
                    IPCMessage msg = new IPCMessage();
                    msg.RackNo = MasterRack.RackIndex;
                    msg.BoxNo = BoxIndex;
                    msg.Temps = new float[(DimModels.Count * 2) + 1];
                    int dimIndex = 0;
                    foreach (DimModel dm in DimModels)
                        msg.Temps[dimIndex++] = (float)dm.DimTemp;
                    foreach (DimModel dm in DimModels)
                        msg.Temps[dimIndex++] = (float)dm.DiffPrev;
                    msg.Temps[dimIndex] = (float)MasterRack.SetSV;
                    Global.gMainVM.sendIPCData(msg);
                }

            }

            if (warnMsg != "")
            {
                warnMsg += "Temp Alram";
                MasterRack.showAlram(ALRAM_TYPE.ERROR, $"[Prog][{MasterRack.Name}][{Name}]" ,  warnMsg);
            }
            if (isError)
            {
                MasterRack.RackStat = RACK_STAT.ERROR;
                return;
            }

            if (pvCnt > 0)
            {
                InsideBoxTemp = Math.Round(pvAvg / pvCnt, 1);
                if (PrevInsideBoxTemp != 0)
                {
                    double diff = InsideBoxTemp - PrevInsideBoxTemp;
                    if (diff > 10)
                    {
                        Global.gMainPageVM.addLog(LOG_TYPE.WARNING,
                            $"[Prog][{MasterRack.Name}][{Name}] Revert PVTemp OrgRd {InsideBoxTemp}, prev {PrevInsideBoxTemp}, diff {diff}");
                        if (InsideBoxTempCheckCount < 5)
                            InsideBoxTemp = PrevInsideBoxTemp;
                        InsideBoxTempCheckCount++;
                    }
                    else
                        InsideBoxTempCheckCount = 0;
                }
                PrevInsideBoxTemp = InsideBoxTemp;
            }
            if (mountCnt > 0) DimTempAvg = Math.Round(dimAvg / mountCnt, 1);
            BoxTemp = DimTempAvg.ToString(); // BoxTemp는 Temp Box 출력용. 
        }


        public (double, int, int) AnalysTrend(Collection<double> collect)
        {
            double avgSum = collect.Sum();
            int down = 0, up = 0;
            foreach (double val in collect)
            {
                if (val > 0) up++;
                if (val < 0) down++;
            }


            return (avgSum, up, down);
        }

        // 표준 편차
        public double calcSD(Collection<double> collect)
        {
            if (collect.Count == 0) return 0;

            double sum = 0, mean = 0, sd = 0;
            foreach (double d in collect)
                sum += d;
            mean = sum / collect.Count;

            foreach (double d in collect)
                sd += Math.Pow(d - mean, 2);

            return Math.Sqrt(sd / collect.Count);
        }


        public bool checkSV(CBoxSub sub, SVC check)
        {
            int ch = (int)check;
            if ((sub.SVCtrl & ch) != 0) return true;
            return false;
        }

        public void SVST(CBoxSub sub, SVC set)
        {
            sub.SVCtrl = (int)set;
        }
        public void processBoxSVSub(CBoxSub sub, double setSV)
        {
            //Console.WriteLine($" {Name} - SortermCnt={ShortTermCount}");
            double prevTarget = sub.TargetSV;
            double DIM_SV_Diff = setSV - sub.SubDimTempAvg;
            if (sub.SubDimTempAvg == 0)
                goto SVDone;



            TRADING trading = TRADING.RISE;
            if (DIM_SV_Diff < 0) trading = TRADING.FALL;
            double direction = 1;
            if (trading == TRADING.FALL) direction = -1;

            DIM_SV_Diff = Math.Abs(DIM_SV_Diff);

            if (DIM_SV_Diff < 16)
            {
                //////////////////////////
                //////////////  Done ACCELERATION
                /////////////////////////////
                if (checkSV(sub, SVC.ACCELERATION))
                {
                    sub.TargetSV = setSV;
                    SVST(sub, SVC.STABLE);
                }

                //////////////////////////
                //////////////  DOWN Action
                /////////////////////////////
                if (checkSV(sub, SVC.DOWN))
                {
                    if (sub.SampleShortTerm.Count <= 4) goto SVDone;
                    (double avg, int up, int down) = AnalysTrend(sub.SampleShortTerm);
                    int orgLevel = sub.Level;
                    if (sub.Level == 1)
                    {
                        if (up > 0) // 상승 시작
                        {
                            Console.Write($"-- 상승시작 -- ");
                            sub.SampleShortTerm.Clear();
                            sub.TargetSV = sub.OrgSV;
                            sub.Level++;
                        }

                    } else if (sub.Level == 2)
                    {

                        if (down > 0) // 상승 실패
                        {
                            Console.Write($"-- 상승실패 일반제어 복귀-- ");
                            sub.TargetSV += Math.Round((DIM_SV_Diff * 1.6) * direction, 1);
                            sub.SVCtrl ^= (int)SVC.DOWN;
                            sub.SampleLongTerm.Clear();
                        }
                        else if (DIM_SV_Diff <= 1)
                        {
                            if (up > 0)  // 오르는중.
                            {

                                sub.SampleShortTerm.Clear();
                                sub.OrgSV = sub.TargetSV;

                                //  85도일때 add 2.6, 40도일때 add 8
                                //  기울기 = -0.12, 옵셋 = 12.8                              
                                double nextSV = (MasterRack.SetSV * -0.12) + 12.8;
                                sub.TargetSV -= nextSV;
                                sub.Level++;
                                Console.Write($"-- 오르는중 calcSV {nextSV}-- ");
                            }
                        }
                    }
                    else if (sub.Level == 3)
                    {
                        if (down > 0)
                        {
                            Console.Write($"--L3 잡힘 --");
                            sub.TargetSV = sub.OrgSV;
                            sub.Level = 0;
                            sub.SVCtrl ^= (int)SVC.DOWN;
                        }
                    }
                    if (sub.ActionTime.AddMinutes(1) < DateTime.Now)
                    {
                        Console.Write($"--Down 시간제한 --");
                        sub.TargetSV = sub.OrgSV;
                        sub.Level = 0;
                        sub.SVCtrl ^= (int)SVC.DOWN;
                    }

               //         Console.WriteLine($"DOWN-Ctrl {Name} Level:{orgLevel} ->  {sub.Level} " +
               //         $"Diff:{Math.Round(DIM_SV_Diff, 1)} Target:{Math.Round(sub.TargetSV, 1)} " +
               //         $"- org:{sub.OrgSV} avg:{Math.Round(avg, 1)} up:{up} down:{down}");
                    goto SVDone;
                }

                //////////////////////////
                ////  UP Action
                //////////////////////////
                if (checkSV(sub, SVC.UP))
                {
                    (double avg, int up, int down) = AnalysTrend(sub.SampleShortTerm);
                    if (down > 1)
                    {
                        
                        sub.SampleShortTerm.Clear();
                        sub.SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 제어완료 --- sv {sub.TargetSV} -> {sub.OrgSV}");
                        sub.TargetSV = sub.OrgSV;

                    }

                    if (sub.ActionTime.AddMinutes(1) < DateTime.Now )
                    {
                        sub.SampleShortTerm.Clear();
                        sub.SVCtrl ^= (int)SVC.UP;
                        Console.WriteLine($"--- Up 시간제한 --- sv {sub.TargetSV} -> {sub.OrgSV}");
                        sub.TargetSV += Math.Round((DIM_SV_Diff / 2) * direction, 1);
                        //sub.TargetSV = sub.OrgSV;
                    }
//                    else 
//                        sub.TargetSV -= 4;

                    goto SVDone;
                }

                //////////////////////////
                //////////// DO LOGN Term
                /////////////////////////////
                if (sub.SampleLongTerm.Count == LongTermCount)
                {
                    double avg = sub.SampleLongTerm.Average();
                    double sd = calcSD(sub.SampleLongTerm);

                    //Console.WriteLine($"{Name} SD : {sd}");

                    if (sd < 2) SVST(sub, SVC.STABLE);

                    if (checkSV(sub, SVC.STABLE))
                    {
                        double stable_diff = Math.Abs(setSV - avg);
                        stable_diff = Math.Round(stable_diff, 1);

                        if (stable_diff > 0.5)
                        {

                            double org = sub.TargetSV;
                            sub.TargetSV += Math.Round((stable_diff / 2) * direction, 1);
                            //Console.WriteLine($"{Name} LongTerm Adjust {org} -> {sub.TargetSV}");
                            sub.SampleLongTerm.Clear();
                            goto SVDone;
                        }
                    }
                }

                //////////////////////////////
                ///////////// Prevent Down / UP Action 
                /////////////////////////////////
                if (sub.PrevTrading != trading)
                    sub.SampleShortTerm.Clear();
                sub.PrevTrading = trading;

                //////////////////////////
                ////////////////  DO SortTerm
                /////////////////////////////
                
                if (sub.SampleShortTerm.Count == ShortTermCount)
                {
                    if (checkSV(sub, SVC.STABLE))
                    {
                        if (!(checkSV(sub, SVC.DOWN) || checkSV(sub, SVC.UP)))
                        {
                            if (DIM_SV_Diff <= 5)
                            {
                                (double avg, int up, int down) = AnalysTrend(sub.SampleShortTerm);
                                double sd = Math.Round(calcSD(sub.SampleShortTerm), 3);

                                avg = Math.Round(avg, 1);
                                //Console.WriteLine($"{Name} SortTerm 편차:{sd} avg:{avg} u:{up} d:{down} T:{trading.ToString()}");
                                if (avg <= -0.8 && down > 1 &&
                                    ((trading == TRADING.RISE) ||
                                    (trading == TRADING.FALL && DIM_SV_Diff < 0.6)))
                                {
                                    sub.ActionTime = DateTime.Now;
                                    sub.Level1Check = 0;
                                    sub.Level = 1;
                                    sub.OrgSV = sub.TargetSV;
                                    //sub.TargetSV += Math.Round((setSV * 8) / 100, 1);
                                    sub.TargetSV += 8;

                                    Console.WriteLine($"다운제어 시작 {Name} SV: {sub.OrgSV} -> {sub.TargetSV}");
                                    sub.SVCtrl |= (int)SVC.DOWN;

                                    sub.SampleShortTerm.Clear();
                                    sub.SampleLongTerm.Clear();
                                }

                                if (avg >= 0.8 && up > 1 &&
                                    ((trading == TRADING.FALL) ||
                                    (trading == TRADING.RISE && DIM_SV_Diff < 0.6)))
                                {
                                    sub.ActionTime = DateTime.Now;
                                    sub.Level = 1;
                                    //sub.TargetSV -= Math.Round((setSV * 8) / 100, 1);
                                    sub.TargetSV -= 8;
                                    sub.OrgSV = sub.TargetSV;
                                    Console.WriteLine($"업 제어 시작 {Name} SV: {sub.OrgSV} -> {sub.TargetSV}");
                                    sub.SVCtrl |= (int)SVC.UP;

                                    sub.SampleShortTerm.Clear();
                                    sub.SampleLongTerm.Clear();
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                SVST(sub, SVC.ACCELERATION);
                sub.TargetSV = setSV + (20.0 * direction);
            }

        //   if (sub.TargetSV > 120 || sub.TargetSV < 16)
        //   {
        //       MasterRack.showAlram(ALRAM_TYPE.ERROR,$"[Prog][{MasterRack.Name}][{Name}]", $"TargetSV={sub.TargetSV} " +
        //           $"SV upper limit is 120 degrees and lower limit is 16 degrees");
        //       BoxStat = BOX_STAT.ERR;
        //   }


        SVDone:
            
            if (Global.gConfigVM.IsMaxLimitTarget)
            {
                if (MasterRack.TimeStart.AddMinutes(10) > DateTime.Now ) // before 10Min
                {
                    int eachidx = (int)MasterRack.SetSV / 5;
                    double min = MinBefore10m[eachidx];
                    double max = MaxBefore10m[eachidx];
                    if (sub.TargetSV > max) sub.TargetSV = max;
                    else if (sub.TargetSV < min) sub.TargetSV = min;
                }

                //                if (MasterRack.TimeStart.AddMinutes(8) > DateTime.Now )
                //                    if (sub.TargetSV > MasterRack.SetSV) sub.TargetSV = MasterRack.SetSV;
            }

            if (Global.gConfigVM.UseMaxCtrl)
            {
                double OverLimit = MasterRack.SetSV + Global.gConfigVM.MaxCtrlBase;

                if (sub.SubDimTempAvg > OverLimit)
                {
                    if (sub.SampleShortTerm.Count > 0)
                    {
                        double val = sub.SampleShortTerm.Last();
                        //Console.WriteLine($"Over 80 {Name}  {val}");
                        if (val > 0)
                        {
                            sub.TargetSV -= Global.gConfigVM.MaxCtrlStep;
                            //Console.WriteLine($"    Minus {Global.gConfigVM.MaxCtrlStep} {Name} -> {Global.gConfigVM.MaxCtrlStep}");
                            //Global.gMainPageVM.addLog(LOG_TYPE.WARNING,$"Over 80 {Name}  {val} Minus {Global.gConfigVM.MaxCtrlStep} {Name} -> {Global.gConfigVM.MaxCtrlStep}");
                        }
                    }

                    // Remove Mode
                    if (checkSV(sub, SVC.DOWN) || checkSV(sub, SVC.UP))
                    {
                        sub.SampleShortTerm.Clear();
                        SVST(sub, SVC.STABLE);
                    }

                }
            }

            if (sub.TargetSV > Global.gConfigVM.CtrlSVMax) sub.TargetSV = Global.gConfigVM.CtrlSVMax;
            if (sub.TargetSV < Global.gConfigVM.CtrlSVMin) sub.TargetSV = Global.gConfigVM.CtrlSVMin;

            if (sub.TargetSV < 0) sub.TargetSV = 0;
            if (prevTarget != sub.TargetSV)
                sendSV(sub, sub.TargetSV);
        }


        public void processBoxSVOrg()
        {
            foreach (CBoxSub sub in BoxSubs)
            {
                double ctrl = sub.TargetSV;
                double diff = MasterRack.SetSV - InsideBoxTemp;
                double step = 0.4;

                if (Math.Abs(diff) <= 15)
                {
                if (DimTempAvg <= MasterRack.SetSV - step)
                    ctrl += step;
                else if (DimTempAvg >= MasterRack.SetSV + step)
                    ctrl -= step;
                } else
                    ctrl = MasterRack.SetSV;

                if (ctrl > MasterRack.SetSV) ctrl = MasterRack.SetSV;
                else if (ctrl < 18) ctrl = 18;

                if (sub.SubDimTempAvg == 0) continue;
                sub.TargetSV = ctrl;
                sendSV(sub, ctrl);

            }
        }


        public void processBoxBS(CBoxSub sub)
        {


            double PV_SV_Diff = Math.Abs( MasterRack.SetSV - sub.SubInsideTemp);
            if (PV_SV_Diff > 1)
            {
                //sub.BSVAlue = 0;
                return;
            }


            double bsValue = (sub.SubDimTempAvg - sub.SubInsideTemp) + sub.BSVAlue;
            if (bsValue > 100 || bsValue < -100)
                return;


            sub.BSVAlue = bsValue;
            Console.WriteLine($"{Name} bs {Math.Round(bsValue,1 )} " +
                $"diff {Math.Round(sub.SubDimTempAvg - sub.SubInsideTemp, 1)} ");
            sendBS1(sub, bsValue);
        }

        public void processBoxSV()
        {
            if (
                    (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.ML) ||
                    ((Global.gConfigVM.ControlMode == (int)CONTROL_MODE.MANUAL) &&
                      (Global.gDimmVM.ManualControls[BoxIndex].IsAuto == false)) ||
                    (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.NOSV)
                )            
                return;

            if (BoxStat == BOX_STAT.STOP) return;


            double set = MasterRack.SetSV + MasterRack.Throttle_SV_Offset;
            set += EachTempOffsets[(int)MasterRack.SetSV / 5];
            //set += ExtSVValue;   //ofs 기울기

            foreach (CBoxSub sub in BoxSubs) {
                if (sub.SubDimTempAvg == 0) continue;
                if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.NORMAL)
                    processBoxSVSub(sub, set);
                else if (Global.gConfigVM.ControlMode == (int)CONTROL_MODE.MMLOGIC)
                    processBoxSVOrg();
                //processBoxBS(sub);
            }
        }

        public void sendFan(CBoxSub sub, int percent)
        {
            foreach (int index in sub.FanCtrlList)
            {
                MasterRack.sendFanDuty(index, percent);
            }
        }

        public void sendFan(int percent)
        {
            foreach (CBoxSub sub in BoxSubs)
            {
                sendFan(sub, percent);
            }
        }


        public void sendBS1(CBoxSub sub,  double set)
        {
            short pvOffset = (short)(set * 10.0);
            //Console.WriteLine($"{Name} SV : {sv}");
            foreach (int index in sub.SVCtrlList)
            {
                HtcMessage msg = Global.newMsg(0xff, index, 0x30);
                msg.data = BitConverter.GetBytes(pvOffset);
                MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
            }
        }


        public void sendSV(CBoxSub sub,  double set)
        {
            short sv = (short)(set * 10.0);            
            foreach (int index in sub.SVCtrlList)
            {
                HtcMessage msg = Global.newMsg(0xff, index, 0x10);
                msg.data = BitConverter.GetBytes(sv);
                MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
            }
            
        }

        public void sendSV(double set)
        {            
            foreach (CBoxSub sub in BoxSubs)            
                foreach (int index in sub.SVCtrlList)                
                    sendSV(sub, set);                            
        }

        public void sendRun()
        {
            short ctrl = 1;
            foreach (CBoxSub sub in BoxSubs)
            {
                foreach (int index in sub.SVCtrlList)
                {
                    HtcMessage msg = Global.newMsg(0xff, index, 0x11);
                    msg.data = BitConverter.GetBytes(ctrl);
                    MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
                }
            }
            NoDimmCount = 0;  
        }

        public void sendStop()
        {
            short ctrl = 0;
            foreach (CBoxSub sub in BoxSubs)
            {
                foreach (int index in sub.SVCtrlList)
                {
                    HtcMessage msg = Global.newMsg(0xff, index, 0x11);
                    msg.data = BitConverter.GetBytes(ctrl);
                    MasterRack._client.pushSendQue(REG_TYPE.TCON, msg);
                }
            }
        }

        public void stopBox()
        {
            foreach (DimModel dim in DimModels)
                dim.Stat = DIMM_STAT.STOP;
        }


        public CBoxModel(string name , RackModel master) { 
            Name = $"CBox {name}"; MasterRack = master;
            BoxColor = Global.gCBoxColors[(int)BOX_STAT.STOP];
        }
    }
}
