//============================================================================
// Name        : SI_2_Black_red.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

#define NMAX 10

typedef enum {Black, Red} node_colour;

struct Node {
	node_colour colour;
	int data;
	unsigned int num_of_version;
	Node *left;
	Node *right;
	Node *parent;
};

class RB_tree {

public:
	Node *son; // like NULL node
	vector<Node*> version_control;
	unsigned int version;
	~RB_tree();
	RB_tree();

	void rotate_left(Node *x, unsigned int num_of_version);
	void rotate_right(Node *x, unsigned int num_of_version);
	void insert_balance(Node *x, unsigned int num_of_version);
	Node *insert_node(int data, unsigned int num_of_version);
	void delete_balance(Node *s, unsigned int num_of_version);
	void delete_node(Node *m, unsigned int num_of_version);
	Node *is_node(int data, unsigned int num_of_version);
	bool cout_tree(Node *root, unsigned int num_of_version);
	Node *copy_node(Node *for_copy, unsigned int version);
	bool new_version(int data, unsigned int num_of_version);
	bool delete_version(Node *current, unsigned int version);
	void check_version();
	void RBT_menu();
};

RB_tree::~RB_tree()
{
	for (unsigned int i = 0; i <= 0; i++) {
		if (version_control[i] != NULL)
			delete_version(version_control[i], i);
	}
}

RB_tree::RB_tree()
{
	version = 0;
	son = new Node;
    son->left = NULL;
    son->right = NULL;
    son->data = 0;
    son->parent = NULL;
    son->colour = Black;
    son->num_of_version = 0;

    version_control.push_back(son);

    for (int i = 1; i <= NMAX; ++i)
        if (new_version(i, 0) == false) {
        	cout << "Out of memory" << endl;
        	break;
        }

    RBT_menu();
}

void RB_tree::rotate_left(Node *x, unsigned int num_of_version)
{
    Node *y = x->right;

    x->right = y->left; //establish x->right link
    if (y->left != son)
    	y->left->parent = x;
    if (y != son) //establish y->parent link
    	y->parent = x->parent;
    if (x->parent == NULL)
    	version_control[num_of_version] = y;
    else if (x == x->parent->left)
    	x->parent->left = y;
    else
    	x->parent->right = y;

    y->left = x;// link x and y

    if (x != son)
    	x->parent = y;

}

void RB_tree::rotate_right(Node *x, unsigned int num_of_version)
{
	Node *y = x->left;

	    x->left = y->right; // establish x->left link
	    if (y->right != son)
	    	y->right->parent = x;
	    if (y != son) // establish y->parent link
	    	y->parent = x->parent;
	    if (x->parent == NULL)
	    	version_control[num_of_version] = y;
	    else if (x == x->parent->right)
	    	x->parent->left = y;
	    else
	    	x->parent->right = y;

	    y->right = x; // link x and y

	    if (x != son)
	    	x->parent = y;
}

void RB_tree::insert_balance(Node *x, unsigned int num_of_version)
{
    while ((x != version_control[num_of_version]) && (x->parent->colour == Red)) { // 1.check is node a root
        if (x->parent == x->parent->parent->left) { // have a violation
            Node *y = x->parent->parent->right;
            if (y->colour == Red) { // 3.uncle is red (also parent too)
                x->parent->colour = Black;
                y->colour = Black;
                x->parent->parent->colour = Red;
                x = x->parent->parent;
            }
            else { // 5.uncle is black
                if (x == x->parent->right) { // check 4., make x a left child
                    x = x->parent;
                    rotate_left(x, num_of_version);
                }
                x->parent->colour = Black; //change colour
                x->parent->parent->colour = Red;
                rotate_right(x->parent->parent, num_of_version);
            }
        }
        else { // on the right
            Node *y = x->parent->parent->left;
            if (y->colour == Red) { // 3.uncle is red (also parent too)
                x->parent->colour = Black;
                y->colour = Black;
                x->parent->parent->colour = Red;
                x = x->parent->parent;
            }
            else { // uncle is black
                if (x == x->parent->left) {
                    x = x->parent;
                    rotate_right(x, num_of_version);
                }
                x->parent->colour = Black;
                x->parent->parent->colour = Red;
                rotate_left(x->parent->parent, num_of_version);
            }
        }
    }
    version_control[num_of_version]->colour = Black;// 2. change colour to save properties of rb_tree
}

