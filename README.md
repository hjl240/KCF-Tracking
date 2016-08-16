# TrackTest4

说明
-----------

-   修改自KCFcpp，KCFcpp源码下载地址：https://github.com/joaofaro/KCFcpp
-   测试环境：Win7 64位 + Visual Studio 2015 + opencv3.1.0
-   只使用opencv3.1.0，不使用opencv_contribute
-   作者：HJL

环境配置
-----------
VC++目录-->包含目录，添加：
-   D:\opencv\opencv310\build\include
-   D:\opencv\opencv310\build\include\opencv
-   D:\opencv\opencv310\build\include\opencv2
-   .\src
-   注：（src是KCFcpp中的一个文件夹，需要添加进包含目录）

VC++目录-->库目录，添加：
-   D:\opencv\opencv310\build\x64\vc14\lib

链接器-->输入-->附加依赖项
-   添加相应的.lib文件

