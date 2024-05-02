#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum { LEFT, RIGHT } direction_t;

// 트리를 출력하는 함수
void print_rbtree(rbtree *t, node_t *node, int space)
{
  if (node == t->nil)
    return;

  space += 10;
  print_rbtree(t, node->right, space);

  printf("\n");
  for (int i = 10; i < space; i++)
    printf(" ");
  printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

  print_rbtree(t, node->left, space);
}

void rb_transplant(rbtree *t, node_t *del_node, node_t *replace_node){
  // 바꾸려는 노드가 루트일때 
  if(del_node->parent==t->nil){
    t->root=replace_node;
  }
  else if (del_node==del_node->parent->left){
    del_node->parent->left=replace_node;
  }
  else{
    del_node->parent->right=replace_node;
  }
  replace_node->parent=del_node->parent;
}

node_t *node_min(rbtree *t, node_t *node) {
  // TODO: implement find
  while(node->left!=t->nil){
    node = node->left;
  }
  return node;
  //return t->root;
}

node_t *node_max(rbtree *t, node_t *node) {
  // TODO: implement find
  while(node->right!=t->nil){
    node = node->right;
  }
  return node;
  //return t->root;
}

//트리 왼쪽 회전 함수, x는 회전의 기준이 되는 노드
void left_rotate(rbtree *T, node_t *axis_node){
  // printf("왼쪽 \n");
  // x의 오른쪽 노드를 y노드라고 정함(y는 나중에 위로올라감)
  node_t *new_axis_node = axis_node->right;
  // 회전하면 y의 왼쪽노드가 x가 되기 떄문에 y의 왼쪽노드를 x의 오른쪽으로 붙여줌 
  axis_node->right= new_axis_node->left;
  // 만약 붙여준 y의 쪽노드가 nil노드가 아닐경우 해당 노드의 부모를 이어준 x로 변경함
  if(new_axis_node->left!=T->nil){
    new_axis_node->left->parent= axis_node;
  }
  new_axis_node->parent= axis_node->parent;
  // 만약 x가 root라면(부모가 nil노드) y를 root로 만들어줌
  // x가 왼쪽이었다면 그 부모의 왼쪽을,오른쪽이었다면 그 부모의 오른쪽을 y로 만든다.
  if(axis_node->parent==T->nil){
    T->root= new_axis_node;
  }
  else if(axis_node==axis_node->parent->left){
    axis_node->parent->left=new_axis_node;
  }
  else{
    axis_node->parent->right=new_axis_node;
  }
  // y의 왼쪽을 x로, x의 부모를 y로 만든다.
  new_axis_node->left=axis_node;
  axis_node->parent=new_axis_node;
}

//트리 오른쪽 회전 함수, y는 회전의 기준이 되는 노드
void right_rotate(rbtree *T, node_t *axis_node){
  // printf("오른쪽 \n");
  // y의 왼쪽 노드를 x노드라고 정함(y는 나중에 위로올라감)
  node_t *new_axis_node = axis_node->left;
  // 회전하면 x의 오른쪽노드가 y가 되기 떄문에 x의 오른쪽노드를 y의 왼쪽으로 붙여줌 
  axis_node->left= new_axis_node->right;
  // 만약 붙여준 x의 오른쪽노드가 nil노드가 아닐경우 해당 노드의 부모를 이어준 y로 변경함
  if(new_axis_node->right!=T->nil){
    new_axis_node->right->parent= axis_node;
  }
  new_axis_node->parent= axis_node->parent;
  // 만약 y가 root라면(부모가 nil노드) x를 root로 만들어줌
  // y가 오른쪽이었다면 그 부모의 오른쪽을,왼쪽이었다면 그 부모의 왼쪽을 x로 만든다.
  if(axis_node->parent==T->nil){
    T->root= new_axis_node;
  }
  else if(axis_node==axis_node->parent->right){
    axis_node->parent->right=new_axis_node;
  }
  else{
    axis_node->parent->left=new_axis_node;
  }
  // x의 오른쪽을 y로, y의 부모를 x로 만든다.
  new_axis_node->right=axis_node;
  axis_node->parent=new_axis_node;
}

void rotate(rbtree *T, node_t *n,int direction){
    // printf("회전 들어옴\n");
  if(direction==LEFT){
    left_rotate(T,n);
  }
  else{
    right_rotate(T,n);
  }
}

