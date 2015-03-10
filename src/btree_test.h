#ifndef BTREE_TREST_H_INCLUDED
#define BTREE_TREST_H_INCLUDED

/*
 * Macro to run tests.
 */
#define run_test(fn_name) \
	printf("%s\n", #fn_name); \
	fn_name();


/*
 * Macro to build a standard testing tree.
 */
#define BUILD_TREE \
int test[8] = { 0, 10, 20, 30, 40, 50 ,60, 70 }; \
struct node *tree = btree_insert(NULL, 5, &test[5]); \
 \
tree = btree_insert(tree, 3, &test[3]); \
tree = btree_insert(tree, 4, &test[4]); \
tree = btree_insert(tree, 7, &test[7]); \
tree = btree_insert(tree, 6, &test[6]); \
tree = btree_insert(tree, 1, &test[1]); \
tree = btree_insert(tree, 2, &test[2]); \
 \
void test_first_node(); \
void test_full_tree(); \
void test_delete_leaf(); \
void test_delete_mid(); \
void test_delete_root(); \
void test_tree_size(); \
void test_tree_depth(); \
void test_massive_load();

/*
 * Our complete test tree looks like this:
 *
 *         5
 *        / \
 *       /   \
 *      3     7
 *     / \   /
 *    1   4 6
 *     \
 *      2
 */

#endif

