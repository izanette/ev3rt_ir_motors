/**
 * This sample program runs motors A & D for 5 seconds.
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

/**
 * Left motor:   Port A
 * Right motor:  Port D
 * Touch sensor: Port 1
 */
const int left_motor   = EV3_PORT_A;
const int right_motor  = EV3_PORT_D;
const int touch_sensor = EV3_PORT_1;

void motor(int m, int power)
{
    if (power)
        ev3_motor_set_power(m, power);
    else
        ev3_motor_stop(m, false);
}

void motors(int left_power, int right_power)
{
    // Start motors
    motor(left_motor,  -left_power);
    motor(right_motor, -right_power);
    
    if (left_power || right_power)
        ev3_led_set_color(LED_GREEN);
    else
        ev3_led_set_color(LED_RED);
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
            if (ev3_button_is_pressed(i))
                stop = 0;
        }
        if (stop) break;
    }
}

void control()
{
    // Draw information
    lcdfont_t font = EV3_FONT_MEDIUM;
    ev3_lcd_set_font(font);
    int32_t fontw, fonth;
    ev3_font_get_size(font, &fontw, &fonth);
    char lcdstr[100];
    const char* lcdclean = "                    ";
    
    int speed = 0;
    int prevSpeed = 0;
    while (1)
    {
        prevSpeed = speed;
        if (ev3_button_is_pressed(UP_BUTTON))
            speed = MINVAL(100, speed + 10);
        else if (ev3_button_is_pressed(DOWN_BUTTON))
            speed = MAXVAL(-100, speed - 10);
        else if (ev3_button_is_pressed(ENTER_BUTTON))
            speed = 0;
        else if (ev3_touch_sensor_is_pressed(touch_sensor))
        {
            //while(ev3_touch_sensor_is_pressed(touch_sensor));
            speed = 0;
        }
        
        if (speed != prevSpeed)
        {
            motors(speed, speed);
            sprintf(lcdstr, "Speed = %d   ", speed);
            ev3_lcd_draw_string(lcdclean, 0, fonth * 3);
            ev3_lcd_draw_string(lcdstr, 0, fonth * 3);
            
            wait_no_button_pressed();
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
    sprintf(lcdstr, "Port%c:Left motor", 'A' + left_motor);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 1);
    sprintf(lcdstr, "Port%c:Right motor", 'A' + right_motor);
    ev3_lcd_draw_string(lcdstr, 0, fonth * 2);

    // Configure motors
    ev3_motor_config(left_motor, LARGE_MOTOR);
    ev3_motor_config(right_motor, LARGE_MOTOR);
    
    // Configure sensors
    //ev3_sensor_config(ir_sensor, ULTRASONIC_SENSOR);
    ev3_sensor_config(touch_sensor, TOUCH_SENSOR);

    control();
}
