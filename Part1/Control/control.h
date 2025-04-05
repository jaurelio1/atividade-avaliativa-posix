#ifndef CONTROL_H
#define CONTROL_H

// Definições do sistema
#define UPDATE_INTERVAL 1
#define BLINK_INTERVAL 1
#define MAX_SPEED 200.0
#define MIN_SPEED 0.0
#define MAX_TEMP_MOTOR 140
#define MAX_ROTACAO 8000
#define KEY_SHM 1234

// Nomes para IPC
#define SHARED_MEM_NAME "/shared_sensors"
#define SEMAPHORE_NAME "/sensor_semaphore"
#define MQ_NAME "/controller_mqueue"

// Estruturas de dados
typedef struct {
    double velocidade;
    int rpm;
    double temperatura;
    bool acelera;
    bool freia;
    int pedal;
} SensorData;

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
    int pedal;
} SystemState;

typedef struct {
    long msg_type;
    char command;
    int pedal;
} Message;

typedef struct {
    bool is_on;
    time_t last_toggle;
} BlinkState;

void clear_screen();

void handle_signal(int signal);

void display_status(const SystemState *state);

void* blink_routine(void* arg);

void process_command(Message *msg, SensorData *shared_data);

void check_system_limits(SensorData *shared_data);

void update_system_state(SystemState *state, SensorData *shared_data);

#endif