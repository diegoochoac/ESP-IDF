#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h" 
#include "cJSON.h"
#include "Parameters.h"
#include "spiffs_lib.h"

static const char TAG[] = "SPIFFS";

void start_spiffs(const char *path)
{

    ESP_LOGI("SPIFFS System", "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = path,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    else
    {
        ESP_LOGI(TAG, "SPIFFS CORRECT!!");
    }
}


void close_spiffs(){
    esp_vfs_spiffs_unregister(NULL);
}

esp_err_t save_data(const char *path, char *data){

    int i = 0;
    char *p = strtok (data, "&");
    char *array[19];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, "&");
    }

    char *type, *SSID, *PASSWORD, *mode, *ip1, *ip2, *ip3, *ip4, *mask1, *mask2, *mask3, *mask4, *gate1, *gate2, *gate3, *gate4, *dns1, *dns2, *dns3, *dns4;

    type = strtok(array[0], "=");
    SSID = strtok(NULL, "=");
    type = strtok(array[1], "=");
    PASSWORD = strtok(NULL, "=");
    type = strtok(array[2], "=");
    mode = strtok(NULL, "=");
    type = strtok(array[3], "=");
    ip1 = strtok(NULL, "=");
    type = strtok(array[4], "=");
    ip2 = strtok(NULL, "=");
    type = strtok(array[5], "=");
    ip3 = strtok(NULL, "=");
    type = strtok(array[6], "=");
    ip4 = strtok(NULL, "=");
    type = strtok(array[7], "=");
    mask1 = strtok(NULL, "=");
    type = strtok(array[8], "=");
    mask2 = strtok(NULL, "=");
    type = strtok(array[9], "=");
    mask3 = strtok(NULL, "=");
    type = strtok(array[10], "=");
    mask4 = strtok(NULL, "=");
    type = strtok(array[11], "=");
    gate1 = strtok(NULL, "=");
    type = strtok(array[12], "=");
    gate2 = strtok(NULL, "=");
    type = strtok(array[13], "=");
    gate3 = strtok(NULL, "=");
    type = strtok(array[14], "=");
    gate4 = strtok(NULL, "=");
    type = strtok(array[15], "=");
    dns1 = strtok(NULL, "=");
    type = strtok(array[16], "=");
    dns2 = strtok(NULL, "=");
    type = strtok(array[17], "=");
    dns3 = strtok(NULL, "=");
    type = strtok(array[18], "=");
    dns4 = strtok(NULL, "=");

    ESP_LOGI(TAG, "Saving Data in %s",path);

    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }

    char *out;
    cJSON *root, *IPnumbers, *Gatenumbers, *Masknumbers, *DNSnumbers;

    
    root = cJSON_CreateObject();
    IPnumbers = cJSON_CreateArray();
    DNSnumbers = cJSON_CreateArray();
    Gatenumbers = cJSON_CreateArray();
    Masknumbers = cJSON_CreateArray();
   
    if(strcmp(mode,"0")==0)
    { 
        cJSON_AddItemToObject(root, "modo IP", cJSON_CreateBool(false));
    }
    else
    {
        cJSON_AddItemToObject(root, "modo IP", cJSON_CreateBool(true));
    }

    cJSON_AddStringToObject(root, "ssid", SSID);
    cJSON_AddStringToObject(root, "password", PASSWORD);
    cJSON_AddItemToObject(root, "IP", IPnumbers);

    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(atof(ip1)));
    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(atof(ip2)));
    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(atof(ip3)));
    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(atof(ip4)));

    cJSON_AddItemToObject(root, "dns", DNSnumbers);

    cJSON_AddItemToArray(DNSnumbers, cJSON_CreateNumber(atof(dns1)));
    cJSON_AddItemToArray(DNSnumbers, cJSON_CreateNumber(atof(dns2)));
    cJSON_AddItemToArray(DNSnumbers, cJSON_CreateNumber(atof(dns3)));
    cJSON_AddItemToArray(DNSnumbers, cJSON_CreateNumber(atof(dns4)));

    cJSON_AddItemToObject(root, "gateway", Gatenumbers);
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(atof(gate1)));
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(atof(gate2)));
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(atof(gate3)));
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(atof(gate4)));

    cJSON_AddItemToObject(root, "mask", Masknumbers);
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(atof(mask1)));
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(atof(mask2)));
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(atof(mask3)));
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(atof(mask4)));

    cJSON_AddStringToObject(root, "pin", "1234");
    cJSON_AddStringToObject(root, "broker", "mqtt://platform.agrum.co");
    cJSON_AddStringToObject(root, "Username", "");
    cJSON_AddStringToObject(root, "Password_broker", "");
    cJSON_AddStringToObject(root, "Topic entrada", "airis/1155/command");
    cJSON_AddStringToObject(root, "Topic salida", "airis/1155/report");
    cJSON_AddStringToObject(root, "ID", "1155");
    cJSON_AddStringToObject(root, "SoftVersion", "1.0.0");
    cJSON_AddStringToObject(root, "HardVersion", "1.0.0");
    cJSON_AddStringToObject(root, "Zona Horaria", "-5:00");

    out = cJSON_Print(root);
    printf("%s\n", out);

    
    cJSON_Delete(root);
    fprintf(f, out);
    fclose(f);

    ESP_LOGI(TAG, "Data Saved Correctly");

    

    return ESP_OK;


}

