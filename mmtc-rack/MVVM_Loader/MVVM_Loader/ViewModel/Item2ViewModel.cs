using MVVM_Loader.Common;
using MVVM_Loader.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MVVM_Loader.ViewModel
{
    public class Item2ViewModel :BaseVM
    {
        private ObservableCollection<Item2Model> _item2Datas;
        public ObservableCollection<Item2Model> Item2Datas
        {
            get => _item2Datas;
            set
            {
                _item2Datas = value;
                OnPropertyChanged(nameof(Item2Datas));
            }
        }
        public Item2ViewModel()
        {
            Item2Datas = new ObservableCollection<Item2Model>
            {
                new Item2Model(3,"FR-L","입력 하한 설정","0", "0", "0", "0", "Visible"),
                new Item2Model(4,"FR-H","입력 상한 설정","100", "100", "100", "100", "Visible"),
                new Item2Model(5,"SL-L","프리스케일 하한","0", "0", "0", "0", "Visible"),
                new Item2Model(6,"SL-H","프리스케일 상한","0", "0", "0", "0", "Visible"),
                new Item2Model(7,"OUT.P","출력 포트","1", "1", "1", "1", "Visible"),
                new Item2Model(8,"O.ACT","출력 동작 선택","0", "0", "0", "0", "Visible"),
                new Item2Model(9,"OUT1","출력1 종류 선택(가열 제어출력)","1", "1", "1", "1", "Visible"),
                new Item2Model(10,"OUT2","출력2 종류 선택(가열 제어출력)","1", "1", "1", "1", "Visible"),
                new Item2Model(11,"SCR.Z","전류출력(SCR) 선택시 전류 범위","1","","","","Hidden")
            };
            //Global.gItem2ViewModel = this;
        }
    }
}
