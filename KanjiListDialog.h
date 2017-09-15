#ifndef __KANJI_LIST_DIALOG_H__
#define __KANJI_LIST_DIALOG_H__

#include <QtWidgets>
#include <QtNetwork>
#include "vocab.h"
#include <vector>

class kanji_list : public QDialog
{
    Q_OBJECT

    QLineEdit* name_edit;
    QTextEdit* list_edit;

    QString current;

    QNetworkAccessManager* network_mgr;

public:

    QString name;
    std::vector<QString> list;

    kanji_list();

signals:

    void finished_parsing();

private:

    void call_api();
    void accept_reply(QNetworkReply*);
    vocab parse_json(const QString&);
};

#endif