void createFile(char *path){
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen(path, "w");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }

    char *out;
    cJSON *root, *IPnumbers, *Gatenumbers, *Masknumbers, *dnsnumbers;


    root = cJSON_CreateObject();
    IPnumbers = cJSON_CreateArray();
    dnsnumbers = cJSON_CreateArray();
    Gatenumbers = cJSON_CreateArray();
    Masknumbers = cJSON_CreateArray();

  
    cJSON_AddItemToObject(root, "modo IP", cJSON_CreateBool(false));
    cJSON_AddStringToObject(root, "ssid", "DinoxInd");
    cJSON_AddStringToObject(root, "password", "Dinorum");
    cJSON_AddItemToObject(root, "IP", IPnumbers);

    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(192));
    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(168));
    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(0));
    cJSON_AddItemToArray(IPnumbers, cJSON_CreateNumber(7));

    cJSON_AddItemToObject(root, "gateway", Gatenumbers);
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(192));
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(168));
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(0));
    cJSON_AddItemToArray(Gatenumbers, cJSON_CreateNumber(1));

    cJSON_AddItemToObject(root, "mask", Masknumbers);
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(192));
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(168));
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(0));
    cJSON_AddItemToArray(Masknumbers, cJSON_CreateNumber(7));

    cJSON_AddItemToObject(root, "dns", dnsnumbers);
    cJSON_AddItemToArray(dnsnumbers, cJSON_CreateNumber(8));
    cJSON_AddItemToArray(dnsnumbers, cJSON_CreateNumber(8));
    cJSON_AddItemToArray(dnsnumbers, cJSON_CreateNumber(8));
    cJSON_AddItemToArray(dnsnumbers, cJSON_CreateNumber(8));

    cJSON_AddStringToObject(root, "pin", "1234");
    cJSON_AddStringToObject(root, "broker", "mqtt://platform.agrum.co");
    cJSON_AddStringToObject(root, "Username", "");
    cJSON_AddStringToObject(root, "Password_broker", "");
    cJSON_AddStringToObject(root, "Topic entrada", "airis/1155/command");
    cJSON_AddStringToObject(root, "Topic salida", "airis/1155/report");
    cJSON_AddStringToObject(root, "ID", "1155");
    cJSON_AddStringToObject(root, "SoftVersion", "1.0.0");
    cJSON_AddStringToObject(root, "HardVersion", "1.0.0");
    cJSON_AddStringToObject(root, "Zona Horaria", "-5:00");

    out = cJSON_Print(root);
    printf("%s\n", out);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    fprintf(f, out);
    fclose(f);

    ESP_LOGI(TAG, "File written");
}

