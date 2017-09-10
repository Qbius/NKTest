#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <string>
#include <set>
#include <QDialog.h>

class file_manager
{
public:

    std::set<std::string> active_files;

    file_manager();

    void add_file(const std::string&);
    void remove_file(const std::string&);
    void open_dialog();
};

class file_manager_dialog : public QDialog
{
    Q_OBJECT

public:

    file_manager_dialog(std::set<std::string>&, std::set<std::string>&);
};

#endif