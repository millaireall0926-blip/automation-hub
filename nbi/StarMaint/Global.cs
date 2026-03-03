using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarMaint
{

    public static class VerInfo
    {
        const string Program = "Maint";
        const string Date = "2024/01/17";
        const string Ver = "V1.0.3";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }
    public static class Global
    {
        
    }
}



/*
 * * Ver 1.0.2
 *    as router upgrade  packet 
 *    MSG_TYPE DEST 바뀐것 수정
 * 
 * Ver 1.0.2
 *    초기 실행시 List 안받아지던것. Slot 배열 구성할때 ui Thread로 변경.
 *    중복실행 방지 Mutex 추가
 * 
 */