"""
该文件使用三个类来封装C++的dll，
GUI将通过这个文件的类的静态方法来调用C++的dll，
从而访问和修改图书库
"""

from ctypes import *
# 通过dll对象作为接口，可以调用C++代码中带有EXP标记的函数
# 调用方式如下：
# 1.通过dll接口获得函数指针： getA = dll.getA
# 2.定义返回Python的数据类型： getA.restype = c_char_p
# 3.调用，如果是c_char_p型的返回值需要转化成str对象，编码是gkb： letter = getA().decode("gkb")
dll = WinDLL("./CreateDLL.dll")

# 对应收银模块
class Reception():
    def __init__(self):
        pass

    @staticmethod
    def lookUpBookByISBN(isbn):
        """
        :param isbn: str对象
        :return: void
        """
        lookUpBookByISBN = dll.lookUpBookByISBN
        lookUpBookByISBN.restype = c_char_p
        search_result = lookUpBookByISBN(c_char_p(isbn.encode("gbk"))).decode("gbk").split()
        return search_result

    @staticmethod
    def changeQty(target_isbn, currentQty):
        """
        :param target_isbn: str对象
        :param currentQty: int对象
        :return: void
        """
        changeQty = dll.changeQty
        input_target_isbn = c_char_p(target_isbn.encode("gbk"))
        input_currentQty = c_int(currentQty)
        changeQty(input_target_isbn, input_currentQty)

# 对应管理模块
class Management():
    def __init__(self):
        pass

    @staticmethod
    def lookUpBookByName(name):
        lookUpBookByName = dll.lookUpBookByName
        lookUpBookByName.restype = c_char_p     # 输出值类型
        look_name = c_char_p(name.encode("gbk"))    # 输入值类型

        data = lookUpBookByName(look_name).decode("gbk").split()
        table = []
        num = len(data) // 8
        for i in range(num):
            table.append(data[i * 8: i * 8 + 8])
        return num, table

    @staticmethod
    def addBook(isbn, bookTitle, author, publisher, dateAdded, qtyOnHand, wholesale, retail):
        addBook = dll.addBook
        input_isbn = c_char_p(isbn.encode("gbk"))
        input_bookTitle = c_char_p(bookTitle.encode("gbk"))
        input_author = c_char_p(author.encode("gbk"))
        input_publisher = c_char_p(publisher.encode("gbk"))
        input_dateAdded = c_char_p(dateAdded.encode("gbk"))
        input_qtyOnHand = c_int(qtyOnHand)
        input_wholesale = c_double(wholesale)
        input_retail = c_double(retail)

        addBook(
                        input_isbn,
                        input_bookTitle,
                        input_author,
                        input_publisher,
                        input_dateAdded,
                        input_qtyOnHand,
                        input_wholesale,
                        input_retail
                        )

    @staticmethod
    def modifyBook(target_isbn, target_book,
                   isbn, bookTitle, author, publisher, dateAdded, qtyOnHand, wholesale, retail):
        modifyBook = dll.modifyBook
        # 索引值
        input_target_isbn = c_char_p(target_isbn.encode("gbk"))
        input_target_book = c_char_p(target_book.encode("gbk"))
        # 修改值
        input_isbn = c_char_p(isbn.encode("gbk"))
        input_bookTitle = c_char_p(bookTitle.encode("gbk"))
        input_author = c_char_p(author.encode("gbk"))
        input_publisher = c_char_p(publisher.encode("gbk"))
        input_dateAdded = c_char_p(dateAdded.encode("gbk"))
        input_qtyOnHand = c_int(qtyOnHand)
        input_wholesale = c_double(wholesale)
        input_retail = c_double(retail)

        modifyBook(
                            input_target_isbn,
                            input_target_book,
                            input_isbn,
                            input_bookTitle,
                            input_author,
                            input_publisher,
                            input_dateAdded,
                            input_qtyOnHand,
                            input_wholesale,
                            input_retail
                            )

    @staticmethod
    def deleteBook(target_isbn, target_book):
        deleteBook = dll.deleteBook
        input_target_isbn = c_char_p(target_isbn.encode("gbk"))
        input_target_book = c_char_p(target_book.encode("gbk"))
        deleteBook(input_target_isbn, input_target_book)

# 对应报表模块
class Report():
    def __init__(self):
        pass

    # 返回书本数量和整合好的二维数组
    @staticmethod
    def getSort1String():   # list all
        getSort1String = dll.getSort1String
        getSort1String.restype = c_char_p
        data = getSort1String().decode("gbk").split()
        table = []
        num = len(data) // 8
        for i in range(num):
            table.append(data[i * 8 : i * 8 + 8])
        return  num, table

    @staticmethod
    def getSort2String(reverse=False):   # list by number, ascend default
        getSort2String = dll.getSort2String
        getSort2String.restype = c_char_p
        data = getSort2String(c_bool(reverse)).decode("gbk").split()
        table = []
        num = len(data) // 8
        for i in range(num):
            table.append(data[i * 8 : i * 8 + 8])
        return  num, table

    @staticmethod
    def getSort3String(reverse=False):   #list by value, ascend default
        getSort3String = dll.getSort3String
        getSort3String.restype = c_char_p
        data = getSort3String(c_bool(reverse)).decode("gbk").split()
        table = []
        num = len(data) // 8
        for i in range(num):
            table.append(data[i * 8 : i * 8 + 8])
        return  num, table

    @staticmethod
    def getSort4String(reverse=False):   #list by date, ascend default
        getSort4String = dll.getSort4String
        getSort4String.restype = c_char_p
        data = getSort4String(c_bool(reverse)).decode("gbk").split()
        table = []
        num = len(data) // 8
        for i in range(num):
            table.append(data[i * 8 : i * 8 + 8])
        return  num, table

if __name__ == "__main__":
    target_isbn = "978-7-302-4589面向对象C++程序设计"
    target_book = "面向对象C++程序设计"

    isbn = "978-7-302-4589"
    bookName = "面向对象C++程序设计"
    author = "皮德常"
    publisher = "清华大学出版社"
    date = "2017-2-1"
    qty = 30
    sale = 10
    retail = 39

    Management.modifyBook(target_isbn, target_book,
                          isbn, bookName, author, publisher, date, qty, sale, retail)

    num, table = Management.lookUpBookByName("清华")
    num, table = Report.getSort1String()

    ans = Reception.lookUpBookByISBN("978-7-5063-4127-1")
    print(ans)