#include<iostream>
#include<algorithm>
#include "cheapest.h"

Solution::Solution():m_bPrint(false), m_eType(ALGOTYPE::DIJKSTRA)
{

}
Solution::~Solution()
{

}
int Solution::findCheapestPrice(int n, vector<vector<int>>& flights, int src, int des, int k)
{
    int ret = 0;
    switch(m_eType)
    {
        case DIJKSTRA:
            ret = dijkstra();
            break;
        case FLOYED:
            ret = floyed();
            break;
        case BELLMANFORD:
            ret = bellmanford();
            break;
        case CUSTOMH:
            ret = customH(n, flights, src, des, k);
            break;
        default:
            cout << "algo type not supported" << endl;
            break;
    }
    return ret;
}

void Solution::openPrintMsg(bool isPrint)
{
    m_bPrint = isPrint;
}

void Solution::setAlgoType(ALGOTYPE type)
{
    m_eType = type;
}

int Solution::dijkstra()
{
    cout << "dijkstra algo not implement" << endl;
    //todo
    return 0;
}

int Solution::floyed()
{
    cout << "floyed algo not implement" << endl;
    //todo
    return 0;
}

int Solution::bellmanford()
{
    cout << "bellmanford algo not implement" << endl;
    //todo
    return 0;
}

void Solution::searchRoute(vector<vector<int>>& flights, int src, int des, int k)
{
    for(auto it : flights)
    {
        if(it[0] == src && it[1] != des && find(m_route.begin(), m_route.end(), it[1]) == m_route.end())
        {
            if(k == 0)
                continue;
            m_nPrice += it[2];
            m_route.push_back(src);
            searchRoute(flights, it[1], des, k-1);
            m_nPrice -= it[2];
            m_route.pop_back();
        }
        if(it[0] == src && it[1] == des)
        {
            if(m_nCheapestPrice == -1 || m_nPrice + it[2] < m_nCheapestPrice)
            {
                m_nCheapestPrice = m_nPrice + it[2];
                m_cheapestRoute.clear();
                m_cheapestRoute.assign(m_route.begin(), m_route.end());
                m_cheapestRoute.push_back(src);
                m_cheapestRoute.push_back(des);
            }
        }
    }
}

int Solution::customH(int n, vector<vector<int>>& flights, int src, int des, int k)
{
    vector<int> citys;
    citys.clear();

    for(auto it : flights)
    {
        if(find(citys.begin(), citys.end(), it[0]) == citys.end())
            citys.push_back(it[0]);
        if(find(citys.begin(), citys.end(), it[1]) == citys.end())
            citys.push_back(it[1]);
    }

    if(find(citys.begin(), citys.end(), src) == citys.end() ||
        find(citys.begin(), citys.end(), des) == citys.end() ||
        citys.size() > n || k < 0)
    {
        cout << "input error" << endl;
        return -1;
    }

    m_nPrice = 0;
    m_nCheapestPrice = -1;
    m_route.clear();
    m_cheapestRoute.clear();
    searchRoute(flights, src, des, k);

    if(m_bPrint)
    {
        if(m_nCheapestPrice != -1)
        {
            cout << "cheapest price is " << m_nCheapestPrice << endl;
            cout << "best route is ";
            for(auto it : m_cheapestRoute)
                cout << it << " ";
            cout << endl;
        }else
        {
            cout << "there is no route from " << src << " to " << des << endl;
        }
    }

    return m_nCheapestPrice;
}
