#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cstddef>
#include <cmath>

using namespace std;

int main()
{
    ifstream file("output.txt", ios::binary);
    if (!file)
    {
        cerr << "Crash!1" << endl;;
    }
	  ofstream result("result.txt");
    if(!result)
    {
        cerr << "Crash!2" << endl;
    }

    int freq_size;
    int freq;
    unsigned short int batch;
    unsigned long long count = 0;
    char s;

    file.read((char*)&batch, sizeof(batch));
    file.read((char*)&count, sizeof(count));
    file.read((char*)&freq_size, sizeof(freq_size));

    map <char, double*> table;
    double a = 0;
    double b = 0;
    while (freq_size > 0) 
    {
        file.read((char*)&s, sizeof(s));
        file.read((char*)&freq, sizeof(freq));

        table[s] = new double[3];
        table[s][0] = double(freq) / double(count);
        b = a + table[s][0];
	table[s][1] = a;
	table[s][2] = b;
	a = b;
        freq_size--;
    }

    // map<char, double*>:: iterator it;
    // for (it = table.begin(); it != table.end(); ++it)
    // {
    //     cout << it->first << "--" << table[it->first][1] << "--" << table[it->first][2] << "::" << table[it->first][0] << endl;
    // }

    double num = 0;
    int temp = batch;
    while(count > 0)
    {
      	  double left = 0;
	  double right = 1;

	  for (int j = 0; j < 7; j++)
	  {
		  char buf;
		  file.read((char*)&buf, sizeof(buf));
		  for (int i = 0; i < 8; i++)
		  {
			  if ((1 << i) & buf)
			  {
				  num += double(1) / pow(2, (j * 8) + i + 1);
			  }
		  }
	  }

	  for (int i = batch; i > 0; i--)
	  {
		  for (auto c : table)
		  {
			  double l = left + table[c.first][1] * (right - left);
			  double r = left + table[c.first][2] * (right - left);
			  if (l <= num && num < r)
			  {
				  count--;
				  left = l;
				  right = r;
				  result << c.first;
				  break;
			  }
		  }
		  if (!count) 
		  {
			  break;
		  }
	  }
	  num = 0;
    }

    file.close();
    result.close();
    return 0;
}
