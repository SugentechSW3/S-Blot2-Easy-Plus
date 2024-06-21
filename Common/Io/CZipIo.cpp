
#include "CZipIo.h"

#ifdef USE_QT

#include <QIODevice>
#include <QFile>
#include <QByteArray>

NS_SUGENTECH_BEGIN

CZipIo::CZipIo(QObject *parent) :
    QObject(parent)
{

}

CZipIo::~CZipIo()
{

}

void CZipIo::zip(const QString& fileFullPath , const QString& zipFullPath)
{
    QFile infile(fileFullPath);
    QFile outfile(zipFullPath);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressed_data = infile.readAll();
    QByteArray compressed_data = qCompress(uncompressed_data, 9);
    outfile.write(compressed_data);
    infile.close();
    outfile.close();
}

void CZipIo::unZip(const QString& zipFullPath , const QString& unZipFullPath)
{
    QFile infile(zipFullPath);
    QFile outfile(unZipFullPath);
    infile.open(QIODevice::ReadOnly);
    outfile.open(QIODevice::WriteOnly);
    QByteArray uncompressed_data = infile.readAll();
    QByteArray compressed_data = qUncompress(uncompressed_data);
    outfile.write(compressed_data);
    infile.close();
    outfile.close();
}

NS_SUGENTECH_END

#endif /* END OF USE QT */
