#include "library.h"

/*
	实现BookData中的函数
	- displayInfo:		格式输出，两个参数是控制箭头的输出宽度和后续的输出宽度，缺省值是5和7
*/
void BookData::displayInfo(int width1, int width2)
{
	cout << setw(width1) << " " << setw(width2) << "ISBN  号" << ": " << isbn << endl;
	cout << setw(width1) << " " << setw(width2) << "书    名" << ": " << bookTitle << endl;
	cout << setw(width1) << " " << setw(width2) << "作 者 名" << ": " << author << endl;
	cout << setw(width1) << " " << setw(width2) << "出 版 社" << ": " << publisher << endl;
	cout << setw(width1) << " " << setw(width2) << "进书日期" << ": " << dateAdded << endl;
	cout << setw(width1) << " " << setw(width2) << "库 存 量" << ": " << qtyOnHand << endl;
	cout << setw(width1) << " " << setw(width2) << "批 发 价" << ": " << wholesale << "元/本" << endl;
	cout << setw(width1) << " " << setw(width2) << "零 售 价" << ": " << retail << "元/本" << endl;
}