#ifndef PDFPRINT_H
#define PDFPRINT_H

#include <QObject>
#include "dbmanager.h"

#include <QSqlQuery>
#include <QTextDocument>
#include <QDate>
#include <QDir>
#include <QFileDialog>

/******************************************/

#include "hpdf.h"
#include <iostream>



namespace {
    void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    fprintf(stderr, "libharu error: error_no=%04X, detail_no=%d\n",
              (unsigned int) error_no, (int) detail_no);
    }

}

/*******************************************/


class Pdfprint : public QObject
{
    Q_OBJECT

public:
    explicit Pdfprint(QObject *parent = nullptr);
    ~Pdfprint();

    void printTable();

    DbManager mdbPdfPrint;


    bool heightcheck(int heightcounter, HPDF_REAL p_height, int borderspace);


// public pdf variables

    HPDF_Doc  pdf;
    HPDF_Page page;

    HPDF_REAL p_height=0;
    HPDF_REAL p_width=0;

    HPDF_Font def_font;
    HPDF_REAL tw;  //text width


    int borderspace = 100;
    int heightcounter= 50;





};

#endif // PDFPRINT_H
