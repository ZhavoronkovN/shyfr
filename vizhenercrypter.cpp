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


const QByteArray VizhenerCrypter::encrypt(QString data, const QString language, const QString key){
    QString res;
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QString>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&VizhenerCrypter::encryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,key));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&VizhenerCrypter::encryptHelper,splitted.begin()+last,splitted.end(),language,key));
        for(auto i : pool){
            res+=i.result();
            i.waitForFinished();
        }
    }
    else{
            res = encryptHelper(splitted.begin(),splitted.end(),language,key);
    }
    return res.toUtf8();
}


const QString VizhenerCrypter::decrypt(QString data, const QString language, const QString key){
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


QString VizhenerCrypter::encryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, const QString key){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    QString res;
    for(auto i=begin; i != end; i++){
        int cnt=-1;
        for(auto j = i->begin(); j != i->end(); j++){
            if(alphabet.contains(j->toLower())){
                cnt++;
                int keyLength = key.length();
                *j = j->toLower();
                if(key.at(cnt%keyLength).isDigit()){
                    if(key.at((cnt+1)%keyLength).isDigit()){
                        res+=j->unicode()^(QString(key.at(cnt%keyLength))+QString(key.at((cnt+1)%keyLength))).toUShort();
                        cnt++;
                    }
                    else{
                        res+=j->unicode()^QString(key.at(cnt%keyLength)).toUShort();
                    }
                }
                else{
                    res+=j->unicode()^key.at(cnt%keyLength).unicode();
                }
            }
        }
    }
    return res;
}



