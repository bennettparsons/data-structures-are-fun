# data-structures-are-fun
Compilation of data structure implementations I've been working on while coding in c++ for the first over the past two weeks.

list.cpp implements a doubly linked list, as well as some other functions that answer questions given in "Cracking the Coding Interview", by Gayle Laakmann. Can easily be used to represent a stack using the push(n) and pop() functions, or a queue using append(n) and pop(). 

btree.cpp implements a binary search tree with a strong balancing invariant to ensure the least possible depth of the tree: for every node, the number of left and right children differs at most by one. Initializes with any amount of randomly chosen values; includes standard insert(n), delete(), member() methods; print_tree() prints a nice representation of the tree, where underscores correspond to blank nodes.
