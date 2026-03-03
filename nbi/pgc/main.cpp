// pgc.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <algorithm>
#include <iostream>
#include <fstream>

#ifdef __linux__
#include <float.h>
#include <memory.h>
#endif

//#include "History.h"
#include "PGComLib.h"
#include "PGC.h"
#include "XGetopt.h"
#include "ErrorLog.h"

//int getopt(int argc, char* argv[], const char* optstring);

int opt_parser(string opt);

void usage()
{
    cout << "pgc PatFileName.xxx" << endl;
    cout << "pgc -c option PatFileName.xxx" << endl;
    cout << "pgc -o outfile PatFileName.xxx" << endl;
    cout << "pgc -c option -o outfile PatFileName.xxx" << endl;
    cout << "option : %IFE Compile option (A, AC, DF, ...), Use only uppercase alphabets." << endl;
    cout << "outfile : output file name.(outfile.mpa)" << endl;
}

#ifndef __linux__
void test_function()
{
    registerset_test test_t;
    cout << "test_t size : " << sizeof(test_t) << endl;
     
    std::string str1 = "   //";
    std::string str2 = "   ";
    size_t ne  = std::min(str1.find("//"), str1.find(";"));
    size_t ne2 = std::min(str2.find("//"), str2.find(";"));

    /////////////////////////// 0x00800000 == (2<<22)  /////////////////////////
    double value = ((double)((0x00400000)-1) * (1.0 / (100000000 )));
    double m_RefTimeDiv_0 = ((double)((0x00400000) - 1) * (1.0 / (100000000))) - DBL_EPSILON;
    double m_RefTimeDiv_1 = ((double)((0x00400000) - 1) * (1.0 / (100000))) - DBL_EPSILON;
    double m_RefTimeDiv_2 = ((double)((0x00400000) - 1) * (1.0 / (100))) - DBL_EPSILON;
    value -= DBL_EPSILON;
    value = DBL_MIN;
    value += DBL_MIN;


    instruction_t inst;
    int size = sizeof(inst);

    inst.op_command.data[0] = 0;
    inst.op_command.data[1] = 0;
    inst.xb.data = 0;
    inst.xn.data[0] = 0;
    inst.xn.data[1] = 0;
    inst.xn.data[2] = 0;
    inst.xn.data[3] = 0;
    inst.xn.data[4] = 0;
    inst.yb.data = 0;

    std::cout << "instruction_t size : " << size << std::endl;

    uint64_t int64_size = sizeof(uint64_t);

    std::cout << "int64 size : " << int64_size << std::endl;

    std::string strLineBuf;
    strLineBuf = "abcdefgh";

    strLineBuf.erase(3);

    std::cout << "strLineBuf : " << strLineBuf << std::endl;

    strLineBuf = "abcdefgh";
    strLineBuf.erase(0, 3);
    std::cout << "strLineBuf : " << strLineBuf << std::endl;

    //CPGC pgc;
    //pgc.LoadFile("");


}
#endif

int main(int argc, char* argv[])
{
    //test_function();
    //cout << argc << " : " << argv[0] << " : " << argv[1] << endl;

    if(argc < 2) {
        usage();
        return -1;
    }
    
    int param_opt;
    int comp_opt = -1;
    bool bDump = false;
    string strOut;
    while ((param_opt = getopt(argc, argv, "c:o:d")) != -1)
    {
        switch (param_opt)
        {
            case 'c': 
                comp_opt = opt_parser(optarg); 
                if (comp_opt == -1) {
                    // 에러 처리
                    CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, optarg, "Unknown option");
                    return -1;
                }
                break;
            case 'o':
                strOut = optarg;
                if (strOut.size() == 0) {
                    // 에러 처리
                    CErrorLog::GetInstance()->AddErrorLog(LOG_TYPE::ERROR, 0, "Out file is nothing");
                    return -1;
                }
                break;
            case 'd':
                bDump = true;
                break;
        }
    }

    CPGC cpgc(argv[argc - 1], comp_opt);
    if (cpgc.IsLoaded()) {
        if (cpgc.DoCompile() == -1) {
            return -1;
        }
        if (cpgc.PostCompile() == -1) {
            return -1;
        }
        if (cpgc.WriteFile(strOut) == -1) {
            return -1;
        }

        if (bDump) {
            cpgc.Dump(strOut);
        }
    }
    else
        return -1;

    return 0;
    
}

std::string MakeUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string MakeLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

int opt_parser(std::string opt)
{
    int comp_opt = 0;
    MakeUpper(opt);
    for (unsigned int i = 0; i < opt.size(); i++) {
        if (isalpha(opt[i])) {
            comp_opt |= 0x1 << (opt[i] - 'A');
        }
        else {
            return -1;
        }
    }

    return comp_opt;
}
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
