#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// 노드 초기화 관련 함수
node_t *create_node(key_t key);
void addNil(node_t *cur, rbtree *tree);
void designateParent(node_t *cur, node_t *parentToBe);
// 순회 관련 함수
node_t *branch_off(node_t *cur, key_t key);
void insert_fix_up(rbtree *t, node_t *cur);
// 회전 함수
void rightRotate(rbtree *t, node_t *cur);
void leftRotate(rbtree *t, node_t *cur);
// 색칠 함수
void parentRedChildsBlack(node_t *parent);
void parentBlackChildsRed(node_t *parent);

void transPlant(rbtree *t, node_t *u, node_t *v);
// 서브노드의 최대,최소 값
node_t *subNodeMin(const rbtree *t, node_t *subNode);
node_t *subNodeMax(const rbtree *t, node_t *subNode);

void printTree(rbtree *t, node_t *cur);

void erase_fix_up(rbtree *t, node_t *x);

rbtree *new_rbtree(void)
{
    // printf("어디서 잘못1\n");
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // NIL node 생성
    node_t *NIL = create_node(INT_MAX);
    NIL->color = RBTREE_BLACK;
    addNil(NIL, p);
    p->root = NIL;
    p->nil = NIL;
    return p;
}

void delete_sub(rbtree *t, node_t *node)
{
    if (node == t->nil)
    {
        return;
    }
    delete_sub(t, node->left);
    delete_sub(t, node->right);
    free(node);
    return;
}
void delete_rbtree(rbtree *t)
{
    delete_sub(t, t->root);
    free(t->nil);
    free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
    // 삽입할 노드의 부모 찾기
    node_t *y = t->nil;
    node_t *x = t->root;
    while (x != t->nil)
    {
        y = x;
        x = branch_off(x, key);
    }

    // 삽입할 노드 초기화
    node_t *nodeToInsert = create_node(key);
    designateParent(nodeToInsert, y);
    addNil(nodeToInsert, t);

    // 노드 삽입
    if (y == t->nil)
    {
        t->root = nodeToInsert;
    }
    else if (key <= y->key)
    {
        y->left = nodeToInsert;
    }
    else
    {
        y->right = nodeToInsert;
    }

    insert_fix_up(t, nodeToInsert);
    return t->root;
}

