#include "pdfprint.h"


Pdfprint::Pdfprint(QObject *parent)
    : QObject{parent}
{

}

Pdfprint::~Pdfprint()= default;


void Pdfprint::printTable()
{

    heightcounter=50;       //reset hight courter before you leave function or use in constructor

    mdbPdfPrint.connectToDb();


    int tblCount=0;
    QSqlQuery qryCount;
    qryCount.exec("SELECT count(*) as cnt from tblMain");
    if(qryCount.next()){
        tblCount=qryCount.value(0).toInt();
    }

    if(tblCount>=1)     //if table is not empty proceed
    {

        /*
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName("MBookPDFoutput.pdf");
        printer.setPaperSize(QPrinter::A4);

        QPageLayout pageLayout = printer.pageLayout();
        pageLayout.setMinimumMargins(QMarginsF(30,10,10,30));
        printer.setPageLayout(pageLayout);


        QSqlQuery qryDate;
        qryDate.exec("SELECT DISTINCT nextdate from tblMain ORDER BY nextdate");



        QString strHeaderandBg;
        QTextStream outHandBg(&strHeaderandBg);

        while (qryDate.next())
        {
            QString dateString = qryDate.value(0).toString();
            QDate qryDt;
            qryDt = QDate::fromString(dateString, "yyyy-MM-dd");
            QString dateStringLong = qryDt.toString(Qt::DefaultLocaleLongDate);


            outHandBg<<"<html>"
                       "<head>"
                       "<meta Content='Text/html; charset=Windows-1251'>"
                       "</head>"
                       "<body>";


            outHandBg<<QString("<h3 align=center>%1</h3>").arg((!dateStringLong.isEmpty()) ? dateStringLong: QString("&nbsp;"));


            outHandBg<<"<p align=center style='font-size:10pt; margin-bottom: 1em; margin-top:1em'>Took seat at .............................................. Rose at  .............................................. </p>"
                       "<p align=center style='font-size:10pt; margin-bottom: 1em; margin-top:1em'>Took seat at .............................................. Rose at  .............................................. </p>"
                       "<p></p>"
                       "<table border=none cellspacing='0' cellpadding='0'>"
                       "<tr><td width=20%></td><td width=80%></td></tr>";


            QSqlQuery qryCasetype;
            qryCasetype.prepare("SELECT DISTINCT casetype from tblMain WHERE nextdate=? ORDER BY nextdate");
            qryCasetype.addBindValue(dateString);
            qryCasetype.exec();
            while (qryCasetype.next()){
                QString casetypeString = qryCasetype.value(0).toString();

                outHandBg<<QString("<tr><td>%1</td>").arg((!casetypeString.isEmpty()) ? casetypeString : QString("&nbsp;"));


                outHandBg<<"<td><p style='line-height: 1.8;'>";

                QSqlQuery qryCaseno;
                qryCaseno.prepare("SELECT caseno, caseyear FROM tblMain WHERE casetype = ? AND nextdate = ?");
                qryCaseno.addBindValue(casetypeString);
                qryCaseno.addBindValue(dateString);
                if (qryCaseno.exec()) {
                    while (qryCaseno.next()) {

                        QString casenoyearString=qryCaseno.value(0).toString()+"/"+qryCaseno.value(1).toString();

                        outHandBg<<QString("%1,&emsp;&emsp;").arg((!casenoyearString.isEmpty()) ? casenoyearString : QString("&emsp;"));
                    }
                } else {
                    qWarning() << "Query failed:" << qryCaseno.lastError().text();
                }

                outHandBg<<"</p></td></tr>";

            }

            outHandBg<<"</table>";
            outHandBg<<"<hr /><p></p>";
        }


        outHandBg<<"</body></html>";

        QTextDocument document;
        document.setHtml(strHeaderandBg);
        document.print(&printer);
        //mdbPdfPrint.closeDb();        //close at end of function


        //file save message
        QString savingDirectory = QDir::currentPath();
        QMessageBox::information(nullptr, QObject::tr("PDF was saved at..."),
            QObject::tr("%1 was saved at %2").arg("MBookPDFoutput.pdf", savingDirectory),
                QMessageBox::Ok);

*/

        pdf = HPDF_New (error_handler, NULL);
        if (!pdf) { printf ("error: cannot create PdfDoc object\n");  //return 1;
        }

        try {
            // do page description processes (we do not have to check the return code of functions)


            page = HPDF_AddPage (pdf);
            HPDF_Page_SetSize  (page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            p_height = HPDF_Page_GetHeight (page);
            p_width = HPDF_Page_GetWidth (page);


            /* Print the lines of the page. */
//            HPDF_Page_SetLineWidth (page, 1);
//            HPDF_Page_Rectangle (page, 50, 50, p_width-borderspace, p_height-borderspace);
//            HPDF_Page_Stroke (page);

            /* Default font */
            def_font = HPDF_GetFont (pdf, "Helvetica", NULL);
            HPDF_Page_SetFontAndSize (page, def_font, 13);


            QSqlQuery qryDate;
            qryDate.exec("SELECT DISTINCT nextdate from tblMain ORDER BY nextdate");
            while (qryDate.next())
            {
                heightcounter=heightcounter+40;
                if(heightcheck(heightcounter,p_height,borderspace)){
                    page=HPDF_AddPage(pdf);
                    HPDF_Page_SetFontAndSize (page, def_font, 13);
                    heightcounter=50;
                }

                QString dateString = qryDate.value(0).toString();
                QDate qryDt;
                qryDt = QDate::fromString(dateString, "yyyy-MM-dd");
                QString dateStringLong = qryDt.toString(Qt::DefaultLocaleLongDate);


                //QString str1 = QString("Text %1").arg(i);
                QByteArray qBytedateString = dateStringLong.toLatin1();
                const char *date_title = qBytedateString.data();

                HPDF_Font font = HPDF_GetFont (pdf, "Helvetica-Bold", NULL);
                HPDF_Page_SetFontAndSize (page, font, 15);

                tw = HPDF_Page_TextWidth (page, date_title);  //text width of "date_title"
                HPDF_Page_BeginText (page);                
                HPDF_Page_TextOut (page, (p_width - tw) / 2, p_height - heightcounter, date_title);
                HPDF_Page_EndText (page);

                HPDF_Page_SetFontAndSize (page, def_font, 13);


                HPDF_REAL tw1;  //text width                
                const char *time_title = "Took seat at ........................................... Rose at  ........................................... ";
                heightcounter=heightcounter+40;

                if(heightcheck(heightcounter,p_height,borderspace)){
                    page=HPDF_AddPage(pdf);
                    HPDF_Page_SetFontAndSize (page, def_font, 13);
                    heightcounter=50;
                }


                tw1 = HPDF_Page_TextWidth (page, time_title);  //text width of "date_title"
                HPDF_Page_BeginText (page);
                HPDF_Page_TextOut (page, (p_width - tw1) / 2, p_height - heightcounter, time_title);
                HPDF_Page_EndText (page);


                QSqlQuery qryCasetype;
                qryCasetype.prepare("SELECT DISTINCT casetype from tblMain WHERE nextdate=? ORDER BY nextdate");
                qryCasetype.addBindValue(dateString);
                qryCasetype.exec();

                while (qryCasetype.next()){

                    heightcounter=heightcounter+40;

                    if(heightcheck(heightcounter,p_height,borderspace)){
                        page=HPDF_AddPage(pdf);
                        HPDF_Page_SetFontAndSize (page, def_font, 13);
                        heightcounter=50;
                    }

                    QString casetypeString = qryCasetype.value(0).toString();
                    QByteArray qByteCaseTypeString = casetypeString.toLatin1();
                    const char *casetype_title = qByteCaseTypeString.data();

                    HPDF_Page_BeginText (page);
                    HPDF_Page_TextOut (page, 50, p_height - heightcounter, casetype_title);
                    HPDF_Page_EndText (page);



                    QSqlQuery qryCaseno;
                    qryCaseno.prepare("SELECT caseno, caseyear FROM tblMain WHERE casetype = ? AND nextdate = ? ");
                    qryCaseno.addBindValue(casetypeString);
                    qryCaseno.addBindValue(dateString);


                    QList<int> xc = { 150, 250, 350, 450};
                    int xcTrack = 0;


                    if (qryCaseno.exec()) {
                        while (qryCaseno.next()) {
                            QString casenoyearString=qryCaseno.value(0).toString()+"/"+qryCaseno.value(1).toString();
                            QByteArray qByteCaseNoString = casenoyearString.toLatin1();
                            const char *caseno_title = qByteCaseNoString.data();

                            int xvalue = xc[xcTrack];



                            HPDF_Page_BeginText (page);

                            HPDF_Page_TextOut (page, xvalue, p_height - heightcounter, caseno_title);
                            HPDF_Page_EndText (page);

                            xcTrack++;
                            if(xcTrack>=4){
                                xcTrack=0;
                                heightcounter=heightcounter+20;
                                if(heightcheck(heightcounter,p_height,borderspace)){
                                    page=HPDF_AddPage(pdf);
                                    HPDF_Page_SetFontAndSize (page, def_font, 13);
                                    heightcounter=50;
                                }


                            }


                        }
                    }

                }

                /* Print the lines below. */
                heightcounter=heightcounter+10;
                HPDF_Page_MoveTo (page, 50, p_height - heightcounter);
                HPDF_Page_LineTo (page, 50 + (p_width-borderspace), p_height - heightcounter);
                HPDF_Page_Stroke (page);

                heightcounter=heightcounter+2;
                HPDF_Page_MoveTo (page, 50, p_height - heightcounter);
                HPDF_Page_LineTo (page, 50 + (p_width-borderspace), p_height - heightcounter);
                HPDF_Page_Stroke (page);

                //qDebug()<<"dp_height"<<hightCounter;
                //qDebug()<<p_height;
            }


            // save the document to a file
//            HPDF_SaveToFile (pdf, "fname.pdf");


            //********************

            QString fileName = QFileDialog::getSaveFileName(0, tr("Save File"),"MBook.pdf", tr("PDF files (*.pdf)"));

            if( !fileName.isEmpty()&& !fileName.isNull() )
            {
                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly))
                {
                    HPDF_SaveToStream(pdf);
                    /* get the data from the stream and write it to file. */
                    for (;;)
                    {
                        HPDF_BYTE buf[4096];
                        HPDF_UINT32 siz = 4096;
                        HPDF_STATUS ret = HPDF_ReadFromStream(pdf, buf, &siz);

                        if (siz == 0)
                        {
                            break;
                        }

                        if (-1 == file.write(reinterpret_cast<const char *>(buf), siz))
                        {
                            qDebug() << "Write PDF error";
                            break;
                        }
                    }
                }

            }


            //***************************

        } catch (...) {
            HPDF_Free (pdf);
            //return 1;
        }

        // clean up
        HPDF_Free (pdf);

        //return 0;


    }//if tblCount end
    else{
        QMessageBox::critical(nullptr, QObject::tr("Cannot Generate PDF"),
                              QObject::tr("Unable to Generate PDF.\n"
                                          "no data available. \n\n"
                                          "Click Cancel to close this dialog."), QMessageBox::Cancel);
    }

    //mdbPdfPrint.closeDb();

}

bool Pdfprint::heightcheck(int heightcounter, HPDF_REAL p_height, int borderspace)
{
    if(heightcounter>=p_height-borderspace){
    return true;
    }
    return false;

}