void printFile(char *path)
{
    FILE *f = fopen(path, "r");
	if (f == NULL)
	{
		printf("NO hay archivo");
		createFile("/spiffs/configuration.json");
	}
	else
	{
		int c;
		while (1)
		{
			c = fgetc(f);
			if (feof(f))
			{
				break;
			}
			printf("%c", c);
		}
	}
}

/*! \brief getconfig.
 *
 *  This function reads the configuration present in the json file.
 * 
 *  \param path path_file.
 */
void getconfig(char *path)
{
	cJSON *root, *ssid, *password, *IP, *mode, *gateway, *mask, *pin, *broker, *Username;
	cJSON *Password_broker, *topicE, *topicS, *ID, *Soft, *Hard, *Zona, *dns;

	char buffer[1024];
	FILE *f = fopen(path, "r");
	fread(buffer, 1024, 1, f);
	fclose(f);

	root = cJSON_CreateObject();
	root = cJSON_Parse(buffer);

	mode = cJSON_GetObjectItem(root, "modo IP");
	ssid = cJSON_GetObjectItem(root, "ssid");
	password = cJSON_GetObjectItem(root, "password");
	IP = cJSON_GetObjectItem(root, "IP");
	dns = cJSON_GetObjectItem(root, "dns");
	gateway = cJSON_GetObjectItem(root, "gateway");
	mask = cJSON_GetObjectItem(root, "mask");
	pin = cJSON_GetObjectItem(root, "pin");
	broker = cJSON_GetObjectItem(root, "broker");
	Username = cJSON_GetObjectItem(root, "Username");
	Password_broker = cJSON_GetObjectItem(root, "Password_broker");
	topicE = cJSON_GetObjectItem(root, "Topic entrada");
	topicS = cJSON_GetObjectItem(root, "Topic salida");
	ID = cJSON_GetObjectItem(root, "ID");
	Soft = cJSON_GetObjectItem(root, "SoftVersion");
	Hard = cJSON_GetObjectItem(root, "HardVersion");
	Zona = cJSON_GetObjectItem(root, "Zona Horaria");

	if (cJSON_IsTrue(mode))
		ConfigurationObject.IP_mode = true;
	else
	{
		ConfigurationObject.IP_mode = false;
	}

	ConfigurationObject.ssid = cJSON_GetStringValue(ssid);
	ConfigurationObject.password = cJSON_GetStringValue(password);
	ConfigurationObject.pin_bt = cJSON_GetStringValue(pin);
	ConfigurationObject.broker_mqtt = cJSON_GetStringValue(broker);
	ConfigurationObject.Username = cJSON_GetStringValue(Username);
	ConfigurationObject.Password_broker = cJSON_GetStringValue(Password_broker);
	ConfigurationObject.Output_topic = cJSON_GetStringValue(topicS);
	ConfigurationObject.Input_topic = cJSON_GetStringValue(topicE);
	ConfigurationObject.ID = cJSON_GetStringValue(ID);
	ConfigurationObject.SoftVersion = cJSON_GetStringValue(Soft);
	ConfigurationObject.HardVersion = cJSON_GetStringValue(Hard);
	ConfigurationObject.time_zone = cJSON_GetStringValue(Zona);

	for (int i = 0; i < cJSON_GetArraySize(IP); i++)
		ConfigurationObject.ip_c[i] = (cJSON_GetArrayItem(IP, i)->valueint);

	for (int i = 0; i < cJSON_GetArraySize(gateway); i++)
		ConfigurationObject.gw_c[i] = (cJSON_GetArrayItem(gateway, i)->valueint);

	for (int i = 0; i < cJSON_GetArraySize(mask); i++)
		ConfigurationObject.msk_c[i] = (cJSON_GetArrayItem(mask, i)->valueint);

	for (int i = 0; i < cJSON_GetArraySize(dns); i++)
		ConfigurationObject.dns_c[i] = (cJSON_GetArrayItem(dns, i)->valueint);
}