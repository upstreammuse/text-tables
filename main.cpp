#include <QApplication>
#include "MainWindow.h"

using namespace tt;

int main(int argc, char* argv[])
{
   QApplication app(argc, argv);
   MainWindow mw;
   mw.show();
   return app.exec();
}
