#ifndef THREADEDCSVIMPORT_H
#define THREADEDCSVIMPORT_H


#include <QObject>
#include <QThread>
#include <QDebug>

#include <QFileInfo>
#include <unistd.h> //usleep

#include <QSqlQuery>
#include "dbmanager.h"

#include <QDate>

class ThreadedCsvImport: public QObject
{
    Q_OBJECT
public:
    explicit ThreadedCsvImport(int n_iterations, QString filename, QObject *parent = 0);
    DbManager mthreadDbManager;

signals:
    void status(int i);
    void finished();
    void finished(ThreadedCsvImport* worker);

public slots:
    void process();
    void quit();

protected:
    std::atomic_bool isRunning_{};
    int n_iterations;
    QString filename;

    int usleepTime=200;

};


#endif // THREADEDCSVIMPORT_H
