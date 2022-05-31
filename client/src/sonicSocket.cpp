#include "sonicSocket.h"

const char ip[] = "localhost";
const short port = 1337;

namespace client::web {
client::web::SocketWrapper &socket(ResponseHandler *handler) {
    static SocketWrapper instance(ip, port, handler);
    return instance;
}
}  // namespace client::web
