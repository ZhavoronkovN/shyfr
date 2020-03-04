#ifndef VIZHENERCRYPTER_H
#define VIZHENERCRYPTER_H

#include "QtConcurrent/QtConcurrent"

class VizhenerCrypter
{
public:
    VizhenerCrypter();
    virtual ~VizhenerCrypter(){}
    const QByteArray encrypt(QString data,const QString language, const QString key);
    const QString decrypt(QString data,const QString language, const QString key);
    const QMap<QString,QString> getLanguages();
private:
    QString encryptHelper(QList<QString>::iterator begin,QList<QString>::iterator end,const QString language, const QString key);
    QMap<QString,QString> wordLists;
    QMap<QString,QString> languages;
    const int maxThreads = 4;
};

#endif // VIZHENERCRYPTER_H
