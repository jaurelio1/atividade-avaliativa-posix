#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <mqueue.h>
#include "control.h"

// Variáveis globais
sem_t *sem;
volatile sig_atomic_t running = 1;
volatile sig_atomic_t paused = 0;
SystemState state = {0};
BlinkState seta_direita_state = {false, 0};
BlinkState seta_esquerda_state = {false, 0};
pthread_t blink_thread;

// Função para limpar a tela
void clear_screen() {
    printf("\033[H\033[J");
}

// Manipulador de sinais
void handle_signal(int signal) {
    if (signal == SIGUSR1) {
        paused = !paused;
    } else if (signal == SIGUSR2) {
        running = 0;
    }
}

// Exibição do painel
void display_status(const SystemState *state) {
    clear_screen();
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║                COMPUTADOR DE BORDO                ║\n");
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║                    VELOCÍMETRO                    ║\n");
    printf("║  ═══════════════════════════════════════════════  ║\n");
    printf("║     Velocidade Atual........: %-8.2f km/h       ║\n", state->velocidade);
    printf("║     Rotação do Motor.......: %-8d RPM         ║\n", state->rpm);
    printf("║     Temperatura do Motor...: %-8.2f °C          ║\n", state->temperatura);
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║                 FORÇA NOS PEDAIS                  ║\n");
    printf("║  ═══════════════════════════════════════════════  ║\n");

    printf("║     [");
    int pedal_force = state->pedal;
    for(int i = 0; i < 30; i++) {
        if(i < (pedal_force * 30 / 100)) {
            printf(state->acelerador ? "A" : (state->freio ? "F" : " "));
        } else {
            printf(" ");
        }
    }
    printf("] %3d%%         ║\n", pedal_force);

    printf("║     %-20s                          ║\n", 
           state->acelerador ? "ACELERADOR ATIVO" : 
           (state->freio ? "FREIO ATIVO" : "NEUTRO"));

    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║                 SISTEMA DE LUZES                  ║\n");
    printf("║  ═══════════════════════════════════════════════  ║\n");
    printf("║     Farol Baixo...: %-25s     ║\n", state->farol_baixo ? "LIGADO" : "DESLIGADO");
    printf("║     Farol Alto....: %-25s     ║\n", state->farol_alto ? "LIGADO" : "DESLIGADO");
    printf("║     Seta Direita..: %-25s     ║\n", state->seta_direita ? "LIGADA" : "DESLIGADA");
    printf("║     Seta Esquerda.: %-25s     ║\n", state->seta_esquerda ? "LIGADA" : "DESLIGADA");
    printf("╚═══════════════════════════════════════════════════╝\n");
}

// Rotina de controle das setas
void* blink_routine(void* arg) {
    while (running) {
        if (!paused) {
            time_t current_time = time(NULL);
            
            if (seta_direita_state.is_on && 
                current_time - seta_direita_state.last_toggle >= BLINK_INTERVAL) {
                state.seta_direita = !state.seta_direita;
                seta_direita_state.last_toggle = current_time;
            }
            
            if (seta_esquerda_state.is_on && 
                current_time - seta_esquerda_state.last_toggle >= BLINK_INTERVAL) {
                state.seta_esquerda = !state.seta_esquerda;
                seta_esquerda_state.last_toggle = current_time;
            }
        }
        sleep(1);
    }
    return NULL;
}

