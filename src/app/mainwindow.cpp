#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(500,400);

    mMainWinDbManager.connectToDb();

    ui->progressBar->hide();
    ui->frame_5->hide();

    ui->progressBar->minimumSize();
    ui->progressBar->setValue(0);

    modelReset();

    selectionModel = ui->listView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::currentChanged, this, &MainWindow::soltSelChanged);

    connect(ui->btnQuit, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);
    connect(ui->actionQuit, &QAction::triggered, this, &QCoreApplication::quit, Qt::QueuedConnection);

    connect(ui->actionGenrate_PDF, &QAction::triggered, this, &MainWindow::on_btnGeneratePdf_clicked);
    connect(ui->actionImport_CSV_File, &QAction::triggered, this, &MainWindow::on_btnImportCsv_clicked);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::widgetOnOff(bool flag)
{
    ui->btnImportCsv->setEnabled(flag);
    ui->btnGeneratePdf->setEnabled(flag);
    ui->btnRemoveCsv->setEnabled(flag);
    ui->btnQuit->setEnabled(flag);

    ui->actionImport_CSV_File->setEnabled(flag);
    ui->actionGenrate_PDF->setEnabled(flag);
    ui->actionQuit->setEnabled(flag);

}

void MainWindow::modelReset()
{
    model->setQuery(modelQuery);
    ui->listView->setModel(model);
    Index= ui->listView->model()->index(0,0);
    ui->listView->selectionModel()->select( Index, QItemSelectionModel::Select );
    ui->listView->setCurrentIndex(Index);
}

void MainWindow::slotThreadFininshed()
{
    mMainWinDbManager.connectToDb();    
    modelReset();
    ui->progressBar->hide();
    ui->frame_5->hide();
    statusBar()->showMessage(tr("File Imported sucessfully"), 4000);
    widgetOnOff(true);
}

void MainWindow::soltSelChanged(const QModelIndex &current)
{
    Index= current;
    ui->listView->setCurrentIndex(Index);

    QStringList strList =model->data(Index,Qt::DisplayRole).toStringList();
    QString str = strList.join("");
    statusBar()->showMessage("Click Remove CSV File to REMOVE "+str+" file data");
}

void MainWindow::on_btnImportCsv_clicked()
{

    mMainWinDbManager.closeDb();


    QString filename =  QFileDialog::getOpenFileName( this, "Open Document", QDir::currentPath(), "CSV files (*.csv)");

    // open file and count lines
    int linecount=0;
    if( !filename.isEmpty()&& !filename.isNull() )
    {
        QFile f(filename.toUtf8());
        if(f.open (QIODevice::ReadOnly)){
            QTextStream ts (&f);
            while(!ts.atEnd()){
                ts.readLine();
                linecount++;
            }
        }
        f.close();
    }
    else{
        mMainWinDbManager.connectToDb();
        modelReset();
    }
    //line counts end



    //Prepare for thread to start
    if(!filename.isEmpty()&& !filename.isNull()){

        //set statusbar
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(linecount);


        // open file check fist line if ok move to thread
        QFile f1(filename.toUtf8());
        if(f1.open (QIODevice::ReadOnly)){
            QTextStream ts1 (&f1);
            QString line1 = ts1.readLine(0);

            if(line1==fileChekStartString1 or line1==fileChekStartString2)      //fist line matched
            {
                f1.close();

                QThread* th = new QThread;
                ThreadedCsvImport* work = new ThreadedCsvImport(linecount, filename);
                work->moveToThread(th);

                connect(th, SIGNAL(started()), work, SLOT(process()));

                ui->progressBar->show();
                ui->frame_5->show();
                statusBar()->showMessage(tr("Importing CSV File, Please wait"));
                widgetOnOff(false);

                connect(work,SIGNAL(status(int)),ui->progressBar,SLOT(setValue(int)));
                connect(work, SIGNAL(finished()), th, SLOT(quit()));
                //connect(work,SIGNAL(finished()),ui->progressBar,SLOT(close())); //after finish what is the status of progressBar?
                connect(work, SIGNAL(finished()), work, SLOT(deleteLater()));
                connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));

                connect(work,SIGNAL(finished()),this,SLOT(slotThreadFininshed())); //after finish what is the status of progressBar?

                th->start();
            }
            else
            {
                f1.close ();
                //ui->listWidget->addItem("Incorrect CSV file");
            }
        }
    }
}



void MainWindow::on_btnRemoveCsv_clicked()
{
    QStringList strList =model->data(Index,Qt::DisplayRole).toStringList();
    QString delString = strList.join("");


    if(delString.isNull() or delString.isEmpty()){
        QMessageBox::critical(nullptr, QObject::tr("Cannot REMOVE"),
                              QObject::tr("Unable to REMOVE a CSV file.\n"
                                          "List is empty. \n\n"
                                          "Click Cancel to close this dialog."), QMessageBox::Cancel);

    }else{

        if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "REMOVE CSV File", "Do you realy want to REMOVE this CSV File ?", QMessageBox::Yes|QMessageBox::No).exec())
        {
            mMainWinDbManager.deleteFromTbl(delString);
            //mMainWinDbManager.arrangeId();
            modelReset();            
        }
    }
}


void MainWindow::on_btnGeneratePdf_clicked()
{
    mMainWinDbManager.closeDb();
    mPdfprint.printTable();

}


void MainWindow::on_actionHow_triggered()
{
    QMessageBox::information(nullptr, QObject::tr("Help File"),
                          QObject::tr("Seprate HELP pdf file for %1 is provided.\n"
                                      "with this utility. \n\n"
                                      "Click Cancel to close this dialog.").arg(versionNo), QMessageBox::Cancel);
}


void MainWindow::on_actiongithub_repository_triggered()
{
    QDesktopServices::openUrl(QUrl(sourchCodeLink));
}
