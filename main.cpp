#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap iconPixmap(":/icons/icons/edit.png");
    if (iconPixmap.isNull()) {
        qDebug() << "Failed to load pixmap!";
    }

    QIcon appIcon(iconPixmap);
    if (appIcon.isNull()) {
        qDebug() << "Failed to load icon!";
    }

    a.setWindowIcon(appIcon);
    MainWindow w;
    w.show();
    return a.exec();
}
