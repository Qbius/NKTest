#include "WrongView.h"

namespace View
{
    Wrong::Wrong(QWidget* parent)
        : QWidget(parent), mistakes(new QTextEdit{ this })
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(mistakes);

        QWidget* buttons_bar = new QWidget{ this };
        QHBoxLayout* buttons_bar_layout = new QHBoxLayout;

        QPushButton* copy = new QPushButton{ "Copy to clipboard", buttons_bar };
        connect(copy, &QPushButton::pressed, this, [this]() {
            emit copy_mistakes(mistakes->toPlainText());
        });
        buttons_bar_layout->addWidget(copy);

        QPushButton* quick_session = new QPushButton{ "Quick session", buttons_bar };
        connect(quick_session, &QPushButton::pressed, this, &Wrong::mistake_session);
        buttons_bar_layout->addWidget(quick_session);
        buttons_bar->setLayout(buttons_bar_layout);

        layout->addWidget(buttons_bar);

        setLayout(layout);

        mistakes->setDisabled(true);
    }

    void Wrong::insert_mistake(const QString& msg)
    {
        mistakes->insertPlainText(msg + "\n");
    }
}
