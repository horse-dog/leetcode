#pragma once

#include <memory>
#include <iostream>

enum class rbcolor { red = false, blk = true };

struct rbnode_base {
  rbcolor m_color;
  rbnode_base* m_parent;
  rbnode_base* m_lchild;
  rbnode_base* m_rchild;

  rbcolor& color() { return m_color; }

  rbnode_base*& parent() { return m_parent; }

  rbnode_base*& lchild() { return m_lchild; }

  rbnode_base*& rchild() { return m_rchild; }

  rbnode_base*& sister() {
    if (this == m_parent->m_lchild)
      return m_parent->m_rchild;
    else
      return m_parent->m_lchild;
  }

  rbnode_base*& gparent() { return m_parent->m_parent; }

  rbnode_base*& uncle() {
    rbnode_base* father = parent();
    rbnode_base* grandfather = gparent();
    if (father == grandfather->lchild())
      return grandfather->rchild();
    else
      return grandfather->lchild();
  }

  bool isRed() const { return m_color == rbcolor::red; }

  bool isBlk() const { return m_color == rbcolor::blk; }

  auto setRed() {
    m_color = rbcolor::red;
    return this;
  }

  auto setBlk() {
    m_color = rbcolor::blk;
    return this;
  }

  auto minimum() const {
    auto x = this;
    while (x->m_lchild != nullptr)
      x = x->m_lchild;
    return x;
  }

  auto maximum() const {
    auto x = this;
    while (x->m_rchild != nullptr)
      x = x->m_rchild;
    return x;
  }

  bool isHead() const {
    return isRed() && m_parent->m_parent == this;
  }

  bool isRoot() const {
    return m_parent->isHead();
  }

  auto prev() const {
    auto x = this;
    if (x->isHead()) {
      x = x->m_rchild;
    } else if (x->m_lchild != nullptr) {
      auto y = x->m_lchild;
      while (y->m_rchild != nullptr)
        y = y->m_rchild;
      x = y;
    } else {
      auto y = x->m_parent;
      while (x == y->m_lchild) {
        x = y;
        y = y->m_parent;
      }
      if (x->m_lchild != y)
        x = y;
    }
    return x;
  }

  auto next() const {
    auto x = this;
    if (x->isHead()) {
      x = x->m_lchild;
    } else if (x->m_rchild != nullptr) {
      auto y = x->m_rchild;
      while (y->m_lchild != nullptr)
        y = y->m_lchild;
      x = y;
    } else {
      auto y = x->m_parent;
      while (x == y->m_rchild) {
        x = y;
        y = y->m_parent;
      }
      /**
       * this is for case:
       * Head (y)       Head (x)
       *    |              |
       *    x     ==>      y
       *  /   \          /   \
       *...    nil     ...   nil
       * 
       * in this case, x->m_rchild == y,
       * but we expect current x as the
       * next node. so we execute `x = y`
       * except `x->rchild == y`.
       */
      if (x->m_rchild != y)
        x = y;
    }
    return x;
  }

};

template <typename T>
struct rbnode : public rbnode_base {
  T m_data;

  rbnode*& parent() { return (rbnode*&)m_parent; }

  rbnode*& lchild() { return (rbnode*&)m_lchild; }

  rbnode*& rchild() { return (rbnode*&)m_rchild; }

  rbnode*& sister() {
    if (this == (rbnode*)m_parent->m_lchild)
      return (rbnode*&)m_parent->m_rchild;
    else
      return (rbnode*&)m_parent->m_lchild;
  }

  rbnode*& gparent() { return (rbnode*&)m_parent->m_parent; }

  rbnode*& uncle() {
    rbnode_base* father = parent();
    rbnode_base* grandfather = gparent();
    if (father == grandfather->lchild())
      return (rbnode*&)grandfather->rchild();
    else
      return (rbnode*&)grandfather->lchild();
  }

  bool isRed() const { return m_color == rbcolor::red; }

  bool isBlk() const { return m_color == rbcolor::blk; }

  auto setRed() {
    m_color = rbcolor::red;
    return this;
  }

  auto setBlk() {
    m_color = rbcolor::blk;
    return this;
  }

  auto minimum() const {
    auto x = this;
    while (x->m_lchild != nullptr)
      x = x->m_lchild;
    return x;
  }

  auto maximum() const {
    auto x = this;
    while (x->m_rchild != nullptr)
      x = x->m_rchild;
    return x;
  }

  bool isHead() const {
    return isRed() && m_parent->m_parent == this;
  }

  bool isRoot() const {
    return m_parent->isHead();
  }

