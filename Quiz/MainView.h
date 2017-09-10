#ifndef __QUIZ_MAIN_VIEW_H__
#define __QUIZ_MAIN_VIEW_H__

#include <QtWidgets>

namespace View
{
    class Main : public QWidget
    {
        Q_OBJECT

        QLabel* label;

    public:

        Main(QWidget* = nullptr);
        void set_text(const QString&);
    };
}

#endif