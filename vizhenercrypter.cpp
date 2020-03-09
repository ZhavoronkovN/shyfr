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
    return encryptHelper(data.begin(),data.end(),language,key).toUtf8();
}


const QString VizhenerCrypter::decrypt(QString data, const QString language, const QString key){
	return encryptHelper(data.begin(),data.end(),language,key);
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



