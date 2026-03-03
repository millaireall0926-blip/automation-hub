using StarBase;
using StarOS.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarOS.ViewModel
{
	public class PgmCheckVM : BaseVM
	{
		// 기준(Master)이 되는 슬롯 객체
		public SlotModel MasterSlot { get; set; }

		// 기준과 PGM 이름이 다른 슬롯들의 목록
		public ObservableCollection<SlotModel> ConflictSlots { get; set; }

		public PgmCheckVM(SlotModel master, List<SlotModel> conflicts)
		{
			MasterSlot = master;
			ConflictSlots = new ObservableCollection<SlotModel>(conflicts);
		}
	}
}
