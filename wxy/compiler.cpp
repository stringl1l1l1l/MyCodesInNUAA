#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;
extern int Mid_p; // ����ָ��
extern int call_p; // �ڵ���ʱ����¼���ص�ַ
extern int act_sp; // spָ��
extern int static_p; // ��̬��ָ��
extern int act_top;
extern vector<int> act;
extern stack<int> data_s; // ����������ջ
bool Compiler(tuple<string, int, int> Mid_code)
{
    int temp1, temp2;
    if (get<0>(Mid_code) == "JMP") {
        Mid_p = get<2>(Mid_code);
        return 1;
    } else if (get<0>(Mid_code) == "OPR") {
        Mid_p++;
        if (get<2>(Mid_code) == 0) { // ���̵��ý�����,���ص��õ㲢��ջ
            Mid_p = act[act_sp + 1]; // RA
            act_top = act_sp - 1;
            act_sp = act[act_sp]; // sp
        } else if (get<2>(Mid_code) == 1) { // ջ��Ԫ��ȡ��
            temp1 = data_s.top();
            data_s.pop();
            temp1 = 0 - temp1;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 2) { // ��ջ����ջ����ӣ�������ջԪ�أ����ֵ��ջ
            temp1 = data_s.top();
            data_s.pop();
            temp2 = data_s.top();
            data_s.pop();
            temp1 = temp1 + temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 3) { // ��ջ����ȥջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = temp1 - temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 4) { // ��ջ������ջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = temp1 * temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 5) { // ��ջ������ջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = temp1 / temp2;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 6) { // ջ��Ԫ�ص���ż�жϣ����ֵ��ջ��
            temp1 = data_s.top();
            temp2 = (temp1 % 2 == 1) ? 1 : 0;
            data_s.push(temp2);
        } else if (get<2>(Mid_code) == 7) { //

        } else if (get<2>(Mid_code) == 8) { // ��ջ����ջ���Ƿ���ȣ�������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 == temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 9) { // ��ջ����ջ���Ƿ񲻵ȣ�������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 != temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 10) { // ��ջ���Ƿ�С��ջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 < temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 11) { // ��ջ���Ƿ���ڵ���ջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 >= temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 12) { // ��ջ���Ƿ����ջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 > temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 13) { // ��ջ���Ƿ�С�ڵ���ջ����������ջԪ�أ����ֵ��ջ
            temp2 = data_s.top();
            data_s.pop();
            temp1 = data_s.top();
            data_s.pop();
            temp1 = (temp1 <= temp2) ? 1 : 0;
            data_s.push(temp1);
        } else if (get<2>(Mid_code) == 14) { // ջ��ֵ�������Ļ
            temp1 = data_s.top();
            cout << temp1 << endl;
            data_s.pop();
        } else if (get<2>(Mid_code) == 15) { // ��Ļ�������
            cout << endl;
        } else if (get<2>(Mid_code) == 16) { // �������ж���һ����������ջ��
            cin >> temp1;
            data_s.push(temp1);
        }
        return 1;
    } else if (get<0>(Mid_code) == "INT") { //***
        Mid_p++;
        act[act_top + 1] = act_sp;
        act_sp = act_top + 1;
        act_top = act_sp + get<2>(Mid_code) - 1;
        act[act_sp + 1] = call_p; //***�˴������д���RA main����ʱ
        act[act_sp + 2] = static_p; // ��̬��?
    } else if (get<0>(Mid_code) == "LIT") { // ȡ����a��������ջջ��
        data_s.push(get<2>(Mid_code));
        Mid_p++;
    } else if (get<0>(Mid_code) == "LOD") { // ȡ��������Ե�ַΪa�����ΪL���ŵ�����ջ��ջ��
        temp1 = act_sp;
        Mid_p++;
        for (int i = 0; i < get<1>(Mid_code); i++) {
            temp1 = act[temp1 + 2];
        }
        // cout <<"lodȡ��" << act[temp1 + get<2>(Mid_code)] << endl;
        data_s.push(act[temp1 + get<2>(Mid_code)]); //***
    } else if (get<0>(Mid_code) == "STO") { // ������ջջ�������ݴ����������Ե�ַΪa����β�ΪL��
        temp2 = data_s.top(); // ��պ�������
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
        // cout << "sto��" <<temp2<<"д��"<< temp1 + get<2>(Mid_code)<<endl;
        data_s.pop();
    } else if (get<0>(Mid_code) == "CAL") { // ���ù��̣�ת��ָ�����ڵ�ַΪa����β�ΪL��***���������̬��
        call_p = Mid_p + 1;
        Mid_p = get<2>(Mid_code); //***call�Ĳ�β
        if (get<1>(Mid_code) == -1) {
            static_p = act_sp;
        } else if (get<1>(Mid_code) == 0) {
            static_p = act[act_sp + 2];
        } else if (get<1>(Mid_code) > 0) {
            static_p = act[act_sp + 2];
            for (int i = 0; i < get<1>(Mid_code); i++) { // ��ǰ��k������ֵ
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