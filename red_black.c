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
void rotFix(rbnode** root, rbnode* g, rbnode* x);
void rightRot(rbnode** root, rbnode* x);

void leftRot(rbnode** root, rbnode* x);
//deletion
void deleteInt(rbnode** root, int x);
void delete(rbnode** root, rbnode* x);
rbnode* deleteNode(rbnode** root, rbnode* x);
void deleteFixup(rbnode** root, rbnode* x, rbnode* sibling);

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

rbnode* sibling(rbnode* x);


int main(){
	rbnode** root = createTree();
	insertInt(root, 7);

	insertInt(root, 3);

	insertInt(root, 18);

	insertInt(root, 10);

	insertInt(root, 22);

	insertInt(root, 8);
	insertInt(root, 11);
	insertInt(root, 26);
	insertInt(root, 2);
	insertInt(root, 6);
	insertInt(root, 13);
	printInorder(*root);
	printf("\n");
	deleteInt(root, 18);
	printInorder(*root);
	printf("\n");
	deleteInt(root, 11);
	printInorder(*root);
	printf("\n");
	deleteInt(root, 3);
	printInorder(*root);
	printf("\n");
	deleteInt(root, 10);
	printInorder(*root);
	printf("\n");
	deleteInt(root, 22);
	
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
		return;
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
	if (x == NULL) return;
	rbnode* p = x->parent;
	if (p == NULL) {
		x->color = BLACK;
		return;
	}
	if (p->color == BLACK) return;
	if (sibling(p) == NULL) {

		rotFix(root, p->parent,x);
	}
	else if (sibling(p)->color == RED) {
		rbnode* g = p->parent;
		g->color = RED;
		p->color = BLACK;
		sibling(p)->color = BLACK;
		insertFixup(root, g);
	}
	else {
		rotFix(root, p->parent,x);
	}
	
}

void rotFix(rbnode** root, rbnode* g, rbnode* x) {
	//g -> 할아버지
	rbnode* p = x->parent;
	if (x->key < g->key) {
		
		if (x->key < p->key) {
			//LL
			g->color = RED;
			p->color = BLACK;
			rightRot(root, g);
		}
		else {
			//LR		
			
			x->color = BLACK;
			g->color = RED;
			leftRot(root, p);
			rightRot(root, g);
		}
	}
	else {
		if (p->key < x->key) {
			//RR
			g->color = RED;
			p->color = BLACK;
			leftRot(root,g);
		}
		else {
			//RL
			g->color = RED;
			x->color = BLACK;
			rightRot(root,p);
			leftRot(root,g);

		}
	}

}



void rightRot(rbnode** root, rbnode* x) {
	rbnode* p = x->parent;
	
	rbnode* temp = x->left;
	x->left = temp->right;

	temp->parent = p;
	
	temp->right = x;
	x->parent = temp;
	if (p != NULL) {
		if (p->key < temp->key)p->right = temp;
		else p->left = temp;
	}
	else {
		*root = temp;
	}
	
	
	return;
}
  
void leftRot(rbnode** root, rbnode* x) {
	rbnode* p = x->parent;
	rbnode* temp = x->right;
	temp->parent = p;
	x->right = temp->left;
	temp->left = x;
	x->parent = temp;
	if (p != NULL) {
		if (p->key < temp->key)p->right = temp;
		else p->left = temp;
	}
	else {
		*root = temp;
	}

	
	return;
	
	
}

rbnode* sibling(rbnode* x) {
	rbnode* p = x->parent;
	if (p->left == x) {
		return p->right;
	}
	return p->left;
}

void deleteInt(rbnode** root, int x) {
	rbnode* node = search(root, x);
	delete(root, node);
}

void delete(rbnode** root, rbnode* x) {
	int color = x->color;
	if (*root == x) {
		rbnode* target = deleteNode(root, x);
		(*root)->color = BLACK;
		return;
	}
	rbnode* s = sibling(x);
	rbnode* target = deleteNode(root, x);
	if (color == BLACK) {
		deleteFixup(root, target,s);
	}
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

void deleteFixup(rbnode** root, rbnode* x, rbnode* sibling) {
	if (x == NULL||x->color==BLACK) {
		if (sibling == NULL) {
			return;
		}
		rbnode* p = sibling->parent;
		
		if (p->key < sibling->key) {
			if (sibling->color == RED) {
				sibling->color = BLACK;
				leftRot(root, p);
				return;
			}
			else {
				rbnode* l = sibling->left;
				rbnode* r = sibling->right;
				if (l == NULL && r == NULL) {
					sibling->color = RED;
					return;

				}
				else if (l == NULL) {
					if (r->color == BLACK) {
						sibling->color = RED;
						return;
					}
					else {
						//2-3
						p->color = BLACK;
						r -> color = BLACK;
						leftRot(root, p);
					}
				}
				else if (r == NULL) {
					if (l->color == BLACK) {
						sibling->color = RED;
						return;
					}
					else {
						//2-4
						p->color = BLACK;
						l->color = BLACK;
						rightRot(root,sibling);
						leftRot(root, p);
					}
				}
				else {
					if (r->color == BLACK&&l->color == BLACK) {
						sibling->color = RED;
						return;
					}
					else if (r->color == RED) {
						//2-3
						p->color = BLACK;
						r->color = BLACK;
						leftRot(root, p);
					}
					else {
						//2-4
						p->color = BLACK;
						l->color = BLACK;


						rightRot(root, sibling);
						leftRot(root, p);
					}
				}
			}
		}
		else {
			if (sibling->color == RED) {
				sibling->color = BLACK;
				rightRot(root, p);
			}
			else {
				rbnode* l = sibling->left;
				rbnode* r = sibling->right;
				if (l == NULL && r == NULL) {
					sibling->color = RED;
					return;
				}
				else if (l == NULL) {
					if (r->color == BLACK) {
						sibling->color = RED;
						return;
					}
					else {
						//2-4 rev
						r->color = BLACK;
						p->color = BLACK;
						leftRot(root, sibling);
						rightRot(root, p);
					}
				}
				else if (r == NULL) {
					if (l->color == BLACK) {
						sibling->color = RED;
						return;
					}
					else {
						//2-3 rev
						l->color = BLACK;
						p->color = BLACK;
						rightRot(root, p);
					}
				}
				else {
					if (r->color == BLACK && l->color == BLACK) {
						sibling->color = RED;
						return;
					}
					else if (r->color == RED) {
						//2-4 rev
						r->color = BLACK;
						p->color = BLACK;
						leftRot(root, sibling);
						rightRot(root, p);
					
					}
					else {
						//2-3 rev
						l->color = BLACK;
						p->color = BLACK;
						rightRot(root, p);
					}
				}
			}
		}
		
	}
	if (x->color == RED) {
		x->color = BLACK;
		return;
	}
	
	
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
	
	/*if (root->parent != NULL) {
		rbnode* p = root->parent;
		printf("-> %d, ", p->key);
		
	}
	if (root->color == RED) {
		printf("RED,");
	}
	else printf("BLACK, ");*/
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