  rbnode* prev() const {
    rbnode_base* x = (rbnode_base*)this;
    if (x->isHead()) {
      x = x->m_rchild;
    } else if (x->m_lchild != nullptr) {
      auto y = x->m_lchild;
      while (y->m_rchild != nullptr)
        y = y->m_rchild;
      x = y;
    } else {
      auto y = x->m_parent;
      while (x == y->m_lchild) {
        x = y;
        y = y->m_parent;
      }
      if (x->m_lchild != y)
        x = y;
    }
    return (rbnode*)x;
  }

  rbnode* next() const {
    rbnode_base* x = (rbnode_base*)this;
    if (x->isHead()) {
      x = x->m_lchild;
    } else if (x->m_rchild != nullptr) {
      auto y = x->m_rchild;
      while (y->m_lchild != nullptr)
        y = y->m_lchild;
      x = y;
    } else {
      auto y = x->m_parent;
      while (x == y->m_rchild) {
        x = y;
        y = y->m_parent;
      }
      if (x->m_rchild != y)
        x = y;
    }
    return (rbnode*)x;
  }

};

template <typename T>
class rbtree {
 private:
  rbnode_base m_head;
  size_t m_node_count;

 protected:
  using node = rbnode<T>;

 public:
  rbtree() 
  { reset(); }

  ~rbtree() 
  { clear(); }

 public:
  void insert_equal(const T& v) {
    node* y = head();
    node* x = root();
    while (x != nullptr) {  
      y = x;
      if (v < x->m_data)
        x = x->lchild();
      else
        x = x->rchild();
    }
    return __insert(y, v);
  }

  bool insert_unique(const T& v) {
    node* y = head();
    node* x = root();
    bool comp = true;
    while (x != nullptr) {
      y = x;
      comp = v < x->m_data;
      x = comp ? x->lchild() : x->rchild();
    }
    node* j = y;
    if (comp) { /* v < y->m_data. */
      if (j == leftmost()) {
        __insert(y, v);
        return true;
      } else {
        j = j->prev();
      }
    }
    if (j->m_data < v) {
      __insert(y, v);
      return true;
    }
    return false;
  }

  size_t erase(const T& x);

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

  friend std::ostream& operator<<(std::ostream& os, const rbtree& tree) {
    os << '{';
    auto begin = tree.leftmost();
    auto end = tree.head();
    if (begin != end)
      os << begin->m_data;
    begin = begin->next();
    while (begin != end) {
      os << ", " << begin->m_data;
      begin = begin->next();
    }
    return os << '}';
  }

 protected:
  node* head() const
  { return (node*)(&m_head); }

  node*& root() 
  { return (node*&)m_head.m_parent; }

  node* root() const
  { return (node*)m_head.m_parent; }

  node*& leftmost()
  { return (node*&)m_head.lchild(); }

  node* leftmost() const
  { return (node*)m_head.m_lchild; }

  node*& rightmost()
  { return (node*&)m_head.rchild(); }

  node* rightmost() const
  { return (node*)m_head.m_rchild; }

 protected:
  static node* create_node(const T& x) { 
    node* tmp = (node*)malloc(sizeof(node));
    try {
       new (&tmp->m_data) T(x);
    } catch (...) {
      free((void*)tmp);
    }
    return tmp;
  }

  static void destroy_node(node* p) {
    (&p->m_data)->~T();
    free((void*)p);
  }

  node* lower_bound(const T& x);

  node* upper_bound(const T& x);

  node* copyfrom();

  void movefrom(const rbtree& x) {
    head()->color() = x.head()->color();
    root() = x.root();
    leftmost() = x.leftmost();
    rightmost() = x.rightmost();
    root()->parent() = head();
    m_node_count = x.m_node_count;
    x.reset();
  }

  void reset() {
    m_head.color() = rbcolor::red;
    root() = nullptr;
    leftmost() = head();
    rightmost() = head();
    m_node_count = 0;
  }

 protected:
  void __insert(node* pos, const T& v) {
    node* newnode;
    /**
     * we insert new node to lchild of pos if 
     * pos is head node or v < pos->m_data.
     */
    if (pos == head() || v < pos->m_data) { 
      newnode = create_node(v);
      pos->lchild() = newnode;
      if (pos == head()) {
        /**
         * update root, leftmost and rightmost.
         * leftmost is already set via
         * `pos->lchild() = newnode`.
         */
        root() = newnode;
        rightmost() = newnode;
      } else if (pos == leftmost()) {
        /* in this case we should update leftmost */
        leftmost() = newnode;
      }
      /**
       * because v < pos->m_data in this branch,
       * rightmost would not be mordified.
       * so we needn't care `pos == rightmost()`.
       */
    } else {/* v >= pos->m_data, we insert new node to rchild of pos */
      newnode = create_node(v);
      pos->rchild() = newnode;
      if (pos == rightmost())
        rightmost() = newnode;
      /* we needn't care `pos == leftmost()` */
    }
    /**
     * now, parent, lchild, rchild and color
     * of new node are not initialized.
     * so we initialize them in follow.
     * we initialze color in `__reblance()`.
     */
    newnode->parent() = pos;
    newnode->lchild() = nullptr;
    newnode->rchild() = nullptr;
    __rebalance((rbnode_base*)newnode);
    ++m_node_count;
  }

