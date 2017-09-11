#include "NKTest.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include "Quiz/QuizMaster.h"
#include "NKTestConstants.h"

NKTest::NKTest()
    : tabs(new QTabWidget{ this })
{
    setCentralWidget(tabs);
    setup_views();
    setup_menus();
}

void NKTest::setup_views()
{
    setAcceptDrops(true);
    setMinimumSize(750, 500);

    start_page* start = new start_page{ this };
    connect(&file_mgr, &file_manager::file_added, start, &start_page::add_filename);
    connect(&file_mgr, &file_manager::file_removed, start, &start_page::remove_filename);
    connect(start, &start_page::start_session, [this](const std::vector<QString>& filenames) {
        std::set<std::string> files_to_parse; 
        for (const auto& filename : filenames)
        {
            files_to_parse.insert(filename.toStdString()); // this whole thing is unacceptable and will be dealt with eventually
        }

        new_session(parse_files(files_to_parse));
    });

    file_mgr.init();

    tabs->addTab(start, "Main");
    tabs->setTabsClosable(true);
    tabs->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
    connect(tabs, &QTabWidget::tabCloseRequested, this, &NKTest::on_tab_close);
}

void NKTest::setup_menus()
{
    auto create_action = [this](const QString& title, const auto& function, const QKeySequence& key_sequence = {}) {
        QAction* act = new QAction(title, this);

        if (!key_sequence.isEmpty())
            act->setShortcut(key_sequence);

        connect(act, &QAction::triggered, this, function);

        return act;
    };

    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(create_action("Add", &NKTest::add_file));
    fileMenu->addAction(create_action("File Manager", &NKTest::file_mgr_dialog));

    QMenu* sessionMenu = menuBar()->addMenu(tr("Current session"));
    //workaround, gotta do something with that fucking lambda
    QAction* newSessionAct = new QAction{ "New session", this };
    newSessionAct->setShortcut(QKeySequence::New);
    connect(newSessionAct, &QAction::triggered, [this]() {
        new_session(parse_files(file_mgr.active_files));
    });
    sessionMenu->addAction(newSessionAct);
}

void NKTest::add_file()
{
    static QString default_path;
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Add files"), default_path, "Tangorin Lists (*.csv)");
    for (const auto& path : paths)
    {
        std::string str_path = path.toStdString();
        file_mgr.add_file(str_path);
        default_path = QString::fromStdString(boost::filesystem::path{ str_path }.parent_path().string());
    }
}

void NKTest::file_mgr_dialog()
{
    file_mgr.open_dialog();
}

void NKTest::new_session(const std::vector<vocab>& m)
{
    if (!m.empty())
    {
        static int session_count = 1;

        quiz_master* session = new quiz_master{ m, this };
        connect(session, &quiz_master::quick_session, this, &NKTest::new_session);
        tabs->addTab(session, "Session " + QString::fromStdString(std::to_string(session_count)));
        tabs->setCurrentIndex(tabs->count() - 1);
        ++session_count;
    }
    else
    {
        //TODO error system
        QMessageBox* alert = new QMessageBox{ QMessageBox::Warning, "Warning", "Material list is empty", QMessageBox::Ok, this };
        alert->exec();
    }
}

std::vector<vocab> NKTest::parse_files(const std::set<std::string>& filenames)
{
    std::vector<vocab> material;

    for (const auto& filename : filenames)
    {
        std::ifstream input(boost::filesystem::path{ nk_quiz_directory / (filename + nk_extension) }.string());
        
        std::string line;
        while (std::getline(input, line))
        {
            QStringList parts{ QString::fromStdString(line).split("\t") };

            if (parts.size() == 4)
            {
                QString
                    writing = parts[0],
                    reading = parts[1],
                    meaning = parts[2],
                    type = parts[3];

                reading = reading.replace("-", "");
                QStringList readings = reading.split(" ・ ");
                material.push_back({ writing, readings, meaning, type });
            }
        }
    }

    return material;
}

void NKTest::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void NKTest::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void NKTest::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void NKTest::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    if (mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();

        for (const auto& url : urls)
        {
            file_mgr.add_file(url.toLocalFile().toStdString()); // crashes on std::string's destructor, but why?
        }

        event->acceptProposedAction();
    }
}

void NKTest::on_tab_close(int index)
{
    if (dynamic_cast<quiz_master*>(tabs->widget(index))->in_progress()) 
    {
        const int result = (new QMessageBox{ QMessageBox::Warning, "Warning", "Session is in progress.\nClose the tab regardless?", QMessageBox::Yes | QMessageBox::No, this })->exec();
        if (result == QMessageBox::No)
            return;
    }
    tabs->removeTab(index);

}