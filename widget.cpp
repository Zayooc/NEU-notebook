#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog >
#include <QList>
#include <QBrush>
#include <QColor>
#include <QMessageBox>
#include <QShortcut>

Widget::Widget( QWidget *parent ) : QWidget( parent ) , ui( new Ui::Widget ){

    ui->setupUi( this );
    //虽然上面一行代码进行widget和ui窗口关联，但如果窗口大小发生变化，其子控件不会随之改变
    //通过setLayout来
    this->setLayout( ui->verticalLayout );
    // ui->widget->setLayout(ui->tailButton);
    //在构造函数中进行信号与槽的绑定
    //QObject::connect(sender, SIGNAL(signal()), receiver,SLOT(slot()));
    QObject::connect(ui->saveButton, SIGNAL(clicked( )), this ,SLOT(on_saveButton_clicked_diy( )));
    //Lambada表达式构建信号与槽,默认接收者是窗体即this。匿名函数，不用在头文件声明
    QObject::connect(ui->newButton, &QPushButton::clicked,[=]( ){

        qDebug( )<<"新建文件" ;
    });
    //使用函数指针直接连接信号和槽
    QObject::connect(ui->writerButton, &QPushButton::clicked, this ,&Widget::on_writerButton_clicked_diy);
    //    connect(this,SIGNAL(my_signal( )),this,SLOT(my_slot( )));//连接自定义的信号与槽
    //    connect(this,SIGNAL(my_signal_params(int)),this,SLOT(my_slot_params(int )));
    //    emit my_signal( );
    //    emit my_signal_params(666 );
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this , SLOT(on_currentIndexChanged( )));
    connect(ui->textEdit , SIGNAL(cursorPositionChanged()) , this , SLOT(on_cursorPositionChanged()));
    ui->saveButton->setShortcut(QKeySequence::Save);//为保存设置快捷键 Ctrl+S
//    ui->openButton->setShortcut(QKeySequence::Open);//为打开设置快捷键Ctrl+O
//    ui->newButton->setShortcut(QKeySequence::New);//为新建设置快捷键  Ctrl+N
    QShortcut *shortcutOpen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),this);
    QShortcut *shortcutNew = new QShortcut(QKeySequence(tr("Ctrl+N", "File|New")),this);
    connect(shortcutOpen, &QShortcut::activated, this, &Widget::on_openButton_clicked);
    connect(shortcutNew, &QShortcut::activated, this, &Widget::on_newButton_clicked);
    //QTextEdit 控件本身就已经自带放大和缩小字体的快捷键
//    QShortcut *shortcutZoomIn = new QShortcut(QKeySequence::ZoomIn, this);//通常对应 Ctrl + + 或 Ctrl + =
//    QShortcut *shortcutZoomIn_Num  = new QShortcut(QKeySequence("Ctrl++"), this);
//    QShortcut *shortcutZoomIn_Main = new QShortcut(QKeySequence("Ctrl+="), this); // 解决不兼容“数字小键盘的 +”和“主键盘的 =”
//    QShortcut *shortcutZoomOut = new QShortcut(QKeySequence::ZoomOut, this);//通常对应 Ctrl + -
////    connect(shortcutZoomIn, SIGNAL(activated()), ui->textEdit, SLOT(zoomIn()));
//    connect(shortcutZoomIn_Num, SIGNAL(activated()), ui->textEdit, SLOT(zoomIn()));
//    connect(shortcutZoomIn_Main, SIGNAL(activated()), ui->textEdit, SLOT(zoomIn())); // 主键盘也会触发放大
//    connect(shortcutZoomOut, SIGNAL(activated()), ui->textEdit, SLOT(zoomOut()));
    //zoomin、zoomout是将视图放大，不是字体本身
//下面用QFont类
    QShortcut *shortcutZoomIn = new QShortcut(QKeySequence("Ctrl+="), this); // 解决不兼容“数字小键盘的 +”和“主键盘的 =”
    QShortcut *shortcutZoomOut = new QShortcut(QKeySequence("Ctrl+-"), this);//通常对应 Ctrl + -
    connect(shortcutZoomIn, &   QShortcut::activated,[=](){

        //获取当前字体信息
        QFont currentFont = ui->textEdit->font();
        //获取当前字体大小
        int currentFontSize = currentFont.pointSize();
        if (currentFontSize == -1)return ;
        //改变字体大小
        int newFontSize = currentFontSize+1;
        //设置字体大小
        currentFont.setPointSize( newFontSize );
        ui->textEdit->setFont( currentFont );
    });
    connect(shortcutZoomOut, &QShortcut::activated,[=](){

        //获取当前字体信息
        QFont currentFont = ui->textEdit->font();
        //获取当前字体大小
        int currentFontSize = currentFont.pointSize();
        if (currentFontSize == -1)return ;
        //改变字体大小
        int newFontSize = currentFontSize-1;
        //设置字体大小
        currentFont.setPointSize( newFontSize );
        ui->textEdit->setFont( currentFont );
    });
