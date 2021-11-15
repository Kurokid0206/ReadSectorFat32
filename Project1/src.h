#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <wchar.h>
#include <io.h>
#include <fcntl.h>    //set win mode
#include <locale> 
#include <codecvt> 

using namespace std;

extern int Glb_bps;
extern int Glb_Sc;
extern int Glb_Sb;
extern int Glb_Nf;
extern int Glb_Nv;
extern int Glb_Sf;
extern int Glb_bgcluster;
extern int Glb_bgsecter;
extern BYTE Glb_FAT[1964544];


class component {
public:
    string name;
    wstring sname;
    string attribute;
    int bgCluster=0;
    vector<int> staken;
    int size=0;
    vector<component*> sub;
    virtual void show(int) = 0;
};

class File :public component {
public:
    File(string n, wstring sn, string a, int bgC, vector<int> st, int s);
    void show(int lv);
};

class Folder :public component {
public:
    Folder(string n, wstring sn, string a, int bgC, vector<int> st);
    void show(int lv);
};

int readsectorfrom(string x);
int ReadFAT(LPCWSTR  drive, int readPoint, BYTE sector[1964544]);
int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);
string dectohex(int n);
wstring strtowstr(string str);
int hextodec(string hexVal);
int sectortoint(BYTE sector[512], string addr, int cnt);
string clusterinfat(BYTE sector[1913856], int pos);
vector<int> cntcluster(BYTE sector[1913856], int pos);

wstring readSEntry(BYTE entry[32]);
component* readEntry(BYTE entry[32], wstring name);
component* ReadRdet(LPCWSTR  drive, int readPoint, int n);
void ReadSRdet(component* outf, LPCWSTR  drive, int readPoint, int n);
wstring readTXT(BYTE sector[512]);
void Readfile(component* f);