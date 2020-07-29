#include "library.h"
/*
	下面定义几个全局函数:
	- strToNum(char*):     将代表数字的char*字符串转化为10进制整数
	- strToNum(string):    将代表数字的string字符串转化为10进制整数

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

int strToNum(char* str)
{
	int num = 0;
	for (unsigned int i = 0; i < strlen(str); i++)
		num += (*(str + i) - '0') * pow(10, strlen(str) - i - 1);
	return num;
}

int strToNum(string str)
{
	int num = 0;
	for (unsigned int i = 0; i < str.length(); i++)
		num += (str[i] - '0') * pow(10, str.length() - i - 1);
	return num;
}

string numToStr(int x)
{
	string result;
	stringstream ss;		// 定义流对象ss
	ss << x;				// 将int x转化为流ss
	ss >> result;			// 将流ss转换成字符串
	return result;
}

string numToStr(double x)
{
	string result;
	stringstream ss;		// 定义流对象ss
	ss << x;				// 将int x转化为流ss
	ss >> result;			// 将流ss转换成字符串
	return result;
}


/*split函数效果展示：

	string a = "apple-banana-pear";
	按照'-'分割得到：
	"apple" "banana" "pear"
	分割结果储存在string型向量中，即第二个参数引用的vector
*/
void split(const string& s, vector<string>& result, const char flag)
{
	result.clear();						// result存放最终的切割结果，所以一开始就先清空一次
	istringstream stimulateCin(s); 		// 模拟输入的一个输入流对象，模拟输入的内容就是我们的要切割的对象
	string splitPiece;					// 暂时存放每一个切割的小块

	while (getline(stimulateCin, splitPiece, flag)) {	// 使用getline函数，片段读取，达到切割的效果
		result.push_back(splitPiece);
	}
}


/*strip函数效果展示：
	string a = "\napple\n";
	按'\n'strip得到：
	"apple"
	原string变成了"apple"

	若string a = "apple";
	按'\n'strip不作任何变化
*/
void strip(string& s, const char flag)
{
	string temp;
	for (unsigned i = 0; i < s.length(); i++) {
		// 被过滤的条件：处于两端且==flag
		if ((i == 0 || i == (s.length() - 1)) && s[i] == flag)
			continue;
		else
			temp += s[i];
	}
	s.swap(temp);
}

void centerPrint(string& s, int width, char flag)
{
	if (s.length() >= width)
		cout << s;
	else {
		int leftWid = (width - s.length()) / 2;
		int rightWid = width - leftWid - s.length();
		string leftStr(leftWid, flag), rightStr(rightWid, flag);
		cout << (leftStr + s + rightStr);
	}
}

void centerPrint(char* p, int width, char flag)
{
	if (strlen(p) >= width)
		cout << p;
	else {
		int leftWid = (width - strlen(p)) / 2;
		int rightWid = width - leftWid - strlen(p);
		string leftStr(leftWid, flag), rightStr(rightWid, flag), str(p);
		cout << (leftStr + str + rightStr);
	}
}

void getXY(int* x, int* y)
{
	CONSOLE_SCREEN_BUFFER_INFO c;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h, &c);
	*x = c.dwCursorPosition.X;
	*y = c.dwCursorPosition.Y;
}

void goToXY(int x, int y)
{
	COORD p = { x,y };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, p);
}
