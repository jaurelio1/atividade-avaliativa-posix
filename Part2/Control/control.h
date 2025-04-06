#ifndef CONTROL_H
#define CONTROL_H

typedef struct {
    int acelerador;
    int freio;
    int farol_baixo;
    int farol_alto;
    int seta_esquerda;
    int seta_direita;
    double velocidade;
    int rpm;
    double temperatura;
    int cruise_control;
    double velocidade_cruzeiro;
} SystemState;


void update_system_state();

void update_actuators();

void process_command(Command cmd);

void verify_system_state();

void update_cruise_control();

void display_status();

void init_gpio();

void cleanup();

void handle_sigint(int sig);

#endif