using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

namespace StarQMOS {
    public class DBCtrl {

        public object _lock = new object();
        public Queue<string> _dbCmdQue = new Queue<string>();

        public bool _useDB = false;
        public string EquipID { get; set;}
        public string PCID { get; set; }

        public DBCtrl() { }
        ~DBCtrl() {
            quit();
        }

        public void push(string query) {
            lock (this._lock) {
                _dbCmdQue.Enqueue(query);
            }
        }
        public string pop() {
            lock (this._lock) {
                return _dbCmdQue.Dequeue();
            }
        }

        bool _isQuit = true;
        Thread dbThread = new Thread(threadFunc);

        public void start() 
        {
            _isQuit = false;
            dbThread.Start(this);
        }

        public void quit() 
        {
            _isQuit = true;
            if (dbThread.IsAlive) 
                dbThread.Join();
        } 

        public static void threadFunc(object obj) 
        {
            DBCtrl db = (DBCtrl)obj;
           while( db._isQuit == false) 
           {
                
                if (db._dbCmdQue.Count != 0) {
                    string query = db.pop();
                    db.sendCmd(query);
                }
                Thread.Sleep(200);
            }
        }

        public void sendCmd(string query) {
            try {
                using (MySqlConnection mysql = new MySqlConnection(_dbConCmd)) {
                    mysql.Open();
                    MySqlCommand cmd = new MySqlCommand(query, mysql);

                    if (cmd.ExecuteNonQuery() != 1)
                        ErrStr = $"\'{query}\' Fail";
                }
            }
            catch (Exception ex) {
                ErrStr = ex.Message;
            }
        }

        public string _dbConCmd;// = "Server=192.168.50.140;Port=3306;Database=testdb;Uid=root;Pwd=housemania2";
        public string ErrStr { get; set; } = "";

        public void loadDB() {
            XmlDocument doc = new XmlDocument();
            try {
                doc.Load("StarQMOS.xml");
            }
            catch (Exception ex) {
                Console.WriteLine(ex.Message);
                return;
            }

            XmlNode nodeDB = doc.SelectSingleNode("/StarQMOS/db");
            if (nodeDB == null) return;

            _useDB = Convert.ToInt32(nodeDB.Attributes["use"].Value) == 1 ? true : false;

            EquipID = nodeDB.Attributes["equip"].Value;
            PCID = nodeDB.Attributes["pc"].Value;

            _dbConCmd = $"Server={nodeDB.Attributes?["ip"]?.Value};port={nodeDB.Attributes?["port"]?.Value};" +
                $"Database={nodeDB.Attributes?["name"]?.Value};Uid={nodeDB.Attributes?["id"]?.Value};Pwd={nodeDB.Attributes?["pw"]?.Value}";
        }


    }
}
