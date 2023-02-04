#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;
extern int Mid_p; // 代码指针
extern int call_p; // 在调用时来记录返回地址
extern int act_sp; // sp指针
extern int static_p; // 静态链指针
extern int act_top;
extern vector<int> act;
extern stack<int> data_s; // 单独的数据栈
bool Compiler(tuple<string, int, int> Mid_code)
{
    int temp1, temp2;
    if (get<0>(Mid_code) == "JMP") {
        Mid_p = get<2>(Mid_code);
        return 1;
    } else if (get<0>(Mid_code) == "OPR") {
        Mid_p++;
        if (get<2>(Mid_code) == 0) { // 过程调用结束后,返回调用点并退栈
            Mid_p = act[act_sp + 1]; // RA
            act_top = act_sp - 1;
            act_sp = act[act_sp]; // sp
        } else if (get<2>(Mid_code) == 1) { // 栈顶元素取反
            temp1 = data_s.top();
            data_s.pop();
            temp1 = 0 - temp1;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 2) { // 次栈顶与栈顶相加，退两个栈元素，结果值进栈
            temp1 = data_s.top();
            data_s.pop();
            temp2 = data_s.top();
            data_s.pop();
            temp1 = temp1 + temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 3) { // 次栈顶减去栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = temp1 - temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 4) { // 次栈顶乘以栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = temp1 * temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 5) { // 次栈顶除以栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = temp1 / temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 6) { // 栈顶元素的奇偶判断，结果值在栈顶
            temp1 = data_s.top();
            temp2 = (temp1 % 2 == 1) ? 1 : 0;
            data_s.push(temp2);
        } else if (get<2>(Mid_code) == 7) { //

        } else if (get<2>(Mid_code) == 8) { // 次栈顶与栈顶是否相等，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 == temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 9) { // 次栈顶与栈顶是否不等，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 != temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 10) { // 次栈顶是否小于栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 < temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 11) { // 次栈顶是否大于等于栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 >= temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 12) { // 次栈顶是否大于栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 > temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 13) { // 次栈顶是否小于等于栈顶，退两个栈元素，结果值进栈
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 <= temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 14) { // 栈顶值输出至屏幕
            temp1 = data_s.top();
            cout << temp1 << endl;
            data_s.pop();
        } else if (get<2>(Mid_code) == 15) { // 屏幕输出换行
            cout << endl;
        } else if (get<2>(Mid_code) == 16) { // 从命令行读入一个输入置于栈顶
            cin >> temp1;
            data_s.push(temp1);
        }
        return 1;
    } else if (get<0>(Mid_code) == "INT") { //***
        Mid_p++;
        act[act_top + 1] = act_sp;
        act_sp = act_top + 1;
        act_top = act_sp + get<2>(Mid_code) - 1;
        act[act_sp + 1] = call_p; //***此处可能有错误RA main函数时
        act[act_sp + 2] = static_p; // 静态链?
    } else if (get<0>(Mid_code) == "LIT") { // 取常量a放入数据栈栈顶
        data_s.push(get<2>(Mid_code));
        Mid_p++;
    } else if (get<0>(Mid_code) == "LOD") { // 取变量（相对地址为a，层差为L）放到数据栈的栈顶
        temp1 = act_sp;
        Mid_p++;
        for (int i = 0; i < get<1>(Mid_code); i++) {
            temp1 = act[temp1 + 2];
        }
        // cout <<"lod取了" << act[temp1 + get<2>(Mid_code)] << endl;
        data_s.push(act[temp1 + get<2>(Mid_code)]); //***
    } else if (get<0>(Mid_code) == "STO") { // 将数据栈栈顶的内容存入变量（相对地址为a，层次差为L）
        temp2 = data_s.top(); // 清空后无数据
        temp1 = act_sp;
        Mid_p++;
        if (get<1>(Mid_code) == -1) {
            act[act_top + get<2>(Mid_code) + 1] = temp2;
            data_s.pop();
            return 1;
        }
        for (int i = 0; i < get<1>(Mid_code); i++) {
            temp1 = act[temp1 + 2];
        }
        act[temp1 + get<2>(Mid_code)] = temp2; //***
        // cout << "sto将" <<temp2<<"写入"<< temp1 + get<2>(Mid_code)<<endl;
        data_s.pop();
    } else if (get<0>(Mid_code) == "CAL") { // 调用过程（转子指令）（入口地址为a，层次差为L）***这里算出静态链
        call_p = Mid_p + 1;
        Mid_p = get<2>(Mid_code); //***call的层次差？
        if (get<1>(Mid_code) == -1) {
            static_p = act_sp;
        } else if (get<1>(Mid_code) == 0) {
            static_p = act[act_sp + 2];
        } else if (get<1>(Mid_code) > 0) {
            static_p = act[act_sp + 2];
            for (int i = 0; i < get<1>(Mid_code); i++) { // 向前走k步里存的值
                static_p = act[static_p + 2];
            }
        }
    } else if (get<0>(Mid_code) == "JPC") {
        temp1 = data_s.top();
        if (temp1 == 0) {
            Mid_p = get<2>(Mid_code);
        } else
            Mid_p++;
        data_s.pop();
    }
    return 1;
}