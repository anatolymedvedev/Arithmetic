#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    ifstream file1("input.txt");
    if (!file1)
    {
        cerr << "Crash!1" << '\n';
    }
    ifstream file2("result.txt");
    if (!file2)
    {
        cerr << "Crash!2" << '\n';
    }

    string str1;
    string str2;
    while(getline(file1, str2))
    {
        str1 += str2;
        if (!file1.eof())
        {
            str1 += '\n';
        }
    }
    string str3;
    string str4;
    while(getline(file2, str4))
    {
        str3 += str4;
        if (!file2.eof())
        {
            str3 += '\n';
        }
    }

    if (str1 == str3)
        cout << "Decoding was successful" << endl;
    else
        cout << "Crash" << endl;
    
    file1.close();
    file2.close();
    return 0;
}