// Processamento de comandos
void process_command(Message *msg, SensorData *shared_data) {
    sem_wait(sem);
    switch (msg->command) {
        case '1': // Acelerador
            if(state.acelerador == 1 && msg->pedal == 0) {
                state.acelerador = 0;
                state.freio = 0;
                state.pedal = 0;
                shared_data->acelera = false;
                shared_data->freia = false;
                shared_data->pedal = 0;
            } else {
                state.acelerador = 1;
                state.freio = 0;
                state.pedal = msg->pedal;
                shared_data->acelera = true;
                shared_data->freia = false;
                shared_data->pedal = msg->pedal;
            }
            break;

        case '2': // Freio
            if(state.freio == 1 && msg->pedal == 0) {
                state.freio = 0;
                state.acelerador = 0;
                state.pedal = 0;
                shared_data->acelera = false;
                shared_data->freia = false;
                shared_data->pedal = 0;
            } else {
                state.freio = 1;
                state.acelerador = 0;
                state.pedal = msg->pedal;
                shared_data->acelera = false;
                shared_data->freia = true;
                shared_data->pedal = msg->pedal;
            }
            break;

        case '3': // Farol baixo
            state.farol_baixo = !state.farol_baixo;
            break;

        case '4': // Farol alto
            state.farol_alto = !state.farol_alto;
            break;

        case '5': // Seta direita
            seta_direita_state.is_on = !seta_direita_state.is_on;
            if (seta_direita_state.is_on) {
                seta_direita_state.last_toggle = time(NULL);
                seta_esquerda_state.is_on = false;
                state.seta_esquerda = 0;
                state.seta_direita = 1;
            } else {
                state.seta_direita = 0;
            }
            break;

        case '6': // Seta esquerda
            seta_esquerda_state.is_on = !seta_esquerda_state.is_on;
            if (seta_esquerda_state.is_on) {
                seta_esquerda_state.last_toggle = time(NULL);
                seta_direita_state.is_on = false;
                state.seta_direita = 0;
                state.seta_esquerda = 1;
            } else {
                state.seta_esquerda = 0;
            }
            break;

        case '0': // Encerrar
            running = 0;
            break;

        default:
            printf("Comando inválido: %c\n", msg->command);
    }
    sem_post(sem);
}

// Verificação dos limites do sistema
void check_system_limits(SensorData *shared_data) {
    sem_wait(sem);
    if (state.velocidade > MAX_SPEED) {
        shared_data->acelera = false;
        shared_data->freia = true;
    } else if (state.velocidade <= MIN_SPEED && !state.freio) {
        shared_data->acelera = false;
        shared_data->freia = false;
    } else if (state.temperatura > MAX_TEMP_MOTOR) {
        shared_data->acelera = false;
        shared_data->freia = true;
    } else if (state.rpm >= MAX_ROTACAO) {
        shared_data->acelera = false;
        shared_data->freia = false;
    }
    sem_post(sem);
}

// Atualização do estado do sistema
void update_system_state(SystemState *state, SensorData *shared_data) {
    sem_wait(sem);
    state->velocidade = shared_data->velocidade;
    state->rpm = shared_data->rpm;
    state->temperatura = shared_data->temperatura;
    state->acelerador = shared_data->acelera;
    state->freio = shared_data->freia;
    state->pedal = shared_data->pedal;
    sem_post(sem);
}

int main() {
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    // Inicialização da memória compartilhada
    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Erro na memória compartilhada");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, sizeof(SensorData)) == -1) {
        perror("Erro no ftruncate");
        exit(EXIT_FAILURE);
    }

    SensorData *shared_data = mmap(NULL, sizeof(SensorData), 
                                 PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("Erro no mmap");
        exit(EXIT_FAILURE);
    }

    // Inicialização do semáforo
    sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("Erro no semáforo");
        exit(EXIT_FAILURE);
    }

    // Configuração da fila de mensagens
    struct mq_attr mq_attributes = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = sizeof(Message),
        .mq_curmsgs = 0
    };

    mqd_t mq_descriptor = mq_open(MQ_NAME, O_RDONLY | O_CREAT | O_NONBLOCK, 
                                 0666, &mq_attributes);
    if (mq_descriptor == (mqd_t)-1) {
        perror("Erro na fila de mensagens");
        exit(EXIT_FAILURE);
    }

    // Inicialização dos valores
    shared_data->velocidade = 0;
    shared_data->rpm = 800;
    shared_data->temperatura = 80;
    shared_data->acelera = false;
    shared_data->freia = false;
    shared_data->pedal = 0;

    // Criação da thread de controle das setas
    if (pthread_create(&blink_thread, NULL, blink_routine, NULL) != 0) {
        perror("Erro na criação da thread");
        exit(EXIT_FAILURE);
    }

    // Loop principal
    while (running) {
        if (!paused) {
            update_system_state(&state, shared_data);
            display_status(&state);
            check_system_limits(shared_data);

            Message msg;
            if (mq_receive(mq_descriptor, (char *)&msg, sizeof(Message), NULL) != -1) {
                process_command(&msg, shared_data);
            }
        }
        sleep(UPDATE_INTERVAL);
    }

    // Limpeza e finalização
    pthread_join(blink_thread, NULL);
    sem_close(sem);
    sem_unlink(SEMAPHORE_NAME);
    munmap(shared_data, sizeof(SensorData));
    close(shm_fd);
    shm_unlink(SHARED_MEM_NAME);
    mq_close(mq_descriptor);
    mq_unlink(MQ_NAME);

    printf("Sistema encerrado.\n");
    return 0;
}