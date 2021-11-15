#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include "src.h"
#include <wchar.h>
#include <locale> 
#include <codecvt> 

using namespace std;



wstring strtowstr(string str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring dest = converter.from_bytes(str);
    return dest;
}

wchar_t hextoUnicode(string hex) {
    char str[4];
    for (int i = 0; i < 4; i++)
    {
        str[i] = hex[i];
    }
    wchar_t wc = strtol(str, NULL, 16);
    return wc;
};

int ReadFAT(LPCWSTR  drive, int readPoint, BYTE sector[1964544])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        //printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

        if (!ReadFile(device, sector, Glb_Sf * Glb_bps, &bytesRead, NULL))
    {
        return-1;
        //printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        return 0;
        //printf("Success!\n");
    }

}

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        //printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        //printf("ReadFile: %u\n", GetLastError());
        return -1;
    }
    else
    {
        //printf("Success!\n");
        return 0;
    }

}

int readsectorfrom(string x) {
    BYTE sector[512];
    wstring src = L"\\\\.\\";
    string drive = x + ':';
    src += strtowstr(drive);
    
    int res=ReadSector(src.c_str(), 0, sector);
    if(res!=0){
        return res;
    }
    Glb_bps = sectortoint(sector, "0B", 2);
    Glb_Sc = sectortoint(sector, "0D", 1);
    Glb_Sb = sectortoint(sector, "0E", 2);
    Glb_Nf = sectortoint(sector, "10", 1);
    Glb_Nv = sectortoint(sector, "20", 4);
    Glb_Sf = sectortoint(sector, "24", 4);
    Glb_bgcluster = sectortoint(sector, "2C", 4);
    cout << "\nThong tin o dia: " << endl;
    cout << "So byte trong mot sector: " << Glb_bps << endl;
    cout << "So sector trong mot cluster: " << Glb_Sc << endl;
    cout << "So sector truoc bang FAT: " << Glb_Sb << endl;
    cout << "So bang Fat: " << Glb_Nf << endl;
    cout << "So sector trong volume: " << Glb_Nv << endl;
    cout << "So sector trong mot bang Fat: " << Glb_Sf << endl;
    cout << "cluster bat dau: " << Glb_bgcluster << endl;

    //////DocBangFat
    ReadFAT(L"\\\\.\\F:", (Glb_Sb) * Glb_bps, Glb_FAT);
    return 0;
}

string dectohex(int n) {
    string s;
    while (n != 0) {
        int temp = 0;
        temp = n % 16;
        if (temp < 10) {
            s = char(temp + 48) + s;
        }
        else {
            s = char(temp + 55) + s;
        }
        n = n / 16;
    }
    if (s.length() < 2) { s = '0' + s; }
    if (s.length() < 2) { s = '0' + s; }
    return s;
}


int hextodec(string hexVal) {
    int len = hexVal.size();

    // Initializing base value to 1, i.e 16^0
    int base = 1;
    int dec_val = 0;
    // Extracting characters as digits from last
    // character
    for (int i = len - 1; i >= 0; i--) {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value
        if (hexVal[i] >= '0' && hexVal[i] <= '9') {
            dec_val += (int(hexVal[i]) - 48) * base;

            // incrementing base by power
            base = base * 16;
        }
        // if character lies in 'A'-'F' , converting
        // it to integral 10 - 15 by subtracting 55
        // from ASCII value
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') {
            dec_val += (int(hexVal[i]) - 55) * base;

            // incrementing base by power
            base = base * 16;
        }
    }
    return dec_val;
}

int sectortoint(BYTE sector[512], string addr, int cnt) {
    int bg = hextodec(addr);
    string s;
    for (int i = bg + cnt-1; i >= bg; i--) {
        s = s + dectohex(sector[i]);
    }
    return hextodec(s);
}

string clusterinfat(BYTE sector[1964544], int pos) {
    string s;
    //if(pos){ pos -= 1; }
    pos *= 4;
    for (int i = pos; i < pos + 4; i++) {
        string temp = dectohex(sector[i]);
        s = dectohex(sector[i]) + s;
    }
    return s;
}

vector<int> cntcluster(BYTE sector[1964544], int pos) {
    vector<int> staken;
    for (int i = 0; i < Glb_Sc; i++) {
        staken.push_back(Glb_bgsecter + (pos - 2) * Glb_Sc + i);
    }
    string s = clusterinfat(sector, pos);
    if (s == "FFFFFFFF") {
        return staken;
    }
    else if (s == "0FFFFFF8" || s == "0FFFFFFF") {
        return staken;
    }
    else {
        pos = hextodec(s);
        vector<int> tmp = cntcluster(sector, pos);
        for (int i = 0; i < tmp.size(); i++) {
            staken.push_back(tmp[i]);
        }
        return staken;
    }
}