//    ui->textEdit->setMouseTracking(true);
}

Widget::~Widget( ){

    delete ui;
}

void Widget::on_openButton_clicked( ){

    //打开多个文件
    QFileDialog multiDia;//设置并打开文件对话框
    multiDia.setFileMode(QFileDialog::ExistingFiles);
    multiDia.setNameFilter("*.txt");
    multiDia.exec( );

    QStringList qstrings = multiDia.selectedFiles( ) ;//获取用户选中的所有文件路径

    ui->textEdit->clear( );//打开新文件时清空控件

    if (qstrings.isEmpty()) {

        qDebug() << "用户没有选择任何文件，操作取消。";
        return;
    }
    QString fileName;
    if (!qstrings.isEmpty()) {
        QFileInfo firstFileInfo(qstrings.first());
        fileName = firstFileInfo.fileName(); // 例如 "example.txt"
    }
    this -> setWindowTitle(fileName+" NEU记事本");
    for ( QString str : qstrings ){

        qDebug( )<<str ;// 测试打印所有选中的路径
        if (file.isOpen()) file.close();
        //            在打开文件函数中定义了一个局部 QFile 对象QFile file(str);，而不是使用成员变量 file。
        //            因此，对局部文件的打开操作不会影响成员变量 file 的状态，成员变量 file 始终处于未打开状态，导致 if(file.isOpen()) 条件永远为假。
        file.setFileName(str);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text )){// 以可读可写、文本的方式打开

            qDebug( )<<"文件打开失败！";
            continue ;
        }

        //      QByteArray data = file.readAll( );
        QTextStream outStream( &file ) ;
        QString codeType = ui->comboBox->currentText( );

        std::string stdStr = codeType.toStdString();   // 持久化
        const char *cT = stdStr.c_str( );// 指针有效，只要 stdStr 不销毁
        //通过 c_str()，你可以将 std::string 对象的内容安全地传递给这些函数。
        outStream.setCodec(cT);
        QString filedata = outStream.readAll();//readAll和readline不要混在一起用

        //输出文件内容
        //qDebug( ).noquote( ) << data;
        ui->textEdit->append(filedata);
    }
    ui->textEdit->document()->setModified(false);
    // 打开单个文件//    QString fileName = QFileDialog::getOpenFileName(this,
    //         tr("Open File"), "/home", tr("Text files (*.txt) ;; Image  (*.png *.jpg *.bmp)"));
    //    qDebug( )<< fileName;
    // file.setFileName( fileName );

    //    QString firstFilePath = qstrings.first();
    //    QFile file(firstFilePath); // 直接在构造函数里把路径传进去//第二个文件：qstrings[1] 或者 qstrings.at(1)
}

void Widget::on_saveButton_clicked_diy( ){

    QString fileName = file.fileName();
    if (file.fileName().isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                "C:/Users/67298/Desktop/CODE/QT/NEU_notebook/untitled.txt",
                                                tr("Text files (*.txt)"));
        // 如果用户在弹窗里点了“取消”，fileName 会是空的，直接退出函数
        if (fileName.isEmpty()) {
            return;
        }
        // 把用户选择的路径设置给 file 对象
        file.setFileName(fileName);
    }
    if (file.isOpen()) {
        file.close();
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){

        qDebug() << "文件打开失败！具体原因：" << file.errorString();
        return; // 失败了必须 return，千万不能往下继续写了
    }
    this -> setWindowTitle(fileName+" NEU记事本");
    QTextStream outStream( &file ) ;
    //    outStream.setCodec("GBK");
    //    outStream<< QString::fromUtf8("save data to QT notebook");

    QString codeType = ui->comboBox->currentText( );
    std::string stdStr = codeType.toStdString();   // 持久化
    const char *cT = stdStr.c_str( );// 指针有效，只要 stdStr 不销毁
    //通过 c_str()，可以将 std::string 对象的内容安全地传递给这些函数。

    outStream.setCodec(cT);
    outStream<< ui->textEdit->toPlainText();
    outStream.flush();//确保内存里的数据被立刻推送到硬盘上
    ui->textEdit->document()->setModified(false);
}

void Widget::on_writerButton_clicked_diy( ){

    qDebug( )<<"作者是Zayooc" ;
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("关于作者");
    msgBox.setWindowIcon(QIcon(":/pic/NEUlogo.png"));
    msgBox.setText("作者：Zayooc");
    msgBox.setInformativeText("欢迎使用 NEU 记事本！\n自强不息，知行合一！");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setButtonText(QMessageBox::Ok, "已知晓");
    //    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);// 默认高亮保存按钮
    msgBox.exec();
}

//void Widget::my_slot( ){

//    qDebug( )<<"自定义槽被调用" ;
//}

//void Widget::my_slot_params(int value ){

//    qDebug( )<<"自定义槽参数被调用" ;
//    qDebug( )<<value ;
//}

