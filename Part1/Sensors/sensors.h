#ifndef SENSORS_H
#define SENSORS_H

// Estrutura de dados compartilhada
typedef struct {
    double velocidade;
    int rpm;
    double temperatura;
    bool acelera;
    bool freia;
    int pedal;
} SensorData;

typedef struct {
    long msg_type;
    char command;
    int pedal;
} Message;

// Definições do sistema
#define SENSOR_COUNT 3
#define SHARED_MEM_NAME "/shared_sensors"
#define SEMAPHORE_NAME "/sensor_semaphore"
#define UPDATE_INTERVAL 1
#define MQ_NAME "/controller_mqueue"

// Constantes de operação
#define FACTOR_ACELERACAO 0.01
#define FATOR_RESFRIAMENTO_AR 0.05
#define MAX_TEMP_MOTOR 140
#define MAX_SPEED 200.0
#define MAX_ROTACAO 8000
#define BASE_TEMP 80
#define RPM_BASE 800
#define VELOCIDADE_BASE 0

void clear_screen();

double calculate_engine_temp(double velocidade, double rpm);

double calculate_speed(int rpm, double velocidade);

int calculate_rpm(int rpm_atual, int pedal);

void *simulate_sensor(void *arg);

#endif