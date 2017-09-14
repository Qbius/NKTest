#ifndef __VOCAB_H__
#define __VOCAB_H__

#include <vector>
#include <QString>

struct vocab
{
    std::vector<QString> writings; //
    std::vector<QString> readings; // switch to QStringList after all?? Jeez, I'm about to abandon all that is holy...
    std::vector<QString> meanings; //

    enum
    {
        kanji,
        word,
        invalid
    } type;

    vocab(const QStringList& ws, const QStringList& rs, const QStringList& ms, const QString& t) : type(t == "kanji" ? kanji : word)
    {
        for (const auto& w : ws)
        {
            writings.push_back(w);
        }

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

        for (const auto& m : ms)
        {
            meanings.push_back(m);
        }
    }

    vocab()
        : type(invalid)
    {
    }
};

#endif