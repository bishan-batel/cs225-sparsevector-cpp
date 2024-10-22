#include "spvector.h"
#include <stdio.h>
#include <stdlib.h>
/* definition of Node is hidden from the client (driver) so that client CANNOT
 * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
 * implementation details.  The main reason for encapsulations -- if I ever
 * decide to change Node struct no client code will break. Example: I decide to
 * change "left" to "Left", all I do is change map.h and map.c, no client code
 * is effected, since clients were FORCED to use getleft(...) function, rather
 * than field name "left".  Also see typedef in map.h
 */
struct ElementNode {
  int data;
  int pos;
  struct ElementNode *next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;

/*print functions*/
void printf_elements(ConstElementNode_handle p_e, char const *fmt, int dim) {
  int i, last_pos = -1;
  while (p_e) {
    for (i = last_pos + 1; i < p_e->pos; ++i) {
      printf(fmt, 0);
    }
    printf(fmt, p_e->data);
    last_pos = p_e->pos;
    p_e = p_e->next;
  }
  for (i = last_pos + 1; i < dim; ++i) {
    printf(fmt, 0);
  }
}

void print_elements(ConstElementNode_handle p_e) {
  while (p_e) {
    printf("%i at pos %i, ", p_e->data, p_e->pos);
    p_e = p_e->next;
  }
}

int insert_element(ElementNode_handle *head, int pos, int val) {
  ElementNode *position = *head;
  ElementNode *prev = NULL;

  /* if the value to set is empty then just delete the element*/
  if (val == 0) {
    delete_element(head, pos);
    return 0;
  }

  /* if list is empty*/
  if (position == NULL) {
    *head = (ElementNode *) malloc(sizeof(ElementNode));

    if (*head != NULL) {
      (*head)->pos = pos;
      (*head)->data = val;
      (*head)->next = NULL;
    }

    return *head == NULL;
  }

  /* if the first element is the position to insert at */
  if (position->pos == pos) {
    position->data = val;
    return 0;
  }

  if (position->pos > pos) {
    *head = (ElementNode *) malloc(sizeof(ElementNode));

    if (*head != NULL) {
      (*head)->pos = pos;
      (*head)->data = val;
      (*head)->next = position;
    }

    return *head == NULL;
  }

  prev = position;
  position = position->next;

  while (position && position->pos < pos) {
    prev = position;
    position = position->next;
  }

  /* if (position != NULL && position->pos == pos) { */
  /* } */

  if (position != NULL && position->pos == pos) {
    position->data = val;
    return 0;
  }

  prev->next = (ElementNode *) malloc(sizeof(ElementNode));

  if (prev->next != NULL) {
    prev->next->pos = pos;
    prev->next->data = val;
    prev->next->next = position;
  }

  return prev->next == NULL;
}

void delete_element(ElementNode_handle *head_handle, int pos) {
  ElementNode *previous = NULL;
  ElementNode *position = *head_handle;

  /* if the given handle is null or the list is empty, return*/
  if (head_handle == NULL || *head_handle == NULL) {
    return;
  }

  /* if the first element is the target*/
  if (position->pos == pos) {
    /* store the second element*/
    position = position->next;

    /* free the previous head*/
    free(*head_handle);

    /* assign the head to the next*/
    *head_handle = position;
    return;
  }

  while (position->next != NULL && position->next->pos != pos) {
    position = position->next;
  }

  /* not found*/
  if (position->next == NULL) {
    return;
  }

  /* not found*/
  if (position->next->pos != pos) {
    return;
  }

  previous = position;
  position = position->next;
  previous->next = position->next;
  free(position);
}

int get(ConstElementNode_handle head, int pos) {
  while (head != NULL) {
    if (head->pos == pos) {
      return head->data;
    }

    head = head->next;
  }

  return 0;
}

int scalar_product(ConstElementNode_handle lhs, ConstElementNode_handle rhs) {
  int output = 0;
  const ElementNode *position = lhs;

  while (position != NULL) {
    output += position->data * get(rhs, position->pos);
    position = position->next;
  }

  return output;
}

ElementNode_handle add(ConstElementNode_handle lhs, ConstElementNode_handle rhs) {
  ElementNode *output = NULL;

  const ElementNode *position = lhs;

  while (position != NULL) {
    insert_element(&output, position->pos, position->data);
    position = position->next;
  }

  position = rhs;
  while (position != NULL) {
    insert_element(&output, position->pos, get(output, position->pos) + position->data);
    position = position->next;
  }

  return output;
}

void free_elements(ElementNode_handle head) {
  ElementNode *temp = NULL;

  if (head == NULL) return;

  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}
