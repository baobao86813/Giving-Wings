#include"CharPicture.h"
#include"FontData.h"
#include <stdio.h>
#include<iostream>
#include<setjmp.h>
#include<cstring>
#include<algorithm>
#include<jpeglib.h>

CharPicture::CharPicture(bool makeGray, bool processSnake, string charSet):m_makeGray(makeGray),m_processSnake(processSnake)
{
    //建立灰度值与字符映射表
    if(!charSet.empty())
    {//使用自定义字符集
        for(auto i:charSet)
        {
            m_mono2font.push_back(make_pair(i,0));
            for(int m = 0; m < 16; ++m)
                for(int n = 0; n < 8; ++n)
                    if((fontdata_8x16[i*16+m] << n) & 0x80)
                        m_mono2font[m_mono2font.size()-1].second++;
        }
    }
    else
    {//使用所有ascii字符集
        for(int i = 0; i < 256; ++i)
        {
            m_mono2font.push_back(make_pair(i,0));
            for(int m = 0; m < 16; ++m)
                for(int n = 0; n < 8; ++n)
                    if((fontdata_8x16[i*16+m] << n) & 0x80)
                        m_mono2font[m_mono2font.size()-1].second++;
        }
    }
    sort(m_mono2font.begin(), m_mono2font.end(), PairCmp());
}

CharPicture::~CharPicture()
{
}

void CharPicture::createSimpleChar(const int char_col, const int char_row, const int quality)
{
    //创建简单字符图片
    //char_col:字符列数
    //char_row:字符行数
    //quality:图片质量
    //字符集使用ascii
    const int width = char_col*m_charWidth;
    const int height = char_row*m_charHeight;
    unsigned char *data = new unsigned char[width*height*3]{};
    const unsigned char *p = fontdata_8x16;
    for(int i = 0; i < char_row; ++i)
    {
        for(int j = 0; j < char_col; ++j)
        {
            //渲染单个字符到图片指定位置
            for(int m = 0; m < m_charHeight; ++m)
            {
                for(int n = 0; n < m_charWidth; ++n)
                {
                    int index = (i*char_col*m_charWidth*m_charHeight+m*char_col*m_charWidth+j*m_charWidth+n)*3;
                    if(!((fontdata_8x16[16*((i*char_col+j)%256)+m] << n) & 0x80))
                    {
                        data[index] = 255;
                        data[index+1] = 255;
                        data[index+2] = 255;
                    }
                }
            }
        }
    }
    save("./ascii.jpg", data, width, height, quality);
    delete[]data;
}

void CharPicture::convert(const string srcFilePath, int quality, int resolutionLevel)
{
    //解码jpeg图片为rgb数据
    if(!load(srcFilePath, m_srcData, m_width, m_height))
        return;

    if(m_processSnake)
    {//特殊处理snake图片
        processSnake(m_srcData, m_width, m_height);
        save("./snake_clear.jpg", m_srcData, m_width, m_height, quality);
    }

    if(m_makeGray)
    {//生成灰度图
        convertToGray(m_srcData, m_width, m_height);
        save("./gray.jpg", m_srcData, m_width, m_height, quality);
    }

    //生成字符图片
    int desWidth = m_width, desHeight = m_height;
    convertToChar(m_desData, m_srcData, desWidth, desHeight, resolutionLevel);
    save("./char.jpg", m_desData, desWidth, desHeight, quality);

    //清理
    delete[]m_srcData;
    delete[]m_desData;
    m_srcData = nullptr;
    m_desData = nullptr;
}

void CharPicture::convertToGray(unsigned char *data, int width, int height)
{
    //rgb转灰度图,不减少通道数量
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            int monoValue = (data[(width*i+j)*3] + data[(width*i+j)*3+1] + data[(width*i+j)*3+2])/3;
            data[(width*i+j)*3] = monoValue;
            data[(width*i+j)*3+1] = monoValue;
            data[(width*i+j)*3+2] = monoValue;
        }
    }
}

