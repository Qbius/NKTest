#ifndef __NK_TEST_CONSTANTS_H__
#define __NK_TEST_CONSTANTS_H__

#include "FilePath.h"
#include <QString>
#include <QStandardPaths>

const QString nk_directory_name = "NKTest";
const QString nk_quiz_directory_name = "quiz";
const QString nk_data_directory_name = "data";
const QString nk_extension = "nkt";

inline file::path get_nk_directory()
{
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0) + "/" + nk_directory_name;
}

const file::path nk_directory{ get_nk_directory() };
const file::path nk_quiz_directory{ nk_directory / nk_quiz_directory_name };
const file::path nk_data_directory{ nk_directory / nk_data_directory_name };

#endif