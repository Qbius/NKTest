#include "StatsView.h"

namespace View
{
    Stats::Stats(unsigned total, QWidget* parent) : QWidget(parent), right(0), count(0), total(total)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout* layout{ new QVBoxLayout };

        progress_label = new QLabel{ this };
        progress_label->setAlignment(Qt::AlignCenter);
        layout->addWidget(progress_label);

        progress_bar = new QProgressBar{ this };
        layout->addWidget(progress_bar);

        setLayout(layout);
    }

    void Stats::update()
    {
        static auto itoq = [](unsigned n) {
            return QString::fromStdString(std::to_string(n));
        };

        progress_label->setText(itoq(count) + "/" + itoq(total));
        progress_bar->setValue(right);
        progress_bar->setMaximum(count);
    }

    void Stats::clear()
    {
        right = total = 0;
        update();
    }
}