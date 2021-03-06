#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include "TCB.h"

/*
* Group members: 
* 1. Sanjay Arivazhagan; ASURITE ID: 1217643921; sarivazh@asu.edu 
* 2. Sethu Manickam; ASURITE ID: 1218452066; smanick4@asu.edu
*/


struct q_element{
	struct q_element *pPrev, *pNext;
	int iValue;
	TCB_t *TCB;
};

typedef struct q_element q_element;

/*
* Function Name: newQueue
* Parameters: None
* Return type: TCB_t*
* Description: Function to return a new queue of TCB_t's
*/
struct TCB_t* newQueue()
{
	struct TCB_t *queue = NULL;
	return queue;
}

/*
* Function Name: newItem
* Parameters: None
* Return type: TCB_t*
* Description: Function to return a pointer to a new element, uses memory allocation
*/
struct TCB_t* newItem()
{
	struct TCB_t *new_item = (struct TCB_t*)malloc(sizeof(struct TCB_t));
	if(new_item == NULL)
	{
		printf("Error: Failed to create the q element");
		errno = ENOMEM;
		return NULL;
	}
	return new_item;
}


/*
* Function Name: AddQueue
* Parameters: struct TCB_t **head, struct TCB_t *item
* Return type: void
* Description: Function to adds a queue item, pointed to by “item”, to the queue pointed to by head
*/
void AddQueue(struct TCB_t **head, struct TCB_t *item)
{
	struct TCB_t *temp = *head;
	
	if(*head == NULL)
	{
		*head = item;
		(*head)->pNext = *head;
		(*head)->pPrev = *head;
		return;
	}
	while(temp->pNext != *head)
	{
		temp = temp->pNext;
	}
	item->pNext = *head;
	item->pPrev = temp;
	temp->pNext = item;
	(*head)->pPrev = item;
}

/*
* Function Name: DelQueue
* Parameters: struct TCB_t **head
* Return type: TCB_t*
* Description: Function to delete an item from head and returns a pointer to the deleted item. If the queue is already empty, flag error
*/
struct TCB_t* DelQueue(struct TCB_t **head)
{
	struct TCB_t *back = *head;
	struct TCB_t *deleted_element = *head;

	if(*head == NULL)
	{
		printf("Error: The queue is empty \n");
		errno = EPERM;
		return NULL;
	}

	if((*head)->pNext == *head)
	{
		*head = NULL;
		return deleted_element;
	}

	while(back->pNext != *head)
	{
		back = back->pNext;
	}

	*head = (*head)->pNext;
	(*head)->pPrev = back;
	back->pNext = *head;
	deleted_element->pNext = NULL;
	deleted_element->pPrev = NULL;
	return deleted_element;
}

/*
* Function Name: FreeItem
* Parameters: struct TCB_t *item
* Return type: void
* Description: Function to free item
*/
void FreeItem(struct TCB_t *item)
{
	if(item != NULL)
	{
		free(item);
		item = NULL;
	}
}
