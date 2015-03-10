#include <stdbool.h>
#include <stdlib.h>

#include "btree.h"

struct node *
_new_node(int key, void *payload)
{
	struct node* node = malloc(sizeof(struct node));

	if (node != NULL) {
		node->key = key;
		node->payload = payload;
		node->left = NULL;
		node->right = NULL;

		return node;
	} else {
		return NULL;
	}
}

struct node *
_parent(struct node *node, int key)
{
	while ((node->left->key != key) && (node->right->key != key)) {
		if (key <= node->key)
			node = node->left;
		else
			node = node->right;
	}

	return node;
}

int
_left_path(struct node *node)
{
	if (node->right != NULL)
		return (1 + _left_path(node->right));

	return 0;
}

int
_right_path(struct node *node)
{
	if (node->left != NULL)
		return (1 + _right_path(node->left));

	return 0;
}

struct node *
_remove_node(struct node *node)
{
	struct node *new_node = malloc(sizeof(struct node));
	if (new_node == NULL)
		/* TODO signal that we could not remove a node */
		return node;

	if ((node->left != NULL) && (node->right != NULL)) {
		int left_side = _left_path(node->left);
		int right_side = _right_path(node->right);

		if (left_side <= right_side) {
			struct node *leaf = node->left;

			new_node = node->left;

			while (leaf->right != NULL)
				leaf = leaf->right;

			leaf->right = node->right;
		} else {
			struct node *leaf = node->right;

			new_node = node->right;

			while (leaf->left != NULL)
				leaf = leaf->left;

			leaf->left = node->left;
		}
	} else if ((node->left != NULL) && (node->right == NULL)) {
		new_node = node->left;
	} else if ((node->left == NULL) && (node->right != NULL)) {
		new_node = node->right;
	}

	free(node);

	return new_node;
}

struct node *
btree_insert(struct node *node, int key, void *payload)
{
	if (node == NULL)
		return _new_node(key, payload);

	if (key <= node->key)
		node->left = btree_insert(node->left, key, payload);
	else
		node->right = btree_insert(node->right, key, payload);

	return node;
}

void *
btree_lookup(struct node *node, int target)
{
	if (node == NULL)
		return NULL;

	if (target == node->key)
		return node->payload;

	if (target < node->key)
		return btree_lookup(node->left, target);

	return btree_lookup(node->right, target);
}

struct node *
btree_remove(struct node *node, int key)
{
	if (node == NULL) { /* No node */
		return NULL;
	} else if (node->key == key) { /* Root node */
		struct node *new_node = _remove_node(node);

		return new_node;
	} else if (btree_lookup(node, key)) { /* Child/leaf node */
		struct node *parent = _parent(node, key);
		struct node *old = malloc(sizeof(node));
		if (old == NULL)
			return node;

		if (parent->left->key == key) {
			old = parent->left;
			parent->left = _remove_node(old);
		} else {
			old = parent->right;
			parent->right = _remove_node(old);
		}
	}

	return node;
}

int
btree_size(struct node *node)
{
	if (node == NULL)
		return 0;

	return (btree_size(node->left) + 1 + btree_size(node->right));
}

int
btree_depth(struct node *node)
{
	if (node == NULL)
		return 0;

	int depth_left = btree_depth(node->left);
	int depth_right = btree_depth(node->right);

		if (depth_left >= depth_right)
			return (depth_left + 1);
		else
			return (depth_right + 1);
}

struct node *
btree_min(struct node *node)
{
	if (node == NULL )
		return NULL;

	if (node->left)
		return btree_min(node->left);

	return node;
}

struct node *
btree_max(struct node *node)
{
	if (node == NULL )
		return NULL;

	if (node->right)
		return btree_max(node->right);

	return node;
}

