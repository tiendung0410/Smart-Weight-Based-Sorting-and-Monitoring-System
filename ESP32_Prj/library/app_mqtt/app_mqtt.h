#ifndef __APP_MQTT_H
#define __APP_MQTT_H

void mqtt_app_start(void);
void mqtt_app_publish(char *topic, char *data, size_t len);
void mqtt_app_subscribe(char *topic);
void send_data_to_server(void);
void mqtt_app_set_data_handler_cb(void (*cb)(char *));

#endif
