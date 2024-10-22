#include <cstddef>
#include <ctime>
#include <iostream>

typedef size_t usize;
namespace CS225 {
  class ElementProxy;

  struct ElementNode {
    int data;
    size_t pos;
    struct ElementNode *next;

    ElementNode(int data, size_t pos, struct ElementNode *next = NULL);
  };
  // forward declaration
  // class ElementProxy;
  class SparseVector {
  public:
    SparseVector();

    SparseVector(const SparseVector &vec);

    SparseVector &operator=(const SparseVector &vec);

    ~SparseVector();

    int Get(size_t pos) const;

    void Insert(int val, size_t pos);

    void Delete(size_t pos);

    int operator[](size_t pos) const;

    ElementProxy operator[](size_t pos);

    SparseVector operator+(const SparseVector &sp) const;

    int operator*(const SparseVector &sp) const;

    void PrintRaw() const { // used for grading
      ElementNode *curr = head;
      std::cout << "Raw vector: ";
      while (curr) {
        std::cout << "(" << curr->data << ", " << curr->pos << ")";
        curr = curr->next;
      }
      std::cout << std::endl;
    }

    friend std::ostream &operator<<(std::ostream &out, const SparseVector &v); // implemented
                                                                               //
  private:
    void free();

    ElementNode *head;
    size_t dimension;
  };

  class ElementProxy {
  public:
    ElementProxy(SparseVector &v, size_t pos);

    operator int() const;

    ElementProxy &operator=(int p);

  private:
    SparseVector *v;
    size_t pos;
  };
} // namespace CS225
