#ifndef __NK_TEST_CONSTANTS_H__
#define __NK_TEST_CONSTANTS_H__

#include "FilePath.h"
#include <QString>

#ifdef WIN32
#include <shlobj.h>
#endif

const QString nk_directory_name = "NKTest";
const QString nk_quiz_directory_name = "quiz";
const QString nk_data_directory_name = "data";
const QString nk_extension = "nkt";

inline file::path get_nk_directory()
{

#ifdef WIN32
    TCHAR appdata_path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata_path)))
    {
        return QString::fromUtf8(appdata_path) + "/" + nk_directory_name;
    }
    else
    {
        return nk_directory_name;
    }
#else
    return nk_directory_name;
#endif
}

const file::path nk_directory{ get_nk_directory() };
const file::path nk_quiz_directory{ nk_directory / nk_quiz_directory_name };
const file::path nk_data_directory{ nk_directory / nk_data_directory_name };

#endif