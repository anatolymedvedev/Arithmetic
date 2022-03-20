#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <cstddef>
#include <cmath>

using namespace std;

const unsigned short int batch = 8; //кол-во символов, кот. мы сжимаем в одно число

int main()
{
	ifstream file("input.txt");
    if (!file)
    {
        cerr << "Crash!1" << endl;;
    }
    ofstream result("output.txt", ios::out | ios::binary);
    if(!result)
    {
        cerr << "Crash!2" << endl;
    }

	map<char, int> freq;
    char s;
	unsigned long long count = 0;
    //подсчитываем частоты символов и кол-во считанных символов
    while ((s = file.get()) && !file.eof())                     
    {															
		freq[s]++;
		count++;											
	}

    file.clear();
	file.seekg(0);

    int freq_size = freq.size();
    result.write((char*)&batch, sizeof(batch));
    result.write((char*)&count, sizeof(count));
    result.write((char*)&freq_size, sizeof(freq_size));

    map<char, int>:: iterator iter;
    map<char, double*> table;
    double a = 0;
    double b = 0;

    //записываем мапу в шапку и находим отрезки для каждого символа
    for(iter = freq.begin(); iter != freq.end(); ++iter)
    {
        result.write((char*)&iter->first, sizeof(iter->first));
		result.write((char*)&iter->second, sizeof(iter->second));

        table[iter->first] = new double[3];
        table[iter->first][0] = double(iter->second) / double(count);       //считаем вероятность для символа
        b = a + table[iter->first][0];
        table[iter->first][1] = a;      //левая граница для символа
        table[iter->first][2] = b;      //правая граница для символа
        a = b;
    }

    // map<char, double*>:: iterator it;
    // for (it = table.begin(); it != table.end(); ++it)
    // {
    //     cout << it->first << "--" << table[it->first][1] << "--" << table[it->first][2] << "::" << table[it->first][0] << endl;
    // }

    file.clear();  
    file.seekg(0); 

    double left = 0;
    double right = 1;
    count = 0;                              //следит за упаковкой batch
    while ((s = file.get()) && !file.eof())
	{
        double l = left + table[s][1]*(right - left);
        double r = left + table[s][2]*(right - left);
        left = l;
        right = r;
        count++;

        if(count % batch == 0)
        {
            double num = 0;
            uint64_t temp = 0;
            int i = 0;
            while (!((left <= num ) && (num < right))) 
            {
                i++;
                //cout << count << endl;
                if ((1 / pow(2, i) + num) < right) 
                {
                    num += 1 / pow(2, i);
                    temp |= (uint64_t(1) << (i - 1));
                }
            }                
            for (int i = 0; i < 7; i++) 
            {
                unsigned char bit = 255 & (temp >> (i * 8));
                result.write((char*)&bit, sizeof(bit));
            }
            left = 0;
            right = 1;
            i = 0;
        }
    }

    if (count % batch)
	{
		uint64_t temp = 0;
		double num = 0;
		int i = 0;
		while (!((left <= num) && (num < right)))
		{
			i++;
			if (1 / pow(2, i) + num < right)
			{
				num += 1 / pow(2, i);
				temp |= (uint64_t(1) << (i - 1));
			}
		}
		for (int i = 0; i < 7; i++)
		{
			unsigned char bit = (temp >> (i * 8)) & 255;
			result.write((char*)&bit, sizeof(bit));
		}
        left = 0;
        right = 1;
        i = 0;
	}

    file.close();
    result.close();
	return 0;
}
