//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_LOGGER_H
#define PALMA_LOGGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
class Logger : public QObject
{
        Q_OBJECT
    public:
        explicit Logger(QObject *parent = 0);
        void setLogDir(QString dir);
        void setLogFile(QString file);
        void log(QString log_type, QString log_msg);

    private:
        QFile log_file;
        QDir log_dir;

};

#endif // PALMA_LOGGER_H
