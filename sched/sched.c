#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Assinaturas de Metodos
void ReadArgs(char **args);
void ReadInputFile();
void ReadFileLine(int line);
void SwitchAlgorithm();
void RunFCFS();
void RunRR();
void RunSJF();
void CPU();

// Variaveis Globais
char *algorithm, *inputFileName, *outputFileName;
int selectedAlgorithm;
int processCount;
int currentProcess;
int currentProcessTick;

FILE *inputFile, *outputFile;

int processId, startTime, processDuration, ioStart, ioEnd;

int main2(int argc, char **argv){
	system("clear");

	// Trata os valores da entrada de Dados
	ReadArgs(argv);
	ReadInputFile();

	// Escolhe o algoritmo
	SwitchAlgorithm();

	// Executa o Algoritmo Escolhido
	switch(selectedAlgorithm){		
	case 0:
		RunFCFS();
		break;

	case 1:
		RunRR();
		break;

	case 2:
		RunSJF();
		break;
	}

	return 0;
}

void ReadArgs(char **args){
	algorithm = args[1];
	inputFileName = args[2];
	outputFileName = args[3];
}

// Le o arquivo de entrada, e descobre quantos processos entraram na fila
void ReadInputFile(){	
	if(!(inputFile = fopen(inputFileName, "r")))
	{
		fprintf(stderr, "Arquivo de Entrada invalido. Encerrando execucao\n");
		exit(0);
	}

	fseek(inputFile, 0, SEEK_SET);

	while(1){
		fscanf(inputFile, "%d;%d;%d", &processId, &startTime, &processDuration);

		if(processId == 0){
			printf("Fim de Arquivo.\n");
			break;
		}

		printf("ID do Processo: %d. Tempo Inicial: %d\n", processId, startTime);
		processCount++;
	}

	fclose(inputFile);
}

void ReadFileLine(int line){
	if(!(inputFile = fopen(inputFileName, "r"))){
		fprintf(stderr, "Arquivo de Entrada invalido. Encerrando execucao.\n");
		exit(0);
	}

	fseek(inputFile, 0, SEEK_SET);

	while(line >= 0){
		fscanf(inputFile, "%d;%d;%d", &processId, &startTime, &processDuration);
		line --;
	}

	fclose(inputFile);
}

void SwitchAlgorithm(){
	if(strcmp(algorithm, "FCFS") == 0)
		selectedAlgorithm = 0;

	if(strcmp(algorithm, "RR") == 0)
		selectedAlgorithm = 1;

	if(strcmp(algorithm, "SJF") == 0)
		selectedAlgorithm = 2;
}

void RunFCFS(){
	printf("--- First Come First Served ---\n");

	for(currentProcess = 0; currentProcess < processCount; currentProcess++){
		ReadFileLine(currentProcess);

		for(currentProcessTick = 0; currentProcessTick < processDuration; currentProcessTick++)
			CPU();
	}

	printf("--- Concluido ---\n");
}

void RunRR(){
	printf("--- Round Robin ---\n");

	printf("--- Concluido ---\n");
}

void RunSJF(){
	printf("--- Shortest Job First ---\n");

	printf("--- Concluido ---\n");
}

void CPU(){
	if(currentProcess != -1)
		printf("Executando Processo %d\n", processId);
	else
		printf("CPU idle\n");
}

