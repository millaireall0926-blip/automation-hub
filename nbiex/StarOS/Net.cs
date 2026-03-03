using HandyControl.Controls;
using StarBase;    
using StarOS.Model;
using StarOS.ViewModel;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Threading;
using System.Windows;
using System.Windows.Threading;
using System.Xml;
using StarBase.Net;
using StarBase.Plc;
using System.Runtime.InteropServices;
using static HandyControl.Tools.Interop.InteropValues;
using Google.Protobuf.WellKnownTypes;
using System.Xml.Linq;
using ScottPlot;
using System.Threading.Tasks;

namespace StarOS
{
    public class Net
    {
        public Net()
        {
            Global.gNet = this;

            _client.deleFunc += processResult;
            _client.recvFileFunc += processRecvFile;
            _client.conFunc += recvCon;

            _msgQueThread.Start(this);
            _checkThread.Start(this);

            _dbCtrl.loadDB();
            if (_dbCtrl._useDB)_dbCtrl.start();

        }

        ~Net()
        {
            
        }

        public void quit() {
            _close = true;
            _client.Close();
            _checkThread.Join();            
            _msgQueThread.Join();
            
            _dbCtrl.quit();
        }

        public DBCtrl _dbCtrl = new DBCtrl();
        public StarClient _client = new StarClient();


        #region Thread Process 
        public Thread _msgQueThread = new Thread(threadMsgQue); // Connection Check 용
        public Thread _checkThread = new Thread(threadNetCheck); // Connection Check 용
        

        public static bool _close = false;

        public static void threadMsgQue(object parent)
        {
            Net net = parent as Net;

            while (Net._close == false)
            {
                if (net._client.getRQSize() > 0)
                {
                    StarMessage msg = net._client.popRQ();
                    if (msg.type == MSG_TYPE.TYPE_REQFILE || msg.type == MSG_TYPE.TYPE_REQDONE)
                    {
                        net.processRecvFile(msg.recvMsg);
                    }else if (msg.type == MSG_TYPE.TYPE_SENDBIN)
                    {
                        net.processRecvBinary(msg.recvMsg, msg.buffer);
                    }
                    else
                    {
                        net.processResult(msg.command, msg.xmlNode);
                    }
                }
                else
                {
                    Thread.Sleep(1);
                }
            }

        }

        public static void threadNetCheck(object parent)
        {
            Net net = parent as Net;

            while (Net._close == false)
            {
                if (net._client.IsConnected == false)
                {
                    net._client.doConnect(Global.gRouterIP, Global.gRouterPort);
                }
                Thread.Sleep(2000);
            }
        }

        #endregion


        #region PGM CMD Process 

        /// <summary>        
        /// <layout x=\"20\" y=\"16\" io=\"4\" m=\"1\" sc=\"20\">
        ///     <line>0=60-63,1=60-63,2=60-63,3=60-63,4=60-63,5=60-63,6=60-63,7=60-63,8=60-63,9=60-63,10=60-63,11=60-63,12=60-63,13=60-63,14=60-63,15=60-63,16=60-63,17=60-63,18=60-63,19=60-63</line><line>0=56-59,1=56-59,2=56-59,3=56-59,4=56-59,5=56-59,6=56-59,7=56-59,8=56-59,9=56-59,10=56-59,11=56-59,12=56-59,13=56-59,14=56-59,15=56-59,16=56-59,17=56-59,18=56-59,19=56-59</line><line>0=52-55,1=52-55,2=52-55,3=52-55,4=52-55,5=52-55,6=52-55,7=52-55,8=52-55,9=52-55,10=52-55,11=52-55,12=52-55,13=52-55,14=52-55,15=52-55,16=52-55,17=52-55,18=52-55,19=52-55</line><line>0=48-51,1=48-51,2=48-51,3=48-51,4=48-51,5=48-51,6=48-51,7=48-51,8=48-51,9=48-51,10=48-51,11=48-51,12=48-51,13=48-51,14=48-51,15=48-51,16=48-51,17=48-51,18=48-51,19=48-51</line><line>0=44-47,1=44-47,2=44-47,3=44-47,4=44-47,5=44-47,6=44-47,7=44-47,8=44-47,9=44-47,10=44-47,11=44-47,12=44-47,13=44-47,14=44-47,15=44-47,16=44-47,17=44-47,18=44-47,19=44-47</line><line>0=40-43,1=40-43,2=40-43,3=40-43,4=40-43,5=40-43,6=40-43,7=40-43,8=40-43,9=40-43,10=40-43,11=40-43,12=40-43,13=40-43,14=40-43,15=40-43,16=40-43,17=40-43,18=40-43,19=40-43</line><line>0=36-39,1=36-39,2=36-39,3=36-39,4=36-39,5=36-39,6=36-39,7=36-39,8=36-39,9=36-39,10=36-39,11=36-39,12=36-39,13=36-39,14=36-39,15=36-39,16=36-39,17=36-39,18=36-39,19=36-39</line><line>0=32-35,1=32-35,2=32-35,3=32-35,4=32-35,5=32-35,6=32-35,7=32-35,8=32-35,9=32-35,10=32-35,11=32-35,12=32-35,13=32-35,14=32-35,15=32-35,16=32-35,17=32-35,18=32-35,19=32-35</line><line>0=28-31,1=28-31,2=28-31,3=28-31,4=28-31,5=28-31,6=28-31,7=28-31,8=28-31,9=28-31,10=28-31,11=28-31,12=28-31,13=28-31,14=28-31,15=28-31,16=28-31,17=28-31,18=28-31,19=28-31</line><line>0=24-27,1=24-27,2=24-27,3=24-27,4=24-27,5=24-27,6=24-27,7=24-27,8=24-27,9=24-27,10=24-27,11=24-27,12=24-27,13=24-27,14=24-27,15=24-27,16=24-27,17=24-27,18=24-27,19=24-27</line><line>0=20-23,1=20-23,2=20-23,3=20-23,4=20-23,5=20-23,6=20-23,7=20-23,8=20-23,9=20-23,10=20-23,11=20-23,12=20-23,13=20-23,14=20-23,15=20-23,16=20-23,17=20-23,18=20-23,19=20-23</line><line>0=16-19,1=16-19,2=16-19,3=16-19,4=16-19,5=16-19,6=16-19,7=16-19,8=16-19,9=16-19,10=16-19,11=16-19,12=16-19,13=16-19,14=16-19,15=16-19,16=16-19,17=16-19,18=16-19,19=16-19</line><line>0=12-15,1=12-15,2=12-15,3=12-15,4=12-15,5=12-15,6=12-15,7=12-15,8=12-15,9=12-15,10=12-15,11=12-15,12=12-15,13=12-15,14=12-15,15=12-15,16=12-15,17=12-15,18=12-15,19=12-15</line><line>0=8-11,1=8-11,2=8-11,3=8-11,4=8-11,5=8-11,6=8-11,7=8-11,8=8-11,9=8-11,10=8-11,11=8-11,12=8-11,13=8-11,14=8-11,15=8-11,16=8-11,17=8-11,18=8-11,19=8-11</line><line>0=4-7,1=4-7,2=4-7,3=4-7,4=4-7,5=4-7,6=4-7,7=4-7,8=4-7,9=4-7,10=4-7,11=4-7,12=4-7,13=4-7,14=4-7,15=4-7,16=4-7,17=4-7,18=4-7,19=4-7</line>
        ///     <line>0=0-3,1=0-3,2=0-3,3=0-3,4=0-3,5=0-3,6=0-3,7=0-3,8=0-3,9=0-3,10=0-3,11=0-3,12=0-3,13=0-3,14=0-3,15=0-3,16=0-3,17=0-3,18=0-3,19=0-3</line>
        /// </layout>"        
        /// </summary>

