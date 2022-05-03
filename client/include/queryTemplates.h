#ifndef QUERY_TEMPLATES_H
#define QUERY_TEMPLATES_H

#include <QString>

namespace client::queries {
QString join_query(int id);
QString key_pressed_query(QString const& buffer);
QString greeting_query(QString const& name);
}

#endif
