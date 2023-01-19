#pragma once

#include <sstream>
#include <iostream>

enum class rbcolor { red = false, blk = true };

struct rbnode_base {
  rbcolor m_color;
  rbnode_base* m_parent;
  rbnode_base* m_lchild;
  rbnode_base* m_rchild;
};

template <typename T>
struct rbnode : public rbnode_base {
  T m_data;

  T& data() const { return (T&)m_data; }

  rbcolor& color() const { return (rbcolor&)m_color; }

  rbnode*& parent() const { return (rbnode*&)m_parent; }

  rbnode*& lchild() const { return (rbnode*&)m_lchild; }

  rbnode*& rchild() const { return (rbnode*&)m_rchild; }

  rbnode* brother() const {
    if (this == parent()->lchild())
      return parent()->rchild();
    else
      return parent()->lchild();
  }

  rbnode* gparent() const { return parent()->parent(); }

  rbnode* uncle() const {
    rbnode* father = parent();
    rbnode* grandfather = gparent();
    if (father == grandfather->lchild())
      return grandfather->rchild();
    else
      return grandfather->lchild();
  }

  bool isRed() const { return m_color == rbcolor::red; }

  bool isBlk() const { return m_color == rbcolor::blk; }

  void setRed() { m_color = rbcolor::red; }

  void setBlk() { m_color = rbcolor::blk; }

};

template <typename T>
class rbtree {
 protected:
  using node = rbnode<T>;

  rbnode_base m_head;
  size_t m_node_count;

 public:
  rbtree() { reset(); }

  ~rbtree() { clear(); }

 public:
  bool insert(const T& value) {
    node* pos = root();
    node* pos_parent = head();
    while (pos != nullptr) {
      pos_parent = pos;
      if (value == pos->data())
        return false;
      else if (value < pos->data())
        pos = pos->lchild();
      else
        pos = pos->rchild();
    }
    __insert(pos_parent, value);
    ++m_node_count;
    return true;
  }

  bool erase(const T& value) {
    node* pos = root();
    while (pos != nullptr) {
      if (value == pos->data()) {
        __erase(pos);
        --m_node_count;
        return true;
      } 
      if (value < pos->data())
        pos = pos->lchild();
      else
        pos = pos->rchild();
    }
    return false;
  }

  void clear() {
    if (!empty()) {
      __destroy(root());
      reset();
    }
  }

  size_t size() const
  { return m_node_count; }

  bool empty() const
  { return size() == 0; }

  void disp() const {
    if (!empty())
      __disp(root());
    std::cout << std::endl;
  }

 protected:
  node* head() const
  { return (node*)(&m_head); }

  node*& root() const
  { return (node*&)m_head.m_parent; }

  node*& leftmost() const
  { return (node*&)m_head.m_lchild; }

  node*& rightmost() const
  { return (node*&)m_head.m_rchild; }

  node* create_node(const T& x) { 
    node* tmp = (node*)malloc(sizeof(node));
    try {
       new (&tmp->m_data) T(x);
    } catch (...) {
      free((void*)tmp);
    }
    return tmp;
  }

  void destroy_node(node* p) {
    (&p->m_data)->~T();
    free(p);
  }
  
  void reset() {
    m_head.m_color = rbcolor::red;
    root() = nullptr;
    leftmost() = head();
    rightmost() = head();
    m_node_count = 0;
  }

 protected:
  void __leftRotate(node* x) {
    node* rchild = x->rchild();
    x->rchild() = rchild->lchild();
    if (rchild->lchild() != nullptr)
      rchild->lchild()->parent() = x;
    if (x == root())
      root() = rchild;
    else if (x == x->parent()->lchild())
      x->parent()->lchild() = rchild;
    else 
      x->parent()->rchild() = rchild;
    rchild->parent() = x->parent();
    rchild->lchild() = x;
    x->parent() = rchild;
  }

