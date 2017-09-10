#include "NKTest.h"
#include "StartPage.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
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
    setMinimumSize(750, 500);
    tabs->addTab(new start_page{ this }, "Main");
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
    sessionMenu->addAction(create_action("New", &NKTest::new_session, QKeySequence::New));
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

void NKTest::new_session()
{
    tabs->addTab(new quiz_master{ parse_active_files(), this }, "New session");
    tabs->setCurrentIndex(tabs->count() - 1);
}

std::vector<vocab> NKTest::parse_active_files()
{
    std::vector<vocab> material;

    for (const auto& filename : file_mgr.active_files)
    {
        std::ifstream input(boost::filesystem::path{ nk_directory / (filename + nk_extension) }.string());
        
        std::string line;
        while (std::getline(input, line))
        {
            std::vector<std::string> parts;
            boost::split(parts, line, boost::is_any_of("\t"));

            if (parts.size() == 4)
            {
                std::string
                    writing = parts[0],
                    reading = parts[1],
                    meaning = parts[2],
                    type = parts[3];

                QString q_reading = QString::fromStdString(reading);
                QString readings_splitter = " ・ ";
                reading = q_reading.replace(readings_splitter, "|").toStdString();

                std::vector<std::string> readings;
                boost::split(readings, reading, boost::is_any_of("|"));

                material.push_back({ writing, readings, meaning, type });
            }
        }
    }

    return material;
}