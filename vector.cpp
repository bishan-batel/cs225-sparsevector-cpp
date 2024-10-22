#include "vector.h"

namespace CS225 {
  /* this is the only place where dimension is used */
  std::ostream &operator<<(std::ostream &out, const SparseVector &v) {
    size_t i, last_pos = -1;
    ElementNode *p_e = v.head;
    while (p_e) {
      for (i = last_pos + 1; i < p_e->pos; ++i) out << " " << "0";
      out << " " << p_e->data;
      last_pos = p_e->pos;
      p_e = p_e->next;
    }
    for (i = last_pos + 1; i < v.dimension; ++i) out << " " << "0";

    return out;
  }

  int SparseVector::Get(size_t pos) const {
    for (const ElementNode *curr = head; curr and curr->pos <= pos; curr = curr->next) {
      if (curr->pos == pos) {
        return curr->data;
      }
    }

    return 0;
  }

  void SparseVector::Insert(int val, size_t pos) {
    ElementNode *position = head;
    ElementNode *prev = NULL;

    /* if the value to set is empty then just delete the element*/
    if (val == 0) {
      Delete(pos);
      return;
    }

    /* if list is empty*/
    if (not position) {
      head = new ElementNode(val, pos, NULL);
      return;
    }

    /* if the first element is the position to insert at */
    if (position->pos == pos) {
      position->data = val;
      return;
    }

    if (position->pos > pos) {
      head = new ElementNode(val, pos, position);

      return;
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
      return;
    }

    prev->next = new ElementNode(val, pos, position);
  }

  void SparseVector::Delete(size_t pos) {
    ElementNode *previous = NULL;
    ElementNode *position = head;

    /* if the given handle is null or the list is empty, return*/
    if (not head) {
      return;
    }

    /* if the first element is the target*/
    if (position->pos == pos) {
      /* store the second element*/
      position = position->next;
      delete head;
      head = position;
      return;
    }

    while (position->next && position->next->pos != pos) {
      position = position->next;
    }

    /* not found*/
    if (not position->next) {
      return;
    }

    /* not found*/
    if (position->next->pos != pos) {
      return;
    }

    previous = position;
    position = position->next;
    previous->next = position->next;
    delete position;
  }

  SparseVector::SparseVector() {
    head = 0;
    dimension = 0;
  }

  SparseVector::SparseVector(const SparseVector &vec) { *this = vec; }

  SparseVector &SparseVector::operator=(const SparseVector &vec) {
    if (&vec == this) return *this;
    free();

    head = NULL;
    dimension = vec.dimension;

    const ElementNode *pos = vec.head;
    while (pos) {
      Insert(pos->data, pos->pos);
      pos = pos->next;
    }

    return *this;
  }

  SparseVector::~SparseVector() { free(); }

  void SparseVector::free() {
    ElementNode *temp = NULL;

    if (head == NULL) return;

    while (head != NULL) {
      temp = head;
      head = head->next;
      delete temp;
    }
  }

  ElementNode::ElementNode(int data, size_t pos, struct ElementNode *next) {
    this->data = data;
    this->pos = pos;
    this->next = next;
  }

  int SparseVector::operator[](size_t pos) const { return Get(pos); }

  ElementProxy SparseVector::operator[](size_t pos) { return ElementProxy(*this, pos); }

  SparseVector SparseVector::operator+(const SparseVector &sp) const {
    SparseVector output;

    const ElementNode *position = head;

    while (position != NULL) {
      output.Insert(position->pos, position->data);
      position = position->next;
    }

    position = sp.head;
    while (position != NULL) {
      output.Insert(position->pos, output[position->pos] + position->data);
      position = position->next;
    }

    return output;
  }

  int SparseVector::operator*(const SparseVector &sp) const {
    int output = 0;
    const ElementNode *position = head;

    while (position != NULL) {
      output += position->data * sp[position->pos];
      position = position->next;
    }

    return output;
  }
  ElementProxy::ElementProxy(SparseVector &v, size_t pos) : v(&v), pos(pos) {}
  ElementProxy::operator int() const { return v->Get(pos); }
  ElementProxy &ElementProxy::operator=(int p) {
    v->Insert(p, pos);

    return *this;
  }
} // namespace CS225
