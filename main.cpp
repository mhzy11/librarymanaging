#include "login.h"
#include "registering.h"
#include <QApplication>
#include "globals.h"
registering *regForm;
control2 *controling2;
Login *login1;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    login1 = new Login();
    login1->show();
    return a.exec();
}