void rb_delete_fixup(rbtree *t, node_t *extra_black_node){
  // printf("지운거 고치러 들어옴\n");
  while(extra_black_node!=t->root && extra_black_node->color==RBTREE_BLACK){
    if(extra_black_node==extra_black_node->parent->left){
      node_t *bro = extra_black_node->parent->right;
      if(bro->color==RBTREE_RED){
        bro->color=RBTREE_BLACK;
        extra_black_node->parent->color= RBTREE_RED;
        rotate(t,extra_black_node->parent,LEFT);
        bro = extra_black_node->parent->right;
      }
      if(bro->left->color==RBTREE_BLACK &&bro->right->color==RBTREE_BLACK){
        bro->color=RBTREE_RED;
        extra_black_node=extra_black_node->parent;
      }
      else{
        if(bro->left->color==RBTREE_RED && bro->right->color==RBTREE_BLACK){
          bro->left->color=RBTREE_BLACK;
          bro->color=RBTREE_RED;
          rotate(t,bro,RIGHT);
          bro= extra_black_node->parent->right;
        } 
        bro->color = extra_black_node->parent->color;
        bro->right->color=RBTREE_BLACK;
        bro->parent->color=RBTREE_BLACK;
        rotate(t,extra_black_node->parent,LEFT);
        extra_black_node= t->root;
      }
    }
    else{
      node_t *bro = extra_black_node->parent->left;
      if(bro->color==RBTREE_RED){
        bro->color=RBTREE_BLACK;
        extra_black_node->parent->color= RBTREE_RED;
        rotate(t,extra_black_node->parent,RIGHT);
        bro = extra_black_node->parent->left;
      }
      if(bro->right->color==RBTREE_BLACK &&bro->left->color==RBTREE_BLACK){
        bro->color=RBTREE_RED;
        extra_black_node=extra_black_node->parent;
      }
      else{
        if(bro->right->color==RBTREE_RED && bro->left->color==RBTREE_BLACK){
          bro->right->color=RBTREE_BLACK;
          bro->color=RBTREE_RED;
          rotate(t,bro,LEFT);
          bro= extra_black_node->parent->left;
        } 
        bro->color = extra_black_node->parent->color;
        bro->left->color=RBTREE_BLACK;
        bro->parent->color=RBTREE_BLACK;
        rotate(t,extra_black_node->parent,RIGHT);
        extra_black_node= t->root;
      }
    }
  }
  extra_black_node->color=RBTREE_BLACK;
}

void rb_delete(rbtree *t, node_t *del_node){
  printf("\n");
  printf("지우러 들어옴: %d\n",del_node->key);
  node_t *replace_node =del_node;
  node_t *del_son_node=NULL;
  color_t replace_node_ori_color = replace_node->color;
  if(del_node->left== t->nil){
    //printf("왼쪽자식 없음\n");
    del_son_node = del_node->right;
    rb_transplant(t,del_node,del_node->right);
  }
  else if(del_node->right==t->nil){
    //printf("오른쪽자식 없음\n");
    del_son_node = del_node->left;
    rb_transplant(t,del_node,del_node->left);
  }
  else{
    //printf("양쪽자식 다있음\n");
    replace_node=node_min(t,del_node->right);
    //printf("오른쪽에서 제일 작은놈: %d\n",y->key);
    replace_node_ori_color= replace_node->color;
    del_son_node= replace_node->right;
    if (replace_node->parent==del_node){
      del_son_node->parent=replace_node;
    }
    else{
      rb_transplant(t,replace_node,replace_node->right);
      replace_node->right= del_node->right;
      replace_node->right->parent= replace_node;
    }
    rb_transplant(t,del_node,replace_node);
    replace_node->left=del_node->left;
    replace_node->left->parent= replace_node;
    replace_node->color= del_node->color;
  }
  if(replace_node_ori_color== RBTREE_BLACK){
    rb_delete_fixup(t,del_son_node);
  }
  // print_rbtree(t, t->root, 0);
}



