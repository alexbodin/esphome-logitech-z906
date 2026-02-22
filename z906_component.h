#ifndef Z906_COMPONENT_H
#define Z906_COMPONENT_H

#include "esphome.h"
#include "Z906.h"
#include "esphome/components/uart/uart.h"

class Z906Component : public Component
{
public:
    Z906Component(uart::UARTComponent *uart_comp) : uart_component(uart_comp)
    {
        z906 = new Z906(uart_comp);
    }

    // ESPHome calls this method once to initialize the component
    void setup() override
    {
        z906->read_full_status();
    }

    uint8_t *read_full_status()
    {
        // Update the status and print it for debugging
        z906->read_full_status();
        return z906->status;
    }

    // Input Selection
    // Input 1 - 3.5mm TRS, 6 channels
    void input_1()
    {
        z906->cmd({MUTE_ON,
                   SELECT_INPUT_1,
                   SELECT_EFFECT_NO,
                   MUTE_OFF});
    }
    // Input 2 - RCA, 2 channels
    void input_2()
    {
        z906->cmd({MUTE_ON,
                   SELECT_INPUT_2,
                   SELECT_EFFECT_41,
                   MUTE_OFF});
    }
    // Input 3 - Optical
    void input_3()
    {
        z906->cmd({MUTE_ON,
                   SELECT_INPUT_3,
                   SELECT_EFFECT_41,
                   MUTE_OFF});
    }
    // Input 4 - Optical
    void input_4()
    {
        z906->cmd({MUTE_ON,
                   SELECT_INPUT_4,
                   SELECT_EFFECT_41,
                   MUTE_OFF});
    }
    // Input 5 - COAXIAL
    void input_5()
    {
        z906->cmd({MUTE_ON,
                   SELECT_INPUT_5,
                   SELECT_EFFECT_41,
                   MUTE_OFF});
    }
    // Input 6 - AUX
    void input_aux()
    {
        z906->cmd({MUTE_ON,
                   SELECT_INPUT_AUX,
                   SELECT_EFFECT_41,
                   MUTE_OFF});
    }

    int get_temperature()
    {
        return z906->get_temperature();
    }

    // Generic method to interact with the Z906
    void cmd(uint8_t cmd)
    {
        z906->cmd(cmd);
    }

    void cmd(std::vector<uint8_t> cmd)
    {
        z906->cmd(cmd);
    }

    void send_command_str(std::string input)
    {
        std::vector<uint8_t> bytes;

        // Replace commas with spaces
        for (auto &c : input)
        {
            if (c == ',')
                c = ' ';
        }

        const char *ptr = input.c_str();
        char *end;

        while (*ptr != '\0')
        {
            // Skip spaces
            while (*ptr == ' ')
            {
                ptr++;
            }

            if (*ptr == '\0')
            {
                break;
            }

            unsigned long value = strtoul(ptr, &end, 0);

            if (ptr == end)
            {
                break; // no conversion
            }

            if (value <= 255)
            {
                bytes.push_back((uint8_t)value);
            }

            ptr = end;
        }

        if (!bytes.empty())
        {
            z906->cmd(bytes);
            z906->read_buffer();
        }
        else
        {
            ESP_LOGW("z906", "No valid bytes parsed");
        }
    }

    Z906 *z906;

private:
    uart::UARTComponent *uart_component;
};

#endif