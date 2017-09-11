#ifndef __VOCAB_H__
#define __VOCAB_H__

#include <vector>
#include <QString>

struct vocab
{
    QString writing;
    std::vector<QString> readings;
    QString meaning;

    enum
    {
        kanji,
        word,
        invalid
    } type;

    vocab(const QString& w, const QStringList& rs, const QString& m, const QString& t)
        : writing(w), meaning(m), type(t == "kanji" ? kanji : word)
    {
        for (const auto& r : rs)
        {
            QStringList parts{ r.split(".") };
            if (parts.size() == 1)
            {
                readings.push_back(r);
            }
            else if (parts.size() == 2)
            {
                readings.push_back(parts[0]);
                readings.push_back(parts[0] + parts[1]);
            }
        }
    }

    vocab()
        : type(invalid)
    {
    }
};

#endif