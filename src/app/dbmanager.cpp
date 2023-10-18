// Class by Praveen Nagnath Nalla for
// 1) Sqlite connection
// 2) Create/Insert/Delete table data



#include "dbmanager.h"

DbManager::DbManager()  = default;


DbManager::~DbManager() {if (mDbManager.isOpen()) { mDbManager.close();}}

void DbManager::connectToDb()
{

    if(!QFile(DATABASE_NAME).exists()){
        this->restoreDb();
    } else {
        this->openDb();
    }




}



bool DbManager::openDb()
{
    //mDbManager = QSqlDatabase::addDatabase("QSQLITE");
    //mDbManager.setDatabaseName(DATABASE_NAME);
    //connName = mDbManager.connectionName();
    //return true;


    const QString& AppDataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir(AppDataPath);

    //qDebug()<<AppDataPath;

    mDbManager = QSqlDatabase::addDatabase("QSQLITE");
    mDbManager.setDatabaseName(AppDataPath+DATABASE_NAME);
    if(mDbManager.open()){
        return true;
    } else {
        return false;
    }


}

void DbManager::closeDb()
{
    //QSqlDatabase::database().close();
    //QSqlDatabase::removeDatabase(connName);


    connName = mDbManager.connectionName();
    mDbManager.close();
    mDbManager = QSqlDatabase();
    mDbManager.removeDatabase(connName);
}


bool DbManager::restoreDb()
{
    return  (this->openDb() and this->createTable());
}

bool DbManager::createTable()
{
    if (!mDbManager.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This tool needs SQLite support. \n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery qryCreateTbl;
    QString stringCreateTblQry="CREATE TABLE IF NOT EXISTS tblMain (mainid INTEGER NOT NULL, casetype TEXT NOT NULL, caseno INTEGER NOT NULL, caseyear INTEGER NOT NULL, nextdate DATE NOT NULL,"
                               "filename TEXT NOT NULL, PRIMARY KEY(mainid AUTOINCREMENT))";

    if(qryCreateTbl.exec(stringCreateTblQry)){
        return true;
    }

    QMessageBox::critical(nullptr, QObject::tr("Database error"), QObject::tr("Unable to create table.\n Click Cancel to exit."), QMessageBox::Cancel);
    return false;
}


void DbManager::csvToTbl(const QString &casetype, const int &caseno, const int &caseyear, const QString &nextdate, const QString &filename)
{

    QSqlQuery pragmS1;
    pragmS1.exec("PRAGMA synchronous = OFF");
    QSqlQuery pragmJ1;
    pragmJ1.exec("PRAGMA journal_mode = MEMORY");


    QSqlQuery qryMainInser;
    qryMainInser.prepare("INSERT INTO tblMain (casetype, caseno, caseyear, nextdate, filename) VALUES (:ct, :cn, :cy, :nd, :fn)");
    qryMainInser.bindValue(0,casetype);
    qryMainInser.bindValue(1,caseno);
    qryMainInser.bindValue(2,caseyear);
    qryMainInser.bindValue(3,nextdate); //check valid date
    qryMainInser.bindValue(4,filename);
    qryMainInser.exec();


        //QSqlQuery qryMainDelDup;
        //qryMainDelDup.prepare("DELETE FROM tblMain WHERE mainid NOT IN (select max(mainid) from tblmain group by casetype, caseno, caseyear, nextdate)");
        //qryMainDelDup.exec();
}

void DbManager::deleteFromTbl(const QString &filename)
{
    QSqlQuery qryMainDelFilename;
    qryMainDelFilename.prepare("DELETE FROM tblMain WHERE filename = ?");
    qryMainDelFilename.addBindValue(filename);
    qryMainDelFilename.exec();
}

void DbManager::deleteDuplicate()
{
    QSqlQuery qryMainDelDup;
    qryMainDelDup.prepare("DELETE FROM tblMain WHERE mainid NOT IN (select max(mainid) from tblmain group by casetype, caseno, caseyear, nextdate)");
    qryMainDelDup.exec();

}


