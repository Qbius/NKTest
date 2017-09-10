#include "WrongView.h"

namespace View
{
    Wrong::Wrong(QWidget* parent)
        : QWidget(parent), mistakes(new QTextEdit{this})
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        mistakes->setEnabled(false);
    }

    void Wrong::insert_mistake(const QString& msg)
    {
        mistakes->insertPlainText(msg + "\n");
    }
}