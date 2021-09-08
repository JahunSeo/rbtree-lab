#include "rbtree.h"

// #include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

/* 
 * 추가 함수 입출력값 정의
 */


// 중위 트리 순회
void rbtree_inorder_walk(const node_t *x);

// rbtree_rotate_left:
void rbtree_rotate_left(rbtree *t, node_t *x);

// rbtree_rotate_right:
void rbtree_rotate_right(rbtree *t, node_t *x);

// rbtree_insert_fixup:
void rbtree_insert_fixup(rbtree *t, node_t *z);

// rbtree_transplant:
void rbtree_transplant(rbtree *t, node_t *v1, node_t *v2);

// rbtree_successor:
node_t *rbtree_successor(rbtree *t, node_t *x);

// rbtree_delete_fixup:
void rbtree_delete_fixup(rbtree *t, node_t *x);

// delete_node: node에 할당된 메모리 반환하기
void delete_node(node_t *);


// 전역 변수로 NIL 초기화 //
node_t _NIL = {
    .color = RBTREE_BLACK,
    .left = NULL, // 변경되는 상황 없음
    .right = NULL, // 변경되는 상황 없음
    .parent = NULL, // 변경될 수 있음!
};
node_t *NIL = &_NIL;

/* 
 * main 함수
 */
// int main(void) {
//     printf("Hello, RBTree!\n");

//     node_t* nodes[10];
//     int values[10] = {5, 12, 10, 14, 20, 83, 65, 1, 99, 78};
//     int delete_result;
//     node_t* found;

//     // 트리 생성
//     rbtree *t = new_rbtree();
//     // 비어 있는 트리 확인
//     printf("rbtree : ");
//     rbtree_inorder_walk(t->root);
//     printf("\n\n");

//     for (int i=0; i<5; i++) {
//         nodes[i] = rbtree_insert(t, values[i]);
//         printf("rbtree : ");
//         rbtree_inorder_walk(t->root);
//         printf("\n");
//     }

//     for (int i=0; i<5; i++) {
//         delete_result = rbtree_erase(t, nodes[i]);
//         printf("rbtree : ");
//         rbtree_inorder_walk(t->root);    
//         printf("\n");
//     }

//     return 0;
// }

/* 
 * rbtree 함수 구현하기 
 */
rbtree *new_rbtree(void) {
    rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
    return t;
}

void delete_node(node_t *x) {
  if (x == NULL) {
    return;
  }
  delete_node(x->left);
  delete_node(x->right);
  free(x);
}

