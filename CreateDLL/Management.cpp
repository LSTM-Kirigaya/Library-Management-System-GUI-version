#include "library.h"
#define _CRT_SECURE_NO_WARNINGS

/* 几个管理的函数:
		- strUpper:       将参数指针指向的字符串中小写字母转换成大写字母
		- lookUpBook:     要求用户输入书名，然后在书库中查找该书，如果没找到
						  在屏幕上显示一个出错信息，以表明书库中没有该书。如
						  果找到了该书，调用BookInfo中的函数，显示该书的信息，
						  支持模糊查找，会匹配列出书名，作者，出版社含有关键字
						  的所有匹配结果
		- increaseBook:   增加书目，要求用户输入新添的书目的各项信息，输入的信息
						  有输入有效性检查
		- editBook:       要求用户输入想修改的信息的书名。同样支持模糊查找，若没
						  有找到，发出错误提示，找到了，若不止一个结果，会让用户
						  在所有结果中做出选择。找到书后会让用户选择检索结果中的
						  一个结果进行修改，并且会询问用户是否选择修改全部信息，
						  每一步判断都有输入有效性检查
		- deleteBook:     要求用户输入想要删除的书的书名。该函数首先在书库中查
						  找，如果没有找到对应的书，那么给出一个提示信息；如果
						  找到了该书，那么就从BookData对象数组中移去此书，并且
						  在删除书目前，程序会提示信息，使用户确认删除操作
		- checkIsbn:      检查输入的isbn号是否符合格式要求
		- checkDateAdded: 检查输入的入库时间是否符合格式要求
		- isLY:           判断是否为闰年
		- checkDate:      判断输入的三个代表年月日的整数组成的日期是否符合规范
*/
/*
		希望改进的地方:
		1.所有展示书目信息的函数若刚好展示完所有书目，则不触发“按回车以显示更多”的提示信息（比如75行处）
		2.editBook函数中可以不需要展示完所有书就让用户修改已经展示的书的信息
*/

char* Management::strUpper(char* str)
{
	char* p;
	p = new char[strlen(str) + 1];
	strcpy(p, str);
	for (unsigned int i = 0; i < strlen(str); ++i)
		*(p + i) = toupper(*(p + i));
	return p;
}

void Management::lookUpBook(list<BookData>& books)
{
	string bookName;				// 用户输入的书名
	int bookNum = 0;				// 存储总共查找到的书目的种数
	list<BookData> tempDict;		// 暂时存储可能符合要求的BookData对象

	cout << "需要查找的书名: ";
	cin >> bookName; cin.ignore();

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		/*
			- validFind表示迭代器指针指向的当前对象中是不是符合查找要求
			- 先用当前对象对的成员数组初始化string对象，方便查找子串
			- 因为查找方式为模糊查找，所以只要bookName是以下三个string对象
			  中的一个string的子串就认定为符合要求
		*/
		string currBookName((*it).getBookTitle()),
			currAuthor((*it).getAuthor()),
			currPulisher((*it).getPublisher());
		bool validFind = (currBookName.find(bookName) + 1)
			|| (currAuthor.find(bookName) + 1)
			|| (currPulisher.find(bookName) + 1);
		if (validFind) {
			bookNum++;
			tempDict.push_back(BookData(*it));
		}
	}

	if (bookNum == 0) cout << "(。·＿·。)/I’m sorry~ 并没有找到相关书籍......" << endl;
	else {
		cout << "共为您找到" << bookNum << "个含有关键词\"" << bookName << "\"的结果(' · ω ·')" << endl;
		int haveShow = 0;							// 已经显示的结果数目
		int x, y;									// 当前光标在屏幕上的坐标

		for (list<BookData>::iterator it = tempDict.begin(); it != tempDict.end(); ++it) {
			haveShow++;
			cout << endl << " book " << haveShow << " :\n" << endl;
			(*it).displayInfo();


			// 每显示2个结果暂停一下
			if (haveShow % 2 == 0 && (it != tempDict.end())) {
				cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
				getchar();							// 接受用户输入的回车键
				getXY(&x, &y);						// 获取光标坐标
				goToXY(x, y - 1);					// 退到上一行
				cout << setw(50) << " ";			// 清空提示信息
				goToXY(x, y - 2);					// 回退到上一行
			}
		}
		cout << endl << "(。·＿·。)/已经到底啦~~" << endl << endl;
	}
}

