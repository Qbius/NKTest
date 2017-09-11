#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__

#include <string>
#include <set>
#include <QDialog>

class file_manager : public QObject
{
    Q_OBJECT

public:

    std::set<std::string> active_files;

    void init();

    void add_file(const std::string&);
    void remove_file(const std::string&);
    void open_dialog();

signals:

    void file_added(const QString&);
    void file_removed(const QString&);
};

class file_manager_dialog : public QDialog
{
    Q_OBJECT

public:

    file_manager_dialog(std::set<std::string>&, std::set<std::string>&);
};

#endif