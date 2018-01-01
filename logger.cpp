//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


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

/*
 * Logger: Set Log Dir
 * Input: QString dir path for file save
 * Return: NONE
 */
void Logger::setLogDir(QString dir){
    //create a dir for log save, default under applicationDirPath/log
    QString app_dir_path = QCoreApplication::applicationDirPath();
    log_dir.mkdir(app_dir_path + "\\" + dir);
}

/*
 * Logger: Set Log File
 * Input: QString file name for log
 * Return: NONE
 */
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

/*
 * Logger: Log Information
 * Input: QString log type: [INFO],[ERROR],[DEBUG],etc. QString log message
 * Return: NONE
 */
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
