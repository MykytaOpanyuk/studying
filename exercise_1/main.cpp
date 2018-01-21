#include <stdio.h>
#include <stdlib.h>

struct list
{
	int data;
	list* next;
};

list *new_list = NULL;

void reverse_rec(list *head)
{
	list *timetable = head->next;
	if (timetable->next != NULL)
		reverse_rec(timetable);
	else
		new_list = timetable;
	timetable->next = head;
	head->next = NULL;
}

void reverse_without_rec(list *head)
{
	list *timetable = head->next;
	head->next = NULL;
	list *another = timetable->next;
	while (timetable->next != NULL) {
		timetable->next = head;
		head = timetable;
		timetable = another;
		another = another->next;
	}
	if (timetable->next == NULL) {
		new_list = timetable;
		timetable->next = head;
	}
}
list *push(list *head, int data) {
	list *timetable = (list*)malloc(sizeof(list));
	timetable->data = data;
	timetable->next = head;
	head = timetable;
	return head;
}

void get_result(list *head) {
	printf(" %d", head->data);
	if (head->next != NULL)
		get_result(head->next);
}
//comments, for testing
int main ()
{
	for (int i = 0; i < 5; i++)
		new_list = push(new_list, i);

	get_result(new_list);
	printf("\n");

	reverse_without_rec(new_list);

	get_result(new_list);
}
