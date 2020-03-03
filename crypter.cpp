#include "crypter.h"

Crypter::Crypter(){
    languages["en"]=" abcdefghijklmnopqrstuvwxyz";
    languages["ukr"]=" абвгґдеєжзиіїйклмнопрстуфхцчшщьюя";
    wordLists["en"]="D:\\Projects\\Qt\\shyfr1\\wordsEN.txt";
    wordLists["ukr"]="D:\\Projects\\Qt\\shyfr1\\wordsUK.txt";
}

const QMap<QString,QString> Crypter::getLanguages(){
    return languages;
}

double Crypter::keyCalc(const int p, QList<double> keys){
    int length = keys.length();
    double res = 0;
    for(auto i : keys){
        res +=i*pow(p,--length);
    }
    return res;
}

const QString Crypter::encrypt(QString data, const QString language, QList<double> keys){
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<void>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&Crypter::encryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,keys));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&Crypter::encryptHelper,splitted.begin()+last,splitted.end(),language,keys));
        for(auto i : pool){
            i.waitForFinished();
        }
    }
    else{
        encryptHelper(splitted.begin(),splitted.end(),language,keys);
    }
    return splitted.join(" ");
}

const QString Crypter::encryptFromBites(QByteArray data, const QString language, QList<double> keys){
    QString res = "";
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QString>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&Crypter::encryptFromBitesHelper,data.begin()+last,data.begin()+last+data.length()/maxThreads,language,keys));
            last += data.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&Crypter::encryptFromBitesHelper,data.begin()+last,data.end(),language,keys));
        for(auto i : pool){
            res.append(i.result());
            i.waitForFinished();
        }
    }
    else{
        res = encryptFromBitesHelper(data.begin(),data.end(),language,keys);
    }
    return res;
}

const QString Crypter::decrypt(QString data, const QString language, QList<double> keys){
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<void>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&Crypter::decryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,keys));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&Crypter::decryptHelper,splitted.begin()+last,splitted.end(),language,keys));
        for(auto i : pool){
            i.waitForFinished();
        }
    }
    else{
        decryptHelper(splitted.begin(),splitted.end(),language,keys);
    }
    return splitted.join(" ");
}

QList<double> Crypter::decryptWithoutKey(QString data, QString output, const QString language){
    return decryptWithoutKeyHelper(data,output,languages[language]);
}

const QByteArray Crypter::decryptFromBites(QString data, const QString language, QList<double> keys){
    QByteArray res;
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QByteArray>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&Crypter::decryptFromBitesHelper,data.begin()+last,data.begin()+last+data.length()/maxThreads,language,keys));
            last += data.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&Crypter::decryptFromBitesHelper,data.begin()+last,data.end(),language,keys));
        for(auto i : pool){
            res.append(i.result());
            i.waitForFinished();
        }
    }
    else{
        res = decryptFromBitesHelper(data.begin(),data.end(),language,keys);
    }
    return res;
}

void Crypter::encryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, QList<double> keys){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    for(auto i=begin; i != end; i++){
        int cnt=0;
        for(auto j = i->begin(); j != i->end(); j++){
            cnt++;
            if(alphabet.contains(j->toLower())){
                bool isUpper = j->isUpper();
                *j = j->toLower();
                if(alphabet.contains(*j)){
                    *j = alphabet.at((alphabet.indexOf(*j)+static_cast<int>(keyCalc(cnt,keys))+maxNumber)%maxNumber);
                    if(isUpper)
                        *j = j->toUpper();
                }
            }
        }
    }
}

QString Crypter::encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, QList<double> keys){
    QString res = "";
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    QChar j;
    int cnt=0;
    for(auto i=begin; i != end; i++){
        j = alphabet.at((*i&0xf0)>>4);
        j = alphabet.at((alphabet.indexOf(j)+static_cast<int>(keyCalc(++cnt,keys))+maxNumber)%maxNumber);
        res += j;
        j = alphabet.at(*i&0xf);
        j = alphabet.at((alphabet.indexOf(j)+static_cast<int>(keyCalc(++cnt,keys))+maxNumber)%maxNumber);
        res += j;
    }
    return res;
}

void Crypter::decryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, QList<double> keys){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    for(auto i=begin; i != end; i++){
        int cnt=0;
        for(auto j = i->begin(); j != i->end(); j++){
            cnt++;
            if(alphabet.contains(j->toLower())){
                bool isUpper = j->isUpper();
                *j = j->toLower();
                if(alphabet.contains(*j)){
                    *j = alphabet.at((alphabet.indexOf(*j)-(static_cast<int>(keyCalc(cnt,keys))%maxNumber)+maxNumber)%maxNumber);
                    if(isUpper)
                        *j = j->toUpper();
                }
            }
        }
    }
}

QList<double> Crypter::decryptWithoutKeyHelper(const QString data, const QString output,QString alphabet){
    return {-1,-1,-1};
}

QByteArray Crypter::decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, QList<double> keys){
    QByteArray res;
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    char j = 0;
    auto i = begin;
    int cnt=0;
    while(true){
        if(i==end)
            break;
        *i = alphabet.at((alphabet.indexOf(*i)-(static_cast<int>(keyCalc(++cnt,keys))%maxNumber)+maxNumber)%maxNumber);
        j = alphabet.indexOf(*i);
        j = j << 4;
        i++;
        if(i==end)
            break;
        *i = alphabet.at((alphabet.indexOf(*i)-(static_cast<int>(keyCalc(++cnt,keys))%maxNumber)+maxNumber)%maxNumber);
        j += alphabet.indexOf(*i);
        res.append(j);
        i++;
    }
    return res;
}

