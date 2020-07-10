#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//General Types and Structures definition
enum {RED, BLACK};
typedef int Color;


//---------- DATA RB-TREE DEFINITION ----------

typedef struct NodeData {
    char* data;
    struct NodeData *left, *right, *parent;
    Color color;
} *nodeData;

typedef nodeData* TreeData;
nodeData TNilData;


//Functions TreeEntity
nodeData createNodeData(nodeData, char*);
void RBInsertData(TreeData, char*);
void RBInsertFixupData(nodeData, nodeData);
void LeftRotateData(TreeData, nodeData);
void RightRotateData(TreeData, nodeData);
nodeData RBSearchData(nodeData, char*);
void RBDeleteData(TreeData, nodeData);
nodeData RBSuccessorData(nodeData);
nodeData RBMinimumData(nodeData);
nodeData RBMaximumData(nodeData);
void RBDeleteFixupData(TreeData, nodeData);
void RBInorderTreeWalkData(nodeData);


//Functions definition
nodeData createNodeData(nodeData parent, char* data) {
    nodeData newNode = (nodeData)malloc(sizeof(struct NodeData));
    newNode->left = newNode->right = TNilData;
    newNode->parent = parent;
    char* dataCopied = malloc(strlen(data)+1);
    strcpy(dataCopied, data);
    newNode->data = dataCopied;
    newNode->color = RED;
    return newNode;
}

void LeftRotateData(TreeData tree, nodeData x) {
    nodeData y = x->right;
    x->right = y->left;
    if (y->left != TNilData) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNilData) {
        *tree = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RightRotateData(TreeData tree, nodeData y) {
    nodeData x = y->left;
    y->left = x->right;
    if (x->right != TNilData) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == TNilData) {
        *tree = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void RBInsertData(TreeData tree, char* data){
    nodeData y = TNilData;
    nodeData x = *tree;
    while (x != TNilData) {
        y = x;
        if (strcmp(data, x->data)<0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    nodeData newNode = createNodeData(y, data);
    if (y == TNilData) {
        *tree = newNode;
    }
    else if (strcmp(newNode->data, y->data)<0) {
        y->left = newNode;
    }
    else {
        y->right = newNode;
    }
    newNode->left = TNilData;
    newNode->right = TNilData;
    newNode->color = RED;
    RBInsertFixupData(*tree, newNode);
    //Keeps the root
    nodeData ptr = newNode;
    while(ptr->parent != TNilData) {
        ptr = ptr->parent;
    }
    *tree = ptr;
}

void RBInsertFixupData(nodeData tree, nodeData newNode) {
    if (newNode == tree) {
        tree->color = BLACK;
    } else {
        nodeData x = newNode->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                nodeData y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    RBInsertFixupData(tree, x->parent);
                } else {
                    if (newNode == x->right) {
                        newNode = x;
                        LeftRotateData(&tree, newNode);
                        x = newNode->parent;
                    }
                    x->color = BLACK;
                    x->parent->color = RED;
                    RightRotateData(&tree, x->parent);
                }
            } else {
                nodeData y = x->parent->left;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    RBInsertFixupData(tree, x->parent);
                } else {
                    if (newNode == x->left) {
                        newNode = x;
                        RightRotateData(&tree, newNode);
                        x = newNode->parent;
                    }
                    x->color = BLACK;
                    x->parent->color = RED;
                    LeftRotateData(&tree, x->parent);
                }
            }
        }
    }
}

nodeData RBSearchData(nodeData tree, char* data) {
    nodeData x = tree;
    if (x == TNilData) {
        return TNilData;
    }
    if (strcmp(x->data, data) == 0) {
        return x;
    }
    if (strcmp(data, x->data)<0) {
        return RBSearchData(x->left, data);
    } else {
        return RBSearchData(x->right, data);
    }
}

nodeData RBMinimumData(nodeData x) {
    while (x->left != TNilData) {
        x = x->left;
    }
    return x;
}

nodeData RBMaximumData(nodeData x) {
    while (x->right != TNilData) {
        x = x->right;
    }
    return x;
}

