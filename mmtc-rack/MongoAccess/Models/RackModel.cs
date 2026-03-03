using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MongoAccess.Models
{
    public class RackModel
    {
        public string Name { get; set; }
        public string IP { get; set; }
        public int Stat { get; set; }
        public List<BoxModel> Boxs { get; set; }

        public string StartT { get; set; }
        public string EndT { get; set; }
    }
}
