#include <string.h>
#include <iostream>

using namespace std;

int main()
{
    int a[10];

    //将每个字节赋予给定的ascii码值, 第三个参数为字节数，对于多字节数据类型会出问题
    memset(a, 1, sizeof(a));
    // 16843009D  ==  00000001 00000001 00000001 00000001B

    for (int i = 0; i < 10; i++)
    {
        cout << a[i] << " ";
    }
}