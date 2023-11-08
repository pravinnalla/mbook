#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dbmanager.h"
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QThread>
#include "threadedcsvimport.h"
#include "pdfprint.h"


#include <QDesktopServices>
#include <QtCore>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    QString fileChekStartString1="\"Sr. No.\",\"Case No.\",\"Next Date\"";
    QString fileChekStartString2="\"Sr. No.\",\"Cases\",\"Next Date\"";

    QString modelQuery = "SELECT DISTINCT filename FROM tblMain ORDER BY filename";

    DbManager mMainWinDbManager;
    Pdfprint mPdfprint;

    QSqlQueryModel *model = new QSqlQueryModel;
    QModelIndex Index;
    QItemSelectionModel *selectionModel;

    void widgetOnOff(bool flag);
    void modelReset();

    QString sourchCodeLink = "https://github.com/pravinnalla/mbook.git";

    QString versionNo = "Version 1.0.2";

private slots:
    void slotThreadFininshed();
    void soltSelChanged(const QModelIndex &current);
    void on_btnImportCsv_clicked();
    void on_btnRemoveCsv_clicked();
    void on_btnGeneratePdf_clicked();

    void on_actionHow_triggered();



    void on_actiongithub_repository_triggered();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();









};
#endif // MAINWINDOW_H
