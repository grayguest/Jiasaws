新手(不懂高深语法,就看过--->谭老的C----->windows 程序设计 第五版)初学win32 SDK Programming,这是第一个小游戏.

本程序为windows sdk下C语言编写,编译环境win2k3 standard + vc6.0.

加载的图片为Pic文件夹下的Default1.bmp,目前只能加载bmp文件,可以把想拼的图片放到Pic目录下改名为Default1.bmp即可.
程序会按照图片大小调整程序客户区大小.若您不想拼3*3的,可以修改代码前几行的

#define CXJIGSAW 3 //定义拼图维数
#define CYJIGSAW 3

CXJIGSAW 为横向块数
CYJIGSAW 为纵向块数 默认都为3

光标键Left Right Up Down操作.暂没添加鼠标操作.

编程大牛也曾经说过:源代码面前,没有神秘.



