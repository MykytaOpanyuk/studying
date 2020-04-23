//============================================================================
// Name        : ALG_ex_2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#define NMAX 10

typedef enum {Black, Red} node_colour;

struct Node {
	node_colour colour;
	int data;
	unsigned int rank;
	Node *left;
	Node *right;
	Node *parent;
};

class RB_tree {
public :

	Node *son; // like NULL node
	Node *root; //root of RB tree

	~RB_tree() {};
	RB_tree();

	void rotate_left(Node *x);
	void rotate_right(Node *x);
	void insert_balance(Node *x);
	Node *insert_node(int data);
	void delete_balance(Node *x);
	void delete_node(Node *x);
	Node *is_node(int data);
	bool cout_tree(Node *current);
	Node *os_select(Node *x, unsigned int i);
	int os_rank(Node *x);
	bool delete_tree(Node *current);
	void RBT_menu();
};

RB_tree::RB_tree ()
{
	son = new Node;
    son->left = NULL;
    son->right = NULL;
    son->data = 0;
    son->parent = NULL;
    son->colour = Black;
    son->rank = 0;

    root = son;

    for (int i = 0; i < NMAX; ++i)
        if (insert_node(i) == NULL) {
        	cout << "Out of memory" << endl;
        	break;
        }
    RBT_menu();
}

void RB_tree::rotate_left(Node *x)
{
    Node *y = x->right;

    x->right = y->left; //establish x->right link
    if (y->left != son)
    	y->left->parent = x;
    if (y != son) //establish y->parent link
    	y->parent = x->parent;
    if (x->parent == NULL)
    	root = y;
    else if (x == x->parent->left)
    	x->parent->left = y;
    else
    	x->parent->right = y;

    y->left = x;// link x and y

    if (x != son)
    	x->parent = y;

    y->rank = x->rank;
    x->rank = x->left->rank + x->right->rank + 1;
}

void RB_tree::rotate_right(Node *x)
{
    Node *y = x->left;

    x->left = y->right; // establish x->left link
    if (y->right != son)
    	y->right->parent = x;
    if (y != son) // establish y->parent link
    	y->parent = x->parent;
    if (x->parent == NULL)
    	root = y;
    else if (x == x->parent->right)
    	x->parent->left = y;
    else
    	x->parent->right = y;

    y->right = x; // link x and y

    if (x != son)
    	x->parent = y;

    y->rank = x->rank;
    x->rank = x->left->rank + x->right->rank + 1;
}

void RB_tree::insert_balance(Node *x)
{
    while ((x != root) && (x->parent->colour == Red)) { // check is node a root
        if (x->parent == x->parent->parent->left) { // have a violation
            Node *y = x->parent->parent->right;
            if (y->colour == Red) { // uncle is red
                x->parent->colour = Black;
                y->colour = Black;
                x->parent->parent->colour = Red;
                x = x->parent->parent;
            }
            else { // uncle is black
                if (x == x->parent->right) { // make x a left child
                    x = x->parent;
                    rotate_left(x);
                }
                x->parent->colour = Black; //change colour
                x->parent->parent->colour = Red;
                rotate_right(x->parent->parent);
            }
        }
        else { // mirror image of above code
            Node *y = x->parent->parent->left;
            if (y->colour == Red) { // uncle is red
                x->parent->colour = Black;
                y->colour = Black;
                x->parent->parent->colour = Red;
                x = x->parent->parent;
            }
            else { // uncle is black
                if (x == x->parent->left) {
                    x = x->parent;
                    rotate_right(x);
                }
                x->parent->colour = Black;
                x->parent->parent->colour = Red;
                rotate_left(x->parent->parent);
            }
        }
    }
    root->colour = Black;
}

Node *RB_tree::insert_node(int data)
{ //working with RB_tree for the first time
    Node *current = root, *parent, *x;
    parent = 0;
    while (current != son) {
        if (data == current->data)
        	return current;
        parent = current;

        //if ((current->left != son) || (current->right != son))
        	current->rank++;

        if (data < current->data)
        	current = current->left;
        else
        	current = current->right;
    }
    //initialize node
    x = new Node;
    if (!x)
    	return NULL;
    x->data = data;
    x->parent = parent;
    x->left = son;
    x->right = son;
    x->colour = Red;
    x->rank = 1;

    if(parent) { // insert node in tree
        if(data < parent->data)
            parent->left = x;
        else
            parent->right = x;
    }
    else
    	root = x;

    insert_balance(x);
    return x;
}

