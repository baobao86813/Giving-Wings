#ifndef CHARPICTURE_H
#define CHARPICTURE_H
#include<string>
#include<vector>

using namespace std;

struct PairCmp
{
    bool operator()(const pair<int, int>&p1, const pair<int, int>&p2)
    {
        return p1.second > p2.second;
    }
};

class CharPicture
{
public:
    CharPicture(bool makeGray, bool processSnake, string charSet);
    ~CharPicture();
    void createSimpleChar(const int col, const int row, const int quality);
    void convert(const string srcFilePath, int quality = 100, int resolutionLevel = 1);
private:
    void processSnake(unsigned char *data, int width, int height);
    void convertToGray(unsigned char *data, int width, int height);
    void convertToChar(unsigned char*&desData, const unsigned char*srcData, int &width, int &height, int resolutionLevel);
    int load(const string path, unsigned char*&data, int &width, int &height);
    void save(const string path, unsigned char*data, int width, int height, int quality);
private:
    bool m_makeGray = false;//是否生成灰度图
    bool m_processSnake = false;//是否需要特殊处理snake图片
    const int m_charHeight = 16;//单个ascii字符点阵高度
    const int m_charWidth = 8;//单个ascii字符点阵宽度
    unsigned char *m_srcData = nullptr;
    unsigned char *m_desData = nullptr;
    int m_width = 0;
    int m_height = 0;
    vector<pair<int, int>> m_mono2font;//保存像素灰度值与字符的映射关系
};
#endif