wstring readSEntry(BYTE entry[32]) {
    wstring temp;
    string hex;
    for (int k = 1; k < 11; k++) {
            hex = dectohex(entry[k+1])+ dectohex(entry[k]);
            k += 1;
            temp += hextoUnicode(hex);
            }
    for (int k = 14; k < 27; k++) {
        hex = dectohex(entry[k + 1]) + dectohex(entry[k]);
        k += 1;
        temp += hextoUnicode(hex);
    }
    for (int k = 28; k < 32; k++) {
        hex = dectohex(entry[k + 1]) + dectohex(entry[k]);
        k += 1;
        temp += hextoUnicode(hex);
    }
    return temp;
}

component* readEntry(BYTE entry[32], wstring sname) {
    component* f = nullptr;
    string att = dectohex(entry[11]);
    bool isfolder = false;
    string name;
    if (att == "08"|| att == "10") {
        isfolder = true;
    }
    if (sname == L"") {
        for (int k = 0; k < 8; k++) {
            if (isascii(entry[k])) {
                name += char(entry[k]);
            }
        }
        if (!isfolder) {
            name += '.';
        }
        for (int k = 8; k < 11; k++) {
            if (isascii(entry[k])) {
                name += char(entry[k]);
            }
        }
    }
    string buffer;
    buffer = dectohex(entry[21]) + dectohex(entry[20])+dectohex(entry[27]) + dectohex(entry[26]) ;
    int bgC = hextodec(buffer);
    buffer = "";
    int sz=0;
    for (int i = 31; i >= 28; i--) {
        buffer += dectohex(entry[i]);
    }
    sz = hextodec(buffer);
    vector<int> staken= cntcluster(Glb_FAT, bgC);
    if (att == "08") {
        f = new Folder(name,sname, "Volume", bgC, staken);
    }
    else if (att == "10") {
        f = new Folder(name, sname, "Thu muc", bgC, staken);
        ReadSRdet(f, L"\\\\.\\F:", (Glb_Sf * Glb_Nf + Glb_Sb+(bgC-2)*Glb_Sc) * Glb_bps, staken.size());
    }
    else if (att == "20") {
        f = new File(name, sname, "Tap tin", bgC,staken, sz);
    }
    return f;
}

component* ReadRdet(LPCWSTR  drive, int readPoint, int n) {
    component* f = nullptr;
    BYTE sector[512];
    vector<wstring> Sname; //vector chua ten tu entry phu
    //doc cac sector cua rdet
    for (int i = 0; i < n; i++) {
        ReadSector(drive, readPoint + i, sector);
        for (int j = 0; j < 16; j++) {
            //lay ra 1 entry
            BYTE entry[32];
            for (int k = 0; k < 32; k++) {
                entry[k] = sector[j * 32 + k];
            }
            int atb = entry[11];
            //kiem tra cac entry phu
            if (dectohex(atb) == "0F") {
                Sname.push_back(readSEntry(entry));
            }
            else if (dectohex(atb) == "16") {
                Sname.clear();
                //file he thong, bo qua
            }
            else if (dectohex(atb) == "00") {
                //ket thuc bang rdet
                return f;
            }
            else {
                //lay ten tu entry phu bo vao entry chinh
                if (Sname.size()) {
                    wstring fullname;
                    for (int k = Sname.size() - 1; k >= 0; k--) {
                        fullname += Sname[k];
                    }  
                    readEntry(entry, fullname);
                    if (dectohex(atb) == "08") {
                        f = readEntry(entry, fullname);
                    }
                    else {
                        f->sub.push_back(readEntry(entry, fullname));
                    }
                    Sname.clear();
                }
                else {
                    if (dectohex(atb) == "08") {
                        f=readEntry(entry, L"");
                    }
                    else {
                        f->sub.push_back(readEntry(entry, L""));
                    }
                }
            }
        }
    }
    return f;
}

