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


//abstract class for file and folder
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
//class file, contain a file information
class File :public component {
public:
    File(string n, wstring sn, string a, int bgC, vector<int> st, int s);
    void show(int lv);
};
//class folder, contain folder information and inner file/folder
class Folder :public component {
public:
    Folder(string n, wstring sn, string a, int bgC, vector<int> st);
    void show(int lv);
};

//Read a sector from read point
int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);
//read FAT table (auto call when read sector from drive 'x')
int ReadFAT(LPCWSTR  drive, int readPoint, BYTE sector[1964544]);
//read sector from drive 'x'
int readsectorfrom(string x);

//convert dec to hex
string dectohex(int n);
//convert hex to dec
int hextodec(string hexVal);
//convert a hex to wchar_t
wchar_t hextoUnicode(string hex);
//convert a string to wstring
wstring strtowstr(string str);
//read cnt bytes from addr in sector and convert into int
int sectortoint(BYTE sector[512], string addr, int cnt);
//Go to pos in fat and get the info of the cluster (end or next cluster)
string clusterinfat(BYTE sector[1964544], int pos);
//get the sectors from begin cluster(pos) to end cluster in fat table
vector<int> cntcluster(BYTE sector[1964544], int pos);
//read a sub entry
wstring readSEntry(BYTE entry[32]);
//read a entry then return a object (file/folder)
component* readEntry(BYTE entry[32], wstring name);
//read Rdet and return drive folder
component* ReadRdet(LPCWSTR  drive, int readPoint, int n);
//read Sub Rdet and push the child objects(file/folder) into outter folder(outf)
void ReadSRdet(component* outf, LPCWSTR  drive, int readPoint, int n);
//Read a txt file
wstring readTXT(BYTE sector[512]);
//read a file (txt for now)
void Readfile(component* f);