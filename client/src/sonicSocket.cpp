#include "sonicSocket.h"

const char ip[] = "46.17.248.131";
const short port = 1337;

namespace client::web {
client::web::SocketWrapper& socket(ResponseHandler* handler) {
    static SocketWrapper wrapper(ip, port, handler);
    return wrapper;
}
}
