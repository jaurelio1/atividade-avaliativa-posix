#ifndef DISPLAY_H
#define DISPLAY_H

void send_command(Command cmd);

void handle_gpio_event(int pin);

void init_gpio();

void cleanup();

void handle_sigint(int sig);

#endif