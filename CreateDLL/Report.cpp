#include "library.h"

/*几个报表函数
	- listAll:        列出书库中所有图书的完整信息
	- listByWholesale:列出书库中所有图书的批发价、库存量以及批发价总额
	- listByRetail:   列出书库中所有图书的零售价、库存量以及零售价总额
	- listByNum:	  先对图书按库存量从大到小排序，然后列出所有图书的库存量
	- listByValue:    先对图书按每种批发价总额从大到小进行排序，然后列出书库
					  中所有图书的批发价、库存量以及批发价总额
	- listByDate:     先对图书按照入库日期排序，然后列出书库中所有图书的日期，
					  通过第三个参数可以决定是从旧书到新书排序还是从新书到旧
					  书排序，默认从新书到旧书
	- cmp:			  排序的排序逻辑
	- reverse:		  这是一个参数，凡是与排序有关的函数都有这个参数。这个参数
					  的默认值为false，代表按照升序排序（从小到大）；若置为true
					  ，则为降序排序（从大到小）
*/


void Report::listAll(list<BookData>& books)
{
	cout << "书库中共有" << books.size() << "本书:" << endl;
	int haveShow = 0;
	int x, y;
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		haveShow++;
		cout << endl << " book " << haveShow << " :\n" << endl;
		(*it).displayInfo();

		// 由于屏幕原因，每次只能显示两本书的信息
		if (haveShow % 2 == 0) {
			cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
			getchar();
			getXY(&x, &y);			// 获取光标坐标
			goToXY(x, y - 1);		// 退回到上一行
			cout << setw(50) << " ";// 清空提示信息
			goToXY(x, y - 2);       // 再退到上一行
		}
	}
	cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
}

void Report::listByWholesale(list<BookData>& books)
{
	cout << "书库中共有" << books.size() << "本书:" << endl;
	int haveShow = 0;
	int x, y;
	int width1 = 5, width2 = 7;

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		haveShow++;
		cout << " book " << haveShow << " :\n" << endl;
		cout << setw(width1) << " " << setw(width2) << "ISBN  号" << ": " << (*it).getIsbn() << endl;
		cout << setw(width1) << " " << setw(width2) << "书    名" << ": " << (*it).getBookTitle() << endl;
		cout << setw(width1) << " " << setw(width2) << "批 发 价" << ": " << (*it).getWholesale() << "元/本" << endl;
		cout << setw(width1) << " " << setw(width2) << "库 存 量" << ": " << (*it).getQtyOnHand() << endl;
		cout << setw(width1) << " " << setw(width2) << "批发价总额" << ": ";
		printf("%.2lf元\n\n", (double)(*it).getQtyOnHand() * (*it).getWholesale());


		// 由于屏幕原因，每次只能显示两本书的信息
		if (haveShow % 2 == 0) {
			cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
			getchar();
			getXY(&x, &y);			// 获取光标坐标
			goToXY(x, y - 1);		// 退回到上一行
			cout << setw(50) << " ";// 清空提示信息
			goToXY(x, y - 2);       // 再退到上一行
		}
	}
	cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
}

void Report::listByRetail(list<BookData>& books)
{
	cout << "书库中共有" << books.size() << "本书:" << endl;
	int haveShow = 0;
	int x, y;
	int width1 = 5, width2 = 7;

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		haveShow++;
		cout << " book " << haveShow << " :\n" << endl;
		cout << setw(width1) << " " << setw(width2) << "ISBN  号" << ": " << (*it).getIsbn() << endl;
		cout << setw(width1) << " " << setw(width2) << "书    名" << ": " << (*it).getBookTitle() << endl;
		cout << setw(width1) << " " << setw(width2) << "零 售 价" << ": " << (*it).getRetail() << "元/本" << endl;
		cout << setw(width1) << " " << setw(width2) << "库 存 量" << ": " << (*it).getQtyOnHand() << endl;
		cout << setw(width1) << " " << setw(width2) << "零售价总额" << ": ";
		printf("%.2lf元\n\n", (double)(*it).getQtyOnHand() * (*it).getRetail());


		// 由于屏幕原因，每次只能显示两本书的信息
		if (haveShow % 2 == 0) {
			cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
			getchar();
			getXY(&x, &y);			// 获取光标坐标
			goToXY(x, y - 1);		// 退回到上一行
			cout << setw(50) << " ";// 清空提示信息
			goToXY(x, y - 2);       // 再退到上一行
		}
	}
	cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
}

void Report::listByNum(list<BookData>& books, bool reverse)
{
	// 先按照库存量排序，然后按部就班
	int haveShow = 0;						// 记录已经展示的书的本数
	int width1 = 5, width2 = 7;				// 设置输出域宽
	int x, y;								// 存取光标坐标的变量

	// 排序，reverse置为false为默认的升序
	if (reverse == false)
		books.sort(cmpByNum1);
	else
		books.sort(cmpByNum2);

	// 输出整个链表
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		haveShow++;
		cout << endl << " book " << haveShow << " :\n" << endl;
		cout << setw(width1) << " " << setw(width2) << "ISBN  号" << ": " << (*it).getIsbn() << endl;
		cout << setw(width1) << " " << setw(width2) << "书    名" << ": " << (*it).getBookTitle() << endl;
		cout << setw(width1) << " " << setw(width2) << "库 存 量" << ": " << (*it).getQtyOnHand() << endl;

		// 由于屏幕原因，每次只能显示两本书的信息
		if (haveShow % 2 == 0) {
			cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
			getchar();
			getXY(&x, &y);			// 获取光标坐标
			goToXY(x, y - 1);		// 退回到上一行
			cout << setw(50) << " ";// 清空提示信息
			goToXY(x, y - 2);       // 再退到上一行
		}
	}
	cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
}

