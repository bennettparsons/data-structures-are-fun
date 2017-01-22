#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>
#include <iomanip>
#include <sstream>
#include <ios>
#include "btree.h"
using namespace std;


/*
*****balanced binary search tree*****
INVARIANT: every node carries with it the number of right and left children; 
	the absolute value of the difference between these values is at most 1
*/


// debug flag
bool debug = false;
// verbose debug flag
bool debug_verbose = false;

// node constructor
node::node (int input) {
	val = input;
	right = NULL;
	left = NULL;
	n_right = 0;
	n_left = 0;
}

// node destructor
node::~node () {
	delete right;
	delete left;
}

/*
tree constructor: first argument indicates number of members to initialize
	with (default 0); second argument indicates digit length (default 0)
NOTE: n !> 10^digit_length, because the tree does not accept duplicate entries
*/
tree::tree (int n, int digit_length) {
	root = NULL;
	if (debug) {
		size = digit_length+4;
	}
	else {
		size = digit_length;
	}
	if (n > pow(10, digit_length)) {
		cout << "Can't produce that many unique values with numbers of length "
			 << digit_length << endl;
		n = 0;
	}
	for (int i=0;i<n;i++) {
		int r;
		do {
			r = random(digit_length);
		} while (member(r));
		insert(r);
	}
}

// returns a random integer of length digit length
int tree::random (int digit_length) {
	return fmod(rand(), pow(10, digit_length));
}

// inserts a member into the tree only if it does not already exist, using a
// recursive helper function
void tree::insert (int n) {
	if (member(n)) {
		cout << "Can't insert a pre-existing member (" << n << ")!" << endl;
	}
	else {
		if (debug) {
			cout << "Inserting: " << n << endl;
		}
		insert_helper(n, root);
		if (debug) {
			print_tree();
		}
		fix(root);
	}
}

// recursive helper inserts a new node with val = n into the tree
void tree::insert_helper (int n, node* &curr) {
	if (curr == NULL) {
		node* new_node = new node(n);
		curr = new_node;
	}
	else {
		if (n < curr->val) {
			curr->n_left++;
			insert_helper(n, curr->left);
		}
		else if (n > curr->val) {
			curr->n_right++;
			insert_helper(n, curr->right);
		}
	}
}

// fixes the tree so that every node satisfies the balancing invariant
void tree::fix (node* &curr) {
	if (curr == NULL) {
		return;
	}
	else {
		int balance = curr->n_right - curr->n_left;
		if (balance > 1) {
			// get value for new root of the tree
			int new_root = new_root_right(curr);
			// save current root in new location (initialized with dummy value)
			node* old_root = new node(0);
			*old_root = *curr;
			// copy in new root value, and link it to old_root
			curr->val = new_root;
			curr->left = old_root;
			curr->n_right = old_root->n_right;
			curr->n_left = old_root->n_left + 1;
			// fix up old root
			old_root->right = NULL;
			old_root->n_right = 0;
			// clean up
			if (debug_verbose) {
				print_tree();
			}
			fix (curr);
		}
		else if (balance < -1) {
			// get value for new root of the tree
			int new_root = new_root_left(curr);
			// save current root in new location (initialized with dummy value)
			node* old_root = new node(0);
			*old_root = *curr;
			// copy in new root value, and link it to old_root
			curr->val = new_root;
			curr->right = old_root;
			curr->n_left = old_root->n_left;
			curr->n_right = old_root->n_right + 1;
			// fix up old root
			old_root->left = NULL;
			old_root->n_left = 0;
			// clean up
			if (debug_verbose) {
				print_tree();
			}
			fix (curr);
		}
		else {
			fix (curr->left);
			fix (curr->right);
		}
	}
}

