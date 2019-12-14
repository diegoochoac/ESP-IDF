#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h" 

void start_spiffs(const char *path);

esp_err_t save_data(const char *path, char *data);

void close_spiffs();
void createFile(char *path);
void printFile(char *path);
void getconfig(char *path);
