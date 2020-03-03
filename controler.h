#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QClipboard>
#include <QGuiApplication>
#include <QPrintDialog>
#include <crypter.h>
#include <vizhenercrypter.h>

class Controler : public QObject
{
    Q_OBJECT
public:
    explicit Controler(QObject *parent = nullptr);
    virtual ~Controler(){ delete vCrypter;}
    virtual bool validateData(QString data,const QString language,const QString key);
    QString outputType;
signals:
    void signalValidateKeyError(const QString& message);
    void signalValidateInputError(const QString& message);
    void signalValidateInputWarning(const QString& message);
    void signalKeyFound(const QList<double> keys);
    void signalKeyFoundStr(const QString key);
    void signalWorkFinished(const QString res);
    void signalFileReadFinished(const QString res);
    void signalOutputTypeChanged(const QString newOutput);
public slots:
    void slotWorkStr(QString input, const QString action, const QString language, const QString type, const QString key);
    void slotWorkWithoutKeyStr(const QString input,const QString output,const QString lang);
    void slotReadFile(const QString& fileurl);
    void slotCopy();
    void slotSaveFile(const QString& fileurl);
private:
    VizhenerCrypter * vCrypter;
    QByteArray bin;
    QByteArray binOutput;
    QString strOutput;
};

#endif // CONTROLER_H
