#include "list.h"
#include <stdio.h>
struct node {
    int data;
    list_entry_t node_link;
};
typedef struct node node_t;
#define le2node(le, member) to_struct((le), node_t, member)
#define to_struct(ptr,type,member) ((type *)((char*)(ptr) - offsetof(type, member)))
#define offsetof(type, member) ((size_t)(&((type *)0)->member))

void main() {
    node_t nodes[10];
    int i;
    for (i=0; i<10; i++)
        nodes[i].data = i;
    list_entry_t *le = &nodes[0].node_link;
    list_init(le);
    for (i=1; i<10; i++)
        list_add_after(le, &(nodes[i].node_link));
    while ((le=list_next(le))!=&nodes[0].node_link) {
        node_t *n = le2node(le, node_link);
        printf("%d ", n->data);
    }
    printf("\n");
}
