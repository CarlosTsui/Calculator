#include "calculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    calculator w;
    w.setWindowTitle("计算器");
    w.setWindowOpacity(0.9);
    w.show();

    return a.exec();
}