void CharPicture::convertToChar(unsigned char*&desData, const unsigned char*srcData, int &width, int &height, int resolutionLevel)
{
    //转换字符图片
    int char_col;
    int char_row;
    int l;
    //计算字符行列
    switch (resolutionLevel)
    {
    case 2:
        l = 2;
        char_col = width;
        char_row = height/2;
        break;
    case 3:
        l = 4;
        char_col = width/2;
        char_row = height/4;
        break;
    case 4:
        l = 8;
        char_col = width/4;
        char_row = height/8;
        break;
    case 5:
        l = 16;
        char_col = width/8;
        char_row = height/16;
        break;
    case 1:
    default:
        l = 1;
        char_col = width*2;
        char_row = height;
        break;
    }
    //计算字符图片分辨率
    width = char_col*m_charWidth;
    height = char_row*m_charHeight;
    desData = new unsigned char[2*width*height*3]{};
    for(int i = 0; i < char_row; ++i)
    {
        for(int j = 0; j < char_col; ++j)
        {
            //计算字符对应原图区域内的灰度值
            int monoValue = 0;
            for(int m = 0; m < l; ++m)
            {
                for(int n = 0; n < l/2; ++n)
                {
                    monoValue += srcData[(l*i*char_col*l/2+m*char_col*l/2+j*l/2+n)*3];
                    monoValue += srcData[(l*i*char_col*l/2+m*char_col*l/2+j*l/2+n)*3+1];
                    monoValue += srcData[(l*i*char_col*l/2+m*char_col*l/2+j*l/2+n)*3+2];
                }
            }
            monoValue/=3*l*l/2;
            if(resolutionLevel==1)
                monoValue = (srcData[(char_col/2*i+j/2)*3] + srcData[(char_col/2*i+j/2)*3+1] + srcData[(char_col/2*i+j/2)*3+2])/3;
            //根据灰度值找到应选择的字符渲染到字符图片指定位置
            for(int m = 0; m < m_charHeight; ++m)
            {
                for(int n = 0; n < m_charWidth; ++n)
                {
                    int desIndex = (i*char_col*m_charWidth*m_charHeight+m*char_col*m_charWidth+j*m_charWidth+n)*3;
                    if(!((fontdata_8x16[m_mono2font[monoValue*m_mono2font.size()/255].first*16+m] << n) & 0x80))
                    {
                        desData[desIndex] = 255;
                        desData[desIndex+1] = 255;
                        desData[desIndex+2] = 255;
                    }
                }
            }
        }
    }
}

