#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "sensors.h"

// Variáveis globais
SensorData *shared_data;
sem_t *sem;

// Limpa a tela do terminal
void clear_screen() {
    printf("\033[H\033[J");
}

// Cálculo da temperatura do motor
double calculate_engine_temp(double velocidade, double rpm) {
    double temp_rise = rpm * FACTOR_ACELERACAO;
    double cooling_effect = velocidade * FATOR_RESFRIAMENTO_AR;
    return BASE_TEMP + temp_rise - cooling_effect;
}

// Cálculo da velocidade
double calculate_speed(int rpm, double velocidade) {
    if(shared_data->acelera) {
        double fator_variacao = rpm * 0.001;
        return fmin(velocidade + fator_variacao, MAX_SPEED);
    }
    else if(!shared_data->acelera && !shared_data->freia) {
        return velocidade;
    }
    else if(!shared_data->acelera && shared_data->freia) {
        double fator_variacao = (rand() % 10) + 1;
        return fmax(velocidade - fator_variacao, 0);
    }
    return velocidade;
}

// Cálculo do RPM
int calculate_rpm(int rpm_atual, int pedal) {
    // Calcula RPM ideal baseado na velocidade atual
    double velocidade_atual = shared_data->velocidade;
    
    // Se a velocidade for zero, RPM deve rapidamente voltar ao RPM_BASE
    if (velocidade_atual < 0.1) {
        if (rpm_atual > RPM_BASE) {
            return fmax(rpm_atual - 200, RPM_BASE);  // Redução mais rápida ao atingir velocidade zero
        }
        return RPM_BASE;
    }

    int rpm_ideal = RPM_BASE + ((velocidade_atual / MAX_SPEED) * (MAX_ROTACAO - RPM_BASE));
    
    if(shared_data->acelera) {
        float pedal_percent = pedal / 100.0;
        int fator_aceleracao = ((rand() % 21) + 50) * pedal_percent;
        int rpm_max = rpm_ideal + (MAX_ROTACAO - rpm_ideal) * pedal_percent;
        return fmin((rpm_atual + fator_aceleracao), rpm_max);
    }
    else if(!shared_data->acelera && shared_data->freia) {
        float pedal_percent = pedal / 100.0;
        // Redução mais agressiva do RPM durante frenagem
        int fator_desaceleracao = ((rand() % 101) + 150) * pedal_percent;
        
        // RPM mínimo baseado na velocidade atual
        int rpm_minimo = RPM_BASE + (velocidade_atual * 15);
        
        // Se estiver quase parando, reduz RPM mais rapidamente
        if (velocidade_atual < 5.0) {
            rpm_minimo = RPM_BASE;
            fator_desaceleracao *= 2;  // Dobra a taxa de desaceleração
        }
        
        return fmax((rpm_atual - fator_desaceleracao), rpm_minimo);
    }
    else if(!shared_data->acelera && !shared_data->freia) {
        // Ajuste gradual para o RPM ideal
        if(abs(rpm_atual - rpm_ideal) > 100) {
            return rpm_atual > rpm_ideal ? 
                   rpm_atual - 100 : 
                   rpm_atual + 50;
        }
        return rpm_ideal;
    }
    return rpm_atual;
}

// Simulação dos sensores
void *simulate_sensor(void *arg) {
    int sensor_id = *(int *)arg;
    free(arg);

    while (1) {
        sem_wait(sem);

        // Atualização dos sensores
        switch (sensor_id) {
            case 0:
                shared_data->rpm = calculate_rpm(shared_data->rpm, shared_data->pedal);
                break;
            case 1:
                shared_data->velocidade = calculate_speed(shared_data->rpm, shared_data->velocidade);
                break;
            case 2:
                shared_data->temperatura = calculate_engine_temp(shared_data->velocidade, shared_data->rpm);
                break;
        }

        // Exibição do painel (apenas pela thread 0)
        if (sensor_id == 0) {
            clear_screen();
            printf("\n╔══════════════════════════════════════╗\n");
            printf("║          PAINEL DO VEÍCULO           ║\n");
            printf("╠══════════════════════════════════════╣\n");
            printf("║  RPM............: %-8d           ║\n", shared_data->rpm);
            printf("║  Velocidade.....: %-8.2f km/h      ║\n", shared_data->velocidade);
            printf("║  Temperatura....: %-8.2f °C        ║\n", shared_data->temperatura);
            printf("╠══════════════════════════════════════╣\n");
            printf("║  Status.........: %-15s    ║\n", 
                   shared_data->acelera ? "Acelerando" : 
                   shared_data->freia ? "Freando" : "Neutro");
            printf("║  Pedal..........: %-3d%%               ║\n", shared_data->pedal);
            printf("╚══════════════════════════════════════╝\n");
        }

        sem_post(sem);
        sleep(UPDATE_INTERVAL);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t threads[SENSOR_COUNT];

    // Inicialização da memória compartilhada
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Erro na criação da memória compartilhada");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, sizeof(SensorData)) == -1) {
        perror("Erro no ftruncate");
        exit(EXIT_FAILURE);
    }

    shared_data = mmap(NULL, sizeof(SensorData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("Erro no mmap");
        exit(EXIT_FAILURE);
    }

    // Inicialização dos dados
    shared_data->velocidade = VELOCIDADE_BASE;
    shared_data->rpm = RPM_BASE;
    shared_data->temperatura = BASE_TEMP;
    shared_data->acelera = false;
    shared_data->freia = false;
    shared_data->pedal = 0;

    // Criação do semáforo
    sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("Erro na criação do semáforo");
        exit(EXIT_FAILURE);
    }

    // Criação das threads
    for (int i = 0; i < SENSOR_COUNT; i++) {
        int *sensor_id = malloc(sizeof(int));
        *sensor_id = i;
        if (pthread_create(&threads[i], NULL, simulate_sensor, sensor_id) != 0) {
            perror("Erro na criação da thread");
            exit(EXIT_FAILURE);
        }
    }

    // Aguarda as threads
    for (int i = 0; i < SENSOR_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Limpeza
    sem_close(sem);
    sem_unlink(SEMAPHORE_NAME);
    munmap(shared_data, sizeof(SensorData));
    shm_unlink(SHARED_MEM_NAME);

    return 0;
}