// call if getting a new root from the right subtree
int tree::new_root_right (node* curr) {
	curr->n_right--;
	if (curr->right->left == NULL && curr->right->right == NULL) {
		int res = curr->right->val;
		delete curr->right;
		curr->right = NULL;
		return res;
	}
	curr = curr->right;
	while (curr->left != NULL) {
		if (curr->left->left == NULL && curr->left->right == NULL) {
			curr->n_left--;
			int res = curr->left->val;
			delete curr->left;
			curr->left = NULL;
			return res;
		}
		curr->n_left--;
		curr = curr->left;
	}
	int res = curr->val;
	// clean up location *** since we got here, we know curr->right != NULL
	node* temp = curr->right;
	*curr = *curr->right;
	del_node (temp);
	return res;
}

// call if getting a new root from the left subtree
int tree::new_root_left (node* curr) {
	curr->n_left--;
	if (curr->left->left == NULL && curr->left->right == NULL) {
		int res = curr->left->val;
		delete curr->left;
		curr->left = NULL;
		return res;
	}
	curr = curr->left;
	while (curr->right != NULL ) {
		if (curr->right->right == NULL && curr->right->left == NULL) {
			curr->n_right--;
			int res = curr->right->val;
			delete curr->right;
			curr->right = NULL;
			return res;
		}
		curr->n_right--;
		curr = curr->right;
	}
	int res = curr->val;
	// clean up location *** since we got here, we know curr->left != NULL
	node* temp = curr->left;
	*curr = *curr->left;
	del_node (temp);
	return res;
}

// deletes only the node given
void tree::del_node (node* &curr) {
	curr->right = NULL;
	curr->left = NULL;
	delete curr;
}

// returns depth of lowest path in tree
int tree::get_depth (node* curr) {
	if (curr == NULL) {
		return 0;
	}
	else {
		int lheight = 1 + get_depth(curr->left);
		int rheight = 1 + get_depth(curr->right);
		if (rheight > lheight) {
			return rheight;
		}
		else {
			return lheight;
		}
	}
}

node* tree::get_root () {
	return root;
}

// print a representation of the tree using the print level helper
void tree::print_tree () {
	cout << endl;
	int d = get_depth(root);
	queue<node*> q;
	q.push(root);
	for (int l=0;l<d;l++) {
		print_level(l+1, d, q);
		cout << endl;
	}
	cout << endl;
}

// print level l of the tree: uses a queue to link all sibling nodes
void tree::print_level (int l, int d, queue<node*> &q) {
	int fst = pow(2, (d-l)) - 1;    // # of spaces to print before first member
	int snd = pow(2, (d-l+1)) - 1;  // # of spaces to print between each member
	int n = pow(2, (l-1));          // # of things to print this level
	print_n_spaces(fst);
	for (int i=0;i<n;i++) {
		print_next(q);
		print_n_spaces(snd);
	}

}

// print the next element of the tree, as given by the front of the queue
void tree::print_next (queue<node*> &q) {
	if (q.front() == NULL) {
		print_underscore();
		q.push(NULL);
		q.push(NULL);
		q.pop();
	}
	else {
		// print value and balance of node if in debug, otherwise just val
		if (debug) {
			cout << format_num(q.front()->val) 
				 << ":" 
				 << q.front()->n_left 
				 << ":" 
				 << q.front()->n_right;
		}
		else {
			cout << format_num(q.front()->val);
		}
		
		// push left child to queue
		if (q.front()->left == NULL) {
			q.push(NULL);
		}
		else {
			q.push(q.front()->left);
		}
		// push right child to queue
		if (q.front()->right == NULL) {
			q.push(NULL);
		}
		else {
			q.push(q.front()->right);
		}
		q.pop();
	}
}

// prints n spaces of length 'size'
void tree::print_n_spaces (int n) {
	for (int j=0;j<size;j++) {
		for (int i=0;i<n;i++) {
			cout << " ";
		}
	}
}

// prints underscores of length 'size'
void tree::print_underscore () {
	for (int i=0;i<size;i++) {
		cout << "_";
	}
}

// pads integer n with leading 0s so it will have 'size' digits
std::string tree::format_num (int n) {
	ostringstream out;
	if (debug) {
		out << std::internal << std::setfill('0') << std::setw(size-4) << n;
	}
	else {
		out << std::internal << std::setfill('0') << std::setw(size) << n;
	}
	return out.str();
}

