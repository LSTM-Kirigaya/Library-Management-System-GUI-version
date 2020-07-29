
#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<iomanip>
#include<cctype>
#include<algorithm>
#include<string>
#include<vector>
#include<list>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<windows.h>
#include<sstream>
#include<fstream>
#include<ctime>
#include<conio.h>
using namespace std;

#define EXP extern "C" _declspec(dllexport)



/*
	- 申明主要的类
	- TitleInfo:  保存图书的基本信息， 主要包含书名、出版社、ISBN号
				  和作者等，以及对这些成员的操作方法
	- BookData:   核心类，继承了TitleInfo类，包含了对TitleInfo所有
				  数据成员的输入输出函数以及对这些成员函数的操作方法
	- Sale:       是BookData的子类，用于处理销售管理
	- Report:     对应<报表模块>的功能函数。由几个函数构成，并没有定
				  义数据成员，它起到限定函数范围的作用，内部都是静态
				  函数成员。这些函数都与报表功能有关
	- Management: 对应<管理模块>的功能函数。与Report作用类似，只有静
				  态函数成员，这些函数的功能都与管理书库的有关
	- Reception:  对应<收银模块>的功能函数。与Report作用类似，只有静
				  态函数成员，这些函数的功能都与收银有关
*/

class TitleInfo
{
protected:
	char isbn[65];        // 书的ISBN号
	char bookTitle[51];   // 书名
	char author[31];      // 作者名
	char publisher[31];   // 出版社的名称
	char dateAdded[11];   // 存放书店进书的日期，比如2017-1-1
	int qtyOnHand;        // 存放该书的库存量
	double wholesale;     // 存放该书的批发价格
	double retail;        // 存放该书的零售价

public:
	// TitleInfo类的构造函数
	TitleInfo()
	{
		isbn[0] = '\0';
		bookTitle[0] = '\0';
		author[0] = '\0';
		publisher[0] = '\0';
		dateAdded[0] = '\0';
		this->qtyOnHand = 0;
		this->wholesale = 0;
		this->retail = 0;
	}
	TitleInfo(char* isbn, char* bookTitle, char* author,
		char* publisher, char* dateAdded, int qty,
		double whole, double retail)
	{
		strcpy(this->isbn, isbn);
		strcpy(this->bookTitle, bookTitle);
		strcpy(this->author, author);
		strcpy(this->publisher, publisher);
		strcpy(this->dateAdded, dateAdded);
		this->qtyOnHand = qty;
		this->wholesale = whole;
		this->retail = retail;
	}

	TitleInfo(const char* isbn, const char* bookTitle, const char* author,
		const char* publisher, const char* dateAdded, int qty,
		double whole, double retail)
	{
		strcpy(this->isbn, isbn);
		strcpy(this->bookTitle, bookTitle);
		strcpy(this->author, author);
		strcpy(this->publisher, publisher);
		strcpy(this->dateAdded, dateAdded);
		this->qtyOnHand = qty;
		this->wholesale = whole;
		this->retail = retail;
	}

	// 设置数据成员的函数
	void setIsbn(char* str) { strcpy_s(this->isbn, str); }
	void setBookTitle(char* str) { strcpy_s(this->bookTitle, str); }
	void setAuthor(char* str) { strcpy_s(this->author, str); }
	void setPublisher(char* str) { strcpy_s(this->publisher, str); }
	void setDateAdded(char* str) { strcpy_s(this->dateAdded, str); }
	void setQtyOnHand(int x) { this->qtyOnHand = x; }
	void setWholesale(double x) { this->wholesale = x; }
	void setRetail(double x) { this->retail = x; }

	// 访问数据成员的函数
	char* getIsbn() { return this->isbn; }
	char* getBookTitle() { return this->bookTitle; }
	char* getAuthor() { return this->author; }
	char* getPublisher() { return this->publisher; }
	char* getDateAdded() { return this->dateAdded; }
	int getQtyOnHand() { return this->qtyOnHand; }
	double getWholesale() { return this->wholesale; }
	double getRetail() { return this->retail; }

};