void ReadSRdet(component* outf, LPCWSTR  drive, int readPoint, int n) {
    BYTE sector[512];
    vector<wstring> Sname; //vector chua ten tu entry phu
    //doc cac sector cua rdet
    for (int i = 0; i < n; i++) {
        ReadSector(drive, readPoint + i, sector);
        for (int j = 2; j < 16; j++) {
            //lay ra 1 entry
            BYTE entry[32];
            for (int k = 0; k < 32; k++) {
                entry[k] = sector[j * 32 + k];
            }
            int atb = entry[11];
            //kiem tra cac entry phu
            if (dectohex(atb) == "0F") {
                Sname.push_back(readSEntry(entry));
            }
            else if (dectohex(atb) == "16") {
                Sname.clear();
                //file he thong, bo qua
            }
            else if (dectohex(atb) == "00") {
                //ket thuc bang rdet
                return;
            }
            else {
                //lay ten tu entry phu bo vao entry chinh
                if (Sname.size()) {
                    wstring fullname;
                    for (int k = Sname.size() - 1; k >= 0; k--) {
                        fullname += Sname[k];
                    }
                    readEntry(entry, fullname);
                    outf->sub.push_back(readEntry(entry, fullname));
                    Sname.clear();
                }
                else {
                    outf->sub.push_back(readEntry(entry, L""));
                }
            }
        }
    }
}

wstring readTXT(BYTE sector[512]) {
    
    string utf8 = "";
    wstring result;
    for (int i = 0; i < 512; i++)
    {
        if(sector[i])
        utf8 += sector[i];
    }
    try {
         result= strtowstr(utf8);
    }
    catch (...) {
        result = L"";
    }
    
    return result;
}

void Readfile(component* f) {
    BYTE sector[512];
    string hex;
    wstring data;
    for (int i = 0; i < f->staken.size(); i++) {
        ReadSector(L"\\\\.\\F:", (Glb_Sf * Glb_Nf + Glb_Sb + f->staken[i]) * Glb_bps, sector);
        wcout<<readTXT(sector);
    }
    wcout << data;
}

File::File(string n, wstring sn, string a, int bgC, vector<int> st, int s) {
    name = n;
    sname = sn;
    while (sname.find(L'\0') != -1) {
        sname.erase(sname.find(L'\0'), 1);
    }
    attribute = a;
    bgCluster = bgC;
    staken = st;
    size = s;
}

void File::show(int lv) {

    string level;
    for (int i = 0; i < lv; i++) {
        level += '\t';
    }

    wcout << strtowstr(level) << "Ten: " << strtowstr(name);
    if (name == "") {
        wcout << sname;
    }
    wcout << endl;
    wcout << strtowstr(level) << "Thuoc tinh: " << strtowstr(attribute) << endl;
    wcout << strtowstr(level) << "Cluster bat dau: " << bgCluster << endl;
    wcout << strtowstr(level) << "So cluster chiem: " << staken.size() / Glb_Sc << endl;
    wcout << strtowstr(level) << "Chiem cac sector tu " << staken[0] << " den " << staken[staken.size() - 1] << endl;
    wcout << strtowstr(level) << "Kich co: " << size << endl;

    int len = name.size();
    int slen = sname.size();
    wstring extension=sname;
    bool istxt=false;

    if (name != "") {
        if (name.find("txt")!=-1|| name.find("TXT") != -1) {
            istxt = true;
        }
    }
    else {
        if (sname.find(L"txt")!=-1|| sname.find(L"TXT") != -1) {
            istxt = true;
        }
    }
    if (istxt) {
        wcout << strtowstr(level) << L"Data: ";
        Readfile(this);
        wcout << endl;
    }
    else {
        wcout << strtowstr(level) << L"Khong ho tro loai tep nay!!!"<<endl;
    }
    wcout << endl ;
}

Folder::Folder(string n, wstring sn, string a, int bgC, vector<int> st) {
    name = n;
    sname = sn;
    while (sname.find(L'\0') != -1) {
        sname.erase(sname.find(L'\0'), 1);
    }
    attribute = a;
    bgCluster = bgC;
    staken = st;
    size = 0;
}

void Folder::show(int lv) {
    string level;
    for (int i = 0; i < lv; i++) {
        level += '\t';
    }
    wcout << strtowstr(level) << "Ten: " << strtowstr(name);
    if (name == "") {
        wcout << sname;
    }
    wcout << endl;
    wcout << strtowstr(level) << "Thuoc tinh: " << strtowstr(attribute) << endl;
    wcout << strtowstr(level) << "Cluster bat dau: " << bgCluster << endl;
    wcout << strtowstr(level) << "Kich co: " << size << endl << endl;
    int i = 0;

    for (i; i < sub.size(); i++) {
        sub[i]->show(lv + 1);
    }
}
