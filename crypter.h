#ifndef CRYPTER_H
#define CRYPTER_H
#include "QtConcurrent/QtConcurrent"
#include <cmath>

class Crypter
{
public:
    Crypter();
    virtual ~Crypter(){}
    const QString encrypt(QString data,const QString language, QList<double> keys);
    const QString decrypt(QString data,const QString language, QList<double> keys);
    const QString encryptFromBites(QByteArray data, const QString language, QList<double> keys);
    const QByteArray decryptFromBites(QString data, const QString language, QList<double> keys);
    const QMap<QString,QString> getLanguages();
    virtual QList<double> decryptWithoutKey(QString data,QString output,const QString language);
private:
    void encryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, QList<double> keys);
    void decryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, QList<double> keys);
    virtual QList<double> decryptWithoutKeyHelper(const QString data,const QString output,QString alphabet);
    QString encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, QList<double> keys);
    QByteArray decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, QList<double> keys);
    QMap<QString,QString> wordLists;
    QMap<QString,QString> languages;
    const int maxThreads = 4;
    double keyCalc(const int p,QList<double> keys);
};

#endif //CRYPTER_H
