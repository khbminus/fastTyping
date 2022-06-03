#include "sonicSocket.h"

//const char ip[] = "localhost";
const char ip[] = "46.17.248.131";
const short port = 1337;

namespace client::web {
client::web::SocketWrapper &socket(ResponseHandler *handler) {
    static SocketWrapper instance(ip, port, handler);
    return instance;
}
}  // namespace client::web
