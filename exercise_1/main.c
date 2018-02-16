#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	int data;
	struct list* next;
} list_t;

list_t *new_list;

void reverse_rec(list_t *head)
{
	list_t *timetable = head->next;
	if (timetable == NULL) {
		printf("\nList have only 1 element.\n");
		return;
	}
	if (timetable->next != NULL)
		reverse_rec(timetable);
	else
		new_list = timetable;
	timetable->next = head;
	head->next = NULL;
}

void reverse_without_rec(list_t *head)
{
	list_t *first = head->next; //Need to save a next element of list
	if (first == NULL) {
		printf("\nList have only 1 element.\n");
		return;
	}
	head->next = NULL;
	list_t *second = first->next;//Next element of saving
	while (first->next != NULL) {
		first->next = head;//Change link of saving to head
		head = first;//Now head is on "first" position
		first = second;//Change position
		second = second->next;//Moving in list
	}
	if (first->next == NULL) {//reach the end of list
		new_list = first;//become a new head of list
		first->next = head;
	}
}
list_t *push(list_t *head, int data)
{
	list_t *timetable = (list_t*)malloc(sizeof(list_t));
	if (timetable == NULL)
		return NULL;
	timetable->data = data;
	timetable->next = head;
	head = timetable;
	return head;
}

void get_result(list_t *head)
{
	if (head != NULL)
		printf(" %d", head->data);
	if (head->next != NULL)
		get_result(head->next);
}
//comments, for testing
int main ()
{
	for (int i = 0; i < 4; i++) {
		new_list = push(new_list, i);
		if (new_list == NULL) {
			printf("\nError, not enough memory for new element of list.");
			return 0;
		}
	}
	get_result(new_list);

	reverse_without_rec(new_list);

	printf("\n Reversed : ");
	get_result(new_list);
	return 0;
}
