#ifndef __START_PAGE_H__
#define __START_PAGE_H__

#include <QtWidgets>
#include <vector>

class start_page : public QWidget
{
    Q_OBJECT

    QListView* file_list;
    QStringListModel* file_list_model;

public:

    start_page(QWidget* = nullptr);
    void add_filename(const QString&);
    void remove_filename(const QString&);

signals:

    void start_session(const std::vector<QString>&);

private:

    void list_context_menu(const QPoint&);
    void file_context_menu(const QPoint&);
};

#endif