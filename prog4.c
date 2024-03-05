#include <stdio.h>
#include <stdlib.h>


struct my_struct {
    int i1, i2;
};

// struct by value
int struct_by_val(struct my_struct a){
    int val =0;
    val = a.i1 + a.i2;
    return val;
}

// struct by ref
int struct_by_ref(struct my_struct *a){
    int val =0;
    val = a->i1 + a->i2;
    return val;
}


// array index, assume idx is valid;
void set_struct_vals (struct my_struct *arr, int idx, int a, int b) {
    arr[idx].i1 = a;
    arr[idx].i2 = b;
}

/*

The next function utilize a simple linked list struct.  I've provided
functions that create and destroy linked lists.

 */

struct ll_node {
    int val;
    struct ll_node *next;
};

struct ll_node* make_sample_list(int *values, int size) {

    if(size == 0)
        return NULL;
    struct ll_node *new_node = malloc(sizeof(struct ll_node));
    (*new_node).val = *values;
    //using recursion isn't most efficient, but it is simple
    struct ll_node* rest_of_list = make_sample_list(values+1, size - 1);
    (*new_node).next = rest_of_list;
    return new_node;
}

void free_list(struct ll_node* head) {
    if(head == NULL)
        return;
    free_list((*head).next);
    free(head);
}

// access a linked list to retrun a value.  Assumes not NULL next!!
int access_list(struct ll_node* head) {
    int val = 0;
    val = head->next->val;
    return val;
}

// helper function to create a list

int main (int argc, char** argv) {
    int idx, a, b;
    struct my_struct struct1 = {1, 2};
    struct my_struct array1[3] = {{19, 65}, {32, 200}, {4, 6}};

    int list1[] = {2,3,1};
    struct ll_node* head = make_sample_list(list1, 3);

    printf("Struct by val %d\n",struct_by_val(struct1));
    printf("Struct by ref %d\n",struct_by_ref(&struct1));
    
    idx = 1;
    a = 4;
    b = 5;
    printf("before set idx %d, i1 %d i2 %d\n",idx,array1[idx].i1,array1[idx].i2);
    set_struct_vals(array1, idx, a, b);
    printf("after set idx %d, i1 %d i2 %d\n",idx,array1[idx].i1,array1[idx].i2);

    printf("access list (head = %p) returns %d\n", head, access_list(head));

    return 0;
}
