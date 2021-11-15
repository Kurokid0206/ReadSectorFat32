#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
#define MAX 100




unsigned char val(char c)
{
    if ('0' <= c && c <= '9') { return c - '0'; }
    if ('a' <= c && c <= 'f') { return c + 10 - 'a'; }
    if ('A' <= c && c <= 'F') { return c + 10 - 'A'; }
    throw "Eeek";
}

string decode(string const& s)
{
    if ((s.size() % 2) != 0) { throw "Eeek"; }

    string result;
    result.reserve(s.size() / 2);

    for (size_t i = 0; i < s.size() / 2; ++i)
    {
        unsigned char n = val(s[2 * i]) * 16 + val(s[2 * i + 1]);
        result += n;
    }

    return result;
}

void ReadFileName(string name, ostream& out)
{
    fstream f;
    f.open(name);
    string arr[MAX][MAX];

    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            f >> arr[i][j];
        }
    }

    vector<string> s1;
    string tendai = "";
    string tenngan = "";
    int cluster_bd = 0;
    int kich_thuoc = 0;

    int row = 0;
    for (row; row < 32; row += 2) {
        s1.clear();
        tendai = "";
        tenngan = "";
        cluster_bd = 0;
        kich_thuoc = 0;

        if (arr[row][11] == "0F")
        {

            while (arr[row][11] == "0F")
            {
                string temp;
                for (int i = row; i < row + 2; i++)
                {
                    for (int j = 0; j < 16; j++)
                    {
                        if (i == row)
                        {
                            if (j != 0 && j != 11 && j != 12 && j != 13)
                                temp += arr[i][j];
                        }
                        else if (i == row + 1)
                        {
                            if (j != 10 && j != 11)
                                temp += arr[i][j];
                        }
                    }
                }
                s1.push_back(decode(temp));
                row += 2;

            }

            for (int t = s1.size() - 1; t >= 0; t--)
            {
                tendai += s1[t]; // Tinh ten
            }
            if (arr[row][11] == "08")
            {
                out << "\nTen volume: " << tendai;
                out << "\nTrang thai: volume label.";
            }
            else if (arr[row][11] == "20")
            {
                out << "\nTen tap tin: " << tendai;
                out << "\nTrang thai: tap tin.";
            }
            else if (arr[row][11] == "10")
            {
                out << "\nTen thu muc: " << tendai;
                out << "\nTrang thai: thu muc.";
            }
            else if (arr[row][11] == "16")
            {
                out << "\nTen thu muc he thong: " << tendai;
                out << "\nTrang thai: thu muc.";
            }

            cluster_bd = hextodec(arr[row + 1][5] + arr[row + 1][4] + arr[row + 1][11] + arr[row + 1][10]);
            kich_thuoc = hextodec(arr[row + 1][15] + arr[row + 1][14] + arr[row + 1][13] + arr[row + 1][12]);
            if (arr[row][11] != "00")
            {
                out << "\nCluster bat dau: " << cluster_bd;
                out << "\nKich co: " << kich_thuoc;
            }
        }
        else
        {

            for (int k = 0; k < 11; k++)
            {
                tenngan += arr[row][k];
            }

            if (arr[row][11] == "08")
            {
                out << "\nTen volume: " << decode(tenngan);
                out << "\nTrang thai: volume label.";
            }
            else if (arr[row][11] == "20")
            {
                out << "\nTen tap tin: " << decode(tenngan);
                out << "\nTrang thai: tap tin.";
            }
            else if (arr[row][11] == "10")
            {
                out << "\nTen thu muc: " << decode(tenngan);
                out << "\nTrang thai: thu muc.";
            }
            else if (arr[row][11] == "16")
            {
                out << "\nTen thu muc he thong: " << decode(tenngan);
                out << "\nTrang thai: thu muc.";
            }
            cluster_bd = hextodec(arr[row + 1][5] + arr[row + 1][4] + arr[row + 1][11] + arr[row + 1][10]);
            kich_thuoc = hextodec(arr[row + 1][15] + arr[row + 1][14] + arr[row + 1][13] + arr[row + 1][12]);
            if (arr[row][11] != "00")
            {
                out << "\nCluster bat dau: " << cluster_bd;
                out << "\nKich co: " << kich_thuoc;
            }
        }
        out << endl;
    }
}

int main()
{
    ReadFileName("Text_rd.txt", cout);
    system("pause");
    return 0;
}