        void pgmBib(SlotModel slot, XmlNode xml)
        {
            XmlNode node = xml.SelectSingleNode("layout");
            if (node == null) return;
            BIBModel layout = slot._layout;

            layout._xSize = Convert.ToInt32(node.Attributes["x"].Value);
            layout._ySize = Convert.ToInt32(node.Attributes["y"].Value);
            layout._ioCount = Convert.ToInt32(node.Attributes["io"].Value);
            layout._MultiChip = Convert.ToInt32(node.Attributes["m"].Value);

            layout._dutCnt = layout._xSize * layout._ySize;

            slot._dutModels.Clear();
            for (int i = 0; i < (layout._xSize * layout._ySize); i++)
            {
                DutModel dut = new DutModel() { Num = i + 1, Text = ""/*(i + 1).ToString()*/ };
                slot._dutModels.Add(dut);
                if (layout._MultiChip != 1) // Multi Chip 경우 Dut Hide
                {
                    if ((i % layout._MultiChip) != 0)
                        dut.Hide = true; 
                }
                slot._dutDatas.Add(new DutData());
            }

            XmlNodeList list = node.SelectNodes("line");
            int cnt = 1;
            foreach (XmlNode line in list)
            {
                string text = line.InnerText;
                string[] duts = text.Split(',');

                int yPos = layout._ySize - cnt;
                int xPos = 0;

                foreach (string dut in duts)
                {
                    int dutIdx = (layout._xSize * yPos) + xPos;
                    slot._dutModels[dutIdx].IO = dut;
                    xPos++;
                }
                cnt++;
            }

            if (Global.gMainVM.checkCurSlot(slot.Name))
                Global.gMainVM.updateUIFlag((int)UI.BIB);
        }

        /// <summary>
        /// OuterXml = "<info date=\"20220829T190023\" pgm=\"Nddr4_78n_320p.pgm\" lot=\"20220829_190014\" bid=\"0x1234\" />"
        /// </summary>
        void pgmInfo(SlotModel slot, XmlNode xml)
        {
            slot.LotID = xml.Attributes["lot"].Value;
            slot.PgmName = xml.Attributes["pgm"].Value;
            slot.BID = xml.Attributes["bid"].Value;
            string raw = xml.Attributes["date"].Value; 

            slot.StartTime = DateTime.ParseExact(
                    raw,
                    "yyyyMMdd'T'HHmmss",
                    System.Globalization.CultureInfo.InvariantCulture
                ).ToString("yyyy-MM-dd HH:mm:ss"); ;

            
            slot.EndTime = slot.StartTime;
        }

        void pgmStartTest(SlotModel slot, XmlNode xml)
        {
            string test = xml.Attributes["id"].Value;
            slot._contact = Convert.ToInt32(xml.Attributes["con"].Value) > 0 ? true : false;
            slot._endTest = false;
            slot.StepName = "";
            slot.StartTime = DateTime.Now.ToString();

            Global.gTestVM.addSlotLog(slot, "[Start-Test]");

            if (Global.gMainVM.checkCurSlot(slot.Name))
                Global.gMainVM.updateUIFlag((int)UI.INFO);
        }

        //public void sendWebData(SlotModel slot) 
        //{
        //    //if (slot._contact) return;
        //    StepModel last = slot._steps.Last();
        //
        //    DateTime unixStartTime = new DateTime(1970, 1, 1, 0, 0, 0, 0);
        //    TimeSpan stTimeSpan = Convert.ToDateTime(slot._startTime) - unixStartTime;
        //    TimeSpan edTimeSpan = Convert.ToDateTime(slot._endTime) - unixStartTime;
        //    string data = "<web-res>";
        //    data += $"<slot id=\"{slot.Name}\" con=\"{(slot._contact?1:0)}\" lot=\"{slot.LotID}\" pgm=\"{slot.PgmName}\" bib=\"{slot.BID}\" " +
        //        $"st=\"{Convert.ToUInt64(stTimeSpan.TotalSeconds)}\" ed=\"{Convert.ToUInt64(edTimeSpan.TotalSeconds)}\" " +
        //        $"m=\"{last._testCnt}\" p=\"{last._totalCnt}\" f=\"{last._testCnt - last._totalCnt}\"/>";
        //    data += "</web-res>";
        //
        //    _client.sendCmd(MSG_TYPE.DEST_ROUTER, "", "", data);
        //}

