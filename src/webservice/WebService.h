/* Copyright (c) 2018 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef WEBSERVICE_WEBSERVICE_H_
#define WEBSERVICE_WEBSERVICE_H_

#include "base/Base.h"
#include "base/Status.h"
#include <proxygen/httpserver/HTTPServer.h>
#include "thread/NamedThread.h"

DECLARE_int32(ws_http_port);
DECLARE_int32(ws_h2_port);
DECLARE_string(ws_ip);
DECLARE_int32(ws_threads);

namespace nebula {

using HandlerGen = std::unordered_map<
    std::string,
    std::function<proxygen::RequestHandler*()>>;

class WebService final {
public:
    // To start the global web server.
    // Two ports would be bound, one for HTTP, another one for HTTP2.
    // If FLAGS_ws_http_port or FLAGS_ws_h2_port is zero, an ephemeral port
    // would be assigned and set back to the gflag, respectively.
    static MUST_USE_RESULT Status start();
    // To stop the web service and join the internal threads
    static void stop();

    // To register a handler generator for a specific path
    // All registrations have to be done before calling start().
    // Anything registered after start() being called will not take
    // effect
    //
    // By default, web service will register the handler for getting/setting
    // the flags, getting the stats
    static void registerHandler(const std::string& path,
                                std::function<proxygen::RequestHandler*()>&& gen);

private:
    WebService() = delete;

    static std::unique_ptr<proxygen::HTTPServer> server_;
    static std::unique_ptr<thread::NamedThread> wsThread_;

    static HandlerGen handlerGenMap_;
};

}  // namespace nebula
#endif  // WEBSERVICE_WEBSERVICE_H_
