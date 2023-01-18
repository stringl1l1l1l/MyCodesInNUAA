#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
int main()
{
    int n = 0;
    double sum = 0;
    double avg = 0, d = 0;
    cin >> n;
    vector<int> arr;
    for (int i = 0; i < n; i++)
    {
        int temp;
        cin >> temp;
        sum += temp;
        arr.push_back(temp);
    }
    avg = sum / n;
    double dSum = 0;
    for (int i = 0; i < n; i++)
    {
        double diff = arr[i] - avg;
        dSum += diff * diff;
    }
    d = dSum / n;
    double sqrtD = sqrt(d);
    for (int i = 0; i < n; i++)
    {
        double f = (arr[i] - avg) / sqrtD;
        printf("%0.15f\n", f);
    }
}