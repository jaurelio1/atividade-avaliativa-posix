#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <signal.h>

// Definição dos pinos conforme a tabela
#define MOTOR_PWM       23  // Potência do Motor (PWM)
#define FREIO_PWM       24  // Intensidade do Freio (PWM)
#define PEDAL_ACEL      27  // Pedal do Acelerador (entrada)
#define PEDAL_FREIO     22  // Pedal do Freio (entrada)
#define MOTOR_DIR1      17  // Direção 1 do motor
#define MOTOR_DIR2      18  // Direção 2 do motor
#define LUZ_FREIO       25  // Luz do freio

volatile int running = 1;

void handle_sigint(int sig) {
    running = 0;
}

void setup() {
    // Inicializar WiringPi
    if (wiringPiSetupGpio() == -1) {
        printf("Erro ao inicializar WiringPi\n");
        exit(1);
    }

    // Configurar pinos
    pinMode(MOTOR_PWM, PWM_OUTPUT);
    pinMode(FREIO_PWM, PWM_OUTPUT);
    pinMode(PEDAL_ACEL, INPUT);
    pinMode(PEDAL_FREIO, INPUT);
    pinMode(MOTOR_DIR1, OUTPUT);
    pinMode(MOTOR_DIR2, OUTPUT);
    pinMode(LUZ_FREIO, OUTPUT);

    // Configurar pull-down para os pedais
    pullUpDnControl(PEDAL_ACEL, PUD_DOWN);
    pullUpDnControl(PEDAL_FREIO, PUD_DOWN);

    // Inicializar PWM
    softPwmWrite(MOTOR_PWM, 0);
    softPwmWrite(FREIO_PWM, 0);

    // Configurar direção inicial do motor
    digitalWrite(MOTOR_DIR1, HIGH);
    digitalWrite(MOTOR_DIR2, LOW);
}

void cleanup() {
    // Parar motor e freio
    softPwmWrite(MOTOR_PWM, 0);
    softPwmWrite(FREIO_PWM, 0);
    digitalWrite(LUZ_FREIO, LOW);
    
    printf("\nLimpeza realizada. Sistema encerrado.\n");
}

int main() {
    printf("Iniciando teste de aceleração e frenagem...\n");
    printf("Pressione Ctrl+C para encerrar\n\n");

    signal(SIGINT, handle_sigint);
    setup();

    int current_speed = 0;
    int pwm_value = 0;

    while (running) {
        // Ler estado dos pedais
        int acel = digitalRead(PEDAL_ACEL);
        int freio = digitalRead(PEDAL_FREIO);

        // Processar acelerador
        if (acel == HIGH) {
            pwm_value = min(pwm_value + 50, 1024); // Aumenta gradualmente até máximo
            softPwmWrite(MOTOR_PWM, pwm_value);
            current_speed = (pwm_value * 200) / 1024; // Simula velocidade máxima de 200 km/h
        } else if (pwm_value > 0 && !freio) {
            pwm_value = max(pwm_value - 20, 0); // Diminui gradualmente
            softPwmWrite(MOTOR_PWM, pwm_value);
            current_speed = (pwm_value * 200) / 1024;
        }

        // Processar freio
        if (freio == HIGH) {
            softPwmWrite(FREIO_PWM, 1024); // Freio máximo
            softPwmWrite(MOTOR_PWM, 0);    // Corta aceleração
            digitalWrite(LUZ_FREIO, HIGH);
            pwm_value = 0;
            current_speed = max(current_speed - 30, 0); // Simula frenagem
        } else {
            softPwmWrite(FREIO_PWM, 0);
            digitalWrite(LUZ_FREIO, LOW);
        }

        // Exibir estado atual
        printf("\rVelocidade: %3d km/h | Acelerador: %s | Freio: %s | PWM: %4d",
               current_speed,
               acel ? "ATIVO" : "     ",
               freio ? "ATIVO" : "     ",
               pwm_value);
        fflush(stdout);

        usleep(50000); // 50ms de delay
    }

    cleanup();
    return 0;
}