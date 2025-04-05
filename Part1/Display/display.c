#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>
#include "display.h"

// Variáveis globais
mqd_t mq_envia;
struct mq_attr mq_attributes = {
    .mq_flags = 0,
    .mq_maxmsg = 10,
    .mq_msgsize = sizeof(Message),
    .mq_curmsgs = 0
};

void clear_screen(){
    printf("\033[H\033[J");
}

// Exibe o cabeçalho do menu
void display_header() {
    clear_screen();
    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║         PAINEL DE CONTROLE            ║\n");
    printf("╠═══════════════════════════════════════╣\n");
}

// Exibe o menu principal
void display_menu() {
    display_header();
    printf("║  [1] Acelerador                       ║\n");
    printf("║  [2] Freio                            ║\n");
    printf("║  [3] Farol Baixo                      ║\n");
    printf("║  [4] Farol Alto                       ║\n");
    printf("║  [5] Seta Direita                     ║\n");
    printf("║  [6] Seta Esquerda                    ║\n");
    printf("║  [0] Sair                             ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    printf("║  Escolha uma opção:                   ║\n");
    printf("╚═══════════════════════════════════════╝\n");
}

// Exibe o rodapé com mensagem de status
void display_footer(const char* message) {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║ Status: %-28s  ║\n", message);
    printf("╚═══════════════════════════════════════╝\n");
}

// Envia mensagem para a fila
void send_message(char command, int percentage) {
    Message msg = {
        .type = 1,
        .command = command,
        .pedal = percentage
    };

    mq_envia = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &mq_attributes);
    if(mq_envia == (mqd_t)-1) {
        perror("Erro ao abrir fila de mensagens");
        exit(1);
    }

    if(mq_send(mq_envia, (char*)&msg, sizeof(Message), 0) == -1) {
        perror("Erro ao enviar mensagem");
    }
}

// Obtém input do pedal do usuário
int get_pedal_input(const char* pedal_type) {
    int percentage;
    char input[10];
    
    printf("║ Digite a força do %s (0-100%%): ", pedal_type);
    fgets(input, sizeof(input), stdin);
    percentage = atoi(input);
    
    if(percentage < 0 || percentage > 100) {
        return -1;
    }
    return percentage;
}

// Processa o comando escolhido
void handle_command(char command) {
    int percentage = 0;
    char status_message[50] = {0};

    switch(command) {
        case CMD_ACCELERATOR:
        case CMD_BRAKE: {
            const char* pedal_type = (command == CMD_ACCELERATOR) ? "acelerador" : "freio";
            percentage = get_pedal_input(pedal_type);
            
            if(percentage >= 0) {
                snprintf(status_message, sizeof(status_message), 
                        "%s ajustado para %d%%", 
                        pedal_type, percentage);
                send_message(command, percentage);
            } else {
                strcpy(status_message, "Valor inválido!");
            }
            break;
        }

        case CMD_LOW_BEAM:
            strcpy(status_message, "Farol baixo alternado");
            send_message(command, 0);
            break;

        case CMD_HIGH_BEAM:
            strcpy(status_message, "Farol alto alternado");
            send_message(command, 0);
            break;

        case CMD_RIGHT_SIGNAL:
            strcpy(status_message, "Seta direita alternada");
            send_message(command, 0);
            break;

        case CMD_LEFT_SIGNAL:
            strcpy(status_message, "Seta esquerda alternada");
            send_message(command, 0);
            break;

        case CMD_EXIT:
            strcpy(status_message, "Encerrando sistema...");
            send_message(command, 0);
            display_footer(status_message);
            exit(0);

        default:
            strcpy(status_message, "Comando inválido!");
    }

    display_footer(status_message);
}

int main(void) {
    char option;
    
    while(1) {
        display_menu();
        option = getchar();
        while(getchar() != '\n'); // Limpa o buffer

        handle_command(option);
        sleep(1);
    }

    mq_close(mq_envia);
    return 0;
}
