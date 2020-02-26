#ifndef SHIFTCRYPTER_H
#define SHIFTCRYPTER_H
#include "crypter.h"
#include "QtConcurrent/QtConcurrent"

class ShiftCrypter : public Crypter
{
public:
    ShiftCrypter();
    ~ShiftCrypter(){}
    const QString encrypt(QString data,const QString language, const int key);
    const QString decrypt(QString data,const QString language,const int key);
    const QString encryptFromBites(QByteArray data, const QString language, const int key);
    const QByteArray decryptFromBites(QString data, const QString language, const int key);
    const QMap<QString,QString> getLanguages();
    virtual int decryptWithoutKey(QString data,const QString language);
private:
    void encryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const int key);
    void decryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const int key);
    virtual int decryptWithoutKeyHelper(const QString data,QSet<QString>& wordList,QString alphabet,QPair<QString::iterator,QString::iterator> firstAndLastLetters,const float neededPercent);
    QString encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, const int key);
    QByteArray decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, const int key);
    QMap<QString,QString> wordLists;
};

#endif // SHIFTCRYPTER_H
