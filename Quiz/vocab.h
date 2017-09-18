#ifndef __VOCAB_H__
#define __VOCAB_H__

#include <vector>
#include <QString>

struct vocab
{
    QStringList writings;
    QStringList readings;
    QStringList meanings;

    enum
    {
        kanji,
        word,
        invalid
    } type;

    vocab(const QStringList& ws, const QStringList& rs, const QStringList& ms, const QString& t) 
        : writings(ws), meanings(ms), type(t == "kanji" ? kanji : word)
    {
        for (const auto& r : rs)
        {
            QStringList parts{ r.split(".") };
            if (parts.size() == 1)
            {
                readings.append(r);
            }
            else if (parts.size() == 2)
            {
                readings.append(parts[0]);
                readings.append(parts[0] + parts[1]);
            }
        }
    }

    vocab()
        : type(invalid)
    {
    }

    vocab(const QString& s)
    {
        QStringList parts{ s.split("\t") };

        if (parts.size() == 4)
        {
            QString
                writing = parts[0],
                reading = parts[1].replace("-", ""),
                meaning = parts[2],
                str_type = parts[3];

            writings = writing.replace(" ・ ", "|").replace("・", "|").split("|");
            meanings = meaning.replace("; ", "|").split("|");
            type = ((str_type == "kanji") ? kanji : word);

            for (const auto& r : reading.replace(" ・ ", "|").replace("・", "|").split("|"))
            {
                QStringList reading_parts{ r.split(".") };
                if (reading_parts.size() == 1)
                {
                    readings.append(r);
                }
                else if (reading_parts.size() == 2)
                {
                    readings.append(reading_parts[0]);
                    readings.append(reading_parts[0] + reading_parts[1]);
                }
            }
        }
        else
            type = invalid;
    }

    operator QString()
    {
        return writings.join("") + "\t" + readings.join("") + "\t" + meanings.join("; ") + "\t" + ((type == kanji) ? "kanji" : "general");
    }
};

#endif