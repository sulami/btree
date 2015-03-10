#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "btree.h"
#include "btree_test.h"

void
test_first_node()
{
	struct node *node = btree_insert(NULL, 5, (void *)50000);

	assert(node != NULL);
	assert(node->key == 5);
	assert(node->payload == (void *)50000);
	assert(node->left == NULL);
	assert(node->right == NULL);
}

void
test_full_tree()
{
	BUILD_TREE

	assert(btree_lookup(tree, 1));
	assert(btree_lookup(tree, 2));
	assert(btree_lookup(tree, 3));
	assert(btree_lookup(tree, 4));
	assert(btree_lookup(tree, 5));
	assert(btree_lookup(tree, 6));
	assert(btree_lookup(tree, 7));
}

void
test_delete_leaf()
{
	BUILD_TREE

	tree = btree_remove(tree, 6); /* Leaf node */

	assert(btree_lookup(tree, 1));
	assert(btree_lookup(tree, 2));
	assert(btree_lookup(tree, 3));
	assert(btree_lookup(tree, 4));
	assert(btree_lookup(tree, 5));
	assert(btree_lookup(tree, 6) == false);
	assert(btree_lookup(tree, 7));
}

void
test_delete_mid()
{
	BUILD_TREE

	tree = btree_remove(tree, 3); /* Middle node, 2 children */

	assert(btree_lookup(tree, 1));
	assert(btree_lookup(tree, 2));
	assert(btree_lookup(tree, 3) == false);
	assert(btree_lookup(tree, 4));
	assert(btree_lookup(tree, 5));
	assert(btree_lookup(tree, 6));
	assert(btree_lookup(tree, 7));

	tree = btree_remove(tree, 1); /* Middle node, 1 child */

	assert(btree_lookup(tree, 1) == false);
	assert(btree_lookup(tree, 2));
	assert(btree_lookup(tree, 3) == false);
	assert(btree_lookup(tree, 4));
	assert(btree_lookup(tree, 5));
	assert(btree_lookup(tree, 6));
	assert(btree_lookup(tree, 7));
}

void
test_delete_root()
{
	BUILD_TREE

	tree = btree_remove(tree, 5); /* Root node, 2 children */

	assert(btree_lookup(tree, 1));
	assert(btree_lookup(tree, 2));
	assert(btree_lookup(tree, 3));
	assert(btree_lookup(tree, 4));
	assert(btree_lookup(tree, 5) == false);
	assert(btree_lookup(tree, 6));
	assert(btree_lookup(tree, 7));

	assert(tree->key == 3); /* The new root node */

	tree = btree_remove(tree, 3); /* Root node, 1 child */

	assert(btree_lookup(tree, 1));
	assert(btree_lookup(tree, 2));
	assert(btree_lookup(tree, 3) == false);
	assert(btree_lookup(tree, 4));
	assert(btree_lookup(tree, 5) == false);
	assert(btree_lookup(tree, 6));
	assert(btree_lookup(tree, 7));
}

void
test_tree_size()
{
	BUILD_TREE

	assert(btree_size(tree) == 7);
}

void
test_tree_depth()
{
	int test[8] = { 0, 10, 20, 30, 40, 50 ,60, 70 };

	assert(btree_depth(NULL) == 0);

	struct node *tree = btree_insert(NULL, 5, &test[5]);

	assert(btree_depth(tree) == 1);

	tree = btree_insert(tree, 3, &test[3]);

	assert(btree_depth(tree) == 2);

	tree = btree_insert(tree, 4, &test[4]);

	assert(btree_depth(tree) == 3);

	tree = btree_insert(tree, 7, &test[7]);
	tree = btree_insert(tree, 6, &test[6]);
	tree = btree_insert(tree, 1, &test[1]);
	tree = btree_insert(tree, 2, &test[2]);

	assert(btree_depth(tree) == 4);
}

void
test_min_max()
{
	assert(btree_min(NULL) == NULL);
	assert(btree_max(NULL) == NULL);

	BUILD_TREE

	assert(btree_min(tree)->key == 1);
	assert(btree_max(tree)->key == 7);
}

void
test_massive_load()
{
	/*
	 * This test is disabled by default, because it takes a few
	 * seconds to complete. It fills a tree with completely random
	 * data, asserts the tree size and prints out the tree depth.
	 * Because the depth can vary with random data, it is not
	 * checked here, but should be somewhere in the range of 60
	 * levels.
	 */
	int i;
	struct node *tree = btree_insert(NULL, 5000000, NULL);

	for (i = 0; i < 10000000; i++)
		tree = btree_insert(tree, rand(), NULL);

	assert(btree_size(tree) == 10000001);

	printf("Massive tree depth: %i levels\n", btree_depth(tree));
}

void
test_save_and_load()
{
	BUILD_TREE

	btree_save(tree, "/tmp/btree_test");

	struct node *tree_two = btree_load("/tmp/btree_test");
	assert(tree_two);
	assert(btree_size(tree_two) == btree_size(tree));
}

int
main()
{
	printf("\nRunning tests:\n");

	run_test(test_first_node);
	run_test(test_full_tree);
	run_test(test_delete_leaf);
	run_test(test_delete_mid);
	run_test(test_delete_root);
	run_test(test_tree_size);
	run_test(test_tree_depth);
	run_test(test_min_max);
	/* run_test(test_massive_load); */
	run_test(test_save_and_load);

	printf("\x1B[32mAll tests successful!\x1B[0m\n\n");

	return 0;
}

