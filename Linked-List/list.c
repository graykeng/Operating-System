#include "list.h"
#include <stdio.h>

// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or 
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

static List array_List[LIST_MAX_NUM_HEADS];
static Node array_Node[LIST_MAX_NUM_NODES];

static List* current_list_address = &array_List[0];
static List* last_list_address = &array_List[LIST_MAX_NUM_HEADS -1];
static int current_node = 0;
static int number_of_list_created = 0;

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    // create 1st list
    if (current_list_address != NULL){
        // if first time create, create MAX heads
        if (number_of_list_created == 0){
            // no more execute this code
            number_of_list_created++;
            // create the first list
            array_List[0].list_index = 0;
            array_List[0].next_list = &array_List[1];
            array_List[0].prev_list = NULL;
            // create the middle lists
            for (int i = 1; i < LIST_MAX_NUM_HEADS - 1; i++){
                array_List[i].list_index = i;
                array_List[i].next_list = &array_List[i+1];
                array_List[i].prev_list = &array_List[i-1];
            }
            // create the last list
            array_List[LIST_MAX_NUM_HEADS-1].list_index = LIST_MAX_NUM_HEADS - 1;
            array_List[LIST_MAX_NUM_HEADS-1].next_list = NULL;
            array_List[LIST_MAX_NUM_HEADS-1].prev_list = &array_List[LIST_MAX_NUM_HEADS-2];
            // initial the first list
            array_List[0].current = NULL;
            array_List[0].head = NULL;
            array_List[0].tail = NULL;
            array_List[0].length = 0;
            array_List[0].sb = 0;
            current_list_address = current_list_address->next_list;
            return &array_List[0];
        }
        else{
            // second and more creations
            current_list_address->current = NULL;
            current_list_address->head = NULL;
            current_list_address->tail = NULL;
            current_list_address->length = 0;
            current_list_address->sb = 0;
            List* returnList = current_list_address;
            current_list_address = current_list_address->next_list;
            return returnList;
        }
    }
    else{
        return NULL;
    }
}

