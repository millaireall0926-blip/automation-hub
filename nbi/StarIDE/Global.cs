using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Xml;
using HandyControl.Controls;
using StarIDE.Model;
using StarIDE.ViewModel;


namespace StarIDE
{

    public static class Global
    {
        public static MainVM gMainVM;
        public static FilesVM gFilesVM;
        public static DockVM gDockVM;
        public static AssistVM gAssistVM;
        
        public static MainWindow gMainView;

        public static string gCompiler = "g++";


        public static string _gAPIHeader = "STAR_TEST.h";
        public static string gAPIHeader {
            get => _gAPIHeader;
            set {
                string prev = _gAPIHeader;
                _gAPIHeader = value;
                if (_gAPIHeader != prev)
                {
                    if (Global.gAssistVM != null)
                    {
                        Global.gAssistVM.setAPIList();
                    }
                }
            }
        }



        public static string gAPIPath = "/star/StarAPI/";
        public static string gEAPIPath = "/star/eapi";
        public static string gHeadPath = "/star/StarAPI/";
        public static string gPGCPath = "/star/StarAPI/";
        public static string gConfigPath = "StarIDE.xml";

        public static bool gDeleteMake = true;

        public static XmlDocument loadConfig()
        {
            XmlDocument doc = new XmlDocument();
            string path = gConfigPath;
            if (!File.Exists(path))
            {
                path = @"StarIDE\" + path;
                if (File.Exists(path))
                    gConfigPath = path;
                else
                    return null;
            }

            try
            {
                doc.Load(path);
            }
            catch (Exception ex)
            {
                Growl.ClearGlobal();
                Growl.InfoGlobal(ex.Message);
                return null;
            }
            return doc;
        }

        public static bool gShowLogo = true;
        public static void loadPath()
        {
            XmlDocument doc =  loadConfig();
            if (doc == null) return;

            var node = doc.SelectSingleNode("/StarIDE");
            if (node != null)
            {
                if (node.Attributes["logo"] != null)
                {
                    if (Convert.ToInt32(node.Attributes["logo"].Value) == 0)
                        gShowLogo = false;
                }
            }


            node = doc.SelectSingleNode("/StarIDE/compile/pgm");
            gCompiler = node.Attributes["cc"].Value;
            gAPIPath = node.Attributes["api"].Value;            
            gEAPIPath = node.Attributes["eapi"].Value;
            node = doc.SelectSingleNode("/StarIDE/compile/pat");
            gPGCPath = node.Attributes["pgc"].Value;
            node = doc.SelectSingleNode("/StarIDE/compile/head");
            gHeadPath = node.Attributes["path"].Value;
            gAPIHeader = node.Attributes["name"].Value;
            node = doc.SelectSingleNode("/StarIDE/compile/makefile");
            gDeleteMake =  Convert.ToInt32(node.Attributes["delete"].Value) == 1 ? true : false;

        }

        public static string getLastText(string path, string cut, bool forword = false)
        {
            int pos = path.LastIndexOf(cut);
            if (pos == -1) return "";

            string ret = "";
            if (forword)
                ret = path.Substring(0, pos);
            else
            {
                pos++;
                ret = path.Substring(pos, path.Length - pos);
            }
            return ret;
        }



        public static void loadRecent()
        {
            XmlDocument doc = loadConfig();
            if (doc == null) return;
        }

        public static void saveRecent()
        {
            XmlDocument doc = loadConfig();
            if (doc == null) return;

            XmlNodeList list = doc.SelectNodes("/StarIDE/Recent");
            foreach (XmlNode node in list)
            {               
                node.Attributes["path"].Value = ""; // input Path
            }

            
            doc.Save("StarIDE.xml");            
        }

        static Global()
        {
            loadPath();
            loadRecent();

            loadAPIList();
            setKeyword(); 
        }

