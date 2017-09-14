#include "StartPage.h"
#include "FileUtilities.h"
#include "NKTestConstants.h"

start_page::start_page(QWidget* parent) : QWidget(parent), file_list(new QListView{ this }), file_list_model(new QStringListModel{ this }), default_path(QDir::homePath())
{
    setup_views();
    setup_files();
}

void start_page::setup_views()
{
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
        emit start_session(get_selected_files());
    });
    file_list_column_layout->addWidget(new_session);
    file_list_column->setLayout(file_list_column_layout);

    layout->addWidget(file_list_column, 0, 0, 2, 1);
    setLayout(layout);
}

void start_page::setup_files()
{
    file_list->setModel(file_list_model);
    file_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
    file_list->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    file_list->setContextMenuPolicy(Qt::CustomContextMenu);

    update();
    QTimer* timer = new QTimer{ this };
    connect(timer, &QTimer::timeout, this, &start_page::update);
    timer->start(1000);

    connect(file_list, &start_page::customContextMenuRequested, [this](const QPoint& pos) {
        if (file_list->indexAt(pos).row() < 0)
            list_context_menu(pos);
        else
            file_context_menu(pos);
    });
}

void start_page::update()
{
    auto quiz_files = nk_quiz_directory.get_files();
    std::vector<QString> basenames;
    for (const auto& quiz_file : quiz_files)
    {
        basenames.push_back(quiz_file.basename());
        if (quiz_file.extension() == nk_extension && find_filename(quiz_file.basename()) == -1)
        {
            add_filename(quiz_file.basename());
        }
    }
    for (int i = file_list_model->rowCount() - 1; i >= 0; --i)
    {
        if (std::find(basenames.begin(), basenames.end(), file_list_model->index(i).data(Qt::DisplayRole).toString()) == basenames.end())
            file_list_model->removeRow(i);
    }
}

void start_page::add_file(const QString& path)
{
    default_path = file::path{ path }.parent();
    file::path imported = file::import_to(path, nk_quiz_directory);
    imported.change_extension(nk_extension);
    add_filename(imported.basename());
}

void start_page::add_files()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, "Add files", default_path, "Tangorin Lists (*.csv)");
    for (const auto& path : paths)
    {
        add_file(path);
    }
}

void start_page::add_filename(const QString& filename)
{
    file_list_model->insertRow(file_list_model->rowCount());
    QModelIndex index = file_list_model->index(file_list_model->rowCount() - 1);
    file_list_model->setData(index, filename);
}

void start_page::remove_filename(const QString& filename)
{
    int i = find_filename(filename);
    if (i != -1)
        file_list_model->removeRow(i);
}

int start_page::find_filename(const QString& filename)
{
    for (int i = 0; i < file_list_model->rowCount(); ++i)
    {
        if (file_list_model->index(i).data(Qt::DisplayRole).toString() == filename)
        {
            return i;
        }
    }
    return -1;
}

std::vector<QString> start_page::get_selected_files()
{
    std::vector<QString> selected_files;
    for (const auto& index : file_list->selectionModel()->selectedIndexes())
    {
        selected_files.push_back(file_list_model->index(index.row()).data().toString() + "." + nk_extension);
    }
    return selected_files;
}

void start_page::list_context_menu(const QPoint& pos)
{
    QMenu menu{ "Context menu", this };

    QAction action1{ "Add", this };
    connect(&action1, &QAction::triggered, this, &start_page::add_files);
    menu.addAction(&action1);

    menu.exec(mapToGlobal(pos));
}

void start_page::file_context_menu(const QPoint& pos)
{

}