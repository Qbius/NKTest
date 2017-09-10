#ifndef __QUIZ_VIEW_INPUT_H__
#define __QUIZ_VIEW_INPUT_H__

#include <QtWidgets>
#include <string>

namespace View
{
    namespace details
    {
        class input_frame;
    }

    class Input : public QWidget
    {
        Q_OBJECT 

        details::input_frame* one_edit;
        details::input_frame* two_edits;

        QStackedLayout* layout;

    public:

        Input(QWidget* = nullptr);
        
        enum input_type {
            KANJI,
            WORD
        };

        void switch_to(input_type, bool = false);

    signals:

        void answers(std::vector<QString>&);

    };

    namespace details
    {
        class input_frame : public QWidget
        {
            Q_OBJECT

            std::vector<QLineEdit*> edits;

        public:

            input_frame(const std::vector<QString>&, QWidget* = nullptr);
            void disable_edit(unsigned);

        private:

            void check_answers();

        signals:

            void answers(std::vector<QString>&);
        };
    }
}

#endif