        public void sendDBTime(SlotModel slot) 
        {
            try
            {
                DateTime st = Convert.ToDateTime(slot.StartTime);
                DateTime ed = Convert.ToDateTime(slot.EndTime);

                TimeSpan dur = ed - st;

                string query = "insert slotactive (equip, pc, slot, startT, endT, durT)";
                query += $"value(\'{_dbCtrl.EquipID}\'," +
                    $"\'{_dbCtrl.PCID}\'," +
                    $"\'{slot.Name}\'," +
                    $"\'{st.ToString("yyyy-MM-dd HH:mm:ss")}\'," +
                    $"\'{ed.ToString("yyyy-MM-dd HH:mm:ss")}\'," +
                    $"\'{dur.TotalSeconds}\')";
                _dbCtrl.push(query);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            
        }

        public void sendDBData(SlotModel slot) 
        {
            if (!_dbCtrl._useDB) return;
            sendDBTime(slot);

            if (slot._contact) return;

            DateTime st = Convert.ToDateTime(slot.StartTime);
            DateTime ed = Convert.ToDateTime(slot.EndTime);
            StepModel last = slot._steps.Last();

            string query = "insert main(lot,equip,pc,slot,bid,pgm,startT,endT,total,mount,pass,fail,layout,mdata,tdata,bdata)";
            query += $"value(\'{slot.LotID}\'," + // lot
                $"\'{_dbCtrl.EquipID}\'," +  // equip
                $"\'{_dbCtrl.PCID}\'," +     // pc
                $"\'{slot.Name}\'," +       // slot
                $"\'{slot.BID}\'," +        // bid
                $"\'{slot.PgmName}\'," +    // pgm
                $"\'{st.ToString("yyyy-MM-dd HH:mm:ss")}\'," +  // startT
                $"\'{ed.ToString("yyyy-MM-dd HH:mm:ss")}\'," +    // endT
                $"\'{slot._dutModels.Count}\'," + // total
                $"\'{last._testCnt}\'," + //mount
                $"\'{last._totalCnt}\'," + //pass
                $"\'{last._testCnt - last._totalCnt}\'," + // fail
                $"\'{slot._layout._xSize},{slot._layout._ySize}, {slot._layout._ioCount}, {slot._layout._MultiChip} \',";

            string name = last.Name;
            uint mValue = 0;
            string mData = "";
            uint tValue = 0;
            string tData = "";
            string bData = "";
            for (int i = 0; i<slot._dutDatas.Count; i++) {

                if (i != 0 && ((i%32) == 0)) {
                    mData += mValue.ToString("X") + ",";
                    mValue = 0;
                    tData += tValue.ToString("X") + ",";
                    tValue = 0;
                }
                if (slot._dutDatas[i]._conDic[name])
                    mValue |= (uint)(1 << (i % 32));
                if (slot._dutDatas[i]._totalDic[name])
                    tValue |= (uint)(1 << (i % 32));

                bData += slot._dutModels[i].Bin + ",";
            }
            mData += mValue.ToString("X");            
            tData += tValue.ToString("X");

            query += $"\'{mData}\', " +
                $"\'{tData}\'," +
                $"\'{bData}\')";

            _dbCtrl.push(query);


        }

        public void saveLoaderFile(SlotModel slot)
        {
            if (Global.gLoaderUse != 1) return;
            if (slot._contact) return;
            
            string fileName = Global.gLoaderPath + "\\" 
                + $"{DateTime.Now.ToString("yyyyMMddHHmm")}.{slot._numBid.ToString("X8")}.txt";

            try
            {

                FileStream fs = new FileStream(fileName, FileMode.Create); ;
                StreamWriter sw = new StreamWriter(fs);

                //slot._dutModels[0].
                for (int i = 0; i< slot._dutModels.Count; ) 
                {
                    bool passBin = true;
                    if (i != 0) sw.Write(",");

                    for (int j = 0; j < slot._layout._MultiChip; j++)
                    {
                        StepModel last = slot._steps.Last();

                        if (!slot._dutDatas[i]._conDic[slot._steps.Last().Name])
                        {
                            sw.Write("21");
                            i++;
                            continue;
                        }


                        int numBin =  Convert.ToInt32(slot._dutModels[i].Bin);
                        if (numBin <= 4)
                        {
                            sw.Write(numBin.ToString("D2"));
                        }
                        else if (numBin <= 8)
                        {
                            sw.Write($"F{numBin}");
                        }
                        else
                        {
                            sw.Write("21");
                        }
                        
                        i++;
                    }


                }
                sw.Close();
            }catch (Exception e)
            {
                Global.gTestVM.addSysLog(LOG_TYPE.ERROR, slot.Name, $"[LOADER] {e.Message}");
            }

        }


        public void pgmEndTest(SlotModel slot, XmlNode xml)
        {
            slot.StepName = "";
            slot._endTest = true;
            slot.EndTime = DateTime.Now.ToString();
            Global.gTestVM.addSlotLog(slot, "[End-Test]");

            if (slot.LotID != "")
            {
                string srcFile = "/star/result/result.xml";
                string saveFile = string.Format($"{Global.gDirResult}\\{slot.LotID}_{slot.Name}.xml");
                _client.sendReqFile(slot.Name, srcFile, saveFile);

                // board temp log
                string fn = $@"\\{slot._ip}\star\result\btemp.csv";
                saveFile = string.Format($"{Global.gDirSystem}\\{slot.LotID}_" +
                    $"{slot.Name}" +
                    $"{(slot._contact? "_C" : "")}.csv");
                Console.WriteLine($"PUSH [{_copyFiles.Count}] = fn : {fn}, save : {saveFile}");
                _copyFiles.Enqueue(fn + ":" + saveFile);

                Console.WriteLine($"thread st {_copyThread.ThreadState}");
                if (_copyThread.ThreadState == ThreadState.Unstarted)
                    _copyThread.Start(this);
                else if (_copyThread.ThreadState == ThreadState.Stopped)
                {
                    _copyThread = new Thread(threadCopy);
                    _copyThread.Start(this);
                }
            }

            if (Global.gMainVM.checkCurSlot(slot.Name))
                Global.gMainVM.updateUIFlag((int)UI.INFO);

            //sendWebData(slot);
            sendDBData(slot);

            //saveLoaderFile(slot);
            Task.Run( ()=> saveLoaderFile(slot));
        }

        void pgmStartStep(SlotModel slot, XmlNode xml)
        {
            string step = xml.Attributes["id"].Value;
            slot.StepName = step;
            slot.PwrMon = true;
            slot.StepType = STEP_TYPE.NORMAL;
            slot.setPowerLogStep(step);

            slot._pwrGraphIndex = 0;

            Global.gTestVM.addSlotLog(slot, "[Start-Step]");
        }

        void pgmEndStep(SlotModel slot, XmlNode xml)
        {
            slot._scanProc = 100;
            slot._timerAging.Stop();

            if (Global.gMainVM.checkCurSlot(slot.Name))
                Global.gTestVM.ScanProg = slot._scanProc;

            slot.setPowerLogStep(slot.StepName, false);            
            Global.gTestVM.addSlotLog(slot, "[End-Step]");
        }

        ///
        ///  <step id=\"INIT\" con=\"1\" start=\"20220830T130205\" end=\"20220830T130207\">
        ///       <con>0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,</con>
        ///       <cur>0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffff...
        ///  </step>
        void pgmResult(SlotModel slot, XmlNode xml)
        {
            if (xml.FirstChild == null) 
                return;

        
            StepModel step = new StepModel()
            {
                Name = xml.Attributes["id"].Value,
                Start = xml.Attributes["start"].Value,
                End = ""
            };
            if (xml.Attributes["end"] != null)
                step.End = xml.Attributes["end"].Value;
            if (Convert.ToInt32(xml.Attributes["con"].Value) == 1)
                step.IsContact = true;

            step.Start = MConv.convertTimeText(step.Start);
            step.End = MConv.convertTimeText(step.End);
            DateTime st = Convert.ToDateTime(step.Start);
            DateTime ed = Convert.ToDateTime(step.End);
            TimeSpan dur = ed - st;

            step.Start = st.ToString("ddDHH:mm:ss");
            step.End = ed.ToString("ddDHH:mm:ss");
            step.Duration = dur.ToString();


            slot._steps.Add(step);

            XmlNode pfNode = xml.SelectSingleNode("con");
            if (pfNode != null)
            {
                int idx = 0;
                string[] buf = pfNode.InnerText.Split(',');
                foreach (string str in buf)
                {
                    if (str == "") break;
                    int pf = Convert.ToInt32(str, 16);
                    for (int i = 0; i < 32; i++)
                    {
                        bool isPass = (pf & (1 << i)) == 0 ? false : true;
                        if (isPass) step._passCnt++;
                        slot._dutDatas[idx]._conDic[step.Name] = isPass;
                        idx++;
                        if (idx >= slot._dutModels.Count) break;
                    }
                    if (idx >= slot._dutModels.Count) break;
                }
            }
            //if (!step.IsContact)
            step._testCnt = step._passCnt;

            step._passCnt = 0;
            pfNode = xml.SelectSingleNode("cur");
            if (pfNode != null)
            {
                int idx = 0;
                string[] buf = pfNode.InnerText.Split(',');
                foreach (string str in buf)
                {
                    if (str == "") break;
                    int pf = Convert.ToInt32(str, 16);
                    for (int i = 0; i < 32; i++)
                    {
                        bool isPass = (pf & (1 << i)) == 0 ? false : true;
                        if (isPass) step._passCnt++;
                        slot._dutDatas[idx]._curDic[step.Name] = isPass;
                        idx++;
                        if (idx >= slot._dutModels.Count) break;
                    }
                    if (idx >= slot._dutModels.Count) break;
                }
            }

            step._totalCnt = 0;
            pfNode = xml.SelectSingleNode("total");
            if (pfNode != null)
            {
                int idx = 0;
                string[] buf = pfNode.InnerText.Split(',');
                foreach (string str in buf)
                {
                    if (str == "") break;
                    int pf = Convert.ToInt32(str, 16);
                    for (int i = 0; i < 32; i++)
                    {
                        bool isPass = (pf & (1 << i)) == 0 ? false : true;
                        if (isPass) step._totalCnt++;
                        slot._dutDatas[idx]._totalDic[step.Name] = isPass;
                        idx++;
                        if (idx >= slot._dutModels.Count) break;
                    }
                    if (idx >= slot._dutModels.Count) break;
                }
            }
            step.Yeild = String.Format($"{step._passCnt}/{step._testCnt} T:{step._totalCnt}");

            if (Global.gMainVM.checkCurSlot(slot.Name))
            {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.addStep(step)
                    ));
                Global.gMainVM.updateUIFlag((int)UI.INFO);
            }

        }


