#include "library.h"

class demo
{
private:
	int a, b;
public:
	void setA(int);
	void setB(int);
	int getMax();
};

void demo::setA(int a)
{
	this->a = a;
}

void demo::setB(int b)
{
	this->b = b;
}

int demo::getMax()
{
	return (a > b) ? a : b;
}



EXP int Add(int a, int b)
{
	return a + b;
}

EXP char* getA(const char* s)
{
	string a = "apple";
	char b[100];
	strcpy_s(b, a.c_str());
	return b;
}

EXP int getMax()
{
	demo D;
	D.setA(10);
	D.setB(20);
	return D.getMax();
}

// Report
EXP char* getSort1String()	// list all
{
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
		return 0;
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	string data, return_string = "";
	string space = " ";
	char charString[10000];
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		data = (string)(*it).getIsbn() + space +
			(string)(*it).getBookTitle() + space +
			(string)(*it).getAuthor() + space +
			(string)(*it).getPublisher() + space +
			(string)(*it).getDateAdded() + space +
			numToStr((*it).getQtyOnHand()) + space +
			numToStr((*it).getWholesale()) + space +
			numToStr((*it).getRetail()) + space;
		return_string += data;
	}
	strcpy_s(charString, return_string.c_str());
	return charString;
}

EXP char* getSort2String(bool reverse = false)	// list by  number
{
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
		return 0;
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// sort first
	if (reverse == false)
		books.sort(Report::cmpByNum1);
	else
		books.sort(Report::cmpByNum2);

	// regular operation then
	string data, return_string = "";
	string space = " ";
	char charString[10000];
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		data = (string)(*it).getIsbn() + space +
			(string)(*it).getBookTitle() + space +
			(string)(*it).getAuthor() + space +
			(string)(*it).getPublisher() + space +
			(string)(*it).getDateAdded() + space +
			numToStr((*it).getQtyOnHand()) + space +
			numToStr((*it).getWholesale()) + space +
			numToStr((*it).getRetail()) + space;
		return_string += data;
	}
	strcpy_s(charString, return_string.c_str());
	return charString;
}

EXP char* getSort3String(bool reverse = false)	// list by value
{
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
		return 0;
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// sort first
	if (reverse == false)
		books.sort(Report::cmpByValue1);
	else
		books.sort(Report::cmpByValue2);

	// regular operation then
	string data, return_string = "";
	string space = " ";
	char charString[10000];
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		data = (string)(*it).getIsbn() + space +
			(string)(*it).getBookTitle() + space +
			(string)(*it).getAuthor() + space +
			(string)(*it).getPublisher() + space +
			(string)(*it).getDateAdded() + space +
			numToStr((*it).getQtyOnHand()) + space +
			numToStr((*it).getWholesale()) + space +
			numToStr((*it).getRetail()) + space;
		return_string += data;
	}
	strcpy_s(charString, return_string.c_str());
	return charString;
}

EXP char* getSort4String(bool reverse = false)	// list by date
{
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
		return 0;
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// sort first
	if (reverse == false)
		books.sort(Report::cmpByDate1);
	else
		books.sort(Report::cmpByDate2);

	// regular operation then
	string data, return_string = "";
	string space = " ";
	char charString[10000];
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		data = (string)(*it).getIsbn() + space +
			(string)(*it).getBookTitle() + space +
			(string)(*it).getAuthor() + space +
			(string)(*it).getPublisher() + space +
			(string)(*it).getDateAdded() + space +
			numToStr((*it).getQtyOnHand()) + space +
			numToStr((*it).getWholesale()) + space +
			numToStr((*it).getRetail()) + space;
		return_string += data;
	}
	strcpy_s(charString, return_string.c_str());
	return charString;
}


// Management
EXP char* lookUpBookByName(char* name)	// 根据书名查询
{
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
		return 0;
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	string bookName(name);				// 用户输入的书名
	int bookNum = 0;				// 存储总共查找到的书目的种数
	list<BookData> tempDict;		// 暂时存储可能符合要求的BookData对象

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
	if (bookNum == 0)
		return (char*)"-1";

	string data, return_string = "";
	string space = " ";
	char charString[10000];
	for (list<BookData>::iterator it = tempDict.begin(); it != tempDict.end(); ++it) {
		data = (string)(*it).getIsbn() + space +
			(string)(*it).getBookTitle() + space +
			(string)(*it).getAuthor() + space +
			(string)(*it).getPublisher() + space +
			(string)(*it).getDateAdded() + space +
			numToStr((*it).getQtyOnHand()) + space +
			numToStr((*it).getWholesale()) + space +
			numToStr((*it).getRetail()) + space;
		return_string += data;
	}
	strcpy_s(charString, return_string.c_str());
	return charString;
}

