//============================================================================
// Name        : Alg_ex_7.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <cmath>

#define NMAX 100000
using namespace std;

struct tree_node {
	int data;
    int degree;
    bool mark; // is it node loose his sons
    tree_node *parent;
    vector<tree_node *> children;
    tree_node *left;
    tree_node *right;
};

void delete_tree(tree_node *cur)
{
	if (cur == NULL)
		return;
	if (cur->degree != 0)
		for (unsigned int i = 0; i < cur->children.size(); i++)
			delete_tree(cur->children[i]);

	delete cur;
}

struct Heap {
    vector<tree_node *> nodes;
    int n;
    tree_node *min;
};

class Fibonacci_heap {

public:
	vector<Heap *> heaps;
	tree_node *head;

	Fibonacci_heap();
	~Fibonacci_heap() {};
	Heap *construct();
	int find_position(Heap *f_heap, tree_node *x);
	void cut(Heap *f_heap, tree_node *x, tree_node *y);
	void cascading_cut(Heap *f_heap, tree_node *y);
	void heap_link(Heap *f_heap, tree_node *x, tree_node *y);
	void insert_data(Heap *f_heap, int data);
	void insert_node(Heap *f_heap, tree_node *x);
	void unite_heaps(int first, int second);
	void heap_delete(Heap *f_heap);
	void delete_node(Heap *f_heap, tree_node *x);
	void consolidate(Heap *f_heap);
	tree_node *extract_min(Heap *f_heap);
	void decrease_key(Heap *f_heap, tree_node *x, int data);
};

Fibonacci_heap::Fibonacci_heap()
{
	heaps.push_back(construct());
	for (int i = 0; i < 10; i++)
		insert_data(heaps[0], i);
}

Heap *Fibonacci_heap::construct()
{
	Heap *new_heap = new Heap;
	new_heap->min = NULL;
	new_heap->n = 0;
	return new_heap;
}

int Fibonacci_heap::find_position(Heap *f_heap, tree_node *x)
{
	if (f_heap != NULL)
		for (unsigned int i = 0; i < f_heap->nodes.size(); i++)
			if (f_heap->nodes[i]->data == x->data)
				return i;
	return 0;
}

void Fibonacci_heap::cut(Heap *f_heap, tree_node *x, tree_node *y)
{
	for (unsigned int i = 0; i < y->children.size(); i++)
		if (y->children[i]->data == x->data) {
			y->children[i]->left->right = y->children[i]->right;
			y->children[i]->right->left = y->children[i]->left;
			y->degree--;
		}
	insert_node(f_heap, x);
	x->parent = NULL;
	x->mark = false;
}

void Fibonacci_heap::cascading_cut(Heap *f_heap, tree_node *y)
{
	tree_node *z = y->parent;
	if (z != NULL) {
		if (y->mark == false)
			y->mark = true;
		else {
			cut(f_heap, y, z);
			cascading_cut(f_heap, z);
		}
	}
}

void Fibonacci_heap::heap_link(Heap *f_heap, tree_node *y, tree_node *x)
{
	int i = find_position(f_heap, y);
	y->children[i]->left->right = y->children[i]->right;
	y->children[i]->right->left = y->children[i]->left;

	x->children.push_back(y);
	if (x->children.size() == 1)
		y->left = y->right = y;
	else {
		y->left = x->children[x->children.size() - 2];
		y->right = x->children[0];
	}
	x->degree = x->degree + y->degree;
	y->mark = false;
}

void Fibonacci_heap::insert_data(Heap *f_heap, int data)
{
	tree_node *x = new tree_node;
	x->degree = 0;
	x->parent = NULL;
	x->mark = false;
	x->data = data;

	f_heap->nodes.push_back(x);

	if (f_heap->min == NULL) {
		f_heap->min = x;
		x->left = x;
		x->right = x;
	}
	else if (x->data < f_heap->min->data) {
		f_heap->min = x;
		f_heap->nodes[f_heap->nodes.size() - 1]->right = x;
		f_heap->nodes[0]->left = x;
		x->left = f_heap->nodes[f_heap->nodes.size() - 2];
		x->right = f_heap->nodes[0];
	}
	f_heap->n++;
}

void Fibonacci_heap::insert_node(Heap *f_heap, tree_node *x)
{
	x->parent = NULL;
	x->mark = false;

	if (f_heap == NULL)
		f_heap = construct();

	f_heap->nodes.push_back(x);

	if (f_heap->min == NULL) {
		f_heap->min = x;
		x->left = x;
		x->right = x;
	}
	else if (x->data < f_heap->min->data) {
		f_heap->min = x;
		f_heap->nodes[f_heap->nodes.size() - 1]->right = x;
		f_heap->nodes[0]->left = x;
		x->left = f_heap->nodes[f_heap->nodes.size() - 2];
		x->right = f_heap->nodes[0];
	}
	f_heap->n++;
}

