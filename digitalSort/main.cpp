#include<iostream>
#include<cstring>
#include<time.h>
#include<vector>

using namespace std;

/*****************************
    function：将数字列表分类，前半部分放奇数，后半部分放偶数，且不改变原列表顺序
    parameter：
        digitalArray：数组指针
        arraySize:数组长度
    时间复杂度：O(2n)
    空间复杂度：O(n)
*****************************/
void sort_one(int*digitalArray, int arraySize)
{
    int *digitalArrayTemp = new int[arraySize];//引入临时空间
    int k = 0;

    for(int i = 0; i < arraySize; i++)
        if(digitalArray[i] % 2 == 1)
            digitalArrayTemp[k++] = digitalArray[i];

    for(int i = 0; i < arraySize; i++)
        if(digitalArray[i] % 2 == 0)
            digitalArrayTemp[k++] = digitalArray[i];
    
    memcpy(digitalArray, digitalArrayTemp, sizeof(int) * arraySize);

    delete[]digitalArrayTemp;
}

/*****************************
    function：将数字列表分类，前半部分放奇数，后半部分放偶数，且不改变原列表顺序
    parameter：
        digitalArray：数组指针
        arraySize:数组长度
    时间复杂度：大于O(n),小于O(2n),平均待研究
    空间复杂度：大于O(0),小于O(1/2n),平均待研究
*****************************/
void sort_two(int*digitalArray, int arraySize)
{
    //整体思想如下：
    //前两个循环整体把列表遍历一遍，第一个循环找到列表第一个偶数，第二个循环把奇数依次放到正确位置，遇到偶数则放到临时可变数组temp
    //后两个循环倒序填充偶数,其中第三个循环填充列表中剩余的偶数，第四个循环填充临时可变数组中的偶数

    int i = -1, j = -1, p = arraySize, q = arraySize;
    vector<int> temp;

    while(digitalArray[++i] % 2 == 1);

    j = i;
    temp.push_back(digitalArray[i]);

    while(++i < arraySize)
        if(digitalArray[i] % 2 == 1)
        {
            digitalArray[j++] = digitalArray[i];
            if(digitalArray[j] % 2 == 0)
                temp.push_back(digitalArray[j]);
        }
    
    while(--p > j)
        if(digitalArray[p] % 2 == 0)
            digitalArray[--q] = digitalArray[p];

    while(!temp.empty())
    {
        digitalArray[--q] = temp.back();
        temp.pop_back();
    }
}

/*****************************
    function：初始化给定的数字列表
    parameter：
        digitalArray：数组指针
        arraySize:数组长度
        isRandom：是否随机填充数值，如果非随机，则按照1，2，3，4，...，n的样式初始化
*****************************/

void initDigitalArray(int*digitalArray, int arraySize, bool isRandom)
{
    srand(time(NULL));
    for(int i = 0; i < arraySize; i++)
    {
        if(isRandom)
            digitalArray[i] = rand() % arraySize + 1;
        else
            digitalArray[i] = i + 1;
    }
}

//显示数字列表
void showDigitalArray(const int*digitalArray, int arraySize)
{
    cout << "          ";
    for(int i = 0; i < arraySize; i++)
        cout << digitalArray[i] << "  ";
    cout << endl;
}

//用户交互
void userInput(int &funcNum, int &arraySize, bool &isRandom)
{
    cout << "本程序将数字列表分类，奇数放前半部分，偶数放后半部分，且原始顺序不变" << endl << endl;
    cout << "程序提供两种分类方法(实现思想参见代码注释)：" << endl;
    cout << "    方法1：时间复杂度O(2n)，空间复杂度：O(n)" << endl;
    cout << "    方法2：时间复杂度：大于O(n),小于O(2n),平均待研究，空间复杂度：大于O(0),小于O(1/2n),平均待研究" << endl;
    cout << endl;
    cout << "请输入要使用的方法序号(输入数字1或2)：";
    cin >> funcNum;
    if(funcNum != 1 && funcNum != 2)
    {
        cout << "输入有误，将使用方法1" << endl;
        funcNum = 1;
    }
    cout << "请输入数字列表长度：";
    cin >> arraySize;
    if(arraySize <= 0)
    {
        cout << "输入有误，长度将使用10" << endl;
        arraySize = 10;
    }
    cout << "请输入数字列表初始化方式(输入数字1或2;1为随机正整数，2则按照1,2,3,...,n)：";
    int r = 0;
    cin >> r;
    if(r != 1 && r != 2)
    {
        cout << "输入有误，将使用随机数填充方式初始化" << endl;
        r = 1;
    }
    isRandom = (r == 1);
}

int main(int argc, char* argv[])
{
    int arraySize = 0;
    bool isRandom = true;
    int *digitalArray = NULL;
    int funcNum = 1;

    //用户交互
    userInput(funcNum, arraySize, isRandom);
    digitalArray = new int[arraySize];

    //初始化数字列表
    cout << "原始列表：" << endl;
    initDigitalArray(digitalArray, arraySize, isRandom);

    //显示分类前的数字列表
    showDigitalArray(digitalArray, arraySize);

    if(funcNum == 1)
        sort_one(digitalArray, arraySize);
    else if(funcNum == 2)
        sort_two(digitalArray, arraySize);

    //显示分类后的数字列表
    cout << "分类后列表：" << endl;
    showDigitalArray(digitalArray, arraySize);

    delete[]digitalArray;

    return 0;
}