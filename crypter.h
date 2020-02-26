#ifndef CRYPTER_H
#define CRYPTER_H
#include <QString>
#include <QList>
#include <QMap>

class Crypter
{
public:
    Crypter();
    virtual ~Crypter();
    virtual const QString encrypt(QString data,const QString language, const int key)=0;
    virtual const QString decrypt(QString data,const QString language,const int key)=0;
    virtual const QString encryptFromBites(QByteArray data, const QString language, const int key)=0;
    virtual const QByteArray decryptFromBites(QString data, const QString language, const int key)=0;
    virtual const QMap<QString,QString> getLanguages()=0;
    virtual int decryptWithoutKey(QString data,const QString language)=0;
protected:
    virtual void encryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const int key)=0;
    virtual void decryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const int key)=0;
    virtual int decryptWithoutKeyHelper(const QString data,QSet<QString>& wordList,QString alphabet,QPair<QString::iterator,QString::iterator> firstAndLastLetters, const float neededPercent)=0;
    virtual QString encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, const int key)=0;
    virtual QByteArray decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, const int key)=0;
    const int maxThreads = 4;
    QMap<QString,QString> languages;
};

#endif // CRYPTER_H
