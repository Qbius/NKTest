#ifndef __FILE_UTILITIES_H__
#define __FILE_UTILITIES_H__

#include "FilePath.h"
#include "vocab.h"

namespace file
{
    inline path import_to(const path& input_file, const path& destination)
    {
        if (input_file.extension() == "csv")
        {
            return input_file.copy_to(destination);
        }
    }

    inline std::vector<vocab> parse(const path& input_file)
    {
        std::vector<vocab> material;

        for (const auto& line : input_file.read())
        {
            QStringList parts{ line.split("\t") };

            if (parts.size() == 4)
            {
                QString
                    writing = parts[0],
                    reading = parts[1].replace("-", ""),
                    meaning = parts[2],
                    type = parts[3];

                material.push_back({ writing.split(" ・ "), reading.split(" ・ "), meaning.split("; "), type });
            }
        }

        return material;
    }
}

#endif