  void __rightRotate(node* x) {
    node* lchild = x->lchild();
    x->lchild() = lchild->rchild();
    if (lchild->rchild() != nullptr)
      lchild->rchild()->parent() = x;
    if (x == root())
      root() = lchild;
    else if (x == x->parent()->lchild())
      x->parent()->lchild() = lchild;
    else 
      x->parent()->rchild() = lchild;
    lchild->parent() = x->parent();
    lchild->rchild() = x;
    x->parent() = lchild;
  }

  node* __get_successor(node* x) {
    if (x == head()) {
      x = x->lchild();
    } else if (x->rchild() != nullptr) {
      node* y = x->rchild();
      while (y->lchild() != nullptr)
        y = y->lchild();
      x = y;
    } else {
      node* y = x->parent();
      while (x == y->rchild()) {
        x = y;
        y = y->parent();
      }
      /**
       * 假如最开始传入的参数 x 是根节点，并且 x 的右孩子为空，
       * 我们期望 x 的 successor 应该是头节点 (类似于循环链表)。
       * 下面这个 if 语句就是为了保证这种期望的。
       */
      if (x->rchild() != y)
        x = y;
    }
    return x;
  }

  void __disp(node* rt) const {
    if (rt->isRed())
      std::cout << "\033[1;31m" << rt->m_data;
    else 
      std::cout << "\033[1;34m" << rt->m_data;
    std::cout << "\033[0m";
    if (rt->lchild() || rt->rchild()) {
      std::cout << '(';
      if (rt->lchild())
        __disp(rt->lchild());
      if (rt->rchild()) {
        std::cout << ", ";
        __disp(rt->rchild());
      }
      std::cout << ')';
    }
  }

  void __destroy(node* pos) {
    while (pos != nullptr) {
      __destroy(pos->rchild());
      node* tmp = pos->lchild();
      destroy_node(pos);
      pos = tmp;
    }
  }

  void __insert(node* pos_parent, const T& value) {
    node* new_node = create_node(value);
    if (pos_parent == head()) {
      leftmost() = rightmost() = root() = new_node;
      new_node->setBlk();
      new_node->parent() = pos_parent;
      new_node->lchild() = nullptr;
      new_node->rchild() = nullptr;
      return;
    } else if (value < pos_parent->data()) {
      pos_parent->lchild() = new_node;
      if (pos_parent == leftmost())
        leftmost() = new_node;
    } else {
      pos_parent->rchild() = new_node;
      if (pos_parent == rightmost())
        rightmost() = new_node;
    }
    new_node->parent() = pos_parent;
    new_node->lchild() = nullptr;
    new_node->rchild() = nullptr;
    __preFixInsert(new_node);
  }

  void __erase(node* pos) {
    node* successor;
    if (pos->lchild() == nullptr && pos->rchild() == nullptr) {
      successor = nullptr;
      if (pos == root()) {
        root() = nullptr;
        leftmost() = head();
        rightmost() = head();
      } else {
        if (pos == pos->parent()->lchild())
          pos->parent()->lchild() = successor;
        else
          pos->parent()->rchild() = successor;
        
        if (pos == leftmost())
          leftmost() = pos->parent();
        if (pos == rightmost())
          rightmost() = pos->parent();
        
        if (pos->isBlk()) {
          node* suc_parent = pos->parent();
          __preFixErase(successor, suc_parent);
        }
      }
    } else if (pos->lchild() != nullptr && pos->rchild() != nullptr) {
      successor = __get_successor(pos);
      pos->data() = std::move(successor->data());
      __erase(successor);
      return;
    } else {
      if (pos->lchild() == nullptr)
        successor = pos->rchild();
      else
        successor = pos->lchild();

      if (pos == root()) {
        root() = successor;
        leftmost() = successor;
        rightmost() = successor;
        successor->setBlk();
      } else {
        if (pos == pos->parent()->lchild())
          pos->parent()->lchild() = successor;
        else
          pos->parent()->rchild() = successor;
        successor->parent() = pos->parent();
        if (pos == leftmost())
          leftmost() = successor;
        if (pos == rightmost())
          rightmost() = successor;
        
        node* suc_parent = successor->parent();
        __preFixErase(successor, suc_parent);
      }
    }
    destroy_node(pos);
  }

