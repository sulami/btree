#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

struct node *
_new_node(KEYTYPE key, void *payload)
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
_parent(struct node *node, KEYTYPE key)
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
btree_insert(struct node *node, KEYTYPE key, void *payload)
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
btree_lookup(struct node *node, KEYTYPE target)
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
btree_remove(struct node *node, KEYTYPE key)
{
	if (node == NULL) { /* No node */
		return NULL;
	} else if (node->key == key) { /* Root node */
		struct node *new_node = _remove_node(node);

		return new_node;
	} else if (btree_lookup(node, key)) { /* Child/leaf node */
		struct node *parent = _parent(node, key);
		struct node *old = malloc(sizeof(struct node));
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

void
_btree_save(FILE *file, struct node *node)
{
	fwrite(&node->key, KEYSIZE, 1, file);
	fwrite(node->payload, PAYLOADSIZE, 1, file);

	if (node->left)
		_btree_save(file, node->left);
	if (node->right)
		_btree_save(file, node->right);
}

void
btree_save(struct node *node, const char *path)
{
	FILE *file = fopen(path, "w");

	_btree_save(file, node);

	fclose(file);
}

struct node *
btree_load(const char *path)
{
	long fsize;
	int i, num_nodes;
	char *buf;
	struct node *tree = NULL;
	FILE *file = fopen(path, "r");
	if (file == NULL)
		return NULL;

	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	rewind(file);

	buf = malloc(KEYSIZE + PAYLOADSIZE);
	if (buf == NULL) {
		fclose(file);
		return NULL;
	}

	num_nodes = fsize / (KEYSIZE + PAYLOADSIZE);

	for (i = 0; i < num_nodes; i++) {
		void *plbuf = malloc(PAYLOADSIZE);
		if (plbuf == NULL) {
			free(buf);
			fclose(file);
			return NULL;
		}

		fread(buf, KEYSIZE + PAYLOADSIZE, 1, file);

		memcpy(plbuf, buf + KEYSIZE, PAYLOADSIZE);

		tree = btree_insert(tree,
		                    (KEYTYPE)*buf + i * (KEYSIZE + PAYLOADSIZE),
		                    plbuf);
	}

	fclose(file);
	free(buf);

	return tree;
}

