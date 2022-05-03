#ifndef SONIC_SOCKET_H
#define SONIC_SOCKET_H

#include "socketWrapper.h"


namespace client::web {
client::web::SocketWrapper& socket(ResponseHandler* handler = nullptr);
}

#endif
