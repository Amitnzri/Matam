//
// Created by tomer on 5/3/2019.
//
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct node_t {
    int x;
    struct node_t *next;
} *Node;
typedef enum {
    SUCCESS=0,
    MEMORY_ERROR,
    EMPTY_LIST,
    UNSORTED_LIST,
    NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
void destroyList(Node list);




ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut) {

    if (!mergedOut) return NULL_ARGUMENT;
    if (!getListLength(list1)||!getListLength(list2)) return EMPTY_LIST;
    if (!isListSorted(list1) || !isListSorted(list2)) return UNSORTED_LIST;

    Node current_node = malloc(sizeof(*current_node)) ;
    if(!current_node) {
        destroyList(*mergedOut);
        return MEMORY_ERROR;
    }
    *mergedOut = current_node;
    if(list1->x <= list2->x){
        current_node->x = list1->x;
        list1 = list1->next;
    }else{
        current_node->x = list2->x;
        list2 = list2->next;
    }

    while(list1&&list2){
        Node new_node = malloc(sizeof(*new_node));
        if(!new_node) {
            destroyList(*mergedOut);
            return MEMORY_ERROR;
        }
        current_node->next = new_node;
        current_node = new_node;
        if(list1->x <= list2->x){
            current_node->x = list1->x;
            list1 = list1->next;
        }else{
            current_node->x = list2->x;
            list2 = list2->next;
        }
    }

    if(!list1){
        while(list2){
            Node new_node = malloc(sizeof(*new_node));
            if(!new_node) {
                destroyList(*mergedOut);
                return MEMORY_ERROR;
            }
            current_node->next = new_node;
            current_node = new_node;
                current_node->x = list2->x;
                list2 = list2->next;
        }
    }else{
        while(list1){
            Node new_node = malloc(sizeof(*new_node));
            if(!new_node) {
                destroyList(*mergedOut);
                return MEMORY_ERROR;
            }
            current_node->next = new_node;
            current_node = new_node;
            current_node->x = list1->x;
            list1 = list1->next;
        }

    }
    return SUCCESS;
}

void destroyList(Node list){
    while(list){
        Node tmp_node = list;
        list = list->next;
        free(tmp_node);
    }
}