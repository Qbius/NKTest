#ifndef __QUIZ_MASTER_H__
#define __QUIZ_MASTER_H__

#include <QtWidgets>
#include "InputView.h"
#include "StatsView.h"
#include "MainView.h"
#include "WrongView.h"
#include "vocab.h"
#include <vector>

class quiz_master : public QWidget
{
    Q_OBJECT

    std::vector<vocab> material;
    std::vector<vocab>::const_iterator current_question;

    std::vector<vocab> mistakes;

    View::Main* main;
    View::Input* input;
    View::Stats* stats;
    View::Wrong* wrong;

public:

    quiz_master(const std::vector<vocab>&, QWidget* = nullptr);

private:
    
    void check_answers(std::vector<QString>&);
    void answered(bool);
    void set_question();

signals:

    void quick_session(const std::vector<vocab>&);
};

#endif