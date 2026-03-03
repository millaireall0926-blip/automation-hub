using StarBase;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace StarOS.Model
{
    public class DBPgmModel : BaseVM
    {
        string _pgm = "";
        public string Pgm
        {
            get { return _pgm; }
            set { _pgm = value; OnPropertyChanged("Pgm"); }
        }

       
        string _lotID = "";
        public string LotID
        {
            get { return _lotID; }
            set { _lotID = value; OnPropertyChanged("LotID"); }
        }

        public string _procutID;
        public string ProcutID
        {
            get { return _procutID; }
            set { _procutID = value; OnPropertyChanged("ProductID"); }
        }


        string _adr = "";
        public string Adr
        {
            get { return _adr; }
            set { _adr = value; OnPropertyChanged("Adr"); }
        }

        public string _path = "";
        public string Path
        {
            get { return _path; }
            set { _path = value; OnPropertyChanged("Path"); }
        }

        public string _auth = "";
        public string Auth
        {
            get { return _auth; }
            set { _auth = value; OnPropertyChanged("Auth"); }
        }

        public string _desc = "";
        public string Desc
        {
            get { return _desc; }
            set { _desc = value; OnPropertyChanged("Desc"); }
        }

        public DelegateCommand DelCmdLoad { get; set; }

        public DBPgmModel()
        {
            DelCmdLoad = new DelegateCommand(e => { 
                Global.gMainVM.loadByDB(this);
            });

        }
    }
}
