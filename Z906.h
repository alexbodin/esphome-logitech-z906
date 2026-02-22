#ifndef Z906_H
#define Z906_H

#include "esphome.h"
#include "esphome/components/uart/uart.h"

#define SERIAL_TIME_OUT 1000

#define STATUS_TOTAL_LENGTH 0x17
#define ACK_TOTAL_LENGTH 0x05
#define TEMP_TOTAL_LENGTH 0x0A

// Single Commands

#define SELECT_INPUT_1 0x02
#define SELECT_INPUT_2 0x05
#define SELECT_INPUT_3 0x03
#define SELECT_INPUT_4 0x04
#define SELECT_INPUT_5 0x06
#define SELECT_INPUT_AUX 0x07

#define LEVEL_MAIN_UP 0x08
#define LEVEL_MAIN_DOWN 0x09
#define LEVEL_SUB_UP 0x0A
#define LEVEL_SUB_DOWN 0x0B
#define LEVEL_CENTER_UP 0x0C
#define LEVEL_CENTER_DOWN 0x0D
#define LEVEL_REAR_UP 0x0E
#define LEVEL_REAR_DOWN 0x0F

#define PWM_OFF 0x10
#define PWM_ON 0x11

#define SELECT_EFFECT_3D 0x14
#define SELECT_EFFECT_41 0x15
#define SELECT_EFFECT_21 0x16
#define SELECT_EFFECT_NO 0x35

#define EEPROM_SAVE 0x36

#define MUTE_ON 0x38
#define MUTE_OFF 0x39

#define BLOCK_INPUTS 0x22
#define RESET_PWR_UP_TIME 0x30
#define NO_BLOCK_INPUTS 0x33

// Double commmands

#define MAIN_LEVEL 0x03
#define READ_LEVEL 0x04
#define CENTER_LEVEL 0x05
#define SUB_LEVEL 0x06

// Requests

#define VERSION 0xF0
#define CURRENT_INPUT 0xF1
#define GET_INPUT_GAIN 0x2F
#define GET_TEMP 0x25
#define GET_PWR_UP_TIME 0x31
#define GET_STATUS 0x34

// MASK

#define EFFECT_3D 0x00
#define EFFECT_21 0x01
#define EFFECT_41 0x02
#define EFFECT_NO 0x03

#define SPK_NONE 0x00
#define SPK_ALL 0xFF
#define SPK_FR 0x01
#define SPK_FL 0x10
#define SPK_RR 0x02
#define SPK_RL 0x08
#define SPK_CENTER 0x04
#define SPK_SUB 0x20

class Z906
{
public:
    Z906(uart::UARTComponent *uart);

    uint8_t status[STATUS_TOTAL_LENGTH];

    void cmd(uint8_t);
    void cmd(std::vector<uint8_t>);
    int update();
    void read_buffer();
    void read_full_status();
    void flush_rx();

    uint8_t get_temperature();

    // Status constants (made public for component access)
    const uint8_t STATUS_STX = 0x00;
    const uint8_t STATUS_MODEL = 0x01;
    const uint8_t STATUS_LENGTH = 0x02;
    const uint8_t STATUS_MAIN_LEVEL = 0x03;
    const uint8_t STATUS_REAR_LEVEL = 0x04;
    const uint8_t STATUS_CENTER_LEVEL = 0x05;
    const uint8_t STATUS_SUB_LEVEL = 0x06;
    const uint8_t STATUS_CURRENT_INPUT = 0x07;
    const uint8_t STATUS_UNKNOWN = 0x08;
    const uint8_t STATUS_FX_INPUT_4 = 0x09;
    const uint8_t STATUS_FX_INPUT_5 = 0x0A;
    const uint8_t STATUS_FX_INPUT_2 = 0x0B;
    const uint8_t STATUS_FX_INPUT_AUX = 0x0C;
    const uint8_t STATUS_FX_INPUT_1 = 0x0D;
    const uint8_t STATUS_FX_INPUT_3 = 0x0E;
    const uint8_t STATUS_SPDIF_STATUS = 0x0F;
    const uint8_t STATUS_SIGNAL_STATUS = 0x10;
    const uint8_t STATUS_VER_A = 0x11;
    const uint8_t STATUS_VER_B = 0x12;
    const uint8_t STATUS_VER_C = 0x13;
    const uint8_t STATUS_STBY = 0x14;
    const uint8_t STATUS_AUTO_STBY = 0x15;
    const uint8_t STATUS_CHECKSUM = 0x16;

private:
    const uint8_t EXP_STX = 0xAA;
    const uint8_t EXP_MODEL_STATUS = 0x0A;
    const uint8_t EXP_MODEL_TEMP = 0x0C;

    uart::UARTComponent *dev_uart;
    uint8_t LRC(uint8_t *, uint8_t);
};

// Implementation inline to avoid linker errors with ESPHome includes

inline Z906::Z906(uart::UARTComponent *uart)
{
    dev_uart = uart;
}

// Longitudinal Redundancy Check {-1,-1}
inline uint8_t Z906::LRC(uint8_t *pData, uint8_t length)
{
    uint8_t LRC = 0;
    for (int i = 1; i < length - 1; i++)
        LRC -= pData[i];
    return LRC;
}

