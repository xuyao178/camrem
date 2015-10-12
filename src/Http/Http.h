//
// Created by Christopher Schmidt on 27.07.15.
//

#ifndef CAMREM_HTTP_H
#define CAMREM_HTTP_H

#include <vector>
#include <regex>
#include "../Helpers.h"
#include "Daemon.h"
#include "Response.h"
#include "Bindings.h"

using std::vector;
using std::regex;
using std::pair;
using std::regex_match;
using std::function;

namespace Http {
    enum StatusCode {
        STATUS_OK = 200,
        STATUS_NOTFOUND = 404,
        STATUS_ERROR = 500
    };

    class Http {
    private:
        vector<Daemon*> daemons_;
        vector<pair<regex, function<bool(Request *)>>> handlers_;
        string document_root_;
        Http(void);
        ~Http(void);

    public:
        /*
         * The instance of Http is defined and created in here.
         * @return The instance of Http
         */
        static Http * getInstance(void);
        /*
         * Initializes and runs every VirtualHost specified in the config.
         */
        void run(void);
        void handle(function<bool(Request *)> callback, regex uri);
        void handle(string filename, regex uri);
        void handle(string filename, StatusCode status, regex uri);
        bool processRequest(Request *request);
    };
}


#endif //CAMREM_HTTP_H
