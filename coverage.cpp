#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
int main()
{
    ifstream file("file.txt");
    string line;
    // 逐行读取文件，并执行每条指令
    while (getline(file, line))
    {
        // 执行指令
        system(line.c_str());
    }
    file.close();
    return 0;
}
