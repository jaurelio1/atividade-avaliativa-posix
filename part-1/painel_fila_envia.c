#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_PERMISSIONS 0660
#define MQ_MAX_MESSAGES 10
#define MQ_MAX_MSG_SIZE 128
#define MQ_MSG_BUFFER_SIZE M

// necessita de alguém para consumir e ver a mensagens, que será o controlador
void sendMessageInQueue(char mensagens[3][20]) {
    mqd_t mqd_sender;
    struct mq_attr attr;
    const char *mq_name = "/mq-minha-fila";
    char tx_buffer[20];

    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAX_MESSAGES;
    attr.mq_msgsize = MQ_MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((mqd_sender = mq_open(mq_name, O_WRONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == (mqd_t)-1) {
        perror("Cliente: mq_open");
        exit(1);
    }

    for (int i = 0; i < 3; i++) {
        sprintf(tx_buffer, "%s", mensagens[i]);
        printf("Escritor: enviando mensagem: [%s]\n", tx_buffer);

        if (mq_send(mqd_sender, tx_buffer, strlen(tx_buffer) + 1, 0) == -1) {
            perror("Escritor: Falha ao enviar mensagem");
        }
    }

    if (mq_close(mqd_sender) == -1) {
        perror("Escritor: mq_close");
        exit(1);
    }
    printf("Escritor: Exit\n");
}

int main() {
    int setas, farol, parabrisa;
    char mensagens[3][20];

    while (1) {
        printf("Setas: 1 - direita, 0 - esquerda: ");
        if (scanf("%d", &setas) != 1 || (setas < 0 || setas > 1)) {
            printf("O valor informado está incorreto. Tente novamente.\n");
            while (getchar() != '\n'); // Limpar buffer
            continue;
        }
        while (getchar() != '\n'); // Limpar buffer

        sprintf(mensagens[0], "Setas: %s", (setas == 1) ? "ligado" : "desligado");

        printf("Farol: 1 - ligado, 0 - desligado: ");
        if (scanf("%d", &farol) != 1 || (farol < 0 || farol > 1)) {
            printf("O valor informado está incorreto. Tente novamente.\n");
            while (getchar() != '\n'); // Limpar buffer
            continue;
        }
        while (getchar() != '\n'); // Limpar buffer

        sprintf(mensagens[1], "Farol: %s", (farol == 1) ? "ligado" : "desligado");

        printf("Parabrisa: 1 - ligado, 0 - desligado: ");
        if (scanf("%d", &parabrisa) != 1 || (parabrisa < 0 || parabrisa > 1)) {
            printf("O valor informado está incorreto. Tente novamente.\n");
            while (getchar() != '\n'); // Limpar buffer
            continue;
        }
        while (getchar() != '\n'); // Limpar buffer

        sprintf(mensagens[2], "Parabrisa: %s", (parabrisa == 1) ? "ligado" : "desligado");

        sendMessageInQueue(mensagens);
    }

    return 0;
}