// Returns the number of items in pList.
int List_count(List* pList){
    return pList->length;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    if (pList->length == 0){
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->head;

    return pList->current->data;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    if (pList->length == 0){
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->tail;

    return pList->current->data;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    if (pList->current == NULL){
        if (pList->sb == 1){
            return NULL;
        }
        else{
            pList->current = pList->head;
            return pList->current->data;
        }
    }
    else if (pList->current->next_node == NULL){
        pList->sb = LIST_OOB_END;
        pList->current = pList->current->next_node;
        return NULL;
    }
    pList->current = pList->current->next_node;
    return pList->current->data;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    if (pList->current == NULL){
        if (pList->sb == 0){
            return NULL;
        }
        else{
            pList->current = pList->tail;
            return pList->current->data;
        }
    }
    else if (pList->current->prev_node == NULL){
        pList->sb = LIST_OOB_START;
        pList->current = pList->current->prev_node;
        return NULL;
    }
    pList->current = pList->current->prev_node;
    return pList->current->data;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    if (pList->current == NULL){
        return NULL;
    }
    return pList->current->data;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem){
    // new a node with the pItem
    if (current_node < LIST_MAX_NUM_NODES){
        if (pList->current == NULL){
            if (pList->sb == 0){
                List_prepend(pList, pItem);
                return 0;
            }
            if (pList->sb == 1){
                List_append(pList, pItem);
                return 0;
            }
        }
        else if (pList->current->next_node == NULL){
            List_append(pList, pItem);
            return 0;
        }
        else{
            array_Node[current_node].data = pItem;
            array_Node[current_node].prev_node = pList->current;
            array_Node[current_node].next_node = pList->current->next_node;
            array_Node[current_node].node_index = pList->list_index;

            pList->current->next_node->prev_node = &array_Node[current_node];
            pList->current->next_node = &array_Node[current_node];
            
            pList->current = &array_Node[current_node];

            current_node++;
            pList->length++;
            return 0;
        }
    }
    else{
        return -1;
    }
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem){
    if (current_node < LIST_MAX_NUM_NODES){
        if (pList->current == NULL){
            if (pList->sb == 0){
                List_prepend(pList, pItem);
                return 0;
            }
            if (pList->sb == 1){
                List_append(pList, pItem);
                return 0;
            }
        }
        else if (pList->current->prev_node == NULL){
            List_prepend(pList, pItem);
            return 0;
        }
        else{
            array_Node[current_node].data = pItem;
            array_Node[current_node].prev_node = pList->current->prev_node;
            array_Node[current_node].next_node = pList->current;
            array_Node[current_node].node_index = pList->list_index;

            pList->current->prev_node->next_node = &array_Node[current_node];
            pList->current->prev_node = &array_Node[current_node];
            
            pList->current = &array_Node[current_node];

            current_node++;
            pList->length++;
            return 0;
        }
    }
    else{
        return -1;
    }
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    if (current_node < LIST_MAX_NUM_NODES){
        // if the list is empty
        if (pList->length == 0){
            array_Node[current_node].data = pItem;
            array_Node[current_node].prev_node = NULL;
            array_Node[current_node].next_node = NULL;
            array_Node[current_node].node_index = pList->list_index;
            pList->head = &array_Node[current_node];
            pList->tail = &array_Node[current_node];
            pList->current = &array_Node[current_node];
            current_node++;
            pList->length++;
            return 0;
        }
        else{
            array_Node[current_node].data = pItem;
            array_Node[current_node].prev_node = pList->tail;
            array_Node[current_node].next_node = NULL;
            array_Node[current_node].node_index = pList->list_index;
            pList->tail->next_node = &array_Node[current_node];
            pList->tail = &array_Node[current_node];
            pList->current = pList->tail;
            current_node++;
            pList->length++;
            return 0;
        }
    }
    else{
        return -1;
    }
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
    if (current_node < LIST_MAX_NUM_NODES){
        // if the list is empty
        if (pList->length == 0){
            array_Node[current_node].data = pItem;
            array_Node[current_node].prev_node = NULL;
            array_Node[current_node].next_node = NULL;
            array_Node[current_node].node_index = pList->list_index;
            pList->head = &array_Node[current_node];
            pList->tail = &array_Node[current_node];
            pList->current = &array_Node[current_node];
            current_node++;
            pList->length++;
            return 0;
        }
        else{
            array_Node[current_node].data = pItem;
            array_Node[current_node].prev_node = NULL;
            array_Node[current_node].next_node = pList->head;
            array_Node[current_node].node_index = pList->list_index;
            pList->head->prev_node = &array_Node[current_node];
            pList->head = &array_Node[current_node];
            pList->current = pList->head;
            current_node++;
            pList->length++;
            return 0;
        }
    }
    else{
        return -1;
    }
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    Node* last_node = &array_Node[current_node - 1];
    Node* removed = pList->current;
    void* currentDataForReturn = pList->current->data;
    Node* fianl_current;

    // Remember the current.next
    if(removed->next_node == last_node){
        fianl_current = removed;
    }
    else{
        fianl_current = removed->next_node;
    }
    // removed == last
    if (removed == NULL){
        return NULL;
    }
    if (removed == last_node){
        // skip swap
        // remove
        // removed == head or tail
        if (removed == pList->head || removed == pList->tail){
            if (removed == pList->head){
                pList->head = removed->next_node;
            }
            else if (removed == pList->tail){
                pList->tail = removed->prev_node;
            }
        }
        // removed's prev or next != NULL
        if (removed->next_node != NULL){
            removed->next_node->prev_node = removed->prev_node;
        }
        if (removed->prev_node != NULL){
            removed->prev_node->next_node = removed->next_node;
        }
        // no specific situation
        // pList->current->data = pList->current->next_node->data;
        pList->current = pList->current->next_node;
        // removed == NULL
        if (removed == NULL){
            pList->sb = LIST_OOB_END;
        }
        pList->length--;
        current_node--;
        return currentDataForReturn;
    }
    // removed->last
    else if (removed->next_node == last_node){
        // if current is head or last is tail
        if (removed == pList->head || last_node == array_List[last_node->node_index].tail){
            // if current is head and last is tail
            if (removed == pList->head && last_node == array_List[last_node->node_index].tail){
                // current's next points to NULL
                removed->next_node = NULL;
                // current's prev points to last
                removed->prev_node = last_node;
                // last's next points to current
                last_node->next_node = removed;
                // last's prev points to NULL
                last_node->prev_node = NULL;
                // reset the head
                pList->head = last_node;
                // reset the tail
                pList->tail = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is head and last is not tail
            else if (removed == pList->head && last_node != array_List[last_node->node_index].tail){
                // save third node
                Node* right = last_node->next_node;
                // current's next points to third node in the list
                removed->next_node = last_node->next_node;
                // current's prev points to last
                removed->prev_node = last_node;
                // last's next points to current
                last_node->next_node = removed;
                // last's prev points to NULL
                last_node->prev_node = NULL;
                // third node points to current
                right->prev_node = removed;
                // reset the head
                pList->head = last_node;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is not head and last is tail
            else if (removed != pList->head && last_node == array_List[last_node->node_index].tail){
                // save the third node from back
                Node* left = removed->prev_node;
                // current's next points to NULL
                removed->next_node = NULL;
                // current's prev points to last
                removed->prev_node = last_node;
                // last's next points to current
                last_node->next_node = removed;
                // last's prev points to third from back
                last_node->prev_node = left;
                // left's connection
                left->next_node = last_node;
                // reset the tail
                pList->tail = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
        }
        // removed->last, and they're not head or tail
        else{
            // save the left and right node
            Node* left = removed->prev_node;
            Node* right = last_node->next_node;
            // removed's connections
            removed->next_node = right;
            removed->prev_node = last_node;
            // last's connections
            last_node->next_node = removed;
            last_node->prev_node = left;
            // right node's connections
            right->prev_node = removed;
            // left node's connections
            left->next_node = last_node;
            // reset the node_indexs
            Node* removed_2 = removed;
            removed->node_index = last_node->node_index;
            last_node->node_index = removed_2->node_index;
            // reset the current (last thing to do)
            removed->data = last_node->data;
            last_node->data = removed_2->data;
            removed = last_node;
        }
    }
    // last->removed
    else if(removed->prev_node == last_node){
        // if current is tail or last is head
        if (removed == pList->tail || last_node == array_List[last_node->node_index].head){
            // if current is tail and last is head
            if (removed == pList->tail && last_node == array_List[last_node->node_index].head){
                // removed's connections
                removed->next_node = last_node;
                removed->prev_node = NULL;
                // last's connections
                last_node->prev_node = removed;
                last_node->next_node = NULL;
                // reset head and tail
                pList->head = removed;
                pList->tail = last_node;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is tail and last is not head
            else if (removed == pList->tail && last_node != array_List[last_node->node_index].head){
                // save the third from the back
                Node* left = last_node->prev_node;
                // removed's connections
                removed->next_node = last_node;
                removed->prev_node = left;
                // last's connections
                last_node->next_node = NULL;
                last_node->prev_node = removed;
                // left node's connections
                left->next_node = removed;
                // reset the tail
                pList->tail = last_node;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is not tail and last is head
            else if (removed != pList->tail && last_node == array_List[last_node->node_index].head){
                // save the third node
                Node* right = removed->next_node;
                // removed 's connections
                removed->next_node = last_node;
                removed->prev_node = NULL;
                // last's connections
                last_node->next_node = right;
                last_node->prev_node = removed;
                // right node's connection
                right->prev_node = last_node;
                // reset the head
                pList->head = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
        }
        // last->removed, and they're not head or tail
            else {
                // save the left and right
                Node* left = last_node->prev_node;
                Node* right = removed->next_node;
                // removed's connections
                removed->next_node = last_node;
                removed->prev_node = left;
                // last's connections
                last_node->next_node = right;
                last_node->prev_node = removed;
                // left node's connection
                left->next_node = removed;
                // right node's connection
                right->prev_node = last_node;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
    }
    // no relationship
    else{
        // if current or last are head or tail
        if (removed == pList->head || last_node == array_List[last_node->node_index].head || removed == pList->tail || last_node == array_List[last_node->node_index].tail){
            // if current is head and last is head
            if (removed == pList->head && last_node == array_List[last_node->node_index].head){
                // save removed's next and last's next
                Node* removed_right = removed->next_node;
                Node* last_right = last_node->next_node;
                // removed's connection
                removed->next_node = last_right;
                // last's connection
                last_node->next_node = removed_right;
                // removed_next's connection
                removed_right->prev_node = last_node;
                // last_next's connection
                last_right->prev_node = removed;
                // reset the heads
                pList->head = last_node;
                array_List[last_node->node_index].head = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is tail and last is tail
            else if (removed == pList->tail && last_node == array_List[last_node->node_index].tail){
                // save the removed's prev and last's prev
                Node* removed_left = removed->prev_node;
                Node* last_left = last_node->prev_node;
                // removed's connection
                removed->prev_node = last_left;
                // last's connection
                last_node->prev_node = removed_left;
                // removed_prev's connection
                removed_left->next_node = last_node;
                // last_prev's connection
                last_left->next_node = removed;
                // reset the tails
                pList->tail = last_node;
                array_List[last_node->node_index].tail = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is tail and last is head
            else if (removed == pList->tail && last_node == array_List[last_node->node_index].head){
                // save the removed_prev and last_next
                Node* removed_left = removed->prev_node;
                Node* last_right = last_node->next_node;
                // removed's connections
                removed->next_node = last_right;
                removed->prev_node = NULL;
                // last's connections
                last_node->next_node = NULL;
                last_node->prev_node = removed_left;
                // removed_prev's connection
                removed_left->next_node = last_node;
                // last_next's connection
                last_right->prev_node = removed;
                // reset pList's tail
                pList->tail = last_node;
                // reset array[]'s head
                array_List[last_node->node_index].head = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is head and last is tail
            else if (removed == pList->head && last_node == array_List[last_node->node_index].tail){
                // save the removed_next and last_prev
                Node* removed_right = removed->next_node;
                Node* last_left = last_node->prev_node;
                // removed's connections
                removed->next_node = NULL;
                removed->prev_node = last_left;
                // last's connections
                last_node->next_node = removed_right;
                last_node->prev_node = NULL;
                // removed_next's connection
                removed_right->prev_node = last_node;
                // last_prec's connection
                last_left->next_node = removed;
                // reset pList's head
                pList->head = last_node;
                // reset array[]'s tail
                array_List[last_node->node_index].tail = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is not both and last is head
            else if (removed != pList->head && removed != pList->tail && last_node == array_List[last_node->node_index].head){
                // save
                Node* removed_left = removed->prev_node;
                Node* removed_right = removed->next_node;
                Node* last_right = last_node->next_node;
                // removed's connection
                removed->next_node = last_right;
                removed->prev_node = NULL;
                // last's connection
                last_node->next_node = removed_right;
                last_node->prev_node = removed_left;
                // removed_left and removed_right and last_right's connections
                removed_left->next_node = last_node;
                removed_right->prev_node = last_node;
                last_right->prev_node = removed;
                // reset array[]'s head
                array_List[last_node->node_index].head = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is not both and last is tail
            else if (removed != pList->head && removed != pList->tail && last_node == array_List[last_node->node_index].tail){
                // save
                Node* removed_left = removed->prev_node;
                Node* removed_right = removed->next_node;
                Node* last_left = last_node->prev_node;
                // removed's connections
                removed->next_node = NULL;
                removed->prev_node = last_left;
                // last's connections
                last_node->next_node = removed_right;
                last_node->prev_node = removed_left;
                // removed_left and removed right and last_left's connections
                removed_left->next_node = last_node;
                removed_right->prev_node = last_node;
                last_left->next_node = removed;
                // reset array[]'s tail
                array_List[last_node->node_index].tail = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is head and last is not both
            else if (removed == pList->head && last_node != array_List[last_node->node_index].head && last_node != array_List[last_node->node_index].tail){
                // save
                Node* removed_right = removed->next_node;
                Node* last_left = last_node->prev_node;
                Node* last_right = last_node->next_node;
                // removed's connections
                removed->next_node = last_right;
                removed->prev_node = last_left;
                // last's connections
                last_node->next_node = removed_right;
                last_node->prev_node = NULL;
                // removed_right and last_left and last_right's connections
                removed_right->prev_node = last_node;
                last_left->next_node = removed;
                last_right->prev_node = removed;
                // reset pList's head
                pList->head = last_node;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current is tail and last is not both
            else if (removed == pList->tail && last_node != array_List[last_node->node_index].head && last_node != array_List[last_node->node_index].tail){
                // save
                Node* removed_left = removed->prev_node;
                Node* last_left = last_node->prev_node;
                Node* last_right = last_node->next_node;
                // removed's connections
                removed->next_node = last_right;
                removed->prev_node = last_left;
                // last's connections
                last_node->next_node = NULL;
                last_node->prev_node = removed_left;
                // removed_left and last_left and last_right's connections
                removed_left->next_node = last_node;
                last_left->next_node = removed;
                last_right->prev_node = removed;
                // reset pList's tail
                pList->tail = last_node;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
            // if current and last are not both
            else{
                // save
                Node* removed_left = removed->prev_node;
                Node* removed_right = removed->next_node;
                Node* last_left = last_node->prev_node;
                Node* last_right = last_node->next_node;
                // removed's connections
                removed->next_node = last_right;
                removed->prev_node = last_left;
                // last's connections
                last_node->next_node = removed_right;
                last_node->prev_node = removed_left;
                // removed_left, removed_right, last_left, last_right's connections
                removed_left->next_node = last_node;
                removed_right->prev_node = last_node;
                last_left->next_node = removed;
                last_right->prev_node = removed;
                // reset the node_indexs
                Node* removed_2 = removed;
                removed->node_index = last_node->node_index;
                last_node->node_index = removed_2->node_index;
                // reset the current (last thing to do)
                removed->data = last_node->data;
                last_node->data = removed_2->data;
                removed = last_node;
            }
        }
    }
    // remove
    // removed == head or tail
    if (removed == pList->head || removed == pList->tail){
        if (removed == pList->head){
            pList->head = removed->next_node;
        }
        else if (removed == pList->tail){
            pList->tail = removed->prev_node;
        }
    }
    // removed's prev or next != NULL
    if (removed->next_node != NULL || removed->prev_node != NULL){
        if (removed->next_node != NULL){
            removed->next_node->prev_node = removed->prev_node;
        }
        if (removed->prev_node != NULL){
            removed->prev_node->next_node = removed->next_node;
        }
    }

    pList->current = fianl_current;
    // removed == NULL
    if (removed == NULL){
        pList->sb = LIST_OOB_END;
    }
    pList->length--;
    current_node--;
    return currentDataForReturn;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    if (pList->length != 0){
        Node* temp = pList->tail;
        List_last(pList);
        List_remove(pList);
        pList->current = pList->tail;
        return temp->data;
    }
    else{
        return NULL;
    }
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.

// incompleted
void fun(void* pItem){
    pItem = NULL;
}

void List_concat(List* pList1, List* pList2){
    FREE_FN ii = fun;
    if (pList1->length == 0 || pList2->length == 0){
        if (pList1->length == 0 && pList2->length == 0){
            printf("List1 and List2 are empty");
        }
        else if (pList1->length == 0 && pList2->length != 0){
            pList1->head = pList2->head;
            pList1->tail = pList2->tail;
            pList1->current = pList1->head;
            pList1->length = pList2->length;
            pList2->length = 0;
            List_free(pList2, ii);
        }
        else if (pList1->length != 0 && pList2->length == 0){
            List_free(pList2, ii);
        }
    }
    else{
        pList1->tail->next_node = pList2->head;
        pList2->head->prev_node = pList1->tail;
        pList1->tail = pList2->tail;
        pList1->length += pList2->length;
        pList2->length = 0;
        List_free(pList2, ii);
    }
    
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    List_last(pList);
    int l = pList->length;
    for (int i = 0; i < l; i++){
        pItemFreeFn(pList->current->data);
        List_trim(pList);
    }
    if(pList->prev_list != NULL){
        pList->prev_list->next_list = pList->next_list;
    }
    if(pList->next_list != NULL){
        pList->next_list->prev_list = pList->prev_list;
    }

    last_list_address->next_list = pList;
    pList->prev_list = last_list_address;
    pList->next_list = NULL;

    last_list_address = pList;

    if(current_list_address == NULL){
        current_list_address = last_list_address;
    }
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    for (int i = 0; i < pList->length; i++){
        if (pList->current != NULL){
            bool checker = pComparator(pList->current->data, pComparisonArg);
            if (checker){
                return pList->current->data;
            }
            else{
                List_next(pList);
            }
        }
        else{
            pList->sb = LIST_OOB_END;
            return NULL;
        }
    }
}