void Management::increaseBook(list<BookData>& books)
{
	cout << "请输入新增书目的基本信息:" << endl << endl;
	char isbn[14];
	char bookTitle[51];
	char author[31];
	char publisher[31];
	char dateAdded[11];
	int qtyOnHand;
	double wholesale;
	double retail;

	int width1 = 3, width2 = 10;

	do {
		cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "ISBN  号" << ": ";
		cin.getline(isbn, 14);
		if (!checkIsbn(isbn)) cout << "您输入的isbn号码不符合格式,请重新输入:(格式为数字与\'-\'的组合,其中\'-\'不能出现在开头或结尾且任意两个\'-\'不能相连)" << endl;
	} while (!checkIsbn(isbn));

	cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "书    名" << ": ";
	cin.getline(bookTitle, 51);

	cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "作 者 名" << ": ";
	cin.getline(author, 31);

	cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "出 版 社" << ": ";
	cin.getline(publisher, 31);

	do {
		cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "进书日期" << ": ";
		cin >> dateAdded; cin.ignore();
		if (!checkDateAdded(dateAdded)) cout << "您输入的时间格式有误,请重新输入:(格式为X-X-X,代表年-月-日,比如2017-1-1)" << endl;
	} while (!checkDateAdded(dateAdded));

	do {
		cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "库 存 量" << ": ";
		cin >> qtyOnHand; cin.ignore();
		if ((qtyOnHand < 0) || (fabs(qtyOnHand - (int)qtyOnHand) > 1e-6)) cout << "您输入的库存量有误,请重新输入:(请确保输入的是非负整数)" << endl;
	} while ((qtyOnHand < 0) || (fabs(qtyOnHand - (int)qtyOnHand) > 1e-6));

	do {
		cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "批 发 价(元/本)" << ": ";
		cin >> wholesale; cin.ignore();
		if (wholesale <= 0) cout << "您输入的批发价有误,请重新输入:(请确保输入的是正实数)" << endl;
	} while (wholesale <= 0);

	do {
		cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "零 售 价(元/本)" << ": ";
		cin >> retail; cin.ignore();
		if (retail <= 0) cout << "您输入的批发价有误,请重新输入:(请确保输入的是正实数)" << endl;
	} while (retail <= 0);

	books.push_back(BookData(isbn, bookTitle, author, publisher, dateAdded, qtyOnHand, wholesale, retail));

	cout << endl << "增添成功!(' · ω ·')" << endl << endl;
}