void insert_fix_up(rbtree *t, node_t *cur)
{
    while (cur->parent->color == RBTREE_RED)
    {
        // 부모가 증부모의 왼쪽일 때
        if (cur->parent->parent->left == cur->parent)
        {
            node_t *uncle = cur->parent->parent->right;
            if (uncle->color == RBTREE_RED)
            {
                parentRedChildsBlack(cur->parent->parent);
                cur = cur->parent->parent;
            }
            else
            {
                node_t *parent = cur->parent;
                if (parent->right == cur)
                {
                    leftRotate(t, parent);
                    parent = cur;
                }
                rightRotate(t, parent);
                // parent->parent는 parent의 자식 노드가 되기 때문에 parent가 최종적으로 부모가 됨.
                parentBlackChildsRed(parent);
            }
        }
        // 부모가 증부모의 오른쪽일 때
        else
        {
            node_t *uncle = cur->parent->parent->left;
            if (uncle->color == RBTREE_RED)
            {
                parentRedChildsBlack(cur->parent->parent);
                cur = cur->parent->parent;
            }
            else
            {
                // triangle
                node_t *parent = cur->parent;
                if (cur->parent->left == cur)
                {
                    rightRotate(t, cur);
                    parent = cur;
                }
                leftRotate(t, parent->parent);
                parentBlackChildsRed(parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
    node_t *cur = t->root;
    while (1)
    {
        if (cur->key == key)
        {
            break;
        }
        else if (cur == t->nil)
        {
            return NULL;
        }
        cur = branch_off(cur, key);
    }
    return cur;
}

node_t *rbtree_min(const rbtree *t)
{
    return subNodeMin(t, t->root);
}

node_t *rbtree_max(const rbtree *t)
{
    return subNodeMax(t, t->root);
}

int rbtree_erase(rbtree *t, node_t *p)
{
    node_t *x = NULL;
    node_t *y = p;
    color_t y_original_color = y->color;

    if (p->left == t->nil)
    {
        x = p->right;
        transPlant(t, p, p->right);
    }
    else if (p->right == t->nil)
    {
        x = p->left;
        transPlant(t, p, p->left);
    }
    else
    {
        y = subNodeMin(t, p->right);
        y_original_color = y->color;
        x = y->right;
        // 삭제한 노드를 대체할 노드가 자식이라면 이 노드의 자식은 부모가 유지된다.
        if (y->parent == p)
        {
            x->parent = y;
        }
        // 아니라면 이 노드의 자식은 부모가 바뀐다.
        else
        {
            transPlant(t, y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }
        transPlant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }
    if (y_original_color == RBTREE_BLACK)
    {
        erase_fix_up(t, x);
    }

    free(p);
    return 0;
}
void erase_fix_up(rbtree *t, node_t *x)
{
    node_t *w;
    while (x != t->root && x->color == RBTREE_BLACK)
    {
        if (x == x->parent->left)
        {
            w = x->parent->right;
            // 경우1
            if (w->color == RBTREE_RED)
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                leftRotate(t, x->parent);
                w = x->parent->right;
            }
            // 경우2 형제의 자손노드 모두 검정 extra black을 부모에게 넘기면서 형제의 색을 바꿔줌.
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
            {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            //
            else
            {
                // 삼각형을 형성하는지 확인
                if (w->right->color == RBTREE_BLACK)
                {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    rightRotate(t, w->left);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                leftRotate(t, x->parent);
                // 종료하기 위해 root로 이동
                x = t->root;
            }
        }
        else
        {
            w = x->parent->left;
            if (w->color == RBTREE_RED)
            {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rightRotate(t, w);
                w = x->parent->left;
            }
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
            {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == RBTREE_BLACK)
                {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    leftRotate(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                rightRotate(t, w);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}
int inorder(key_t *arr, int n, node_t *node, const rbtree *t, const int maxSize)
{
    if (node == t->nil)
        return n;
    if (n >= maxSize)
        return n;
    n = inorder(arr, n, node->left, t, maxSize);
    *(arr + n) = node->key;
    n++;
    n = inorder(arr, n, node->right, t, maxSize);
    return n;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
    inorder(arr, 0, t->root, t, n);
    return 0;
}

node_t *create_node(key_t key)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    // 노드 초기화
    node->color = RBTREE_RED;
    node->parent = NULL;
    node->right = NULL;
    node->left = NULL;
    node->key = key;
    return node;
}

// 다음으로 갈 위치 반환 같거나 작으면 왼쪽, 크면 오른쪽
node_t *branch_off(node_t *cur, key_t key)
{
    if (key > cur->key)
    {
        return cur->right;
    }
    return cur->left;
}

void addNil(node_t *cur, rbtree *tree)
{
    cur->left = tree->nil;
    cur->right = tree->nil;
}

void designateParent(node_t *cur, node_t *parentToBe)
{
    cur->parent = parentToBe;
}

// 회전 관련 함수
// 자식을 매개변수
void rightRotate(rbtree *t, node_t *cur)
{
    node_t *parent = cur->parent;
    cur->parent = parent->parent;
    // 증부모에 연결
    if (cur->parent == t->nil)
    {
        t->root = cur;
    }
    else if (parent->parent->left == parent)
    {
        parent->parent->left = cur;
    }
    else
    {
        parent->parent->right = cur;
    }
    // 현재의 오른쪽이 부모의 왼쪽이 되기 때문에 현재의 오른쪽의 부모를 부모로 설정
    if (cur->right != t->nil)
    {
        cur->right->parent = parent;
    }
    parent->left = cur->right;
    cur->right = parent;
    parent->parent = cur;

    return;
}
// 부모를 매개변수
void leftRotate(rbtree *t, node_t *cur)
{
    node_t *rightChild = cur->right;

    // 증부모에 연결
    rightChild->parent = cur->parent;
    if (rightChild->parent == t->nil)
        t->root = rightChild;
    else if (cur->parent->left == cur)
        cur->parent->left = rightChild;
    else
        cur->parent->right = rightChild;

    // 부모에게 왼쪽을 넘겨줄때 왼쪽의 부모를 부모로 바꾸어 줘야 하는데 nil이라면 바꿔주지 않는다.
    if (rightChild->left != t->nil)
    {
        rightChild->left->parent = cur;
    }
    // 부모 노드 자식으로 하기 전에 왼쪽을 부모의 오른쪽으로 바꾸기
    cur->right = rightChild->left;
    // 자식과 부모의 관계를 바꾸기
    rightChild->left = cur;
    cur->parent = rightChild;

    return;
}

void parentRedChildsBlack(node_t *parent)
{
    parent->color = RBTREE_RED;
    parent->left->color = RBTREE_BLACK;
    parent->right->color = RBTREE_BLACK;

    return;
}

void parentBlackChildsRed(node_t *parent)
{
    parent->color = RBTREE_BLACK;
    parent->left->color = RBTREE_RED;
    parent->right->color = RBTREE_RED;

    return;
}

void printTree(rbtree *t, node_t *cur)
{
    if (cur == t->nil)
    {
        return;
    }
    // printf("p%d ",cur->parent->key);
    printf("%d{%d} ", cur->key, cur->color);
    printTree(t, cur->left);
    printTree(t, cur->right);
    return;
}

void transPlant(rbtree *t, node_t *u, node_t *v)
{
    if (u->parent == t->nil)
    {
        t->root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

node_t *subNodeMin(const rbtree *t, node_t *subNode)
{
    node_t *cur = subNode;
    while (cur->left != t->nil)
    {
        cur = cur->left;
    }
    return cur;
}

node_t *subNodeMax(const rbtree *t, node_t *subNode)
{
    node_t *cur = subNode;
    while (cur->right != t->nil)
    {
        cur = cur->right;
    }
    return cur;
}

// int main(){
//   rbtree* t = new_rbtree();
//   // node_t* p = rbtree_insert(t,1024);
//   // p = rbtree_insert(t,10);
//   int arr[14] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};

//   for (int i = 0; i < 14; i++) {
//     printf("%d\n",i);
//     node_t *p = rbtree_insert(t, arr[i]);
//     node_t *q = rbtree_find(t, arr[i]);
//   }
//   printTree(t,t->root);
//   delete_rbtree(t);
//   // for(int i=0;i<14;i++){
//   //   rbtree_insert(t,arr[i]);
//   // }
//   // printf("\n");
//   // node_t* node;
//   // for(int i=0;i<14;i++){
//   //   printTree(t,t->root);
//   //   node = rbtree_find(t,arr[i]);
//   //   printf("%d\n",node->key);
//   //   rbtree_erase(t,node);
//   // }
//   // printTree(t,t->root);
//   // node_t *node2 = rbtree_find(t,8);
//   // if (node2==NULL){
//   //   printf("null");
//   // }else{
//   //   printf("%d\n",node2->key);
//   // }
//   // rbtree_erase(t,node2);
//   // printTree(t,t->root);
//   // printf("\n");
//   // // printf("%d\n",p->key);
//   // return 0;
// }