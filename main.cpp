#include "modelviewdemowidget.h"
#include <QApplication>
#include<QFileDialog>

int main( int argc, char* argv[] ) {
    int t;
    QApplication a( argc, argv );
    ModelViewDemoWidget w;
    t++;
    //QString str = QFileDialog::getOpenFileName();
    w.show();
    return a.exec();
}