void RB_tree::delete_balance(Node *x, unsigned int num_of_version)
{
    while ((x != version_control[num_of_version]) && (x->colour == Black)) {
        if (x == x->parent->left) {
        	Node *w;
        	if ((x->parent->right->num_of_version == num_of_version) || (x->parent->right == son))
            	w = x->parent->right;
        	else {
        		w = copy_node(x->parent->right, num_of_version);
        		x->parent->right = w;
        		w->parent = x->parent;
        	}
            if (w->colour == Red) {
                w->colour = Black;
                x->parent->colour = Red;
                rotate_left(x->parent, num_of_version);
                w = x->parent->right;
            }
            if (w->left->colour == Black && w->right->colour == Black) {
                w->colour = Red;
                x = x->parent;
            }
            else {
                if (w->right->colour == Black) {
                    //w->left->colour = Black;
                    if ((w->left->num_of_version == num_of_version) || (w->left == son))
                    	w->left->colour = Black;
                    else {
                        w->left = copy_node(w->left, num_of_version);
                        w->left->parent = w;
                        w->left->colour = Black;
                    }
                    w->colour = Red;
                    rotate_right (w, num_of_version);
                    w = x->parent->right;
                }
                w->colour = x->parent->colour;
                x->parent->colour = Black;
                if ((w->right->num_of_version == num_of_version) || (w->right == son))
                	w->right->colour = Black;
                else {
                    w->right = copy_node(w->right, num_of_version);
                    w->right->parent = w;
                    w->right->colour = Black;
                }
                //w->right->colour = Black;
                rotate_left (x->parent, num_of_version);
                x = version_control[num_of_version];
            }
        }
        else {
            Node *w;
            if ((x->parent->left->num_of_version == num_of_version) || (x->parent->left == son))
            	w = x->parent->left;
            else {
                w = copy_node(x->parent->left, num_of_version);
                x->parent->left = w;
            }

            if (w->colour == Red) {
                w->colour = Black;
                x->parent->colour = Red;
                rotate_right (x->parent, num_of_version);
                w = x->parent->left;
            }
            if (w->right->colour == Black && w->left->colour == Black) {
                w->colour = Red;
                x = x->parent;
            }
            else {
                if (w->left->colour == Black) {
                	if ((w->right->num_of_version == num_of_version) || (w->right == son))
                		w->right->colour = Black;
                	else {
                		w->right = copy_node(w->left, num_of_version);
                	    w->right->parent = w;
                	    w->right->colour = Black;
                	}
                    w->colour = Red;
                    rotate_left (w, num_of_version);
                    w = x->parent->left;
                }
                w->colour = x->parent->colour;
                x->parent->colour = Black;
                if ((w->left->num_of_version == num_of_version) || (w->left == son))
                	w->left->colour = Black;
                else {
                    w->left = copy_node(w->left, num_of_version);
                    w->left->parent = w;
                    w->left->colour = Black;
                }
                rotate_right (x->parent, num_of_version);
                x = version_control[num_of_version];
            }
        }
    }
    x->colour = Black;
}

