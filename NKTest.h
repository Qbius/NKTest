#include <QtWidgets>
#include "FileManager.h"
#include <vector>
#include "Quiz/vocab.h"

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
    void new_session();
    std::vector<vocab> parse_active_files();
};