void delete_rbtree(rbtree *t) {
    // 재귀적으로 모든 노드의 메모리 반환하기
    delete_node(t->root);
    // 트리의 메모리 반환하기
    free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // 리턴할 노드를 루트로 초기화
  node_t *node = t->root;
  // 조건에 맞는 노드를 찾을 때까지 반복
  while (node != NULL && key != node->key) {
    if (key < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return node;
}


node_t *rbtree_min(const rbtree *t) {
  node_t *node = t->root;
  // 빈 트리인 경우, 루트 리턴
  if (node == NULL) {
    return t->root;
  }
  // 빈 트리가 아닌 경우, 왼쪽 자식을 계속 따라 내려감
  while (node->left != NULL) {
    node = node->left;
  }
  return node;
}


node_t *rbtree_max(const rbtree *t) {
  node_t *node = t->root;
  // 빈 트리인 경우, 루트 리턴
  if (node == NULL) {
    return t->root;
  }
  // 빈 트리가 이닌 경우, 오른쪽 자식을 계속 따라 내려감
  while (node->right != NULL) {
    node = node->right;
  }
  return node;
}


int rbtree_to_array(const rbtree *t, key_t *key, const size_t size) {
  // TODO:
  return 0;
}



void rbtree_inorder_walk(const node_t *x) {
    if (x != NULL) {
        rbtree_inorder_walk(x->left);
        int parent_key;
        char color;
        if (x->parent != NULL) {
            parent_key = x->parent->key;
        } else {
            parent_key = 0;
        }
        if (x->color == RBTREE_RED) {
            color = 'R';
        } else {
            color = 'B';
        }
        printf(" %d>[%d](%c) ", parent_key, x->key, color);
        rbtree_inorder_walk(x->right);
    }
}


void rbtree_rotate_left(rbtree *t, node_t *x) {
    // 1. 'y' 설정: 기준이 되는 x의 오른쪽 자식
    node_t *y = x->right;
    // 2. 'y'의 왼쪽 서브 트리를 'x'의 오른쪽 서브 트리로 변경
    // - x 관점
    x->right = y->left;
    // - y의 왼쪽 자식 관점
    if (y->left != NULL) {
        y->left->parent = x;
    } 
    // 3. 'y'를 'x 부모'의 자식으로 변경
    // - y 관점
    y->parent = x->parent;
    // - x 부모 관점
    //   - x가 루트인 경우
    if (x->parent == NULL) {
        t->root = y;
    //   - x가 왼쪽 자식인 경우
    } else if (x == x->parent->left) {
        x->parent->left = y;
    //   - x가 오른쪽 자식인 경우
    } else {
        x->parent->right = y;
    }
    // 4. 'x'를 'y'의 자식으로 변경: x는 y의 왼쪽 자식
    // - x 관점
    x->parent = y;
    // - y 관점
    y->left = x;
}


void rbtree_rotate_right(rbtree *t, node_t *x) {
    // 1. 'y' 설정: 기준이 되는 x의 왼쪽 자식
    node_t *y = x->left;
    // 2. 'y'의 오른쪽 서브 트리를 'x'의 왼쪽 서브 트리로 변경
    // - x 관점
    x->left = y->right;
    // - y 의 오른쪽 자식 관점
    if (y->right != NULL) {
        y->right->parent = x;
    }
    // 3. 'y'를 'x 부모'의 자식으로 변경
    // - y 관점
    y->parent = x->parent;
    // - x 부모 관점
    //   - x 부모가 없는 경우 (즉, x가 루트인 경우)
    if (x->parent == NULL) {
        t->root = y;
    //   - x가 왼쪽 자식인 경우
    } else if (x == x->parent->left) {
        x->parent->left = y;
    //   - x가 오른쪽 자식인 경우
    } else {
        x->parent->right = y;
    }
    // 4. 'x'를 'y'의 자식으로 변경: x는 y의 오른쪽 자식
    // - x 관점
    x->parent = y;
    // - y 관점
    y->right = x;
}


void rbtree_insert_fixup(rbtree *t, node_t *z) {
    // [주의] 노드의 NULL 체크
    // - 경계노드 NIL을 도입하면 NIL의 컬러가 BLACK이기 때문에 아래 2개 상황에서 코드가 간결해짐
    // 1) while (z->parent != NULL && z->parent->color == RBTREE_RED) {
    //   - z->parent 가 NIL인 경우, 컬러가 BLACK이어서 두 번째 조건만으로도 처리 가능
    // 2) if (uncle != NULL && uncle->color == RBTREE_RED) {
    //   - uncle이 NIL인 경우, 컬러가 BLACK이어서 두 번째 조건만으로 처리 가능

    // 1. 트리 규칙을 위반한 영역 수정하기
    // - 'z'는 트리 규칙에 문제가 생길 수 있는 노드이며, 그 시작은 트리에 새로 추가된 노드
    // - 'z'-'z 부모'가 RED-RED 인지 확인하여 규칙을 유지할 수 있도록 트리의 구조 변경
    // - (주의! 경계노드 NIL을 별도로 활용하지 않으므로 부모 노드 NULL 여부 체크 필요)
    node_t *uncle;    
    while (z->parent != NULL && z->parent->color == RBTREE_RED) {
        // 'z 부모'가 왼쪽 자식인 경우
        if (z->parent == z->parent->parent->left) {
            // 'z 부모의 형제(=z 삼촌)' 설정: 할아버지의 오른쪽 자식
            uncle = z->parent->parent->right;
            // [CASE 1] 'z 삼촌'이 RED인 경우
            // - 부모~삼촌이 RED~RED 이므로, 할아버지는 반드시 BLACK
            // - 부모와 삼촌을 BLACK, 할아버지를 RED로 바꾸면, 일단 z와 부모 간의 문제는 해결
            // - 하지만 할아버지가 RED로 변하여 본인의 부모와 없던 문제가 생길 수 있으므로,
            // - z를 'z의 부모 부모(=할아버지)'로 업데이트
            // - (주의! 경계노드 NIL을 별도로 활용하지 않으므로 uncle 노드 NULL 여부 체크 필요)
            if (uncle != NULL && uncle->color == RBTREE_RED) {
                // 부모와 삼촌을 BLACK으로
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                // 할아버지를 RED로
                z->parent->parent->color = RBTREE_RED;
                // 새로운 타겟 노드로 z 변경
                z = z->parent->parent;
            // [CASE 2,3] 'z 삼촌'이 BLACK인 경우
            // - 부모~삼촌이 RED~BLACK 이므로, 할아버지는 반드시 BLACK
            // - 이 때는 '할아버지' 위치를 BLACK으로 유지하면서 '삼촌' 위치를 RED로 만들어 문제 해결
            // - (한쪽으로 치우친 RED를 반대편으로 넘겨주는 셈)
            } else {
                // [CASE 2] 'z'가 오른쪽 자식인 경우
                // - 이 경우 할아버지-부모-z가 < 모양으로 꺾여 있음
                // - 할아버지-부모-z가 일직선에 있는 CASE 3 상태로 먼저 만듦
                if (z == z->parent->right) {
                    z = z->parent;
                    rbtree_rotate_left(t, z);
                }
                // [CASE 3] 'z'가 왼쪽 자식인 경우
                // - 할아버지-부모-z가 / 모양으로 일직선 위에 있음
                // - RED가 한 쪽으로 치우쳐 있으므로, 할아버지를 기준으로 반대편에 RED를 하나 넘겨주어 균형을 맞춤
                // - 이를 위해 할아버지를 RED, 부모를 BLACK으로 바꾼 뒤, 할아버지를 기준으로 오른쪽 회전함
                // - 그러면 부모 BLACK에 두 자식이 RED인 형태로 균형이 맞게 됨
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rbtree_rotate_right(t, z->parent->parent);
                // CASE 2,3 혹은 CASE 3을 수행한 이후에는
                // z-부모가 RED-BLACK 이므로 루프 종료
            }
        // 'z 부모'가 오른쪽 자식인 경우
        } else {
            // 'z 삼촌' 설정: 할아버지의 왼쪽 자식
            uncle = z->parent->parent->left;
            // [CASE 1] 'z 삼촌'이 RED 인 경우
            if (uncle != NULL && uncle->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            // [CASE 2,3] 'z 삼촌'이 BLACK 인 경우
            } else {
                // [CASE 2] 'z'가 왼쪽 자식인 경우 
                // - 할아버지-부모-z가 > 모양으로 꺾여 있음
                if (z == z->parent->left) {
                    z = z->parent;
                    rbtree_rotate_right(t, z);
                }
                // [CASE 3] 'z'가 오른쪽 자식인 경우
                // - 할아버지-부모-z가 \ 모양으로 일직선 위에 있음
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                rbtree_rotate_left(t, z->parent->parent);
            }
        } 

    }
    // 2. 루트 노드 BLACK으로 설정하기
    t->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
    printf("[rbtree_insert] %d\n", key);
    // 1. 새로운 노드 생성하기
    node_t *new_n = (node_t *)malloc(sizeof(node_t));
    new_n->key = key;
    new_n->left = new_n->right = NULL;
    new_n->color = RBTREE_RED;
    // 2. 트리에서 들어갈 장소 찾기
    // - 새로운 노드와 그 부모 노드 초기회
    node_t *parent_n = NULL;
    node_t *child_n = t->root;
    // - 리프 노드에 도달할 때까지 이동 
    while (child_n != NULL) {
        // - 부모 노드 위치 업데이트
        parent_n = child_n;
        // - 새로운 노드 위치 업데이트
        if (new_n->key < child_n->key) {
            child_n = child_n->left;
        } else {
            child_n = child_n->right;
        }
    }
    // 3. 트리에 새로운 노드 배치하기
    // - 새로운 노드에 부모 노드 표시
    new_n->parent = parent_n;
    // - 부모 노드에 새로운 노드 표시
    //   - 부모 노드가 비어 있는 경우 (즉 새로운 노드가 루트인 경우) 
    if (parent_n == NULL) {
        t->root = new_n;
    //   - 부모 노드가 있고 새로운 노드가 왼쪽 자식인 경우
    } else if (new_n->key < parent_n->key) {
        parent_n->left = new_n;
    //   - 부모 노드가 있고 새로운 노드가 오른쪽 자식인 경우
    } else {
        parent_n->right = new_n;
    }
    // 4. RB트리 특성 유지시키기
    /* DEBUGGING */
    // printf("before fixup : ");
    // rbtree_inorder_walk(t->root);
    // printf("\n");
    /* ** ** ** */

    rbtree_insert_fixup(t, new_n);

    /* DEBUGGING */
    // printf("after fixup : ");
    // rbtree_inorder_walk(t->root);
    // printf("\n");
    /* ** ** ** */

    // 5. 새로운 노드의 주소 리턴하기
    return new_n;
}


void rbtree_transplant(rbtree* t, node_t* v1, node_t* v2) {
    // 노드 v1을 노드 v2로 대체하기 (v1의 부모와 v2를 연결하기)
    // - (주의) 이 때 v2가 NULL일 수 있음
    // - 'v1 부모' 관점
    //   - v1이 루트인 경우
    if (v1->parent == NULL) {
        t->root = v2;
    //   - v1이 왼쪽 자식인 경우
    } else if (v1 == v1->parent->left) {
        v1->parent->left = v2;
    //   - v1이 오른쪽 자식인 경우
    } else {
        v1->parent->right = v2;
    }
    // - v2 관점
    if (v2 != NULL) {
        v2->parent = v1->parent;
    }
}


// rbtree_successor:
node_t *rbtree_successor(rbtree *t, node_t *x) {
    // 직후 원소 찾기
    // 본인의 오른쪽 자식이 있는 경우
    // - 오른쪽 서브 트리에서 가장 작은 노드
    if (x->right != NULL) {
        node_t* child = x->right;
        while (child->left != NULL) {
            child = child->left;
        }
        return child;
    }
    // 본인의 오른쪽 자식이 없는 경우 (bstree 맥락애서는 쓰이지 않음)
    // - 왼쪽 위로 올라가다 처음 오른쪽 위로 꺾였을 때의 첫 노드
    node_t* parent = x->parent;
    while (parent != NULL && x == parent->right) {
        x = parent;
        parent = parent->parent;
    }
    return parent;
}


// rbtree_node_or_nil: x가 없는 경우, 더미 노드 NIL을 리턴함
node_t* rbtree_node_or_nil(node_t *x) {
    if (x == NULL) {
        return NIL;
    } else {
        return x;
    }
}


// rbtree_delete_fixup:
void rbtree_delete_fixup(rbtree *t, node_t *x) {
    // x의 형제 노드 sibling
    node_t *sib, *sib_left, *sib_right;
    // 여분의 BLACK 노드를 위쪽으로 이동시킴
    // - x가 루트가 아니므로 x의 parent는 반드시 있음
    while (x != t->root && x->color == RBTREE_BLACK) {
        // x가 왼쪽 자식인 경우
        if (x == x->parent->left) {
            // 형제 노드는 부모의 오른쪽 자식
            // - sibling이 없는 경우 발생 가능? sib->color를 RED로 설정하는 걸 보면, 발생 불가능할 것 같긴 한데..
            //  (x와 sib가 모두 NIL이더라도 x->parent만 활용하고 수정할 상황이 없으므로 문제 없음)
            sib = x->parent->right; // rbtree_node_or_nil(x->parent->right);
            // [CASE 1] 형제가 RED인 경우
            // - x의 sibling을 BLACK으로 만들어 줌
            if (sib->color == RBTREE_RED) {
                sib->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rbtree_rotate_left(t, x->parent);
                sib = x->parent->right;
            }
            // [CASE 2] 형제가 BLACK이고 형제의 두 자식이 모두 BLACK인 경우
            // - 이 경우는 빈 공간을 위로 올리기 때문에 루트에 도달할 때까지 반복될 수 있음
            // - (형제의 자식들이 NULL일 수 있을까?)
            sib_left = rbtree_node_or_nil(sib->left);
            sib_right = rbtree_node_or_nil(sib->right);
            if (sib_left->color == RBTREE_BLACK && sib_right->color == RBTREE_BLACK) {
                sib->color = RBTREE_RED;
                x = x->parent;
            } else {
                // [CASE 3] 형제가 BLACK이고 형제의 오른쪽 자식만 BLACK인 경우
                // - 형제의 왼쪽 자식은 RED
                if (sib_right->color == RBTREE_BLACK) {
                    sib_left->color = RBTREE_BLACK;
                    sib->color = RBTREE_RED;
                    rbtree_rotate_right(t, sib);
                    sib = x->parent->right;
                } 
                // [CASE 4] 형제가 BLACK이고 형제의 오른쪽 자식이 RED인 경우
                // - 형제의 왼쪽 자식은 RED, BLACK 둘 다 가능
                sib->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                sib->right->color = RBTREE_BLACK;
                rbtree_rotate_left(t, x->parent);
                x = t->root;
            }
        // x가 오른쪽 자식인 경우
        } else {
            // 형제 노드는 부모의 왼쪽 자식
            sib = x->parent->left;
            // [CASE 1]
            if (sib->color == RBTREE_RED) {
                sib->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                rbtree_rotate_right(t, x->parent);
                sib = x->parent->left;
            }
            // [CASE 2] 형제 BLACK, 형제 두 자식 모두 BLACK
            sib_left = rbtree_node_or_nil(sib->left);
            sib_right = rbtree_node_or_nil(sib->right);
            if (sib_left->color == RBTREE_BLACK && sib_right->color == RBTREE_BLACK) {
                sib->color = RBTREE_RED;
                x = x->parent;
            } else {
                // [CASE 3] 형제 BLACK, 형제의 왼쪽 자식 BLACK
                // - 형제의 오른쪽 자식은 RED
                if (sib_left->color == RBTREE_BLACK) {
                    sib->right->color = RBTREE_BLACK;
                    sib->color = RBTREE_RED;
                    rbtree_rotate_left(t, sib);
                    sib = x->parent->left;
                }
                // [CASE 4] 형제 BLACK, 형제의 왼쪽 자식 RED
                // - 형재의 오른쪽 자식은 RED, BLACK 둘 다 가능
                sib->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                sib->left->color = RBTREE_BLACK;
                rbtree_rotate_right(t, x->parent);
                x = t->root;
            }
        }
    }
    // x의 색깔을 BLACK으로 설정
    x->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *z) {
    printf("[rbtree_erase] %d\n", z->key);
    // 삭제될 노드 y
    node_t* y = z;
    // 삭제될 노드의 본래 색깔 저장
    color_t y_original_color = y->color;
    // y 자리를 채울 노드 x
    node_t* x;
    // [CASE 1] 삭제될 노드의 왼쪽 자식이 없는 경우
    // - 오른쪽 자식으로 빈 자리를 채움 (이 때 오른쪽 자식이 NULL일 수도 있음)
    if (z->left == NULL) {
        // z의 오른쪽 자식으로 x를 설정
        // - (실험적 코드) z의 오른쪽 자식이 비어 있는 경우, 더미 노드 NIL로 설정
        x = rbtree_node_or_nil(z->right);
        // z의 부모와 x를 연결
        rbtree_transplant(t, z, x);
        // z에 할당된 메모리 반환하기
        free(z);
    // [CASE 2] 삭제될 노드의 오른쪽 자식이 없는 경우
    // - 왼쪽 자식으로 빈 자리를 채움 (이 때는 왼쪽 자식이 반드시 있음)
    } else if (z->right == NULL) {
        // z의 왼쪽 자식으로 x를 설정
        // - (실험적 코드) z의 왼쪽 자식이 비어 있는 경우, 더미 노드 NIL로 설정
        x = rbtree_node_or_nil(z->left);
        // z의 부모와 x를 연결
        rbtree_transplant(t, z, x);
        // z에 할당된 메모리 반환하기
        free(z);
    // [CASE 3] 삭제될 노드의 양쪽 자식이 모두 있는 경우
    } else {
        // 삭제될 노드의 자리로 이동할 노드 y 업데이트
        // - 삭제될 노드의 직후 원소이며 NULL일 수 없음 
        y = rbtree_successor(t, z);
        // 이동할 노드 y의 본래 색깔 저장
        y_original_color = y->color;
        // y 자리를 채울 노드 x 업데이트
        // - y의 오른쪽 자식으로 설정
        // - y는 z의 오른쪽 서브트리에서 최소 노드이므로, y의 왼쪽 자식은 반드시 비어 있음
        x = rbtree_node_or_nil(y->right);
        // [CASE 3-1] y가 z의 오른쪽 자식인 경우
        // - z y x가 일직선에 놓인 상황
        if (y->parent == z) {
            // y와 x를 연결
            // - 다른 상황에서는 transplant 함수를 통해 y의 부모에 x를 연결하지만, 
            // - 이 상황에서는 y의 부모가 z여서 삭제되기 때문에 y에 연결함
            // - x가 NIL인 상황을 위해 명시적으로 처리
            x->parent = y;
        // [CASE 3-2] y가 z의 오른쪽 자식이 아닌 경우
        } else {
            // y의 부모와 x를 연결 (y를 빼냄)
            rbtree_transplant(t, y, x);
            // z의 오른쪽 서브트리를 y의 오른쪽 자식으로 연결
            y->right = z->right;
            y->right->parent = y;
        }
        // [CASE 3-1, 3-2] 공통
        // z의 부모와 y를 연결
        rbtree_transplant(t, z, y);
        // z의 왼쪽 서브트리를 y의 왼쪽 자식으로 연결
        y->left = z->left;
        y->left->parent = y;
        // y의 색깔을 기존 z의 색깔로 변경
        y->color = z->color;
        // z에 할당된 메모리 반환하기: z가 삭제되고 빈 공간은 y 자리에 발생
        free(z);

        // [ DEPRECATED ] CASE 3-1, 3-2 통일 시도
        // - 폐기 이유: 노드의 주소값을 유지해야하는 경우 활용할 수 없음
        // - 하지만 엄청 간결하기 때문에 기록 목적으로 남겨둠
        // // z 자리로 y를 이동: 키값만 변경하는 방식으로 간접적으로 대체
        // z->key = y->key;
        // // y의 부모와 x를 연결: y 빼내기
        // //   - CASE 3-1의 경우, y의 부모는 z
        // //   - CASE 3-2의 경우, y의 부모는 z의 오른쪽 서브 트리 중 하나
        // rbtree_transplant(t, y, x);
        // // y에 할당된 메모리 반환하기: z가 삭제되고 빈 공간은 y 자리에 발생
        // free(y);
    }
    // 삭제 혹은 이동으로 발생한 빈 공간에 본래 검은 노드가 있었다면,
    // 트리 높이에 불균형이 발생한 것이므로 fixup 실행
    if (y_original_color == RBTREE_BLACK) {
        rbtree_delete_fixup(t, x);
    }
    // NIL과의 연결을 끊어주기
    // - 가령 CASE 1에서 z의 부모와 NIL이 연결되었다면, 
    //   z의 부모가 NIL을 가리키는 간선을 NULL로 변경해 주어야 함
    // - (주의!) NIL의 parent가 없을 수 있으므로 체크
    // 루트가 NIL이 된 경우
    if (t->root == NIL) {
      t->root = NULL;
    // NIL이 자식으로 설정된 노드가 있는 경우
    } else if (NIL->parent != NULL) {
      // NIL 부모 관점
      // - NIL이 왼쪽 자식
      if (NIL->parent->left == NIL) {
            NIL->parent->left = NULL;
      // - NIL이 오른쪽 자식
      } else {
          NIL->parent->right = NULL;
      }
      // NIL 관점
      NIL->parent = NULL;
    }

    return 1;
}
