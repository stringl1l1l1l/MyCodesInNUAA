#include <iostream>
#include <vector>
using namespace std;

template <typename T>
void binary2Char(vector<char> &result, T num)
{

    int byteLenth = sizeof(num);
    unsigned char *bitStream = new unsigned char[byteLenth];
    for (int i = 0; i < byteLenth; i++)
    {
        //注意: 强转从低位开始截断
        bitStream[i] = *((unsigned char *)&num + byteLenth - i - 1);
    }

    for (int i = 0; i < byteLenth; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if ((bitStream[i] << j) & 0x80)
                result.push_back('1');
            else
                result.push_back('0');
        }
    }
}

int main() {

    vector<char> byteStream;
    binary2Char(byteStream, 16843009);

    int count = 1;
    for (vector<char>::iterator beg = byteStream.begin(); beg != byteStream.end(); beg++)
    {
        cout << *beg;
        if(! (count++ % 8) )
            cout << " ";
    }
}
