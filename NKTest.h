#include <QtWidgets>
#include "FileManager.h"
#include <vector>
#include <set>
#include "vocab.h"
#include "StartPage.h"

class NKTest : public QMainWindow
{
    Q_OBJECT

    QTabWidget* tabs;
    file_manager file_mgr;

public:

    NKTest();
   
private:

    void setup_views();
    void setup_menus();
    void add_file();
    void file_mgr_dialog();
    void new_session(const std::vector<vocab>&);
    std::vector<vocab> parse_files(const std::set<std::string>&);

    void dragEnterEvent(QDragEnterEvent*);
    void dragMoveEvent(QDragMoveEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

    void NKTest::on_tab_close(int);

signals:

    void file_added(const QString&);
    void file_removed(const QString&);
};