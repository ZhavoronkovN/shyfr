#include "shiftcrypter.h"

ShiftCrypter::ShiftCrypter(){
    languages["en"]=" abcdefghijklmnopqrstuvwxyz";
    languages["ukr"]=" абвгґдеєжзиіїйклмнопрстуфхцчшщьюя";
    wordLists["en"]="D:\\Projects\\Qt\\shyfr1\\wordsEN.txt";
    wordLists["ukr"]="D:\\Projects\\Qt\\shyfr1\\wordsUK.txt";
}

const QMap<QString,QString> ShiftCrypter::getLanguages(){
    return languages;
}

const QString ShiftCrypter::encrypt(QString data, const QString language, const int key){
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<void>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&ShiftCrypter::encryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,key));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&ShiftCrypter::encryptHelper,splitted.begin()+last,splitted.end(),language,key));
        for(auto i : pool){
            i.waitForFinished();
        }
    }
    else{
        encryptHelper(splitted.begin(),splitted.end(),language,key);
    }
    return splitted.join(" ");
}

const QString ShiftCrypter::encryptFromBites(QByteArray data, const QString language, const int key){
    QString res = "";
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QString>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&ShiftCrypter::encryptFromBitesHelper,data.begin()+last,data.begin()+last+data.length()/maxThreads,language,key));
            last += data.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&ShiftCrypter::encryptFromBitesHelper,data.begin()+last,data.end(),language,key));
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

const QString ShiftCrypter::decrypt(QString data, const QString language, const int key){
    QList<QString> splitted = data.split(" ");
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<void>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&ShiftCrypter::decryptHelper,splitted.begin()+last,splitted.begin()+last+splitted.length()/maxThreads,language,key));
            last += splitted.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&ShiftCrypter::decryptHelper,splitted.begin()+last,splitted.end(),language,key));
        for(auto i : pool){
            i.waitForFinished();
        }
    }
    else{
        decryptHelper(splitted.begin(),splitted.end(),language,key);
    }
    return splitted.join(" ");
}

int ShiftCrypter::decryptWithoutKey(QString data, const QString language){
    if(!wordLists.contains(language))
        return -1;
    QFile file(wordLists[language]);
       if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
           return -1;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QString line;
    QSet<QString> wordList;
    while((line = stream.readLine())!=QString())
    {
        wordList.insert(line);
    }
    int key = -1;
    bool keyFound = false;
    float firstAttempt = 0.3;
    float step = 0.2;
    int maxNumber = languages[language].length();
    while(!keyFound){
        int last = 0;
        QList<QFuture<int>> threadPool;
        for(int i = 0; i < maxThreads-1; i++){
            threadPool.append(QtConcurrent::run(this,&ShiftCrypter::decryptWithoutKeyHelper,data,wordList,languages[language],
            QPair<QString::iterator,QString::iterator>(languages[language].begin()+last,languages[language].begin()+last+maxNumber/maxThreads),firstAttempt));
            last += maxNumber/maxThreads;
        }
        threadPool.append(QtConcurrent::run(this,&ShiftCrypter::decryptWithoutKeyHelper,data,wordList,languages[language],QPair<QString::iterator,QString::iterator>(languages[language].begin()+last,languages[language].end()),firstAttempt));
        for(auto i : threadPool){
            int maybeKey = i.result();
            if(maybeKey!=-1){
                if(key==-1){
                    key = maybeKey;
                    keyFound = true;
                }
                else if(maybeKey!=key){
                    keyFound = false;
                }
                else
                    keyFound = true;
            }
        }
        if(key == -1)
            break;
        firstAttempt+=step;
        if(firstAttempt>1)
            break;
    }
    wordList.clear();
    return key;
}

