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
            material.push_back(line);
        }

        return material;
    }
}

#endif