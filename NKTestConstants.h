#ifndef __NK_TEST_CONSTANTS_H__
#define __NK_TEST_CONSTANTS_H__

#include <boost/filesystem.hpp>
#include <shlobj.h>

inline boost::filesystem::path get_nk_directory()
{

#ifdef WIN32
    TCHAR appdata_path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata_path)))
    {
        return boost::filesystem::path{ appdata_path } / "NKTest";
    }
    else
    {
        return "NKData";
    }
#else
    return "NKData";
#endif
}

const boost::filesystem::path nk_directory{ get_nk_directory() };
const std::string nk_extension{ ".nkt" };

#endif