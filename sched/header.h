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

// ==================== METHODS ==========================

// Inicializa��o
List* newList();
CircularList* newCircularList();

// Manipula��o de Node
int addNode(List *L, Process *node);
int removeNode(List *L, Process *node);

int addNodeC(CircularList *L, Process *node);
int removeNodeC(CircularList *L, Process *node);
Process* next(CircularList *L);

void ReadFile(char *path);
void InitProgram();

void addProcess(Process *p);
void removeProcess(Process *p);