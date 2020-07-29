from ctypes import *
from time import time

start_time = time()
dll = WinDLL("./CreateDLL.dll")

"""
addBook = dll.addBook
# prepare the data
isbn = c_char_p("555".encode("gbk"))
bookName = c_char_p("面向对象C++程序设计".encode("gbk"))
author = c_char_p("皮德常".encode("gbk"))
publisher = c_char_p("清华大学出版社".encode("gbk"))
date = c_char_p("2017-2-1".encode("gbk"))
qty = c_int(30)
sale = c_double(10)
retail = c_double(39)

addBook(isbn, bookName, author, publisher, date, qty, sale, retail)
"""
changeQty = dll.changeQty
target_isbn = c_char_p("978-7-5063-4127-1".encode("gbk"))
currentQty = c_int(15)
changeQty(target_isbn, currentQty)

lookUpBookByISBN = dll.lookUpBookByISBN
lookUpBookByISBN.restype = c_char_p
ans = lookUpBookByISBN(c_char_p("978-7-5063-4127-1".encode("gbk"))).decode("gbk").split()

print(ans)


getSort1String = dll.getSort1String
getSort1String.restype = c_char_p
ans = getSort1String().decode("gbk").split()





data = []
for i in range(len(ans) // 8):
    data.append(ans[i * 8 : i * 8 + 8])



