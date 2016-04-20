#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include "widget_status.h"
#include "widget_cursor.h"
#include "widget_gesture.h"
#include "widget_dial.h"
#include "widget_palm.h"

class Display : public QObject
{
        Q_OBJECT
    public:
        explicit Display(QObject *parent = 0);
        void showStatus();
        void hideStatus();
        void showGesture();
        void hideGesture();
        void showPalm();
        void hidePalm();
        void showDial(QString dial_type, double x, double y);
        void hideDial();
        void updateDial(double x, double y);
        void updateCursor(double x, double y);
        void updateStatus(int status_code);
        void setGestureDisplay(QString gesture_type);
        
        void widgetStatus();
        void widgetCursor();
        void widgetGesture();
        void widgetDial();
        void widgetPalm();
        
    signals:

    public slots:
        


    private:
        StatusWidget *status_widget;
        CursorWidget *cursor_widget;
        GestureWidget *gesture_widget;
        DialWidget *dial_widget;
        PalmWidget  *palm_widget;
};

#endif // DISPLAY_H
