#include <QtWidgets/QApplication>
#include "NKTest.h"
#include "NKTestConstants.h"

int main(int argc, char** argv)
{
    nk_directory.create();
    nk_quiz_directory.create();
    nk_data_directory.create();

    QApplication a(argc, argv);
    NKTest w;
    w.show();
    return a.exec();
}