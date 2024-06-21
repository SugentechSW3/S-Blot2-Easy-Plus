#include <QVector>
#include <QDebug>
#include <QPrinter>
#include <QPainter>

#include "CPrintSupport.h"

NS_SUGENTECH_BEGIN

struct CPrintSupport::privateStruct
{
    QVector<QPicture> mData;
    int printPage;
};

CPrintSupport::CPrintSupport(QObject *parent) :
                                                QThread(parent),
                                                d(new privateStruct)
{
    d->printPage = 1;
    connect(this, &QThread::finished, this, [this]
            {
                this->deleteLater();
            });
}

CPrintSupport::~CPrintSupport()
{
}

void CPrintSupport::setPrintData(const QVector<QPicture>& data, const int printPage)
{
    d->mData = data;
    d->printPage = printPage;
}

void CPrintSupport::run()
{
    QPrinter printer(QPrinter::ScreenResolution);
#ifndef QT_DEBUG
    printer.setOutputFormat(QPrinter::NativeFormat);
#else
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("output.pdf");
#endif
    printer.setPageSize(QPagedPaintDevice::A4);
    printer.setPageMargins(8, 25, 8, 25,QPrinter::Millimeter);

    if(d->printPage == 1)
    {
        for(int i =0; i< d->mData.count(); i++)
        {
            QPainter painter(&printer);
            painter.scale(1.05, 1.05);

            painter.drawPicture(0, 0, d->mData[i]);
        }
    }
    else
    {
        if(d->mData.count() % d->printPage == 0)
        {
            for(int i=0; i< d->mData.count() / d->printPage; i++)
            {
                QPainter painter(&printer);
                painter.scale(1.05, 1.05);

                for(int j=0; j< d->printPage; j++)
                {
                    if(j == 0)
                    {
                        painter.drawPicture(0, 0, d->mData[i * d->printPage + j]);
                    }
                    else
                    {
                        printer.newPage();
                        painter.drawPicture(0, 0, d->mData[i * d->printPage + j]);
                    }
                }
            }
        }
    }
}

NS_SUGENTECH_END
