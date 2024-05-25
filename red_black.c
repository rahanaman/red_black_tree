#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct node{
	int key;
	struct rbnode* left;
	struct rbnode* right;
	int color;
	struct rbnode* parent;
}rbnode;

typedef struct traverse{
	rbnode* key;
	struct traverseNode* next;
}traverseNode;

// Insert into tree via integer
void insertInt(rbnode** root, int x);
void insert(rbnode **root, rbnode* x );
void insertNode(rbnode** root, rbnode* parent, rbnode* x);
void insertFixup(rbnode** root, rbnode* x);

//deletion
void deleteInt(rbnode** root, int x);
void delete(rbnode** root, rbnode* x);
rbnode* deleteNode(rbnode** root, rbnode* x);
void deleteFixup(rbnode** root, rbnode* x);

//traverse
void printInorder(rbnode* root);
void printLevelorder(rbnode* root);


void printNode(rbnode* node);


void traverse(traverseNode* list);
void addtraverse(traverseNode* list, rbnode* node);


rbnode* search(rbnode** root, int x);
rbnode* searchLeftmost(rbnode** root);

rbnode** createTree();

rbnode* newNode(int x);


int main(){
	rbnode** root = createTree();
	insertInt(root, 5);
	insertInt(root, 1);
	insertInt(root, 2);
	insertInt(root, 3);
	insertInt(root, 4);
	insertInt(root, 7);
	insertInt(root, 6);
	insertInt(root, 8);
	deleteInt(root, 7);
	deleteInt(root, 5);
	
	printInorder(*root);
	printf("\n");
	printLevelorder(*root);
	return 0; 
}

rbnode** createTree() {
	
	rbnode** root= (rbnode**)malloc(sizeof(rbnode*));;
	*root = NULL;
	return root;

}

rbnode* newNode(int x) {
	rbnode* node = (rbnode*)malloc(sizeof(rbnode));
	if (node == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}
	node->key = x;
	node->color = RED;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	
	return node;
}

void insertInt(rbnode** root, int x) {
	return insert(root, newNode(x));
}


void insert(rbnode** root, rbnode* x) {
	if (*root == NULL) {
		*root = x;
		(*root)->color = BLACK;
		return;
	}
	if (x->key < (*root)->key) {
		insertNode(&((*root)->left),*root, x);
	}
	else if (x->key > (*root)->key) {
		insertNode(&((*root)->right),*root, x);
	}
	else {
		printf("ERROR: already in tree.");
		free(x);
	}
	insertFixup(root, x);
}

void insertNode(rbnode** root, rbnode* parent, rbnode* x) {
	if (*root == NULL) {
		x->parent = parent;
		*root = x;
		return;
	}
	if (x->key < (*root)->key) {
		insertNode(&((*root)->left), *root,x);
	}
	else if (x->key > (*root)->key) {
		insertNode(&((*root)->right), *root, x);
	}
	else {
		printf("ERROR: already in tree.");
		free(x);
	}
	return;
}

void insertFixup(rbnode** root, rbnode* x) {
	
}

void deleteInt(rbnode** root, int x) {
	rbnode* node = search(root, x);
	delete(root, node);
}

void delete(rbnode** root, rbnode* x) {
	rbnode* target = deleteNode(root, x);
	deleteFixup(root, target);
}

rbnode* deleteNode(rbnode** root, rbnode* x) {
	if (*root == NULL) {
		return NULL;
	}
	if (x->key < (*root)->key) {
		deleteNode(&((*root)->left), x);
	}
	else if (x->key > (*root)->key) {
		deleteNode(&((*root)->right), x);
	}
	else {
		if ((*root)->left == NULL && (*root)->right == NULL) {
			rbnode* d = *root;
			*root = NULL;
			free(d);
			return NULL;
		}
		else if ((*root)->left == NULL || (*root)->right == NULL) {
			rbnode* temp;
			rbnode* d = *root;
			if ((*root)->left == NULL) {
				temp = (*root)->right;
				temp->parent = (*root)->parent;
				*root = temp;
			}
			else {
				temp = (*root)->left;
				temp->parent = (*root)->parent;
				(*root) = (*root)->left;
			}
			free(d);
			return temp;
		}
		else {
			rbnode* temp = searchLeftmost(&((*root)->right));
			(*root)->key = temp->key;
			(*root)->color = temp->color;
			deleteNode(&((*root)->right), temp);
			return (*root);
		}
	}
}

void deleteFixup(rbnode** root, rbnode* x) {
	if (x == NULL) return;

}

rbnode* searchLeftmost(rbnode** root) {
	if ((*root) == NULL)
		return NULL;
	if ((*root)->left != NULL) {
		return searchLeftmost(&((*root)->left));
	}
	return *root;
}

rbnode* search(rbnode** root, int x) {
	if (*root == NULL) {
		return NULL;
	}
	if (x < (*root)->key) {
		search(&((*root)->left), x);
	}
	else if (x > (*root)->key) {
		search(&((*root)->right), x);
	}
	else return (*root);
}

void printNode(rbnode* node) {
	printf("%d ", node->key);
}

void printInorder(rbnode* root) {
	if (root == NULL) return;
	printInorder(root->left);
	printNode(root);
	
	if (root->parent != NULL) {
		rbnode* p = root->parent;
		printf("-> %d, ", p->key);
	}
	printInorder(root->right);
	return;
}

void printLevelorder(rbnode* root) {
	if (root == NULL) return;
	traverseNode* t = (traverseNode*)malloc(sizeof(traverseNode));
	if (t == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}
	t->key = root;
	t->next = NULL;
	traverse(t);
}

void traverse(traverseNode* list) {
	if (list == NULL) return;
	traverseNode* temp = list;
	printNode(temp->key);
	
	addtraverse(list, temp->key->left);
	addtraverse(list, temp->key->right);
	list = list->next;
	free(temp);
	traverse(list);
}

void addtraverse(traverseNode* list, rbnode* node) {
	if (node == NULL) return;
	traverseNode* newNode = (traverseNode*)malloc(sizeof(traverseNode));
	newNode->key = node;
	newNode->next = NULL;
	traverseNode* temp = list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = newNode;
	return;
}