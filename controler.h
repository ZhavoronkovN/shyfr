#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QClipboard>
#include <QGuiApplication>
#include <QPrintDialog>
#include <shiftcrypter.h>

class Controler : public QObject
{
    Q_OBJECT
public:
    explicit Controler(QObject *parent = nullptr);
    virtual ~Controler(){ delete crypter; }
    virtual bool validateData(QString data,const QString language,const int key);
    QString outputType;
signals:
    void signalValidateKeyError(const QString& message);
    void signalValidateInputError(const QString& message);
    void signalValidateInputWarning(const QString& message);
    void signalWorkFinished(const QString res);
    void signalFileReadFinished(const QString res);
    void signalOutputTypeChanged(const QString newOutput);
public slots:
    void slotWork(QString input, const QString action, const QString language, const QString type, const int key);
    void slotReadFile(const QString& fileurl);
    void slotCopy();
    void slotSaveFile(const QString& fileurl);
private:
    Crypter * crypter;
    QByteArray bin;
    QByteArray binOutput;
    QString strOutput;
};

#endif // CONTROLER_H