void Management::editBook(list<BookData>& books)
{
	string bookName;				// 用户输入的书名
	int bookNum = 0;				// 存储总共查找到的书目的种数
	list<BookData> tempDict;		// 暂时存储可能符合要求的BookData对象

	cout << "请输入需要修改信息的书名:";
	cin >> bookName; cin.ignore();

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {

		// 此处和lookUpBook函数中的匹配逻辑完全一致
		string currBookName((*it).getBookTitle()),
			currAuthor((*it).getAuthor()),
			currPulisher((*it).getPublisher());
		bool validFind = (currBookName.find(bookName) + 1)
			|| (currAuthor.find(bookName) + 1)
			|| (currPulisher.find(bookName) + 1);
		if (validFind) {
			bookNum++;
			tempDict.push_back(BookData(*it));
		}
	}
	if (bookNum == 0) {
		cout << "(。·＿·。)/I’m sorry~ 并没有找到相关书籍......" << endl;
		return;
	}

	else {
		cout << "共为您找到" << bookNum << "个含有关键词\"" << bookName << "\"的结果(' · ω ·')" << endl;
		int haveShow = 0;							// 已经显示的结果数目
		int x, y;									// 当前光标在屏幕上的坐标
		int id = 0;									// 用户选择的要修改的书的编号
		char decision[10], * p;

		for (list<BookData>::iterator it = tempDict.begin(); it != tempDict.end(); ++it) {
			haveShow++;
			cout << endl << " book " << haveShow << " :\n" << endl;
			(*it).displayInfo();


			// 每显示2个结果暂停一下
			if (haveShow % 2 == 0 && (it != tempDict.end())) {
				cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
				getchar();							// 接受用户输入的回车键
				getXY(&x, &y);						// 获取光标坐标
				goToXY(x, y - 1);					// 退到上一行
				cout << setw(50) << " ";			// 清空提示信息
				goToXY(x, y - 2);					// 回退到上一行
			}
		}
		cout << endl << " " << endl;
		bool flag = true;							// 用来判断用户是否输入了异常信息

		// 要求用户输入需要修改的序号，带有输入有效性检查
		do {
			cout << "您想要修改哪本书的信息呢？（输入‘book’字样后的序号）:";
			cin >> id; cin.ignore();
			if (id >= 1 && id <= bookNum && (fabs(id - (int)id) <= 1e-6)) flag = true;
			else {
				cout << "请您输入有效的序号!" << endl;
				flag = false;
			}
		} while (!flag);

		/*
			- 根据id在tempDict链表中找到对象的isbn号，再根据isbn号在原来的总的链表中找到那个数据项
			- 令迭代器editPoint指针指向原链表中需要修改的节点
		*/
		int i = 1;									// 用来匹配id的临时变量，记得++
		char editIsbn[20];							// 用来储存需要修改的BookData对象的isbn号
		list<BookData>::iterator editPoint;			// 用来指向引用中需要修改的BookData对象的迭代器指针

		for (list<BookData>::iterator it = tempDict.begin(); it != tempDict.end(); ++it) {
			if (id == i) strcpy(editIsbn, (*it).getIsbn());
			i++;
		}
		for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
			if (strcmp(editIsbn, (*it).getIsbn()) == 0) editPoint = it;
		}

		// 询问用户是否修改全部数据项，会检查有效性
		do {

			cout << "您是否选择修改该本书的全部数据项？（输入Yes或No，不区分大小写）:";
			cin >> decision; cin.ignore();
			system("cls");
			p = strUpper(decision);
			if (strcmp(p, "YES") == 0 || strcmp(p, "NO") == 0) flag = true;
			else {
				cout << "请输入Yes或者No！您貌似输入的不是这两个之间的任何一个(' · ω ·')" << endl;
				flag = false;
			}
		} while (!flag);

		// 用来临时储存用户对某一项或几项数据的修改值
		char isbn[14];
		char bookTitle[51];
		char author[31];
		char publisher[31];
		char dateAdded[11];
		int qtyOnHand;
		double wholesale;
		double retail;
		char ch;

		int width1 = 3, width2 = 10;

		// 根据用户的选择进入不同的修改方式
		// YES代表修改全部数据项，此处带有输入检测，此处的输入检测与increaseBook中的检测逻辑一致
		if (strcmp(p, "YES") == 0) {
			do {
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "ISBN  号" << ": ";
				cin >> isbn; cin.ignore();
				if (!checkIsbn(isbn)) cout << "您输入的isbn号码不符合格式,请重新输入:(格式为数字与\'-\'的组合,其中\'-\'不能出现在开头或结尾且任意两个\'-\'不能相连)" << endl;
			} while (!checkIsbn(isbn));
			cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "书    名" << ": ";
			cin >> bookTitle; cin.ignore();
			cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "作 者 名" << ": ";
			cin >> author; cin.ignore();
			cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "出 版 社" << ": ";
			cin >> publisher; cin.ignore();
			do {
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "进书日期" << ": ";
				cin >> dateAdded; cin.ignore();
				if (!checkDateAdded(dateAdded)) cout << "您输入的时间格式有误,请重新输入:(格式为X-X-X,代表年-月-日,比如2017-1-1)" << endl;
			} while (!checkDateAdded(dateAdded));
			do {
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "库 存 量" << ": "; cin >> qtyOnHand;
				cin.ignore();
				if ((qtyOnHand < 0) || (fabs(qtyOnHand - (int)qtyOnHand) > 1e-6)) cout << "您输入的库存量有误,请重新输入:(请确保输入的是非负整数)" << endl;
			} while ((qtyOnHand < 0) || (fabs(qtyOnHand - (int)qtyOnHand) > 1e-6));
			do {
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "批 发 价(元/本)" << ": "; cin >> wholesale;
				cin.ignore();
				if (wholesale <= 0) cout << "您输入的批发价有误,请重新输入:(请确保输入的是正实数)" << endl;
			} while (wholesale <= 0);
			do {
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "零 售 价(元/本)" << ": "; cin >> retail;
				cin.ignore();
				if (retail <= 0) cout << "您输入的批发价有误,请重新输入:(请确保输入的是正实数)" << endl;
			} while (retail <= 0);

			cout << "确认要修改吗？（y/n）";
			ch = getchar();
			if (ch == 'n') return;
			(*editPoint).setIsbn(isbn);
			(*editPoint).setAuthor(author);
			(*editPoint).setBookTitle(bookTitle);
			(*editPoint).setPublisher(publisher);
			(*editPoint).setDateAdded(dateAdded);
			(*editPoint).setQtyOnHand(qtyOnHand);
			(*editPoint).setWholesale(wholesale);
			(*editPoint).setRetail(retail);
			cout << endl << "修改成功!(' · ω ·')" << endl << endl;
		}

		// NO代表修改部分数据项
		else if (strcmp(p, "NO") == 0) {
			bool flag = true;				//检测输入有效性的flag
			int editId;
			width1 = 5;
			width2 = 7;

			// 获取编号，有提示板与输入检查
			do {
				cout << setw(width1) << " 1 : " << setw(width2) << "ISBN  号" << endl;
				cout << setw(width1) << " 2 : " << setw(width2) << "书    名" << endl;
				cout << setw(width1) << " 3 : " << setw(width2) << "作 者 名" << endl;
				cout << setw(width1) << " 4 : " << setw(width2) << "出 版 社" << endl;
				cout << setw(width1) << " 5 : " << setw(width2) << "进书日期" << endl;
				cout << setw(width1) << " 6 : " << setw(width2) << "库 存 量" << endl;
				cout << setw(width1) << " 7 : " << setw(width2) << "批 发 价" << endl;
				cout << setw(width1) << " 8 : " << setw(width2) << "零 售 价" << endl;
				cout << "请输入需要修改的数据项之前的编号（1-8）：";
				cin >> editId; cin.ignore();
				if (editId >= 1 && editId <= 8 && (fabs(editId - (int)editId) <= 1e-8)) flag = true;
				else {
					cout << "请输入有效的编号！" << endl;
					flag = false;
				}
			} while (!flag);

			switch (editId)
			{
			case 1:
				do {
					cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "ISBN  号" << ": ";
					cin >> isbn; cin.ignore();
					if (!checkIsbn(isbn)) cout << "您输入的isbn号码不符合格式,请重新输入:(格式为数字与\'-\'的组合,其中\'-\'不能出现在开头或结尾且任意两个\'-\'不能相连)" << endl;
				} while (!checkIsbn(isbn));
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setIsbn(isbn);
				break;

			case 2:
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "书    名" << ": ";
				cin >> bookTitle; cin.ignore();
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setBookTitle(bookTitle);
				break;

			case 3:
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "作 者 名" << ": ";
				cin >> author; cin.ignore();
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setAuthor(author);
				break;

			case 4:
				cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "出 版 社" << ": ";
				cin >> publisher; cin.ignore();
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setPublisher(publisher);
				break;

			case 5:
				do {
					cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "进书日期" << ": ";
					cin >> dateAdded; cin.ignore();
					if (!checkDateAdded(dateAdded)) cout << "您输入的时间格式有误,请重新输入:(格式为X-X-X,代表年-月-日,比如2017-1-1)" << endl;
				} while (!checkDateAdded(dateAdded));
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setDateAdded(dateAdded);
				break;

			case 6:
				do {
					cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "库 存 量" << ": "; cin >> qtyOnHand;
					cin.ignore();
					if ((qtyOnHand < 0) || (fabs(qtyOnHand - (int)qtyOnHand) > 1e-6)) cout << "您输入的库存量有误,请重新输入:(请确保输入的是非负整数)" << endl;
				} while ((qtyOnHand < 0) || (fabs(qtyOnHand - (int)qtyOnHand) > 1e-6));
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setQtyOnHand(qtyOnHand);
				break;

			case 7:
				do {
					cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "批 发 价(元/本)" << ": "; cin >> wholesale;
					cin.ignore();
					if (wholesale <= 0) cout << "您输入的批发价有误,请重新输入:(请确保输入的是正实数)" << endl;
				} while (wholesale <= 0);
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setWholesale(wholesale);
				break;

			case 8:
				do {
					cout << setw(width1) << setiosflags(ios::left) << "->" << setw(width2) << setiosflags(ios::left) << "零 售 价(元/本)" << ": "; cin >> retail;
					cin.ignore();
					if (retail <= 0) cout << "您输入的批发价有误,请重新输入:(请确保输入的是正实数)" << endl;
				} while (retail <= 0);
				cout << "确认要修改吗？（y/n）";
				ch = getchar();
				if (ch == 'n') return;
				(*editPoint).setRetail(retail);
				break;
			}
			cout << endl << "修改成功!(' · ω ·')" << endl << endl;
		}

	}

}

