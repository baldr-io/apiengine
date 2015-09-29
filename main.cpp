#include <microhttpd.h>
#include <json/json.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <dirent.h>
#include "scriptapi.hpp"

using namespace std;

#define MIMETYPE "application/json"
#define APIVERSION "0.1"

static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls) {
    ScriptAPI sapi;
    Json::Value root;
    Json::FastWriter writer;
    string response;
    struct MHD_Response *resp;
    int retCode = MHD_NO;
    printf ("New %s request for %s using version %s\n", method, url, version);

    response = sapi.Execute(string(url));

    resp = MHD_create_response_from_buffer (response.size(), (void*) response.c_str(), MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header (resp, "Content-Type", MIMETYPE);
    retCode = MHD_queue_response (connection, MHD_HTTP_OK, resp);
    MHD_destroy_response (resp);

    return retCode;
}

int main(int argc, char ** argv) {
    if(argc != 2) {
        cout << "Wrong number of arguments" << endl;
    }

    const int portNumber = atoi(argv[1]);
    cout << "Starting HTTP server on port " << portNumber << endl;
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, portNumber, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) return 1;
    getchar ();
    MHD_stop_daemon (daemon);
    return 0;
}
