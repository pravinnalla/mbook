#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


#include <QMessageBox>

#include <QDir>
#include <QStandardPaths>


#define DATABASE_NAME "pdfDb.tbl"          //"Name.db"

class DbManager
{
public:
    DbManager();
    ~DbManager();
    void connectToDb();
    void closeDb();

    void csvToTbl(const QString &casetype, const int &caseno, const int &caseyear, const QString &nextdate, const QString &filename);    

    void deleteFromTbl(const QString &filename);


    void deleteDuplicate();
    int maxId();
    void arrangeId();



private:
    bool openDb();
    bool restoreDb();
    bool createTable();

    QSqlDatabase mDbManager;

protected:
    QString connName;
};

#endif // DBMANAGER_H
