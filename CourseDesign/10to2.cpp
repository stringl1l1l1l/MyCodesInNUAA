#include <iostream>
#define uchar unsigned char
using namespace std;
void binary_print(uchar c)
{
    for (int i = 0; i < 8; ++i)
    {
        if ((c << i) & 0x80)
            cout << '1';
        else
            cout << '0';
    }
    cout << ' ';
}

int main()
{
    float a;
    uchar c_save[4];
    uchar i;
    void *f;
    f = &a;
    cout << "������һ��������:";
    cin >> a;
    cout << endl;
    for (i = 0; i < 4; i++)
    {
        c_save[i] = *((uchar *)f + i);
    }
    cout << "�˸������ڼ�����ڴ��д����ʽ���£�" << endl;
    for (i = 4; i != 0; i--)
        binary_print(c_save[i - 1]);
    cout << endl;
}