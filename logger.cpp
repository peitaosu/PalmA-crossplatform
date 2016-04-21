#include "logger.h"
#include <iostream>
#include <QTime>
Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::setLogDir(QString dir)
{
    log_dir.mkdir(dir);
}

void Logger::setLogFile(QString file)
{
    log_file.setFileName(file);
    if(!log_file.exists()){
        log_file.open(QIODevice::WriteOnly | QIODevice::Text);
        log_file.close();
    }
}

void Logger::log(QString log_type, QString log_msg)
{
    log_file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
    QTextStream in(&log_file);
    QTime time;
    in<<time.currentTime().toString("hh:mm:ss")<<" ["<<log_type<<"]: "<<log_msg<<"\n";
    log_file.close();
}
