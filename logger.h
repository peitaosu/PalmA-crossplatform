#ifndef LOGGER_H
#define LOGGER_H

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

#endif // LOGGER_H
