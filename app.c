/**
 * This sample program runs motors A, B, C & D using a IR Remote control on channels 1 & 2.
 *
 */

#include "ev3api.h"
#include "app.h"
#include <stdlib.h>

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

#define MINVAL(X, Y)  ((X) < (Y) ? (X) : (Y))
#define MAXVAL(X, Y)  ((X) > (Y) ? (X) : (Y))

const motor_port_t x_motor   = EV3_PORT_A;
const motor_port_t y_motor   = EV3_PORT_D;
const motor_port_t z_motor   = EV3_PORT_B;
const motor_port_t c_motor   = EV3_PORT_C;
const sensor_port_t ir_sensor = EV3_PORT_1;

void motor(motor_port_t m, int power)
{
    if (power)
        ev3_motor_set_power(m, power);
    else
        ev3_motor_stop(m, false);
}

void wait_no_button_pressed()
{
    while(1)
    {
        // wait 10 mili-seconds
        tslp_tsk(10);
        
        int stop = 1;
        for(int i = 0; i < 6; i++)
        {
            if (ev3_button_is_pressed((button_t)i))
                stop = 0;
        }
        if (stop) break;
    }
}

void control()
{
    while (1)
    {
        ir_remote_t val = ev3_infrared_sensor_get_remote(ir_sensor);
        for(int c = 0; c < 2; c++)
        {
            int redup    = (val.channel[c] & IR_RED_UP_BUTTON);
            int reddown  = (val.channel[c] & IR_RED_DOWN_BUTTON);
            int blueup   = (val.channel[c] & IR_BLUE_UP_BUTTON);
            int bluedown = (val.channel[c] & IR_BLUE_DOWN_BUTTON);
            
            if (c == 0) // x and y motors
            {
                // x motor
                if (redup)
                {
                    motor(x_motor, 10);
                }
                else if (reddown)
                {
                    motor(x_motor, -10);
                }
                else
                {
                    motor(x_motor, 0);
                }
                
                // y motor
                if (blueup)
                {
                    motor(y_motor, 10);
                }
                else if (bluedown)
                {
                    motor(y_motor, -10);
                }
                else
                {
                    motor(y_motor, 0);
                }
            }
            else // z and c motors
            {
                // z motor
                if (redup)
                {
                    motor(z_motor, -10);
                }
                else if (reddown)
                {
                    motor(z_motor, 10);
                }
                else
                {
                    motor(z_motor, 0);
                }
                
                // c motor
                if (blueup)
                {
                    motor(c_motor, -10);
                }
                else if (bluedown)
                {
                    motor(c_motor, 10);
                }
                else
                {
                    motor(c_motor, 0);
                }
            }
        }
        // wait 10 mili-seconds
        tslp_tsk(10);
    }
}

void main_task(intptr_t unused)
{
    // Draw information
    lcdfont_t font = EV3_FONT_MEDIUM;
    ev3_lcd_set_font(font);
    int32_t fontw, fonth;
    ev3_font_get_size(font, &fontw, &fonth);
    char lcdstr[100];
    ev3_lcd_draw_string("App: Run Motor", 0, 0);
    sprintf(lcdstr, "Port%c:X motor", 'A' + x_motor);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 1);
    sprintf(lcdstr, "Port%c:Y motor", 'A' + y_motor);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 2);
    sprintf(lcdstr, "Port%c:Z motor", 'A' + z_motor);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 3);
    sprintf(lcdstr, "Port%c:C motor", 'A' + c_motor);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 4);

    // Configure motors
    ev3_motor_config(x_motor, LARGE_MOTOR);
    ev3_motor_config(y_motor, LARGE_MOTOR);
    ev3_motor_config(z_motor, LARGE_MOTOR);
    ev3_motor_config(c_motor, MEDIUM_MOTOR);
    
    // Configure sensors
    ev3_sensor_config(ir_sensor, INFRARED_SENSOR);

    control();
}
