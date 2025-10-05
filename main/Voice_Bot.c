#include "driver/dac_continuous.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <math.h>
#include <string.h>

static const char *TAG = "VOICE_BOT";

#define SAMPLE_RATE     16000   // 16 kHz - chuẩn thoại
#define FREQ_HZ         440     // 440 Hz - nốt La
#define NUM_SAMPLES     256     // số mẫu mỗi chu kỳ sóng

static uint8_t voice_data[NUM_SAMPLES];

static void generate_voice_data(void)
{
    for (int i = 0; i < NUM_SAMPLES; i++) {
        float theta = 2.0f * M_PI * FREQ_HZ * i / SAMPLE_RATE;
        voice_data[i] = (uint8_t)((sinf(theta) + 1.0f) * 127.5f); // giá trị 0–255
    }
    ESP_LOGI(TAG, "Generated %d samples for tone %d Hz", NUM_SAMPLES, FREQ_HZ);
}

void app_main(void)
{
    generate_voice_data();

    dac_continuous_handle_t dac_handle = NULL;

    dac_continuous_config_t dac_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_CH0,      // GPIO25
        .desc_num = 4,                          // số lượng descriptor DMA
        .buf_size = sizeof(voice_data),         // kích thước buffer
        .freq_hz = SAMPLE_RATE,                 // tần số lấy mẫu
        .clk_src = DAC_DIGI_CLK_SRC_DEFAULT,    // đúng trường trong IDF 5.5.1
        .offset = 0,                            // không offset
    };

    ESP_ERROR_CHECK(dac_continuous_new_channels(&dac_cfg, &dac_handle));
    ESP_ERROR_CHECK(dac_continuous_enable(dac_handle));

    ESP_LOGI(TAG, "DAC continuous mode started on GPIO25 (CH0)");

    while (1) {
        size_t bytes_written = 0;
        esp_err_t ret = dac_continuous_write(
            dac_handle,
            voice_data,
            sizeof(voice_data),
            &bytes_written,
            -1 // block cho đến khi xong
        );

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Wrote %d bytes to DAC", bytes_written);
        } else {
            ESP_LOGE(TAG, "DAC write failed: %s", esp_err_to_name(ret));
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Dừng DAC nếu cần
    // ESP_ERROR_CHECK(dac_continuous_disable(dac_handle));
    // ESP_ERROR_CHECK(dac_continuous_del_channels(dac_handle));
}
