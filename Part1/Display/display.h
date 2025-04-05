#ifndef DISPLAY_H
#define DISPLAY_H

#define MQ_NAME "/controller_mqueue" 

// Comandos do sistema
typedef enum {
    CMD_EXIT = '0',
    CMD_ACCELERATOR = '1',
    CMD_BRAKE = '2',
    CMD_LOW_BEAM = '3',
    CMD_HIGH_BEAM = '4',
    CMD_RIGHT_SIGNAL = '5',
    CMD_LEFT_SIGNAL = '6'
} CommandType;

// Estrutura da mensagem
typedef struct {
    long type;
    char command;
    int pedal;
} Message;

// Protótipos de funções
void display_menu(void);
void send_message(char command, int percentage);
int get_pedal_input(const char* pedal_type);
void handle_command(char command);
void display_header(void);
void display_footer(const char* message);  // Corrigido o protótipo

void clear_screen();

void display_header();

void display_menu();

void display_footer(const char* message);

void send_message(char command, int percentage);

int get_pedal_input(const char* pedal_type);

void handle_command(char command);

#endif