void CharPicture::processSnake(unsigned char *data, int width, int height)
{
    //特殊处理snake图片

    //step 1
    //上半部分做上下镜像,[0,171,960,341]->[0,0,960,170]
    for(int i = 170; i>=0; --i)
    {
        for(int j = 0; j<width; ++j)
        {
            data[(i*width+j)*3] = data[((341-i)*width+j)*3];
            data[(i*width+j)*3+1] = data[((341-i)*width+j)*3+1];
            data[(i*width+j)*3+2] = data[((341-i)*width+j)*3+2];
        }
    }
    //step 2
    //第一步做镜像会把部分蛇身镜像到顶端，继续做二次镜像
    for(int i = 70; i>=0; --i)
    {
        for(int j = 0; j<width; ++j)
        {
            data[(i*width+j)*3] = data[((141-i)*width+j)*3];
            data[(i*width+j)*3+1] = data[((141-i)*width+j)*3+1];
            data[(i*width+j)*3+2] = data[((141-i)*width+j)*3+2];
        }
    }
    //step 3
    //下半部分做左右镜像
    for(int i = 1150; i < height; ++i)
    {
        for(int j = 0; j < width/2; ++j)
        {
            data[(i*width+width-j)*3] = data[(i*width+j)*3];
            data[(i*width+width-j)*3+1] = data[(i*width+j)*3+1];
            data[(i*width+width-j)*3+2] = data[(i*width+j)*3+2];
        }
    }
    //step 4
    //根据色域去除蓝色字体
    //没有步骤1-3也可以在此步骤去除蓝色字体，但是会形成较多空洞，造成后续步骤5补洞之后图片效果不好
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(data[(width*i+j)*3+2]>data[(width*i+j)*3]+30)
            {
                data[(width*i+j)*3] = 255;
                data[(width*i+j)*3+1] = 255;
                data[(width*i+j)*3+2] = 255;
            }
        }
    }
    //step 5
    //填补步骤4形成的空洞
    //使用简单的邻域平均值，邻域取3x3
    //正常情况下空洞邻域内有大于5个点不为空洞，则取平均值填补当前空洞
    //极端情况下遍历一遍图像后所有空洞点都不满足邻域内有大于5个非空洞点的要求，此时需要逐步降低该要求
    int holeCnt = 0,holeCntOld = -1;
    int t = 5;
    do
    {
        holeCnt = 0;
        for(int i = height-1; i >= 0; --i)
        {
            for(int j = 0; j < width; ++j)
            {
                if(data[(i*width+j)*3]==255&&data[(i*width+j)*3+1]==255&&data[(i*width+j)*3+2]==255)
                {//判断该点为空洞
                    int notHoleCnt = 0;
                    int left = j-1, right = j+1, top = i-1, bottom = i+1;
                    int r = 0,g = 0,b = 0;
                    left<0?left=0:int();
                    top<0?top=0:int();
                    right>width-1?right=width-1:int();
                    bottom>height-1?bottom=height-1:int();
                    //查找邻域内非空洞点并计算rgb
                    for(int m = top; m <= bottom; ++m)
                    {
                        for(int n = left; n <= right; ++n)
                        {
                            if(data[(m*width+n)*3]!=255||data[(m*width+n)*3+1]!=255||data[(m*width+n)*3+2]!=255)
                            {
                                r += data[(m*width+n)*3];
                                g += data[(m*width+n)*3+1];
                                b += data[(m*width+n)*3+2];
                                ++notHoleCnt;
                            }
                        }
                    }
                    if(notHoleCnt>t)
                    {//满足要求，补洞
                        data[(i*width+j)*3] = r/notHoleCnt;
                        data[(i*width+j)*3+1] = g/notHoleCnt;
                        data[(i*width+j)*3+2] = b/notHoleCnt;
                    }else
                    {//不满足要求，空洞数量增加，当某次遍历完成无空洞则结束补洞操作
                        ++holeCnt;
                    }
                }
            }
        }
        if(holeCntOld == holeCnt)//此次遍历没有符合要求的点，需降低要求
            --t;
        else if(t < 5)//此次遍历有符合要求的点，且上次遍历无符合要求的点，需要恢复t值
            t = 5;
        holeCntOld = holeCnt;
        // cout << holeCnt << endl;
    } while (holeCnt>0);
}

int CharPicture::load(const string filename, unsigned char*&data, int &width, int &height)
{
    /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
    struct jpeg_decompress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * infile;		/* source file */
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */

    if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename.c_str());
        return 0;
    }

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */

    (void) jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
    *   (a) suspension is not possible with the stdio data source, and
    *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
    * See libjpeg.doc for more info.
    */

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

    /* Step 5: Start decompressor */

    (void) jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    height = cinfo.output_height;
    width = cinfo.output_width;
    data = new unsigned char[height*width*3];
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        /* Assume put_scanline_someplace wants a pointer and sample count. */
        memcpy(data+(cinfo.output_scanline-1)*row_stride, buffer[0], row_stride);
        // put_scanline_someplace(buffer[0], row_stride);
    }

    /* Step 7: Finish decompression */

    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* After finish_decompress, we can close the input file.
    * Here we postpone it until after no more JPEG errors are possible,
    * so as to simplify the setjmp error logic above.  (Actually, I don't
    * think that jpeg_destroy can do an error exit, but why assume anything...)
    */
    fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    /* And we're done! */
    return 1;
}

void CharPicture::save(const string filename, unsigned char*data, int width, int height, int quality)
{

    JSAMPLE * image_buffer = data;	/* Points to large array of R,G,B-order data */
    int image_height = height;	/* Number of rows in image */
    int image_width = width;		/* Number of columns in image */
    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
    * stdio stream.  You can also write your own code to do something else.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to write binary files.
    */
    if ((outfile = fopen(filename.c_str(), "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename.c_str());
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = image_width; 	/* image width and height, in pixels */
    cinfo.image_height = image_height;
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
    row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);
}
