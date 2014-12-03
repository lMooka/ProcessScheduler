#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <sys/types.h>

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

	InitProgram();

	printf("%s \n", argv[1]);
	ReadFile(argv[1]);
	size = pScheduler->list.CircularList->size;

	//printf("id\ta time\te time\tIO start\tIO end\n\n");
	for(i = 0; i < size; i++)
	{
		Process *p = next(pScheduler->list.CircularList);
		printf("%d\t%d\t%d\t%d\t%d \n", p->Id, p->ArrivalTime, p->ExecutionTimeNeeded, p->IOStartTime, p->IOEndTime);
	}

	getchar();
}

void InitProgram()
{
	pScheduler = (ProcessScheduler*) malloc(sizeof(ProcessScheduler));
	pScheduler->list.CircularList = newCircularList();
	pScheduler->list.LinkedList = newList();
	pScheduler->algorithm = RR;
	return;
}

// ReadFile utiliza conceito de Máquina de Estado Finito, maiores dúvidas observar diagrama no arquivo ReadFileFMS.png
void ReadFile(char *path)
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
		fprintf(stderr, "arquivo entrada.txt não foi encontrado.");
		exit(1);
		return;
	}

	p = (Process*) malloc(sizeof(Process));

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
					p = (Process*) malloc(sizeof(Process));
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
					return;
				}
			}
			else if(state == 6)
			{
				if(c == '\n')
				{
					state = 0;
					addProcess(p);
					p = (Process*) malloc(sizeof(Process));
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
					return;
				}
			}
			else
			{
				fprintf(stderr, "Arquivo em formato incorreto, impossível leitura.");
				return;
			}
		}
	}
}

void addProcess(Process *p)
{
	switch (pScheduler->algorithm)
	{
	case FIFO:
	case SJF:
		addNode(pScheduler->list.LinkedList, p);
		break;

	case RR:
		addNodeC(pScheduler->list.CircularList, p);
		break;
	}
}

void removeProcess(Process *p)
{
	switch (pScheduler->algorithm)
	{
	case FIFO:
	case SJF:
		removeNode(pScheduler->list.LinkedList, p);
		break;

	case RR:
		removeNodeC(pScheduler->list.CircularList, p);
		break;
	}
}

void SetCPUProcess(Process *p)
{

}

void CPUInterruption()
{

}