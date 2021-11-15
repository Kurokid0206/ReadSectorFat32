#include <iostream>
#include <string>
#include <fstream>
#include "src.h"
#include <vector>
#include <locale> 
#include <codecvt> 

using namespace std;


 int Glb_bps;
 int Glb_Sc;
 int Glb_Sb;
 int Glb_Nf;
 int Glb_Nv;
 int Glb_Sf;
 int Glb_bgcluster;
 int Glb_bgsecter;
 BYTE Glb_FAT[1964544];


int main(int argc, char** argv){

    BYTE sector[512];
    string drive;
    cout << "Nhap o dia: ";
    cin >> drive;
    while (readsectorfrom(drive)!=0) {
        cout << "Nhap lai o dia: ";
        cin >> drive;
    }
    
    component* f = ReadRdet(L"\\\\.\\F:", (Glb_Sf * Glb_Nf + Glb_Sb) * Glb_bps, 8);
    cout << "\n\nCay thu muc: " << endl;
    _setmode(_fileno(stdout), _O_U16TEXT);
    f->show(0);

    return 0;
}

