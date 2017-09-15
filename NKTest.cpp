#include "NKTest.h"
#include <fstream>
#include "Quiz/QuizMaster.h"
#include "NKTestConstants.h"
#include "FileUtilities.h"
#include "KanjiListDialog.h"

NKTest::NKTest()
    : tabs(new QTabWidget{ this }), main_page(new start_page{ this })
{
    setCentralWidget(tabs);
    setup_views();
    setup_menus();
}

void NKTest::setup_views()
{
    setAcceptDrops(true);
    setMinimumSize(750, 500);

    connect(main_page, &start_page::start_session, [this](const std::vector<QString>& filenames) {
        new_session(parse_files(filenames));
    });

    tabs->addTab(main_page, "Main");
    tabs->setTabsClosable(true);
    tabs->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
    connect(tabs, &QTabWidget::tabCloseRequested, this, &NKTest::on_tab_close);
}

void NKTest::setup_menus()
{
    auto create_action = [this](const QString& title, auto receiver, const auto& function, const QKeySequence& key_sequence = {}) {
        QAction* act = new QAction(title, this);

        if (!key_sequence.isEmpty())
            act->setShortcut(key_sequence);

        connect(act, &QAction::triggered, receiver, function);

        return act;
    };

    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(create_action("Add", main_page, &start_page::add_files));
    fileMenu->addAction(create_action("New kanji list", this, &NKTest::new_kanji_list));

    QMenu* sessionMenu = menuBar()->addMenu(tr("Current session"));
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

void NKTest::new_kanji_list()
{
    kanji_list dlg;
    if (dlg.exec() != QDialog::Rejected)
    {
        file::path new_file{ nk_quiz_directory / (dlg.name + "." + nk_extension) };
        new_file.write(dlg.list);
    }

}

std::vector<vocab> NKTest::parse_files(const std::vector<QString>& filenames)
{
    std::vector<vocab> all_material;

    for (const auto& filename : filenames)
    {
        auto material = file::parse(nk_quiz_directory / filename);
        std::copy(material.begin(), material.end(), std::back_inserter(all_material));
    }

    return all_material;
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
            main_page->add_file(url.toLocalFile());
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