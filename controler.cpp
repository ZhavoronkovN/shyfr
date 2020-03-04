#include "controler.h"

Controler::Controler(QObject *parent) : QObject(parent),outputType(QString()),strOutput(QString())
{
    outputType = "none";
    vCrypter = new VizhenerCrypter();
}

bool Controler::validateData(QString data, const QString language){
    if(!vCrypter->getLanguages().contains(language)){
        emit signalValidateInputError("Language is not supported");
        return false;
    }
    bool containsNeededLetters=false;
    bool containsUnneededLetters = false;
    for(auto i : data){
        if(i.isLetter()){
            if(vCrypter->getLanguages()[language].contains(i.toLower())){
                containsNeededLetters=true;
            }
            else{
                containsUnneededLetters = true;
            }
        }
    }
    if(!containsNeededLetters){
        emit signalValidateInputError("Letters of chosen language wasnt found. En(de)crypting unnecessary.");
        return false;
    }
    else if(containsUnneededLetters){
        emit signalValidateInputWarning("We have found letters of different language. Note, that these wont be en(de)crypted and left unchanged.");
    }
    return true;
}

void Controler::slotWorkStr(QString input, const QString action, const QString language, const QString type, const QString key){
    if(!validateData(input,language))
        return;
    if(type == "normal"){
        if(action.toLower()=="encode")
        {
            outputType = "binary";
            emit signalOutputTypeChanged(outputType);
            binOutput = vCrypter->encrypt(input,language,key);
            emit signalWorkFinished(QString("Binary file"));
        }
        else if(action.toLower()=="decode")
        {
            outputType = "text";
            emit signalOutputTypeChanged(outputType);
            strOutput = vCrypter->decrypt(bin,language,key);
            emit signalWorkFinished(strOutput);
        }
    }
}

void Controler::slotReadFile(const QString& fileurl){
    QFile file(fileurl);
       if (!file.open(QIODevice::ReadOnly)){
           emit signalValidateInputError("Cannot read file!");
       }
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    if(fileurl.endsWith(".txt")){
        emit signalFileReadFinished(stream.readAll());
    }
    else{
        bin = stream.readAll().toUtf8();
        emit signalFileReadFinished(QString("Binary file"));
    }
}

void Controler::slotCopy(){
    if(outputType =="text"){
        QClipboard * clip = QGuiApplication::clipboard();
        clip->setText(strOutput);
    }
}

void Controler::slotSaveFile(const QString& fileurl){
    QFile file(fileurl);
    if( file.open(QIODevice::ReadWrite))
    {
        if(outputType=="text"){
            QTextStream stream( &file );
            stream.setCodec("UTF-8");
            stream << strOutput;
        }
        else if(outputType=="binary"){
            QTextStream stream( &file );
            stream.setCodec("UTF-8");
            stream << binOutput;
        }
    }
}

void Controler::slotGenerateKey(const QString type,const int seed,const int length){
    srand(seed);
    QString res = "";
    if(type=="number"){
        for(int i = 0;i<length;i++)
            res+=QString::number(rand()%99+1);
    }
    else{
        QString alphabet = vCrypter->getLanguages()[type];
        int maxNumber = alphabet.length();
        for(int i = 0;i<length;i++){
            res+=alphabet.at(rand()%maxNumber);
        }
    }
    emit signalKeyGenerated(res);
}
