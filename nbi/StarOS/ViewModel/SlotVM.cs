using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Xml;
using StarBase;
using StarOS.Model;

namespace StarOS.ViewModel
{
    public class SlotVM : BaseVM
    {
        public SlotVM()
        {
            Global.gSlotVM = this;            
        }

        ~SlotVM()
        {

        }

        public SlotModel CurSlot = null;
        public List<SlotModel> SelectSlots = new List<SlotModel>();
        


        ObservableCollection<SlotModel> _slots = null;
        public ObservableCollection<SlotModel> Slots
        {
            get
            {
                if (_slots == null)
                {
                    _slots = new ObservableCollection<SlotModel>();
                }
                return _slots;
            }
            set
            {
                _slots = value;
                OnPropertyChanged(nameof(Slots));
            }
        } 


        public void addSlot(string id, string ip)
        {
            SlotModel st = new SlotModel(id);   
            st._ip = ip;
            Slots.Add(st);
            //st._numBid = 0x11001234;
            //Global.gMainVM.loadLoaderFile(st);
        }

        public void setSlot(string id, string bid, bool online, bool isPgm, bool isDiag)
        {
            SlotModel slot = getSlot(id);

            
            if (slot == null) return;

            slot._isOnline = online;
            slot.BID = bid;

            if (online == false)
            {
                slot.StatType = STAT_TYPE.DISCONNECT;
                slot.BID = "";
            }else if (isDiag)
            {
                slot.StatType = STAT_TYPE.DIAG;
            }
            else
            {
                if (slot.StatType == STAT_TYPE.DIAG) // finish diag
                {
                    if (!isDiag)
                    {
                        if (slot.BID == "")
                            slot.StatType = STAT_TYPE.CONNECT;
                        else
                            slot.StatType = STAT_TYPE.INSERT;

                        string date = DateTime.Now.ToString("yyyyMMdd_HHmmss");
                        string srcFile = "/star/diag/log/diag.log";
                        string saveFile = string.Format($"{Global.gDirDiag}\\{date}_{slot.Name}_diag.log");
                        slot._diagPath = saveFile;
                        Global.gNet._client.sendReqFile(slot.Name, srcFile, saveFile);
                        
                    }
                }

                if ( (slot.StatType >= STAT_TYPE.LOAD && slot.StatType <= STAT_TYPE.STOP) 
                    || slot.StatType == STAT_TYPE.ABORT )
                {
                    Global.gNet._client.sendSlotStat(id);
                    return;
                }

                slot.StatType = STAT_TYPE.CONNECT;
            }
        }

        public void setSlotDetail(string id, XmlNode node)
        {
            int slotIndex = getSlotIndex(id);
            if (slotIndex >= Slots.Count) return;

            string bid = node.Attributes["bid"].Value;
            
            bool isPGM = Convert.ToInt32(node.Attributes["pgm"].Value) == 1 ? true : false;
            bool isDiag = Convert.ToInt32(node.Attributes["diag"].Value) == 1 ? true : false;

            XmlNode nodeStep = node.SelectSingleNode("run-step");
            string lot = nodeStep.Attributes["lotid"].Value;
//            string startTime = nodeStep.Attributes["st"].Value;
            string pgm = nodeStep.Attributes["name"].Value;
            string step = nodeStep.Attributes["step"].Value;
            bool isCon = Convert.ToInt32(nodeStep.Attributes["con"].Value) == 1 ? true : false;

            SlotModel slot = Slots[slotIndex];

            if (slot._timerRunCheck.IsEnabled)
                slot._timerRunCheck.Stop();

            slot.BID = bid;
            slot.LotID = lot;
//            slot.StartTime = startTime;
            slot.PgmName = pgm;
            slot.StepName = step;
            slot._contact = isCon;

            if (isPGM)
            {
                if (isCon)
                    Slots[slotIndex].StatType = STAT_TYPE.CONTACTING;
                else
                    Slots[slotIndex].StatType = STAT_TYPE.RUNNING;

            }
            else if (isDiag)
            {
                Slots[slotIndex].StatType = STAT_TYPE.DIAG;
            }else
            {
                if (slot.PwrMon == true)
                {
                    slot.PwrMon = false;
                    if (Global.gMainVM.checkCurSlot(slot.Name))
                        Global.gMainVM.updateUIFlag((int)UI.POWER);
                }

                if (slot.StatType == STAT_TYPE.CONTACTING || slot.StatType == STAT_TYPE.RUNNING)
                {
                    if (slot._endTest == false)
                    {                        
                        slot.StatType = STAT_TYPE.ERROR;
                        // kill msg : PG Stop, Power OFF
                        Global.gNet._client.sendPgmStop(slot.Name);
                        Global.gTestVM.addSlotLog(slot,
                            String.Format($"\'{slot.PgmName}\' Teminated abnormally"), LOG_TYPE.ERROR);
                        return;
                    }

                    if (slot.StatType == STAT_TYPE.CONTACTING)
                        slot.StatType = STAT_TYPE.CONTACT;
                    else if (slot.StatType == STAT_TYPE.RUNNING)
                        slot.StatType = STAT_TYPE.STOP;

                    return;
                }


                if (slot.StatType < STAT_TYPE.LOAD)
                    slot.StatType = STAT_TYPE.CONNECT;
                
            }

        }

        public SlotModel getSlot(string id)
        {
            foreach (SlotModel st in Slots)
            {
                if (st.Name == id) return st;
            }
            return null;
        }

        public int getSlotIndex(string id)
        {
            int index = 0;
            foreach (SlotModel st in Slots)
            {
                if (st.Name == id) break;
                index++;
            }
            return index;
        }

    }
}
