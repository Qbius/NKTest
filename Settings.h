#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QtWidgets>
#include "NKTestConstants.h"

enum class quiz_type {
    READING,
    WRITING,
    RANDOM
};

class nk_settings : public QObject
{
    Q_OBJECT

    QSettings setts;

public:

    quiz_type type;
    unsigned timer;

    nk_settings()
        : setts(application_name, application_name), timer(setts.value(setting_timer).toInt())
    {
    }

    void set_type(quiz_type new_type)
    {
        type = new_type;
        emit type_changed(new_type);
    }

    ~nk_settings()
    {
        setts.setValue(setting_timer, timer);
    }

signals:

    void type_changed(quiz_type);
};

inline nk_settings& settings()
{
    static nk_settings nk_setts;
    return nk_setts;
}

#endif