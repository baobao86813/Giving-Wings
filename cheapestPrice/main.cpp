#include<iostream>
#include "cheapest.h"

using namespace std;

int main(int argc, char* argv[])
{
    //可以有回路，但不能出现负权值
    vector<vector<int>> flights = {{0,1,100},{0,2,500},{1,2,100},{0,3,5},{3,4,6},{4,5,7},{5,2,13}};
    int n = 6, src = 4, des = 2, k = 1, price = 0;

    Solution s;
    cout << "use algo CUSTOMH" << endl;
    s.setAlgoType(ALGOTYPE::CUSTOMH);
    s.openPrintMsg(true);
    price = s.findCheapestPrice(n, flights, src, des, k);

    return 0;
}