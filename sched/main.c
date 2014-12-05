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

	CPUStart();
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
	printf("\n\n\n");
}

void InitProgram(char* algorithm, char* inputFile, char* outputFile)
{
	printf("%s %s %s \n", algorithm, inputFile, outputFile);
	processList = newList();

	pScheduler = (ProcessScheduler*) malloc(sizeof(ProcessScheduler));
	pScheduler->list.CircularList = newCircularList();
	pScheduler->list.LinkedList = newList();

	if(strcmp(algorithm, "FCFS"))
		pScheduler->algorithm = FCFS;
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
	removeNode(processList, p, 0);
}

void CPUStart()
{

	while(1)
	{
		Clock();
	}
}

void Clock()
{
	Sleep(1000);
	printf("\n[Clock %d]\n\n", cpu->Clock);

	DoIO();
	ScheduleProcesses();

	if(cpu->ExecProcess == NULL)
	{
		printf("Nenhum processo executando.\n");
	}
	else
	{
		cpu->ExecProcess->ExecutingTime++;
		printf("Processo %d executando. [%d/%d]\n", cpu->ExecProcess->Id, cpu->ExecProcess->ExecutingTime, cpu->ExecProcess->ExecutionTimeNeeded);
	}

	cpu->Clock++;
}

void DoIO()
{

	switch(pScheduler->algorithm)
	{
	case FCFS:
	case SJF:
		{
			Process *p = pScheduler->list.LinkedList->begin;
			Process *endp = pScheduler->list.LinkedList->end;

			if(p == NULL)
				return;

			while(1)
			{
				if(p->IsExecutingIO)
				{
					p->ExecutingTime++;
					if(p->ExecutingTime >= p->IOEndTime && p->IsExecutingIO)
					{
						p->IsExecutingIO = 0;
						printf("Processo %d terminou o IO. [%d/%d]\n", p->Id, p->ExecutingTime, p->ExecutionTimeNeeded);
					}
					else
						printf("Processo %d executando IO. [%d/%d]\n", p->Id, p->ExecutingTime, p->ExecutionTimeNeeded);
				}

				if(p == endp)
					return;

				p = p->next;
			}
		}
		break;

	case RR:
		break;
	}
}

void SetCPUProcess(Process *p)
{
	cpu->ExecProcess = p;
}

void SetNextProcessReady()
{
	if(pScheduler->algorithm != RR)
	{
		Process *p;
		Process *pend;

		p = pScheduler->list.LinkedList->begin;
		pend = pScheduler->list.LinkedList->end;
		while(1)
		{
			// Verifica IO
			if(p != pend && p->IsExecutingIO)
			{
				p = p->next;
				continue;
			}

			if(p == NULL || p == pend)
			{
				printf("Nenhum processo foi escalonado para executar.\n", cpu->Clock);
				SetCPUProcess(NULL);
				return;
			}
			else
			{
				SetCPUProcess(p);			
				printf("Processo %d foi escalonado para executar.\n", p->Id);
				return;
			}
		}
	}
	else
	{
		// Não implementado ainda
	}
}

// Retorna o próximo processo a ser executado além de escalonar os processos
void ScheduleProcesses()
{
	switch (pScheduler->algorithm)
	{
	case FCFS:
		DoFCFS(); break;
	case SJF:
		DoSJF(); break;
	case RR:
		DoRR(); break;
	}
}

void DoFCFS()
{
	Process *p;

	// Verifica se algum processo se inicializa com o CLOCK atual e o adiciona ao escalonador.
	p = processList->begin;
	while(p != NULL)
	{
		if(p->ArrivalTime == cpu->Clock)
		{
			Process* nextp = p->next;

			removeNode(processList, p, 0);

			p->next = NULL;
			p->previous = NULL;
			addNode(pScheduler->list.LinkedList, p);

			printf("Novo processo de id %d. Aguardando escalonamento.\n", p->Id);
			p = nextp;
			continue;
		}

		p = p->next;
	}

	if(pScheduler->list.LinkedList->size == 0 && processList->size == 0)
	{
		printf("Execução finalizada.");
		exit(0);
	}

	// Adiciona um processo caso cpu esteja livre, se possível.
	if(cpu->ExecProcess == NULL)
	{
		SetNextProcessReady();
		return;
	}

	// Realiza escalonamento
	if(cpu->ExecProcess->ExecutingTime >= cpu->ExecProcess->ExecutionTimeNeeded)
	{
		printf("Processo %d terminou de executar.\n", cpu->ExecProcess->Id);
		removeNode(pScheduler->list.LinkedList, cpu->ExecProcess, 0);
		SetCPUProcess(NULL);
		return;
	} // Entrou em IO
	else if(cpu->ExecProcess->ExecutingTime == cpu->ExecProcess->IOStartTime)
	{
		// Adiciona o node ao final da lista (perda de prioridade)
		removeNode(pScheduler->list.LinkedList, cpu->ExecProcess, 0);
		addNode(pScheduler->list.LinkedList, cpu->ExecProcess);

		// Define que o processo está em IO
		cpu->ExecProcess->IsExecutingIO = 1;
		printf("Processo %d entrou em IO.\n", cpu->ExecProcess->Id);

		// Define o próximo para pronto para executar
		SetNextProcessReady();

		return;
	}
}

void DoSJF()
{

}

void DoRR()
{

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
		fprintf(stderr, "arquivo %s não foi encontrado.\n", path);
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
				fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.\n");
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
					addProcess(p);
					return;
				}
				else if(c == '\n')
				{
					state = 0;
					addProcess(p);
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
					fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.\n");
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
					fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.\n");
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
					fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.\n");
					exit(1);
					return;
				}
			}
			else
			{
				fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.\n");
				exit(1);
				return;
			}
		}
	}
}
