#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

/*
 * These might be changed for different use cases.
 */
#define KEYTYPE int
#define KEYSIZE (sizeof(KEYTYPE))
#define PAYLOADSIZE (sizeof(int))

/*
 * Node struct. Contains a key for sorting/searching, a payload void
 * pointer and pointers to children nodes.
 */
struct node {
	KEYTYPE key;
	void *payload;
	struct node *left;
	struct node *right;
};

/* PUBLIC FUNCTIONS */

/*
 * Insert a new node. If the first argument, the root node, is NULL,
 * creates a new tree and returns that. If an existing tree is being
 * passed, the new root node could be discarded (but not encouraged).
 */
struct node *btree_insert(struct node *, KEYTYPE, void *);

/*
 * Lookup a given key in a given tree, returns the payload pointer, or
 * NULL if the element is not part of the tree. Also returns the first
 * found element in the event of duplicate keys.
 */
void *btree_lookup(struct node *, KEYTYPE);

/*
 * Remove a node from a tree, identified by the given key. Like
 * btree_lookup(), it uses the first found match. Returns the new root
 * node, which has to be used as the root could have changed.
 */
struct node *btree_remove(struct node *, KEYTYPE);

/*
 * Calculates the total count of elements in a given tree and returns
 * it.
 */
int btree_size(struct node *);

/*
 * Calculates the depth (levels) of a given tree and returns it.
 */
int btree_depth(struct node *);

/*
 * Finds the smallest key in the tree and returns the corrosponding
 * node.
 */
struct node *btree_min(struct node *);

/*
 * Finds the biggest key in the tree and returns the corrosponding
 * node.
 */
struct node *btree_max(struct node *);

/* PRIVATE FUNCTIONS */

/*
 * Allocate a new node and return it.
 */
struct node *_new_node(KEYTYPE, void *);

/*
 * Try to find the parent node for a given key, so we can use it later
 * on to attach something else when we have to replace the child.
 */
struct node *_parent(struct node *, KEYTYPE);

/*
 * Calculate how deep the left path is (always going left), to make a
 * restructured tree as compact as possible.
 */
int _left_path(struct node *);

/*
 * Calculate how deep the right path is (always going right), to make a
 * restructured tree as compact as possible.
 */
int _right_path(struct node *);

/*
 * Handle the internals of removing nodes, replacing nodes if needed
 * and restructuring the tree. Returns the new node in this position,
 * or NULL if it was a leaf.
 */
struct node *_remove_node(struct node *);

#endif

