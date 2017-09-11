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

    std::random_device seed;
    std::mt19937 rng(seed());
    std::shuffle(material.begin(), material.end(), rng);

    set_question();
}

bool quiz_master::in_progress()
{
    return current_question != material.end();
}

void quiz_master::check_answers(std::vector<QString>& answers)
{
    for (auto& answer : answers)
    {
        answer = answer.replace("ｎ", "ん");
    }

    if (current_question->type == vocab::kanji && !answers.empty())
        *answers.begin() = hiragana_to_katakana(*answers.begin());

    bool res = true;
    for (const auto& answer : answers)
    {
        if (std::find(current_question->readings.begin(), current_question->readings.end(), answer) == current_question->readings.end())
        {
            res = false;
            break;
        }
    }

    answered(res);
}

void quiz_master::answered(bool correct)
{
    if (correct)
    {
        ++*stats;
    }
    else
    {
        --*stats;
        
        QString correct_answer{ current_question->writing + ": " };
        for (auto it = current_question->readings.begin(); it != current_question->readings.end(); ++it)
        {
            if (it != current_question->readings.begin())
                correct_answer += ", ";
            correct_answer += *it;
        }

        wrong->insert_mistake(correct_answer);
        mistakes.push_back(*current_question);
    }

    ++current_question;
    if (current_question != material.end())
        set_question();
    else
        input->disable();
}

void quiz_master::set_question()
{
    main->set_text(current_question->writing);

    const View::Input::input_type input_type = current_question->type == vocab::kanji ? View::Input::KANJI : View::Input::WORD;
    const bool disable_kunyomi = is_katakana(current_question->readings);

    input->switch_to(input_type, disable_kunyomi);
}