  void __erase(const T& v);

  void __destroy(node* pos) {
    while (pos != nullptr) {
      __destroy(pos->rchild());
      node* tmp = pos->lchild();
      destroy_node(pos);
      pos = tmp;
    }
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

  void __rebalance(rbnode_base* newnode) {
    /**
     * we always set newnode as red at first.
     * case1: parent of newnode is red
     *  case1.1: newnode is root ==> put newnode to black and return.
     *  case1.2: newnode is not root ==> fix the tree.
     * case2: parent of newnode is black
     *  case2.1: newnode is root ==> it's impossible, if newnode is root, 
     *                               it's parent is head, and head must be red.
     *  case2.2: newnode is not root ==> it's always when parent of newnode is black.
     *                                   we just return.
     * 
     * @question: why don't we set newnode as black at first ?
     * if we set newnode as black at first, black node count in
     * the branch of the new node will larger than others. we should always fix the tree.
     * but if we set newnode as red first, we may not need to fix the tree.
     */
    newnode->setRed();
    if (newnode->parent()->isRed()) { /* case 1 */
      if (newnode->isRoot()) {  /* case 1.1 */
        newnode->setBlk();
        return;
      } else {  /* case 1.2 */
        __fixInsert(newnode);
      }
    } else {  /* case 2 */
      return;
    }
  }

  void __leftRotate(rbnode_base* x) {
    /* rchild would not be nil when left rotate. */
    rbnode_base* rchild = x->rchild();
    x->rchild() = rchild->lchild();
    if (rchild->lchild() != nullptr)
      rchild->lchild()->parent() = x;
    if (x->isRoot())
      root() = (node*)rchild;
    else if (x == x->parent()->lchild())
      x->parent()->lchild() = rchild;
    else 
      x->parent()->rchild() = rchild;
    rchild->parent() = x->parent();
    rchild->lchild() = x;
    x->parent() = rchild;
  }

  void __rightRotate(rbnode_base* x) {
    /* lchild would not be nil when right rotate. */
    rbnode_base* lchild = x->lchild();
    x->lchild() = lchild->rchild();
    if (lchild->rchild() != nullptr)
      lchild->rchild()->parent() = x;
    if (x->isRoot())
      root() = (node*)lchild;
    else if (x == x->parent()->lchild())
      x->parent()->lchild() = lchild;
    else 
      x->parent()->rchild() = lchild;
    lchild->parent() = x->parent();
    lchild->rchild() = x;
    x->parent() = lchild;
  }

  /**
   * @brief fix insert.
   * @note we use [x] to discribe x is red node.
   * 
   * @param x newnode.
   */
  void __fixInsert(rbnode_base* x) {
    /**
     * this function is called in case 1.2 of function __reblance.
     * so we know x is not root and parent of x is red.
     * we use while instead of recurse.
     * 
     * @rule0: root node is black.
     * @rule1: no read node are adjacent.
     * @rule2: black count of all branch is equal.
     * @rule3: leaf node is nil. leaf node is always black.
     * 
     * @tip0: currently we only violate rule1 (x and it's parent are all red).
     * we should guarantee rule0, rule2 and rule3 are satisfied when
     * we do the fix operation.
     * 
     * @tip1: if a node's lchild or rchild is null, we consider lchild or 
     * rchild of this node as nil(black node, and leaf node, @see: rule3).
     * 
     * @tip2: because parent is red, so parent will not be root(@see: rule0).
     * so gparent will not be head. and x always has uncle(@see: tip1).
     * 
     * case 1: parent is gparent's lchild.
     * 
     *  case 1.1: uncle is red.
     *    case 1.1.1: x is parent's lchild.
     *    case 1.1.2: x is parent's rchild.
     * 
     *  case 1.2: uncle is black.
     *    case 1.2.1: x is parent's lchild.
     *    case 1.2.2: x is parent's rchild.
     * 
     * case 2: parent is gparent's rchild. ==> all like case1 (mirror operation).
     */

    /**
     * case 1.1.1: gparent must be black (@see: rule1).
     * "..." represents a subtree, and we assume black count of "..." is `N`.
     * 
     *         gparent                     [gparent]
     *         /     \                      /     \
     *    [parent]   [uncle]             parent    uncle
     *      /   \     /   \     ==>      /   \     /   \
     *    [x]   ... ...   ...          [x]   ... ...   ...
     *   /  \                         /  \
     * ...  ...                     ...  ...
     * 
     * this operation is compatible with tip0. after this operaion:
     * case 1.1.1.1: gparent is root ==> put gparent(root) to black and return.
     * case 1.1.1.2: gparent is not root
     * case 1.1.1.2.1: gparent's parent is red ==> x = gparent; go on fix.
     * case 1.1.1.2.2: gparent's parent is black ==> return.
     */

    /**
     * case 1.1.2: gparent must be black (@see: rule1).
     * "..." represents a subtree, and we assume black count of "..." is `N`.
     * 
     *         gparent                     [gparent]
     *         /     \                      /     \
     *    [parent]   [uncle]             parent    uncle
     *      /   \     /   \     ==>      /   \     /   \
     *    ...   [x] ...   ...          ...   [x] ...   ...
     *          /  \                         /  \
     *        ...  ...                     ...  ...
     * operaion of this case is same as case 1.1.1.
     */

    /**
     * case 1.2.1: gparent must be black (@see: rule1).
     * "..." represents a subtree, and we assume black count of "..." is `N`.
     * ".." represents a subtree, and we assume black count of ".." is `N-1`.
     * 
     *          gparent                     [gparent]                        parent
     *          /     \                      /     \        rightrotate      /    \
     *     [parent]   uncle             parent     uncle      gparent      [x]    [gparent]
     *      /   \     /   \     ==>      /   \     /   \        ==>       /   \    /     \
     *    [x]   ...  ..    ..         [x]    ...  ..    ..              ...   ... ...   uncle
     *   /  \                        /  \                                               /   \
     * ...  ...                    ...  ...                                            ..   ..
     * 
     * this operation is compatible with tip0. after this operaion:
     * fix is completed. return.
     */

    /**
     * case 1.2.2: gparent must be black (@see: rule1).
     * "..." represents a subtree, and we assume black count of "..." is `N`.
     * ".." represents a subtree, and we assume black count of ".." is `N-1`.
     * 
     *          gparent                         gparent                             gparent
     *          /     \       leftrotate        /     \                             /     \
     *     [parent]   uncle     parent        [x]    uncle   swap(x,parent)    [parent]   uncle
     *      /   \     /   \       ==>        /   \    /  \         ==>           /   \     /  \
     *    ...   [x]  ..    ..           [parent] ... ..   ..                   [x]   ...  ..   ..
     *          /  \                      /  \                                /  \
     *        ...  ...                  ...  ...                             ... ...
     * 
     * this operation is compatible with tip0. after this operaion:
     * this case is changed into case 1.2.1.
     */

    while (true) {
      if (x->parent() == x->gparent()->lchild()) {  /* case 1 */
        if (x->uncle() != nullptr && x->uncle()->isRed()) { /* case 1.1 */
          /** 
           * operation of case 1.1.1 and 1.1.2 are same. 
           * so we consider 1.1.2 as 1.1.1.
           * following code are all describe in case 1.1.1.
           */
          x->parent()->setBlk();
          x->uncle()->setBlk();
          x->gparent()->setRed();
          if (x->gparent()->isRoot()) { /* case 1.1.1.1 */
            x->gparent()->setBlk();
            return;
          } else {  /* case 1.1.1.2 */
            if (x->gparent()->parent()->isRed()) { /* case 1.1.1.2.1 */
              x = x->gparent();
              continue; /* go on fix */
            } else {  /* case 1.1.1.2.2 */
              return;
            }
          }
        } else {  /* case 1.2 */
          if (x == x->parent()->rchild()) { /* case 1.2.2 */
            /* transform 1.2.2 to 1.2.1 */
            x = x->parent();
            __leftRotate(x);
          }
          /* case 1.2.1 */
          x->parent()->setBlk();
          x->gparent()->setRed();
          __rightRotate(x->gparent());
          return;
        }
      } else {  /* case 2, mirror operation as case 1 */
        if (x->uncle() != nullptr && x->uncle()->isRed()) {
          x->parent()->setBlk();
          x->uncle()->setBlk();
          x->gparent()->setRed();
          if (x->gparent()->isRoot()) {
            x->gparent()->setBlk();
            return;
          } else {
            if (x->gparent()->parent()->isRed()) {
              x = x->gparent();
              continue;
            } else {
              return;
            }
          }
        } else {
          if (x == x->parent()->lchild()) {
            x = x->parent();
            __rightRotate(x);
          }
          x->parent()->setBlk();
          x->gparent()->setRed();
          __leftRotate(x->gparent());
          return;
        }
      }
    }
    return; /* would not reach. */
  }

  void __fixErase(rbnode_base* x);
  
};