// 삽입후 트리를 수정하는 코드
void insertFix(rbtree *t, node_t *z){
  // printf("삽입한 %d 수정작업 시작 \n",z->key);
  node_t *y;
  while(z->parent->color==RBTREE_RED){
    // printf("부모가 레드임 \n");
    // 부모가 조부모의 왼쪽일때
    if(z->parent==z->parent->parent->left){
      y = z->parent->parent->right; // 부모의 형제노드 초기화
      // 케이스1
      if(y->color==RBTREE_RED){
        // printf("삼촌도 레드라서 조부모로 올림 \n");
        z->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        z->parent->parent->color= RBTREE_RED;
        z= z->parent->parent;
      }
      else{ // 케이스 2
        if(z == z->parent->right){
          z=z->parent;
          rotate(t,z,LEFT);        
        }
        //케이스 3
        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        rotate(t,z->parent->parent,RIGHT);
      } 
    }
    else{
      y = z->parent->parent->left;
      if(y->color==RBTREE_RED){
        z->parent->color=RBTREE_BLACK;
        y->color=RBTREE_BLACK;
        z->parent->parent->color= RBTREE_RED;
        z= z->parent->parent;
      }
      else{
        if(z == z->parent->left){
          z=z->parent;
          rotate(t,z,RIGHT);        
        }
        z->parent->color=RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        rotate(t,z->parent->parent,LEFT);
      } 
    }
  }
  t->root->color=RBTREE_BLACK;
  
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // 공용 nil을 위한 nil 초기화
  p->nil=(node_t *)calloc(1, sizeof(node_t));;
  p->nil->color=RBTREE_BLACK;
  // 빈 트리라서 root == nil
  p->root = p->nil;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // 삽입할 노드 생성
  printf("\n");
  printf("삽입: %d \n",key);
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key=key;

  node_t *y = t->nil;
  node_t *x = t->root;
  while(x != t->nil){
    // printf("root 아님: %d(%d)\n",x->key,x->color);
    y=x;
    if(z->key<x->key){
      // printf("z:%d < x:%d 왼쪽임\n",z->key,x->key);
      x=x->left;
    }
    else{
      // printf("z:%d > x:%d 오른쪽임\n",z->key,x->key);
      x= x->right;
    }
  }
  // printf("리프노드 찾음\n");
  z->parent= y;
  if(y == t->nil){
    // 루트일떄 
    t->root=z;
  }
  else if(z->key<y->key){
    y->left=z;
  }
  else {
    y->right=z;
  }
  z->left= t->nil;
  z->right= t->nil;
  z->color= RBTREE_RED;
  
  insertFix(t,z);
  // print_rbtree(t, t->root, 0);
  return t->root;
}



node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  printf("\n");
  printf("탐색 들어옴 찾는거: %d\n",key);
  node_t * rbNode = t->root;
  while(rbNode!=t->nil){
    // printf("이번꺼: %d\n",rbNode->key);
    if (rbNode->key==key){
      printf("발견함\n");
      return rbNode;
    }
    else if (rbNode->key>key){
      // printf("왼쪽인듯\n");
      rbNode=rbNode->left;
    }
    else{
      // printf("오른쪽인듯\n");
      rbNode=rbNode->right;
    }
  }
  printf("%d 못찾음\n",key);
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *nd= t->root;
  while(nd->left!=t->nil){
    nd = nd->left;
  }
  return nd;
}



node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *nd= t->root;
  while(nd->right!=t->nil){
    nd = nd->right;
  }
  return nd;
}



int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // root삭제시
  // if(t->root==p){
  //   t->root= t->nil;
  //   free(p);
  // }
  rb_delete(t,p);
   
  return 0;
}

int inorder_rbtree(const rbtree *t, key_t *arr,node_t *nd , int i){
  if (nd== t->nil){
    return i;
  }
  i = inorder_rbtree(t,arr,nd->left,i);
  arr[i++]=nd->key;

  i = inorder_rbtree(t,arr,nd->right,i++);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *nd=t->root;   
  key_t *arrTemp= arr; 
  inorder_rbtree(t,arr,nd,0);
  arr=arrTemp;
  return 0;
}


// int main()
// { ()
//   const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
//   const size_t n = sizeof(arr) / sizeof(arr[0]);()
//   rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
//   int key;
//   for (int i = 0; i < n; i++) {
//     node_t *p = rbtree_insert(t, arr[i]);
//     print_rbtree(t, t->root, 0);
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }