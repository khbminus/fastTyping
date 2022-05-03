#include "sonicSocket.h"

const char ip[] = "localhost";
const short port = 1337;

namespace client::web {
client::web::SocketWrapper& socket(ResponseHandler* handler) {
    static SocketWrapper wrapper(ip, port, handler);
    return wrapper;
}
}
