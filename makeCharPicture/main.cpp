#include"CharPicture.h"
#include<iostream>
#include<sstream>
#include<unistd.h>
#include<cstring>

void usage()
{
    cout << "this program will convert an image to char image named char.jpg, option -s must be given, please adjust -r param if src image resolution is too high, otherwise program may abort" << endl << endl;
    cout << "usage:" << endl;
    cout << "  -s srcpath" << endl;
    cout << "  -q value" << endl;
    cout << "     output picture quality,0-100(default 100),means" << endl;
    cout << "         0   lowest" << endl;
    cout << "         100 highest" << endl;
    cout << "  -r value" << endl;
    cout << "     char image resolution,1-5(default 1),means:" << endl;
    cout << "         1      pix use 2 char" << endl;
    cout << "         2x2    pix use 2 char" << endl;
    cout << "         4x4    pix use 2 char" << endl;
    cout << "         8x8    pix use 2 char" << endl;
    cout << "         16x16  pix use 2 char" << endl;
    cout << "                1 char has 8x16 pix" << endl;
    cout << "  -g" << endl;
    cout << "     make a gray image named gray.jpg" << endl;
    cout << "  -p" << endl;
    cout << "     process snake image, don't use this option if not a snake image, will make an image named snake_clear.jpg" << endl;
    cout << "  -c colxrow" << endl;
    cout << "     make simple char image named ascii.jpg, format example:120x120(not pix)" << endl;
    cout << "  -m charset" << endl;
    cout << "     use a custom char set to make char image" << endl;
}

int main(int argc, char *argv[])
{
    int ch;
    string src,charset;
    int quality = 100,resolutionLevel = 1,char_col = 0,char_row = 0;
    bool makeGray = false, processSnake = false;
    stringstream ss;
    while( (ch = getopt(argc, argv, "s:q:r:gpc:m:")) != -1 )
    {
        switch(ch)
        {
            case 's':
                src = optarg;
                break;
            case 'q':
                ss << optarg;
                ss >> quality;
                if(quality < 0 || quality > 100)
                    quality = 100;
                break;
            case 'r':
                ss.clear();
                ss << optarg;
                ss >> resolutionLevel;
                if(resolutionLevel < 1 || resolutionLevel > 5)
                    resolutionLevel = 1;
                break;
            case 'g':
                makeGray = true;
                break;
            case 'p':
                processSnake = true;
                break;
            case 'm':
                charset = optarg;
                break;
            case 'c':
                ss.clear();
                ss << strtok(optarg,"x");
                ss >> char_col;
                ss.clear();
                ss << strtok(nullptr,"x");
                ss >> char_row;
                break;
            case '?':
            default:
                usage();
                exit(0);
                break;
        }
    }
    if(src.empty())
    {
        cout << "image src path is empyt" << endl << endl;
        usage();
        exit(0);
    }
    CharPicture charPicture(makeGray, processSnake, charset);
    charPicture.convert(src, quality, resolutionLevel);
    if(char_col > 0 && char_row > 0)
        charPicture.createSimpleChar(char_col, char_row, quality);
    return 0;
}
