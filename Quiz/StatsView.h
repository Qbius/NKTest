#ifndef __QUIZ_VIEW_STATS_H__
#define __QUIZ_VIEW_STATS_H__

#include <QtWidgets>

namespace View
{
    class Stats : public QWidget
    {
        Q_OBJECT

        unsigned right;
        unsigned count;
        unsigned total;

        QLabel* progress_label;
        QProgressBar* progress_bar;

    public:

        Stats(unsigned, QWidget* = nullptr);
        
        Stats& operator++()
        {
            ++right;
            if (count < total)
                ++count;
            update();
            return *this;
        }

        Stats& operator--()
        {
            if (count < total)
                ++count;
            update();
            return *this;
        }

    private:

        void update();
        void clear();
    };
}

#endif