void Report::listByValue(list<BookData>& books, bool reverse)
{
	// 先按照库存量排序，然后按部就班
	int haveShow = 0;						// 记录已经展示的书的本数
	int width1 = 5, width2 = 7;				// 设置输出域宽
	int x, y;								// 存取光标坐标的变量

	// 排序，reverse置为false为默认的升序
	if (reverse == false)
		books.sort(cmpByValue1);
	else
		books.sort(cmpByValue2);

	// 输出整个链表
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		haveShow++;
		cout << " book " << haveShow << " :\n" << endl;
		cout << setw(width1) << " " << setw(width2) << "ISBN  号" << ": " << (*it).getIsbn() << endl;
		cout << setw(width1) << " " << setw(width2) << "书    名" << ": " << (*it).getBookTitle() << endl;
		cout << setw(width1) << " " << setw(width2) << "批 发 价" << ": " << (*it).getWholesale() << "元/本" << endl;
		cout << setw(width1) << " " << setw(width2) << "库 存 量" << ": " << (*it).getQtyOnHand() << endl;
		cout << setw(width1) << " " << setw(width2) << "批发价总额" << ": ";
		printf("%.2lf元\n\n", (double)(*it).getQtyOnHand() * (*it).getWholesale());


		// 由于屏幕原因，每次只能显示两本书的信息
		if (haveShow % 2 == 0) {
			cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
			getchar();
			getXY(&x, &y);			// 获取光标坐标
			goToXY(x, y - 1);		// 退回到上一行
			cout << setw(50) << " ";// 清空提示信息
			goToXY(x, y - 2);       // 再退到上一行
		}
	}
	cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
}

void Report::listByDate(list<BookData>& books, bool reverse)
{
	// 先按照库存量排序，然后按部就班
	int haveShow = 0;						// 记录已经展示的书的本数
	int width1 = 5, width2 = 7;				// 设置输出域宽
	int x, y;								// 存取光标坐标的变量

	// 排序
	if (reverse == false)
		books.sort(cmpByDate1);
	else
		books.sort(cmpByDate2);

	// 输出整个链表
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		haveShow++;
		cout << " book " << haveShow << " :\n" << endl;
		cout << setw(width1) << " " << setw(width2) << "ISBN  号" << ": " << (*it).getIsbn() << endl;
		cout << setw(width1) << " " << setw(width2) << "书    名" << ": " << (*it).getBookTitle() << endl;
		cout << setw(width1) << " " << setw(width2) << "进书日期" << ": " << (*it).getDateAdded() << endl << endl;

		// 由于屏幕原因，每次只能显示两本书的信息
		if (haveShow % 2 == 0) {
			cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
			getchar();
			getXY(&x, &y);			// 获取光标坐标
			goToXY(x, y - 1);		// 退回到上一行
			cout << setw(50) << " ";// 清空提示信息
			goToXY(x, y - 2);       // 再退到上一行
		}
	}
	cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
}

// 升序
bool Report::cmpByNum1(BookData& book1, BookData& book2)
{
	return (book1.getQtyOnHand() < book2.getQtyOnHand());

	return (book1.getQtyOnHand() > book2.getQtyOnHand());
}

// 降序
bool Report::cmpByNum2(BookData& book1, BookData& book2)
{
	return (book1.getQtyOnHand() > book2.getQtyOnHand());
}

// 升序
bool Report::cmpByValue1(BookData& book1, BookData& book2)
{
	return (((int)book1.getQtyOnHand() * book1.getWholesale()) < ((int)book2.getQtyOnHand() * book2.getWholesale()));
}

// 降序
bool Report::cmpByValue2(BookData& book1, BookData& book2)
{
	return (((int)book1.getQtyOnHand() * book1.getWholesale()) > ((int)book2.getQtyOnHand() * book2.getWholesale()));
}

// 升序(从旧书到新书)
bool Report::cmpByDate1(BookData& book1, BookData& book2)
{
	// 先按“-”分割年月日
	vector<string> date1, date2;
	split(book1.getDateAdded(), date1, '-');
	split(book2.getDateAdded(), date2, '-');

	// 多级比较，优先级为: 年 > 月 > 日
	if (strToNum(date1[0]) != strToNum(date2[0]))
		return (strToNum(date1[0]) < strToNum(date2[0]));
	if (strToNum(date1[1]) != strToNum(date2[1]))
		return (strToNum(date1[1]) < strToNum(date2[1]));
	return (strToNum(date1[2]) < strToNum(date2[2]));
}

// 降序(从新书到旧书)
bool Report::cmpByDate2(BookData& book1, BookData& book2)
{
	// 先按“-”分割年月日
	vector<string> date1, date2;
	split(book1.getDateAdded(), date1, '-');
	split(book2.getDateAdded(), date2, '-');

	// 多级比较，优先级为: 年 > 月 > 日
	if (strToNum(date1[0]) != strToNum(date2[0]))
		return (strToNum(date1[0]) > strToNum(date2[0]));
	if (strToNum(date1[1]) != strToNum(date2[1]))
		return (strToNum(date1[1]) > strToNum(date2[1]));
	return (strToNum(date1[2]) > strToNum(date2[2]));
}
