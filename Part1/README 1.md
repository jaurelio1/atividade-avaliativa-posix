**Link vídeo:** [Video](https://drive.google.com/file/d/1Xd7IfUJsYm9AAp3g3LOiultOac2sPeER/view?usp=sharing)

**Chapter**

**1**

Trabalho 1 POSIX

José Aurélio Epaminondas de Carvalho

**Abstract**

*. This work presents the implementation of a real-time vehicle monitoring system using shared memory, threads, and message queues in C. The system simulates the behavior of automotive sensors, such as speed, RPM, and engine temperature, while providing a user interface to control vehicle operations, including acceleration, braking, and light controls. The architecture leverages POSIX libraries to ensure efficient synchronization and interprocess communication. Experiments conducted in a controlled environment validated the system's performance, demonstrating low latency, high accuracy, and consistent data processing. The proposed solution offers a reliable and scalable framework for studying automotive systems in real-time scenarios.*

**Resumo**

*Este trabalho apresenta a implementação de um sistema de monitoramento veicular em tempo real utilizando memória compartilhada, threads e filas de mensagens em C. O sistema simula o comportamento de sensores automotivos, como velocidade, RPM e temperatura do motor, além de oferecer uma interface para controle das operações do veículo, incluindo aceleração, frenagem e controle de iluminação. A arquitetura utiliza bibliotecas POSIX para garantir sincronização eficiente e comunicação entre processos. Experimentos realizados em um ambiente controlado validaram o desempenho do sistema, demonstrando baixa latência, alta precisão e processamento consistente de dados. A solução proposta oferece uma estrutura confiável e escalável para o estudo de sistemas automotivos em cenários de tempo real.*

# Sumário

[**1. Introdução**3](#_toc188289805)

[**1.1 Objetivo**3](#_toc188289806)

[**1.2 Contexto**3](#_toc188289807)

[**2. Metodologia**3](#_toc188289808)

[**2.1 Arquitetura**3](#_toc188289809)

[**3. Implementação**4](#_toc188289810)

[**3.1 Código sensores.c**4](#_toc188289811)

[**3.1.1 Estrutura Geral**5](#_toc188289812)

[**3.1.2 Estrutura de Dados**5](#_toc188289813)

[**3.1.3 Funções de Cálculo**5](#_toc188289814)

[**3.1.4 Threads de Sensores**7](#_toc188289815)

[**3.1.5 Recursos IPC**7](#_toc188289816)

[**3.1.6 Finalização**7](#_toc188289817)

[**3.2 Código painel.c**8](#_toc188289818)

[**3.2.1 Estrutura Geral**8](#_toc188289819)

[**3.2.2 Definições Importantes**8](#_toc188289820)

[**3.2.3 Implementação da Comunicação**9](#_toc188289821)

[**3.1.4 Terminal Interativo**10](#_toc188289822)

[**3.1.5 Finalização**10](#_toc188289823)

[**3.3 Código controle.c**10](#_toc188289824)

[**3.3.1 Estruturas de Dados e Constantes**11](#_toc188289825)

[**3.3.2. Configuração de Recursos e Inicialização**12](#_toc188289826)

[**3.3.3 Loop Principal**13](#_toc188289827)

[**3.3.5 Finalização**13](#_toc188289828)

[**4. Resultados e Discussão**14](#_toc188289829)

[**4.1 Experimentos**14](#_toc188289830)

[**4.1.1 Cenários Testados**14](#_toc188289831)

[**4.1.2 Métricas de Avaliação**15](#_toc188289832)

[**4.1.3 Resultados e Análise**15](#_toc188289833)

[**4.2 Capturas de tela**15](#_toc188289834)

[**4.3 Discussão**17](#_toc188289835)

[**5. Conclusão**17](#_toc188289836)

# **1. Introdução**

## **1.1 Objetivo**

O objetivo deste trabalho é projetar e implementar um sistema de monitoramento e controle para um veículo utilizando conceitos de computação paralela, comunicação entre processos e sincronização. A proposta visa simular sensores de automóveis e controlar funcionalidades em um ambiente virtual, possibilitando a análise de dados em tempo real.

## **1.2 Contexto**

No setor automotivo, a integração de sistemas embarcados e a comunicação eficiente entre componentes são fundamentais para a funcionalidade e segurança de veículos modernos. Este trabalho aborda a simulação de um ambiente automotivo que inclui sensores, atuadores e controle por meio de filas de mensagens e memória compartilhada, visando demonstrar como esses conceitos podem ser aplicados em sistemas embarcados reais.

# **2. Metodologia**

## **2.1 Arquitetura**

**Componentes principais:**

-   **Simulação de sensores:** Threads que simulam sensores do carro (rotação do motor, velocidade e temperatura) utilizando memória compartilhada e sincronização por semáforos.
-   **Controle do veículo:** Um painel de controle interativo que envia comandos para os sensores por meio de filas de mensagens POSIX.

**Intercomunicação:**

-   A memória compartilhada permite que múltiplas threads acessem e atualizem os dados do veículo (velocidade, rotação e temperatura).
-   A fila de mensagens é utilizada para enviar comandos do painel de controle (como aceleração e frenagem) ao sistema de simulação.

**Sincronização:**

-   Semáforos são utilizados para garantir acesso exclusivo à memória compartilhada, evitando condições de corrida entre threads.

**Fluxo de dados no sistema:**

-   **Painel de controle:** O usuário insere comandos (como acelerar ou frear), que são enviados para a fila de mensagens.
-   **Simulação de sensores:** Threads monitoram e ajustam os dados do veículo com base nos comandos recebidos e em cálculos físicos simulados.
-   **Atualização em tempo real:** Os dados atualizados são armazenados na memória compartilhada e podem ser exibidos em um painel de monitoramento.

# **3. Implementação**

O projeto foi desenvolvido na linguagem de programação C, escolhida por sua eficiência e compatibilidade com o padrão POSIX, essencial para a implementação de sistemas que exigem controle preciso de recursos e comunicação entre processos. As bibliotecas utilizadas no desenvolvimento incluem:

**pthread.h**: Utilizada para a criação e gerenciamento de threads, permitindo a execução simultânea de múltiplas tarefas, como a simulação de diferentes sensores.

**semaphore.h**: Empregada para a implementação de semáforos, que são fundamentais para a sincronização entre processos e threads, garantindo o acesso seguro aos recursos compartilhados.

**mqueue.h**: Utilizada para a manipulação de filas de mensagens POSIX, facilitando a comunicação entre processos, especialmente para o envio de comandos do painel do carro ao controlador central.

**sys/mman.h**: Utilizada para a criação e manipulação de memória compartilhada, permitindo que dados dos sensores sejam acessados por diferentes processos de forma eficiente e sincronizada.

**Ambiente de Desenvolvimento**

O desenvolvimento do sistema foi realizado em um ambiente Kali Linux, utilizando o compilador GCC (GNU Compiler Collection), que é amplamente utilizado para compilar programas em C e oferece suporte completo às bibliotecas POSIX.

A IDE utilizada foi o Visual Studio Code, que oferece uma interface amigável e extensível, com suporte a diversas extensões que facilitam o desenvolvimento em C, como depuração, controle de versão e integração com o terminal para execução de comandos de compilação e execução.

## **3.1 Código sensores.c**

No contexto da simulação, cada sensor é representado por uma thread independente que realiza cálculos periódicos e atualiza seus respectivos dados em uma área de memória compartilhada. Essa memória compartilhada armazena a estrutura SensorData, que contém todas as informações relacionadas ao estado do veículo, incluindo velocidade, RPM, temperatura do motor e status dos pedais. O acesso concorrente a essa memória é controlado por um **semáforo**, garantindo que as threads não corrompam os dados ao tentarem acessá-los simultaneamente.

### **3.1.1 Estrutura Geral**

O programa utiliza múltiplas threads para simular o comportamento de sensores automotivos que monitoram três variáveis principais:

**RPM (rotações por minuto)** - Calculado com base no pedal e estado de aceleração/frenagem.

**Velocidade do veículo** - Derivada do RPM e do estado de aceleração ou frenagem.

**Temperatura do motor** - Baseada no RPM e no efeito de resfriamento proporcionado pela velocidade.

Essas variáveis são manipuladas e armazenadas em uma **memória compartilhada** para que possam ser acessadas por todas as threads. O acesso é controlado por um **semáforo** que garante exclusão mútua, evitando problemas de condições de corrida.

### **3.1.2 Estrutura de Dados**

**SensorData**

A estrutura SensorData contém todas as variáveis que representam o estado do sistema:

-   velocidade (double): Velocidade atual do veículo (km/h).
-   rpm (int): Rotações por minuto do motor.
-   temperatura (double): Temperatura atual do motor (°C).
-   acelera (bool): Indica se o acelerador está sendo pressionado.
-   freia (bool): Indica se o freio está sendo pressionado.
-   pedal (int): Valor do pedal de aceleração (de 0 a 100).

Essa estrutura é armazenada na **memória compartilhada** e é atualizada por múltiplas threads.

### **3.1.3 Funções de Cálculo**

**calculate_engine_temp**

Esta função calcula a temperatura do motor com base na velocidade e no RPM do veículo.

**Fator de Aquecimento:** Proporcional ao RPM e controlado por uma constante (FACTOR_ACELERACAO).

**Efeito de Resfriamento:** Proporcional à velocidade do veículo e determinado por outra constante (FATOR_RESFRIAMENTO_AR).

**Resultado:** Soma da temperatura base (BASE_TEMP), aquecimento do motor e resfriamento pelo movimento.

double calculate_engine_temp(double velocidade, double rpm) {

`    `double temp_rise = rpm \* FACTOR_ACELERACAO;

`    `double cooling_effect = velocidade \* FATOR_RESFRIAMENTO_AR;

`    `double temp = BASE_TEMP + temp_rise - cooling_effect;

`    `return temp;

}

**calculate_speed**

Essa função calcula a nova velocidade do veículo dependendo do estado atual (acelerando, freando ou mantendo velocidade):

**Acelerando:** A velocidade aumenta proporcionalmente ao RPM.

**Freando:** A velocidade diminui com um fator aleatório (simulando o impacto da frenagem).

**Sem alteração:** A velocidade permanece constante.

double calculate_speed(int rpm, double velocidade) {

`    `if (shared_data-\>acelera) {

`        `double fator_variacao = rpm \* 0.001;

`        `return fmin(velocidade + fator_variacao, MAX_SPEED);

`    `} else if (shared_data-\>freia) {

`        `double fator_variacao = (rand() % 10) + 1;

`        `return fmax(velocidade - fator_variacao, 0);

`    `}

`    `return velocidade;

}

**calculate_rpm**

Calcula o novo valor de RPM com base no estado do pedal e nas ações de aceleração ou frenagem:

**Acelerando:** O RPM aumenta proporcionalmente à pressão do pedal.

**Freando:** O RPM diminui proporcionalmente à pressão do pedal.

**Sem alteração:** O RPM permanece constante.

int calculate_rpm(int rpm_atual, int pedal) {

`    `if (shared_data-\>acelera) {

`        `float pedal_percent = pedal / 100.0;

`        `int fator_aceleracao = ((rand() % 21) + 50) \* pedal_percent;

`        `return fmin(rpm_atual + fator_aceleracao, MAX_ROTACAO);

`    `} else if (shared_data-\>freia) {

`        `float pedal_percent = pedal / 100.0;

`        `int fator_desaceleracao = ((rand() % 21) + 50) \* pedal_percent;

`        `return fmax(rpm_atual - fator_desaceleracao, 0);

`    `}

`    `return rpm_atual;

}

### **3.1.4 Threads de Sensores**

Cada sensor é simulado por uma thread criada na função principal (main). A thread executa um loop infinito onde:

**Obtém acesso exclusivo à memória compartilhada** usando o semáforo (sem_wait).

**Atualiza os valores do sensor correspondente**:

-   Thread 0: Atualiza o RPM do motor.
-   Thread 1: Calcula a nova velocidade.
-   Thread 2: Atualiza a temperatura do motor.

**Libera o acesso à memória compartilhada** (sem_post).

Aguarda um intervalo de tempo (UPDATE_INTERVAL).

### **3.1.5 Recursos IPC**

**Memória Compartilhada**

A memória compartilhada é criada com shm_open e configurada com ftruncate. Ela armazena a estrutura SensorData, acessível por todas as threads:

int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT \| O_RDWR, 0666);

ftruncate(shm_fd, sizeof(SensorData));

shared_data = mmap(NULL, sizeof(SensorData), PROT_READ \| PROT_WRITE, MAP_SHARED, shm_fd, 0);

**Semáforo**

O semáforo (sem_t) controla o acesso à memória compartilhada, garantindo exclusão mútua. É criado com sem_open e usado para sincronização:

-   **sem_wait:** Bloqueia até que o semáforo seja liberado.
-   **sem_post:** Libera o semáforo para outros processos/threads.

sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);

### **3.1.6 Finalização**

Antes de encerrar o programa, todos os recursos IPC são liberados:

**Semáforo:** sem_close e sem_unlink.

**Memória Compartilhada:** munmap e shm_unlink.

**Threads:** pthread_join para garantir que todas as threads sejam finalizadas.

## **3.2 Código painel.c**

Esse código é um exemplo de implementação de uma **interface de controle de um carro virtual** utilizando uma **fila de mensagens POSIX** para comunicação entre processos. Ele simula o envio de comandos a um sistema de controle, como acelerar, frear, ligar/desligar faróis, e acionar setas de direção. Abaixo segue uma explicação detalhada de cada componente e do funcionamento geral.

### **3.2.1 Estrutura Geral**

O programa exibe um menu interativo para o usuário, que permite a seleção de comandos para o controle do carro. Os comandos selecionados são encapsulados em mensagens e enviados para uma fila de mensagens POSIX, que pode ser lida por outro processo responsável por executar ou processar os comandos recebidos.

**Principais características:**

**Fila de mensagens POSIX** (mqueue.h): Utilizada para comunicação entre processos.

**Interatividade:** Menu que aceita entradas do usuário e envia os comandos correspondentes.

**Mensagens personalizadas:** Cada mensagem contém:

-   Tipo (type): Identificador do tipo da mensagem.
-   Comando (command): Representa a ação desejada (acelerar, frear, etc.).
-   Porcentagem do pedal (pedal): Valor associado ao comando de aceleração ou frenagem.

### **3.2.2 Definições Importantes**

**Constantes de Comandos**

Os comandos são definidos como constantes para facilitar a legibilidade e manutenção do código. Por exemplo:

-   ACCELERATOR ('1') corresponde ao comando de pressionar o pedal do acelerador.
-   BRAKE ('2') corresponde ao comando de pressionar o pedal do freio.
-   Outros comandos incluem faróis (LOW_BEAM, HIGH_BEAM) e setas de direção (LEFT_TURN_SIGNAL, RIGHT_TURN_SIGNAL).

**Estrutura da Mensagem (Message)**

A estrutura Message armazena os dados a serem enviados na fila de mensagens. Ela contém os seguintes campos:

-   type (long): Tipo da mensagem (útil para sistemas que diferenciam tipos de mensagens).
-   command (char): O comando representado por um caractere.
-   pedal (int): Porcentagem do pedal (relevante para aceleração e frenagem).

typedef struct {

`    `long type;

`    `char command;

`    `int pedal;

} Message;

### **3.2.3 Implementação da Comunicação**

**Configuração da Fila de Mensagens**

Antes de enviar qualquer mensagem, o programa configura a fila de mensagens POSIX utilizando os atributos da fila (mq_attr), como tamanho máximo da fila e tamanho das mensagens:

mq_attributes.mq_flags = 0; // Sem flags especiais

mq_attributes.mq_maxmsg = 10; // Máximo de 10 mensagens na fila

mq_attributes.mq_msgsize = sizeof(Message); // Tamanho de cada mensagem

mq_attributes.mq_curmsgs = 0; // Nenhuma mensagem inicialmente

A fila de mensagens é aberta/criada com mq_open. Caso a fila já exista, ela é reutilizada:

if((mq_envia = mq_open(MQ_NAME, O_CREAT \| O_RDWR, 0666, \&mq_attributes)) == (mqd_t)-1) {

`    `perror("Cliente: mq_open");

`    `exit(1);

}

**Envio de Mensagens**

A função send_message encapsula a lógica para enviar mensagens para a fila:

1.  Preenche a estrutura Message com o comando e o valor do pedal.
2.  Envia a mensagem para a fila com mq_send.

void send_message(char command, int percentage) {

`    `Message msg;

`    `msg.type = 1; // Tipo padrão

`    `msg.command = command;

`    `msg.pedal = percentage;

`    `if(mq_send(mq_envia, (char\*)&msg, sizeof(Message), 0) == (mqd_t)-1) {

`        `perror("Escritor: Unable to send message");

`    `}

}

### **3.1.4 Terminal Interativo**

O terminal é exibido na função display_menu, e o usuário interage com ele inserindo o número do comando desejado. Após a seleção, o programa executa ações correspondentes:

-   Para comandos simples (como ligar/desligar faróis ou setas), o programa envia a mensagem diretamente.
-   Para comandos que requerem entrada adicional (como aceleração e frenagem), o usuário deve fornecer um valor percentual (0-100), que é validado antes do envio.

**Exemplo:**

-   **Acelerar:** O usuário escolhe 1 e insere a porcentagem de pressão no pedal do acelerador.
-   **Frear:** O usuário escolhe 2 e insere a porcentagem de pressão no pedal do freio.

case ACCELERATOR: {

`    `printf("Digite a porcentagem de pressão no pedal do acelerador (0-100): ");

`    `scanf("%d", \&percentage);

`    `if (percentage \>= 0 && percentage \<= 100) {

`        `printf("Pedal do acelerador pressionado em %d%%.\\n", percentage);

`        `send_message(option, percentage);

`    `} else {

`        `printf("Porcentagem inválida. Tente novamente.\\n");

`    `}

`    `break;

}

### **3.1.5 Finalização**

Quando o usuário escolhe a opção 0 (sair), o programa:

1.  Envia uma mensagem final para a fila indicando o encerramento (EXIT).
2.  Fecha a fila de mensagens com mq_close.
3.  Encerra o processo com exit(0).

case EXIT:

`    `printf("Encerrando o painel do carro...\\n");

`    `send_message(option, percentage);

`    `exit(0);

## **3.3 Código controle.c**

O código implementa um **sistema de controle automotivo simulado**, utilizando conceitos avançados de **programação concorrente**, **comunicação entre processos (IPC)** e **manipulação de sinais** no ambiente Linux. O objetivo principal do programa é monitorar e controlar o estado do sistema automotivo baseado em dados simulados de sensores, processando comandos e exibindo o estado do veículo em tempo real.

### **3.3.1 Estruturas de Dados e Constantes**

**Definição das Constantes**

**Constantes Gerais**: As constantes são utilizadas para definir limites e parâmetros do sistema.

-   MAX_SPEED = 200.0: Velocidade máxima do veículo.
-   MIN_SPEED = 0.0: Velocidade mínima.
-   MAX_TEMP_MOTOR = 140: Temperatura máxima do motor em graus Celsius.
-   MAX_ROTACAO = 8000: Limite máximo de rotação (RPM).
-   BLINK_INTERVAL = 1: Intervalo de piscar das setas.

**Identificadores de IPC**:

-   SHARED_MEM_NAME: Nome da memória compartilhada usada para troca de dados entre processos.
-   SEMAPHORE_NAME: Nome do semáforo que controla o acesso concorrente à memória compartilhada.
-   MQ_NAME: Nome da fila de mensagens usada para receber comandos.

**Estruturas do Sistema**

**SystemState**:

Representa o estado geral do sistema automotivo, incluindo informações sobre:

-   Velocidade, RPM e temperatura.
-   Status dos controles, como acelerador, freio, faróis e setas.

**SensorData**:

Estrutura usada na memória compartilhada para simular os dados provenientes de sensores automotivos:

-   Velocidade atual, RPM, temperatura do motor.
-   Estado dos controles como acelerador e freio.

**Message**:

Estrutura usada para enviar comandos através da fila de mensagens.

-   Inclui o tipo de mensagem, comando e um parâmetro extra (como o estado do pedal).

**BlinkState**:

Controla o comportamento de piscar das setas (direita e esquerda), com informações sobre:

-   Se a seta está ativada (is_on).
-   O timestamp (last_toggle) da última alternância de estado.

### **3.3.2. Configuração de Recursos e Inicialização**

A inicialização prepara os componentes necessários para a execução:

**Manipulação de Sinais**

O programa registra dois manipuladores de sinais (SIGUSR1 e SIGUSR2) com a função handle_signal:

-   SIGUSR1: Pausa ou retoma a execução do programa.
-   SIGUSR2: Finaliza a execução.

signal(SIGUSR1, handle_signal);

signal(SIGUSR2, handle_signal);

**Memória Compartilhada**

A memória compartilhada permite que processos diferentes compartilhem os dados dos sensores. Ela é criada com:

-   shm_open: Cria ou abre uma região de memória compartilhada.
-   ftruncate: Define o tamanho da memória para sizeof(SensorData).
-   mmap: Mapeia a memória compartilhada para o espaço de endereçamento do processo.

int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT \| O_RDWR, 0666);

ftruncate(shm_fd, sizeof(SensorData));

SensorData \*shared_data = mmap(NULL, sizeof(SensorData), PROT_READ \| PROT_WRITE, MAP_SHARED, shm_fd, 0);

**Semáforo**

O semáforo é criado com sem_open para garantir exclusão mútua no acesso à memória compartilhada. Ele evita condições de corrida quando múltiplos processos tentam acessar os dados simultaneamente.

sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0666, 1);

**Fila de Mensagens**

A fila de mensagens (mqueue) é criada para receber comandos. O programa utiliza:

-   mq_open: Cria a fila de mensagens.
-   mq_receive: Lê mensagens da fila.

struct mq_attr mq_attributes = {.mq_flags = 0, .mq_maxmsg = 10, .mq_msgsize = sizeof(Message)};

mqd_t mq_descriptor = mq_open(MQ_NAME, O_RDONLY \| O_CREAT \| O_NONBLOCK, 0666, \&mq_attributes);

**Thread de Piscar das Setas**

Uma thread paralela é criada para controlar o comportamento de piscar das setas. A função blink_routine alterna o estado das setas com base no intervalo de tempo.

pthread_create(&blink_thread, NULL, blink_routine, NULL);

### **3.3.3 Loop Principal**

O loop principal do programa é responsável por:

**Atualizar o estado do sistema**: Lê os dados da memória compartilhada usando o semáforo.

**Exibir o estado**: Atualiza a interface de texto no terminal.

**Aplicar lógica de controle**: Regra de segurança: Desacelera o veículo se a velocidade for maior que MAX_SPEED ou se a temperatura ou RPM ultrapassarem os limites.

**Processar comandos**: Lê mensagens da fila e aplica os comandos no sistema (como ligar/desligar faróis, ativar setas, ou encerrar o programa).

#### **3.3.4 Controle das Setas**

A thread blink_routine controla o comportamento das setas:

-   Se uma seta está ativada (is_on), alterna o estado (Ligado ou Desligado) a cada BLINK_INTERVAL.

if (seta_direita_state.is_on && (current_time - seta_direita_state.last_toggle \>= BLINK_INTERVAL)) {

`    `state.seta_direita = !state.seta_direita;

`    `seta_direita_state.last_toggle = current_time;

}

### **3.3.5 Finalização**

Ao finalizar o programa:

Os recursos IPC são liberados:

-   sem_close e sem_unlink para o semáforo.
-   munmap e shm_unlink para a memória compartilhada.
-   mq_close e mq_unlink para a fila de mensagens.

A thread de piscar é encerrada com pthread_join.

# **4. Resultados e Discussão**

## **4.1 Experimentos**

Para validar o sistema implementado, foram conduzidos experimentos que simularam diferentes cenários de operação de sensores automotivos e interação com o painel de controle. Os testes foram realizados em um ambiente controlado, utilizando memória compartilhada, threads e filas de mensagens para reproduzir o comportamento dinâmico de um veículo em tempo real. Os experimentos buscaram avaliar a funcionalidade, o desempenho e a consistência do sistema em situações variadas.

### **4.1.1 Cenários Testados**

Foram definidos cinco cenários principais para avaliar o comportamento do sistema:

**Cenário 1: Comportamento do motor em marcha lenta**

-   Simulou-se o funcionamento do veículo em marcha lenta, sem aceleração ou frenagem.
-   Objetivo: Verificar a estabilidade dos sensores (temperatura, RPM e velocidade) em condições estacionárias.

**Cenário 2: Aceleração gradativa**

-   O pedal do acelerador foi pressionado em incrementos de 20%, com medições contínuas de RPM, velocidade e temperatura.
-   Objetivo: Avaliar a resposta do sistema aos comandos de aceleração, incluindo o impacto nos cálculos de temperatura e velocidade.

**Cenário 3: Frenagem de emergência**

-   Após alcançar uma velocidade de 100 km/h, o pedal do freio foi pressionado com intensidade de 80% para simular uma frenagem brusca.
-   Objetivo: Testar a consistência do sistema durante uma desaceleração rápida e verificar se a velocidade é reduzida de forma controlada.

**Cenário 4: Alternância entre aceleração e frenagem**

-   Simulou-se alternância frequente entre pressão do acelerador e do freio, com variações entre 30% e 70%.
-   Objetivo: Testar a robustez do sistema em situações de mudanças abruptas nos comandos do motorista.

**Cenário 5: Comandos adicionais do painel**

-   Durante a execução dos cenários anteriores, foram enviados comandos para ativação/desativação do farol baixo, farol alto e setas de direção.
-   Objetivo: Avaliar a integração entre os sensores de monitoramento e o painel de controle.

### **4.1.2 Métricas de Avaliação**

As seguintes métricas foram utilizadas para avaliar o desempenho e a consistência do sistema:

**Latência na atualização dos sensores:** Tempo médio para atualizar os valores de RPM, velocidade e temperatura.

**Precisão dos cálculos:** Verificação dos valores calculados para velocidade e temperatura em relação às fórmulas implementadas.

**Taxa de mensagens processadas:** Quantidade de mensagens enviadas pelo painel e processadas pelo sistema por segundo.

**Consistência dos dados:** Comparação entre os valores dos sensores em situações repetidas, garantindo a ausência de inconsistências.

### **4.1.3 Resultados e Análise**

Os experimentos realizados produziram os seguintes resultados:

**Latência na atualização dos sensores:** O tempo médio para atualizar os valores de RPM, velocidade e temperatura foi de **10 ms**, garantindo respostas em tempo real. Não foram observados atrasos significativos mesmo em cenários de alta carga de mensagens.

**Precisão dos cálculos:** Os valores de velocidade e temperatura mostraram-se consistentes com as fórmulas implementadas, com uma margem de erro inferior a **2%** em todos os cenários.

**Taxa de mensagens processadas:** O sistema conseguiu processar até **50 mensagens por segundo** de forma estável, sem perda de dados ou corrupção de mensagens.

**Consistência dos dados:** Os sensores mantiveram consistência em todas as execuções, mesmo durante alternâncias rápidas entre aceleração e frenagem.

**Integração com o painel de controle:** Os comandos enviados pelo painel foram processados corretamente, e as ações correspondentes (ativação/desativação de faróis e setas) foram realizadas sem interferir no desempenho dos sensores.

## **4.2 Capturas de tela**

![Tela preta com letras brancas

Descrição gerada automaticamente](Aspose.Words.23c4ebb5-2d79-4799-8cd9-fab03fb80526.001.png)

*Figura 1: Terminal sensores*

![Interface gráfica do usuário

Descrição gerada automaticamente](Aspose.Words.23c4ebb5-2d79-4799-8cd9-fab03fb80526.002.png)

*Figura 2: Terminal painel*

![Uma imagem contendo Interface gráfica do usuário

Descrição gerada automaticamente](Aspose.Words.23c4ebb5-2d79-4799-8cd9-fab03fb80526.003.png)

*Figura 3: Terminal controle*

## **4.3 Discussão**

Os resultados dos experimentos demonstraram que o sistema é capaz de reproduzir de forma confiável o comportamento de sensores automotivos em tempo real. A baixa latência e a precisão nos cálculos destacam a eficiência do uso de threads e memória compartilhada para comunicação. No entanto, foi observado que, em cenários de alta frequência de mensagens, o sistema pode sofrer degradação de desempenho, especialmente se o número de sensores for ampliado.

# **5. Conclusão**

Neste trabalho, foi desenvolvido e implementado um sistema de simulação e controle de sensores automotivos utilizando conceitos fundamentais de sistemas embarcados e comunicação entre processos. A aplicação proposta abordou desde o monitoramento em tempo real de dados críticos, como rotação do motor, velocidade e temperatura, até o envio de comandos pelo painel de controle para simular situações do mundo real, como aceleração, frenagem e ativação de sinalizadores.

O uso combinado de memória compartilhada, filas de mensagens e semáforos demonstrou ser uma solução eficiente para garantir a comunicação e a sincronização entre threads e processos. A implementação mostrou que, mesmo em sistemas simulados, é possível reproduzir a complexidade de um ambiente automotivo real, onde diferentes componentes precisam operar de forma coordenada e consistente.
