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

    vocab(const std::string& w, const std::vector<std::string>& rs, const std::string& m, const std::string& t)
        : writing(QString::fromStdString(w)), meaning(QString::fromStdString(m)), type(t == "kanji" ? kanji : word)
    {
        for (const auto& r : rs)
        {
            auto dot = r.find('.');
            if (dot == std::string::npos)
            {
                readings.push_back(QString::fromStdString(r));
            }
            else
            {
                readings.push_back(QString::fromStdString(r.substr(0, dot)));
                readings.push_back(QString::fromStdString(r).replace(".", ""));
            }
        }
    }

    vocab()
        : type(invalid)
    {
    }
};

#endif