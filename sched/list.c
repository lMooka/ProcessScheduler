#include <stdlib.h>
#include <stdio.h>
#include "header.h"

// ***********************************************
// ******  initList                         ******
// ***********************************************
// Inicializa a Lista alocando um espaço na memória
// e definine configurações iniciais.
// Retorna o ponteira da Lista alocada.
List* newList()
{
	List *L = (List*) malloc(sizeof(List));

	//Realiza as definições iniciais
	L->begin = NULL;
	L->end = NULL;
	L->size = 0;

	return L;
}

// ***********************************************
// ******  addNode                          ******
// ***********************************************
// Adiciona um node na lista.
int addNode(List *L, Process *node)
{
	// Verifica se o elemento passado é igual a NULL,
	// se sim, não há necessidade de continuar.
	if(!L || !node)
		return 0;

	// Verifica se a lista está vazia
	if(L->begin == NULL)
	{
		L->begin = node;
		L->end = node;
		node->next = NULL;
		node->previous = NULL;
	}
	else
	{
		node->previous = L->end;
		L->end->next = node;
		L->end = node;
		node->next = NULL;
	}

	L->size++;

	return 1;
}
// ***********************************************
// ******  removeNode                       ******
// ***********************************************
// Remove um node da lista.
int removeNode(List *L, Process *node, int useFree)
{
	//Elemento NULL
	if(!node) return 0;


	if(node == L->begin && node == L->end)
	{
		L->begin = NULL;
		L->end = NULL;
		L->size--;
	}
	else if(node == L->begin)
	{

		L->begin->next->previous = NULL;
		L->begin = L->begin->next;
		L->size--;
	}
	else if(node == L->end)
	{
		L->end->previous->next = NULL;
		L->end = L->end->previous;
		L->size--;
	}
	else
	{
		node->previous->next = node->next;
		node->next->previous = node->previous;
		L->size--;
	}

	if(useFree) 
		free(node);

	return 1;
}

// ==================================================================================


// ***********************************************
// ******  initCircularList                 ******
// ***********************************************
// Inicializa a Lista alocando um espaço na memória
// e definine configurações iniciais.
// Retorna o ponteira da Lista alocada.
CircularList* newCircularList()
{
	CircularList *L = (CircularList*) malloc(sizeof(CircularList));

	//Realiza as definições iniciais
	L->head = NULL;
	L->size = 0;

	return L;
}

// ***********************************************
// ******  addNode                          ******
// ***********************************************
// Adiciona um node na lista.
int addNodeC(CircularList *L, Process *node)
{
	Process *h = L->head;

	// Verifica se o elemento passado é igual a NULL,
	// se sim, não há necessidade de continuar.
	if(!L || !node)
		return 0;

	// Verifica se a lista está vazia
	if(L->size == 0)
	{
		L->head = node;
		node->next = node;
		node->previous = node;
	}
	else
	{
		Process *h = L->head;

		node->next = h->next;
		node->previous = h;

		h->next->previous = node;
		h->next = node;
	}

	L->size++;

	return 1;
}
// ***********************************************
// ******  removeNode                       ******
// ***********************************************
// Remove um node da lista.
int removeNodeC(CircularList *L, Process *node, int useFree)
{
	//Elemento NULL
	if(!node)
		return 0;

	if(node == L->head)
	{
		L->head = NULL;
		node->next = NULL;
		node->previous = NULL;
		L->size = 0;
	}
	else
	{
		node->previous->next = node->next;
		node->next->previous = node->previous;
		L->size--;
	}

	if(useFree)
		free(node);

	return 1;
}

Process* next(CircularList *L)
{
	L->head = L->head->next;
	return L->head;
}

Process* newProcess()
{
	Process *p = (Process*) malloc(sizeof(Process));

	p->Id = -1;
	p->ArrivalTime = -1;
	p->ExecutionTimeNeeded = -1;
	p->IOStartTime = -1;
	p->IOEndTime = -1;
	p->ExecutingTime = 0;
	p->ExecutingQuantum = 0;
	p->IsExecutingIO = 0;
	p->next = NULL;
	p->previous = NULL;	

	return p;
}