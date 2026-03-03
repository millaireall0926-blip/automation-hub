#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "eeprom.h"
//#include "/star/StarAPI/STAR_TEST.h"
//#include "/star/StarAPI/DIAG_TEST.h"

#define BUSID_EC_I2C		3

using namespace std;

void useage ()
{
    cout << "romwrite -f filename.xxx" <<endl;
    cout << "romwrite -a 0x5000 -f filename.xxx" <<endl;
    cout << "-a address (Default address : 0x5000)" << endl;
    cout << "-f file name" << endl;
}

eeprom gROM;

uint32_t diag_romWrite(uint16_t addr, int32_t length, uint8_t* data)
{
	return gROM.write_eeprom(BUSID_EC_I2C, TCB_ROM, addr, length, data );
}

uint32_t diag_romRead(uint16_t addr, int32_t length, uint8_t* data)
{
	return gROM.read_eeprom(BUSID_EC_I2C, TCB_ROM, addr, length, data );
}

void RomWrite(string strFile, uint16_t addr = 0x5000)
{
    //if(addr == 0) addr = 0x5000;
    uint raddr = addr;
    float data[4];
    //ifstream in("data.txt");
    ifstream in(strFile);
    if(!in.is_open()) {
        cout<< "File open failed."<< endl;
        return;
    }
    cout << hex << addr << " " << strFile << endl;
    int count = 0;
    int rv = 0;
    cout << "Write Data" << endl;
    while(!in.eof()) {
        in >> data[count%4];
        if(count%4 == 3){
            cout << data[0] << " "<< data[1] << " "<< data[2] << " "<< data[3] << endl;
            rv = diag_romWrite(addr, sizeof(data), (uint8_t*)data);
            addr += sizeof(data);
        }
        count++;
    }

    cout << "Read Data" << endl;

    while(raddr < addr){
        rv = diag_romRead(raddr, sizeof(data), (uint8_t*)data);
        cout << data[0] << " "<< data[1] << " "<< data[2] << " "<< data[3] << endl;
        raddr += sizeof(data);
    }
}

int main(int argc, char* argv[])
{
    int flags, opt;
    string strFile;
    uint16_t addr = 0x5000;

    while ((opt = getopt(argc, argv, "a:f:")) != -1) {
        switch (opt) {
        case 'a':
            if(optarg[0] == '0' && optarg[1] == 'x')
                addr = strtol(optarg,NULL,16);
            else 
                addr = strtol(optarg,NULL,10);
            //cout<<"addr : "<< hex << "0x" << addr << endl;
            break;
        case 'f':
            strFile = optarg;
            break;
        default: /* '?' */
            useage();
            return -1;
        }
    }

    if(strFile.size() == 0) {
        useage();
        return -1;
    }

    if(strFile == "") strFile = "data.txt";
    RomWrite(strFile, addr);
    return 0;
}  


