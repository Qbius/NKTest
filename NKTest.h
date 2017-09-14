#include <QtWidgets>
#include <vector>
#include <set>
#include "vocab.h"
#include "StartPage.h"

class NKTest : public QMainWindow
{
    Q_OBJECT

    QTabWidget* tabs;
    start_page* main_page;

public:

    NKTest();

private:

    void setup_views();
    void setup_menus();
    void new_session(const std::vector<vocab>&);
    std::vector<vocab> parse_files(const std::vector<QString>&);

    void dragEnterEvent(QDragEnterEvent*);
    void dragMoveEvent(QDragMoveEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

    void on_tab_close(int);
};