void Fibonacci_heap::unite_heaps(int first, int second) //index of heaps in vector
{
	heaps.push_back(construct());
	if (heaps[first] != NULL)
		for (int i = 0; i < heaps[first]->n; i++)
			heaps[heaps.size()]->nodes.push_back(heaps[first]->nodes[i]);
	heaps[heaps.size()]->n = heaps[heaps.size()]->n + heaps[first]->n;

	if (heaps[second] != NULL)
		for (int i = 0; i < heaps[first]->n; i++)
			heaps[heaps.size()]->nodes.push_back(heaps[first]->nodes[i]);
	heaps[heaps.size()]->n = heaps[heaps.size()]->n + heaps[second]->n;

	heaps[heaps.size()]->nodes[0]->left = heaps[heaps.size()]->nodes[heaps[heaps.size()]->nodes.size()];
	heaps[heaps.size()]->nodes[heaps[heaps.size()]->nodes.size()]->right = heaps[heaps.size()]->nodes[0];
	heaps[first]->nodes[heaps[first]->nodes.size()]->right = heaps[second]->nodes[0];
	heaps[second]->nodes[0]->left = heaps[first]->nodes[heaps[first]->nodes.size()];

	heaps[heaps.size()]->min = heaps[first]->min;

	if ((heaps[heaps.size()]->min == NULL) || ((heaps[second]->min != NULL) && (heaps[second]->min->data < heaps[heaps.size()]->min->data)))
		heaps[heaps.size()]->min = heaps[second]->min;

	heap_delete(heaps[first]);
	heap_delete(heaps[second]);
	heaps.erase(heaps.begin() + first); // delete first
	heaps.erase(heaps.begin() + second); // delete second
}

void Fibonacci_heap::heap_delete(Heap *f_heap)
{
	if (f_heap == NULL)
		return;

	unsigned int i = 0;
	tree_node *cur = f_heap->min;

	while (i != f_heap->nodes.size()) {
		delete_tree(cur);
		cur = cur->right;
		i++;
	}
}

void Fibonacci_heap::delete_node(Heap *f_heap, tree_node *x)
{
	decrease_key(f_heap, x, -NMAX);
	extract_min(f_heap);
}

void Fibonacci_heap::consolidate(Heap *f_heap)
{
	vector<tree_node *> array;
	for (int i = 0; i < (int)log(f_heap->n); i++)
		array.push_back(NULL);
	for (unsigned int i = 0; i < f_heap->nodes.size(); i++) {
		tree_node *x = f_heap->nodes[i];
		int d = x->degree;
		while (array[d] != NULL) {
			tree_node *y = array[d];
			if (x->data > y->data)
				swap(x, y);
			heap_link(f_heap, x, y);
			array[d] = NULL;
			d++;
		}
		array[d] = x;
	}
	f_heap->min = NULL;

	for (int i = 0; i < (int)log(f_heap->n); i++)
		if (array[i] != NULL) {
			if (f_heap->min == NULL) {
				f_heap->nodes.push_back(array[i]);
				f_heap->min = array[i];
				array[i]->left = array[i];
				array[i]->right = array[i];
			}
			else
				insert_node(f_heap, array[i]);
		}
}

tree_node *Fibonacci_heap::extract_min(Heap *f_heap)
{
	tree_node *to_extract = f_heap->min;
	if (to_extract != NULL) {
		for (unsigned int i = 0; i < to_extract->children.size(); i++)
			insert_node(f_heap, to_extract->children[i]);
	}
	f_heap->min = NULL;

	int i = find_position(f_heap, to_extract);
	tree_node *y = f_heap->nodes[i];
	y->children[i]->left->right = y->children[i]->right;
	y->children[i]->right->left = y->children[i]->left;
	f_heap->nodes.erase(f_heap->nodes.begin() + i);

	if (to_extract == to_extract->right)
		f_heap = NULL;
	else {
		f_heap->min = f_heap->nodes[i + 1];
		consolidate(f_heap);
	}
	f_heap->n--;
	return to_extract;
}

void Fibonacci_heap::decrease_key(Heap *f_heap, tree_node *x, int data)
{
	if (data > x->data) {
		cout << "ERROR!" << endl;
		return;
	}
	x->data = data;
	tree_node *y = x->parent;
	if ((y != NULL) && (x->data < y->data)) {
		cut(f_heap, x, y);
		cascading_cut(f_heap, y);
	}
	if (x->data < f_heap->min->data)
		f_heap->min = x;
}

int main() {
	Fibonacci_heap new_heap;
	return 0;
}
