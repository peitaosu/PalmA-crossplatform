#include "logger.h"
#include <iostream>
#include <QTime>
#include <QCoreApplication>
#include <QDate>

Logger::Logger(QObject *parent) : QObject(parent){
    //setup the default log file
    QDate date;
    QString current_date = date.currentDate().toString("yyyy-MM-dd");
    QString log_file_name = current_date + ".log";
    setLogDir("log");
    setLogFile("log\\" + log_file_name);
}

void Logger::setLogDir(QString dir){
    //create a dir for log save, default under applicationDirPath/log
    QString app_dir_path = QCoreApplication::applicationDirPath();
    log_dir.mkdir(app_dir_path + "\\" + dir);
}

void Logger::setLogFile(QString file){
    //setup log file location
    QString app_dir_path = QCoreApplication::applicationDirPath();
    log_file.setFileName(app_dir_path + "\\" + file);
    //if log file not exists, new one
    if(!log_file.exists()){
        //only new, not do anything
        log_file.open(QIODevice::WriteOnly | QIODevice::Text);
        log_file.close();
    }
}

void Logger::log(QString log_type, QString log_msg){
    //set the file open as R/W, text and append
    log_file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
    //append use QTextStream
    QTextStream in(&log_file);
    //log format is "hh:mm:ss[<log_type>]: <log_msg>", time is the currentTime
    QTime time;
    in<<time.currentTime().toString("hh:mm:ss")<<" ["<<log_type<<"]: "<<log_msg<<"\n";
    //log finished and close IO
    log_file.close();
}
