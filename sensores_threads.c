#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

#define SHM_NAME "/shared_mem_sensores"
#define SEM_NAME "/semaphore_safe_sensores"
#define NUM_SENSORS 4 // Número de sensores

//formato do dado a ser armazenado na memória compartilhada
//onde será informado o valor a ser armazenado e de qual sensor pertence aquele valor
typedef struct {
	int valor;
	int sensor;
} Data;

int shm_fd;
Data *valor_coletado;
sem_t *sem;

//escreve o valor na memória compartilhada
void writeValueInShm(int index, Data *data){
	printf("Processo: Tentando adquirir uma conexão...\n");

    	// Tenta adquirir o semáforo
    	sem_wait(sem);
	
	printf("Valor: %d; do sensor: %d", data->valor, data->sensor);

    	// Salva na memória compartilhada
    	valor_coletado[index].valor = data->valor;
	valor_coletado[index].sensor = data->sensor;

    	// Libera o semáforo
    	sem_post(sem);   	
}

void *writeVelCarro(){
	int value = 0, is_crescent = 1;
	Data data_vel_carro;
	int index = 0; // index do sensor no array 

	while(1){				
		value += (is_crescent) ? 10 : -10;
		if(value == 200){
			is_crescent = 0;
		}

		if(value == 0){
			is_crescent = 1;
		}
		
		data_vel_carro.valor = value;
		data_vel_carro.sensor = 0;

		writeValueInShm(index, &data_vel_carro);

		usleep(100000); //100 miliseconds			
	}
}

void *writeRotMotor(){
	int value = 0, is_crescent = 1; 
	Data data_rot_motor;
	int index = 1; // index do sensor no array 

	while(1){
		value += (is_crescent) ? 200 : -200;
		if(value == 4000){
			is_crescent = 0;
		}

		if(value == 0){
			is_crescent = 1;
		}
		
		data_rot_motor.valor = value;
		data_rot_motor.sensor = 1;

		writeValueInShm(index, &data_rot_motor);

		usleep(100000); //100 miliseconds			
	}
}

void *writeTempMotor(){
	int value = 0, is_crescent = 1; 
	Data data_temp_motor;
	int index = 2; // index do sensor no array 

	while(1){
		value += (is_crescent) ? 5 : -5;
		if(value == 100){
			is_crescent = 0;
		}

		if(value == 0){
			is_crescent = 1;
		}
		
		data_temp_motor.valor = value;
		data_temp_motor.sensor = 2;

		writeValueInShm(index, &data_temp_motor);

		usleep(100000); //100 miliseconds			
	}
}

void *writeProxCarro(){
	int value = 0, is_crescent = 1; 
	Data data_prox_carro;
	int index = 3; // index do sensor no array 

	while(1){
		value += (is_crescent) ? 5 : -5;
		if(value == 100){
			is_crescent = 0;
		}

		if(value == 0){
			is_crescent = 1;
		}
		
		data_prox_carro.valor = value;
		data_prox_carro.sensor = 3;

		writeValueInShm(index, &data_prox_carro);

		usleep(100000); //100 miliseconds			
	}
}


int main(){
	// Criando memória compartilhada
	shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
        	perror("Erro ao criar memória compartilhada");
        	exit(1);
    	}
	
	// Ajustar tamanho da memória compartilhada
    	if (ftruncate(shm_fd, NUM_SENSORS * sizeof(Data)) == -1) {
        	perror("Erro ao ajustar tamanho da memória compartilhada");
        	exit(1);
    	}

	// Mapear memória compartilhada
    	valor_coletado = mmap(0, NUM_SENSORS * sizeof(Data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    	if (valor_coletado == MAP_FAILED) {
        	perror("Erro ao mapear memória compartilhada");
        	exit(1);
    	}

	// Inicializar memória compartilhada
	for(int i = 0; i < NUM_SENSORS; i++){
		valor_coletado[i].valor = 0;
        	valor_coletado[i].sensor = i;
	}	

	// Criando o semáforo
	sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
	if (sem == SEM_FAILED) {
        	perror("sem_open falhou");
        	exit(1);
    	}
	
	//threads da velocidade do carro, rotação do motor, temperatura motor e proximidade do carro
	pthread_t vel_carro_t, rot_motor_t, temp_motor_t, prox_carro_t;

	//criando as threads
	pthread_create(&vel_carro_t, NULL, writeVelCarro, NULL);
	pthread_create(&rot_motor_t, NULL, writeRotMotor, NULL);
	pthread_create(&temp_motor_t, NULL, writeTempMotor, NULL);
	pthread_create(&prox_carro_t, NULL, writeProxCarro, NULL);

	//esperando as threads
	pthread_join(vel_carro_t, NULL);
	pthread_join(rot_motor_t, NULL);
	pthread_join(temp_motor_t, NULL);
	pthread_join(prox_carro_t, NULL);

	// Desanexar memória e fechar semáforo
    	munmap(valor_coletado, sizeof(Data));
    	close(shm_fd);
    	sem_close(sem);
    	sem_unlink(SEM_NAME);
    	shm_unlink(SHM_NAME);

	return 0;	

}
