#include "KanjiListDialog.h"
#include "vocab.h"

const QString url_base = "http://jisho.org/api/v1/search/words?keyword=";

kanji_list::kanji_list() : name_edit(new QLineEdit{ this }), list_edit(new QTextEdit{ this }), network_mgr(new QNetworkAccessManager{ this })
{
    connect(network_mgr, &QNetworkAccessManager::finished, this, &kanji_list::accept_reply);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(new QLabel{ "Name:", this });
    layout->addWidget(name_edit);
    layout->addWidget(new QLabel{ "Kanji:", this });
    layout->addWidget(list_edit);

    QPushButton* create = new QPushButton{ "Create", this };
    connect(create, &QPushButton::pressed, [this, layout, create]() {
        name = name_edit->text();

        create->setDisabled(true);
        layout->removeWidget(create);
        QProgressBar* bar = new QProgressBar{ this };
        QString kanji = list_edit->toPlainText();
        bar->setMaximum(kanji.size());
        bar->setAlignment(Qt::AlignCenter);
        bar->setStyleSheet("QProgressBar { background-color: white; }");
        bar->setStyleSheet("QProgressBar::chunk { background-color: #d7d6d5; }");
        layout->addWidget(bar);

        int i = 0;
        for (const auto& c : list_edit->toPlainText())
        {
            bar->setValue(i);
            bar->setFormat(QString::number(i) + "/" + QString::number(kanji.size()));
            current = c;
            call_api();

            QEventLoop loop;
            connect(this, &kanji_list::finished_parsing, &loop, &QEventLoop::quit);
            loop.exec();

            ++i;
        }

        emit accept();
    });
    layout->addWidget(create);

    setLayout(layout);
}

void kanji_list::call_api()
{
    network_mgr->get(QNetworkRequest{ url_base + current });
}

void kanji_list::accept_reply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) 
    {
        vocab v{ parse_json(reply->readAll()) };
        if (v.type != vocab::invalid)
            list.push_back(v);
    }

    emit finished_parsing();
}

QString kanji_list::parse_json(const QString& json_string)
{
    QStringList
        writings{ current },
        readings,
        meanings;


    QJsonArray json_data = QJsonDocument::fromJson(json_string.toUtf8()).object()["data"].toArray();
    for (const auto& data : json_data)
    {
        QJsonArray japanese = data.toObject()["japanese"].toArray();
        for (const auto& jp : japanese)
        {
            if (jp.toObject()["word"].toString() == current)
            {
                readings.append(jp.toObject()["reading"].toString());

                QJsonArray senses = data.toObject()["senses"].toArray();
                for (const auto& sense : senses)
                {
                    QJsonArray definitions = sense.toObject()["english_definitions"].toArray();
                    for (const auto& definition : definitions)
                    {
                        meanings.append(definition.toString());
                    }
                }
            }

            break;
        }
    }

    if (!readings.isEmpty() && !meanings.isEmpty())
        return vocab{ writings, readings, meanings, "kanji" };
    else
        return vocab{};
}