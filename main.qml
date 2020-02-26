import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import Controler 1.0

Window {
    id : main
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Шифр зсуву")
    Button{
        anchors{
            right: parent.right
            top : parent.top
            margins: 5
        }
        height: 50
        width: 50
        text: "?"
        font.pixelSize: height/2
        onClicked: dialogAbout.open()
    }
    Label{
        anchors{
            left: scrollInput.left
            bottom: scrollInput.top
            bottomMargin: 10
        }
        text: "Input text : "
        font.pixelSize: 20
    }
    Rectangle{
        id : scrollInput
        anchors{
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: 50
            leftMargin: 50
            rightMargin: 140
            bottomMargin: 20
        }
        height: main.height/3-60
        border.color: "lightgray"
        ScrollView{
            anchors.fill: parent
            TextArea{
                id : textInput
                font.pixelSize: 20
                placeholderText: "Enter text to start"
                wrapMode: Text.Wrap
            }
        }
    }
    Rectangle{
        id : rectTools
        anchors{
            left: parent.left
            right: parent.right
            top: scrollInput.bottom
            bottom: scrollOutput.top
            leftMargin: 50
            rightMargin: 140
            topMargin: 5
            bottomMargin: 20
        }
        Rectangle{
            id : rectFile
            anchors{
                left: parent.left
                right: parent.right
                top : parent.top
            }
            height: parent.height/4
            Label{
                id : labelSelectFile
                anchors{
                    left: parent.left
                    top : parent.top
                    bottom: parent.bottom
                }
                text: "Or select file : "
                font.pixelSize: 20
            }
            ScrollView{
                anchors{
                    top : parent.top
                    bottom: parent.bottom
                    left: labelSelectFile.right
                    right: buttonFileClear.left
                    leftMargin: 5
                    rightMargin: 5
                }
                clip : true
                TextEdit{
                    id : editSelectFile
                    anchors.fill: parent
                    font.pixelSize: 20
                    readOnly: true;
                }
            }
            Button{
                id: buttonFileClear
                anchors{
                    right: buttonFileChoose.left
                    rightMargin: 3
                }
                text : "X"
                width: height
                height: buttonFileChoose.height
                font.pixelSize: 20
                onClicked: {
                    editSelectFile.text = "";
                    textInput.text = "";
                    textInput.readOnly = false;
                    checkDontKnowKey.enabled = radioActionDecode.checked;
                }
            }
            Button{
                id : buttonFileChoose
                anchors{
                    right: parent.right
                }
                text : "Choose"
                width: 100
                height: Math.min(50,parent.height)
                font.pixelSize: 20
                onClicked: {
                    dialogFile.open()
                }
            }
        }
        Rectangle{
            id : rectSettings
            anchors{
                left: parent.left
                right: parent.right
                top : rectFile.bottom
            }
            height: parent.height/2
            Rectangle{
                id : rectSettingLanguage
                anchors{
                    top : parent.top
                    bottom: parent.bottom
                    left: parent.left
                }
                width: parent.width/2
                Label{
                    id : labelSettingsLanguage
                    anchors{
                        left : parent.left
                    }
                    text : "Language : "
                    font.pixelSize: 17
                }
                RadioButton{
                    id : radioLanguageUkr
                    anchors{
                        left : labelSettingsLanguage.right
                        leftMargin: 20
                        top : parent.top
                    }
                    text: "Ukrainian"
                    font.pixelSize: 17
                }
                RadioButton{
                    id : radioLanguageEn
                    anchors{
                        left : labelSettingsLanguage.right
                        leftMargin: 20
                        top : radioLanguageUkr.bottom
                    }
                    text : "English"
                    font.pixelSize: 17
                    checked: true
                }
            }
            Rectangle{
                id : rectSettingAction
                anchors{
                    top : parent.top
                    bottom: parent.bottom
                    right: parent.right
                }
                width: parent.width/2
                Label{
                    id : labelSettingsAction
                    anchors{
                        left : parent.left
                    }
                    text : "Action : "
                    font.pixelSize: 17
                }
                RadioButton{
                    id : radioActionEncode
                    anchors{
                        left : labelSettingsAction.right
                        leftMargin: 20
                        top : parent.top
                    }
                    text: "Encode"
                    font.pixelSize: 17
                    checked: true
                    onClicked: {
                        checkDontKnowKey.enabled = false;
                        checkDontKnowKey.checked = false;
                    }
                }
                RadioButton{
                    id : radioActionDecode
                    anchors{
                        left : labelSettingsAction.right
                        leftMargin: 20
                        top : radioActionEncode.bottom
                    }
                    text : "Decode"
                    font.pixelSize: 17
                    onClicked: {
                        if(textInput.readOnly == false)
                            checkDontKnowKey.enabled = true;
                    }
                }
            }
        }
        Rectangle{
            id : rectKey
            anchors{
                left: parent.left
                right : parent.right
                top : rectSettings.bottom
            }
            height: parent.height/4
            Label{
                id : labelKey
                anchors{
                    left: parent.left
                    top : parent.top
                    bottom: parent.bottom
                }
                text : "Select key : "
                font.pixelSize: 17
            }
            Label{
                id : labelKeyPicker
                anchors{
                    left : labelKey.right
                    top : parent.top
                    leftMargin: sliderKey.width/2-10
                }
                text : sliderKey.value.toString()
                font.pixelSize: 17
            }
            Slider{
                id : sliderKey
                anchors{
                    left : labelKey.right
                    top : labelKeyPicker.bottom
                    bottom: parent.bottom
                }
                from: 1
                value: 1
                to : radioLanguageEn.checked?26:33
                stepSize: 1
            }
            CheckBox{
                id : checkDontKnowKey
                anchors{
                    right: parent.right
                    top : parent.top
                    bottom: parent.bottom
                }
                text : "I dont know key"
                font.pixelSize: 17
                enabled: false
            }
        }
    }
    Rectangle{
        id : scrollOutput
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 90
            leftMargin: 50
            rightMargin: 140
        }
        height: parent.height/3-60
        border.color: "lightgray"
        ScrollView{
            anchors.fill: parent
            TextArea{
                id : textOutput
                font.pixelSize: 20
                placeholderText: "Here will be final text"
                wrapMode: Text.Wrap
                readOnly: true
            }
        }
    }
    Button{
        id : buttonCopy
        anchors{
            top : scrollOutput.top
            left: scrollOutput.right
            right: parent.right
            leftMargin: 20
            rightMargin: 20
            topMargin: 0
        }
        enabled: textOutput.text
        width: 100
        height: Math.min(50,scrollOutput.height/3-10)
        text: "Copy"
        font.pixelSize: 20
        onClicked: controler.slotCopy();
    }
    Button{
        id : buttonPrint
        anchors{
            top : buttonCopy.bottom
            left: scrollOutput.right
            right: parent.right
            leftMargin: 20
            rightMargin: 20
            topMargin: 15
        }
        enabled: textOutput.text
        width: 100
        height: Math.min(50,scrollOutput.height/3-10)
        text: "Print"
        font.pixelSize: 20
        onClicked: dialogPrint.open();
    }
    Button{
        id : buttonSave
        anchors{
            top : buttonPrint.bottom
            left: scrollOutput.right
            right: parent.right
            leftMargin: 20
            rightMargin: 20
            topMargin: 15
        }
        enabled: textOutput.text
        width: 100
        height: Math.min(50,scrollOutput.height/3-10)
        text: "Save"
        font.pixelSize: 20
        onClicked: dialogFileSave.open();
    }
    Button{
        anchors{
            bottom: parent.bottom
            right: parent.right
            margins: 20
        }
        width: 100
        height: 50
        text: "Start"
        font.pixelSize: 20
        enabled: textInput.text
        onClicked : {
            var lang = radioLanguageEn.checked?"en":"ukr";
            var action = radioActionDecode.checked?"decode":"encode";
            var type = textInput.readOnly == true?"binary":"normal";
            var key = checkDontKnowKey.checked == true?-1:sliderKey.value;
            controler.slotWork(textInput.text,action,lang,type,key);
        }
    }
    Button{
        anchors{
            bottom: parent.bottom
            left: parent.left
            margins: 20
        }
        width: 100
        height: 50
        text: "Exit"
        font.pixelSize: 20
        onClicked: Qt.quit();
    }
    MessageDialog{
        id : dialogAbout
        icon: StandardIcon.Question
        title: "About developer"
        text : "Developed by Zhavoronkov Nikita, student of group PMI-33 for \"Mathematical fundaments of Cryptology\" somewhere in February."
        detailedText: "        Developer name, group and subject are not registered trademarks.
        If you want, you could register them.
        In fact, there are no information about subject, but just dont try to register February.
        No rights are reserved. What is more, no rights even exist."
    }
    MessageDialog{
        id : dialogError
        icon : StandardIcon.Critical
        title: "An error occured!"
    }
    MessageDialog{
        id : dialogWarning
        icon : StandardIcon.Warning
        title : "Warning!"
    }
    MessageDialog{
        id : dialogPrint
        icon: StandardIcon.NoIcon
        text: "File was send to printer"
    }

    FileDialog{
        id : dialogFile
        title: "Select file"
        folder: shortcuts.home
        nameFilters: [ "Text files (*.txt)","All files (*)","Encrypted files (*.encrypted)"]
        onAccepted: {
            var path = fileUrl.toString();
            path = path.replace(/^(file:\/{3})/,"");
            editSelectFile.text = decodeURIComponent(path);
            var splitted = path.split(".");
            if(!splitted.length>1||splitted[splitted.length-1]!=="txt"){
                textInput.readOnly = true;
                checkDontKnowKey.enabled = false;
                checkDontKnowKey.checked = false;
            }
            controler.slotReadFile(editSelectFile.text);
        }
    }
    FileDialog{
        id: dialogFileSave
        title: "Save file"
        folder: shortcuts.home
        selectExisting: false
        onAccepted:{
            var path = fileUrl.toString();
            path = path.replace(/^(file:\/{3})/,"");
            controler.slotSaveFile(path);
        }
    }
    Controler{
        id: controler
        onSignalWorkFinished: {
            textOutput.text=res;
        }
        onSignalValidateInputError: {
            dialogError.text = message;
            dialogError.open();
        }
        onSignalValidateKeyError: {
            dialogError.text = message;
            dialogError.open();
        }
        onSignalValidateInputWarning: {
            dialogWarning.text = message;
            dialogWarning.open();
        }
        onSignalFileReadFinished: {
            textInput.text=res;
        }
        onSignalOutputTypeChanged: {
            if(newOutput==="text"){
                buttonCopy.enabled = true;
                buttonPrint.enabled = true;
                dialogFileSave.nameFilters = ["Text file (*.txt)","All files (*)"];
            }
            else{
                buttonCopy.enabled = false;
                buttonPrint.enabled = false;
                if(radioActionEncode.checked)
                    dialogFileSave.nameFilters = ["Encrypted file (*.encrypted)","All files (*)"];
                else
                    dialogFileSave.nameFilters = ["All files (*)"];
            }
        }
    }
}
