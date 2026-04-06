#include "mytextedit.h"
#include <QDebug>
#include <QWheelEvent>

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent){


}

void MyTextEdit::keyPressEvent(QKeyEvent *e){

    if ( e->key() == Qt::Key_Control ){//如果按下的按键是Ctrl
        qDebug()<<"contrl Pressed";
        ctrlKeyPressed = 1;
    }
    QTextEdit::keyPressEvent(e);
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *e){

    if ( e->key() == Qt::Key_Control ){
        qDebug()<<"contrl Pressed";
        ctrlKeyPressed = 0;
    }
    QTextEdit::keyReleaseEvent(e);
}

void MyTextEdit::wheelEvent(QWheelEvent *e){

    qDebug()<<e->angleDelta().y();//Qpiont
    if ( ctrlKeyPressed == 1){
        if (e->angleDelta().y() < 0){
            zoomOut();//界面缩小
        }else if (e->angleDelta().y() > 0){
            zoomIn();//界面放大
        }
        e->accept();//子控件中的事件处理完毕，accept结束。
        //如果没有处理完毕，用ignore放到更大一级的控件中处理。如果一直到最大一级还没被处理，就不处理这个事件
    }else {
        QTextEdit::wheelEvent(e);// 交给父类，该怎么处理就怎么处理
    }
}
