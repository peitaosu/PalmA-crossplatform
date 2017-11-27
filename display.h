//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_DISPLAY_H
#define PALMA_DISPLAY_H

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

        //show and hide widgets
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

        //gesture display
        void setGestureDisplay(QString gesture_type);
        
        StatusWidget *widgetStatus();
        CursorWidget *widgetCursor();
        GestureWidget *widgetGesture();
        DialWidget *widgetDial();
        PalmWidget *widgetPalm();
        
    signals:

    public slots:
        


    private:
        StatusWidget *status_widget;
        CursorWidget *cursor_widget;
        GestureWidget *gesture_widget;
        DialWidget *dial_widget;
        PalmWidget  *palm_widget;
};

#endif // PALMA_DISPLAY_H
