#ifndef WIDGET_H
#define WIDGET_H
#include <QDebug>
#include <QFile>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget{

        Q_OBJECT

    public :

        QStringList m_openedFilePaths; // 用来记住打开的多文件路径
        Widget(QWidget *parent = nullptr);
        ~Widget();

    signals://自定义信号
//        void my_signal( );
//        void my_signal_params(int value);
    private slots:
        void on_openButton_clicked();
        void on_saveButton_clicked_diy( );
        void on_writerButton_clicked_diy( );
//        void my_slot( );//自定义槽
//        void my_slot_params(int value );
        void on_newButton_clicked();

//        void on_saveButton_clicked();

        void on_closeButton_clicked();
        void on_cursorPositionChanged();
    public  slots:
        void on_currentIndexChanged( );
private:
        Ui::Widget *ui;
        QFile file;//file 是成员变量（成员对象），在打开文件的函数中，使用的是另外一个同名的file(str) ，这是局部变量。所以file一直没被打开。
};

#endif // WIDGET_H
