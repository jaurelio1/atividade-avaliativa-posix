#ifndef SENSORS_H
#define SENSORS_H

void handle_sigint(int sig);

void update_measurements(unsigned long current_time);

void process_hall_sensors();

void init_gpio();

void init_shared_memory();

void display_status();

void cleanup();

#endif