void RB_tree::delete_node(Node *m, unsigned int num_of_version)
{
    Node *x, *c, *current = version_control[num_of_version], *parent;
    Node *copy_current;
    do {
    	if (current == m)
    		break;
    	if (current->num_of_version != num_of_version) // copying nodes
    	    parent = copy_current;
    	else
    	    parent = current;

    	if (m->data < current->data) {
    	    current = current->left;
    	    if ((current->num_of_version != num_of_version) && (current != son)) {
    	    	copy_current = copy_node(current, num_of_version);
    	    	if (copy_current == NULL)
    	    		return;
    	    	copy_current->parent = parent;
    	    	copy_current->parent->left = copy_current;
    	    }
    	}
    	else {
    	    current = current->right;
    	    if ((current->num_of_version != num_of_version) && (current != son)) {
    	    	copy_current = copy_node(current, num_of_version);
    	    	if (copy_current == NULL)
    	    		return;
    	    	copy_current->parent = parent;
    	    	copy_current->parent->right = copy_current;
    	    }
    	}
    } while (current != m);

    m = copy_current;

    if ((m == NULL) || (m == son))
    	return;

    if ((m->left == son) || (m->right == son))
        c = m;
    else {
        // find tree successor with a NIL node as a child
        if ((m->right->num_of_version == num_of_version) || (m->right == son))
        	c = m->right;
        else {
        	c = copy_node(m->right, num_of_version);
        	c->parent = m;
        	m->right = c;
        }
        while (c->left != son) {
        	if ((c->left->num_of_version == num_of_version) || (c->left == son))
        		c = c->left;
        	else {
        		parent = c;
        		c = copy_node(c->left, num_of_version);
        		c->parent = parent;
        		parent->right = c;
        	}
        }
    }

    // x is "c" only child
    if (c->left != son) {
        if ((c->left->num_of_version == num_of_version) || (c->left == son))
        	x = c->left;
        else {
        	x = copy_node(c->left, num_of_version);
        	x->parent = c;
        	c->left = x;
        }
    }
    else { // right son
        if ((c->right->num_of_version == num_of_version) || (c->right == son))
        	x = c->right;
        else {
            x = copy_node(c->right, num_of_version);
            x->parent = c;
            c->right = x;
        }
    }
    // remove "c" from the parent chain
    x->parent = c->parent;
    if (c->parent)
        if (c == c->parent->left)
            c->parent->left = x;
        else
            c->parent->right = x;
    else
    	version_control[num_of_version] = x;

    if (c != m) {
    	m->data = c->data;
    }

    if (c->colour == Black)
        delete_balance(x, num_of_version);

    delete c;
    c = son;
}