void RB_tree::delete_balance(Node *x)
{  //working with main version of RB_tree
	Node* y = x;
	while (y) {
		y->rank--;
		y = y->parent;
	}
    while ((x != root) && (x->colour == Black)) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->colour == Red) {
                w->colour = Black;
                x->parent->colour = Red;
                rotate_left(x->parent);
                w = x->parent->right;
            }
            if (w->left->colour == Black && w->right->colour == Black) {
                w->colour = Red;
                x = x->parent;
            }
            else {
                if (w->right->colour == Black) {
                    w->left->colour = Black;
                    w->colour = Red;
                    rotate_right (w);
                    w = x->parent->right;
                }
                w->colour = x->parent->colour;
                x->parent->colour = Black;
                w->right->colour = Black;
                rotate_left (x->parent);
                x = root;
            }
        }
        else {
            Node *w = x->parent->left;
            if (w->colour == Red) {
                w->colour = Black;
                x->parent->colour = Red;
                rotate_right (x->parent);
                w = x->parent->left;
            }
            if (w->right->colour == Black && w->left->colour == Black) {
                w->colour = Red;
                x = x->parent;
            }
            else {
                if (w->left->colour == Black) {
                    w->right->colour = Black;
                    w->colour = Red;
                    rotate_left (w);
                    w = x->parent->left;
                }
                w->colour = x->parent->colour;
                x->parent->colour = Black;
                w->left->colour = Black;
                rotate_right (x->parent);
                x = root;
            }
        }
    }
    x->colour = Black;
}

void RB_tree::delete_node(Node *z) {
    Node *x, *y;

    if ((z == NULL) || (z == son))
    	return;

    if ((z->left == son) || (z->right == son))
        y = z;
    else { // find tree successor with a NIL node as a child
    	y = z->right;
        while (y->left != son) {
        	y = y->left;
        }
    }

    if (y->left != son) { // x is y's only child
    	x = y->left;
    }
    else {
        x = y->right;
        x->rank--;
    }
    // remove y from the parent chain
    x->parent = y->parent;
    if (y->parent)
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    else
    	root = x;

    if (y != z) {
    	z->data = y->data;
    }

    if (y->colour == Black)
        delete_balance(x);

    delete y;
}

Node *RB_tree::is_node(int data)
{
    Node *current = root;
    while ((current != son) && (current != NULL)) {
        if (data == current->data)
            return current;
        else
        	if (data < current->data)
            		current = current->left;
        	else current = current->right;
    }
    return NULL;
}

bool RB_tree::cout_tree(Node *current)
{
	if ((current != son) && (current != NULL)) {
    	cout << current->data << ", rank = " << current->rank << ". ";
    	cout_tree(current->left);
    	cout_tree(current->right);
    	return true;
	}
    else
    	return false;
}

Node *RB_tree::os_select(Node *x, unsigned int i) // finding rang of node, using decrease of i
{ // i is a sum of node's rang
	unsigned int rang = x->left->rank + 1;
	if (i == rang)
		return x;
	else {
		if (i < rang)
			return os_select(x->left, i);
		else
			return os_select(x->right, (i - rang));
	}
}

int RB_tree::os_rank(Node *x)
{
	unsigned int rank = x->left->rank + 1;
	Node *y = x;
	while (y != root) {
		if (y == y->parent->right)
			rank = rank + y->parent->left->rank + 1;
		y = y->parent;
	}
	return rank;
}

bool RB_tree::delete_tree(Node *current)
{
	if ((current != son) && (current != NULL)) {
	    delete_tree(current->left);
	    delete_tree(current->right);
	    delete current;
	    current = NULL;

	    return true;
	}
	else
	    return false;
}

void RB_tree::RBT_menu()
{
	int check = 1;
	while (check != 0) {
		cout << endl << "Main version of tree is ready." << endl;
		cout_tree(root);
		cout << endl << "Welcome to \"order-statistic tree\" menu." << endl;
		cout << "1 - Add new node." << endl;
		cout << "2 - Check rank of node." << endl;
		cout << "3 - Find out a node by decreasing of rank." << endl;
		cout << "4 - Delete node." << endl;
		cout << "0 - exit." << endl;
		cin >> check;
		if (check == 1) {
			int data;
			cout << "Write data of number." << endl;
			cin >> data;
			if (insert_node(data) == NULL)
				cout << "Out of memory.";
			cout << data << " has been added." << endl;
		}
		if (check == 2) {
			int data;
			cout << "Write data of number to check rank." << endl;
			cin >> data;
			cout << "Rank is : " << os_rank(is_node(data)) << endl;
		}
		if (check == 3) {
			int rank;
			cout << "Write down a rank." << endl;
			cin >> rank;
			Node *check = os_select(root, rank);
			cout << "The key of node is " << check->data << endl;
		}
		if (check == 4) {
			int number;
			cout << "Write a data of node to delete." << endl;
			cin >> number;
			delete_node(is_node(number));
		}
		if (check == 0) {
			delete_tree(root);		
		}	
	}
}

int main() {
	RB_tree tree; // prints !!!Hello World!!!
	return 0;
}