EXP void addBook(char* isbn, char* bookTitle, char* author, char* publisher, char* dateAdded, int qtyOnHand, double wholesale, double retail)
{
	// open the file
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// do the add operation
	books.push_back(BookData(isbn, bookTitle, author, publisher, dateAdded, qtyOnHand, wholesale, retail));

	// save the additon into the file
	fstream dataFile2("repository.dat", ios::binary | ios::out);
	if (dataFile2.fail()) {
		cout << "文件保存失败！";
		exit(0);
	}

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		buff = *it;
		dataFile2.write((char*)&buff, sizeof(buff));
	}

	dataFile2.close();
}

EXP void modifyBook(char* target_isbn, char* target_book,	// 输入目标书本对象的书名和isbn号，修改为后面的一堆参数
	char* isbn, char* bookTitle, char* author, char* publisher, char* dateAdded, int qtyOnHand, double wholesale, double retail)	// query book, return -1 if not find，将名字为book,isbn号为isbn的书信息修改为参数内的值
{
	// open the file
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// 修改指定isbn号的一本书
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		if (string((*it).getIsbn()) == string(target_isbn) && string((*it).getBookTitle()) == string(target_book)) {
			(*it).setIsbn(isbn);
			(*it).setBookTitle(bookTitle);
			(*it).setAuthor(author);
			(*it).setPublisher(publisher);
			(*it).setDateAdded(dateAdded);
			(*it).setQtyOnHand(qtyOnHand);
			(*it).setWholesale(wholesale);
			(*it).setRetail(retail);
			break;
		}
	}

	// save the additon into the file
	fstream dataFile2("repository.dat", ios::binary | ios::out);
	if (dataFile2.fail()) {
		cout << "文件保存失败！";
		exit(0);
	}

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		buff = *it;
		dataFile2.write((char*)&buff, sizeof(buff));
	}

	dataFile2.close();

}

EXP void deleteBook(char* target_isbn, char* target_book)
{
	// open the file
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// delete operation
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		if (string((*it).getIsbn()) == string(target_isbn) && string((*it).getBookTitle()) == string(target_book)) {
			books.erase(it);
			break;
		}
	}

	// save the additon into the file
	fstream dataFile2("repository.dat", ios::binary | ios::out);
	if (dataFile2.fail()) {
		cout << "文件保存失败！";
		exit(0);
	}

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		buff = *it;
		dataFile2.write((char*)&buff, sizeof(buff));
	}

	dataFile2.close();

}

EXP char* lookUpBookByISBN(char* isbn)	// 根据书名查询
{
	// open the file
	list<BookData> books;
	BookData buff;
	fstream dataFile1("repository.dat", ios::binary | ios::in);
	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
		return 0;
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	string Isbn(isbn);				// 用户输入的书名

	string data;
	string space = " ";
	char charString[1000];

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		if (string((*it).getIsbn()) == Isbn) {
			data = (string)(*it).getIsbn() + space +
				(string)(*it).getBookTitle() + space +
				(string)(*it).getAuthor() + space +
				(string)(*it).getPublisher() + space +
				(string)(*it).getDateAdded() + space +
				numToStr((*it).getQtyOnHand()) + space +
				numToStr((*it).getWholesale()) + space +
				numToStr((*it).getRetail()) + space;
			strcpy_s(charString, data.c_str());
			return charString;
		}
	}

	return (char*)"-1";
}

EXP void changeQty(char *target_isbn, int currentQty)	// 将isbn为target_isbn的书的当前库存量改为currentQty
{
	// open the file
	list<BookData> books;
	BookData buff;

	fstream dataFile1("repository.dat", ios::binary | ios::in);

	if (dataFile1.fail()) {
		cout << "文件打开失败！";
		exit(0);
	}

	while (!dataFile1.eof()) {
		dataFile1.read((char*)&buff, sizeof(buff));
		books.push_back(buff);
		if (dataFile1.fail())
			break;
	}
	dataFile1.close();
	books.pop_back();

	// delete operation
	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		if (string((*it).getIsbn()) == string(target_isbn)) {
			(*it).setQtyOnHand(currentQty);
			break;
		}
	}

	// save the additon into the file
	fstream dataFile2("repository.dat", ios::binary | ios::out);
	if (dataFile2.fail()) {
		cout << "文件保存失败！";
		exit(0);
	}

	for (list<BookData>::iterator it = books.begin(); it != books.end(); ++it) {
		buff = *it;
		dataFile2.write((char*)&buff, sizeof(buff));
	}

	dataFile2.close();
}