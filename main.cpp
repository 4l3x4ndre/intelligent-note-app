#include <QApplication>
#include "./gui/editor.h"


int main(int argc, char *argv[]) {



    QApplication app(argc, argv);
    Editor editor;
    editor.show();
    return QApplication::exec();
}
