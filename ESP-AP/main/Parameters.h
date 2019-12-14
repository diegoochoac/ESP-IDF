#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/event_groups.h"
#include "mdns.h"
#include "lwip/api.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "cJSON.h"
#include "soc/timer_group_struct.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "driver/spi_master.h"
#include <math.h>


struct Config
{
	bool IP_mode;
	char *ssid;
	char *password;
	int ip_c[4];
	int gw_c[4];
	int dns_c[4];
	int msk_c[4];
	char *pin_bt;
	char *broker_mqtt;
	char *Username;
	char *Password_broker;
	char *Output_topic;
	char *Input_topic;
	char *ID;
	char *SoftVersion;
	char *HardVersion;
	char *time_zone;
};

struct Config ConfigurationObject;


TaskHandle_t task_http_server;
TaskHandle_t task_wifi_manager;
