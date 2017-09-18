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

        layout->addWidget(label);
        setLayout(layout);
    }

    void Main::set_text(const QString& msg)
    {
        label->setText(msg);
        set_font(500 / (msg.size() + 2));
    }

    void Main::set_font(int size)
    {
        
        label->setStyleSheet("QLabel { font: " + QString::number(size > 175 ? 175 : size) + "pt; }");
    }
}