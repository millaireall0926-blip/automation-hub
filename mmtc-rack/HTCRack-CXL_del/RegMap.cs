using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HTCRack
{

    
    public static class RegMap
    {
        #region Index Define

        public static int Fan_Multi;        
        public static int Fan_SetDuty;
        public static int Fan_GetDuty;
        public static int Fan_GetTach;

        public static int Cur_Multi;
        public static int Cur_GetCur;

        public static int Temp_Multi;
        public static int Temp_GetTemp;

        public static int Dim_GetDim;        
        public static int DimTempCount;

        public static int TCon_Multi;
        public static int TCon_SetSV;
        public static int TCon_SetCtrl;
        public static int TCon_GetSV;
        public static int TCon_GetCoolerPV;
        public static int TCon_GetPV;
        public static int TCon_GetMV;
        public static int TCon_GetCHSTS;
        public static int TCon_GetALM;
        public static int TCon_GetRunST;
        public static int TCon_GetTuneST;

        public static int TCon_SetBS1;
        public static int TCon_GetBS1;

        public static int GPIO_SSRCtrl;
        public static int GPIO_BDCon;
        public static int GPIO_SideClose;
        public static int GPIO_SSR_Pulse;
        public static int GPIO_SSR_Cold;

        #endregion

        #region Etc Define 
        public const int MaxCoolerCount = 4;
        #endregion

    }
}
