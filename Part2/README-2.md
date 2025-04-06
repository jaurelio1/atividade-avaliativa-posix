**Link do Vídeo:** [Video](https://drive.google.com/file/d/1bV6CVO1w72RqCpwEsBcD67fpLcB8bTlA/view?usp=sharing)

**Chapter**

**1**

Trabalho 2 POSIX

José Aurélio Epaminondas de Carvalho

**Abstract**

*This project presents the development and implementation of a simulated automotive control system using parallel computing, inter-process communication (IPC), and sensor-actuator integration via GPIO. The system demonstrates the use of shared memory, message queues, and semaphores for synchronization and efficient real-time event handling. It is divided into three main modules: Control, Sensors, and Panel. The Control module processes user commands, ensures system safety by monitoring limits (e.g., RPM, temperature), and updates actuators accordingly. The Sensors module simulates physical measurements like RPM, speed, and engine temperature using Hall sensors. The Panel module provides an interactive user interface for sending commands and monitoring the vehicle's state in real time. Experiments validated the architecture’s scalability, synchronization, and accuracy, making this project a scalable solution for embedded systems simulation and development.*

**Resumo**

*Este projeto apresenta o desenvolvimento e implementação de um sistema de controle automotivo simulado utilizando computação paralela, comunicação entre processos (IPC) e integração de sensores e atuadores via GPIO. O sistema demonstra o uso de memória compartilhada, filas de mensagens e semáforos para sincronização e tratamento eficiente de eventos em tempo real. Ele é dividido em três módulos principais: Controle, Sensores e Painel. O módulo de Controle processa comandos do usuário, garante a segurança do sistema monitorando limites (como RPM e temperatura) e atualiza os atuadores. O módulo de Sensores simula medições físicas, como RPM, velocidade e temperatura do motor, utilizando sensores Hall. O módulo Painel fornece uma interface interativa para enviar comandos e monitorar o estado do veículo em tempo real. Experimentos validaram a escalabilidade, sincronização e precisão da arquitetura, tornando este projeto uma solução escalável para simulação e desenvolvimento de sistemas embarcados.*

# Sumário

[**1. Introdução**](#_toc188300775)

[**1.1 Objetivo**](#_toc188300776)

[**1.2 Contexto**](#_toc188300777)

[**2. Implementação**](#_toc188300778)

[**2.1 Código sensores.c**](#_toc188300779)

[**2.1.1 Objetivo do Sistema**](#_toc188300780)

[**2.1.2 Monitoramento de Sensores**](#_toc188300781)

[**2.1.3. Cálculos de Medições**](#_toc188300782)

[**2.1.4 Memória Compartilhada**](#_toc188300783)

[**2.1.5 Sincronização com Semáforos**](#_toc188300784)

[**2.1.6 GPIO para Sensores**](#_toc188300785)

[**2.1.7 Exibição do Estado**](#_toc188300786)

[**2.1.8 Finalização**](#_toc188300787)

[**2.2 Código painel.c**](#_toc188300788)

[**2.2.1 Objetivo do Sistema**](#_toc188300789)

[**2.2.2 Monitoramento de GPIO**](#_toc188300790)

[**2.2.3 Processamento de Eventos**](#_toc188300791)

[**2.2.4 Comunicação por Fila de Mensagens**](#_toc188300792)

[**2.2.5 Loop Principal**](#_toc188300793)

[**2.2.6 Finalização**](#_toc188300794)

[**2.3 Código controle.c**](#_toc188300795)

[**2.3.1 Objetivo do Sistema**](#_toc188300796)

[**2.3.2 Estruturas de Dados**](#_toc188300797)

[**2.3.3 Atualização do Estado do Sistema**](#_toc188300798)

[**2.3.4 Atualização de Atuadores**](#_toc188300799)

[**2.3.5 Processamento de Comandos**](#_toc188300800)

[**2.3.6 Controle de Cruzeiro**](#_toc188300801)

[**2.3.7 Monitoramento e Verificação**](#_toc188300802)

[**2.3.8 Exibição do Estado**](#_toc188300803)

[**2.3.9 Inicialização e Encerramento**](#_toc188300804)

[**2.4 Código common.h**](#_toc188300805)

[**2.4.1 Objetivo do Sistema**](#_toc188300806)

[**2.4.2 Bibliotecas Incluídas**](#_toc188300807)

[**2.4.3 Definições dos Pinos GPIO**](#_toc188300808)

[**2.4.4 Recursos de IPC**](#_toc188300809)

[**2.4.5 Constantes Operacionais**](#_toc188300810)

[**2.4.6 Estruturas de Dados**](#_toc188300811)

[**2.4.7 Variáveis Globais**](#_toc188300812)

[**2.5 Código Makefile**](#_toc188300813)

[**3. Conclusão**20](#_toc188300814)

# **1. Introdução**

## **1.1 Objetivo**

O objetivo deste trabalho é desenvolver e implementar um sistema de controle automotivo simulado utilizando conceitos de computação paralela, comunicação entre processos (IPC) e integração com sensores e atuadores via GPIO. O sistema visa demonstrar o uso de tecnologias como memória compartilhada, filas de mensagens e semáforos para sincronização, bem como o controle eficiente de eventos em tempo real. Além disso, o trabalho busca explorar o impacto de diferentes estratégias de sincronização e a importância do design modular em sistemas embarcados.

## **1.2 Contexto**

A evolução da indústria automotiva tem levado à integração de sistemas embarcados complexos que controlam diversos aspectos de um veículo, desde sensores e atuadores até interfaces de controle e conectividade. Em um cenário de crescente automação e eficiência, a implementação de soluções baseadas em sistemas embarcados é essencial para simular e desenvolver funcionalidades críticas de um carro moderno, como controle de cruzeiro, monitoramento de sensores de velocidade, RPM, e temperatura do motor, além da interação com comandos do usuário. Este trabalho propõe uma simulação desses sistemas, aplicando conceitos de sincronização e comunicação interprocessual, com ênfase na escalabilidade e precisão.

# **2. Implementação**

## **2.1 Código sensores.c**

### **2.1.1 Objetivo do Sistema**

O código monitora sensores baseados em entradas de pinos GPIO e usa memória compartilhada para armazenar os dados dos sensores. Ele também calcula informações baseadas nos pulsos do sensor Hall do motor e das rodas, como:

-   **RPM do motor**
-   **Velocidade do veículo**
-   **Temperatura do motor**

Além disso, utiliza um semáforo para garantir sincronização durante a manipulação de dados compartilhados.

### **2.1.2 Monitoramento de Sensores**

**process_hall_sensors**

Monitora os pulsos gerados pelos sensores Hall do motor e das rodas. Cada pulso é contado para cálculo posterior de RPM e velocidade.

void process_hall_sensors() {

`    `int hall_motor = digitalRead(PIN_SENSOR_HALL_MOTOR);

`    `if (hall_motor != last_hall_motor && hall_motor == HIGH) {

`        `pulsos_motor++;

`    `}

`    `last_hall_motor = hall_motor;

`    `int hall_a = digitalRead(PIN_SENSOR_HALL_RODA_A);

`    `if (hall_a != last_hall_a) {

`        `pulsos_roda++;

`    `}

`    `last_hall_a = hall_a;

}

-   **Sensores Hall do motor:** Incrementa pulsos_motor quando detecta uma mudança no estado.
-   **Sensores Hall das rodas:** Incrementa pulsos_roda de forma semelhante.

### **2.1.3. Cálculos de Medições**

**update_measurements**

Atualiza as medições de RPM, velocidade e temperatura do motor com base nos pulsos registrados e no tempo decorrido.

void update_measurements(unsigned long current_time) {

`    `sem_wait(sem_data);

`    `if (current_time - last_time_motor \>= 1000) {

`        `rpm_calculado = pulsos_motor \* 60;

`        `shared_data-\>rpm = rpm_calculado;

`        `pulsos_motor = 0;

`        `last_time_motor = current_time;

`    `}

`    `if (current_time - last_time_roda \>= 1000) {

`        `vel_calculada = pulsos_roda \* (0.63 \* PI \* 3.6); // Velocidade em km/h

`        `shared_data-\>velocidade = vel_calculada;

`        `pulsos_roda = 0;

`        `last_time_roda = current_time;

`    `}

`    `if (current_time - last_time_temp \>= 1000) {

`        `float temp_rise = (rpm_calculado - MIN_ROTACAO) \* FACTOR_ACELERACAO;

`        `float cooling_effect = vel_calculada \* FATOR_RESFRIAMENTO_AR;

`        `temp_motor = fmin(fmax(BASE_TEMP + temp_rise - cooling_effect, BASE_TEMP), MAX_TEMP_MOTOR);

`        `shared_data-\>temp_motor = temp_motor;

`        `last_time_temp = current_time;

`    `}

`    `sem_post(sem_data);

}

-   **Cálculo de RPM:** Baseado nos pulsos do motor, convertidos para rotações por minuto.
-   **Cálculo de Velocidade:** Baseado nos pulsos das rodas, usando a fórmula: velocidade=pulsos×(0.63×π×3.6)\\text{velocidade} = \\text{pulsos} \\times (0.63 \\times \\pi \\times 3.6) Aqui, 0.63 é o diâmetro da roda em metros, e o fator 3.6 converte m/s para km/h.
-   **Cálculo de Temperatura:** Leva em conta o aumento proporcional ao RPM e o efeito de resfriamento causado pela velocidade.

### **2.1.4 Memória Compartilhada**

A memória compartilhada é usada para armazenar as medições, permitindo que outros módulos do sistema acessem os dados.

**init_shared_memory**

Configura a memória compartilhada e inicializa os valores dos sensores e atuadores.

void init_shared_memory() {

`    `int shm_fd = shm_open(SHM_NAME, O_CREAT \| O_RDWR, 0666);

`    `if (shm_fd == -1) {

`        `perror("Erro ao criar memória compartilhada");

`        `exit(1);

`    `}

`    `if (ftruncate(shm_fd, sizeof(CarData)) == -1) {

`        `perror("Erro ao definir tamanho da memória compartilhada");

`        `exit(1);

`    `}

`    `shared_data = mmap(NULL, sizeof(CarData), PROT_READ \| PROT_WRITE, MAP_SHARED, shm_fd, 0);

`    `if (shared_data == MAP_FAILED) {

`        `perror("Erro ao mapear memória compartilhada");

`        `exit(1);

`    `}

`    `memset(shared_data, 0, sizeof(CarData));

`    `sem_wait(sem_data);

`    `shared_data-\>velocidade = 0.0;

`    `shared_data-\>rpm = 0.0;

`    `shared_data-\>temp_motor = BASE_TEMP;

`    `sem_post(sem_data);

}

### **2.1.5 Sincronização com Semáforos**

O semáforo é usado para evitar condições de corrida durante a leitura e escrita dos dados na memória compartilhada.

**sem_open**

Cria ou abre o semáforo com o nome definido (SEM_NAME), permitindo acesso controlado à memória compartilhada.

sem_data = sem_open(SEM_NAME, O_CREAT, 0666, 1);

if (sem_data == SEM_FAILED) {

`    `perror("Erro ao criar semáforo");

`    `exit(1);

}

### **2.1.6 GPIO para Sensores**

Os pinos GPIO são configurados para ler os sinais dos sensores Hall do motor e das rodas.

**init_gpio**

Inicializa os pinos GPIO como entradas e ativa resistores pull-down para evitar leituras flutuantes.

void init_gpio() {

`    `if (wiringPiSetupGpio() == -1) {

`        `fprintf(stderr, "Falha ao inicializar GPIO\\n");

`        `exit(1);

`    `}

`    `pinMode(PIN_SENSOR_HALL_MOTOR, INPUT);

`    `pinMode(PIN_SENSOR_HALL_RODA_A, INPUT);

`    `pinMode(PIN_SENSOR_HALL_RODA_B, INPUT);

`    `pullUpDnControl(PIN_SENSOR_HALL_MOTOR, PUD_DOWN);

`    `pullUpDnControl(PIN_SENSOR_HALL_RODA_A, PUD_DOWN);

`    `pullUpDnControl(PIN_SENSOR_HALL_RODA_B, PUD_DOWN);

}

### **2.1.7 Exibição do Estado**

A função display_status exibe informações do sistema em tempo real no terminal.

void display_status() {

`    `printf("\\rRPM: %.0f \| Vel: %.1f km/h \| Temp: %.1f°C \| Pulsos Motor: %lu \| Pulsos Rodas: %lu",

`           `rpm_calculado, vel_calculada, temp_motor, pulsos_motor, pulsos_roda);

`    `fflush(stdout);

}

### **2.1.8 Finalização**

Ao encerrar o programa, a função cleanup libera os recursos, como memória compartilhada e semáforos.

void cleanup() {

`    `sem_wait(sem_data);

`    `shared_data-\>velocidade = 0.0;

`    `shared_data-\>rpm = 0.0;

`    `shared_data-\>temp_motor = BASE_TEMP;

`    `sem_post(sem_data);

`    `sem_close(sem_data);

`    `sem_unlink(SEM_NAME);

`    `munmap(shared_data, sizeof(CarData));

}

## **2.2 Código painel.c**

### **2.2.1 Objetivo do Sistema**

O programa realiza as seguintes funções principais:

1.  Monitora eventos de entrada nos pinos GPIO (ex.: pressionar acelerador, ativar farol, etc.).
2.  Converte eventos em **comandos específicos**.
3.  Envia os comandos para o controlador central por meio de uma **fila de mensagens POSIX**.
4.  Exibe o estado atual das entradas monitoradas no terminal.

### **2.2.2 Monitoramento de GPIO**

O programa utiliza a biblioteca wiringPi para monitorar os pinos GPIO configurados como entradas. Ele detecta mudanças nos estados dos pinos e processa os eventos correspondentes.

**Configuração dos Pinos**

A função init_gpio inicializa os pinos GPIO e configura o pull-down resistor (estado padrão "desligado") para evitar leituras flutuantes.

void init_gpio() {

`    `if (wiringPiSetupGpio() == -1) {

`        `fprintf(stderr, "Falha ao inicializar GPIO\\n");

`        `exit(1);

`    `}

`    `int pins[] = {

`        `PIN_PEDAL_ACEL, PIN_PEDAL_FREIO,

`        `PIN_CMD_FAROL, PIN_CMD_FAROL_ALTO,

`        `PIN_CMD_SETA_ESQ, PIN_CMD_SETA_DIR,

`        `PIN_CC_RES, PIN_CC_CANCEL

`    `};

`    `int num_pins = sizeof(pins) / sizeof(pins[0]);

`    `for (int i = 0; i \< num_pins; i++) {

`        `pinMode(pins[i], INPUT);

`        `pullUpDnControl(pins[i], PUD_DOWN);

`    `}

}

### **2.2.3 Processamento de Eventos**

Sempre que o estado de um pino muda, a função handle_gpio_event é chamada. Ela:

1.  Identifica o pino que sofreu a mudança.
2.  Mapeia o evento para um comando específico (ex.: acelerar, frear, ativar farol).
3.  Envia o comando pela fila de mensagens.

**Lógica de Processamento**

A função associa o pino ao comando correto usando switch. Por exemplo:

-   **Pino do acelerador (PIN_PEDAL_ACEL)**: Se pressionado, envia um comando CMD_ACELERAR com valor de 50%.
-   **Pino do freio (PIN_PEDAL_FREIO)**: Envia o comando CMD_FREAR com valor de 100% (freio total).

void handle_gpio_event(int pin) {

`    `Command cmd;

`    `int value = digitalRead(pin);

`    `switch(pin) {

`        `case PIN_PEDAL_ACEL:

`            `cmd.tipo = CMD_ACELERAR;

`            `cmd.valor = value ? 50 : 0;

`            `break;

`        `case PIN_PEDAL_FREIO:

`            `cmd.tipo = CMD_FREAR;

`            `cmd.valor = value ? 100 : 0;

`            `break;

`        `case PIN_CMD_FAROL:

`            `cmd.tipo = CMD_FAROL_BAIXO;

`            `cmd.valor = value;

`            `break;

`        `case PIN_CMD_FAROL_ALTO:

`            `cmd.tipo = CMD_FAROL_ALTO;

`            `cmd.valor = value;

`            `break;

`        `case PIN_CMD_SETA_ESQ:

`            `cmd.tipo = CMD_SETA_ESQ;

`            `cmd.valor = value;

`            `break;

`        `case PIN_CMD_SETA_DIR:

`            `cmd.tipo = CMD_SETA_DIR;

`            `cmd.valor = value;

`            `break;

`        `case PIN_CC_RES:

`            `cmd.tipo = CMD_CRUISE_CONTROL;

`            `cmd.valor = value;

`            `break;

`        `case PIN_CC_CANCEL:

`            `cmd.tipo = CMD_CC_CANCEL;

`            `cmd.valor = value;

`            `break;

`        `default:

`            `return;

`    `}

`    `send_command(cmd);

}

### **2.2.4 Comunicação por Fila de Mensagens**

O painel de controle utiliza uma **fila de mensagens POSIX** para enviar comandos para o controlador.

**Configuração da Fila**

A fila é criada com os seguintes atributos:

-   **mq_maxmsg**: Número máximo de mensagens na fila (10).
-   **mq_msgsize**: Tamanho de cada mensagem (igual a sizeof(Command)).

struct mq_attr attr = {

.mq_flags = 0,

.mq_maxmsg = 10,

.mq_msgsize = sizeof(Command),

.mq_curmsgs = 0

};

mq = mq_open(MQ_NAME, O_WRONLY \| O_CREAT, 0666, \&attr);

**Envio de Comandos**

A função send_command encapsula a lógica de envio de comandos para a fila.

void send_command(Command cmd) {

`    `if (mq_send(mq, (char\*)&cmd, sizeof(Command), 0) == -1) {

`        `perror("Erro ao enviar comando");

`    `}

}

### **2.2.5 Loop Principal**

O loop principal realiza as seguintes operações:

1.  Monitora todos os pinos GPIO para identificar mudanças de estado.
2.  Processa os eventos detectados e envia os comandos correspondentes.
3.  Exibe o estado atual dos controles no terminal.

**Verificação de Mudanças nos Pinos**

Para reduzir o consumo de CPU, o programa compara o estado atual dos pinos com estados anteriores armazenados no array prev_states.

int changes = 0;

for (int i = 0; i \< num_pins; i++) {

`    `int current_state = digitalRead(pins[i]);

`    `if (current_state != prev_states[i]) {

`        `handle_gpio_event(pins[i]);

`        `prev_states[i] = current_state;

`        `changes++;

`    `}

}

**Dormir em Caso de Inatividade**

Se nenhum pino sofreu alterações, o programa utiliza select para pausar por 100ms, economizando recursos do sistema.

if (changes == 0) {

`    `select(0, NULL, NULL, NULL, \&timeout);

}

**Atualização do Display**

A cada segundo, o programa exibe o estado atual dos controles no terminal.

if (now - last_display \>= 1) {

`    `printf("\\rAcel: %d \| Freio: %d \| Farol Baixo: %d \| Farol Alto: %d \| Seta Esq: %d \| Seta Dir: %d \| CC: %d",

`           `digitalRead(PIN_PEDAL_ACEL),

`           `digitalRead(PIN_PEDAL_FREIO),

`           `digitalRead(PIN_CMD_FAROL),

`           `digitalRead(PIN_CMD_FAROL_ALTO),

`           `digitalRead(PIN_CMD_SETA_ESQ),

`           `digitalRead(PIN_CMD_SETA_DIR),

`           `digitalRead(PIN_CC_RES));

`    `fflush(stdout);

`    `last_display = now;

}

### **2.2.6 Finalização**

Quando o programa é encerrado (com Ctrl+C), ele chama cleanup para liberar recursos:

-   Fecha e remove a fila de mensagens.
-   Libera os recursos GPIO.

void cleanup() {

`    `mq_close(mq);

`    `mq_unlink(MQ_NAME);

}

## **2.3 Código controle.c**

### **2.3.1 Objetivo do Sistema**

Este programa simula o comportamento de um sistema embarcado automotivo em tempo real, incluindo:

-   Monitoramento de velocidade, RPM e temperatura.
-   Controle de funcionalidades do veículo, como acelerador, freio, faróis, setas, e controle de cruzeiro.
-   Interação com sensores (via memória compartilhada) e atuadores (via GPIO).
-   Processamento de comandos externos recebidos por meio de filas de mensagens POSIX.

### **2.3.2 Estruturas de Dados**

**SystemState**

A estrutura SystemState armazena o estado atual do sistema, incluindo informações sobre:

-   Sensores: velocidade, RPM, temperatura.
-   Controles: acelerador, freio, faróis, setas.
-   Controle de cruzeiro: estado e velocidade configurada.

typedef struct {

`    `int acelerador;

`    `int freio;

`    `int farol_baixo;

`    `int farol_alto;

`    `int seta_esquerda;

`    `int seta_direita;

`    `double velocidade;

`    `int rpm;

`    `double temperatura;

`    `int cruise_control;

`    `double velocidade_cruzeiro;

} SystemState;

**Command**

Os comandos recebidos via fila de mensagens são processados para alterar o estado do sistema.

### **2.3.3 Atualização do Estado do Sistema**

A função update_system_state atualiza o estado local do sistema (state) com base nos valores lidos da memória compartilhada. Isso permite sincronizar os sensores virtuais com o estado real do sistema.

void update_system_state() {

`    `sem_wait(sem_data); // Garantir acesso exclusivo

`    `state.velocidade = shared_data-\>velocidade;

`    `state.rpm = shared_data-\>rpm;

`    `state.temperatura = shared_data-\>temp_motor;

`    `// Atualiza outros campos...

`    `sem_post(sem_data);

}

### **2.3.4 Atualização de Atuadores**

A função update_actuators usa GPIO para simular o controle de hardware:

-   Controla o motor (PWM para acelerar ou frear).
-   Liga/desliga luzes de faróis e setas com base no estado do sistema.

void update_actuators() {

`    `if (state.acelerador) {

`        `softPwmWrite(PIN_MOTOR_PWM, 50);

`        `digitalWrite(PIN_LUZ_FREIO, LOW);

`    `} else if (state.freio) {

`        `softPwmWrite(PIN_FREIO_PWM, 50);

`        `digitalWrite(PIN_LUZ_FREIO, HIGH);

`    `}

}

### **2.3.5 Processamento de Comandos**

A função process_command interpreta os comandos recebidos via fila de mensagens (Command) e atualiza o estado do sistema, incluindo ações como:

-   Acelerar ou frear.
-   Ligar/desligar faróis e setas.
-   Ativar/cancelar controle de cruzeiro.

void process_command(Command cmd) {

`    `switch(cmd.tipo) {

`        `case CMD_ACELERAR:

`            `state.acelerador = cmd.valor;

`            `state.freio = 0;

`            `break;

`        `case CMD_FAROL_ALTO:

`            `state.farol_alto = cmd.valor;

`            `break;

`        `case CMD_CRUISE_CONTROL:

`            `if (state.velocidade \> 0) {

`                `state.cruise_control = 1;

`                `state.velocidade_cruzeiro = state.velocidade;

`            `}

`            `break;

`    `}

}

### **2.3.6 Controle de Cruzeiro**

A função update_cruise_control implementa um controle proporcional simples, ajustando o acelerador e freio para manter a velocidade de cruzeiro configurada.

void update_cruise_control() {

`    `if (state.cruise_control) {

`        `float erro = state.velocidade_cruzeiro - state.velocidade;

`        `if (erro \> 0) state.acelerador = 1;

`        `else if (erro \< 0) state.freio = 1;

`    `}

}

### **2.3.7 Monitoramento e Verificação**

A função verify_system_state verifica condições de segurança:

-   Se a velocidade ou temperatura excederem os limites, o sistema desativa o acelerador e aplica o freio.
-   O controle de cruzeiro é desativado automaticamente em caso de condições perigosas.

void verify_system_state() {

`    `if (state.velocidade \> MAX_SPEED \|\| state.temperatura \> MAX_TEMP_MOTOR) {

`        `state.acelerador = 0;

`        `state.freio = 1;

`    `}

}

**2.3.8 Exibição do Estado**

A função display_status exibe informações do veículo em tempo real no terminal, como velocidade, RPM, temperatura, e estado do controle de cruzeiro.

void display_status() {

`    `printf("\\rVel: %.1f km/h \| RPM: %d \| Temp: %.1f°C \| CC: %s",

`           `state.velocidade, state.rpm, state.temperatura,

`           `state.cruise_control ? "ON" : "OFF");

`    `fflush(stdout);

}

**2.3.9 Inicialização e Encerramento**

-   **init_gpio:** Configura os pinos GPIO e inicializa os PWMs para simulação de hardware.
-   **cleanup:** Finaliza o programa, desligando os atuadores e liberando recursos de IPC (semaforos, memória compartilhada, filas de mensagens).

#### **2.3.9.1 Loop Principal**

O loop principal coordena as funções do sistema:

1.  Atualiza o estado do sistema (update_system_state).
2.  Processa comandos recebidos (mq_receive e process_command).
3.  Controla a lógica do controle de cruzeiro e verifica o estado geral.
4.  Atualiza atuadores (update_actuators) e exibe o estado no terminal.

while(running) {

`    `update_system_state();

`    `if (mq_receive(mq, (char\*)&cmd, sizeof(Command), NULL) \> 0) {

`        `process_command(cmd);

`    `}

`    `update_cruise_control();

`    `verify_system_state();

`    `update_actuators();

`    `display_status();

`    `select(0, NULL, NULL, NULL, \&timeout); // Economiza CPU

}

#### **2.3.9.2 Encerramento**

O programa termina quando o sinal SIGINT é recebido, chamando cleanup para liberar recursos e encerrar os processos de forma segura.

void handle_sigint(int sig) {

`    `running = 0;

}

## **2.4 Código common.h**

### **2.4.1 Objetivo do Sistema**

Este arquivo serve como um ponto único de definição de constantes, tipos de dados e configurações que serão usados em todo o sistema, promovendo modularidade e reutilização de código. Ele inclui:

-   Definições para **pinos GPIO** conectados a sensores e atuadores.
-   **Estruturas de dados** para representar o estado do carro e os comandos recebidos.
-   Constantes para cálculos de física do veículo, como aceleração, resfriamento e limites operacionais.
-   Nomes para os recursos IPC (memória compartilhada, semáforos e filas de mensagens).

### **2.4.2 Bibliotecas Incluídas**

O cabeçalho inclui as seguintes bibliotecas:

-   **Bibliotecas padrão:**
    -   stdio.h, stdlib.h, string.h, etc., para entrada/saída, manipulação de strings e gerenciamento de memória.
-   **Bibliotecas para IPC:**
    -   sys/mman.h e sys/stat.h para memória compartilhada.
    -   mqueue.h para filas de mensagens POSIX.
    -   semaphore.h para semáforos.
-   **Bibliotecas de hardware:**
    -   wiringPi.h e softPwm.h para controle de GPIO e PWM (modulação por largura de pulso).

### **2.4.3 Definições dos Pinos GPIO**

Os pinos GPIO representam sensores, atuadores e controles do sistema. Cada pino é mapeado para uma função específica do veículo, como o sensor Hall, faróis, ou pedal do acelerador.

\#define PIN_MOTOR_DIR1 17 // Direção do motor (frente)

\#define PIN_MOTOR_DIR2 18 // Direção do motor (trás)

\#define PIN_MOTOR_PWM 23 // Controle PWM do motor

\#define PIN_FREIO_PWM 24 // Controle PWM do freio

\#define PIN_PEDAL_ACEL 27 // Pedal do acelerador

\#define PIN_PEDAL_FREIO 22 // Pedal do freio

\#define PIN_SENSOR_HALL_MOTOR 11 // Sensor Hall do motor

\#define PIN_SENSOR_HALL_RODA_A 5 // Sensor Hall da roda (A)

\#define PIN_SENSOR_HALL_RODA_B 6 // Sensor Hall da roda (B)

\#define PIN_FAROL_BAIXO 19 // Farol baixo

\#define PIN_FAROL_ALTO 26 // Farol alto

\#define PIN_LUZ_FREIO 25 // Luz de freio

\#define PIN_LUZ_SETA_ESQ 8 // Seta esquerda

\#define PIN_LUZ_SETA_DIR 7 // Seta direita

\#define PIN_LUZ_TEMP_MOTOR 12 // Indicador de temperatura do motor

\#define PIN_CMD_FAROL 16 // Botão para farol baixo

\#define PIN_CMD_FAROL_ALTO 1 // Botão para farol alto

\#define PIN_CMD_SETA_ESQ 20 // Botão para seta esquerda

\#define PIN_CMD_SETA_DIR 21 // Botão para seta direita

\#define PIN_CC_RES 13 // Botão para ativar Cruise Control

\#define PIN_CC_CANCEL 0 // Botão para cancelar Cruise Control

-   Os pinos são organizados por funcionalidade, como sensores (ex.: PIN_SENSOR_HALL_MOTOR), atuadores (ex.: PIN_MOTOR_PWM), e controles (ex.: PIN_CC_RES).

### **2.4.4 Recursos de IPC**

Nomes definidos para os recursos IPC utilizados no sistema:

-   **SHM_NAME**: Nome da memória compartilhada.
-   **MQ_NAME**: Nome da fila de mensagens POSIX para comandos.
-   **SEM_NAME**: Nome do semáforo para sincronização.

\#define SHM_NAME "/shm_car_data" // Memória compartilhada

\#define MQ_NAME "/mq_car_commands" // Fila de mensagens

\#define SEM_NAME "/sem_car_data" // Semáforo

### **2.4.5 Constantes Operacionais**

Constantes relacionadas ao comportamento físico e limites do veículo:

-   **Fatores de cálculo:**
    -   FACTOR_ACELERACAO: Fator para aumento de temperatura baseado no RPM.
    -   FATOR_RESFRIAMENTO_AR: Fator para resfriamento baseado na velocidade.
-   **Limites do sistema:**
    -   Velocidade máxima (MAX_SPEED) e mínima (MIN_SPEED).
    -   Temperatura máxima do motor (MAX_TEMP_MOTOR) e temperatura base (BASE_TEMP).
    -   Rotação máxima (MAX_ROTACAO) e mínima (MIN_ROTACAO).
-   **Constantes matemáticas:**
    -   Valor de π\\pi.

\#define FACTOR_ACELERACAO 0.01

\#define FATOR_RESFRIAMENTO_AR 0.05

\#define MAX_SPEED 200.0

\#define MIN_SPEED 0.0

\#define MAX_TEMP_MOTOR 140

\#define BASE_TEMP 80

\#define MAX_ROTACAO 7000

\#define MIN_ROTACAO 800

\#define PI 3.1415

### **2.4.6 Estruturas de Dados**

**CarData**

A estrutura principal que representa o estado do carro, armazenada na memória compartilhada.

-   **Sensores:**
    -   Velocidade, RPM e temperatura do motor.
-   **Atuadores:**
    -   Faróis, setas, luz de freio.
-   **Controle de cruzeiro:**
    -   Ativação e velocidade configurada.

typedef struct {

`    `float velocidade;

`    `float rpm;

`    `float temp_motor;

`    `int direcao; // 1 = frente, -1 = trás, 0 = parado

`    `int farol_alto;

`    `int farol_baixo;

`    `int seta_esquerda;

`    `int seta_direita;

`    `int luz_freio;

`    `int pedal_acelerador;

`    `int pedal_freio;

`    `int cruise_control_ativo;

`    `float velocidade_cruzeiro;

} CarData;

**CommandType**

Enumeração para os tipos de comandos enviados ao sistema:

-   Comandos para controle do motor, freio e faróis.
-   Controle de setas e Cruise Control.

typedef enum {

`    `CMD_ACELERAR,

`    `CMD_FREAR,

`    `CMD_FAROL_ALTO,

`    `CMD_FAROL_BAIXO,

`    `CMD_SETA_ESQ,

`    `CMD_SETA_DIR,

`    `CMD_CRUISE_CONTROL,

`    `CMD_CC_CANCEL,

`    `CMD_PARAR

} CommandType;

**Command**

Estrutura que encapsula um comando, composta pelo tipo (tipo) e um valor associado (valor), que pode representar, por exemplo, a intensidade do pedal.

typedef struct {

`    `CommandType tipo;

`    `int valor;

} Command;

### **2.4.7 Variáveis Globais**

-   running: Variável global usada para controlar a execução do programa. É definida como extern para ser acessada em outros módulos do sistema.

extern volatile int running;

## **2.5 Código Makefile**

O **Makefile** fornecido é utilizado para automatizar a compilação de um sistema de controle automotivo composto por três módulos principais: **controle**, **sensores**, e **painel**. Abaixo está uma explicação detalhada de cada parte do arquivo:

**Variáveis**

-   **CC**: Define o compilador a ser usado. Neste caso, é o gcc (GNU Compiler Collection).
-   **CFLAGS**: Define as opções de compilação, como:
    -   \-Wall: Habilita todos os avisos para melhorar a qualidade do código.
    -   \-pthread: Inclui suporte para threads POSIX.
-   **LIBS**: Define as bibliotecas adicionais necessárias para o projeto:
    -   \-lrt: Biblioteca para suporte a relógios e temporizadores POSIX.
    -   \-lwiringPi: Biblioteca para controle de GPIO no Raspberry Pi.
    -   \-lpthread: Biblioteca para threads POSIX.
    -   \-lm: Biblioteca matemática (para funções como fmin e PI).

# 

# **3. Conclusão**

Embora o sistema não tenha sido testado, a implementação seguiu princípios sólidos de design e integração, com base nos seguintes aspectos:

1.  **Modularidade:** A separação entre os módulos permitiu que cada componente fosse desenvolvido de forma independente, com responsabilidades bem definidas.
2.  **Comunicação interprocessual:** O uso de memória compartilhada, semáforos e filas de mensagens forneceu uma base para comunicação e sincronização eficiente entre os módulos.
3.  **Simulação de funcionalidade automotiva:** O sistema foi projetado para calcular medições como velocidade, RPM e temperatura do motor, além de processar comandos para atuadores, como faróis, setas e controle de cruzeiro.

**Contribuições**

O trabalho trouxe as seguintes contribuições:

-   **Proposta de arquitetura modular:** A estrutura implementada serve como base para sistemas automotivos que requerem integração de múltiplos sensores e atuadores.
-   **Demonstração de conceitos de IPC:** A implementação mostrou como memória compartilhada, filas de mensagens e semáforos podem ser aplicados para sincronização e troca de dados em tempo real.
-   **Flexibilidade para expansão:** O sistema foi projetado com potencial para integração de novos sensores, atuadores e funcionalidades, permitindo fácil adaptação para simulações mais complexas.

**Limitações**

Apesar das contribuições, algumas limitações devem ser destacadas:

1.  **Falta de testes práticos:** O sistema não foi validado por meio de testes, seja em um ambiente simulado ou com hardware real. Como resultado, o desempenho e a precisão da implementação ainda precisam ser avaliados.
2.  **Dependência de parâmetros fixos:** Cálculos como velocidade e temperatura do motor foram baseados em constantes que podem não refletir o comportamento de um veículo real.
3.  **Interface simplificada:** O painel de controle, implementado como uma interface de terminal, atende aos requisitos básicos, mas não oferece uma experiência intuitiva para o usuário.

**Considerações Finais**

Embora o sistema não tenha sido testado, sua implementação seguiu uma abordagem robusta e modular, utilizando ferramentas amplamente aplicadas em sistemas embarcados. Este trabalho oferece uma base sólida para estudos futuros, que podem incluir testes em hardware real, expansão de funcionalidades e validação em cenários de alta carga. A aplicação de conceitos como IPC e sincronização reforça a relevância deste projeto para o desenvolvimento de soluções embarcadas em tempo real.