nodeData RBSuccessorData(nodeData x) {
    if (x->right != TNilData) {
        return RBMinimumData(x->right);
    }
    nodeData y = x->parent;
    while (y != TNilData && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

void RBDeleteData(TreeData tree, nodeData z) {
    nodeData y;
    nodeData x;
    if (z->left == TNilData || z->right == TNilData) {
        y = z;
    } else {
        y = RBSuccessorData(z);
    }
    if (y->left != TNilData) {
        x = y->left;
    } else {
        x = y->right;
    }
    x->parent = y->parent;
    if (y->parent == TNilData) {
        *tree = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    if (y != z) {
        free(z->data);
        char* dataCopied = malloc(strlen(y->data)+1);
        strcpy(dataCopied, y->data);
        z->data = dataCopied;
    }
    if (y->color == BLACK) {
        RBDeleteFixupData(tree, x);
    }
    //Keeps memory free
    if (y->data) free(y->data);
    free(y);
}


void RBDeleteFixupData(TreeData tree, nodeData x) {
    while (x != *tree && x->color == BLACK) {
        if (x == x->parent->left) {
            nodeData w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotateData(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotateData(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotateData(tree,x->parent);
                x = *tree;
            }
        } else {
            nodeData w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                RightRotateData(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotateData(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotateData(tree,x->parent);
                x = *tree;
            }
        }
    }
    x->color = BLACK;
}

void RBInorderTreeWalkData(nodeData x) {
    if (x != TNilData) {
        RBInorderTreeWalkData(x->left);
        fputs(x->data, stdout);
        fputs(" ", stdout);
        RBInorderTreeWalkData(x->right);
    }
}

//---------- LIST-OUT DEFINITION ----------

typedef struct NodeListOut {
    char* idRel;
    nodeData treeOut;
    struct NodeListOut *next, *prev;
} *nodeListOut;
typedef nodeListOut* NodeListOutPtr;

nodeListOut createNewNodeListOut(char*, char*);
nodeListOut insertTreeOutInListOut(char*, char*, NodeListOutPtr);
nodeListOut findTreeInListOut(char*, nodeListOut);
void removeTreeOutFromListOut(char*, NodeListOutPtr);
void printListOut(nodeListOut);
void freeTreeData(TreeData);

nodeListOut createNewNodeListOut(char* idRel, char * idEnt) {
    nodeListOut newNode = (nodeListOut)malloc(sizeof(struct NodeListOut));
    char* idRelCopied = malloc(strlen(idRel)+1);
    strcpy(idRelCopied, idRel);
    newNode->idRel = idRelCopied;
    newNode->treeOut = TNilData;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

nodeListOut insertTreeOutInListOut(char* idRel, char * idEnt, NodeListOutPtr listOutHeadPtr) {
    nodeListOut newNode = createNewNodeListOut(idRel, idEnt);
    RBInsertData(&(newNode->treeOut), idEnt);
    if (*listOutHeadPtr == NULL) {
        *listOutHeadPtr = newNode;
        return *listOutHeadPtr;
    }
    nodeListOut ptr = *listOutHeadPtr;
    if (strcmp(idRel, ptr->idRel)<0) {
        newNode->next = *listOutHeadPtr;
        newNode->prev = NULL;
        (*listOutHeadPtr)->prev = newNode;
        (*listOutHeadPtr) = newNode;
        return *listOutHeadPtr;
    }
    while (ptr->next && strcmp(idRel, ptr->next->idRel)>0) {
        ptr = ptr->next;
    }
    newNode->prev = ptr;
    newNode->next = ptr->next;
    if (ptr->next) ptr->next->prev = newNode;
    ptr->next = newNode;
    return newNode;
}

nodeListOut findTreeInListOut(char* idRel, nodeListOut listOutHead) {
    nodeListOut ptr = listOutHead;
    while (ptr != NULL) {
        if (strcmp(ptr->idRel, idRel) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void removeTreesListOut(NodeListOutPtr listOutHeadPtr) {
    nodeListOut ptr = *listOutHeadPtr;
    nodeListOut temp;
    //frees the entire list
    while (ptr) {
        temp = ptr;
        ptr = ptr->next;
        free(temp->idRel);
        free(temp);
    }
    *listOutHeadPtr = NULL;
}

void removeTreeOutFromListOut(char* idRel, NodeListOutPtr listOutHeadPtr) {
    nodeListOut ptr = *listOutHeadPtr;
    //if the list is empty
    if (ptr == NULL) return;
    //if it matches with the head of the list
    if (strcmp(ptr->idRel, idRel) == 0) {
        *listOutHeadPtr = (*listOutHeadPtr)->next;
        if (*listOutHeadPtr) (*listOutHeadPtr)->prev = NULL;
        free(ptr->idRel);
        free(ptr);
        return;
    }
    while (ptr->next != NULL) {
        ptr = ptr->next;
        if (strcmp(ptr->idRel, idRel) == 0) {
            //if it matches with the last element of the list
            if (ptr->next == NULL) {
                ptr->prev->next = NULL;
                free(ptr->idRel);
                free(ptr);
                return;
            }
            //if it matches with a node in the middle of the list
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
            free(ptr->idRel);
            free(ptr);
            return;
        }
    }
}

void printListOut(nodeListOut listOutHead) {
    nodeListOut ptr = listOutHead;
    while(ptr != NULL) {
        printf("[ %s ", ptr->idRel);
        if (ptr->idRel == "") printf(" idRel ABSENT ");
        RBInorderTreeWalkData(ptr->treeOut);
        printf("]");
        ptr = ptr->next;
    }
}

//---------- LIST-COUNTER DEFINITION ----------

typedef struct NodeListCounter {
    char* idRel;
    nodeData treeOut;
    int counter;
    struct NodeListCounter *next, *prev;
} *nodeListCounter;
typedef nodeListCounter* NodeListCounterPtr;

typedef struct NodeEntity {
    char* idEnt;
    nodeListOut listOutHead;
    nodeListCounter listInHead;
    struct NodeEntity *left, *right, *parent;
    Color color;
} *nodeEntity;

typedef nodeEntity* TreeEntity;

nodeListCounter createNewNodeListCounter(char*);
nodeListCounter insertTreeOutInListCounter(char*, char*, NodeListCounterPtr);
nodeListCounter findTreeInListCounter(char*, nodeListCounter);
void removeTreeOutFromListCounter(char*, NodeListCounterPtr);
void printListCounter(nodeListCounter);
void removeTreesListCounter(NodeListCounterPtr);
void removeNodesListCounterAndUpdateListReport(char*, TreeEntity, NodeListCounterPtr, NodeListCounterPtr);
void computeMaxCounter(char*, nodeEntity, nodeListCounter);
void updateTreeOutListReport(char*, nodeEntity, nodeListCounter);
void computeMaxCounterWithException(char*, char*, nodeEntity, nodeListCounter);
void updateTreeOutListReportWithException(char*, char*, nodeEntity, nodeListCounter);

nodeListCounter createNewNodeListCounter(char* idRel) {
    nodeListCounter newNode = (nodeListCounter)malloc(sizeof(struct NodeListCounter));
    char* idRelCopied = malloc(strlen(idRel)+1);
    strcpy(idRelCopied, idRel);
    newNode->idRel = idRelCopied;
    newNode->treeOut = TNilData;
    newNode->counter = 1;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

nodeListCounter insertTreeOutInListCounter(char* idRel, char * idEnt, NodeListCounterPtr listCounterHeadPtr) {
    nodeListCounter newNode = createNewNodeListCounter(idRel);
    RBInsertData(&(newNode->treeOut), idEnt);
    if (*listCounterHeadPtr == NULL) {
        *listCounterHeadPtr = newNode;
        return *listCounterHeadPtr;
    }
    nodeListCounter ptr = *listCounterHeadPtr;
    if (strcmp(idRel, ptr->idRel)<0) {
        newNode->next = *listCounterHeadPtr;
        newNode->prev = NULL;
        (*listCounterHeadPtr)->prev = newNode;
        (*listCounterHeadPtr) = newNode;
        return *listCounterHeadPtr;
    }
    while (ptr->next && strcmp(idRel, ptr->next->idRel)>0) {
        ptr = ptr->next;
    }
    newNode->prev = ptr;
    newNode->next = ptr->next;
    if (ptr->next) ptr->next->prev = newNode;
    ptr->next = newNode;
    return newNode;
}

nodeListCounter findTreeInListCounter(char* idRel, nodeListCounter listCounterHead) {
    nodeListCounter ptr = listCounterHead;
    while (ptr != NULL) {
        if (strcmp(ptr->idRel, idRel) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void removeTreesListCounter(NodeListCounterPtr listCounterHeadPtr) {
    nodeListCounter ptr = *listCounterHeadPtr;
    nodeListCounter temp;
    //frees the entire list
    while (ptr) {
        temp = ptr;
        ptr = ptr->next;
        free(temp->idRel);
        free(temp);
    }
    *listCounterHeadPtr = NULL;
}

void removeNodesListCounterAndUpdateListReport(char* idEnt, TreeEntity treeEntity, NodeListCounterPtr listCounterHeadPtr, NodeListCounterPtr listReportHeadPtr) {
    nodeListCounter ptr = *listCounterHeadPtr;
    nodeListCounter temp;
    while(ptr) {
        //Manages listReport
        nodeListCounter nodeReportFound = findTreeInListCounter(ptr->idRel, *listReportHeadPtr);
        if (nodeReportFound) {
            if (ptr->counter == nodeReportFound->counter) {
                ptr->counter = 0;

                // => the idDest is present within the treeOut of the nodeReport. It needs to be deleted
                nodeData nodeReportDestFound = RBSearchData(nodeReportFound->treeOut, idEnt);
                RBDeleteData(&(nodeReportFound->treeOut), nodeReportDestFound);
                if (nodeReportFound->treeOut == TNilData) {
                    //Computes new idDest with max counter and updades to treeReport
                    nodeReportFound->counter = 0;
                    computeMaxCounterWithException(idEnt, ptr->idRel, *treeEntity, nodeReportFound);
                    if (nodeReportFound->counter != 0){
                        updateTreeOutListReportWithException(idEnt, ptr->idRel, *treeEntity, nodeReportFound);
                    } else {
                        removeTreeOutFromListCounter(ptr->idRel, listReportHeadPtr);
                    }
                }
            }
            ptr->counter = 0;
        }
        temp = ptr;
        ptr = ptr->next;
        free(temp->idRel);
        free(temp);
    }
    *listCounterHeadPtr = NULL;
}

void removeTreeOutFromListCounter(char* idRel, NodeListCounterPtr listCounterHeadPtr) {
    nodeListCounter ptr = *listCounterHeadPtr;
    //if the list is empty
    if (ptr == NULL) return;
    //if it matches with the head of the list
    if (strcmp(ptr->idRel, idRel) == 0) {
        *listCounterHeadPtr = (*listCounterHeadPtr)->next;
        if (*listCounterHeadPtr) (*listCounterHeadPtr)->prev = NULL;
        free(ptr->idRel);
        free(ptr);
        return;
    }
    while (ptr->next != NULL) {
        ptr = ptr->next;
        if (strcmp(ptr->idRel, idRel) == 0) {
            //if it matches with the last element of the list
            if (ptr->next == NULL) {
                ptr->prev->next = NULL;
                free(ptr->idRel);
                free(ptr);
                return;
            }
            //if it matches with a node in the middle of the list
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
            free(ptr->idRel);
            free(ptr);
            return;
        }
    }
}

void printListCounter(nodeListCounter listCounterHead) {
    nodeListCounter ptr = listCounterHead;
    while(ptr != NULL) {
        printf("[ %s %d ", ptr->idRel , ptr->counter);
        printf("]");
        ptr = ptr->next;
    }
}

//---------- ENTITY RB-TREE DEFINITION ----------

//Definition on nilNodeEntity
nodeEntity TNilEntity;


//Functions TreeEntity
nodeEntity createNodeEntity(nodeEntity, char*);
void RBInsertEntity(TreeEntity, char*);
void RBInsertFixupEntity(nodeEntity, nodeEntity);
void LeftRotateEntity(TreeEntity, nodeEntity);
void RightRotateEntity(TreeEntity, nodeEntity);
nodeEntity RBSearchEntity(nodeEntity, char*);
void RBDeleteEntity(TreeEntity, nodeEntity);
nodeEntity RBSuccessorEntity(nodeEntity);
nodeEntity RBMinimumEntity(nodeEntity);
nodeEntity RBMaximumEntity(nodeEntity);
void RBDeleteFixupEntity(TreeEntity, nodeEntity);
void RBInorderTreeWalkEntity(nodeEntity);


//Functions definition
nodeEntity createNodeEntity(nodeEntity parent, char* data) {
    nodeEntity newNode = (nodeEntity)malloc(sizeof(struct NodeEntity));
    newNode->left = newNode->right = TNilEntity;
    newNode->parent = parent;
    char* dataCopied = malloc(strlen(data)+1);
    strcpy(dataCopied, data);
    newNode->idEnt = dataCopied;
    newNode->listOutHead = NULL;
    newNode->listInHead = NULL;
    newNode->color = RED;
    return newNode;
}

void LeftRotateEntity(TreeEntity tree, nodeEntity x) {
    nodeEntity y = x->right;
    x->right = y->left;
    if (y->left != TNilEntity) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNilEntity) {
        *tree = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RightRotateEntity(TreeEntity tree, nodeEntity y) {
    nodeEntity x = y->left;
    y->left = x->right;
    if (x->right != TNilEntity) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == TNilEntity) {
        *tree = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void RBInsertEntity(TreeEntity tree, char* idEnt){
    nodeEntity y = TNilEntity;
    nodeEntity x = *tree;
    while (x != TNilEntity) {
        y = x;
        if (strcmp(idEnt, x->idEnt)<0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    nodeEntity newNode = createNodeEntity(y, idEnt);
    if (y == TNilEntity) {
        *tree = newNode;
    }
    else if (strcmp(newNode->idEnt, y->idEnt)<0) {
        y->left = newNode;
    }
    else {
        y->right = newNode;
    }
    newNode->left = TNilEntity;
    newNode->right = TNilEntity;
    newNode->color = RED;
    RBInsertFixupEntity(*tree, newNode);
    //Keeps the root
    nodeEntity ptr = newNode;
    while(ptr->parent != TNilEntity) {
        ptr = ptr->parent;
    }
    *tree = ptr;
}

void RBInsertFixupEntity(nodeEntity tree, nodeEntity newNode) {
    if (newNode == tree) {
        tree->color = BLACK;
    } else {
        nodeEntity x = newNode->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                nodeEntity y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    RBInsertFixupEntity(tree, x->parent);
                } else {
                    if (newNode == x->right) {
                        newNode = x;
                        LeftRotateEntity(&tree, newNode);
                        x = newNode->parent;
                    }
                    x->color = BLACK;
                    x->parent->color = RED;
                    RightRotateEntity(&tree, x->parent);
                }
            } else {
                nodeEntity y = x->parent->left;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    RBInsertFixupEntity(tree, x->parent);
                } else {
                    if (newNode == x->left) {
                        newNode = x;
                        RightRotateEntity(&tree, newNode);
                        x = newNode->parent;
                    }
                    x->color = BLACK;
                    x->parent->color = RED;
                    LeftRotateEntity(&tree, x->parent);
                }
            }
        }
    }
}

nodeEntity RBSearchEntity(nodeEntity tree, char* idEnt) {
    nodeEntity x = tree;
    if (x == TNilEntity) {
        return TNilEntity;
    }
    if (strcmp(x->idEnt, idEnt) == 0) {
        return x;
    }
    if (strcmp(idEnt, x->idEnt)<0) {
        return RBSearchEntity(x->left, idEnt);
    } else {
        return RBSearchEntity(x->right, idEnt);
    }
}

nodeEntity RBMinimumEntity(nodeEntity x) {
    while (x->left != TNilEntity) {
        x = x->left;
    }
    return x;
}

nodeEntity RBMaximumEntity(nodeEntity x) {
    while (x->right != TNilEntity) {
        x = x->right;
    }
    return x;
}

nodeEntity RBSuccessorEntity(nodeEntity x) {
    if (x->right != TNilEntity) {
        return RBMinimumEntity(x->right);
    }
    nodeEntity y = x->parent;
    while (y != TNilEntity && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

void RBDeleteEntity(TreeEntity tree, nodeEntity z) {
    nodeEntity y;
    nodeEntity x;
    if (z->left == TNilEntity || z->right == TNilEntity) {
        y = z;
    } else {
        y = RBSuccessorEntity(z);
    }
    if (y->left != TNilEntity) {
        x = y->left;
    } else {
        x = y->right;
    }
    x->parent = y->parent;
    if (y->parent == TNilEntity) {
        *tree = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    if (y != z) {
        free(z->idEnt);
        char* idEntCopied = malloc(strlen(y->idEnt)+1);
        strcpy(idEntCopied, y->idEnt);
        z->idEnt = idEntCopied;
        z->listOutHead = y->listOutHead;
        z->listInHead = y->listInHead;
    }
    if (y->color == BLACK) {
        RBDeleteFixupEntity(tree, x);
    }
    //Keeps memory free
    if (y->idEnt) free(y->idEnt);
    free(y);
}


void RBDeleteFixupEntity(TreeEntity tree, nodeEntity x) {
    while (x != *tree && x->color == BLACK) {
        if (x == x->parent->left) {
            nodeEntity w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotateEntity(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotateEntity(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotateEntity(tree,x->parent);
                x = *tree;
            }
        } else {
            nodeEntity w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                RightRotateEntity(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotateEntity(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotateEntity(tree,x->parent);
                x = *tree;
            }
        }
    }
    x->color = BLACK;
}

void RBInorderTreeWalkEntity(nodeEntity x) {
    if (x != TNilEntity) {
        RBInorderTreeWalkEntity(x->left);
        printf("\n(%s)", x->idEnt);
        printf(" ListIn: ");
        printListCounter(x->listInHead);
        printf(" ListOut: ");
        printListOut(x->listOutHead);
        printf(" ");
        RBInorderTreeWalkEntity(x->right);
    }
}



//----------- General Functions definition -----------
void addent(char*, TreeEntity);
void addrel(char*, char*, char*, TreeEntity, NodeListCounterPtr);
void delrel(char*, char*, char*, TreeEntity, NodeListCounterPtr);
void delent(char*, TreeEntity, NodeListCounterPtr);
void report(nodeListCounter);
void deleteRecursivelyListOut(char*, char*, TreeData, TreeEntity, NodeListCounterPtr);
void deleteNodeInEveryListOut(char*, nodeEntity);



int main() {
    //Initialization of TNilEntity, TNilData, TNilRel
    TNilEntity = (nodeEntity)malloc(sizeof(struct NodeEntity));
    TNilEntity->parent = TNilEntity;
    TNilEntity->right = TNilEntity;
    TNilEntity->left = TNilEntity;
    TNilEntity->idEnt = NULL;
    TNilEntity->listOutHead = NULL;
    TNilEntity->color = BLACK;

    TNilData = (nodeData)malloc(sizeof(struct NodeData));
    TNilData->parent = TNilData;
    TNilData->right = TNilData;
    TNilData->left = TNilData;
    TNilData->data = NULL;
    TNilData->color = BLACK;


    //Initialize RBTrees
    nodeEntity treeEntity = TNilEntity;
    nodeListCounter listReportHead = NULL;



    char buffer[100];
    char* command = "";
    while (strcmp(command, "end") != 0) {
        char* str = fgets(buffer, 100, stdin);
        command = strtok(buffer, " \"\n");
        if (strcmp(command, "addent") == 0) {
            char* idEnt = strtok(NULL, " \n");
            addent(idEnt, &treeEntity);
        }
        else if (strcmp(command, "delent") == 0) {
            char * idEnt = strtok(NULL, " \n");
            delent(idEnt, &treeEntity, &listReportHead);
        }
        else if (strcmp(command, "addrel") == 0) {
            char * idOrig = strtok(NULL, " \n");
            char * idDest = strtok(NULL, " \n");
            char * idRel = strtok(NULL, " \n");
            addrel(idOrig, idDest, idRel, &treeEntity, &listReportHead);
        }
        else if (strcmp(command, "delrel") == 0) {
            char * idOrig = strtok(NULL, " \n");
            char * idDest = strtok(NULL, " \n");
            char * idRel = strtok(NULL, " \n");
            delrel(idOrig, idDest, idRel, &treeEntity, &listReportHead);
        }
        else if (strcmp(command, "report") == 0) {
            report(listReportHead);
            fputs("\n", stdout);
        }
    }
    return 0;
}

void addent(char* idEnt, TreeEntity treeEntity) {
    nodeEntity nodeEntityFound = RBSearchEntity(*treeEntity, idEnt);
    if (nodeEntityFound == TNilEntity) {
        RBInsertEntity(treeEntity, idEnt);
    }
}

void addrel(char* idOrig, char* idDest, char* idRel, TreeEntity treeEntity, NodeListCounterPtr listReportHeadPtr) {
    nodeEntity nodeOrigFound = RBSearchEntity(*treeEntity, idOrig);
    if (nodeOrigFound != TNilEntity) {
        nodeEntity nodeDestFound = RBSearchEntity(*treeEntity, idDest);
        if (nodeDestFound != TNilEntity) {
            //Manages the listOut - checks if idDest is in the listOut of idOrig. If not, it adds it.
            nodeListOut nodeListOutFound = findTreeInListOut(idRel, nodeOrigFound->listOutHead);
            if (nodeListOutFound) {
                nodeData nodeDestInListOut = RBSearchData(nodeListOutFound->treeOut, idDest);
                if (nodeDestInListOut == TNilData) {
                    //the relationship isn't already present
                    RBInsertData(&(nodeListOutFound->treeOut), idDest);
                    //Manages the listIn
                    nodeListCounter nodeListInFound = findTreeInListCounter(idRel, nodeDestFound->listInHead);
                    if (nodeListInFound) {
                        nodeListInFound->counter += 1;
                        nodeListCounter nodeReportFound = findTreeInListCounter(idRel, *listReportHeadPtr);
                        if (nodeReportFound->counter < nodeListInFound->counter) {
                            freeTreeData(&(nodeReportFound->treeOut));
                            nodeReportFound->treeOut = TNilData;
                            RBInsertData(&(nodeReportFound->treeOut), idDest);
                            nodeReportFound->counter = nodeListInFound->counter;
                        } else if (nodeReportFound->counter == nodeListInFound->counter) {
                            RBInsertData(&(nodeReportFound->treeOut), idDest);
                        }
                    } else {
                        insertTreeOutInListCounter(idRel, idOrig, &(nodeDestFound->listInHead));
                        //if there isn't the node associated with the idRel or its counter is = 1 it adds the node.
                        nodeListCounter nodeReportFound = findTreeInListCounter(idRel, *listReportHeadPtr);
                        if (nodeReportFound && nodeReportFound->counter == 1) {
                            RBInsertData(&(nodeReportFound->treeOut), idDest);
                        } else if (nodeReportFound == NULL) {
                            insertTreeOutInListCounter(idRel, idDest, listReportHeadPtr);
                        }
                    }
                }
            } else {
                nodeListOut nodeInserted = insertTreeOutInListOut(idRel, idDest, &(nodeOrigFound->listOutHead));
                //Manages the listIn
                nodeListCounter nodeListInFound = findTreeInListCounter(idRel, nodeDestFound->listInHead);
                if (nodeListInFound) {
                    nodeListInFound->counter += 1;
                    nodeListCounter nodeReportFound = findTreeInListCounter(idRel, *listReportHeadPtr);
                    if (nodeReportFound->counter < nodeListInFound->counter) {
                        freeTreeData(&(nodeReportFound->treeOut));
                        nodeReportFound->treeOut = TNilData;
                        RBInsertData(&(nodeReportFound->treeOut), idDest);
                        nodeReportFound->counter = nodeListInFound->counter;
                    } else if (nodeReportFound->counter == nodeListInFound->counter) {
                        RBInsertData(&(nodeReportFound->treeOut), idDest);
                    }
                } else {
                    insertTreeOutInListCounter(idRel, idOrig, &(nodeDestFound->listInHead));
                    //if there isn't the node associated with the idRel or its counter is = 1 it adds the node.
                    nodeListCounter nodeReportFound = findTreeInListCounter(idRel, *listReportHeadPtr);
                    if (nodeReportFound && nodeReportFound->counter == 1) {
                        RBInsertData(&(nodeReportFound->treeOut), idDest);
                    } else if (nodeReportFound == NULL) {
                        insertTreeOutInListCounter(idRel, idDest, listReportHeadPtr);
                    }
                }
            }
        }
    }
}

void freeTreeData(TreeData treeData) {
    if (*treeData == TNilData) return;

    freeTreeData(&((*treeData)->left));
    freeTreeData(&((*treeData)->right));

    free((*treeData)->data);
    free(*treeData);
}

void delent(char* idEnt, TreeEntity treeEntity, NodeListCounterPtr listReportHeadPtr) {

    nodeEntity nodeEntityFound = RBSearchEntity(*treeEntity, idEnt);
    if (nodeEntityFound != TNilEntity) {
        nodeListOut ptrOut = nodeEntityFound->listOutHead;
        while (ptrOut) {
            deleteRecursivelyListOut(idEnt, ptrOut->idRel, &(ptrOut->treeOut), treeEntity, listReportHeadPtr);
            ptrOut = ptrOut->next;
        }
        removeTreesListOut(&(nodeEntityFound->listOutHead));
        deleteNodeInEveryListOut(idEnt, *treeEntity);
        removeNodesListCounterAndUpdateListReport(idEnt, treeEntity, &(nodeEntityFound->listInHead), listReportHeadPtr);
        RBDeleteEntity(treeEntity, nodeEntityFound);
    }
}

void deleteNodeInEveryListOut(char* idEnt, nodeEntity treeEntity) {
    if (treeEntity != TNilEntity) {
        deleteNodeInEveryListOut(idEnt, treeEntity->left);
        deleteNodeInEveryListOut(idEnt, treeEntity->right);

        if (strcmp(treeEntity->idEnt, idEnt) != 0) {
            nodeListOut ptrListOut = treeEntity->listOutHead;
            nodeListOut temp;
            while (ptrListOut) {
                nodeData nodeDestInListOut = RBSearchData(ptrListOut->treeOut, idEnt);
                if (nodeDestInListOut != TNilData) RBDeleteData(&(ptrListOut->treeOut), nodeDestInListOut);
                ptrListOut = ptrListOut->next;
            }
        }
    }
}

void deleteRecursivelyListOut(char* idEnt, char* idRel, TreeData treeData, TreeEntity treeEntity, NodeListCounterPtr listReportHeadPtr) {
    if (*treeData != TNilData) {
        deleteRecursivelyListOut(idEnt, idRel, &((*treeData)->left), treeEntity, listReportHeadPtr);
        deleteRecursivelyListOut(idEnt, idRel, &((*treeData)->right), treeEntity, listReportHeadPtr);

        nodeEntity nodeDestFound = RBSearchEntity(*treeEntity, (*treeData)->data);
        nodeListCounter nodeListInFound = findTreeInListCounter(idRel, nodeDestFound->listInHead);

        nodeListInFound->counter -= 1;
        if (nodeListInFound->counter == 0) {
            removeTreeOutFromListCounter(idRel, &(nodeDestFound->listInHead));
        }
        int counterUpdated = nodeListInFound->counter;
        //Manage the listReport
        nodeListCounter nodeReportFound = findTreeInListCounter(idRel, *listReportHeadPtr);
        if (nodeReportFound->counter == counterUpdated + 1) {
            nodeData nodeReportDestFound = RBSearchData(nodeReportFound->treeOut, (*treeData)->data);
            RBDeleteData(&(nodeReportFound->treeOut), nodeReportDestFound);
            if (nodeReportFound->treeOut == TNilData) {
                //Computes new idDest with max counter and updades to treeReport
                nodeReportFound->counter = 0;
                computeMaxCounter(idRel, *treeEntity, nodeReportFound);
                if (nodeReportFound->counter != 0){
                    updateTreeOutListReport(idRel, *treeEntity, nodeReportFound);
                } else {
                    removeTreeOutFromListCounter(idRel, listReportHeadPtr);
                }
            }
        }

        //Frees the node within the TreeData in the listOut
        free((*treeData)->data);
        free(*treeData);
        *treeData = TNilData;
    }
}

void delrel(char* idOrig, char* idDest, char* idRel, TreeEntity treeEntity, NodeListCounterPtr listReportHeadPtr) {
    nodeEntity nodeOrigFound = RBSearchEntity(*treeEntity, idOrig);
    if (nodeOrigFound != TNilEntity) {
        nodeEntity nodeDestFound = RBSearchEntity(*treeEntity, idDest);
        if (nodeDestFound != TNilEntity) {
            //verifies whether the relationship exists or not
            nodeListOut nodeListOutFound = findTreeInListOut(idRel, nodeOrigFound->listOutHead);
            if (nodeListOutFound) {
                nodeData nodeDestInListOut = RBSearchData(nodeListOutFound->treeOut, idDest);
                if (nodeDestInListOut != TNilData) {
                    // => the rel exists => it can be deleted from listOut and reduce counter in listIn in the nodeDestFound
                    //Deletes the idDest present within the listOut of the idOrig.
                    RBDeleteData(&(nodeListOutFound->treeOut), nodeDestInListOut);

                    //Deletes the idOrig present within the listIn of the idDest.
                    nodeListCounter nodeListInFound = findTreeInListCounter(idRel, nodeDestFound->listInHead);
                    if (nodeListInFound) {
                        nodeListInFound->counter -= 1;
                        int counterUpdated = nodeListInFound->counter;
                        if (nodeListInFound->counter == 0) {
                            removeTreeOutFromListCounter(idRel, &(nodeDestFound->listInHead));
                        }
                        //Manages the listReport
                        nodeListCounter nodeReportFound = findTreeInListCounter(idRel, *listReportHeadPtr);
                        if (nodeReportFound->counter == counterUpdated + 1) {
                            // => the idDest is present within the treeOut of the nodeReport. It needs to be deleted
                            nodeData nodeReportDestFound = RBSearchData(nodeReportFound->treeOut, idDest);
                            RBDeleteData(&(nodeReportFound->treeOut), nodeReportDestFound);
                            if (nodeReportFound->treeOut == TNilData) {
                                //Computes new idDest with max counter and updades to treeReport
                                nodeReportFound->counter = 0;
                                computeMaxCounter(idRel, *treeEntity, nodeReportFound);
                                if (nodeReportFound->counter != 0){
                                    updateTreeOutListReport(idRel, *treeEntity, nodeReportFound);
                                } else {
                                    removeTreeOutFromListCounter(idRel, listReportHeadPtr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void updateTreeOutListReport(char* idRel, nodeEntity treeEntity, nodeListCounter nodeListReport) {
    if (treeEntity != TNilEntity) {
        updateTreeOutListReport(idRel, treeEntity->left, nodeListReport);
        nodeListCounter nodeListInFound = findTreeInListCounter(idRel, treeEntity->listInHead);
        if (nodeListInFound && nodeListInFound->counter == nodeListReport->counter) {
            RBInsertData(&(nodeListReport->treeOut), treeEntity->idEnt);
        }
        updateTreeOutListReport(idRel, treeEntity->right, nodeListReport);
    }
}

void updateTreeOutListReportWithException(char* idEnt, char* idRel, nodeEntity treeEntity, nodeListCounter nodeListReport) {
    if (treeEntity != TNilEntity) {
        updateTreeOutListReportWithException(idEnt, idRel, treeEntity->left, nodeListReport);
        if (strcmp(idEnt, treeEntity->idEnt) != 0) {
            nodeListCounter nodeListInFound = findTreeInListCounter(idRel, treeEntity->listInHead);
            if (nodeListInFound && nodeListInFound->counter == nodeListReport->counter) {
                RBInsertData(&(nodeListReport->treeOut), treeEntity->idEnt);
            }
        }
        updateTreeOutListReportWithException(idEnt, idRel, treeEntity->right, nodeListReport);
    }
}


void computeMaxCounter(char* idRel, nodeEntity treeEntity, nodeListCounter nodeListReport) {
    if (treeEntity != TNilEntity) {
        computeMaxCounter(idRel, treeEntity->left, nodeListReport);
        if (treeEntity->listInHead) {
            nodeListCounter nodeListInFound = findTreeInListCounter(idRel, treeEntity->listInHead);
            if (nodeListInFound && nodeListInFound->counter > nodeListReport->counter) nodeListReport->counter = nodeListInFound->counter;
        }
        computeMaxCounter(idRel, treeEntity->right, nodeListReport);
    }
}

void computeMaxCounterWithException(char* idEnt, char* idRel, nodeEntity treeEntity, nodeListCounter nodeListReport) {
    if (treeEntity != TNilEntity) {
        computeMaxCounterWithException(idEnt, idRel, treeEntity->left, nodeListReport);
        if (strcmp(idEnt, treeEntity->idEnt) != 0) {
            if (treeEntity->listInHead) {
                nodeListCounter nodeListInFound = findTreeInListCounter(idRel, treeEntity->listInHead);
                if (nodeListInFound && nodeListInFound->counter > nodeListReport->counter) nodeListReport->counter = nodeListInFound->counter;
            }
        }
        computeMaxCounterWithException(idEnt, idRel, treeEntity->right, nodeListReport);
    }
}

void report(nodeListCounter listReportHead) {
    nodeListCounter ptr = listReportHead;
    if (ptr == NULL) {
        fputs("none", stdout);
        return;
    }
    while (ptr) {
        fputs(ptr->idRel, stdout);
        fputs(" ", stdout);
        RBInorderTreeWalkData(ptr->treeOut);
        printf("%d;", ptr->counter);
        ptr = ptr->next;
        if (ptr) fputs(" ", stdout);
    }
}