        public static Dictionary<string, string> gAPIAutoComp = new Dictionary<string, string>();
        public static ObservableCollection<APIModel> gAPIFuns = new ObservableCollection<APIModel>();
        public static void loadAPIList()
        {
            string fn = Global.gAPIHeader;
            if (!File.Exists(fn))
            {
                fn = @"CM\" + fn;
                if (!File.Exists(fn))
                    fn = @"..\" + fn;
            }


            try
            {
                FileStream fs = new FileStream(fn, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                StreamReader reader = new StreamReader(fs);

                string Text = reader.ReadToEnd();
                string[] apis = Text.Split('\n');
                bool bBrief = false;
                APIModel curAPI = null;
                foreach (string api in apis)
                {
                    if (api.IndexOf("(") >= 0)
                    {
                        string str = Global.getLastText(api, "(", true);
                        string name = Global.getLastText(str, " ");
                        if (name == "") continue;                                                
                        if (!Char.IsUpper(name, 0)) continue;

                        if (name.Length > 2)
                        {
                            if (name.Substring(0, 3) == "NAN")
                            {
                                if (!gAPIAutoComp.ContainsKey("NAND"))
                                    gAPIAutoComp["NAND"] = "";
                                gAPIAutoComp[name.Substring(0, 4)] += name + " ";
                            }
                            else if (name.Substring(0, 3) == "GET")
                            {
                                if (!gAPIAutoComp.ContainsKey("GET"))
                                    gAPIAutoComp["GET"] = "";
                                gAPIAutoComp[name.Substring(0, 3)] += name + " ";
                            }
                            else if (name.Substring(0, 3) == "SET")
                            {
                                if (!gAPIAutoComp.ContainsKey("SET"))
                                    gAPIAutoComp["SET"] = "";
                                gAPIAutoComp[name.Substring(0, 3)] += name + " ";
                            }
                            else if (name.Substring(0, 3) == "REA")
                            {
                                if (!gAPIAutoComp.ContainsKey("READ"))
                                    gAPIAutoComp["READ"] = "";
                                gAPIAutoComp[name.Substring(0, 4)] += name + " ";
                            }
                            else
                            {
                                if (!gAPIAutoComp.ContainsKey(name.Substring(0, 1)))
                                    gAPIAutoComp[name.Substring(0, 1)] = "";
                                gAPIAutoComp[name.Substring(0, 1)] += name + " ";
                            }
                        }
                        
                        curAPI = new APIModel { Name = name, Brief = api.Replace(";", "") };
                        gAPIFuns.Add(curAPI);
                    }

                    if (bBrief)
                    {
                        if (curAPI == null)
                            continue;
                        if (api.IndexOf("*/") >= 0)
                            bBrief = false;
                        else                         
                            curAPI.Brief += api;
                    }

                    if (api.IndexOf("Brieping") >= 0)
                    {
                        bBrief = true;
                    }
                }

                fs.Close();
                reader.Close();

            }
            catch (Exception ex)
            {

            }


        }

        public static string gKeyDefCPP;
        public static string gKeyFunCPP;
        public static string gKeyApiCPP;
        public static Collection<string> gExceptGotoCPP = new Collection<string>();

        public static string gKeyOpcodePAT;
        public static string gKeyFuncPAT;
        public static string gKeyRegPAT;
        public static Collection<string> gExceptGotoPAT = new Collection<string>();

        static void setExcept(Collection<string> collection, string except)
        {
            string[] buf = except.Split(' ');
            foreach (string item in buf)
            {
                if (item == "") continue;
                collection.Add(item);
            }
            

        }

        static void setKeyword()
        {
            gKeyDefCPP = "void int byte char unsigned string uint word long ulong double float const short  ushort uint8_t uint16_t uint32_t auto vector map " 
                + "return struct sbyte sealed sizeof stackalloc volatile static select this true false HND std " 
                + "null new delete typeof class unchecked unsafe using virtual typedef bool extern enum "
                + "VS1 VS2 VS4 VS5 VS3 PE DR IO SCAN CLK ALL IDX0 IDX1 IDX2 IDX3 IDX4 IDX5 IDX6 IDX7 IDX8 IDX9 IDX10 IDX11 IDX12 IDX13 IDX14 IDX15 "
                + "XMAX YMAX  LMAX HMAX XH XT YH YT ZRSV2 D1A D1B D1C D1D D1E D1F D1G D1H D2A D2B D2C D2D D3B D4B "
                + "TPH1 TPH1A TPH1B TPH1C TPH1D TPH2 TPH2A TPH2B TPH2C TPH2D XOS YOS "
                + "DCMR DCMR2 DBMAP JET_TIMER PWAIT ISP REF_TIMER JNCFLAG REF_MODE SCAN_REG VMPA STATUS "
                + "XMASK YMASK ZRSV3 CCD RCD DSD DCMRH DCMRL MODE BL BT WL RL XMIN YMIN  S1A MLOOP ZMAX ZH ZMASK ZMIN "
                + "ADDR0 ADDR0_B  ADDR1  ADDR1_B  ADDR2  ADDR2_B  ADDR3  ADDR3_B  ADDR4  ADDR4_B  ADDR5  ADDR5_B  ADDR6  ADDR6_B  ADDR7  ADDR7_B "
                + "ADDR8  ADDR8_B  ADDR9  ADDR9_B  ADDR10  ADDR10_B  ADDR11  ADDR11_B  ADDR12  ADDR12_B  ADDR13  ADDR13_B  ADDR14  ADDR14_B  ADDR15  ADDR15_B "
                + "ADDR16  ADDR16_B  ADDR17  ADDR17_B  ADDR18  ADDR18_B  ADDR19  ADDR19_B  ADDR20  ADDR20_B  ADDR21  ADDR21_B  ADDR22  ADDR22_B  ADDR23 "
                + "ADDR23_B ADDR24  ADDR24_B  ADDR25  ADDR25_B  ADDR26  ADDR26_B  ADDR27  ADDR27_B  ADDR28  ADDR28_B  ADDR29  ADDR29_B  ADDR30  ADDR30_B  ADDR31  DDR31_B "
                + "CLK0  CLK0_B  CLK1  CLK1_B  CLK2  CLK2_B  CLK3  CLK3_B  CLK4  CLK4_B  CLK5  CLK5_B  CLK6  CLK6_B  CLK7 CLK7_B "
                + "IO0  IO0_B  IO1  IO1_B  IO2  IO2_B  IO3  IO3_B  IO4  IO4_B  IO5  IO5_B  IO6  IO6_B  IO7  IO7_B "
                + "IO8  IO8_B  IO9  IO9_B  IO10  IO10_B  IO11  IO11_B  IO12  IO12_B  IO13  IO13_B  IO14  IO14_B  IO15  IO15_B "
                + "IO16  IO16_B  IO17  IO17_B  IO18  IO18_B  IO19  IO19_B  IO20  IO20_B  IO21  IO21_B  IO22  IO22_B  IO23  IO23_B "
                + "IO24  IO24_B  IO25  IO25_B  IO26  IO26_B  IO27  IO27_B  IO28  IO28_B  IO29  IO29_B  IO30  IO30_B  IO31  IO31_B "
                + "SCAN0  SCAN0_B  SCAN1  SCAN1_B  SCAN2  SCAN2_B  SCAN3  SCAN3_B  SCAN4  SCAN4_B  SCAN5  SCAN5_B  SCAN6  SCAN6_B  SCAN7  SCAN7_B "
                + "SCAN8  SCAN8_B  SCAN9  SCAN9_B  SCAN10  SCAN10_B  SCAN11  SCAN11_B  SCAN12  SCAN12_B  SCAN13  SCAN13_B  SCAN14  SCAN14_B  SCAN15  SCAN15_B "
                + "SCAN16  SCAN16_B  SCAN17  SCAN17_B  SCAN18  SCAN18_B  SCAN19  SCAN19_B  SCAN20  SCAN20_B  SCAN21  SCAN21_B  SCAN22  SCAN22_B  SCAN23  SCAN23_B "
                + "SCAN24  SCAN24_B  SCAN25  SCAN25_B  SCAN26  SCAN26_B  SCAN27  SCAN27_B  SCAN28  SCAN28_B  SCAN29  SCAN29_B  SCAN30  SCAN30_B  SCAN31  SCAN31_B "
                + "STRB  STRB_B DRE  DRE_B BCLK1 CCLK1   BCLK2  CCLK2  BCLK3  CCLK3  BCLK4  CCLK4  BCLK5  CCLK5  BCLK6  CCLK6 "
                + "BCLK7  CCLK7  BCLK8  CCLK8  BCLK9  CCLK9  BCLK10  CCLK10  BCLK11  CCLK11  BCLK12  CCLK12 BCLK13  CCLK13  BCLK14  CCLK14  BCLK15  CCLK15  BCLK16  CCLK16 "
                + "ECLK1 ECLK2  ECLK3  ECLK4  ECLK5  ECLK6  ECLK7  ECLK8  ECLK9 ECLK10  ECLK11  ECLK12  ECLK13  ECLK14  ECLK15  ECLK16  ECLK_SAME "
                + "NRZB  _NRZB  NRZC _NRZC NRZBC _NRZBC RZO _RZO RZZ _RZZ RZX 	_RZX DNRZ _DNRZ FIXL FIXHINFO WARN ERR PS NS US MS S MIN HOUR UV MV V UA MA A "
                + "SCR_ADR SCR_DATA SCR_ALL PV SV NO_WAIT WAIT CON CUR TOTAL BOTH MASK BINARY TEXT START REMIND END SHOW HIDE";
            setExcept(gExceptGotoCPP, gKeyDefCPP);

            gKeyFunCPP = "if else switch case default while for where do goto continue catch try throw "
                + "printf sprintf stricmp strlen strncat strncmp strncmpi strncpy strnicmp strset strrchr strrev strset "
                + "strspn strstr strtod strtok strtol strtoul strupr swab system stpcpy strcat strchr strcmp strcmpi strcoll "
                + "strcpy strcspn strdup stpcpy strcat strchr strcmp strcmpi strcoll strcpy strcspn strdup abs atof atoi atol memchr memcmp memcpy memicmp "
                + "memmove memset ltoa malloc remove realloc fopen ftell fprintf fclose fgetc fgetchar fgetpos fgets fscanf fseek fputc fputchar fputs fread free " 
                + "push_back clear ";
            setExcept(gExceptGotoCPP, gKeyFunCPP);

            gKeyApiCPP = "CONTACT TEST SETUP  STEP RUN_STEP RUN_STEPI RUN_STEP STEPI BIB PDS SCRAM SYNC";
            foreach (APIModel api in gAPIFuns)
                gKeyApiCPP += " " + api.Name;
            setExcept(gExceptGotoCPP, gKeyApiCPP);


            gKeyOpcodePAT = "NOP FLGLI JZD JET RTN JRB STPS JMP JSR OUT REF_START TM_START  TIMER0 TIMER1 REPEAT PAUSE JNC0  JNC1 JNC2 JNC3 JNC4 JNC5 JNC6 JNC7 JNC8 JNC9"
                              + "JNC10 JNC11 JNC12 JNC13 JNC14 JNC15 JNI0 JNI1 JNI2 JNI3 JNI4 JNI5 JNI6 JNI7 JNI8 JNI9 JNI10 JNI11 JNI12 JNI13 JNI14 JNI15 JNI0D JNI1D JNI2D JNI3D "
                              + "JNI4D JNI5D JNI6D JNI7D INC0 INC1 INC2 INC3 INC4 INC5 INC6 INC7 STI0 STI1 STI2 STI3 STI4 STI5 STI6 STI7 STI8 STBAR STISP LDI0 LDI1 LDI2 LDI3 "
                              + "LDI4 LDI5 LDI6 LDI7 MODE VMPA ASCROFF DSCROFF END_PAGE END_BLOCK VM_INC VM_ON VM_OFF INFO_INC end_block end_page";
            setExcept(gExceptGotoPAT, gKeyOpcodePAT);

            gKeyFuncPAT = "INSERT REGISTER ENDREGISTER START END SDEF FUNCTION ENDFUNCTION";
            setExcept(gExceptGotoPAT, gKeyFuncPAT);

            gKeyRegPAT = "IDX0 IDX1 IDX2 IDX3 IDX4 IDX5 IDX6 IDX7 IDX8 IDX9 IDX10 IDX11  IDX12 IDX13 IDX14 IDX15 " +
                "XMAX YMAX ZMAX LMAX HMAX XH XT YH YT ZH D1A D1B D1C D1D D1E D1F D1G D1H D2A D2B D2C D2D D3B D4B " +
                "TPH1  TPH1A TPH1B TPH1C TPH1D TPH2 TPH2A TPH2B TPH2C TPH2D D5 D6 D7 D8 XOS YOS DCMR DCMR2 DBMAP TIMER PWAIT ISP REFTIME CFLG REF_MODE SCAN STATUS " +
                "XMASK YMASK ZMASK CCD RCD DSD  DCMRH DCMRL BL BT WL RL XMIN YMIN ZMIN S1A MLOOP S SA SB S1A TPX1 TPX2 " +
                "XB XN YB YN D3 D4 Z X Y TP TP1 TP2 D7 D8 RF DX I T0 T1 T XB XC XS XK XA XD XE XF YB YC YS YK YA YD YE YF /X /Y /Z /D /D1 /D2 " +
                "TS0 TS1 TS2 TS3 TS4 TS5 TS6 TS7 TS8 TS9 TS10 TS11 TS12 TS13 TS14 TS15 CYP0 CYP1 CYP2 CYP3 CYP4 CYP5 CYP6 CYP7 CYP8 CYP9 CYP10 CYP11 CYP12 CYP13 CYP14 CYP15 " +
                "C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 C10 C11 C12 C13 C14 C15 C16 C17 C18 C19 C20 C21 C22 C23 C24 C25 C26 C27 C28 C29 C30 C31 C32 C33 C34 C35 C36 C37 C38 C39";
            setExcept(gExceptGotoPAT, gKeyRegPAT);
        }
    }


    public static class VerInfo
    {
        const string Program = "IDE";
        const string Date = "2025/07/02";
        const string Ver = "V1.0.7";
        const string Author = "uiop";
        public static string getVer() => Program + "    " + Date + " " + Ver;
    }

    /*  
     *  
     *   V1.0.7 07/02
     *   mod : cpp compile시 기다리거나 fail나는 에러 수정
     *  
     *   V1.0.6 01/20
     *   mod : save시 wsl sync 명령
     *  
     *  *  *  *  V1.0.6 01/20
     *   mod : compile시 save 되는것 기다리도록.
     *  
     *  *  *  *  V1.0.5 03/06
     *   mod : bib size io 512까지
     *  
     *  *  *  V1.0.4 02/06
     *   add : Terminal Window 추가
     *  
     *  *  V1.0.3 01/25
     *   mod : cpp 에서 include 읽어서 type 확인 
     *   
     *  
     *  *  V1.0.3 
     *   mod : Target Header xml 지정
     *         Target Title bar 출력
     *  
     *  *  V1.0.2 
     *   mod : bib scan size 128 -> 256
     *   
     *  V1.0.1 
     *   mod : pgc error message check word add
     */


}
