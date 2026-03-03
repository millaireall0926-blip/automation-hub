using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MongoAccess.Models
{
    public class BoxModel
    {
        public double DimmAvg { get; set; }
        public double InsideBox { get; set; }        
        public List<double> Dimms { get; set; }
    }
}
