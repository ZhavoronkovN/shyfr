#ifndef VIZHENERCRYPTER_H
#define VIZHENERCRYPTER_H

#include "QtConcurrent/QtConcurrent"

class VizhenerCrypter
{
public:
    VizhenerCrypter();
    virtual ~VizhenerCrypter(){}
    const QString encrypt(QString data,const QString language, const QString key);
    const QString decrypt(QString data,const QString language, const QString key);
    const QString encryptFromBites(QByteArray data, const QString language, const QString key);
    const QByteArray decryptFromBites(QString data, const QString language, const QString key);
    const QMap<QString,QString> getLanguages();
    virtual const QString decryptWithoutKey(QString data,QString output,const QString language);
private:
    void encryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const QString key);
    void decryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const QString key);
    virtual const QString decryptWithoutKeyHelper(const QString data,const QString output,QString alphabet);
    QString encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, const QString key);
    QByteArray decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, const QString key);
    QMap<QString,QString> wordLists;
    QMap<QString,QString> languages;
    const int maxThreads = 4;
};

#endif // VIZHENERCRYPTER_H
