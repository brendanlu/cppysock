#ifndef PARSE_JSON_CONFIG_H
#define PARSE_JSON_CONFIG_H

#include <stdio.h>
#include <string.h>

#define MAX_IP_ADDR_LEN     39
#define BUF                 100 

typedef struct {
    char addr[MAX_IP_ADDR_LEN]; 
    int port;
} config;

const config ERROR_CONFIG = {"ERROR", 0}; 

config create_config_from_file(const char* relpath) {
    FILE* configFile = fopen(relpath, "r");
    if (configFile == NULL) {
        return ERROR_CONFIG; 
    }
}

#endif