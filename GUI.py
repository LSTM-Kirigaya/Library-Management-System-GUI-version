import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtPrintSupport import *
from library import *

class QBookManagement(QWidget):
    def __init__(self):
        super(QBookManagement, self).__init__()

        self.setWindowTitle("图书管理系统1.0")
        self.resize(1800, 1200)

        # 几个会用到的变量
        self.currentFileName = ""    # 当前打开的文件名称
        self.currentFilePath  = ""    # 当前打开的文件的路径
        self.globalFont = QFont()   # 全局字体对象
        self.globalFont.setFamily("UD Digi Kyokasho NK-R")
        self.globalFont.setPointSize(15)
        self.shopCart = []       # 购物车，显示当前已经放入的书本
        self.shopCart_ISBN_Qty = []  # 购物车里面的书对应的ISBN号和库存量
        self.tex = 0.05  # 税率

        # 添加菜单栏，工具栏和状态栏
        self.menuBar = QMenuBar()
        self.toolBar = QToolBar()
        self.statusBar = QStatusBar()
        self.barFont = QFont()   # 全局字体对象
        self.barFont.setFamily("UD Digi Kyokasho NK-R")
        self.barFont.setPointSize(12)
        self.statusBar.setFont(self.barFont)

        # 为菜单栏，工具栏和状态栏添加控件与方法
        self.init3Bar()

        # 创建列表控件，也就是左侧的列表
        self.list = QListWidget()
        self.item1 = QListWidgetItem(QIcon("./icon/mod1.ico"), "收银")
        self.item2 = QListWidgetItem(QIcon("./icon/mod2.ico"), "图书管理")
        self.item3 = QListWidgetItem(QIcon("./icon/mod3.ico"), "报表")
        self.list.insertItem(0, self.item1)
        self.list.insertItem(1, self.item2)
        self.list.insertItem(2, self.item3)
        # 更改字体
        self.list.setFont(self.globalFont)
        #self.list.setFixedWidth(200)

        # 创建堆栈窗口控件，也就是右侧的显示空间
        # 创建三个子页面，并将三个子页面载入
        self.stack = QStackedWidget()
        self.stack1 = QWidget()
        self.stack2 = QWidget()
        self.stack3 = QWidget()
        self.stack.addWidget(self.stack1)
        self.stack.addWidget(self.stack2)
        self.stack.addWidget(self.stack3)

        # 下三个函数用来为堆栈中的子页面添加控件
        self.initStack1UI()
        self.initStack2UI()
        self.initStack3UI()

        # 为全局添加布局，将最外层的控件加入其中
        globalLayout = QVBoxLayout()

        # 第一层(菜单)
        globalLayout.addWidget(self.menuBar)

        # 第二层(工具栏)
        globalLayout.addWidget(self.toolBar)
        self.groupBox  = QGroupBox()
        globalLayout.addWidget(self.groupBox)

        # 第三层(主控件)
        mainLay = QHBoxLayout()
        mainLay.addWidget(self.list)
        mainLay.addWidget(self.stack)
        mainLay.setStretch(0, 1)    # 设置左右宽比例
        mainLay.setStretch(1, 7)
        # 加入QSplitter来使得边界可以拖动
        """
        splitter = QSplitter(Qt.Horizontal)
        splitter.addWidget(self.list)
        splitter.addWidget(self.stack)
        globalLayout.addWidget(splitter)
        """
        globalLayout.addLayout(mainLay)

        # 第四层(状态栏)
        globalLayout.addWidget(self.statusBar)

        self.setLayout(globalLayout)

        # 为列表触发的信号（选中的标签行数发生变化）添加槽函数
        # 槽函数负责让堆栈对应发生改变，切换到相应的页面
        self.list.currentRowChanged.connect(self.changeSubUI)

    def init3Bar(self):
        # 菜单栏
        file_label = self.menuBar.addMenu("文件")
        self.actionOpen = QAction("打开")
        self.actionPrint = QAction("打印")
        file_label.addActions([self.actionOpen, self.actionPrint])

        edit_label = self.menuBar.addMenu("编辑")
        self.actionFont = QAction("字体设置")
        self.changeTex = QAction("修改税率")
        edit_label.addActions([self.actionFont, self.changeTex])

        # 工具栏
        self.toolOpen = QAction(QIcon("./icon/open.ico"), "打开(open)")
        self.toolPrint = QAction(QIcon("./icon/print.ico"), "打印(print)")
        self.toolFont = QAction(QIcon("./icon/font.ico"), "字体设置(font set)")
        self.toolCopy = QAction(QIcon("./icon/copy.ico"), "复制(copy)")
        self.toolPaste = QAction(QIcon("./icon/paste.ico"), "粘贴(paste)")
        self.toolShopCart = QAction(QIcon("./icon/shopCartTool.ico"), "购物车")

        self.toolBar.addActions([self.toolOpen, self.toolPrint, self.toolFont, self.toolCopy, self.toolPaste, self.toolShopCart])

        # 状态栏
        # 平常显示目前打开的文件

        # 连接一些信号和槽
        self.actionOpen.triggered.connect(self.toolOpenSlot)
        self.actionPrint.triggered.connect(self.toolPrintSlot)
        self.actionFont.triggered.connect(self.toolFontSlot)
        self.changeTex.triggered.connect(self.changeTexSlot)
        self.toolOpen.triggered.connect(self.toolOpenSlot)
        self.toolPrint.triggered.connect(self.toolPrintSlot)
        self.toolFont.triggered.connect(self.toolFontSlot)
        self.toolCopy.triggered.connect(self.toolCopySlot)
        self.toolPaste.triggered.connect(self.toolPasteSlot)
        self.toolShopCart.triggered.connect(self.displayShopCart)

    def initStack1UI(self): # 第一个窗口，“收银”
        # 先添加搜索用的编辑条和按钮
        self.searchEdit = QLineEdit()
        self.searchEdit.setPlaceholderText("请输入书本的ISBN号")  # 灰色提示信息
        self.searchEdit.setFont(self.globalFont)
        # 使用正则表达式设计校验器
        reg = QRegExp("^[0-9-]+$")
        val = QRegExpValidator(self)
        val.setRegExp(reg)
        # 设置校验器
        self.searchEdit.setValidator(val)
        self.searchbutton = QPushButton(QIcon("./icon/search.ico"), "搜索")
        self.searchbutton.setIconSize(QSize(40, 40))
        self.searchbutton.setFont(self.globalFont)

        # 加入呈现结果的表格控件
        self.searchTable = QTableWidget()
        #self.stack1_table.setFixedHeight(900)
        self.searchTable.setColumnCount(8)
        self.searchTable.setHorizontalHeaderLabels(["ISBN号", "书名", "作者", "出版社", "进书日期", "库存量", "批发价(元/本)", "零售价(元/本)"])
        self.searchTable.setEditTriggers(QAbstractItemView.NoEditTriggers)     # 设置成不可编辑
        self.searchTableHeader = self.searchTable.horizontalHeader()  # 获取横向表格头对象

        # 外观设置
        self.searchTableHeader.setSectionResizeMode(QHeaderView.Stretch)  # 让表格和外框同宽
        # 将横向表头字体和表格内容的字体都改成全局字体
        self.searchTableHeader.setFont(self.globalFont)
        self.searchTable.setFont(self.globalFont)

        # 加入最后购物车按钮和购买按钮
        self.shopCartButton = QPushButton(QIcon("./icon/shopCart.ico") ,"加入购物车")
        self.buyButton = QPushButton(QIcon("./icon/buy.ico"), "结算")
        self.shopCartButton.setFont(self.globalFont)
        self.buyButton.setFont(self.globalFont)
        self.shopCartButton.setIconSize(QSize(40, 40))
        self.buyButton.setIconSize(QSize(40, 40))

        # 将上述所有控件添加进入布局中
        globalLay = QVBoxLayout()
        searchLay = QHBoxLayout()
        searchLay.addWidget(self.searchEdit)
        searchLay.addWidget(self.searchbutton)
        globalLay.addLayout(searchLay)
        globalLay.addWidget(self.searchTable)
        buyLay = QHBoxLayout()
        buyLay.addStretch(1)    # 加入弹簧，让两个按钮往右边靠
        buyLay.addWidget(self.shopCartButton)
        buyLay.addWidget(self.buyButton)
        buyLay.setStretch(0, 4) # 设置弹簧， 购物车按钮，购买按钮的宽之比
        buyLay.setStretch(1, 1)
        buyLay.setStretch(2, 1)
        globalLay.addLayout(buyLay)
        self.stack1.setLayout(globalLay)

        # 绑定信号和槽
        self.searchbutton.clicked.connect(self.searchbuttonSlot)
        self.shopCartButton.clicked.connect(self.shopCartButtonSlot)
        self.buyButton.clicked.connect(self.displayShopCart)
        self.searchTable.itemClicked.connect(self.displaySearchTableItem)

    def initStack2UI(self): # 第二个窗口，“图书管理”
        # 先添加搜索用的编辑条和按钮
        self.queryEdit = QLineEdit()
        self.queryEdit.setPlaceholderText("请输入书名")  # 灰色提示信息
        self.queryEdit.setFont(self.globalFont)

        self.querybutton = QPushButton(QIcon("./icon/search.ico"), "搜索")
        self.querybutton.setIconSize(QSize(40, 40))
        self.querybutton.setFont(self.globalFont)

        # 加入呈现结果的表格控件
        self.queryTable = QTableWidget()
        # self.stack1_table.setFixedHeight(900)
        self.queryTable.setRowCount(0)
        self.queryTable.setColumnCount(8)
        self.queryTable.setHorizontalHeaderLabels(["ISBN号", "书名", "作者", "出版社", "进书日期", "库存量", "批发价(元/本)", "零售价(元/本)"])
        self.queryTable.setEditTriggers(QAbstractItemView.NoEditTriggers)  # 设置成不可编辑
        self.queryTableHeader = self.queryTable.horizontalHeader()  # 获取横向表格头对象

        # 外观设置
        self.queryTableHeader.setSectionResizeMode(QHeaderView.Stretch)  # 让表格和外框同宽
        # 将横向表头字体和表格内容的字体都改成全局字体
        self.queryTableHeader.setFont(self.globalFont)
        self.queryTable.setFont(self.globalFont)

        # 加入最后增删改按钮
        self.addButton = QPushButton(QIcon("./icon/add.ico"), "增加")
        self.deleteButton = QPushButton(QIcon("./icon/delete.ico"), "删除")
        self.modifyButton = QPushButton(QIcon("./icon/modify.ico"), "修改")
        self.addButton.setFont(self.globalFont)
        self.deleteButton.setFont(self.globalFont)
        self.modifyButton.setFont(self.globalFont)
        self.addButton.setIconSize(QSize(40, 40))
        self.deleteButton.setIconSize(QSize(40, 40))
        self.modifyButton.setIconSize(QSize(40, 40))

        # 将上述所有控件添加进入布局中
        globalLay = QVBoxLayout()
        queryLay = QHBoxLayout()
        queryLay.addWidget(self.queryEdit)
        queryLay.addWidget(self.querybutton)
        globalLay.addLayout(queryLay)
        globalLay.addWidget(self.queryTable)
        managementLay = QHBoxLayout()
        managementLay.addStretch(1)  # 加入弹簧，让两个按钮往右边靠
        managementLay.addWidget(self.addButton)
        managementLay.addWidget(self.deleteButton)
        managementLay.addWidget(self.modifyButton)
        managementLay.setStretch(0, 5)
        managementLay.setStretch(1, 1)
        managementLay.setStretch(2, 1)
        managementLay.setStretch(3, 1)
        globalLay.addLayout(managementLay)
        self.stack2.setLayout(globalLay)

        # 连接信号与槽
        self.querybutton.clicked.connect(self.querybuttonSlot)
        self.addButton.clicked.connect(self.addButtonSlot)
        self.deleteButton.clicked.connect(self.deleteButtonSlot)
        self.modifyButton.clicked.connect(self.modifyButtonSlot)
        self.queryTable.itemClicked.connect(self.displayQueryTableItem)

    def initStack3UI(self): # 第三个窗口，“报表”
        # 通过下拉列表来让用户选择排序方式
        self.sortStyleComBox = QComboBox()
        self.sortStyleComBox.addItems(["列出书库中所有图书的完整信息",
                                                             "按库存量排序",
                                                             "按批发价总额排序",
                                                             "按照入库日期排序"])
        self.sortStyleComBox.setFont(self.globalFont)

        # 选择排序方式升序(从小到大)或逆序的按钮
        self.AscendingOrderButton = QPushButton(QIcon("./icon/ascend.ico"), "升序")
        self.DescendingOrderButton = QPushButton(QIcon("./icon/descend.ico"), "逆序")
        self.AscendingOrderButton.setFont(self.globalFont)
        self.DescendingOrderButton.setFont(self.globalFont)
        self.AscendingOrderButton.setIconSize(QSize(40, 40))
        self.DescendingOrderButton.setIconSize(QSize(40, 40))

        self.is_reverse = False
        self.AscOrDes = "升序"

        # 呈现报表结果的表格
        self.reportTable = QTableWidget()
        self.reportTable.setRowCount(0)
        self.reportTable.setColumnCount(8)
        self.reportTable.setHorizontalHeaderLabels(["ISBN号", "书名", "作者", "出版社", "进书日期", "库存量", "批发价(元/本)", "零售价(元/本)"])
        self.reportTableHeader = self.reportTable.horizontalHeader()    # 获取表格头对象

        # 表格与外框同宽；表格不可编辑
        self.reportTableHeader.setSectionResizeMode(QHeaderView.Stretch)
        self.reportTable.setEditTriggers(QAbstractItemView.NoEditTriggers)

        # 设置表格字体
        self.reportTableHeader.setFont(self.globalFont)
        self.reportTable.setFont(self.globalFont)

        book_num, table = Report().getSort1String()
        self.book_num = book_num
        # 填充表格
        for row, one_data in enumerate(table):
            for col, item in enumerate(one_data):
                unitGrid = QTableWidgetItem(item)
                unitGrid.setTextAlignment(Qt.AlignCenter)
                self.reportTable.setItem(row, col, unitGrid)

        globalLay = QVBoxLayout()

        sortStyleLay = QHBoxLayout()
        sortStyleLay.addWidget(self.sortStyleComBox)
        sortStyleLay.addStretch(36)
        sortStyleLay.addWidget(self.AscendingOrderButton)
        sortStyleLay.addStretch(1)
        sortStyleLay.addWidget(self.DescendingOrderButton)
        sortStyleLay.addStretch(2)
        globalLay.addLayout(sortStyleLay)
        globalLay.addWidget(self.reportTable)
        self.stack3.setLayout(globalLay)

        # 其实先显示默认的
        self.book_num, table = Report.getSort1String()
        self.reportTable.setRowCount(self.book_num)
        for row, one_data in enumerate(table):
            for col, item in enumerate(one_data):
                unitGrid = QTableWidgetItem(item)
                unitGrid.setTextAlignment(Qt.AlignCenter)
                self.reportTable.setItem(row, col, unitGrid)
        self.statusBar.showMessage(f"{self.sortStyleComBox.currentText()}({self.AscOrDes})")

        # 绑定信号和槽
        self.sortStyleComBox.currentIndexChanged.connect(self.sortStyleChangedSlot)
        self.AscendingOrderButton.clicked.connect(self.AscendSlot)
        self.DescendingOrderButton.clicked.connect(self.DescendSlot)
        self.reportTable.itemClicked.connect(self.displayReportTableItem)

    def changeSubUI(self, index):
        # 根据list发出的改变到的行数，堆栈窗口会做出改变
        self.stack.setCurrentIndex(index)
        # 并且根据初始化的需要，在状态栏上显示些什么
        if index == 2:
            self.statusBar.showMessage("初始化呈现方式共得到{}个结果".format(self.book_num))

    """
    第一块槽函数：工具栏和菜单栏QAction控件的槽函数
    """
    def toolOpenSlot(self):     # 点击菜单栏的“打开”或者工具栏的“打开”按钮触发的槽函数
        dialog = QFileDialog()
        dialog.setFileMode(QFileDialog.AnyFile)
        dialog.setFileMode(QFileDialog.AnyFile)  # 允许任何文件格式
        dialog.setFilter(QDir.Files)  # 文件过滤，只显示文件

        if dialog.exec():
            filenames = dialog.selectedFiles()  # 返回一个包括了所有选择文件的列表
            self.currentFilePath = filenames[0]
            self.currentFileName = self.currentFilePath.split("/")[-1]
            # 这边要继续写...
            # ......

        self.statusBar.showMessage("打开{}路径下的文件".format(self.currentFilePath))

    def toolPrintSlot(self):    # 点击菜单栏的“打印”或者工具栏的“打印”按钮触发的槽函数
        self.printer = QPrinter  # 打印机对象
        printDialog = QPrintDialog()
        # 判断是否正常执行打印机命令
        if QDialog.Accepted == printDialog.exec():
            self.reportTable.print(self.printer)

        self.statusBar.showMessage("执行打印机指令")

    def toolFontSlot(self):     # 点击菜单栏的“字体”或者工具栏的“字体”按钮触发的槽函数
        fontDialog = QFontDialog()
        font, ok = fontDialog.getFont()
        if ok:
            self.globalFont = font
            self.refresh()

    def toolCopySlot(self):     # 复制当前单元格中的内容
        clipboard = QApplication.clipboard()
        clipboard.setText(self.currentItemText)
        self.statusBar.showMessage(f"已将{self.currentItemText}复制到剪贴板")

    def toolPasteSlot(self):
        row = self.list.currentRow()
        clipboard = QApplication.clipboard()
        # 本GUI中可供粘贴的地方只有两处：两个搜索框处
        if row == 0:
            self.searchEdit.setText(clipboard.text())
            self.statusBar.showMessage("粘贴成功")
        elif row == 1:
            self.searchEdit.setText(clipboard.text())
            self.statusBar.showMessage("粘贴成功")

    def changeTexSlot(self):
        tex, ok = QInputDialog.getDouble(self, "修改税率", "请输入税率(0-1)", self.tex, 0, 1, 3)
        if ok:
            self.tex = tex
            self.statusBar.showMessage(f"当前税率已修改为{tex}")
        else:
            self.statusBar.showMessage("您取消了修改税率的操作")

    def displayShopCart(self):
        if self.shopCart:
            # 购物车按钮弹出的对话框
            self.shopCartDialog = QDialog()
            self.shopCartDialog.setWindowTitle("购物车")
            self.shopCartDialog.resize(800, 800)
            self.shopCartTable = QTableWidget()
            self.shopCartTable.setColumnCount(5)
            self.shopCartTable.setHorizontalHeaderLabels(["书名", "作者", "出版社", "购买数量", "价格"])

            self.shopCartTableHeader = self.shopCartTable.horizontalHeader()
            self.shopCartTableHeader.setSectionResizeMode(QHeaderView.Stretch)
            self.shopCartTable.setEditTriggers(QAbstractItemView.NoEditTriggers)

            self.shopCartTable.setFont(self.globalFont)
            self.shopCartTableHeader.setFont(self.globalFont)


            book_num = len(self.shopCart)
            self.shopCartTable.setRowCount(book_num)

            total_price = 0

            for row, one_data in enumerate(self.shopCart):
                for col, item in enumerate(one_data):
                    unitGrid = QTableWidgetItem(str(item))
                    unitGrid.setTextAlignment(Qt.AlignCenter)
                    self.shopCartTable.setItem(row, col ,unitGrid)
                total_price += float(one_data[-1])

            self.total_price_label = QLabel(f"含税共计{round(total_price * (1 + self.tex), 2)}元")
            self.payButton = QPushButton("购买")
            self.total_price_label.setFont(self.globalFont)
            self.payButton.setFont(self.globalFont)

            hbox = QHBoxLayout()
            vbox = QVBoxLayout()
            hbox.addWidget(self.total_price_label, 1)
            hbox.addStretch(1)
            hbox.addWidget(self.payButton, 1)
            vbox.addWidget(self.shopCartTable)
            vbox.addLayout(hbox)
            self.shopCartDialog.setLayout(vbox)

            self.payButton.clicked.connect(self.payButtonSlot)

            self.shopCartDialog.exec_()



        else:
            QMessageBox.information(self, "提示", "现在您的购物车空空如也，试着去买点东西吧！")

    """
    第二块槽函数：收银模块的槽函数
    """

    def searchbuttonSlot(self):
        if self.searchEdit.text():
            result = Reception.lookUpBookByISBN(self.searchEdit.text())
            if result[0] == "-1":
                QMessageBox.information(self, "提示", f"我家书库里并没有找到ISBN号为“{self.searchEdit.text()}”的书")
            else:
                self.searchTable.setRowCount(1)
                for col_idx, item in enumerate(result):
                    unitGrid = QTableWidgetItem(item)
                    unitGrid.setTextAlignment(Qt.AlignCenter)
                    self.searchTable.setItem(0, col_idx, unitGrid)
                self.statusBar.showMessage("已经呈现")

        else:
            QMessageBox.information(self, "提示", "请输入内容呀亲")


    def shopCartButtonSlot(self):
        # 如果此处不异常处理，那么如果用户在没有选中书的情况下选中按钮，那么会报错
        try:
            # 将当前的结果加入购物车
            index = self.searchTable.currentIndex()
            # 通过输入对话框得到用户想要购买的数量
            num, ok = QInputDialog.getInt(self, "加入购物车", "需要购买的数量", 1, 1, int(self.searchTable.item(index.row(), 5).text()))
            if ok:
                isbn = self.searchTable.item(index.row(), 0).text()
                bookName = self.searchTable.item(index.row(), 1).text()
                author = self.searchTable.item(index.row(), 2).text()
                publisher = self.searchTable.item(index.row(), 3).text()
                price = int(num) * float(self.searchTable.item(index.row(), 7).text())

                currentBook = [bookName, author, publisher, int(num), price]
                currentQty = int(self.searchTable.item(index.row(), 5).text()) - int(num)

                # 将书的基本信息和修改库存量需要的量放入
                self.shopCart.append(currentBook)
                self.shopCart_ISBN_Qty.append([isbn, currentQty])

                self.statusBar.showMessage("已经成功加入购物车")
            else:
                self.statusBar.showMessage("您取消了加入购物车的操作")
        except:
            QMessageBox.information(self, "提示", "请选择你要购买的图书...")

    def displaySearchTableItem(self):
        index = self.searchTable.currentIndex()
        item = self.searchTable.item(index.row(), index.column()).text()
        self.statusBar.showMessage(f"当前单元格中的内容：{item}")
        self.currentItemText = item

    def payButtonSlot(self):    # 最终购买，并将购买数目扣除的函数
        reply = QMessageBox.information(self, "提示", "您确定要购买吗？",
                                            QMessageBox.Yes | QMessageBox.No, QMessageBox.Yes)
        if reply == QMessageBox.Yes:
            for isbn, qty in self.shopCart_ISBN_Qty:
                Reception.changeQty(isbn, qty)
            QMessageBox.information(self, "提示", "购买成功！")
            self.statusBar.showMessage("您完成了购买操作")
            # 刷新一下当前显示的数据
            self.searchbuttonSlot()
            self.sortStyleChangedSlot()
            # 清空购物车
            self.shopCart = []
            self.shopCart_ISBN_Qty = []
            # 退出对话框
            self.shopCartDialog.close()

        else:
            self.statusBar.showMessage("您取消了购买操作")

    """
    第三块槽函数：管理模块的槽函数
    """
    def querybuttonSlot(self):

        if self.queryEdit.text():   # 只有非空才执行查询命令
            query_num, table = Management.lookUpBookByName(self.queryEdit.text())
            if table:
                self.queryTable.setRowCount(query_num)
                for row, one_data in enumerate(table):
                    for col, item in enumerate(one_data):
                        unitGrid = QTableWidgetItem(item)
                        unitGrid.setTextAlignment(Qt.AlignCenter)
                        self.queryTable.setItem(row, col, unitGrid)
                self.statusBar.showMessage(f"含有“{self.queryEdit.text()}”字样的查询结果共有{query_num}个")
            else:
                QMessageBox.information(self, "提示", f"我家书库里并没有找到书名、作者、出版社中含有“{self.queryEdit.text()}”的书")
                self.statusBar.showMessage(f"含有“{self.queryEdit.text()}”字样的查询结果共有{query_num}个")

        else:
            QMessageBox.information(self, "提示", "请输入内容呀亲")

    def addButtonSlot(self):
        self.statusBar.showMessage("进入添加模块")
        self.addDialog = QDialog()
        self.addDialog.resize(700, 500)
        self.addDialog.setWindowTitle("增添图书项")
        self.addDialog.setFont(self.globalFont)

        # 下面是用来让用户输入添加图书的表单，分为：
        # 1.控件创建与装载入布局
        # 2.设置字体
        # 3.设置校验器
        self.isbn_label = QLabel("ISBN号")
        self.isbn_lineEdit = QLineEdit()
        self.book_label = QLabel("书名")
        self.book_lineEdit = QLineEdit()
        self.author_label = QLabel("作者")
        self.author_lineEdit = QLineEdit()
        self.publisher_label = QLabel("出版社")
        self.publisher_lineEdit = QLineEdit()
        self.dateAdded_label = QLabel("进书日期")
        self.dateAdded_lineEdit = QLineEdit()
        self.qtyOnHand_label = QLabel("库存量")
        self.qtyOnHand_lineEdit = QLineEdit()
        self.wholesale_label = QLabel("批发价（元/本）")
        self.wholesale_lineEdit = QLineEdit()
        self.retail_label = QLabel("零售价（元/本）")
        self.retail_lineEdit = QLineEdit()
        self.yesButton = QPushButton("确定")
        self.noButton = QPushButton("取消")

        # 把所有控件放入一个列表中，迭代这个列表来完成对所有控件相同的操作，简化程序
        addDialog_widget = [self.isbn_label, self.isbn_lineEdit,
                            self.book_label, self.book_lineEdit,
                            self.author_label, self.author_lineEdit,
                            self.publisher_label, self.publisher_lineEdit,
                            self.dateAdded_label, self.dateAdded_lineEdit,
                            self.qtyOnHand_label, self.qtyOnHand_lineEdit,
                            self.wholesale_label, self.wholesale_lineEdit,
                            self.retail_label, self.retail_lineEdit,
                            self.yesButton, self.noButton]

        localFont = QFont()
        localFont.setFamily("UD Digi Kyokasho NK-R")
        localFont.setPointSize(14)

        for widget in addDialog_widget:
            widget.setFont(localFont)

        addDialogLayout = QFormLayout()
        for i in range(8):
            addDialogLayout.addRow(addDialog_widget[i * 2], addDialog_widget[i * 2 + 1])

        # 往lineEdit中加入灰色提示信息
        self.isbn_lineEdit.setPlaceholderText("形如XXX-XXX-XXX的标识号")
        self.book_lineEdit.setPlaceholderText("不能含有空格")
        self.author_lineEdit.setPlaceholderText("不能含有空格")
        self.publisher_lineEdit.setPlaceholderText("不能含有空格")
        self.dateAdded_lineEdit.setPlaceholderText("形如XXXX-XX-XX的日期格式，比如2019-7-1")
        self.qtyOnHand_lineEdit.setPlaceholderText("整数")
        self.wholesale_lineEdit.setPlaceholderText("双精度浮点数")
        self.retail_lineEdit.setPlaceholderText("双精度浮点数")
        # 设置校验器
        reg1 = QRegExp("^[0-9-]+$")  # 这是检验isbn和日期输入的校验器，使用正则表达式写
        validator1 = QRegExpValidator(self)
        validator1.setRegExp(reg1)

        reg2 = QRegExp("^[^ ]+$")  # 这是检验不能含有空格的那几项的
        validator2 = QRegExpValidator(self)
        validator2.setRegExp(reg2)

        intVal = QIntValidator()  # 检验库存量的
        intVal.setRange(0, 2000000000)  # 设置范围

        doubleVal = QDoubleValidator()  # 检验批发价和零售价的
        doubleVal.setNotation(QDoubleValidator.StandardNotation)  # 标准双精度检验模式
        doubleVal.setRange(0, 20000000000)  # 设置范围
        doubleVal.setDecimals(2)  # 至多输入小数点后二位

        self.isbn_lineEdit.setValidator(validator1)
        self.book_lineEdit.setValidator(validator2)
        self.author_lineEdit.setValidator(validator2)
        self.publisher_lineEdit.setValidator(validator2)
        self.dateAdded_lineEdit.setValidator(validator1)
        self.qtyOnHand_lineEdit.setValidator(intVal)
        self.wholesale_lineEdit.setValidator(doubleVal)
        self.retail_lineEdit.setValidator(doubleVal)

        judgeLayout = QHBoxLayout()
        judgeLayout.addStretch(1)
        judgeLayout.addWidget(self.yesButton, 1)
        judgeLayout.addStretch(1)
        judgeLayout.addWidget(self.noButton, 1)
        judgeLayout.addStretch(1)

        globalLayout = QVBoxLayout()
        globalLayout.addLayout(addDialogLayout)
        globalLayout.addLayout(judgeLayout)

        self.addDialog.setLayout(globalLayout)
        # 让对话框以模式的状态显示，即触发对话框后，主窗口的控件时无法使用的
        self.addDialog.setWindowModality(Qt.ApplicationModal)

        # 在二级绑定中调用API完成添加
        self.yesButton.clicked.connect(self.useAddAPI)
        self.noButton.clicked.connect(self.cancelAdd)

        self.addDialog.exec_()

    def useAddAPI(self):
        # 先判断非空
        j1 = bool(self.isbn_lineEdit.text())
        j2 = bool(self.book_lineEdit.text())
        j3 = bool(self.author_lineEdit.text())
        j4 = bool(self.publisher_lineEdit.text())
        j5 = bool(self.dateAdded_lineEdit.text())
        j6 = bool(self.qtyOnHand_lineEdit.text())
        j7 = bool(self.wholesale_lineEdit.text())
        j8 = bool(self.retail_lineEdit.text())
        judge = j1 and j2 and j3 and j4 and j5 and j6 and j7 and j8  # 全部非空才为True

        if judge:
            # 注意lineEdit返回的都是str对象
            Management.addBook(isbn=self.isbn_lineEdit.text(),
                               bookTitle=self.book_lineEdit.text(),
                               author=self.author_lineEdit.text(),
                               publisher=self.publisher_lineEdit.text(),
                               dateAdded=self.dateAdded_lineEdit.text(),
                               qtyOnHand=int(self.qtyOnHand_lineEdit.text()),
                               wholesale=float(self.wholesale_lineEdit.text()),
                               retail=float(self.retail_lineEdit.text()))
            self.addDialog.close()
            # 使用查询API刷新一下
            self.querybuttonSlot()
            self.statusBar.showMessage("图书项添加操作完成")

        else:
            QMessageBox.information(self, "提示", "检测到您还有信息未填入，请补充完整")

    def cancelAdd(self):
        self.statusBar.showMessage("您取消了图书项的添加操作")
        self.addDialog.close()

    def deleteButtonSlot(self):
        self.statusBar.showMessage("进入删除模块")
        try:
            index = self.queryTable.currentIndex()
            row = index.row()
            isbn = self.queryTable.item(row, 0).text()
            book = self.queryTable.item(row, 1).text()
            reply = QMessageBox.information(self, "提示", "您确定要删除改行的数据吗？", QMessageBox.Yes | QMessageBox.No,
                                            QMessageBox.Yes)
            if reply == QMessageBox.Yes:
                Management.deleteBook(target_isbn=isbn, target_book=book)
                self.querybuttonSlot()  # 使用查询API刷新一下
                self.statusBar.showMessage("删除成功")
            else:
                self.statusBar.showMessage("您取消了图书项的删除操作")
        except:
            pass

    def modifyButtonSlot(self):
        try:
            self.statusBar.showMessage("进入修改模块")

            self.modifyDialog = QDialog()
            self.modifyDialog.resize(700, 500)
            self.modifyDialog.setWindowTitle("修改图书项")
            self.modifyDialog.setFont(self.globalFont)

            # 获取对当前行数
            row = self.queryTable.currentIndex().row()

            # 确定修改对象的isbn和书名
            self.modify_isbn = self.queryTable.item(row, 0).text()
            self.modify_book = self.queryTable.item(row, 1).text()

            # 下面是用来让用户输入修改图书的表单，大致结构和添加的相同
            self.isbn_label2 = QLabel("ISBN号")
            self.isbn_lineEdit2 = QLineEdit()
            self.book_label2 = QLabel("书名")
            self.book_lineEdit2 = QLineEdit()
            self.author_label2 = QLabel("作者")
            self.author_lineEdit2 = QLineEdit()
            self.publisher_label2 = QLabel("出版社")
            self.publisher_lineEdit2 = QLineEdit()
            self.dateAdded_label2 = QLabel("进书日期")
            self.dateAdded_lineEdit2 = QLineEdit()
            self.qtyOnHand_label2 = QLabel("库存量")
            self.qtyOnHand_lineEdit2 = QLineEdit()
            self.wholesale_label2 = QLabel("批发价（元/本）")
            self.wholesale_lineEdit2 = QLineEdit()
            self.retail_label2 = QLabel("零售价（元/本）")
            self.retail_lineEdit2 = QLineEdit()
            self.yesButton2 = QPushButton("确定")
            self.noButton2 = QPushButton("取消")

            # 把所有控件放入一个列表中，迭代这个列表来完成对所有控件相同的操作，简化程序
            addDialog_widget = [self.isbn_label2, self.isbn_lineEdit2,
                                self.book_label2, self.book_lineEdit2,
                                self.author_label2, self.author_lineEdit2,
                                self.publisher_label2, self.publisher_lineEdit2,
                                self.dateAdded_label2, self.dateAdded_lineEdit2,
                                self.qtyOnHand_label2, self.qtyOnHand_lineEdit2,
                                self.wholesale_label2, self.wholesale_lineEdit2,
                                self.retail_label2, self.retail_lineEdit2,
                                self.yesButton2, self.noButton2]

            localFont = QFont()
            localFont.setFamily("UD Digi Kyokasho NK-R")
            localFont.setPointSize(14)

            for widget in addDialog_widget:
                widget.setFont(localFont)

            addDialogLayout = QFormLayout()
            for i in range(8):
                addDialogLayout.addRow(addDialog_widget[i * 2], addDialog_widget[i * 2 + 1])

            # 往lineEdit中加入灰色提示信息
            self.isbn_lineEdit2.setPlaceholderText("形如XXX-XXX-XXX的标识号")
            self.book_lineEdit2.setPlaceholderText("不能含有空格")
            self.author_lineEdit2.setPlaceholderText("不能含有空格")
            self.publisher_lineEdit2.setPlaceholderText("不能含有空格")
            self.dateAdded_lineEdit2.setPlaceholderText("形如XXXX-XX-XX的日期格式，比如2019-7-1")
            self.qtyOnHand_lineEdit2.setPlaceholderText("整数")
            self.wholesale_lineEdit2.setPlaceholderText("双精度浮点数")
            self.retail_lineEdit2.setPlaceholderText("双精度浮点数")
            # 设置校验器
            reg1 = QRegExp("^[0-9-]+$")  # 这是检验isbn和日期输入的校验器，使用正则表达式写
            validator1 = QRegExpValidator(self)
            validator1.setRegExp(reg1)

            reg2 = QRegExp("^[^ ]+$")  # 这是检验不能含有空格的那几项的
            validator2 = QRegExpValidator(self)
            validator2.setRegExp(reg2)

            intVal = QIntValidator()  # 检验库存量的
            intVal.setRange(0, 2000000000)  # 设置范围

            doubleVal = QDoubleValidator()  # 检验批发价和零售价的
            doubleVal.setNotation(QDoubleValidator.StandardNotation)  # 标准双精度检验模式
            doubleVal.setRange(0, 20000000000)  # 设置范围
            doubleVal.setDecimals(2)  # 至多输入小数点后二位

            # 由于是修改，所以我们填充原有数据

            self.isbn_lineEdit2.setText(self.queryTable.item(row, 0).text())
            self.book_lineEdit2.setText(self.queryTable.item(row, 1).text())
            self.author_lineEdit2.setText(self.queryTable.item(row, 2).text())
            self.publisher_lineEdit2.setText(self.queryTable.item(row, 3).text())
            self.dateAdded_lineEdit2.setText(self.queryTable.item(row, 4).text())
            self.qtyOnHand_lineEdit2.setText(self.queryTable.item(row, 5).text())
            self.wholesale_lineEdit2.setText(self.queryTable.item(row, 6).text())
            self.retail_lineEdit2.setText(self.queryTable.item(row, 7).text())

            self.isbn_lineEdit2.setValidator(validator1)
            self.book_lineEdit2.setValidator(validator2)
            self.author_lineEdit2.setValidator(validator2)
            self.publisher_lineEdit2.setValidator(validator2)
            self.dateAdded_lineEdit2.setValidator(validator1)
            self.qtyOnHand_lineEdit2.setValidator(intVal)
            self.wholesale_lineEdit2.setValidator(doubleVal)
            self.retail_lineEdit2.setValidator(doubleVal)

            judgeLayout = QHBoxLayout()
            judgeLayout.addStretch(1)
            judgeLayout.addWidget(self.yesButton2, 1)
            judgeLayout.addStretch(1)
            judgeLayout.addWidget(self.noButton2, 1)
            judgeLayout.addStretch(1)

            globalLayout = QVBoxLayout()
            globalLayout.addLayout(addDialogLayout)
            globalLayout.addLayout(judgeLayout)

            self.modifyDialog.setLayout(globalLayout)
            # 让对话框以模式的状态显示，即触发对话框后，主窗口的控件时无法使用的
            self.modifyDialog.setWindowModality(Qt.ApplicationModal)

            # 在二级绑定中调用API完成修改
            self.yesButton2.clicked.connect(self.useModifyAPI)
            self.noButton2.clicked.connect(self.cancelModify)

            self.modifyDialog.exec_()
        except:
            pass

    def useModifyAPI(self):
        # 先判断非空
        j1 = bool(self.isbn_lineEdit2.text())
        j2 = bool(self.book_lineEdit2.text())
        j3 = bool(self.author_lineEdit2.text())
        j4 = bool(self.publisher_lineEdit2.text())
        j5 = bool(self.dateAdded_lineEdit2.text())
        j6 = bool(self.qtyOnHand_lineEdit2.text())
        j7 = bool(self.wholesale_lineEdit2.text())
        j8 = bool(self.retail_lineEdit2.text())
        judge = j1 and j2 and j3 and j4 and j5 and j6 and j7 and j8  # 全部非空才为True

        if judge:
            print(self.isbn_lineEdit2.text())
            Management.modifyBook(
                target_isbn=self.modify_isbn,
                target_book=self.modify_book,
                isbn=self.isbn_lineEdit2.text(),
                bookTitle=self.book_lineEdit2.text(),
                author=self.author_lineEdit2.text(),
                publisher=self.publisher_lineEdit2.text(),
                dateAdded=self.dateAdded_lineEdit2.text(),
                qtyOnHand=int(self.qtyOnHand_lineEdit2.text()),
                wholesale=float(self.wholesale_lineEdit2.text()),
                retail=float(self.retail_lineEdit2.text()))
            self.querybuttonSlot()
            self.modifyDialog.close()
            self.statusBar.showMessage("修改成功")

        else:
            QMessageBox.information(self, "提示", "检测到您还有信息未填入，请补充完整")

    def cancelModify(self):
        self.statusBar.showMessage("您取消了图书项的修改操作")
        self.modifyDialog.close()

    def displayQueryTableItem(self):
        index = self.queryTable.currentIndex()
        item = self.queryTable.item(index.row(), index.column()).text()
        self.statusBar.showMessage(f"当前单元格中的内容：{item}")
        self.currentItemText = item



    """
    第四块槽函数：报表模块的槽函数
    """
    def sortStyleChangedSlot(self):     # 根据用户在下拉框中选的选项来排序
        if self.sortStyleComBox.currentIndex() == 0:    #  list all
            self.book_num, table = Report.getSort1String()
            self.reportTable.setRowCount(self.book_num)
            for row, one_data in enumerate(table):
                for col, item in enumerate(one_data):
                    unitGrid = QTableWidgetItem(item)
                    unitGrid.setTextAlignment(Qt.AlignCenter)
                    self.reportTable.setItem(row, col, unitGrid)
            self.statusBar.showMessage(f"{self.sortStyleComBox.currentText()}({self.AscOrDes})")


        elif self.sortStyleComBox.currentIndex() == 1:  # list by number
            self.book_num, table = Report.getSort2String(self.is_reverse)
            self.reportTable.setRowCount(self.book_num)
            for row, one_data in enumerate(table):
                for col, item in enumerate(one_data):
                    unitGrid = QTableWidgetItem(item)
                    unitGrid.setTextAlignment(Qt.AlignCenter)
                    self.reportTable.setItem(row, col, unitGrid)
            self.statusBar.showMessage(f"{self.sortStyleComBox.currentText()}({self.AscOrDes})")

        elif self.sortStyleComBox.currentIndex() == 2:  # list by value
            self.book_num, table = Report.getSort3String(self.is_reverse)
            self.reportTable.setRowCount(self.book_num)
            for row, one_data in enumerate(table):
                for col, item in enumerate(one_data):
                    unitGrid = QTableWidgetItem(item)
                    unitGrid.setTextAlignment(Qt.AlignCenter)
                    self.reportTable.setItem(row, col, unitGrid)
            self.statusBar.showMessage(f"{self.sortStyleComBox.currentText()}({self.AscOrDes})")

        elif self.sortStyleComBox.currentIndex() == 3:  # list by date
            self.book_num, table = Report.getSort4String(self.is_reverse)
            self.reportTable.setRowCount(self.book_num)
            for row, one_data in enumerate(table):
                for col, item in enumerate(one_data):
                    unitGrid = QTableWidgetItem(item)
                    unitGrid.setTextAlignment(Qt.AlignCenter)
                    self.reportTable.setItem(row, col, unitGrid)
            self.statusBar.showMessage(f"{self.sortStyleComBox.currentText()}({self.AscOrDes})")

    def AscendSlot(self):   # 升序
        self.is_reverse = False
        self.AscOrDes = "升序"
        self.sortStyleChangedSlot()

    def DescendSlot(self):  # 降序
        self.is_reverse = True
        self.AscOrDes = "逆序"
        self.sortStyleChangedSlot()

    def displayReportTableItem(self):
        index = self.reportTable.currentIndex()
        item = self.reportTable.item(index.row(), index.column()).text()
        self.statusBar.showMessage(f"当前单元格中的内容：{item}")
        self.currentItemText = item

    """
    第五块槽函数：系统函数
    """
    def refresh(self):  # 刷新字体
        self.list.setFont(self.globalFont)

        # 收银模块的控件
        self.searchEdit.setFont(self.globalFont)
        self.searchbutton.setFont(self.globalFont)
        self.searchTableHeader.setFont(self.globalFont)
        self.searchTable.setFont(self.globalFont)
        self.shopCartButton.setFont(self.globalFont)
        self.buyButton.setFont(self.globalFont)

        # 书库管理模块的控件

        # 报表模块的控件
        self.sortStyleComBox.setFont(self.globalFont)
        self.AsendingOrderButton.setFont(self.globalFont)
        self.DescendingOrderButton.setFont(self.globalFont)
        self.reportTableHeader.setFont(self.globalFont)
        self.reportTable.setFont(self.globalFont)


        self.statusBar.showMessage("字体刷新完成")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon("./icon/app.ico"))
    book = QBookManagement()
    book.show()
    sys.exit(app.exec_())