const QByteArray ShiftCrypter::decryptFromBites(QString data, const QString language, const int key){
    QByteArray res;
    if(data.length()>1000){
        int last = 0;
        QList<QFuture<QByteArray>> pool;
        for(int i = 0; i < maxThreads-1; i++){
            pool.append(QtConcurrent::run(this,&ShiftCrypter::decryptFromBitesHelper,data.begin()+last,data.begin()+last+data.length()/maxThreads,language,key));
            last += data.length()/maxThreads;
        }
        pool.append(QtConcurrent::run(this,&ShiftCrypter::decryptFromBitesHelper,data.begin()+last,data.end(),language,key));
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

void ShiftCrypter::encryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, const int key){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    for(auto i=begin; i != end; i++){
        for(auto j = i->begin(); j != i->end(); j++){
            if(alphabet.contains(j->toLower())){
                bool isUpper = j->isUpper();
                *j = j->toLower();
                if(alphabet.contains(*j)){
                    *j = alphabet.at((alphabet.indexOf(*j)+key+maxNumber)%maxNumber);
                    if(isUpper)
                        *j = j->toUpper();
                }
            }
        }
    }
}

QString ShiftCrypter::encryptFromBitesHelper(QByteArray::iterator begin, QByteArray::iterator end, const QString language, const int key){
    QString res = "";
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    QChar j;
    for(auto i=begin; i != end; i++){
        j = alphabet.at((*i&0xf0)>>4);
        j = alphabet.at((alphabet.indexOf(j)+key+maxNumber)%maxNumber);
        res += j;
        j = alphabet.at(*i&0xf);
        j = alphabet.at((alphabet.indexOf(j)+key+maxNumber)%maxNumber);
        res += j;
    }
    return res;
}

void ShiftCrypter::decryptHelper(QList<QString>::iterator begin, QList<QString>::iterator end, const QString language, const int key){
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    for(auto i=begin; i != end; i++){
        for(auto j = i->begin(); j != i->end(); j++){
            if(alphabet.contains(j->toLower())){
                bool isUpper = j->isUpper();
                *j = j->toLower();
                if(alphabet.contains(*j)){
                    *j = alphabet.at((alphabet.indexOf(*j)-key+maxNumber)%maxNumber);
                    if(isUpper)
                        *j = j->toUpper();
                }
            }
        }
    }
}

int ShiftCrypter::decryptWithoutKeyHelper(const QString data, QSet<QString> &wordList,QString alphabet,QPair<QString::iterator,QString::iterator> firstAndLastLetters, const float neededPercent){
    int key = -1;
    int maxNumber = alphabet.length();
    for(auto i = firstAndLastLetters.first; i != firstAndLastLetters.second; i++){
        int maybeKey = alphabet.indexOf(i)+1;
        int wordsCorrect = 0;
        auto splitted = data.split(" ");
        for(auto j : splitted){
            QString newWord = "";
            for(auto k : j){
                if(k.isLetter()&&alphabet.contains(k))
                {
                    bool isUpper = k.isUpper();
                    k = k.toLower();
                    k = alphabet.at((alphabet.indexOf(k)-maybeKey+maxNumber)%maxNumber);
                    if(isUpper)
                        k = k.toUpper();
                }
                newWord += k;
            }
            if(wordList.contains(newWord.toLower()))
            {
                wordsCorrect++;
            }
        }
        if(wordsCorrect>=splitted.length()*neededPercent){
            key = maybeKey;
            break;
        }
    }
    return key;
}

QByteArray ShiftCrypter::decryptFromBitesHelper(QString::iterator begin, QString::iterator end, const QString language, const int key){
    QByteArray res;
    QString alphabet = languages[language.toLower()];
    ushort maxNumber = alphabet.length();
    char j = 0;
    auto i = begin;
    while(true){
        if(i==end)
            break;
        *i = alphabet.at((alphabet.indexOf(*i)-key+maxNumber)%maxNumber);
        j = alphabet.indexOf(*i);
        j = j << 4;
        i++;
        if(i==end)
            break;
        *i = alphabet.at((alphabet.indexOf(*i)-key+maxNumber)%maxNumber);
        j += alphabet.indexOf(*i);
        res.append(j);
        i++;
    }
    return res;
}

