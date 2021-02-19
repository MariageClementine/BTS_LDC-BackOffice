#include <QtGui/QApplication>
#include "mainwindow.h"
#include "connexion.h"
#include <QSqlDatabase>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //connexion a la base
    QSqlDatabase base = QSqlDatabase::addDatabase("QMYSQL");
    base.setHostName("192.168.1.26");
    base.setUserName("LCourse");
    base.setPassword("passlc");
    base.setPort(3306);
    base.setDatabaseName("listeCourses");
    Connexion c;
    if(!base.open())
    {
        qDebug()<<"Le serveur de données est inaccessible.\r\nMerci de contacter votre administrateur et de relancer l'application.";
        return(1);
    }
    else
    {

        if(c.exec()==Connexion::Accepted)
        {
            MainWindow w;
            w.show();
            return a.exec();
        }
    }

}
