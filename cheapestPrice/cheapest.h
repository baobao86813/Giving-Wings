#ifndef __CHEAPEST_H__
#define __CHEAPEST_H__

#include<vector>

using namespace std;

enum ALGOTYPE{
    DIJKSTRA = 1,
    FLOYED,
    BELLMANFORD,
    CUSTOMH
};

class Solution{
    public:
        Solution();
        ~Solution();
        int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int des, int k);
        void openPrintMsg(bool isPrint);
        void setAlgoType(ALGOTYPE type);
    private:
        int dijkstra();
        int floyed();
        int bellmanford();
        void searchRoute(vector<vector<int>>& flights, int src, int des, int k);
        int customH(int n, vector<vector<int>>& flights, int src, int des, int k);
    private:
        bool m_bPrint;
        ALGOTYPE m_eType;
        int m_nPrice;
        vector<int> m_route;
        int m_nCheapestPrice;
        vector<int> m_cheapestRoute;
};

#endif