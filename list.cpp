#include <iostream>
#include <cstdlib>
#include <ctime>
#include "list.h"
using namespace std;


// debug flag
bool debug = false;


node::node (int input) {
	val = input;
	right = NULL;
	left = NULL;
}

linkedList::linkedList (int n) {
	if (debug) {
		cout << "constructing!\n";
	}
	head = NULL;
	tail = NULL;
	curr = NULL;
	size = 0;
	for (int i=0;i<n;i++) {
		push(rand() % 10);
	}
}

void linkedList::reset_head(node* ptr) {
	head = ptr;
	curr = head;
}

// add node to front of list, like a stack
void linkedList::push (int n) {
	if (debug) {
		cout << "Pushing: " << n << endl;
	}
	node* new_node = new node(n);
	if (head == NULL) {
		reset_head(new_node);
		tail = new_node;
	}
	else {
		head->left = new_node;
		new_node->right = head;
		reset_head(new_node);
	}
	size++;
}

// add node to back of list, like enqueue
void linkedList::append (int n) {
	if (debug) {
		cout << "Appending: " << n << endl;
	}
	node* new_node = new node(n);
	tail->right = new_node;
	new_node->left = tail;
	tail = new_node;
	if (head == NULL) {
		reset_head(new_node);
	}
	size++;
}

// remove first item, like a stack
int linkedList::pop () {
	int temp_int = head->val;
	node* temp_ptr = head;
	head = head->right;
	head->left = NULL;
	curr = head;
	delete temp_ptr;
	return temp_int;
	size--;
}

void linkedList::print_list () {
	curr = head;
	while (curr != NULL) {
		cout << curr->val <<  "  ";
		curr = curr->right;
	}
	cout << endl;
	curr = head;
}

void linkedList::right () {
	curr = curr->right;
}

void linkedList::left () {
	curr = curr->left;
}

linkedList::~linkedList () {
	if (debug) {
		cout << "destructing!\n";
	}
	int ctr = 1;
	curr = head;
	while (curr != NULL) {
		if (debug) {
			cout << "killing thing " << ctr << endl;
		}
		node* temp = curr;
		curr = curr->right;
		delete temp;
		ctr++;
	}
}


// adding leading 0 padding to smaller list
// INVARIANT: x must be the SMALLER list
void pad (linkedList &x, linkedList &y) {
	while (x.size < y.size) {
		x.push(0);
	}
}

/*
add elements of linked lists of equal size, where elements are single digits:

ex.
$ add_ll (1<->2<->3, 7<->8<->9)
=> 9<->1<->2

*/
linkedList add_ll (linkedList &x, linkedList &y) {
	// prep lists
	if (x.size < y.size) {
		pad(x, y);
	}
	else {
		pad(y, x);
	}

	// initialize vars
	linkedList res(0);
	x.curr = x.tail;
	y.curr = y.tail;
	int carry = 0;

	// main loop
	while (x.curr != NULL && y.curr != NULL) {
		res.push((x.curr->val + y.curr->val) % 10 + carry);
		carry = (x.curr->val + y.curr->val) / 10;
		x.left();
		y.left();
	}

	// final digit
	if (carry > 0) {
		res.push(carry);
	}
	return res;
}

// return nth to last element (treating it as singly linked list): O(n^2)
node* nth_to_last (linkedList &ll, int n) {
	if (n > ll.size) {
		return NULL;
	}
	while (ll.curr != NULL) {
		node* temp_curr = ll.curr;
		int ctr = 1;
		while (temp_curr->right != NULL) {
			temp_curr = temp_curr->right;
			ctr++;
		}
		if (ctr == n) {
			return ll.curr;
		}
		ll.right();
	}
	return NULL;
}

// delete node form linkedList, given only access to that node
// (without taking advantage of doubly linked list properties)
void del_node (node* curr) {
	curr->right->left = curr->left;
	node* temp = curr->right;
	*curr = *curr->right;
	delete temp;
}

// remove duplicates from a list without temp buffer: O(n^2)
void remdup (linkedList &ll) {
	node* temp_curr = NULL;
	while (ll.curr->right != NULL) {
		temp_curr = ll.curr->right;
		while (temp_curr != NULL) {
			if (ll.curr->val == temp_curr->val) {
				if (temp_curr->right == NULL) {
					temp_curr->left->right = NULL;
					node* temp = temp_curr;
					temp_curr = NULL;
					delete temp;
				}
				else {
					temp_curr->left->right = temp_curr->right;
					temp_curr->right->left = temp_curr->left;
					node* temp = temp_curr;
					temp_curr = temp_curr->right;
					delete temp;
				}
			}
			else {
				temp_curr = temp_curr->right;
			}
		}
		ll.right();
	}
}

int main() {
	// set changing seed based on current time
	srand(time(0));
	// declare a linked list with 10 random values
	linkedList l(10);
	l.print_list();
	l.pop();
	l.pop();
	l.print_list();
	l.push(0);
	l.append(0);
	l.print_list();
	// reverse list, using doubly linked assets
	node* curr = l.tail;
	while (curr != NULL) {
		cout << curr->val << "  ";
		curr = curr->left;
	}
	cout << endl;
}

