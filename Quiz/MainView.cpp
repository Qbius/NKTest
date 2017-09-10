#include "MainView.h"

namespace View
{
    Main::Main(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout* layout = new QVBoxLayout;

        label = new QLabel(this);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setAlignment(Qt::AlignCenter);

        QFont font;
        font.setPointSize(144);
        label->setFont(font);

        layout->addWidget(label);
        setLayout(layout);
    }

    void Main::set_text(const QString& msg)
    {
        label->setText(msg);
    }
}