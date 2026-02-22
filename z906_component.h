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
        z906->print_status();
    }

    // ESPHome calls this method every loop
    void loop() override
    {
        z906->update();
    }

    // Generic method to interact with the Z906
    int send_command(uint8_t cmd)
    {
        return z906->cmd(cmd);
    }

    // Standby Controls
    int standby_on()
    {
        return z906->cmd(PWM_ON);
    }
    int standby_off()
    {
        return z906->cmd(PWM_OFF);
    }

    // Input Selection
    // Input 1 - 3.5mm TRS, 6 channels
    int input_1()
    {
        return z906->cmd({LEVEL_MAIN_DOWN, SELECT_INPUT_1, SELECT_EFFECT_NO, LEVEL_MAIN_UP});
    }
    // Input 2 - RCA, 2 channels
    int input_2()
    {
        return z906->cmd({LEVEL_MAIN_DOWN,
                          SELECT_INPUT_2,
                          SELECT_EFFECT_21,
                          LEVEL_MAIN_UP});
    }
    // Input 3 - Optical
    int input_3()
    {
        return z906->cmd({LEVEL_MAIN_DOWN,
                          SELECT_INPUT_3,
                          SELECT_EFFECT_3D,
                          LEVEL_MAIN_UP});
    }
    // Input 4 - Optical
    int input_4()
    {
        return z906->cmd({LEVEL_MAIN_DOWN,
                          SELECT_INPUT_4,
                          SELECT_EFFECT_3D,
                          LEVEL_MAIN_UP});
    }
    // Input 5 - COAXIAL
    int input_5()
    {
        return z906->cmd({LEVEL_MAIN_DOWN,
                          SELECT_INPUT_5,
                          SELECT_EFFECT_21,
                          LEVEL_MAIN_UP});
    }
    // Input 6 - AUX
    int input_aux()
    {
        return z906->cmd({LEVEL_MAIN_DOWN,
                          SELECT_INPUT_AUX,
                          SELECT_EFFECT_21,
                          LEVEL_MAIN_UP});
    }

    // Volume Controls
    int main_volume_up()
    {
        return z906->cmd(LEVEL_MAIN_UP);
    }
    int main_volume_down()
    {
        return z906->cmd(LEVEL_MAIN_DOWN);
    }
    int sub_volume_up()
    {
        return z906->cmd(LEVEL_SUB_UP);
    }
    int sub_volume_down()
    {
        return z906->cmd(LEVEL_SUB_DOWN);
    }
    int center_volume_up()
    {
        return z906->cmd(LEVEL_CENTER_UP);
    }
    int center_volume_down()
    {
        return z906->cmd(LEVEL_CENTER_DOWN);
    }
    int rear_volume_up()
    {
        return z906->cmd(LEVEL_REAR_UP);
    }
    int rear_volume_down()
    {
        return z906->cmd(LEVEL_REAR_DOWN);
    }
    int mute_on()
    {
        return z906->cmd(MUTE_ON);
    }
    int mute_off()
    {
        return z906->cmd(MUTE_OFF);
    }

    // Effects
    int effect_3D()
    {
        return z906->cmd(SELECT_EFFECT_3D);
    }
    int effect_41()
    {
        return z906->cmd(SELECT_EFFECT_41);
    }
    int effect_21()
    {
        return z906->cmd(SELECT_EFFECT_21);
    }
    int effect_no()
    {
        return z906->cmd(SELECT_EFFECT_NO);
    }

    // Requests
    int get_power_status()
    {
        return z906->request(z906->STATUS_STBY);
    }
    int get_current_input()
    {
        return z906->request(z906->STATUS_CURRENT_INPUT);
    }
    int get_main_volume()
    {
        return z906->request(z906->STATUS_MAIN_LEVEL);
    }
    int get_rear_volume()
    {
        return z906->request(z906->STATUS_REAR_LEVEL);
    }
    int get_center_volume()
    {
        return z906->request(z906->STATUS_CENTER_LEVEL);
    }
    int get_sub_volume()
    {
        return z906->request(z906->STATUS_SUB_LEVEL);
    }
    int get_temperature()
    {
        return z906->main_sensor();
    }
    void get_full_status()
    {
        z906->print_status();
    }

private:
    Z906 *z906;
    uart::UARTComponent *uart_component;
};

#endif