#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QTimer>
#include "display.h"
#include "motion.h"
#include "operation.h"
#include "logger.h"

class Process : public QObject
{
        Q_OBJECT
    public:
        explicit Process(QObject *parent = 0);
        void init(int argc, char* argv[]);


    private:
        QTimer *loop_timer;
        Display display;
        Motion motion;
        Operation operation;
        Logger logger;
    signals:

    public slots:
        //void init();
        void start();
        void run();
        void stop();
        void restart();
};

#endif // PROCESS_H
