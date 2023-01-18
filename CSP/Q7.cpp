/*一次放学的时候，小明已经规划好了自己回家的路线，并且能够预测经过各个路段的时间。
同时，小明通过学校里安装的“智慧光明”终端，看到了出发时刻路上经过的所有红绿灯的指示状态。
请帮忙计算小明此次回家所需要的时间。

输入格式：
输入的第一行包含空格分隔的三个正整数 r、y、g，表示红绿灯的设置。这三个数均不超过 106。
输入的第二行包含一个正整数 n，表示小明总共经过的道路段数和路过的红绿灯数目。
接下来的 n 行，每行包含空格分隔的两个整数 k、t。
k=0 表示经过了一段道路，将会耗时 t 秒，此处 t 不超过 106；
k=1、2、3 时，分别表示出发时刻，此处的红绿灯状态是红灯、黄灯、绿灯，且倒计时显示牌上显示的数字是 t，
此处 t 分别不会超过 r、y、g。

输出格式：
输出一个数字，表示此次小明放学回家所用的时间。

样例输入：
30 3 30
8
0 10
1 5
0 11
2 2
0 6
0 3
3 10
0 3

样例输出：
46

样例说明：
小明先经过第一段路，用时10秒。第一盏红绿灯出发时是红灯，还剩5秒；
小明到达路口时，这个红绿灯已经变为绿灯，不用等待直接通过。
接下来经过第二段路，用时11秒。第二盏红绿灯出发时是黄灯，还剩两秒；
小明到达路口时，这个红绿灯已经变为红灯，还剩11秒。
接下来经过第三、第四段路，用时9秒。第三盏红绿灯出发时是绿灯，还剩10秒；
小明到达路口时，这个红绿灯已经变为红灯，还剩两秒。接下来经过最后一段路，用时3秒。
共计10+11+11+9+2+3=46秒。

评测用例规模与约定：
　　有些测试点具有特殊的性质：
　　* 前2个测试点中不存在任何信号灯。
　　测试点的输入数据规模：
　　* 前6个测试点保证n≤1000。
　　* 所有测试点保证n≤100000。
*/

#include <cstdio>

typedef struct StepPair
{
    int light;
    long long time;
} StepPair;

int main()
{
    long long r = 0, y = 0, g = 0;
    scanf("%lld %lld %lld", &r, &y, &g);
    long long timeOfLight[4] = {0, r, r + g, r + g + y};

    int n = 0, light = 0;
    long long time = 0;
    StepPair initTime[100001] = {{0, 0}};
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d%lld", &light, &time);
        getchar();
        initTime[i].light = light;
        initTime[i].time = time;
    }

    long long t = 0;
    for (int i = 0; i < n; i++)
    {
        long long lightTime = 0;

        //如果没有信号灯直接加上时间
        if (initTime[i].light == 0)
            t += initTime[i].time;
        //有信号灯计算此时信号灯颜色和剩余时间
        else
        {
            int index = 0;
            switch (initTime[i].light)
            {
            case 1: //初始为红灯
                index = 1;
                break;
            case 2: //初始为黄灯
                index = 3;
                break;
            case 3: //初始为绿灯
                index = 2;
                break;
            }
            lightTime = timeOfLight[index] - initTime[i].time;

            long long cur = (t + lightTime) % (r + y + g);
            //遇到红灯和黄灯加上到绿灯的时间
            if (lightTime && cur >= 0 && cur <= r)
                t += r - cur;
            else if (lightTime && cur >= r + g && cur < r + g + y)
                t += r + g + y - cur + r;
        }
    }
    printf("%lld", t);
    return 0;
}