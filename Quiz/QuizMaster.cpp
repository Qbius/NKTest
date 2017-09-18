#include "Quiz/QuizMaster.h"
#include "JPUtilities.h"
#include <random>

quiz_master::quiz_master(const std::vector<vocab>& mat, QWidget* parent) : 
    QWidget(parent), 
    material(mat), 
    main(new View::Main{ this }), 
    input(new View::Input{ this }), 
    stats(new View::Stats{ static_cast<unsigned>(material.size()), this }), 
    wrong(new View::Wrong{ this }),
    current_question(material.begin())
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout* layout = new QGridLayout;

    layout->setRowMinimumHeight(0, 250);
    layout->setRowMinimumHeight(1, 250);
    layout->setColumnMinimumWidth(0, 250);
    layout->setColumnMinimumWidth(1, 250);
    layout->setColumnMinimumWidth(2, 250);
    

    layout->addWidget(main, 0, 0, 1, 2);
    layout->addWidget(input, 1, 1);
    layout->addWidget(stats, 1, 0);
    layout->addWidget(wrong, 0, 2, 2, 1);

    int rows = layout->rowCount();
    int cols = layout->columnCount();


    setLayout(layout);

    connect(input, &View::Input::answers, this, &quiz_master::check_answers);
    connect(wrong, &View::Wrong::copy_mistakes, [this](const QString& copied_text) {
        QApplication::clipboard()->setText(copied_text);
    });
    connect(wrong, &View::Wrong::mistake_session, [this]() {
        emit quick_session(mistakes);
    });
    connect(&settings(), &nk_settings::type_changed, [this](quiz_type type) {
        if (in_progress()) set_question(type);
    });

    std::random_device seed;
    std::mt19937 rng(seed());
    std::shuffle(material.begin(), material.end(), rng);

    set_question(settings().type);
}

bool quiz_master::in_progress()
{
    return current_question != material.end();
}

void quiz_master::check_answers(std::vector<QString>& answers)
{
    const QStringList* to_check;

    switch (settings().type)
    {
        case quiz_type::READING:
        {
            for (auto& answer : answers)
            {
                answer = answer.replace("ｎ", "ん");
            }

            if (current_question->type == vocab::kanji && !answers.empty())
                *answers.begin() = hiragana_to_katakana(*answers.begin());

            to_check = &current_question->readings;
        }
        break;

        case quiz_type::WRITING:
        {
            to_check = &current_question->writings;
        }
    }

    bool res = true;
    for (const auto& answer : answers)
    {
        if (std::find(to_check->begin(), to_check->end(), answer) == to_check->end())
        {
            res = false;
            break;
        }
    }

    answered(res);
}

void quiz_master::answered(bool correct)
{
    static auto random_type = []() -> quiz_type {
        static std::random_device seed;
        static std::mt19937 rng(seed());
        static std::uniform_int_distribution<int> range(0, 1);

        return static_cast<quiz_type>(range(rng));
    };
    quiz_type current = (settings().type == quiz_type::RANDOM) ? random_type() : settings().type;

    if (correct)
    {
        ++*stats;
    }
    else
    {
        --*stats;

        QString mistake_header;
        QStringList mistake_answers;

        switch (current)
        {
            case quiz_type::READING:
            {
                mistake_header = current_question->writings.front();
                mistake_answers = current_question->readings;
            }
            break;
            case quiz_type::WRITING:
            {
                mistake_header = current_question->meanings.front();
                mistake_answers = current_question->writings;
            }
            break;
        }

        wrong->insert_mistake(mistake_header, mistake_answers);
        mistakes.push_back(*current_question);
    }

    ++current_question;
    if (current_question != material.end())
        set_question(current);
    else
        input->disable();
}

void quiz_master::set_question(quiz_type type)
{
    QString display;
    View::Input::input_type input_type;

    switch (type)
    {
        case quiz_type::READING:
            display = current_question->writings.back();
            input_type = current_question->type == vocab::kanji ? View::Input::KANJI_READING : View::Input::WORDS_READING;
            break;
        case quiz_type::WRITING:
            display = current_question->meanings.front();
            input_type = current_question->type == vocab::kanji ? View::Input::KANJI_WRITING : View::Input::WORDS_WRITING;
            break;
    }

    main->set_text(display);
    input->switch_to(input_type, is_katakana(current_question->readings));
}