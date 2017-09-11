#include "StartPage.h"

start_page::start_page(QWidget* parent) : QWidget(parent), file_list(new QListView{ this }), file_list_model(new QStringListModel{ this })
{
    file_list->setModel(file_list_model);
    file_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    file_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGridLayout* layout = new QGridLayout;

    layout->setRowMinimumHeight(0, 250);
    layout->setRowMinimumHeight(1, 250);
    layout->setColumnMinimumWidth(0, 250);
    layout->setColumnMinimumWidth(1, 250);
    layout->setColumnMinimumWidth(2, 250);

    QWidget* file_list_column = new QWidget{ this };
    QVBoxLayout* file_list_column_layout = new QVBoxLayout;

    file_list_column_layout->addWidget(file_list);

    QPushButton* new_session = new QPushButton{ "New session", this };
    connect(new_session, &QPushButton::pressed, [this]() {
        std::vector<QString> selected_files;
        for (const auto& index : file_list->selectionModel()->selectedIndexes())
        {
            selected_files.push_back(file_list_model->index(index.row()).data().toString());
        }
        emit start_session(selected_files);
    });
    file_list_column_layout->addWidget(new_session);
    file_list_column->setLayout(file_list_column_layout);

    layout->addWidget(file_list_column, 0, 0, 2, 1);
    setLayout(layout);

}

void start_page::add_filename(const QString& filename)
{
    file_list_model->insertRow(file_list_model->rowCount());
    QModelIndex index = file_list_model->index(file_list_model->rowCount() - 1);
    file_list_model->setData(index, filename);
}

void start_page::remove_filename(const QString& filename)
{
    for (int i = 0; i < file_list_model->rowCount(); ++i)
    {
        if (file_list_model->index(i).data(Qt::DisplayRole).toString() == filename)
        {
            file_list_model->removeRow(i);
            break;
        }
    }
}