void Widget::on_newButton_clicked( ){

    //打开文件
    //QFile file ;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C:/Users/67298/Desktop/CODE/QT/NEU_notebook/untitled.txt",
                                                    tr("Text files (*.txt)"));
    qDebug( )<<fileName;
    if (fileName.isEmpty()) return;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text |QIODevice::Truncate )){// 以可读可写、文本的方式打开 QIODevice::Truncate，这样如果是覆盖已有文件，会先清空旧内容

        qDebug( )<<"文件打开失败！";
        return;
    }

    QTextStream outStream( &file ) ;
    QString codeType = ui->comboBox->currentText( );
    std::string stdStr = codeType.toStdString();   // 持久化
    const char *cT = stdStr.c_str( );// 指针有效，只要 stdStr 不销毁
    //通过 c_str()，你可以将 std::string 对象的内容安全地传递给这些函数。

    outStream.setCodec(cT);
    outStream<< ui->textEdit->toPlainText();
    //    outStream<< QString::fromUtf8("我写入了一条信息！");
    //    //使用 fromUtf8 告诉 Qt：“我在这行代码里手写的中文是 UTF-8 格式的”
}

void Widget::on_closeButton_clicked( ){

    if (ui->textEdit->document()->isModified()) {

        QMessageBox msgBox;
        msgBox.setText("文件可能已被修改");
        msgBox.setInformativeText("您想保存更改吗？");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);// 默认高亮保存按钮
        msgBox.setButtonText(QMessageBox::Save, "保存");
        msgBox.setButtonText(QMessageBox::Discard, "不保存");
        msgBox.setButtonText(QMessageBox::Cancel, "取消");

        int ret = msgBox.exec();//阻塞等待用户点击，并将点击结果存入 ret
        switch (ret) {
        case QMessageBox::Save:
            // 用户点击了“保存 (Save)”
            on_saveButton_clicked_diy(); // 直接调用你写好的保存逻辑！
            break;

        case QMessageBox::Discard:
            // 用户点击了“不保存 (Discard)”
            // 什么都不用做，直接跳出 switch，往下执行清空和关闭逻辑即可
            break;

        case QMessageBox::Cancel:
            // 用户点击了“取消 (Cancel)”
            // 说明用户不想关了，直接 return 退出当前函数，中止关闭过程
            return;

        default:
            break;
        }
    }
    ui->textEdit->clear();
    if (file.isOpen( )){
        //判断文件是否被打开，如果被打开，关闭键关闭当前窗口并清空文本框
        file.close( );
    }
    file.setFileName("");
    this -> setWindowTitle(" NEU记事本");
    ui->textEdit->document()->setModified(false);
}

void Widget::on_cursorPositionChanged(){

    QTextCursor cursor = ui->textEdit->textCursor();
    qDebug()<<cursor.blockNumber() + 1<<cursor.columnNumber() +1;//行号、列号
    QString rawNum = QString::number(cursor.blockNumber() + 1);
    QString columnNum = QString::number(cursor.columnNumber() +1);
    const QString labelMes = "行:"+ rawNum+" 列:"+columnNum ;
    ui->label->setText(labelMes);
    //设置当前行高亮
    QList<QTextEdit::ExtraSelection> extraSelections;// QList 用于存放所有的额外高亮选择区
    QTextEdit::ExtraSelection newSelection;// 创建一个高亮选择区对象
    //获取当前行号
    newSelection.cursor = ui->textEdit ->textCursor( );// 将当前 textEdit 的光标赋给这个选择区，定位到当前行
    newSelection.cursor.clearSelection();// 清除光标可能自带的选择状态，防止用户拖拽选中文字时高亮逻辑冲突
    //设置背景颜色
    //QBrush qBrush(Qt::Dense4Pattern);
    //    如果要纯色高亮，可以直接用 QColor，例如 QColor(Qt::yellow) 或 QColor("#e8e8e8")
    QColor highlightColor(135, 206, 235, 40);
    QBrush qBrush(highlightColor);
    newSelection.format.setBackground(qBrush);
    newSelection.format.setProperty(QTextFormat::FullWidthSelection, true);//开启全宽选择属性 让高亮背景色铺满这一整行，而不是只停留在有文字的地方
    //设置
    extraSelections.append( newSelection );// 将配置好的选择区放入 QList 容器中
    ui->textEdit->setExtraSelections( extraSelections );// 将 QList 应用到 textEdit 组件，渲染高亮效果
}

void Widget::on_currentIndexChanged( ){//索引从0开始

    qDebug(  )<<"switch code mode" ;
    if (!file.isOpen()) {
        qDebug() << "文件未打开，无法切换编码";
        return;
    }
    if( file.isOpen()){
        qDebug(  )<<"file is open" ;

        file.seek(0);
        if (!file.seek(0)) {
            qDebug() << "无法定位到文件开头";
            return;
        }
        QTextStream outStream( &file ) ;
        QString codeType = ui->comboBox->currentText( );
        //        if (codeType == "ANSI") codeType = "GBK";
        outStream.setCodec(codeType.toLocal8Bit().constData());

        QString fileContent =outStream.readAll();
        ui->textEdit->setPlainText(fileContent);
    }
}
