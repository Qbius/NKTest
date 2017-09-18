#include "InputView.h"
#include "Settings.h"
#include <chrono>

namespace View
{
    Input::Input(QWidget* parent) : 
        QWidget(parent), 
        layout(new QStackedLayout),       
        timer(new QTimer{ this }),
        count(0)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        input_frames.insert(std::make_pair(KANJI_READING, new details::input_frame{ { "音読み", "訓読み" }, this }));
        input_frames.insert(std::make_pair(WORDS_READING, new details::input_frame{ { "読み" }, this }));
        input_frames.insert(std::make_pair(KANJI_WRITING, new details::input_frame{ { "漢字" }, this }));
        input_frames.insert(std::make_pair(WORDS_WRITING, new details::input_frame{ { "Word" }, this }));

        for (const auto& input_frame : input_frames)
        {
            layout->addWidget(input_frame.second);
            connect(input_frame.second, &details::input_frame::answers, this, &Input::answers);
        }
    
        connect(timer, &QTimer::timeout, [this]() {
            if (++count == settings().timer)
            {
                auto* p = dynamic_cast<details::input_frame*>(layout->currentWidget());
                p->check_answers();
                p->enabled() ? count = 0 : timer->stop();
            }
        });
        timer->start(1000);

        setLayout(layout);
    }

    void Input::switch_to(input_type type, bool disable_kunyomi)
    {
        if (disable_kunyomi && type == KANJI_READING)
            input_frames[KANJI_READING]->disable_edit(1);
        layout->setCurrentIndex(static_cast<int>(type));
    }

    void Input::disable()
    {
        for (const auto& input_frame : input_frames)
        {
            input_frame.second->disable();
        }
    }

    namespace details
    {
        input_frame::input_frame(const std::vector<QString>& label_texts, QWidget* parent)
            : QWidget(parent)
        {
            QVBoxLayout* layout = new QVBoxLayout;
            QSpacerItem* topSpacer = new QSpacerItem{ 0, 1, QSizePolicy::Minimum, QSizePolicy::Minimum };
            QSpacerItem* bottomSpacer = new QSpacerItem{ 0, 1, QSizePolicy::Minimum, QSizePolicy::Minimum };

            layout->addItem(topSpacer);
            for (const auto& text : label_texts)
            {
                QLabel* label = new QLabel{ text, this };

                QFont font;
                font.setPointSize(36);
                label->setFont(font);
                label->setAlignment(Qt::AlignCenter);

                edits.push_back(new QLineEdit(this));
                connect(edits.back(), &QLineEdit::returnPressed, this, &input_frame::check_answers);

                layout->addWidget(label, Qt::AlignHCenter);
                layout->addWidget(edits.back(), Qt::AlignHCenter);
            }
            setLayout(layout);
            layout->addItem(bottomSpacer);
        }

        void input_frame::disable_edit(unsigned index)
        {
            if (index < edits.size())
            {
                edits[index]->setDisabled(true);
            }
        }

        void input_frame::disable()
        {
            for (const auto edit : edits)
            {
                edit->setDisabled(true);
            }
        }

        bool input_frame::enabled()
        {
            for (const auto edit : edits)
            {
                if (edit->isEnabled())
                    return true;
            }
            return false;
        }

        void input_frame::check_answers()
        {
            std::vector<QString> answers_list;
            for (const auto edit : edits)
            {
                if (edit->isEnabled())
                {
                    answers_list.push_back(edit->text());
                    edit->setText("");
                }
                else
                {
                    edit->setEnabled(true);
                }
            }
            emit answers(answers_list);
            edits.front()->setFocus();
        }
    }
}