class BookData : public TitleInfo
{
public:
	// BookData的构造函数
	BookData() {}
	BookData(char* isbn, char* bookTitle, char* author,
		char* publisher, char* dateAdded, int qty,
		double whole, double retail)
		: TitleInfo(isbn, bookTitle, author, publisher, dateAdded, qty, whole, retail)
	{}

	// 定义拷贝构造函数
	BookData(const BookData& obj)
	{
		strcpy(this->isbn, obj.isbn);
		strcpy(this->bookTitle, obj.bookTitle);
		strcpy(this->author, obj.author);
		strcpy(this->publisher, obj.publisher);
		strcpy(this->dateAdded, obj.dateAdded);
		this->qtyOnHand = obj.qtyOnHand;
		this->wholesale = obj.wholesale;
		this->retail = obj.retail;
	}

	// 重载赋值运算符
	BookData& operator=(const BookData right)
	{
		if (this == &right) return *this;
		strcpy(this->isbn, right.isbn);
		strcpy(this->bookTitle, right.bookTitle);
		strcpy(this->author, right.author);
		strcpy(this->publisher, right.publisher);
		strcpy(this->dateAdded, right.dateAdded);
		this->qtyOnHand = right.qtyOnHand;
		this->wholesale = right.wholesale;
		this->retail = right.retail;
	}

	// 展示书的全部信息
	void displayInfo(int width1 = 5, int width2 = 7);

};

class Sale :public BookData
{
private:
	int num;					// 该种书购买的数量

public:
	static double taxRate;		// 税率，对于任何一种书，税率相同，故设为静态变量

	// 拷贝构造函数，其中参数是一个BookData对象，因为购买书是会用BookData对象初始化Sale
	Sale(const BookData& book) :BookData(book)
	{
		num = 0;
	}

	Sale(const BookData& book, int num) :BookData(book)
	{
		this->num = num;
	}

	void setNum(int x) { num = x; }
	int getNum() { return num; }
	double calcuSaleMoney();			// 计算该本书总的销售额
	double calcuTax();					// 计算该本书应交的税
	double calcuTotalMoney();			// 计算该本书总的应交的钱

};

class Report
{
public:
	/*几个报表函数
		- listAll:        列出书库中所有图书的完整信息
		- listByWholesale:列出书库中所有图书的批发价、库存量以及批发价总额
		- listByRetail:   列出书库中所有图书的零售价、库存量以及零售价总额
		- listByNum:	  先对图书按库存量从大到小排序，然后列出所有图书的库存量
		- listByValue:    先对图书按每种批发价总额从大到小进行排序，然后列出书库
						  中所有图书的批发价、库存量以及批发价总额
		- listByDate:     先对图书按照入库日期排序，然后列出书库中所有图书的日期
	*/
	static void listAll(list<BookData>&);
	static void listByWholesale(list<BookData>&);
	static void listByRetail(list<BookData>&);
	static void listByNum(list<BookData>&, bool reverse = false);
	static void listByValue(list<BookData>&, bool reverse = false);
	static void listByDate(list<BookData>&, bool reverse = false);
	static bool cmpByNum1(BookData&, BookData&);
	static bool cmpByNum2(BookData&, BookData&);
	static bool cmpByValue1(BookData&, BookData&);
	static bool cmpByValue2(BookData&, BookData&);
	static bool cmpByDate1(BookData&, BookData&);
	static bool cmpByDate2(BookData&, BookData&);
};

class Management
{
public:

