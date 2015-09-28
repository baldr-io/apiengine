#include <microhttpd.h>
#include <json/json.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/utsname.h>
#include <sys/statvfs.h>

using namespace std;

#define MIMETYPE "application/json"
#define APIVERSION "0.1"

Json::Value getUname() {
    Json::Value root;
    struct utsname unamebuf;
    uname(&unamebuf);
    root["uname"]["sysname"] = unamebuf.sysname;
    root["uname"]["nodename"] = unamebuf.nodename;
    root["uname"]["release"] = unamebuf.release;
    root["uname"]["version"] = unamebuf.version;
    root["uname"]["machine"] = unamebuf.machine;
    root["version"] = APIVERSION;
    return root;
}

Json::Value getDisk() {
    Json::Value root;
    struct statvfs diskInfo;
    statvfs("/srv/raid", &diskInfo);
    auto blockSize = diskInfo.f_bsize;
    auto fragSize = diskInfo.f_frsize;
    unsigned long totalSize = (diskInfo.f_blocks * fragSize);
    unsigned long freeSpace = (diskInfo.f_bfree * blockSize);
    auto usedPercent = 100 - (((double)freeSpace / (double)totalSize) * 100);
    cout << "Calculated free percent: " << usedPercent << endl;
    root["disk"]["totalSpace"] = (Json::UInt64)totalSize;
    root["disk"]["freeSpace"] = (Json::UInt64)freeSpace;
    root["disk"]["usedPercent"] = (Json::UInt64)usedPercent;
    root["version"] = APIVERSION;
    return root;
}

static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls) {
    Json::Value root;
    Json::FastWriter writer;
    string response;
    struct MHD_Response *resp;
    int retCode = MHD_NO;
    printf ("New %s request for %s using version %s\n", method, url, version);

    if(strcmp(url,"/api") == 0) {
        Json::Value methods;
        methods[0] = "uname";
        methods[1] = "disk";
        root["version"] = APIVERSION;
        root["methods"] = methods;
        response = writer.write(root);
        retCode = MHD_YES;
    } else if (strcmp(url, "/api/uname") == 0) {
        response = writer.write(getUname());
        retCode = MHD_YES;
    } else if (strcmp(url, "/api/disk") == 0) {
        response = writer.write(getDisk());
        retCode = MHD_YES;
    }

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