inline void Z906::flush_rx()
{
    uint8_t dummy;
    while (dev_uart->available() > 0)
    {
        dev_uart->read_byte(&dummy);
    }
}

inline int Z906::update()
{
    // Small delay to ensure any previous command responses have been received
    delay(500);
    flush_rx();
    uint8_t status_cmd = GET_STATUS;
    dev_uart->write_array(&status_cmd, 1);

    unsigned long currentMillis = millis();

    while (dev_uart->available() < STATUS_TOTAL_LENGTH)
        if (millis() - currentMillis > SERIAL_TIME_OUT)
            return 0;

    for (int i = 0; i < STATUS_TOTAL_LENGTH; i++)
        dev_uart->read_byte(&status[i]);

    if (status[STATUS_STX] != EXP_STX)
    {
        ESP_LOGD("Z906", "Invalid STX: 0x%02X", status[STATUS_STX]);
        return 0;
    }
    if (status[STATUS_MODEL] != EXP_MODEL_STATUS)
    {
        ESP_LOGD("Z906", "Invalid MODEL: 0x%02X", status[STATUS_MODEL]);
        return 0;
    }
    if (status[STATUS_CHECKSUM] == LRC(status, STATUS_TOTAL_LENGTH))
        return 1;

    ESP_LOGD("Z906", "Invalid CHECKSUM: 0x%02X", status[STATUS_CHECKSUM]);
    return 0;
}

inline void Z906::cmd(uint8_t cmd)
{
    flush_rx();
    ESP_LOGD("Z906", "Sending command: 0x%02X", cmd);
    dev_uart->write_array(&cmd, 1);
}

inline void Z906::cmd(std::vector<uint8_t> cmd)
{
    flush_rx();
    ESP_LOGD("Z906", "Sending command sequence:");
    for (size_t i = 0; i < cmd.size(); i++)
    {
        ESP_LOGI("Z906", "  0x%02X", cmd[i]);
    }
    dev_uart->write_array(cmd.data(), cmd.size());
}

inline void Z906::read_buffer()
{
    unsigned long currentMillis = millis();
    while (dev_uart->available() == 0)
        if (millis() - currentMillis > SERIAL_TIME_OUT)
        {
            ESP_LOGW("Z906", "No response received within timeout");
            return;
        }

    // Add a small delay to ensure all bytes have arrived before reading
    delay(500);

    ESP_LOGD("Z906", "Reading buffer, %d bytes available:", dev_uart->available());
    while (dev_uart->available() > 0)
    {
        uint8_t byte;
        dev_uart->read_byte(&byte);
        ESP_LOGD("Z906", "  0x%02X", byte);
    }
}

inline void Z906::read_full_status()
{
    update();
    ESP_LOGI("Z906", "Z906 Status:");
    ESP_LOGI("Z906", "\tMAIN_LEVEL: %d", status[STATUS_MAIN_LEVEL]);
    ESP_LOGI("Z906", "\tREAR_LEVEL: %d", status[STATUS_REAR_LEVEL]);
    ESP_LOGI("Z906", "\tCENTER_LEVEL: %d", status[STATUS_CENTER_LEVEL]);
    ESP_LOGI("Z906", "\tSUB_LEVEL: %d", status[STATUS_SUB_LEVEL]);
    ESP_LOGI("Z906", "\tCURRENT_INPUT: %d", status[STATUS_CURRENT_INPUT]);
    ESP_LOGI("Z906", "\tFX_INPUT_4: %d", status[STATUS_FX_INPUT_4]);
    ESP_LOGI("Z906", "\tFX_INPUT_5: %d", status[STATUS_FX_INPUT_5]);
    ESP_LOGI("Z906", "\tFX_INPUT_2: %d", status[STATUS_FX_INPUT_2]);
    ESP_LOGI("Z906", "\tFX_INPUT_AUX: %d", status[STATUS_FX_INPUT_AUX]);
    ESP_LOGI("Z906", "\tFX_INPUT_1: %d", status[STATUS_FX_INPUT_1]);
    ESP_LOGI("Z906", "\tFX_INPUT_3: %d", status[STATUS_FX_INPUT_3]);
    ESP_LOGI("Z906", "\tSPDIF_STATUS: %d", status[STATUS_SPDIF_STATUS]);
    ESP_LOGI("Z906", "\tSTBY: %d", status[STATUS_STBY]);
    ESP_LOGI("Z906", "\tAUTO_STBY: %d", status[STATUS_AUTO_STBY]);
}

inline uint8_t Z906::get_temperature()
{
    flush_rx();
    uint8_t temp_cmd = GET_TEMP;
    dev_uart->write_array(&temp_cmd, 1);

    unsigned long currentMillis = millis();

    while (dev_uart->available() < TEMP_TOTAL_LENGTH)
    {
        if (millis() - currentMillis > SERIAL_TIME_OUT)
        {
            ESP_LOGW("Z906", "Timeout waiting for temperature response");
            return 0;
        }
    }

    uint8_t temp[TEMP_TOTAL_LENGTH];

    for (int i = 0; i < TEMP_TOTAL_LENGTH; i++)
        dev_uart->read_byte(&temp[i]);

    if (temp[2] != EXP_MODEL_TEMP)
        return 0;

    ESP_LOGD("Z906", "Temperature response received, raw data: %d", temp[7]);
    return temp[7];
}

#endif
