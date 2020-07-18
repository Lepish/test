#include "modelviewdemowidget.h"
#include <QApplication>
#include<QFileDialog>

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );
    ModelViewDemoWidget w;
    //QString str = QFileDialog::getOpenFileName();
    w.show();
    return a.exec();
}
