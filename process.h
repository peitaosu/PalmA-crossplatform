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
        QString getForegroundWindow();

    private:
        QTimer *loop_timer;
        Display display;
        Motion motion;
        Operation operation;
        Logger logger;
        QVariantMap config;
        QVariantMap config_display;
        QVariantMap config_gesture;
    signals:
        void grab(int status);
        void grab(double x, double y);
        void grab(double x, double y, int status);
        void pinch(int status);
        void pinch(double x, double y);
        void pinch(double x, double y, int status);
        void circle(int status);
        void circle_anti(int status);
        void swipe(int status);
        void screen_tap(int status);
        void screen_tap(double x, double y, int status);
        void key_tap(double x, double y, int status);
        void key_tap(int status);
        void setGesture(QString gesture_type);

    public slots:
        //void init();
        void start();
        void run();
        void stop();
        void restart();

        void mouse(double x, double y);
        void mouse(double x, double y, int event);

        void showGesture(QString gesture_type);
};

#endif // PROCESS_H
