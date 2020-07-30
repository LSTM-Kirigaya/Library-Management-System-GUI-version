# 图书管理系统GUI版
## 一、程序的主要功能

- GUI版的图书管理系统继承了原来的所有类，所以具有原版本的所有功能。
- 使用PyQt5编写的图形用户界面加上C++编写的操作书库文件的接口，对用户更加友好同时保证了操作图书库的效率。
- 采用堆栈式的多页面设计，将UI的大篇幅交给了图书信息的展示，让查询大量书籍更加方便。
- 用户可在三个模块间自由切换，可随时查看购物车中的已选择的图书项。
- 实时保存文件， 以防突然退出程序导致修改失效。
- 内含控件文字复制粘贴按钮，让用户查询和购买更加快速。
- 用户可根据自身喜好设置全局字体。
- GUI的状态栏会提供操作成功与否的提示，提高了用户体验。




> 下面是GUI展示

<img src="https://i.loli.net/2020/07/29/31m75GZn2dWUyuh.png" alt="image.png" style="zoom:67%;" />





## 二、系统的总框架图
![image.png](https://i.loli.net/2020/07/29/RH9TWurJzl1KQXV.png)

需要说明的是，“C++项目”，“动态链接库”，“可供Python直接调用的静态方法”，“Python项目->GUI”分别对应于`CreateDLL文件夹`，`CreateDLL.dll`，`library.py`和`GUI.py`。其中`GUI.py`是最终的启动程序。

## 三、UI控件结构
使用PyQt5编写的UI，主体控件之间的排布如下：

![image.png](https://i.loli.net/2020/07/30/hKoXt3ASY7BElkR.png)


























