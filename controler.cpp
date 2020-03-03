#include "controler.h"

Controler::Controler(QObject *parent) : QObject(parent),outputType(QString()),strOutput(QString())
{
    outputType = "none";
    vCrypter = new VizhenerCrypter();
}

bool Controler::validateData(QString data, const QString language,const QString key){
    if(!vCrypter->getLanguages().contains(language)){
        emit signalValidateInputError("Language is not supported");
        return false;
    }
    for(auto i : key){
        if(!vCrypter->getLanguages()[language].contains(i)){
            emit signalValidateKeyError("Unsupported characters in key.");
            return false;
        }
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
    if(!validateData(input,language,key))
        return;
    if(type == "normal"){
        outputType = "text";
        emit signalOutputTypeChanged(outputType);
        if(action.toLower()=="encode")
        {
            strOutput = vCrypter->encrypt(input,language,key);
        }
        else if(action.toLower()=="decode")
        {
            strOutput = vCrypter->decrypt(input,language,key);
        }
        emit signalWorkFinished(strOutput);
    }
    else{
        outputType = "binary";
        emit signalOutputTypeChanged(outputType);
        if(action.toLower()=="encode" && bin.size()>0)
        {
            outputType = "text";
            strOutput = vCrypter->encryptFromBites(bin,language,key);
            emit signalWorkFinished(strOutput);
        }
        else if(action.toLower()=="decode")
        {
            outputType = "binary";
            binOutput = vCrypter->decryptFromBites(input,language,key);
            emit signalWorkFinished(QString("Binary file"));
        }
    }
}

void Controler::slotWorkWithoutKeyStr(const QString input,const QString output,const QString lang){
    emit signalKeyFoundStr(vCrypter->decryptWithoutKey(input,output,lang));
}

void Controler::slotReadFile(const QString& fileurl){
    QFile file(fileurl);
       if (!file.open(QIODevice::ReadOnly)){
           emit signalValidateInputError("Cannot read file!");
       }
    if(fileurl.endsWith(".txt")||fileurl.endsWith(".encrypted")){
           QTextStream stream(&file);
           stream.setCodec("UTF-8");
           emit signalFileReadFinished(stream.readAll());
    }
    else{
        bin = file.readAll();
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
            QDataStream stream(&file);
            stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
            stream << binOutput;
        }
    }
}
