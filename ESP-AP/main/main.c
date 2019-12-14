#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_system.h"
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

#include "esp_log.h"

#include "http_server.h"
#include "wifi_manager.h"
#include "spiffs_lib.h"
#include "wifi_lib.h"
#include "Parameters.h"


const char *TAG_name = "main";

void Network_Control(void *p)
{
	bool network_signal = false;
	
	for (;;)
	{
		if (!Isconnected())
		{
			//disable the default wifi logging
			esp_log_level_set("wifi", ESP_LOG_NONE);
			//start the HTTP Server task
			xTaskCreate(&http_server, "http_server", 4096, NULL, 5, &task_http_server);
			//start the wifi manager task
			xTaskCreate(&wifi_manager, "wifi_manager", 4096, NULL, 4, &task_wifi_manager);
			network_signal = false;
		}
		else
		{
			if (!network_signal)
			{
				ResetCount();
				network_signal = true;
			}
		}
		vTaskDelay(10000);
	}
}

void app_main()
{
	//start spiffs - file configuration
	start_spiffs("/spiffs");

	//print file
	printFile("/spiffs/configuration.json");

	//get default configuration
	getconfig("/spiffs/configuration.json");
	printf("Parameters saved\nSSID: %s, PASSWORD: %s,\n BROKER: %s,\n topic Input: %s,\n topic Output: %s\n", ConfigurationObject.ssid, ConfigurationObject.password, ConfigurationObject.broker_mqtt, ConfigurationObject.Input_topic, ConfigurationObject.Output_topic);

	//initialize flash memory
	nvs_flash_init();

	//Wifi Configuration
	wifi_begin(ConfigurationObject.ssid, ConfigurationObject.password);
	//wifi_begin("CEMUSA", "Ofiled@8031");
	// wifi_begin("ID", "batiscafo");

	xTaskCreatePinnedToCore(
		Network_Control,   /* Function to implement the task */
		"Network_Control", /* Name of the task */
		4096,			   /* Stack size in words */
		NULL,			   /* Task input parameter */
		3,				   /* Priority of the task */
		NULL,			   /* Task handle. */
		1);
}