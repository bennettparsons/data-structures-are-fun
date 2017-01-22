#ifndef BTREE_H
#define BTREE_H

class node {
public:
	int val;
	node* right;
	node* left;
	int n_right;
	int n_left;

	node (int);
	~node();
};

class tree {
private:
	node* root;
	unsigned int size;  // that is, the number of characters to be printed per line

	// random number generator used
	int random (int);

	// insert helpers
	void insert_helper (int, node*&);
	int new_root_right (node*);
	int new_root_left (node*);
	void fix (node*&);
	void del_node (node*&);

	// remove helper
	void remove_helper (int, node*);

	// searchers
	bool s_search (int, node*); // standard smart search for binary search trees
	bool d_search(int, node*); // standard depth first-search for binary trees
	bool b_search(int, node*); // standard breadth-first search for binary trees

	// printer helpers
	int get_depth (node*);
	void print_n_spaces(int);
	void print_underscore();
	void print_level(int, int, std::queue<node*>&);
	void print_next(std::queue<node*>&);
	std::string format_num(int);

public:
	tree(int=0, int=1);
	void insert(int);
	void pop();
	void remove(int);
	bool member(int);
	node* get_root();
	void print_tree();
	~tree();
};

#endif // BTREE_H