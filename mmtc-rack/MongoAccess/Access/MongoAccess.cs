using DnsClient.Protocol;
using MongoAccess.Models;
using MongoDB.Bson;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MongoAccess.Access
{
    public class DBAccess
    {
        public string _conString = "mongodb://ui:housemania2@222.98.49.24:27017/admin";
        public string _dbName = "htc_db";
        public string _equipCollection = "equip_stat";
        public bool _isConnect = false;

        public void setConStr(string ip, int port, string user, string pw, string auth)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("mongodb://");
            sb.Append(user);
            sb.Append(":" + pw);
            sb.Append("@" + ip);
            sb.Append(":" + port.ToString());
            sb.Append("/" + auth);                      
            _conString = sb.ToString();
        }

        public bool checkConnection()
        {
            MongoClient client = new MongoClient(_conString);
            IMongoDatabase db = client.GetDatabase(_dbName);
            bool iscon = db.RunCommandAsync((Command<BsonDocument>)"{ping:1}").Wait(4000);
            _isConnect = iscon;
            return iscon;
        }

        public IMongoCollection<T> connectMongoDB<T>(in string collection)
        {
            MongoClient client = new MongoClient(_conString);            
            IMongoDatabase db = client.GetDatabase(_dbName);            
            bool isMongoLive = db.RunCommandAsync((Command<BsonDocument>)"{ping:1}").Wait(4000);
            if (!isMongoLive)
            {
                _isConnect = false;
                return null;
            }
            _isConnect = true;
            return db.GetCollection<T>(collection);
            
        }

        public List<EquipModel> getAllEquip()
        {
            var equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return null;
        
            return equipCollection.Find(new BsonDocument()).ToList();
            
        }

        public async Task<List<EquipModel>> getAllEauipAsync()
        {
            var equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return null; 
            var results = await equipCollection.FindAsync(_ => true);
            return results.ToList();
        }

        public EquipModel getEquip(string equipName)
        {
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return null;
            FilterDefinition<EquipModel> filter = Builders<EquipModel>.Filter.Eq("Equip", equipName);
            IFindFluent<EquipModel, EquipModel> findEquip =  equipCollection.Find<EquipModel>(filter);
            List<EquipModel> bbb = findEquip.ToList();
            return bbb.FirstOrDefault();
            
        }

        public Task createEquipAsync(EquipModel equip)
        {
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return null;
            return equipCollection.InsertOneAsync(equip);
        }

        public void createEquip(EquipModel equip)
        {
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return;
            equipCollection.InsertOne(equip);
        }

                
        public void updateAlive(EquipModel equip)
        {
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return;
            FilterDefinition<EquipModel> filter = Builders<EquipModel>.Filter.Eq("Equip", equip.Equip);
            
            equip.Alive++;            
            UpdateDefinition<EquipModel> updete = Builders<EquipModel>.Update.Set("Alive", equip.Alive);
            equipCollection.UpdateOne(filter, updete);
        }

        public Task replaceEquipAsync(EquipModel equip)
        {
            
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return null;
            var filter = Builders<EquipModel>.Filter.Eq("Equip", equip.Equip);
            return equipCollection.ReplaceOneAsync(filter, equip, new ReplaceOptions { IsUpsert = true});
        }

        public void replaceEquip(EquipModel equip)
        {
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return;
            var filter = Builders<EquipModel>.Filter.Eq("Equip", equip.Equip);

            equipCollection.ReplaceOne(filter, equip);

        }

        public void deleteEquip(EquipModel equip)        
        {
            IMongoCollection<EquipModel> equipCollection = connectMongoDB<EquipModel>(_equipCollection);
            if (equipCollection == null) return;
            var filter = Builders<EquipModel>.Filter.Eq("Equip", equip.Equip);
            equipCollection.DeleteOne(filter);
        }
    }
}
