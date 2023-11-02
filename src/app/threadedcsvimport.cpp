// Class by Praveen Nagnath Nalla
// 1) import csv file data to sqlite


#include "threadedcsvimport.h"
#include <utility>

ThreadedCsvImport::ThreadedCsvImport(int n_it, QString filename, QObject *parent) : QObject(parent),n_iterations(n_it), filename(std::move(filename))
{
    isRunning_ = false;
}


void ThreadedCsvImport::quit(){ isRunning_ = false;}

void ThreadedCsvImport::process()
{
    mthreadDbManager.connectToDb();

    QFileInfo fInfo(filename.toUtf8());

    if(!this->isRunning_)
    {
        isRunning_ = true;

        QFile f(filename.toUtf8());
        if(f.open (QIODevice::ReadOnly)){
            QTextStream ts (&f);

            //Travel through the csv file
            int i=0;
            while(!ts.atEnd() && isRunning_){

                QString readline=ts.readLine();
                QStringList line=readline.split(',');
                int lineSize = line.size();
                if(lineSize == 3)                      // CSV file line size is 3 No-CaseNo-Year
                {

                    //      "\"R.C.C./1100157/2009\""      working on line[1]
                    QStringList lineCaseNo=line[1].split('/');
                    int lineCaseNoSize=lineCaseNo.size();
                    if(lineCaseNoSize ==3)
                    {
                        lineCaseNo[0]=lineCaseNo[0].remove('\"');
                        lineCaseNo[0]=lineCaseNo[0].remove('.');
                        lineCaseNo[2]=lineCaseNo[2].remove('\"');


                        //"\"22-08-2023\""			wokring on line[2]
                        QStringList lineDate=line[2].split('-');
                        int lineDateSize=lineDate.size();
                        if(lineDateSize ==3)
                        {
                            lineDate[0]=lineDate[0].remove('\"');
                            lineDate[2]=lineDate[2].remove('\"');

                            QString newDate = lineDate[2]+"-"+lineDate[1]+"-"+lineDate[0];

                            //db.csvToTbl(line[1], line[2]);
                            mthreadDbManager.csvToTbl(lineCaseNo[0], lineCaseNo[1].toInt(), lineCaseNo[2].toInt(), newDate, fInfo.lastModified().toString("dd-MM-yyyy HH.mm.ss"));

                        }
                    }
                }

                emit this->status(i);
                usleep(usleepTime);
                i++;
            }
            //delete duplicate
            mthreadDbManager.deleteDuplicate();

            // rearrange id vale
            mthreadDbManager.arrangeId();

            f.close ();

            mthreadDbManager.closeDb();
        }
        emit this->finished(this); // added to inform a controller object that the threadcsvimport has finished
        emit this->finished();
    }
}
