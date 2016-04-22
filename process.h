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

        //foreground
        QString getForegroundWindow();
        void changedToDesktop();
        void changedToExplorer();
        void changedToBrowser();
        void changedToOther();

        //signals/slots
        void connectToDial();
        void disconnectToDial();
        void disconnectAll();

        //log
        void logKeyInfo();

    private:
        QTimer *loop_timer;
        Display display;
        Motion motion;
        Operation * operation;
        Logger logger;
        QVariantMap config;
        QVariantMap config_display;
        QVariantMap config_gesture;
        QVariantMap config_operation;
        QString foreground_prev;

    signals:

        //gesture
        void grab(int status);
        void grab(double x, double y);
        void grab(double x, double y, bool status);
        void pinch(int status);
        void pinch(double x, double y);
        void pinch(double x, double y, bool status);
        void circle(int status);
        void circle_anti(int status);
        void swipe(int status);
        void screen_tap(int status);
        void screen_tap(double x, double y, int status);
        void key_tap(double x, double y, int status);
        void key_tap(int status);
        void setGesture(QString gesture_type);

        //dial
        void showDial(bool);
        void showDial(double x, double y);
        void updateDial(double x, double y);
        void doneDial();
        void choose_up(QString);
        void choose_down(QString);
        void choose_left(QString);
        void choose_right(QString);

    public slots:

        //main functions
        void start();
        void run();
        void stop();
        void restart();

        //dial
        void dial_up();
        void dial_down();
        void dial_left();
        void dial_right();

        //gesture
        void showGesture(QString gesture_type);
};

#endif // PROCESS_H
