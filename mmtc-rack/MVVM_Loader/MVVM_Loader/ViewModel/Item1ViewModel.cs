using MVVM_Loader.Common;
using MVVM_Loader.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace MVVM_Loader.ViewModel
{
    public class Item1ViewModel : BaseVM 
    {
        public BackgroundWorker _UploadWorker1 = null;
        private ObservableCollection<Item1Model> _item1Datas;
        public ObservableCollection<Item1Model> Item1Datas
        {
            get => _item1Datas;
            set
            {
                _item1Datas = value;
                OnPropertyChanged(nameof(Item1Datas));
            }
        }
        public Item1ViewModel()
        {
            Item1Datas = new ObservableCollection<Item1Model>
            {
                new Item1Model(1,"INP","입력 종류 설정","1", "1", "1", "1", "Visible"),
                new Item1Model(2,"UNIT","입력 단위 설정","0","","","","Hidden")
            };
            //Global.gItem1ViewModel = this;
        }
        
    }
}
