using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using System.Collections.Generic;

namespace MongoAccess.Models
{
    public class EquipModel
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string ID { get; set; }        
        public string Equip { get; set; }
        public int Alive { get; set; }        
        public List<RackModel> Racks { get; set; } 


    }
}
