#include <QFile>
#include <QJsonDocument>
#include "event.h"
#include "error_code.h"

Event::Event(){

    //load virtual key code json file as QVariantMap
    QString vk_code_string;
    QFile vk_code_file;
    //file for macOS is under mac/
    vk_code_file.setFileName(":/mac/event_keyboard.json");
    vk_code_file.open(QIODevice::ReadOnly | QIODevice::Text);
    vk_code_string = vk_code_file.readAll();
    vk_code_file.close();
    //convert from QFile to QJsonDocument
    QJsonDocument vk_code_doc = QJsonDocument::fromJson(vk_code_string.toUtf8());
    //convert yo VariantMap
    virtual_key_code = vk_code_doc.toVariant().toMap();
}

Event::~Event(){
    
}

