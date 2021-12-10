#include<iostream>
#include<cstring>

using namespace std;

int recursive(int n)
{
    if(n == 1)
        return 0;
    if(n == 2)
        return 1;
    return recursive(n-1) + recursive(n-2);
}

int unRecursive(int n)
{
    int temp[2] = {0, 1}, k = -1;
    if(n == 1)
        return 0;
    if(n == 2)
        return 1;
    for(int i = 3; i <= n; i++)
        temp[++k%2] = temp[0] + temp[1];
    return temp[k%2];
}

//用户交互
void userInput(int &funcNum, int &n)
{
    cout << "本程序计算斐波那契数列第n项数值" << endl;
    cout << "请输入n：";
    cin >> n;
    while(n <= 0)
    {
        cout << "输入有误，请重新输入" << endl;
        cin >> n;
    }

    cout << "请输入要使用的方法序号(输入数字1或2，1为递归，2为非递归)：";
    cin >> funcNum;
    if(funcNum != 1 && funcNum != 2)
    {
        cout << "输入有误，将使用方法1" << endl;
        funcNum = 1;
    }
}

int main(int argc, char* argv[])
{
    int i = 0;
    cout << i << "fsd" << ++i;
    int n = 0;
    int funcNum = 1;
    int ret = 0;

    userInput(funcNum, n);

    if(funcNum == 1)
        ret = recursive(n);
    else if(funcNum == 2)
        ret = unRecursive(n);

    cout << "斐波那契数列第" << n << "项为：" << ret << endl;

    return 0;
}