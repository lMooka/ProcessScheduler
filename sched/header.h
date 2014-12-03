//#ifndef  HEADER 
#define isNumeric(num) (num >= '0' && num <= '9')

// [id]; [instante de chegada]; [tempo de execução]; [instante inicial do IO], [instante final do IO];
typedef struct structProcess
{
	// Informações de inicialização
	int Id;
	int ArrivalTime;
	int ExecutionTimeNeeded;
	int IOStartTime;
	int IOEndTime;

	// Informações de execução
	int ExecutingTime;
	int Quantum;

	// Informações de Node
	struct structProcess *next;
	struct structProcess *previous;
} Process;

typedef struct
{
	Process *begin; //Endereço do primeiro elemento da lista
	Process *end; //Endereço do último elemento ADICIONADO na lista
	int size; //Contém tamanho da lista
} List;

typedef struct
{
	Process *head; //Endereço atual
	int size; //Contém tamanho da lista
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

// Inicialização
List* newList();
CircularList* newCircularList();

// Manipulação de Node
int addNode(List *L, Process *node);
int removeNode(List *L, Process *node);

int addNodeC(CircularList *L, Process *node);
int removeNodeC(CircularList *L, Process *node);
Process* next(CircularList *L);

void ReadFile(char *path);
void InitProgram();

void addProcess(Process *p);
void removeProcess(Process *p);