  void __preFixInsert(node* new_node) {
    new_node->setRed();
    if (new_node->parent()->isRed())
      __fixInsert(new_node);
  }

  void __preFixErase(node* successor, node* suc_parent) {
    if (successor != nullptr && successor->isRed()) {
      successor->setBlk();
      return;
    }
    __fixErase(successor, suc_parent);
  }

  void __fixInsert(node* new_node) {
    node* parent = new_node->parent();
    node* gparent = new_node->gparent();
    node* uncle = new_node->uncle();
    while (true) {
      if (parent == gparent->lchild()) {
        if (uncle == nullptr || uncle->isBlk()) {
          if (new_node == parent->rchild()) {
            __leftRotate(parent);
            new_node = parent;
            parent = new_node->parent();
          }
          parent->setBlk();
          gparent->setRed();
          __rightRotate(gparent);
          return;
        } else {
          parent->setBlk();
          uncle->setBlk();
                      
          if (gparent == root())
            return;
          gparent->setRed();
          if (gparent->parent()->isBlk())
            return;
          new_node = gparent;
          parent = new_node->parent();
          gparent = new_node->gparent();
          uncle = new_node->uncle();
          continue;
        }
      } else {
        if (uncle == nullptr || uncle->isBlk()) {
          if (new_node == parent->lchild()) {
            __rightRotate(parent);
            new_node = parent;
            parent = new_node->parent();
          }
          parent->setBlk();
          gparent->setRed();
          __leftRotate(gparent);
          return;
        } else {
          parent->setBlk();
          uncle->setBlk();
          if (gparent == root())
            return;
          gparent->setRed();
          if (gparent->parent()->isBlk())
            return;
          new_node = gparent;
          parent = new_node->parent();
          gparent = new_node->gparent();
          uncle = new_node->uncle();
          continue;
        }
      }
    }
    return; // never reach.
  }

  void __fixErase(node* successor, node* suc_parent) {
    node* parent = suc_parent;
    node* brother;
    while (true) {
      if (successor == parent->lchild()) {
        brother = parent->rchild();
        if (brother->isRed()) {
          parent->setRed();
          brother->setBlk();
          __leftRotate(parent);
          brother = parent->rchild();
        }
        if ((brother->lchild() == nullptr || brother->lchild()->isBlk()) 
         && (brother->rchild() == nullptr || brother->rchild()->isBlk())) {
          brother->setRed();
          if (parent->isRed()) {
            parent->setBlk();
            return;
          }
          if (parent == root())
            return;
          successor = parent;
          parent = successor->parent();
          brother = successor->brother();
          continue;
        } else {
          if (brother->rchild() == nullptr || brother->rchild()->isBlk()) {
            brother->setRed();
            brother->lchild()->setBlk();
            __rightRotate(brother);
            brother = parent->rchild();
          }
          brother->color() = parent->color();
          parent->setBlk();
          brother->rchild()->setBlk();
          __leftRotate(parent);
          return;
        }
      } else {
        brother = parent->lchild();
        if (brother->isRed()) {
          parent->setRed();
          brother->setBlk();
          __rightRotate(parent);
          brother = parent->lchild();
        }
        if ((brother->rchild() == nullptr || brother->rchild()->isBlk()) 
         && (brother->lchild() == nullptr || brother->lchild()->isBlk())) {
          brother->setRed();
          if (parent->isRed()) {
            parent->setBlk();
            return;
          }
          if (parent == root())
            return;
          successor = parent;
          parent = successor->parent();
          brother = successor->brother();
          continue;
        } else {
          if (brother->lchild() == nullptr || brother->lchild()->isBlk()) {
            brother->setRed();
            brother->rchild()->setBlk();
            __leftRotate(brother);
            brother = parent->lchild();
          }
          brother->color() = parent->color();
          parent->setBlk();
          brother->lchild()->setBlk();
          __rightRotate(parent);
          return;
        }
      }
    }
    return; // never reach.
  }
  
};
