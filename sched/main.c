#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <sys/types.h>
#include <Windows.h> // #include <unistd.h> ON LINUX

CPU *cpu;
List *processList;
ProcessScheduler *pScheduler;

int main(int argc, char **argv)
{
	int i;
	int size;

	if(argc < 1)
	{
		fprintf(stderr, "Erro de parametro");
		exit(1);
	}

	InitProgram(argv[1],argv[2], argv[3]);
	printProcesses();
	getchar();
}

void printProcesses()
{
	Process *p;
	int size = pScheduler->list.CircularList->size;

	//printf("id\ta time\te time\tIO start\tIO end\n\n");
	p = processList->begin;
	while(p != NULL)
	{
		printf("%d\t%d\t%d\t%d\t%d \n", p->Id, p->ArrivalTime, p->ExecutionTimeNeeded, p->IOStartTime, p->IOEndTime);
		p = p->next;
	}

}

void InitProgram(char* algorithm, char* inputFile, char* outputFile)
{
	printf("%s %s %s \n", algorithm, inputFile, outputFile);
	processList = newList();

	pScheduler = (ProcessScheduler*) malloc(sizeof(ProcessScheduler));
	pScheduler->list.CircularList = newCircularList();
	pScheduler->list.LinkedList = newList();

	if(strcmp(algorithm, "FIFO"))
		pScheduler->algorithm = FIFO;
	else if(strcmp(algorithm, "SJF"))
		pScheduler->algorithm = SJF;
	else if(strcmp(algorithm, "RR"))
		pScheduler->algorithm = RR;

	cpu = (CPU*) malloc(sizeof(CPU));
	cpu->Clock = 0;
	cpu->ExecProcess = NULL;

	LoadFile(inputFile);

	return;
}

void addProcess(Process *p)
{
	addNode(processList, p);
}

void removeProcess(Process *p)
{
	switch (pScheduler->algorithm)
	{
	case FIFO:
	case SJF:
		removeNode(pScheduler->list.LinkedList, p, 0);
		break;

	case RR:
		removeNodeC(pScheduler->list.CircularList, p, 0);
		break;
	}
}

void CPUStart()
{

	while(1)
	{
		Process *p = GetNextProcess();

		if(p != NULL)
			SetCPUProcess(p);
		Clock();
	}
}

void Clock()
{
	printf("[Clock %d] Executando Processo: %d.", cpu->Clock, cpu->ExecProcess->Id);
	Sleep(1000);
	cpu->Clock++;
	cpu->ExecProcess->ExecutingTime++;
}

void SetCPUProcess(Process *p)
{
	cpu->ExecProcess = p;
}

// Retorna o próximo processo a ser executado além de escalonar os processos
Process* ScheduleProcesses()
{
	switch (pScheduler->algorithm)
	{
	case FIFO:
		return DoFIFO();
	case SJF:
		return DoSJF();
	case RR:
		return DoRR();
	}
}

Process* DoFIFO()
{
	Process *p;

	// Verifica se algum processo se inicializa com o CLOCK atual e o adiciona ao escalonador.
	p = processList->begin;
	while(p != NULL)
	{
		if(p->ArrivalTime == cpu->Clock)
		{
			Process* auxp = p;

			p = p->next;
			removeNode(processList, auxp, 0);

			auxp->next = NULL;
			auxp->previous = NULL;
			addNode(pScheduler->list.LinkedList, auxp);
		}
	}

	// Realiza escalonamento
	if(cpu->ExecProcess->ExecutingTime >= cpu->ExecProcess->ExecutionTimeNeeded)
	{
		printf("[%d] Processo %d terminou sua execução.", cpu->Clock, cpu->ExecProcess->Id);
		removeNode(pScheduler->list.LinkedList, cpu->ExecProcess, 0);
	}
	else if(cpu->ExecProcess->IOStartTime >= cpu->Clock && cpu->ExecProcess->IOStartTime <= cpu->Clock)
	{
		printf("[%d] Processo %d entrou em IO.", cpu->Clock, cpu->ExecProcess->Id);
	}

	return NULL;
}


Process* DoSJF()
{
	int i;
	return NULL;
}

Process* DoRR()
{
	int i;
	return NULL;
}


// ReadFile utiliza conceito de Máquina de Estado Finito, maiores dúvidas observar diagrama no arquivo ReadFileFMS.png
void LoadFile(char *path)
{
	char c;
	char buf[10];
	int bufCounter = 0;
	int state = 0;
	FILE *fp = NULL;
	Process *p = NULL;

	// Abre arquivo e verifica se ocorreu erros
	fp = fopen("entrada.txt", "r");
	if(!fp)
	{
		fprintf(stderr, "arquivo %s não foi encontrado.", path);
		exit(1);
		return;
	}

	p = newProcess();

	while(1)
	{
		c = (char)fgetc(fp);

		if(state < 3)
		{
			if(isNumeric(c))
			{
				// Adiciona o caractere lido ao buffer e depois incrementa o bufCounter
				buf[bufCounter++] = c;
			}
			else if(c == ';')
			{
				// Adiciona o caractere finalizador de string \0
				buf[bufCounter] = '\0';
				// Reseta contador do buf
				bufCounter = 0;

				// Decide o local para salvar a informação.
				switch(state)
				{
				case 0: p->Id = atoi(buf); break;
				case 1: p->ArrivalTime = atoi(buf); break;
				case 2: p->ExecutionTimeNeeded = atoi(buf); break;
				}

				state++;
				continue;
			}
			else if(c == ' ')
			{
				continue;
			}
			else
			{
				fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.");
				exit(1);
				return;
			}
		}
		else if(state <= 6) 
		{
			if(state == 3)
			{
				if(c == EOF)
				{
					p->IOStartTime =  0;
					p->IOEndTime = 0;
					addProcess(p);
					return;
				}
				else if(c == '\n')
				{
					state = 0;
					addProcess(p);
					p->IOStartTime =  0;
					p->IOEndTime = 0;
					p = newProcess();
					continue;
				}
				else if(isNumeric(c))
				{
					buf[bufCounter++] = c;
					state++;
					continue;
				}
				else
				{
					fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.");
					exit(1);
					return;
				}
			}
			else if(state == 4 || state == 5)
			{
				if(isNumeric(c))
				{
					buf[bufCounter++] = c;
				}
				else if(c == ',' && state == 4)
				{
					// Adiciona o caractere finalizador de string \0
					buf[bufCounter] = '\0';
					// Reseta contador do buf
					bufCounter = 0;
					// Converte a string para int e passa o valor pro IOStartTime
					p->IOStartTime = atoi(buf);
					state++;					
				}
				else if(c == ';' && state == 5)
				{
					// Adiciona o caractere finalizador de string \0
					buf[bufCounter] = '\0';
					// Reseta contador do buf
					bufCounter = 0;
					// Converte a string para int e passa o valor pro IOStartTime
					p->IOEndTime = atoi(buf);
					state++;
				}
				else
				{
					fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.");
					exit(1);
					return;
				}
			}
			else if(state == 6)
			{
				if(c == '\n')
				{
					state = 0;
					addProcess(p);
					p = newProcess();
					continue;
				}
				else if(c == EOF)
				{
					addProcess(p);
					return;
				}
				else
				{
					fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.");
					exit(1);
					return;
				}
			}
			else
			{
				fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.");
				exit(1);
				return;
			}
		}
	}
}
