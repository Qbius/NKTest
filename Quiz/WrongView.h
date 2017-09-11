#ifndef __QUIZ_WRONG_VIEW_H__
#define __QUIZ_WRONG_VIEW_H__

#include <vector>
#include <QtWidgets>

namespace View
{
    class Wrong : public QWidget
    {
        Q_OBJECT

        QTextEdit* mistakes;
        
    public:

        Wrong(QWidget* = nullptr);
        void insert_mistake(const QString&);

    signals:

        void mistake_session();
        void copy_mistakes(const QString&);
    };
}

#endif