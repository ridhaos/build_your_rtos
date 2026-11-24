/*
 *  This just simple code that showing how 
 *  in general simple embedded application
 *  looks like, for simple application you
 *  only need just task (read sensors, update
 *  display and process ).
 */


#include <stdio.h>

int main(void)
{
    hardware_init();

    while (1) {
        read_sensor();
        update_display();
        process_commands();
    }
}

