# CompilersTechnology

此为学习编译原理课程时使用Qt开发的桌面应用程序，尚存在部分bug，希望多多包涵 

如有需要可以下载上传的exe程序，或者源代码 

## （扩展）VS + Qt项目工程的打包：
首先需要在VS中使用Release模式下编译运行生成一个exe可执行程序（在此以CompilersTechnology.exe为例），但是该程序运行后会报错，因为还缺少该程序的所有依赖项，找到该exe在工程文件的路径 
  
例如：E:\git\work\CompilersTechnology\x64\Release 
  
复制该exe文件并且在桌面新建一个文件夹作为打包程序后存放的路径，我暂时命名为asd，在该文件夹下粘贴先前的exe文件 

然后需要使用Qt自带的环境打包工具windeployqt.exe把Qt的环境打包到asd文件夹，具体步骤如下： 

在Windows的搜索中输入Qt 6.8.0(msvc 2022 64-bit) （可能会因为版本不同而找不到该程序，输入Qt + 版本号即可，找到类似的程序） ，打开该程序输入 

`cd 你所创建的asd文件夹的路径`  

`cd C:\Users\dsq2\Desktop\asd` 

然后输入c：

`c:` 

最后输入windeployqt6.exe + 先前的exe文件名（CompilersTechnology.exe） 

`windeployqt6.exe CompilersTechnology.exe` 

等待打包完成就可以看见asd文件夹中已经多出许多文件 

### （注意）：在打包程序windeployqt6.exe执行过程中可能会出现如 VCINSTALLDIR is not set 的报错信息 
原因是VS的环境没有配置成功，需要在电脑的系统环境中配置 

首先需要找到VS的目录，例如：C:\Program Files\Microsoft Visual Studio\2022\Professional\VC 

复制该路径，后序配置环境需要使用该路径 

配置方法如下： 

找到“我的电脑”，右键，属性，高级属性，环境变量 

然后在系统变量中新建 

变量名为 VCINSTALLDIR

变量值为 C:\Program Files\Microsoft Visual Studio\2022\Professional\VC 

确定即可，在根据上面的打包步骤重新打包即可成功打包所有的程序以及其依赖项 

### （可选）Enigma Virtual Box
Enigma Virtual Box是一个打包程序的第三方软件，可以把程序以及其所有依赖项打包成只有一个exe文件的桌面应用程序



## 编译原理程序预览图： 
![CompilersTechnologyPreview](https://github.com/AkingDsq/CompilersTechnology/blob/master/img/CompilersTechnologyPreview.png)

## 文法判断功能预览图：
![文法类型判断预览图](https://github.com/AkingDsq/CompilersTechnology/blob/master/img/文法类型判断预览图.png)

## NFA确定化功能预览图
![NFA确定化预览图](https://github.com/AkingDsq/CompilersTechnology/blob/master/img/NFA确定化预览图.png)
