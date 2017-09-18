#ifndef __QUIZ_VIEW_INPUT_H__
#define __QUIZ_VIEW_INPUT_H__

#include <QtWidgets>
#include <string>
#include <map>

namespace View
{
    namespace details
    {
        class input_frame;
    }

    class Input : public QWidget
    {
        Q_OBJECT

    public:

        enum input_type {
            KANJI_READING,
            WORDS_READING,
            KANJI_WRITING,
            WORDS_WRITING
        };

    private:

        std::map<input_type, details::input_frame*> input_frames;

        QStackedLayout* layout;

        QTimer* timer;
        int count;

    public:

        Input(QWidget* = nullptr);
        void switch_to(input_type, bool = false);
        void disable();

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
            void disable();
            bool enabled();
            void check_answers();

        signals:

            void answers(std::vector<QString>&);
        };
    }
}

#endif

