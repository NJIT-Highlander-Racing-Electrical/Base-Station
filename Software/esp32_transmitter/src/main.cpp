#include <heltec_unofficial.h>
#include <baja_data_compression.h>

#include <BajaCAN_forLORA.h>
#include <driver/twai.h>

void setup() {
    Serial.begin(115200);

    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_1, GPIO_NUM_38, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        Serial.println("CAN driver installed");
        if (twai_start() == ESP_OK) {
            Serial.println("CAN driver started");
        }
    }
}

void loop() {
    twai_message_t message;
    message.identifier = 0x123;
    message.extd = 0;
    message.data_length_code = 8;
    for (int i = 0; i < 8; i++) message.data[i] = i;

    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        Serial.println("Message sent");
    }

    delay(1000);
}