Node *RB_tree::is_node(int data, unsigned int num_of_version)
{
    Node *current = version_control[num_of_version];
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

bool RB_tree::cout_tree(Node *current, unsigned int num_of_version)
{
	if ((current != son) && (current != NULL) && (current->num_of_version == num_of_version)) {
    	cout << current->data << ". " ;
    	cout_tree(current->left, num_of_version);
    	cout_tree(current->right, num_of_version);
    	return true;
	}
    else
    	return false;
}

Node* RB_tree::copy_node(Node *for_copy, unsigned int nversion)
{
	Node *data_copy = new Node;
	if (data_copy == NULL)
		return NULL;
	data_copy->colour = for_copy->colour;
	data_copy->data = for_copy->data;
	data_copy->left = for_copy->left;
	data_copy->parent = for_copy->parent;
	data_copy->right = for_copy->right;
	data_copy->num_of_version = nversion;
	return data_copy;
}

bool RB_tree::new_version(int data, unsigned int num_of_version)
{
    Node *copy_parent = NULL, *current, *x, *copy_current;
    if (num_of_version != 999) {
    	current = version_control[num_of_version];//changing version
        if (current == NULL)
        	return false;
    }
    else {
    	//create a new version
    	version_control.push_back(copy_node(version_control[version], (version + 1)));
    	current = version_control[version];
    	version++;
    	num_of_version = version;
    	copy_current = version_control[num_of_version];
    }

    while ((current != son) && (current != NULL)) { // copying nodes
    	if (current->num_of_version != num_of_version)
    		copy_parent = copy_current;
    	else
    		copy_parent = current;

    	if (data < current->data) {
    		current = current->left;
    		if ((current != son) && (current != NULL) && (current->num_of_version != num_of_version)) {
    			copy_current = copy_node(current, num_of_version);
    			if (copy_current == NULL)
    				return false;
    			copy_current->parent = copy_parent;
    			copy_current->parent->left = copy_current;
    		}
    		else if (copy_parent->num_of_version != num_of_version) { //copy a node, which take part in rotation
    			copy_current->parent->right = copy_node(copy_current->parent->right, num_of_version);
    			copy_current->parent->right->parent = copy_current->parent;
    		}
    	}
    	else {
    		current = current->right;
    		if ((current != son) && (current != NULL) && (current->num_of_version != num_of_version)) {
    			copy_current = copy_node(current, num_of_version);
    			copy_current->parent = copy_parent;
    			copy_current->parent->right = copy_current;
    		}
    		else if (copy_parent->num_of_version != num_of_version) {
    			copy_current->parent->left = copy_node(copy_current->parent->left, num_of_version);
    			copy_current->parent->left->parent = copy_current->parent;
    		}
    	}
    }

    //Node *copy_parent = new Node;
    //initialize node
    x = new Node;
    if (!x)
    	return false;

    x->data = data;
    x->parent = copy_parent;
    x->left = son;
    x->right = son;
    x->colour = Red;
    x->num_of_version = num_of_version;

    // insert node in tree
    if(copy_parent) {
        if(data < copy_parent->data)
        	copy_parent->left = x;
        else
        	copy_parent->right = x;
    }
    else
    	version_control[num_of_version] = x;

    insert_balance(x, num_of_version);
    return true;
}

bool RB_tree::delete_version(Node *current, unsigned int version)
{
	if ((current != son) && (current != NULL) && (current->num_of_version == version)) {
	    delete_version(current->left, version);
	    delete_version(current->right, version);
	    delete current;
	    current = NULL;

	    return true;
	}
	else
	    return false;
}

void RB_tree::check_version()
{
	cout << "Here is a versions of this tree : " << endl;
	for (int i = 0; i < (int)version_control.size(); i++)
		if (version_control[i] != NULL)
			cout << i << " ";
	cout << endl;
}

void RB_tree::RBT_menu()
{
	int check = 1;
	cout << endl << "Main version of tree is ready." << endl;
	cout_tree(version_control[0], 0);
	while (check != 0) {
		cout << endl << "Welcome to version control." << endl;
		cout << "1 - Create new version." << endl;
		cout << "2 - Check version." << endl;
		cout << "3 - Delete version." << endl;
		cout << "0 - exit." << endl;
		cin >> check;
		if (check == 1) {
			int data, num_of_version = 999;

			check_version();
			cout << "Write data of number." << endl;
			cin >> data;

			if (new_version(data, num_of_version) == false)
				cout << "Out of memory.";
			cout << version << " version has been created." << endl;
		}
		if (check == 2) {
			int number, what_to_do;

			check_version();

			cout << "Write a number of version." << endl;
			cin >> number;

			cout << "1.Cout version. " << endl;
			cout << "2.Add new node. " << endl;
			cout << "3.Delete node. " << endl;
			cin >> what_to_do;

			if (what_to_do == 1)
				if (cout_tree(version_control[number], number) == false)
					cout << "This version is NULL." << endl;
			if (what_to_do == 2) {
				int data;
				cout << "Write data." << endl;
				cin >> data;
				if (new_version(data, number) == false)
					cout << "Out of memory.";
			}
			if (what_to_do == 3) {
				int data;
				cout << "Write data." << endl;
				cin >> data;
				delete_node(is_node(data, number), number);
			}
		}
		if (check == 3) {
			int number;

			check_version();
			cout << "Write a number of version to delete." << endl;
			cin >> number;
			if (delete_version(version_control[number], number) == false)
				cout << "This version is NULL." << endl;
			else
				version_control[number] = NULL;
		}
		if (check == 0) {
			for (unsigned int i = 0; i <= 0; i++)
				if (version_control[i] != NULL)
					delete_version(version_control[i], i);
			delete son;		
		}	
	}
}

int main() {
	RB_tree tree;
	return 0;
}
