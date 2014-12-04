//#ifndef  HEADER 
#define isNumeric(num) (num >= '0' && num <= '9')

// [id]; [instante de chegada]; [tempo de execu��o]; [instante inicial do IO], [instante final do IO];
typedef struct structProcess
{
	// Informa��es de inicializa��o
	int Id;
	int ArrivalTime;
	int ExecutionTimeNeeded;
	int IOStartTime;
	int IOEndTime;

	// Informa��es de execu��o
	int ExecutingTime;
	int Quantum;

	// Informa��es de Node
	struct structProcess *next;
	struct structProcess *previous;
} Process;

typedef struct
{
	Process *begin; //Endere�o do primeiro elemento da lista
	Process *end; //Endere�o do �ltimo elemento ADICIONADO na lista
	int size; //Cont�m tamanho da lista
} List;

typedef struct
{
	Process *head; //Endere�o atual
	int size; //Cont�m tamanho da lista
} CircularList;

typedef struct
{
	enum { RR = 0, FIFO, SJF } algorithm;
	union
	{
		List *LinkedList;
		CircularList *CircularList;
	} list;
} ProcessScheduler;

typedef struct
{
	int Clock;
	Process *ExecProcess;
} CPU;

// ==================== METHODS ==========================

// Inicializa��o
List* newList();
CircularList* newCircularList();

// Manipula��o de Node
int addNode(List *L, Process *node);
int removeNode(List *L, Process *node, int useFree);

int addNodeC(CircularList *L, Process *node);
int removeNodeC(CircularList *L, Process *node, int useFree);
Process* next(CircularList *L);

void LoadFile(char *path);
void InitProgram();

void addProcess(Process *p);
void removeProcess(Process *p);

Process* GetNextProcess();
void SetCPUProcess(Process *p);
void printProcesses();
Process* newProcess();
Process* GetCopy(Process p);


void Clock();
Process* DoFIFO();
Process* DoSJF();
Process* DoFIFO();