void Management::deleteBook(list<BookData>& books)
{
	string bookName;
	int bookNum = 0;
	list<BookData> tempDict;

	cout << "请输入需要删去的书的书名:";
	cin >> bookName; cin.ignore();

	// 在原链表中开始模糊匹配，匹配逻辑和lookUpBook中一致
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {

		string currBookName((*it).getBookTitle()),
			currAuthor((*it).getAuthor()),
			currPulisher((*it).getPublisher());
		bool validFind = (currBookName.find(bookName) + 1)
			|| (currAuthor.find(bookName) + 1)
			|| (currPulisher.find(bookName) + 1);
		if (validFind) {
			bookNum++;
			tempDict.push_back(BookData(*it));
		}
	}
	if (bookNum == 0) {
		cout << "(。·＿·。)/I’m sorry~ 并没有找到相关书籍......" << endl;
		return;
	}
	else {
		cout << "共为您找到" << bookNum << "个含有关键词\"" << bookName << "\"的结果(' · ω ·')" << endl;
		int haveShow = 0;							// 已经显示的结果数目
		int x, y;									// 当前光标在屏幕上的坐标
		int id = 0;									// 用户选择的要修改的书的编号

		for (list<BookData>::iterator it = tempDict.begin(); it != tempDict.end(); ++it) {
			haveShow++;
			cout << endl << " book " << haveShow << " :\n" << endl;
			(*it).displayInfo();


			// 每显示2个结果暂停一下
			if (haveShow % 2 == 0 && (it != tempDict.end())) {
				cout << endl << "已为您展示" << haveShow << "个搜寻结果,按回车键以显示更多结果...";
				getchar();							// 接受用户输入的回车键
				getXY(&x, &y);						// 获取光标坐标
				goToXY(x, y - 1);					// 退到上一行
				cout << setw(50) << " ";			// 清空提示信息
				goToXY(x, y - 2);					// 回退到上一行
			}
		}
		cout << endl << " " << endl;
		bool flag = true;							// 用来判断用户是否输入了异常信息

		// 要求用户输入需要修改的序号，带有输入有效性检查
		do {
			cout << "您想要删去哪本书呢？（输入‘book’字样后的序号）:";
			cin >> id; cin.ignore();
			if (id >= 1 && id <= bookNum && (fabs(id - (int)id) <= 1e-6)) flag = true;
			else {
				cout << "请您输入有效的序号!" << endl;
				flag = false;
			}
		} while (!flag);

		// 此处给迭代器指针赋值的逻辑与editBook中完全相同
		int i = 1;									// 用来匹配id的临时变量，记得++
		char deleteIsbn[20];						// 用来储存需要删去的BookData对象的isbn号
		list<BookData>::iterator deletePoint;		// 用来指向引用中需要删去的BookData对象的迭代器指针

		for (list<BookData>::iterator it = tempDict.begin(); it != tempDict.end(); ++it) {
			if (id == i) strcpy(deleteIsbn, (*it).getIsbn());
			i++;
		}
		for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
			if (strcmp(deleteIsbn, (*it).getIsbn()) == 0) deletePoint = it;
		}
		cout << "您确定要删去这一本书吗？（y/n）";
		char ch = getchar();
		if (ch == 'n') return;
		books.erase(deletePoint);
		cout << endl << "删除成功!(' · ω ·')" << endl << endl;
	}
}

