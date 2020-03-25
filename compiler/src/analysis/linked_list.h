#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listnode
{
    const char *key;
    const char *value;
    struct listnode* next;
} listnode;

typedef void (*callback)(listnode* data);


/**
 *  create a new listnode
 *  initialize the data and next field
 *
 *  return the newly created listnode
*/
listnode *LLcreate(const char *key, const char *value, listnode* next)
{
    listnode* new_listnode = (listnode*)malloc(sizeof(listnode));
    new_listnode->key = key;
    new_listnode->value = value;
    new_listnode->next = next;

    return new_listnode;
}

/**
 *  add a new listnode at the beginning of the list
 */
listnode *LLprepend(listnode* head, const char *key, const char *value)
{
    listnode* new_listnode = LLcreate(key, value, head);
    head = new_listnode;
    return head;
}

/**
 *  add a new listnode at the end of the list
 */
listnode *LLappend(listnode* head, const char * key, const char *value)
{
    if(head == NULL) return NULL;
    /* go to the last listnode */
    listnode *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    /* create a new listnode */
    listnode* new_listnode =  LLcreate(key, value, NULL);
    cursor->next = new_listnode;

    return head;
}

/*
 *  traverse the linked list
*/
void LLtraverse(listnode* head, callback f)
{
    listnode* cursor = head;
    while(cursor != NULL)
    {
        f(cursor);
        cursor = cursor->next;
    }
}

/**
 *  remove listnode from the front of list
 */
listnode *LLremove_front(listnode* head)
{
    if ( head == NULL) return NULL;
    listnode *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last listnode in the list */
    if (front == head) head = NULL;
    free(front);

    return head;
}

/**
 *  remove listnode from the back of the list
 */
listnode *LLremove_back(listnode* head)
{
    if(head == NULL)
        return NULL;

    listnode *cursor = head;
    listnode *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }

    if(back != NULL)
        back->next = NULL;

    /* if this is the last listnode in the list*/
    if(cursor == head)
        head = NULL;

    free(cursor);

    return head;
}

/**
 *  remove a listnode from the list
 */
listnode *LLremove_any(listnode* head, listnode* nd)
{
    if(nd == NULL) return NULL;
    /* if the listnode is the first listnode */
    if(nd == head) return LLremove_front(head);

    /* if the listnode is the last listnode */
    if(nd->next == NULL)
        return LLremove_back(head);

    /* if the listnode is in the middle */
    listnode* cursor = head;
    while(cursor != NULL)
    {
        if(cursor->next == nd)
            break;
        cursor = cursor->next;
    }

    if(cursor != NULL)
    {
        listnode* tmp = cursor->next;
        cursor->next = tmp->next;
        tmp->next = NULL;
        free(tmp);
    }
    return head;
}

/**
 *  display a listnode
 */
void LLdisplay(listnode* cursor)
{
        while (cursor!=NULL)
    {
        printf("%s: %s\n", cursor->key, cursor->value);
        cursor = cursor->next;
    }
}

/**
 *  Search for a specific listnode with input data
 *
 *  return the first matched listnode that stores the input data,
 *  otherwise return NULL
 */
listnode *LLsearch(listnode* head, const char * key)
{

    listnode *cursor = head;
    while (cursor!=NULL)
    {
        if (strcmp ( cursor->key, key) == 0) return cursor;

        cursor = cursor->next;
    }
    return NULL;
}

/**
 *  remove all element of the list
 */
void LLdispose(listnode *head)
{
    listnode *cursor, *tmp;

    if (head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while (cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}
/**
 *  return the number of elements in the list
 */
int LLcount(listnode *head)
{
    listnode *cursor = head;
    int c = 0;
    while (cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}

/**
 *  reverse the linked list
 */
listnode *LLreverse(listnode* head)
{
    listnode* prev    = NULL;
    listnode* current = head;
    listnode* next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
}