// returns boolean indicating whether n is in the tree
bool tree::member(int n) {
	return s_search (n, root);
	// return d_search (n, root);
	// return b_search (n, root);
}

// smart search implementation for binary search trees
bool tree::s_search (int n, node* curr) {
	if (curr == NULL) {
		return false;
	}
	else if (curr->val == n) {
		return true;
	}
	else if (curr->val > n) {
		return s_search (n, curr->left);
	}
	else {
		return s_search (n, curr->right);
	}
}

// depth-first search implementation
bool tree::d_search (int n, node* curr) {
	if (curr == NULL) {
		return false;
	}
	if (curr->val == n) {
		return true;
	}
	else if (curr->left != NULL && curr->right != NULL) {
		return d_search(n, curr->left) || d_search(n, curr->right);
	}
	else if (curr->right != NULL) {
		return d_search(n, curr->right);
	}
	else {
		return d_search(n, curr->left);
	}
}

// breadth-first search implementation (used bread-first search in print_level)
bool tree::b_search (int n, node* curr) {
	cout << "Not yet implemented!\n";
	return false;
}

// removes the value n from the tree, if it exists, by calling remove_helper
void tree::remove (int n) {
	if (!member(n)) {
		cout << "Could not find " << n << " in the tree!\n";
		return;
	}
	if (debug) {
		cout << "Removing: " << n << endl;
	}
	remove_helper (n, root);
	fix (root);
	if (debug) {
		print_tree();
	}
}

// recursive helper for removing n from the subtree with root curr
void tree::remove_helper (int n, node* curr) {
	if (n < curr->val) {
		curr->n_left--;
		if (curr->left->val == n) {
			if (curr->left->n_left > curr->left->n_right) {
				int new_root = new_root_left (curr->left);
				curr->left->val = new_root;
			}
			else if (curr->left->n_left < curr->left->n_right) {
				int new_root = new_root_right (curr->left);
				curr->left->val = new_root;
			}
			else if (curr->left->left != NULL) {
				int new_root = new_root_left (curr->left);
				curr->left->val = new_root;
			}
			else if (curr->left->right != NULL) {
				int new_root = new_root_right (curr->left);
				curr->left->val = new_root;
			}
			else {
				delete curr->left;
				curr->left = NULL;
			}
		}
		else {
			remove_helper (n, curr->left);
		}
	}
	else if (n > curr->val) {
		curr->n_right--;
		if (curr->right->val == n) {
			if (curr->right->n_left > curr->right->n_right) {
				int new_root = new_root_left (curr->right);
				curr->right->val = new_root;
			}
			else if (curr->right->n_left < curr->right->n_right) {
				int new_root = new_root_right (curr->right);
				curr->right->val = new_root;
			}
			else if (curr->right->left != NULL) {
				int new_root = new_root_left (curr->right);
				curr->right->val = new_root;
			}
			else if (curr->right->right != NULL) {
				int new_root = new_root_right (curr->right);
				curr->right->val = new_root;
			}
			else {
				delete curr->right;
				curr->right = NULL;
			}
		}
		else {
			remove_helper (n, curr->right);
		}
	}
	// curr->val == n, only if curr is initial root of tree
	else {
		if (curr->n_left > curr->n_right) {
			int new_root = new_root_left (curr);
			curr->val = new_root;
		}
		else if (curr->n_left < curr->n_right) {
			int new_root = new_root_right (curr);
			curr->val = new_root;
		}
		else if (curr->left != NULL) {
			int new_root = new_root_left (curr);
			curr->val = new_root;
		}
		else if (curr->right != NULL) {
			int new_root = new_root_right (curr);
			curr->val = new_root;
		}
		else {
			delete curr;
			root = NULL;
		}
	}
}

// tree destructor
tree::~tree () {
	delete root;
}


int main () {
	// prints a tree of 63 random 2 digit numbers
	srand(time(0));
	tree t(63, 2);
	t.print_tree();

	cout << endl;
}