void Management::changeTaxRate()
{
	double rate;
	bool flag = true;
	do {
		cout << "请输入新的税率值（0-1）:";
		cin >> rate; cin.ignore();
		if (rate > 0 && rate < 1) flag = true;
		else {
			cout << "请输入有效的税率！" << endl;
			flag = false;
		}
	} while (!flag);
	Sale::taxRate = rate;
	cout << endl << "税率修改成功!(' · ω ·')" << endl << endl;
}

bool Management::checkIsbn(char* isbn)
{
	bool flag = true;
	for (unsigned int i = 0; i < strlen(isbn); i++) {
		// isbn码中只有数字和-,若有不是数字或-的，flag置为false
		if (!((*(isbn + i) >= '0' && *(isbn + i) <= '9') || (*(isbn + i) == '-'))) {
			flag = false;
			//cout << "您输入的isbn码中出现了数字和\'-\'之外的符号" << endl;
		}
		// isbn中任意两个-不能连在一起
		if (i != strlen(isbn) - 1) {
			if (*(isbn + i) == '-' && *(isbn + i + 1) == '-') {
				flag = false;
				//cout << "您输入的isbn码中出现了连在一起的\'-\'" << endl;
			}

		}
	}
	// -不能出现在开头或结尾
	if (*isbn == '-') flag = false;
	if (*(isbn + strlen(isbn) - 1) == '-') flag = false;
	return flag;
}

bool Management::checkDateAdded(char* dateAdded)
{
	bool flag = true;
	int _num = 0;		//代表-的个数
	for (unsigned int i = 0; i < strlen(dateAdded); ++i) {
		if (*(dateAdded + i) == '-') _num++;
	}
	// -必须是两个，才能分割出年月日
	if (_num != 2) return false;

	// 分割年月日
	vector<string> date;
	split(string(dateAdded), date, '-');

	// 检查有效性
	flag = checkDate(strToNum(date[0]), strToNum(date[1]), strToNum(date[2]));

	return flag;
}

bool Management::isLY(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) return true;
	else return false;
}

bool Management::checkDate(int year = 2020, int month = 1, int day = 1)
{
	bool flag = true;
	int calender[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	// 日期不能小于0
	if (year <= 0 || month <= 0 || day <= 0) flag = false;
	// 将润年的二月份单独拿出来考察
	if (isLY(year) && month == 2)
		flag = (day <= 29) ? true : false;
	else
		flag = (day <= calender[month - 1]) ? true : false;
	return flag;
}

