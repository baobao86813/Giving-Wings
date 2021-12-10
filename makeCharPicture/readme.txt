编译时确保系统安装libjpeg
    mac:brew install libjpeg
    ubuntu:apt install libjpeg

编译脚本为：compile.sh

使用示例(详细使用方法参照帮助信息，各参数可组合使用)：
1、./CharPicture
    输出帮助信息
2、./CharPicture -s snake.jpg
    只生成字符图片char.jpg，图片分辨率和压缩质量都为最高，字符集选取全体ascii
3、./CharPicture -s snake.jpg -g
    同1，并额外生成灰度图gray.jpg
4、./CharPicture -s snake.jpg -p
    同1，对图片进行特殊处理，并额外生成snake_clear.jpg
5、./CharPicture -s snake.jpg -c 20x20
    同1，并额外生成一张简单字符图片，该图片包含20x20个字符，按照ascii顺序排列
6、./CharPicture -s snake.jpg -m abcdefghijklmnopqrstuvwxyz
    同1，但字符集使用[abcdefghijklmnopqrstuvwxyz]



jpegsr6.zip是libjpeg源代码，当系统安装命令无法使用时可以用源码编译