	/* 几个管理的函数:
			- strUpper:       将参数指针指向的字符串中小写字母转换成大写字母
			- lookUpBook:     要求用户输入书名，然后在书库中查找该书，如果没找到
							  在屏幕上显示一个出错信息，以表明书库中没有该书。如
							  果找到了该书，调用BookInfo中的函数，显示该书的信息，
							  支持模糊查找，会匹配列出书名，作者，出版社含有关键字
							  的所有匹配结果
			- increaseBook:   增加书目，要求用户输入新添的书目的各项信息
			- editBook:       要求用户输入想修改的信息的书名。同样支持模糊查找，若没
							  有找到，发出错误提示，找到了，若不止一个结果，会让用户
							  在所有结果中做出选择
			- deleteBook:     要求用户输入想要删除的书的书名。该函数首先在书库中查
							  找，如果没有找到对应的书，那么给出一个提示信息；如果
							  找到了该书，那么就从BookData对象数组中移去此书，并且
							  在删除书目前，程序会提示信息，使用户确认删除操作
			- changeRate:	  修改税率
			- checkIsbn:      检查输入的isbn号是否符合格式要求
			- checkDateAdded: 检查输入的入库时间是否符合格式要求
			- isLY:           判断是否为闰年
			- checkDate:      判断输入的三个代表年月日的整数组成的日期是否符合规范
		*/
	static char* strUpper(char*);
	static void lookUpBook(list<BookData>&);
	static void increaseBook(list<BookData>&);
	static void editBook(list<BookData>&);
	static void deleteBook(list<BookData>&);
	static void changeTaxRate();
	static bool checkIsbn(char*);
	static bool checkDateAdded(char*);
	static bool isLY(int);
	static bool checkDate(int, int, int);
};

class Reception
{
	/*
		几个收银函数:
		- displayTime:			展示一行时间信息，参数代表展示方式
		- displayShoppingCart:  展示账单详情，即用户已经放入购物车中放入，返回列表的宽度
		- displayBill:			展示账单详情
	*/
public:
	static void displayTime(int style = 1);
	static int displayShoppingCart(list<Sale>&);
	static void displayBill(list<Sale>&);
};





/*(在Global Function中定义了以下函数)
	下面申明几个全局函数:
	- strToNum(char*):     将代表数字的char*字符串转化为10进制整数
	- strToNum(string):    将代表数字的string字符串转化为10进制整数
	- numToStr(int):	   将int型数字转化成string字符串
	- numToStr(double):	   将double型数字转化成string字符串

	- split(const string &, vector<string> &, const char flag = ' '):

						   与python中的split功能一致，将第一个参数string
						   按照flag的内容进行切割，结果储存在第二个参数引用
						   的string向量中
	- strip(string&, const char):

						   与python中的strip函数功能一致，将送入的字符串的首
						   尾去掉给定的字符，如果送入的字符串的首尾字符与所需
						   删除的字符均不相同，则不作改动
	- centerPrint(string &, int, char flag = ' '):
						   居中输出第一个参数，第二个参数表示输出域宽，多余部
						   分会被flag代替,flag默认为空格。如果域宽小于string的
						   长度，则会不加修饰地输出string
	- getXY(int*, int*):   接受指针或int地址，获取光标在屏幕上的坐标(x,y)
	- goToXY(int, int):    使光标移动到指定坐标上去


*/

int strToNum(char*);
int strToNum(string);
string numToStr(int);
string numToStr(double);
void split(const string&, vector<string>&, const char);
void strip(string&, const char);
void centerPrint(string&, int, char flag = ' ');
void centerPrint(char*, int, char flag = ' ');
void getXY(int*, int*);
void goToXY(int, int);

/*(在Module中定义了以下的模块函数)
	模块：最终放入主函数的模块，共有主菜单、收银模块、管理模块和报表模块
		  参数均为需要操作或访问的代表整个书库的BookData类型对的list
	有时间，想要增加重置模块，以防操作不当，可以重置整个书库
*/

int mainMenuModule(list<BookData>&);					// 主菜单模块，返回用户选项，
void receptionModule(list<BookData>&);					// 收银模块
void managementModule(list<BookData>&);				// 书库管理模块
void reportModule(list<BookData>&);					// 报表模块
void resetModule(list<BookData>&, list<BookData>&);		// 重置模块

#endif // !TEST_H


