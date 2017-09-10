#ifndef __QUIZ_WRONG_VIEW_H__
#define __QUIZ_WRONG_VIEW_H__

#include <QtWidgets>

namespace View
{
    class Wrong : public QWidget
    {
        QTextEdit* mistakes;
        
    public:

        Wrong(QWidget* = nullptr);
        void insert_mistake(const QString&);
    };
}

#endif