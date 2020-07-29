#include "library.h"

/*
	几个收银函数:
	- displayTime:			展示一行时间信息，参数style代表展示方式，默认值为1
							style == 1:直接按照ctime返回的格式打印，如Mon Apr 13 10:51:17 2020
							style == 2:只打印年月日，如:2018年12月26日
							style == 3:电脑时间格式，如2020/4/13 11:04
	- displayShoppingCart:  展示购物车详情，即用户已经放入购物车中放入的书目
	- displayBill:			展示账单详情
*/

void Reception::displayTime(int style)
{
	// 先通过ctime可读的当地时间的字符串
	time_t t;					// time_t对象储存了当地时间
	time(&t);					// time()函数接受time_t对象的指针，将系统时间戳给time_t对象
	string curTime(ctime(&t));  // ctime()函数根据接收的系统时间戳计算出当地时间，返回char*
	strip(curTime, '\n');		// 通过strip操作去掉curTime结尾的\n

	// 按照‘ ’split以获得的string对象以获得各个代表时间和日期的参数
	vector<string> date;
	split(curTime, date, ' ');
	string MONTH("JanFebMarAprMayJunJulAugSepOctNovDec");

	// 按照输入的格式类型打印时间
	if (style == 1) {
		cout << "日期:" << curTime << endl;
	}
	if (style == 2) {
		cout << "日期:" << date[4] << "年";
		cout << (MONTH.find(date[1]) / 3 + 1) << "月";
		cout << date[2] << "日" << endl;
	}
	if (style == 3) {
		cout << "日期:" << date[4] << "/";
		cout << (MONTH.find(date[1]) / 3 + 1) << "/";
		cout << date[2] << " " << date[3] << endl;
	}
}

int Reception::displayShoppingCart(list<Sale>& shoppingCart)
{
	// 根据购物车中的情况获得各个字符串型数据项需要的最大输出域宽
	int maxISBNLength = 8, maxBookNameLength = 8;
	for (list<Sale>::iterator it = shoppingCart.begin(); it != shoppingCart.end(); ++it) {
		maxISBNLength = max(maxISBNLength, strlen((*it).getIsbn()));
		maxBookNameLength = max(maxBookNameLength, strlen((*it).getBookTitle()));
	}
	// 打印上方列表信息
	centerPrint((char*)"数量", 3); cout << " | ";
	centerPrint((char*)"ISBN号", maxISBNLength); cout << " | ";
	centerPrint((char*)"书名", maxBookNameLength); cout << " | ";
	centerPrint((char*)"单价", 10); cout << " | ";
	centerPrint((char*)"金额", 10); cout << " | ";
	cout << endl;

	string num;
	int listLength;				// 打印的表格的宽度，作为函数的返回值
	// 循环打印具体信息
	for (list<Sale>::iterator it = shoppingCart.begin(); it != shoppingCart.end(); ++it) {
		num = numToStr((*it).getNum());
		centerPrint(num, 3); cout << "  | ";
		centerPrint((*it).getIsbn(), maxISBNLength); cout << " | ";
		centerPrint((*it).getBookTitle(), maxBookNameLength); cout << " | ";

		cout << "RMB"; printf("%6.2lf", (*it).getRetail()); cout << "  | ";
		cout << "RMB"; printf("%6.2lf", (*it).calcuSaleMoney()); cout << "  | ";
		cout << endl;
	}
	listLength = 3 + 4 + maxISBNLength + 3 + maxBookNameLength + 3 + 6 + 4 + 6 + 4 + 6;
	return listLength;
}

void Reception::displayBill(list<Sale>& shoppingCart)
{
	double totalSale = 0;
	for (list<Sale>::iterator it = shoppingCart.begin(); it != shoppingCart.end(); ++it) {
		totalSale += (*it).calcuSaleMoney();
	}
	double totalTax = totalSale * Sale::taxRate;
	double totalMoney = totalSale + totalTax;
	cout << "销售总额： RMB ";
	printf("%6.2lf\n", totalSale);
	cout << "零售税：   RMB ";
	printf("%6.2lf\n", totalTax);
	cout << "应付总额： RMB ";
	printf("%6.2lf\n", totalMoney);
}