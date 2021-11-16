#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <locale> 
#include <codecvt> 
#include "src.h"

using namespace std;


//Create global variable to read the usb
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

    string drive;
    cout << "Nhap o dia: ";
    cin >> drive;
    while (readsectorfrom(drive)!=0) {
        cout << "Nhap lai o dia: ";
        cin >> drive;
        if (drive == "exit") {
            return 0;
        }
    }

    //Read usb and pass into component f(a folder)
    component* f = ReadRdet(L"\\\\.\\F:", (Glb_Sf * Glb_Nf + Glb_Sb) * Glb_bps, Glb_Sc);
    if (!f) { cout << "Khong phai usb dinh dang FAT32!!!";return -1; }
    cout << "\n\nCay thu muc: " << endl;
    //Set cout mode, unable to use cout from this line
    if (_setmode(_fileno(stdout), _O_U16TEXT)) {
        f->show(0);
    }
    

    return 0;
}

