#ifndef __START_PAGE_H__
#define __START_PAGE_H__

#include <QtWidgets>
#include <vector>

class start_page : public QWidget
{
    Q_OBJECT

    QListView* file_list;
    QStringListModel* file_list_model;
    QString default_path;

public:

    start_page(QWidget* = nullptr);
    void add_file(const QString&);
    void add_files();
    void add_filename(const QString&);
    void remove_filename(const QString&);

signals:

    void start_session(const std::vector<QString>&);
    void edit_files(const std::vector<QString>&);

private:

    void setup_views();
    void setup_files();

    void update();

    int find_filename(const QString&);

    std::vector<QString> get_selected_files();

    void list_context_menu(const QPoint&);
    void file_context_menu(const QPoint&);
};

#endif