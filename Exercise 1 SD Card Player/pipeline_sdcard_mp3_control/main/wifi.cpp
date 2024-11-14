#ifndef WIFI_CPP
#define WIFI_CPP


#include <stdio.h>
#include <string.h>
#include <cstring>  // For strncpy
#include <memory>

#include <esp_wifi.h>
#include <esp_netif.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "lwip/inet.h"
#include "esp_log.h"

#include "esp_websocket_client.h"
#include "esp_http_server.h"

#include "audio.cpp"

#include "esp_system.h" // Include ESP-IDF system header for esp_restart()

#include "cJSON.h"

#include "esp_http_server.h"

#include "http_app.h"
#include "wifi_manager.h"

namespace Wifi{
	static const char TAG[] = "WIFI";
    esp_err_t websocket_handler(httpd_req_t *req);

    httpd_req_t *websocket_req_handle = NULL;


	void cb_connection_ok(void *pvParameter){
		ip_event_got_ip_t* param = (ip_event_got_ip_t*)pvParameter;

		/* transform IP to human readable string */
		char str_ip[16];
		esp_ip4addr_ntoa(&param->ip_info.ip, str_ip, IP4ADDR_STRLEN_MAX);

		ESP_LOGI(TAG, "I have a connection and my IP is %s!", str_ip);
	}

    esp_err_t send_websocket_message(httpd_req_t *req, const char *message) {
        httpd_ws_frame_t ws_pkt;
        memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
        ws_pkt.type = HTTPD_WS_TYPE_TEXT;
        ws_pkt.payload = (uint8_t *)message;
        ws_pkt.len = strlen(message);

        esp_err_t ret = httpd_ws_send_frame(req, &ws_pkt);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send WebSocket message: %d", ret);
        }
        return ret;
    }

    esp_err_t websocket_handler(httpd_req_t *req) {
        websocket_req_handle = req;

        // Check if the request is a WebSocket handshake
        if (strcmp(req->uri, "/ws") == 0) {
            if (req->method == HTTP_GET){
                ESP_LOGI(TAG, "Handshake done, the new connection was opened");
                return ESP_OK;
            }
        }

        else if (req->method == HTTP_GET) {
            // This is the initial WebSocket connection
            httpd_ws_frame_t ws_pkt;
            memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
            ws_pkt.type = HTTPD_WS_TYPE_TEXT;
            ws_pkt.payload = (uint8_t*)"Connected to ESP32 WebSocket!";
            ws_pkt.len = strlen((const char*)ws_pkt.payload);
            return httpd_ws_send_frame(req, &ws_pkt);
        }

        else{
            httpd_ws_frame_t ws_pkt;
            uint8_t *buf = NULL;
            memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
            ws_pkt.type = HTTPD_WS_TYPE_TEXT;
            esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
            if (ret != ESP_OK){
                ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
                return ret;
            }

            if (ws_pkt.len){
                buf = (uint8_t*)calloc(1, ws_pkt.len + 1);
                if (buf == NULL)
                {
                    ESP_LOGE(TAG, "Failed to calloc memory for buf");
                    return ESP_ERR_NO_MEM;
                }
                ws_pkt.payload = buf;
                ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
                if (ret != ESP_OK){
                    ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
                    free(buf);
                    return ret;
                }
                ESP_LOGI(TAG, "Got packet with message: %s", ws_pkt.payload);
                cJSON *json = cJSON_Parse((const char *)ws_pkt.payload);
                cJSON *command = cJSON_GetObjectItem(json, "command");

                if(command != NULL && cJSON_IsString(command)){

                    if(strcmp(command->valuestring, "play") == 0){
                        ESP_LOGI(TAG, "Play button is pressed");
                        Audio::pipeline_play();
                    }

                    if(strcmp(command->valuestring, "pause") == 0){
                        ESP_LOGI(TAG, "Pause button is pressed");
                        Audio::pipeline_pause();
                        
                    }

                    if(strcmp(command->valuestring, "next") == 0){
                        ESP_LOGI(TAG, "Next button is pressed");
                        Audio::pipeline_next();
                        
                    }

                    if(strcmp(command->valuestring, "previous") == 0){
                        ESP_LOGI(TAG, "Previous button is pressed");
                        Audio::pipeline_previous();

                    }
                    Audio::pipeline_show_song();
                }
            }
        }

        return ESP_OK;
    }               

	void init(){
        ESP_LOGI(__FILE__, "init start");

        uint8_t ssid[32] = "Music Player";
        uint8_t pwd[64] = "password123";

        strncpy((char*)wifi_settings.ap_ssid, (const char*)ssid, sizeof(wifi_settings.ap_ssid) - 1);
        wifi_settings.ap_ssid[sizeof(wifi_settings.ap_ssid) - 1] = '\0';  // Null-terminate

        strncpy((char*)wifi_settings.ap_pwd, (const char*)pwd, sizeof(wifi_settings.ap_pwd) - 1);
        wifi_settings.ap_pwd[sizeof(wifi_settings.ap_pwd) - 1] = '\0';  // Null-terminate

        wifi_manager_start();

        http_app_set_handler_hook(HTTP_PUT, &websocket_handler);
    
		wifi_manager_set_callback(WM_EVENT_STA_GOT_IP, &cb_connection_ok);
        
        ESP_LOGI(__FILE__, "init complete");
	}
}

#endif