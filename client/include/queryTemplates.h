#ifndef QUERY_TEMPLATES_H
#define QUERY_TEMPLATES_H

#include <QString>

namespace client::queries {
QString join_query(int id);
QString key_pressed_query(QString const &buffer);
QString backspace_pressed_query();
QString greeting_query(QString const &name);
QString sign_in_query(QString const &username, QString const &password);
QString sign_on_query(QString const &username, QString const &password);
QString create_game_query(bool auto_join = true);
QString start_query();
QString wait_game_query();
QString leave_query();
QString get_line_query();
QString getStatesQuery();
QString user_finish_query();
QString get_game_stat_query();
}  // namespace client::queries

#endif
