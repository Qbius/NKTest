#include "InputView.h"

namespace View
{
    Input::Input(QWidget* parent)
        : QWidget(parent), layout(new QStackedLayout), one_edit(new details::input_frame{ { "読み" }, this }), two_edits(new details::input_frame{ { "音読み", "訓読み" }, this })
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout->addWidget(one_edit);
        layout->addWidget(two_edits);
        setLayout(layout);

        connect(one_edit, &details::input_frame::answers, [this](std::vector<QString>& a) {
            emit answers(a);
        });
        connect(two_edits, &details::input_frame::answers, [this](std::vector<QString>& a) {
            emit answers(a);
        });
    }

    void Input::switch_to(input_type type, bool disable_kunyomi)
    {
        if (disable_kunyomi && type == KANJI)
            two_edits->disable_edit(1);

        switch (type)
        {
            case KANJI:
                layout->setCurrentIndex(1);
                break;
            case WORD:
                layout->setCurrentIndex(0);
                break;
            default:
                break;
        }
    }

    void Input::disable()
    {
        one_edit->disable_edit(0);
        two_edits->disable_edit(0);
        two_edits->disable_edit(1);
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