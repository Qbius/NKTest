#include "StatsView.h"

namespace
{
    QString itoq(unsigned n) 
    {
        return QString::fromStdString(std::to_string(n));
    };
}

namespace View
{
    Stats::Stats(unsigned total, QWidget* parent) : QWidget(parent), right(0), count(1), total(total)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout* layout{ new QVBoxLayout };
        QSpacerItem* topSpacer = new QSpacerItem{ 0, 50, QSizePolicy::Minimum, QSizePolicy::Minimum };
        QSpacerItem* bottomSpacer = new QSpacerItem{ 0, 50, QSizePolicy::Minimum, QSizePolicy::Minimum };

        layout->addItem(topSpacer);

        progress_label = new QLabel{ this };
        progress_label->setAlignment(Qt::AlignCenter);
        progress_label->setStyleSheet("QLabel { font: 28pt; }");
        layout->addWidget(progress_label);

        progress_bar = new QProgressBar{ this };
        progress_bar->setAlignment(Qt::AlignCenter);
        progress_bar->setStyleSheet("QProgressBar { background-color: white; }");
        progress_bar->setStyleSheet("QProgressBar::chunk { background-color: #d7d6d5; }");
        layout->addWidget(progress_bar);

        layout->addItem(bottomSpacer);

        progress_label->setText(itoq(count) + "/" + itoq(total));
        progress_bar->setFormat("0/0");
        progress_bar->setValue(1);
        progress_bar->setMaximum(1);
        setLayout(layout);
    }

    void Stats::update()
    {
        progress_label->setText(itoq(count) + "/" + itoq(total));
        progress_bar->setFormat(itoq(right) + "/" + itoq(count));
        progress_bar->setValue(right);
        progress_bar->setMaximum(count);
    }

    void Stats::clear()
    {
        right = total = 0;
        update();
    }
}