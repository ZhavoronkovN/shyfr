#include "vizhenercrypter.h"

VizhenerCrypter::VizhenerCrypter(){
    languages["en"]=" abcdefghijklmnopqrstuvwxyz";
    languages["ukr"]=" абвгґдеєжзиіїйклмнопрстуфхцчшщьюя";
    wordLists["en"]="D:\\Projects\\Qt\\shyfr1\\wordsEN.txt";
    wordLists["ukr"]="D:\\Projects\\Qt\\shyfr1\\wordsUK.txt";
}

const QMap<QString,QString> VizhenerCrypter::getLanguages(){
    return languages;
}


const QString VizhenerCrypter::encrypt(QString data, const QString language, const QString key){
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<void>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&VizhenerCrypter::encryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,key));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&VizhenerCrypter::encryptHelper,splitted.begin()+last,splitted.end(),language,key));
        for(auto i : pool){
            i.waitForFinished();
        }
    }
    else{
            encryptHelper(splitted.begin(),splitted.end(),language,key);
    }
    return splitted.join(" ");
}

const QString VizhenerCrypter::encryptFromBites(QByteArray data, const QString language, const QString key){
    QString res = "";
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QString>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&VizhenerCrypter::encryptFromBitesHelper,data.begin()+last,data.begin()+last+data.length()/maxThreads,language,key));
            last += data.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&VizhenerCrypter::encryptFromBitesHelper,data.begin()+last,data.end(),language,key));
        for(auto i : pool){
            res.append(i.result());
            i.waitForFinished();
        }
    }
    else{
        res = encryptFromBitesHelper(data.begin(),data.end(),language,key);
    }
    return res;
}

const QString VizhenerCrypter::decrypt(QString data, const QString language, const QString key){
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<void>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&VizhenerCrypter::decryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,key));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&VizhenerCrypter::decryptHelper,splitted.begin()+last,splitted.end(),language,key));
        for(auto i : pool){
            i.waitForFinished();
        }
    }
    else{
        decryptHelper(splitted.begin(),splitted.end(),language,key);
    }
    return splitted.join(" ");
}

const QString VizhenerCrypter::decryptWithoutKey(QString data, QString output, const QString language){
    return decryptWithoutKeyHelper(data,output,languages[language]);
}

const QByteArray VizhenerCrypter::decryptFromBites(QString data, const QString language, const QString key){
    QByteArray res;
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QByteArray>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&VizhenerCrypter::decryptFromBitesHelper,data.begin()+last,data.begin()+last+data.length()/maxThreads,language,key));
            last += data.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&VizhenerCrypter::decryptFromBitesHelper,data.begin()+last,data.end(),language,key));
        for(auto i : pool){
            res.append(i.result());
            i.waitForFinished();
        }
    }
    else{
        res = decryptFromBitesHelper(data.begin(),data.end(),language,key);
    }
    return res;
}

void VizhenerCrypter::encryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, const QString key){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    for(auto i=begin; i != end; i++){
        int cnt=-1;
        for(auto j = i->begin(); j != i->end(); j++){
            if(alphabet.contains(j->toLower())){
                cnt++;
                int keyLength = key.length();
                bool isUpper = j->isUpper();
                *j = j->toLower();
                *j = alphabet.at((alphabet.indexOf(*j)+alphabet.indexOf(key.at(cnt%keyLength)))%maxNumber);
                if(isUpper)
                    *j = j->toUpper();
            }
        }
    }
}

QString VizhenerCrypter::encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, const QString key){
    QString res = "";
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    QChar j;
    int cnt=0;
    for(auto i=begin; i != end; i++){
        j = alphabet.at((*i&0xf0)>>4);
        j = alphabet.at((alphabet.indexOf(j)+alphabet.indexOf(key.at(++cnt%key.length()+1)+maxNumber))%maxNumber);
        res += j;
        j = alphabet.at(*i&0xf);
        j = alphabet.at((alphabet.indexOf(j)+alphabet.indexOf(key.at(++cnt%key.length()+1)+maxNumber))%maxNumber);
        res += j;
    }
    return res;
}

void VizhenerCrypter::decryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, const QString key){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    for(auto i=begin; i != end; i++){
        int cnt=-1;
        for(auto j = i->begin(); j != i->end(); j++){
            if(alphabet.contains(j->toLower())){
                cnt++;
                int keyLength = key.length();
                bool isUpper = j->isUpper();
                *j = j->toLower();
                *j = alphabet.at((alphabet.indexOf(*j)+maxNumber-alphabet.indexOf(key.at(cnt%keyLength)))%maxNumber);
                if(isUpper)
                    *j = j->toUpper();
            }
        }
    }
}

const QString VizhenerCrypter::decryptWithoutKeyHelper(const QString data, const QString output,QString alphabet){
    auto i = data.begin();
    auto j = output.begin();
    int maxNumber = alphabet.length();
    QString key ="";
    int keyLength = 0;
    for(;i!=data.end()||j!=output.end();i++,j++){
        if(alphabet.contains(*i)&&alphabet.contains(*j))
        {
            key.append(alphabet.at((alphabet.indexOf(*i)+maxNumber-alphabet.indexOf(*j))%maxNumber));
            if(keyLength>0){
                if(key[key.length()-1]!=key.at(key.length()%keyLength))
                    keyLength = key.length();
            }
            else{
                keyLength++;
            }
        }
    }
    return key.section("",0,keyLength-1);
}

QByteArray VizhenerCrypter::decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, const QString key){
    QByteArray res;
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    char j = 0;
    auto i = begin;
    int cnt=0;
    while(true){
        if(i==end)
            break;
        *i = alphabet.at((alphabet.indexOf(*i)-alphabet.indexOf(key.at(++cnt%key.length()+1)+maxNumber))%maxNumber);
        j = alphabet.indexOf(*i);
        j = j << 4;
        i++;
        if(i==end)
            break;
        *i = alphabet.at((alphabet.indexOf(*i)-alphabet.indexOf(key.at(++cnt%key.length()+1)+maxNumber))%maxNumber);
        j += alphabet.indexOf(*i);
        res.append(j);
        i++;
    }
    return res;
}