        void recvPowerSet(SlotModel slot, XmlNode xml)
        {
            XmlNodeList list = xml.ChildNodes;

            string dps = string.Empty;
            string hv = string.Empty;
            string dac = string.Empty;
            
            foreach (XmlNode node in list)
            {
                if (node.Name == "dps")                
                    dps += node.InnerText;
                else if(node.Name == "hv")
                    hv += node.InnerText;
                else if(node.Name == "dac")
                    dac += node.InnerText;
			}

			string[] dpsArr = dps.Split(',');
            string[] hvArr = hv.Split(',');
            string[] dacArr = dac.Split(',');

			int i = 0;
            if (dpsArr.Length < PowerVM.DPS_SIZE * 2) return;
            for (i = 0; i< PowerVM.DPS_SIZE; i++)
            {
                slot._DPSs[i].SetV  = dpsArr[i * 2];
                slot._DPSs[i].OC    = dpsArr[i * 2 +1];
            }

            if(hvArr.Length < PowerVM.HV_SIZE * 2) return;
            for(i = 0; i < PowerVM.HV_SIZE; i++)
            {
                slot._HVs[i].SetV = hvArr[i * 2];
                slot._HVs[i].OC   = hvArr[i * 2 +1];
            }

            if(dacArr.Length < PowerVM.DAC_SIZE) return;
            for(i = 0; i < PowerVM.DAC_SIZE; i++)
                slot._DACs[i].SetV = dacArr[i];

            slot.addPowerLogSet();

            if (Global.gMainVM.checkCurSlot(slot.Name))  //message 받은것 ui Model에 바로 update 안하면 index 틀어질수있음.
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gPowerVM.updatePowerSet(Global.gSlotVM.CurSlot)));
        }


        void recvPowerRead(SlotModel slot, XmlNode xml)
        {
            XmlNodeList list = xml.ChildNodes;

            string dps = string.Empty;
            string hv = string.Empty;

            foreach (XmlNode node in list)
            {
                if (node.Name == "dps")
                    dps = node.InnerText;                
                else if(node.Name == "hv")
                    hv = node.InnerText;
            }

            string[] dpsArr = dps.Split(',');
            string[] hvArr = hv.Split(',');

            int i = 0;
            if (dpsArr.Length < PowerVM.DPS_SIZE * 2) return;
            for (i = 0; i < PowerVM.DPS_SIZE; i++)
            {
                slot._DPSs[i].GetV = dpsArr[i * 2];
                slot._DPSs[i].GetC = dpsArr[i * 2 + 1];

                slot._DPSs[i].setData(Convert.ToDouble(slot._DPSs[i].GetV),
                    Convert.ToDouble(slot._DPSs[i].GetC), slot._pwrGraphIndex);


            }

            if(hvArr.Length < PowerVM.HV_SIZE * 2) return;
            for(i = 0; i < PowerVM.HV_SIZE; i++)
            {
                slot._HVs[i].GetV = hvArr[i * 2];
                slot._HVs[i].GetC = hvArr[i * 2 + 1];

                slot._HVs[i].setData(Convert.ToDouble(slot._HVs[i].GetV),
                    Convert.ToDouble(slot._HVs[i].GetC), slot._pwrGraphIndex);
            }

            slot.addPowerLog();

            slot._pwrGraphIndex++;
            if (slot._pwrGraphIndex >= 200_000)
                slot._pwrGraphIndex = 0;

            if (Global.gMainVM.checkCurSlot(slot.Name))  //message 받은것 ui Model에 바로 update 안하면 index 틀어질수있음.
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gPowerVM.updatePower(Global.gSlotVM.CurSlot)));
        }

        void pgmPower(SlotModel slot, XmlNode xml)
        {
            XmlNodeList list = xml.ChildNodes;

            foreach (XmlNode node in list)
            {
                if (node.Name == "set")
                    recvPowerSet(slot, node);
                else if (node.Name == "read")
                    recvPowerRead(slot, node);
            }

        }

        void pgmScanProc(SlotModel slot, XmlNode xml)
        {
            int cur = Convert.ToInt32(xml.Attributes["cur"].Value);
            int max = Convert.ToInt32(xml.Attributes["max"].Value);
            bool all = Convert.ToInt32(xml.Attributes["all"].Value) > 0 ? true : false ;            
            if (max == 0)
                slot._scanProc = 0;
            else
            {
                if (!(slot.StepType == STEP_TYPE.NORMAL ||
                    slot.StepType == STEP_TYPE.ALL_SCAN)) return;

                if (cur > max)cur = max;

                if (all)
                {
                    slot._scanProc = 50;
                    slot.StepType = STEP_TYPE.ALL_SCAN;
                }
                else
                {
                    slot._scanProc = (cur * 100) / max;
                    slot.StepType = STEP_TYPE.NORMAL;
                }

                if (Global.gMainVM.checkCurSlot(slot.Name))  
                    Global.gTestVM.ScanProg = slot._scanProc;
            }


        }

        void pgmStepList(SlotModel slot, XmlNode xml)
        {
            slot._stepList.Clear();

            string list = xml.InnerText;
            string[] steps = list.Split(',');
            foreach (string step in steps)
            {
                if (step == "") continue;
                slot._stepList.Add(step);
            }

            if (Global.gMainVM.checkCurSlot(slot.Name))
                Global.gMainVM.updateUIFlag((int)UI.STEPLIST);
        }

        void pgmMessage(SlotModel slot, XmlNode xml)
        {
            LOG_TYPE type = (LOG_TYPE)Convert.ToInt32(xml.Attributes["type"].Value);
            string msg = xml.InnerText;
            Global.gTestVM.addSlotLog(slot, "[Message] " + msg, type);
        }

        void pgmAging(SlotModel slot, XmlNode xml)
        {
            // aging = 1, sock = 2
            int soak = Convert.ToInt32(xml.Attributes["sock"].Value);
            slot._agingTime = Convert.ToInt32(xml.Attributes["time"].Value);
            if (soak == 1) 
                slot.StepType = STEP_TYPE.AGING;
            else if (soak == 2)
                slot.StepType = STEP_TYPE.SOAK_TIME;

            int sec = slot._agingTime % 60; // sec
            int min = (slot._agingTime / 60) % 60; // min
            int hour = slot._agingTime / 3600; // Hour 

            string addStr = "  " + 
                ((hour == 0) ? "" : string.Format($"{hour}:")) +
                string.Format($"{min}:{sec}");
            slot.MeThod += addStr;

            slot._agingCnt = 0;
            slot._timerAging.Start();        
        }

        void pgmTemp(SlotModel slot, XmlNode xml)
        {
            string cmd = xml.Attributes["cmd"].Value;  
            switch (cmd)
            {
                case "set":
                    double target = Convert.ToDouble(xml.Attributes["sv"].Value);
                     slot._plcModel._modbus.setTempFunc(target);                    
                    bool ret = slot._plcModel._modbus.setOperationFunc(PLC_STAT_TYPE.RUN);
                    if (ret == false)
                        slot._plcModel._modbus.setOperationFunc(PLC_STAT_TYPE.RUN);
                    break;
                case "stop":
                    slot._plcModel._modbus.setOperationFunc(PLC_STAT_TYPE.STOP);
                    break;
                case "ambient":
                    slot._plcModel._modbus.setOperationFunc(PLC_STAT_TYPE.AMBI);
                    break;
                case "req":
                    string text = string.Format("<temp st=\"{0}\" pv=\"{1:F1}\" sv=\"{2:F1}\" />",
                        (int)slot._plcModel.Stat, slot._plcModel.CurTemp, slot._plcModel.TargetTemp);
                    _client.sendCmd((int)MSG_TYPE.DEST_PGM, slot.Name, "temp", text);
                    break;
            }
        }

        void pgmFialBit(SlotModel slot, XmlNode xml)
        {
            XmlNodeList list = xml.ChildNodes;

            foreach (XmlNode node in list)
            {
                int no = Convert.ToInt32(node.Attributes["no"].Value);
                int fb = Convert.ToInt32(node.Attributes["fb"].Value, 16);
                slot._dutDatas[no]._fBit[slot.StepName] = fb;
            }
            if (Global.gMainVM.checkCurSlot(slot.Name) && Global.gResultVM.TextMode == 3)
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.setDutText()
                    ));
        }

        void pgmBin(SlotModel slot, XmlNode xml)
        {
            string str = xml.InnerText;
            string[] toks = str.Split(',');
            int no = 0;

            slot._binGathering.Clear();            
            foreach (string tok in toks)
            {
                slot._dutModels[no].Bin = tok;                
                
                int bin = Convert.ToInt32(tok);
                if (slot._binGathering.ContainsKey(bin))
                    slot._binGathering[bin]++;
                else
                    slot._binGathering.Add(bin, 1);

                no++;
                if (no >= slot._dutModels.Count) break;
            }

            if (Global.gMainVM.checkCurSlot(slot.Name)) {
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => {
                            if (Global.gResultVM.TextMode == 5)
                                Global.gResultVM.setDutText();
                            Global.gResultVM.setBinList();
                        }));
                
            }
                        
                    

            
        }

        void pgmChipID(SlotModel slot, XmlNode xml)
        {
            XmlNodeList list = xml.ChildNodes;

            foreach (XmlNode node in list)
            {
                int no = Convert.ToInt32(node.Attributes["no"].Value);
                string chip = node.InnerText;
                slot._dutModels[no].ChipID = chip;
            }
            if (Global.gMainVM.checkCurSlot(slot.Name) && Global.gResultVM.TextMode == 4)
                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gResultVM.setDutText()
                    ));

        }


        
        public Queue<string> _copyFiles = new Queue<string>();
        public static Thread _copyThread = new Thread(threadCopy);
        public static void threadCopy(object parent)
        {
            Net net = parent as Net;

            while (Net._close == false)
            {
                if (net._copyFiles.Count > 0)
                {
                    string str = net._copyFiles.Dequeue();
                    int pos  = str.IndexOf(":");
                    string fn = str.Substring(0, pos);
                    string save = str.Substring(pos + 1);
                    if (!File.Exists(fn)) continue;
                    Console.WriteLine($"POP [{net._copyFiles.Count}] = fn : {fn}, save : {save}");
                    File.Copy(fn, save, overwrite: true);                                    
                }
                else
                {
                    break;
                }
            }

        }


        void pgmSub(SlotModel slot, XmlNode xml)
        {
            string type = xml.Attributes["type"].Value;
            string lot = xml.Attributes["lot"].Value;
            string id = xml.Attributes["id"].Value;

            StepModel step = slot.findStep(id);
            if (step != null)
            {
                step._subType = StepModel.findSubType(type);
                if (Global.gMainVM.checkCurSlot(slot.Name))
                {
                    StepModel curStep = Global.gResultVM.StepModels.FirstOrDefault(a => a.Name == id);
                    if (curStep != null) curStep._subType = step._subType;
                    
                }
            }
            Console.WriteLine(String.Format($"{slot.Name} {step.Name} {step._subType.ToString()}"));

            if (step._subType == SUB_TYPE.NONE ) return;
            if (step._subType == SUB_TYPE.DATA)
            {                
                string[] buf = xml.InnerText.Split(',');
                foreach (string s in buf)
                {
                    if (s == "") break;
                    int dno = Convert.ToInt32(s);
                    string fn = $@"\\{slot._ip}\star\result\data\D{dno.ToString("D4")}.bin";
                    string saveFile = string.Format($"{Global.gDirData}\\{lot}_{id}_{slot.Name}_D{dno.ToString("D4")}.bin");
                    Console.WriteLine($"PUSH [{_copyFiles.Count}] = fn : {fn}, save : {saveFile}");
                    _copyFiles.Enqueue(fn + ":" + saveFile);
                }
            }
            else if (step._subType == SUB_TYPE.MEAS_DATA)
            {
                string[] buf = xml.InnerText.Split(',');
                foreach (string s in buf)
                {
                    if (s == "") break;
                    int dno = Convert.ToInt32(s);
                    string fn = $@"\\{slot._ip}\star\result\data\D{dno.ToString("D4")}.csv";
                    string saveFile = string.Format($"{Global.gDirData}\\{lot}_{id}_{slot.Name}_D{dno.ToString("D4")}.csv");
                    Console.WriteLine($"PUSH [{_copyFiles.Count}] = fn : {fn}, save : {saveFile}");
                    _copyFiles.Enqueue(fn + ":" + saveFile);
                }

            }
            else if (step._subType == SUB_TYPE.CYCLE)
            {
                string fn = $@"\\{slot._ip}\star\result\sub.txt";
                string saveFile = string.Format($"{Global.gDirSub}\\{lot}_{id}_{slot.Name}.csv");
                Console.WriteLine($"PUSH [{_copyFiles.Count}] = fn : {fn}, save : {saveFile}");
                _copyFiles.Enqueue(fn + ":" + saveFile);
            }
            else
            {
                string fn = $@"\\{slot._ip}\star\result\sub.xml";                
                string saveFile = string.Format($"{Global.gDirSub}\\{lot}_{id}_{slot.Name}.xml");
                Console.WriteLine($"PUSH [{_copyFiles.Count}] = fn : {fn}, save : {saveFile}");
                _copyFiles.Enqueue(fn + ":" + saveFile);
                
             
            }
            Console.WriteLine($"thread st {_copyThread.ThreadState}");
            if (_copyThread.ThreadState == ThreadState.Unstarted)
                _copyThread.Start(this);
            else if (_copyThread.ThreadState == ThreadState.Stopped)
            {
                _copyThread = new Thread(threadCopy);
                _copyThread.Start(this);
            }
            //Task.Run(() => File.Copy(fn, saveFile, overwrite: true));
            //Task t = new Task(() => File.Copy(fn, saveFile, overwrite: true));  
            //t.Start();
            //Task.Run(  File.Copy(fn, saveFile, overwrite: true));
        }

        



        void doCmdPGM(XmlNode xml)
        {
            if (xml.Attributes["path"] == null) return;
            string cmID = xml.Attributes["path"].Value.Trim();
            SlotModel slot = Global.gSlotVM.getSlot(cmID);
            if (slot == null) return;

            XmlNodeList list = xml.SelectSingleNode("result").ChildNodes;
            foreach (XmlNode node in list)
            {
                string work = node.Name.Trim();
                switch (work)
                {
                    case "bib": pgmBib(slot, node); break;
                    case "info": pgmInfo(slot, node); break;
                    case "flow": break;
                    case "start-test": pgmStartTest(slot, node); break; 
                    case "start-step": pgmStartStep(slot, node); break;
                    case "step": pgmResult(slot, node); break;
                    case "fbit": pgmFialBit(slot, node); break;
                    case "bin":  pgmBin(slot, node); break;
                    case "end-step": pgmEndStep(slot, node); break;
                    case "end-test": pgmEndTest(slot, node); break;
                    case "power-ex": pgmPower(slot, node); break;
                    case "msg": pgmMessage(slot, node); break;
                    case "chipid": pgmChipID(slot, node); break;
                    case "aging": pgmAging(slot, node); break;
                    case "temp": pgmTemp(slot, node); break;
                    case "scan-proc": pgmScanProc(slot, node); break;
                    case "step-list": pgmStepList(slot, node); break;
                    case "sub": pgmSub(slot, node); break;

                    default:         break;
                }
            }
        }

        #endregion

        void makeSelection()
        {
            if (Global.gSlotVM.SelectSlots.Count != 0)return;

            foreach (SlotModel slot in Global.gSlotVM.Slots)
            {
                if (slot.StatType == STAT_TYPE.RUNNING)
                {
                    Global.gSlotVM.SelectSlots.Add(slot);
                    Global.gSlotVM.CurSlot = Global.gSlotVM.SelectSlots[0];
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                            new Action(() =>
                            {
                                Global.gPowerVM.setSlot(Global.gSlotVM.CurSlot);
                            }));

                    Global.gMainVM.updateUIFlag((int)UI.MENU | (int)UI.INFO | (int)UI.SLOTLOG
                        | (int)UI.STEPLIST | (int)UI.POWER | (int)UI.RESULT | (int)UI.REGISTER);


                    if (Global.gSlotVM.CurSlot._plcModel != Global.gChamberVM.CurPLC)
                    {
                        Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                            new Action(() =>
                            {
                                Global.gChamberVM.CurPLC = Global.gSlotVM.CurSlot._plcModel;
                                Global.gChamberVM.setTempData();
                            }));
                    }
                    break;
                }                 
            }
        }


        void doCUList(XmlNode xml)
        {
            
            XmlNodeList listCM = xml.SelectNodes("result/cu-list/cu");
            //Console.WriteLine($"CULIST - {xml.OuterXml}");
            
            int cnt = 0;                        
            foreach (XmlNode nodeCM in listCM)
            {
                if (nodeCM == null) continue;
                string cmID = nodeCM.Attributes["id"].Value;
                string ip = nodeCM.Attributes["ip"].Value;
                string bid = nodeCM.Attributes["bid"].Value;
                bool isOnline = Convert.ToInt32(nodeCM.Attributes["online"].Value) == 1 ? true : false;
                bool isPGM = Convert.ToInt32(nodeCM.Attributes["pgm"].Value) == 1 ? true : false;
                bool isDiag = Convert.ToInt32(nodeCM.Attributes["diag"].Value) == 1 ? true : false;

                if (Global.gSlotVM.Slots.Count <= cnt)
                {
                    Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gSlotVM.addSlot(cmID, ip)));            
                    if (isOnline && isPGM)
                    {
                        string srcFile = "/star/result/result.xml";
                        string saveFile = string.Format($"{Global.gDirResult}\\NowTest_{cmID}.xml");
                        _client.sendReqFile(cmID, srcFile, saveFile);

                        Task.Run(async () => {
                             await Task.Delay(5000); // 5초 후 selection 수행
                            makeSelection();
                        });
                    }
                        
                }
                cnt++;

                Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal,
                    new Action(() => Global.gSlotVM.setSlot(cmID, bid, isOnline, isPGM, isDiag)));
            }
            
        }
        
        /*
         * <slot-stat bid=\"0x%X\" pgm=\"%d\" diag=\"%d\" >
         *      <run-step lotid=\"%s\" con=\"%d\" path=\"%s\" name=\"%s\" st=\"%s\" flow=\"%s\" step=\"%s\" />
         * </slot-stat>
        */
        void doSlotStat(XmlNode xml)
        {
            if (xml.Attributes["path"] == null) return;
            string cmID = xml.Attributes["path"].Value.Trim();

            XmlNode nodeStat = xml.SelectSingleNode("result/slot-stat");
            if (nodeStat == null) return;

            Global.gSlotVM.setSlotDetail(cmID, nodeStat);

        }

        /*
         * <star-p path = "Slot02 " >< result seq="0" cmd="SEND-FILE" return="OK" type="xml"></result></star-p>
         */
        void doSendFile(XmlNode xml)
        {
            if (xml.Attributes["path"] == null) return;
            string cmID = xml.Attributes["path"].Value.Trim();

            XmlNode node = xml.SelectSingleNode("result");
            string ret = node.Attributes["return"].Value;

            SlotModel slot =  Global.gSlotVM.getSlot(cmID);
            if (slot == null) return;

            if (ret != "OK")
            {
                slot._sendFileProcess.stop();
                slot.StatType = STAT_TYPE.ERROR;
                Global.gTestVM.addSysLog(LOG_TYPE.ERROR, slot.Name, "[SEND-FILE] Error");
            }

            // contact ,  run 종료시 send file ok가 왜 오는지.. ??
            if (slot.StatType == STAT_TYPE.CONTACTING || slot.StatType == STAT_TYPE.RUNNING) return;
            

            if (slot._sendFileProcess.isRunning)
            {
                slot._sendFileProcess.next();
                return;
            }else
            {
                if (slot._fileSet == null) 
                    return;
                slot._fileSet._sendIndex++;
                if (slot.isSendFileRemind())
                    Global.gMainVM.loadFileSet(slot);
                else
                {

                    slot.StatType = STAT_TYPE.LOAD;
                    Global.gMainVM.updateUIFlag((int)UI.INFO | (int)UI.MENU);                    
                    Global.gTestVM.addSysLog(LOG_TYPE.INFO, slot.Name, "[Load] " + slot._fileSet._excute);

                }
            }

        }

        void doPlcConfig(XmlNode xml)
        {
            XmlNode node = xml.SelectSingleNode("result/plc-config");
            XmlNodeList list = node.ChildNodes;
            foreach(XmlNode plcNode in list)
            {
                
                if (plcNode.Name == "plc")
                {
                    string id = plcNode.Attributes["id"].Value;
                    string ip = plcNode.Attributes["ip"].Value;
                    int port = Convert.ToInt32(plcNode.Attributes["port"].Value);
                    PLCModel plc = Global.gChamberVM.getPLC(id);
                    if (plc == null)
                    {
                        plc = new PLCModel();
                        Global.gChamberVM._plcs.Add(plc);
                        plc.Name = id;
                    }
                    plc._modbus._plcIP = ip;
                    plc._modbus._port = port;
                    Thread.Sleep(200); // slot 구성 전 sync 때문

                }
                
                if (plcNode.Name == "zone")
                {
                    XmlNodeList slotNodes = plcNode.ChildNodes;
                    PLCModel plc = Global.gChamberVM.getPLC(plcNode.Attributes["plc"].Value);
                    if (plc == null) continue;
                    foreach (XmlNode slotNode in slotNodes)
                    {
                        string slotName = slotNode.Attributes["id"].Value;
                        SlotModel slot = Global.gSlotVM.getSlot(slotName);
                        if (slot == null) continue;
                        slot._plcModel = plc;
                    }
                }
            }
        }

        void doCmdDiag(XmlNode xml)
        {
            if (xml.Attributes["path"] == null) return;
            string cmID = xml.Attributes["path"].Value.Trim();
            SlotModel slot = Global.gSlotVM.getSlot(cmID);
            if (slot == null) return;

            XmlNodeList list = xml.SelectSingleNode("result").ChildNodes;
            foreach (XmlNode node in list)
            {
                string msg = node.Name.Trim();
                if (msg != "msg") continue;
                LOG_TYPE type = (LOG_TYPE)Convert.ToInt32(node.Attributes["type"].Value);
                
                Global.gTestVM.addSysLog(type, slot.Name, xml.InnerText);
                
                if (type == LOG_TYPE.REQUEST)
                {
                    Global.gTestVM.ShowReqInfo = Visibility.Visible;
                    Global.gTestVM.ReqMessage = $"Insert Cal-Board : {slot.Name} &#10; {msg}";
                }else if (type == LOG_TYPE.REQUEST_COMPLATE)
                {
                    Global.gTestVM.ShowReqInfo = Visibility.Hidden;
                }
            }
        }

        void doCmdProcess(XmlNode xml)
        {

        }


        void doResult(RESULT_CMD type, XmlNode xml)
        {
//            Console.WriteLine(type.ToString());
            try
            {
                switch (type)
                {
                    case RESULT_CMD.CMD_CU_LIST:
                    case RESULT_CMD.CMD_CU_LIST_AUTO:   doCUList(xml); break;
                    case RESULT_CMD.CMD_STATUS:         doSlotStat(xml); break;
                    case RESULT_CMD.CMD_SEND_FILE:      doSendFile(xml); break;
                    case RESULT_CMD.CMD_PLC_CONFIG:     doPlcConfig(xml); break;
                    case RESULT_CMD.CMD_PROC:           doCmdProcess(xml); break;
                    case RESULT_CMD.CMD_PGM:            doCmdPGM(xml); break;
                    case RESULT_CMD.CMD_DIAG:           doCmdDiag(xml); break;


                    case RESULT_CMD.CMD_ROM:          //  doRom(xml); break;
                    case RESULT_CMD.CMD_CM_CTRL:      //  doCmCtrl(xml); break;
                    case RESULT_CMD.CMD_REQ_FILE:     //  -> REQ File Process 에서 따로 처리
                    default: break;
                }
            }
            catch (Exception ex)
            { 
                Console.WriteLine(ex.ToString()); 
            }
        }

        #region Main Thread 처리.. Que로 Serial Process이므로 병렬처리 필요없음
        /*        
                async void processResult(RESULT_CMD type, XmlNode xml)
                {            
                    Task t = Task.Run(() => doResult(type, xml));
                    await t;

                }
        */
        #endregion

        
        void processResult(RESULT_CMD type, XmlNode xml)
        {
            doResult(type, xml);
        }


        public void recvNowResult(int slotIndex, string path)
        {
            XmlDocument doc = new XmlDocument();
            try
            {
                doc.Load(path);
                XmlNode node = doc.SelectSingleNode("/test-result/info");                
                if (node == null) return;
                SlotModel slot = Global.gSlotVM.Slots[slotIndex];
                pgmInfo(slot, node);
                
                node = doc.SelectSingleNode("/test-result/bib");
                if (node == null) return;
                pgmBib(slot, node);

                node = doc.SelectSingleNode("/test-result/step-list");
                if (node == null) return;
                string stepsText = node.InnerText;

                string[] steps = stepsText.Split(',');
                slot._stepList.Clear();
                foreach (string step in steps)
                {
                    if (step == "") continue;
                    slot._stepList.Add(step);
                }

                var nodes = doc.SelectNodes("/test-result/step");
                foreach (XmlNode nodeStep in nodes)
                {
                    pgmResult(slot, nodeStep);
                }




            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

        }

        void processRecvFile(string xmlText)
        {
            int pos = xmlText.IndexOf("path=");
            string slotName = xmlText.Substring(pos + 6, 6);

            int st = xmlText.IndexOf("name=\"") + 6;
            int end = xmlText.IndexOf("\"", st) - st;
            string fileName = xmlText.Substring(st, end);

            st = xmlText.IndexOf("ofs", pos) + 5;
            end = xmlText.IndexOf("\"", st) - st;
            int ofs = Convert.ToInt32(xmlText.Substring(st, end));


            pos = xmlText.IndexOf("send-file");
            st = xmlText.IndexOf(">", pos) + 1;
            end = xmlText.IndexOf("</send-file>") - st;
            if (end < 0) return;

            string text = xmlText.Substring(st, end);


            if (ofs == -1)
            {
                if (fileName.IndexOf("NowTest") != -1)
                {
                    int index = fileName.IndexOf("Slot");
                    if (index != -1 && index + 4 + 2 <= fileName.Length)
                    {
                        string numberStr = fileName.Substring(index + 4, 2); // "05"
                        if (int.TryParse(numberStr, out int slotNumber))
                        {
                            slotNumber -= 1;
                            if (slotNumber >= 0)
                                Task.Run(() => recvNowResult(slotNumber, fileName));
                        }
                    }
                }
            }
            else if (ofs == 0)
            {
                //                File.WriteAllText(fileName, text, Encoding.Default);
                FileStream fs = new FileStream(fileName, FileMode.Create);
                StreamWriter sw = new StreamWriter(fs);
                sw.Write(text);
                sw.Close();

            }
            else
            {
                StreamWriter sw = File.AppendText(fileName);
                sw.Write(text);
                sw.Close();

            }

            _client.sendResponse(slotName, "SEND-FILE", true);

        }

        void processRecvBinary(string xmlText, byte[] buffer)
        {
            Console.WriteLine(xmlText);
            int pos = xmlText.IndexOf("path=");
            string slotName = xmlText.Substring(pos + 6, 6);
            SlotModel slot = Global.gSlotVM.getSlot(slotName);
            if (slot == null) return;

            pos = xmlText.IndexOf("binary");
            int st = xmlText.IndexOf(">", pos) + 2;
            int end = xmlText.IndexOf("</binary>") - st;
            if (end < 0) return;

            int size = Marshal.SizeOf<RegisterStruct>();
            
            var ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, st, ptr, size);
            slot.RegSet = (RegisterStruct)Marshal.PtrToStructure(ptr, typeof(RegisterStruct));
            Marshal.FreeHGlobal(ptr);

            if (Global.gMainVM.checkCurSlot(slot.Name))
                Global.gMainVM.updateUIFlag((int)UI.REGISTER);

        }

        public void recvCon(bool isCon) {
            if (isCon) {
                _client.sendIdentify("StarOS");
                Global.gMainVM.MainText = "SLOT SELECTION";
            }
            else
                Global.gMainVM.MainText = "NO ROUTER";

        }

        /*
		//Simulation
		private DispatcherTimer _simTimer;
		private Random _rand = new Random();
		private bool _isFirstRun = true;

		public void StartSimulation(string slotName)
		{
			if (_simTimer != null) _simTimer.Stop();
			_isFirstRun = true;

			_simTimer = new DispatcherTimer();
			_simTimer.Interval = TimeSpan.FromSeconds(5); // 5초 간격
			_simTimer.Tick += (s, e) => RunSimulationStep(slotName);

			_simTimer.Start();

			// 최초 1회 즉시 실행 (Set 데이터 처리)
			RunSimulationStep(slotName);
		}

		private void RunSimulationStep(string slotName)
		{
			string xmlString = "";

			if (_isFirstRun)
			{
				// 1. 최초 1회: <set> 데이터 생성
				xmlString = CreateDummySetXml(slotName);
				_isFirstRun = false;

				// 콘솔 출력
				Console.WriteLine("======= [SIMULATION: SET XML START] =======");
				Console.WriteLine(xmlString);
				Console.WriteLine("======= [SIMULATION: SET XML END]   =======");
			}
			else
			{
				// 2. 이후 반복: <read> 데이터 생성
				xmlString = CreateDummyReadXml(slotName);

				// 콘솔 출력
				Console.WriteLine($"\n[SIMULATION: READ XML] Time: {DateTime.Now:HH:mm:ss}");
				Console.WriteLine(xmlString);
			}

			try
			{
				XmlDocument doc = new XmlDocument();
				doc.LoadXml(xmlString);

				// 실제 로직 호출: doCmdPGM 내에서 pgmPower -> recvPowerSet/Read 순으로 처리됨
				doCmdPGM(doc.DocumentElement);
			}
			catch (Exception ex)
			{
				Console.WriteLine($"[Sim Error] XML 형식이 잘못되었습니다: {ex.Message}");
			}
		}

		private string CreateDummySetXml(string slotName)
		{
			// DPS 16개 * 2 (SV, OC) = 32개
			string dpsSet = string.Join(",", Enumerable.Range(1, 32).Select(x => (x % 2 == 1 ? "1.20" : "1.50")));
			// HV 4개 * 2 (SV, OC) = 8개
			string hvSet = string.Join(",", Enumerable.Range(1, 8).Select(x => (x % 2 == 1 ? "12.00" : "0.05")));
			// DAC 3개
			string dacSet = "1.11,2.22,3.33";

			return $@"<star-p path='{slotName}'>
                <result seq='0' cmd='pgm' return='ok' type='xml'>
                    <power-ex>
                        <set>
                            <dps>{dpsSet}</dps>
                            <hv>{hvSet}</hv>
                            <dac>{dacSet}</dac>
                        </set>
                    </power-ex>
                </result>
              </star-p>";
		}

		private string CreateDummyReadXml(string slotName)
		{
			// 실시간 데이터는 랜덤값 생성
			List<string> dpsRead = new List<string>();
			for (int i = 0; i < 16; i++)
			{
				dpsRead.Add((1.15 + _rand.NextDouble() * 0.1).ToString("F2")); // RV
				dpsRead.Add((0.02 + _rand.NextDouble() * 0.05).ToString("F2")); // RC
			}

			List<string> hvRead = new List<string>();
			for (int i = 0; i < 4; i++)
			{
				hvRead.Add((11.90 + _rand.NextDouble() * 0.2).ToString("F2")); // RV
				hvRead.Add((0.03 + _rand.NextDouble() * 0.02).ToString("F3")); // RC
			}

			return $@"<star-p path='{slotName}'>
                <result seq='0' cmd='pgm' return='ok' type='xml'>
                    <power-ex>
                        <read>
                            <dps>{string.Join(",", dpsRead)}</dps>
                            <hv>{string.Join(",", hvRead)}</hv>
                        </read>
                    </power-ex>
                </result>
              </star-p>";
		}
        */
	}
}
