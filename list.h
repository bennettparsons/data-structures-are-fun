#ifndef LIST_H
#define LIST_H

class node {
public:
	int val;
	node* right;
	node* left;

	node (int);
};

class linkedList {
public:
	node* head;
	node* tail;
	node* curr;
	int size;

	linkedList(int=0);
	void reset_head (node*);
	// void make_list (int);
	void push(int);
	void append(int);
	int pop();
	void print_list();
	void right();
	void left();
	~linkedList();
};

#endif // LIST_H