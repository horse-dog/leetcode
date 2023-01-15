#pragma once

#include <memory>
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

  rbcolor& color() { return m_color; }

  rbnode*& parent() { return (rbnode*&)m_parent; }

  rbnode*& lchild() { return (rbnode*&)m_lchild; }

  rbnode*& rchild() { return (rbnode*&)m_rchild; }

  rbnode*& brother() {
    if (this == parent()->lchild())
      return parent()->rchild();
    else
      return parent()->lchild();
  }

  rbnode*& gparent() { return parent()->parent(); }

  rbnode*& uncle() {
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

  bool isHead() const 
  { return isRed() && m_parent->m_parent == this; }

  rbnode* prev() const {
    rbnode_base* x = (rbnode_base*)this;
    if (x->m_color == rbcolor::red 
     && x->m_parent->m_parent == x) {
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
    if (x->m_color == rbcolor::red 
     && x->m_parent->m_parent == x) {
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

  size_t erase(const T& x) {
    node* lb = lower_bound(x);
    node* rb = upper_bound(x);
    size_t n = 0;
    while (lb != rb) {
      node* tmp = lb;
      lb = lb->next();
      __erase(tmp);
      ++n;
    }
    m_node_count -= n;
    return n;
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

  friend std::ostream& operator<<(std::ostream& os, const rbtree& tree) {
    os << '{';
    auto begin = tree.leftmost();
    auto end = tree.head();
    if (begin != end) {
      os << begin->m_data;
      begin = begin->next();
      while (begin != end) {
        os << ", " << begin->m_data;
        begin = begin->next();
      }
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
  { return (node*&)m_head.m_lchild; }

  node* leftmost() const
  { return (node*)m_head.m_lchild; }

  node*& rightmost()
  { return (node*&)m_head.m_rchild; }

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

  node* lower_bound(const T& v) const {
    node* y = head();
    node* x = root();
    while (x != nullptr) {
      if (v <= x->m_data) {
        y = x;
        x = x->lchild();
      } else {
        x = x->rchild();
      }
    }
    return y;
  }

  node* upper_bound(const T& v) const {
    node* y = head();
    node* x = root();
    while (x != nullptr) {
      if (v < x->m_data) {
        y = x;
        x = x->lchild();
      } else {
        x = x->rchild();
      }
    }
    return y;    
  }

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
    m_head.m_color = rbcolor::red;
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
    __rebalance(newnode);
    ++m_node_count;
  }

  void __erase(node* pos) {
    /**
     * case 1: pos's lchild and rchild are all nil.     ==> finally erase position is pos.
     * case 2: pos's lchild is not nil, rchild is nil.  ==> finally erase position is pos.
     * case 3: pos's lchild is nil, rchild is not nil.  ==> finally erase position is pos.
     * case 4: pos's lchild and rchild are all not nil. ==> transform to case 1 or case 3.
     *         tramsform finally erase position to pos's successor.
     *         pos's successor must match case 1 or case 3.
     * @tip: in case 2 and case 3, pos must be black, and pos's child must be red, 
     *       and pos's child's children must be all nil.(@see: rule1, rule2).
     */
    node* y = pos; /* finally erase position in rbtree, initialized with pos */
    node* x = nullptr;         /* child of fianlly erase pos */
    node* x_parent = nullptr;  /* x's real-time parent */
    
    /**
     * first of all, we find finally-erase-node and it's child,
     * we know finally-erase-node has only one no-nil-child at most.
     * so we can let @p x to present finally erase node's child.
     */
    if (y->lchild() == nullptr) {
      x = y->rchild();
    } else {
      if (y->rchild() == nullptr) {
        x = y->lchild();
      } else {
        y = y->rchild();
        while (y->lchild() != 0)
          y = y->lchild();
        x = y->rchild();
      }
    }

    if (y != pos) { /* if y != pos, this is cause only by case 4. */
      /**
       * becasue pos has two non-nil children in case 4,
       * we know after unlink pos, leftmost and rightmost would not
       * be mordified.
       */

      y->lchild() = pos->lchild();
      pos->lchild()->parent() = y;

      if (y != pos->rchild()) {
        x_parent = y->parent();
        if (x != nullptr)
          x->parent() = y->parent();
        y->parent()->lchild() = x;
        y->rchild() = pos->rchild();
        pos->rchild()->parent() = y;
      } else {
        x_parent = y;
      }

      if (pos == root())
        root() = y;
      else if (pos->parent()->lchild() == pos)
        pos->parent()->lchild() = y;
      else
        pos->parent()->rchild() = y;
      y->parent() = pos->parent();
      rbcolor tmp = y->color();
      y->color() = pos->color();
      pos->color() = tmp;
      y = pos;
    } else {  /* case 1,2,3. */
      x_parent = y->parent();
      if (x != nullptr)
        x->parent() = y->parent();
      
      if (pos == root()) {
        root() = x;
      } else {
        if (pos->parent()->lchild() == pos)
          pos->parent()->lchild() = x;
        else
          pos->parent()->rchild() = x;
      }

      if (pos == leftmost()) {
        /* pos is leftmost, so pos's lchild must be nil. */
        if (pos->rchild() == nullptr)
          leftmost() = pos->parent();
        else
          /* x must be red and x's children are all nil.(@see: tip) */
          leftmost() = x; /* leftmost() == minimum(x) ? */
      }

      if (pos == rightmost()) {
        if (pos->lchild() == nullptr)
          rightmost() = pos->parent();
        else 
          /* x must be red and x's children are all nil.(@see: tip) */
          rightmost() = x; /* leftmost() == maximum(x) ? */
      }
    }

    if (y->isBlk()) /* if y is black, now we are violate with rule2. */
      /* fix the rbtree. */
      __fixErase(x, x_parent);
    
    destroy_node(y);
  }

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

  void __rebalance(node* newnode) {
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
      if (newnode == root()) {  /* case 1.1 */
        newnode->setBlk();
        return;
      } else {  /* case 1.2 */
        __fixInsert(newnode);
      }
    } else {  /* case 2 */
      return;
    }
  }

  void __leftRotate(node* x) {
    /* rchild would not be nil when left rotate. */
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
    /* lchild would not be nil when right rotate. */
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

  /**
   * @brief fix insert.
   * @note we use [x] to discribe x is red node.
   * @note "..." represents a subtree, and we assume black count of "..." is `N`.
   * @note ".." represents a subtree, and we assume black count of ".." is `N-1`.
   * 
   * @param x newnode.
   */
  void __fixInsert(node* x) {
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
          if (x->gparent() == root()) { /* case 1.1.1.1 */
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
          if (x->gparent() == root()) {
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

  /**
   * @brief fix erase.
   * @note we use [x] to discribe x is red node.
   * @note we use (x) to discribe x is red node or black node.
   * @note "..." represents a subtree, and we assume black count of "..." is `N`.
   * @note ".." represents a subtree, and we assume black count of ".." is `N-1`.
   */
  void __fixErase(node* x, node* x_parent) {
    /**
     * no we are violate with rule2 bacause of the unlink operation in
     * function __erase. 
     * 
     * @tip0: black count in the branch to x is one less than
     * black count in other branch.
     * 
     * @tip1: currently we only violate rule2.
     * we should guarantee rule0, rule1 and rule3 are satisfied when
     * we do the fix operation.
     * 
     * @tip2: if a node's lchild or rchild is null, we consider lchild or 
     * rchild of this node as nil(black node, and leaf node, @see: rule3).
     * 
     * case 1: x is root (x's parent is head).
     *  case 1.1: x is red.  ==> put x to black to satisfy rule0, then return.
     *  case 1.2: x is black ==> return.
     * 
     * case 2: x is not root (x's parent is not head).
     *  case 2.1: x is red.  ==> put x to black to satisfy rule2, then return.
     *  case 2.2: x is black.
     *    case 2.2.1: x is x_parent's lchild.
     *      case 2.2.1.1: x's brother is red.
     *      case 2.2.1.2: x's brother is black.
     *        case 2.2.1.2.1: x's brother's children are all black.
     *        case 2.2.1.2.2: x's brother's lchild is red, rchild is black.
     *        case 2.2.1.2.3: x's brother's rchild is red, lchild is black.
     *        case 2.2.1.2.4: x's brother's children are all red.
     *    case 2.2.2: x is x_parent's rchild.(mirror operation of case 2.2.1).
     */

    /**
     * case 2.2.1.1: in this case, x's brother's children must be all black.(@see: rule1).
     * and x's brother's children must not be nil.(@see: rule2).
     * in this case, x's brother is red, so x's parent must be black.(@see: rule1).
     * 
     *     parent                 [parent]                           brother
     *     /    \                  /    \         leftrotate         /     \
     *    x   [brother]           x    brother      parent      [parent]    br
     *  /  \    /    \    ==>   /  \    /    \        ==>         /   \     / \
     * ..  ..  bl     br       ..  ..  bl     br                 x     bl ... ...
     *        / \    /  \             / \    /  \               / \   /  \
     *      ... ... ... ...         ... ... ... ...            .. .. ... ...
     * 
     * after this operation: refresh brother to bl, transform to case 2.2.1.2.
     */

    /**
     * case 2.2.1.2.1:
     * 
     *   (parent)                (parent)
     *    /    \                  /     \
     *   x    brother   ==>     x    [brother]
     *  / \    /  \            / \     /   \
     * .. .. ...  ...         .. ..  ...   ...
     * 
     * case 2.2.1.2.1.1: parent is red ==> put parent to black; return.
     * case 2.2.1.2.1.2: parent is red ==> x = parent; go on fix.
     */

    /**
     * case 2.2.1.2.2:
     * 
     *   (parent)           (parent)                   (parent)
     *    /    \              /   \       rightrotate    /   \
     *   x    brother        x  [brother]   brother     x     bl
     *  / \    /   \    ==> / \   /   \       ==>      / \   /  \
     * .. .. [bl]   br     .. .. bl    br             .. .. ... [brother]
     *       /  \   / \         /  \   / \                      /  \
     *     ... ... .. ..      ... ... .. ..                   ...   br
     *                                                             /  \
     *                                                            ..   ..
     * after this operation: refresh brother to bl, transform to case 2.2.1.2.3.
     */

    /**
     * case 2.2.1.2.3:
     * 
     *    (parent)              parent                        (brother)
     *    /     \              /     \       leftrotate         /   \
     *   x    brother         x   (brother)    parent      parent    br
     *  / \   /    \    ==>  / \   /    \        ==>        /  \    /  \
     * .. .. bl    [br]     .. .. bl     br                x    bl ... ...
     *      / \    /  \          / \    /  \              / \   / \
     *     .. ..  ... ...       .. .. ...  ...           .. .. .. ..
     * 
     * after this operation: satisfiy all rules, return.
     */

    /**
     * case 2.2.1.2.4:
     * 
     *    (parent)              parent                       (brother)
     *    /     \              /     \       leftrotate       /     \
     *   x    brother         x   (brother)    parent     parent     br
     *  / \    /   \    ==>  / \    /    \       ==>      /   \     /  \
     * .. .. [bl]  [br]     .. .. [bl]    br             x    [bl] ... ...
     *      /  \   /  \           /  \   /  \           / \   /  \
     *    ... ... ... ...       ... ... ... ...        .. .. ... ...
     * 
     * operation is same as case 2.2.1.2.3.
     * after this operation: satisfiy all rules, return.
     */

    while (true) {
      if (x == root()) {  /* case 1 */
        if (x != nullptr)
          x->setBlk();
        return;
      }

      if (x != nullptr && x->isRed()) { /* case 2.1 */
        x->setBlk();
        return;
      }

      if (x == x_parent->lchild()) {  /* case 2.2.1 */
        node* brother = x_parent->rchild();
        if (brother->isRed()) { /* case 2.2.1.1 */
          /* transform to case 2.2.1.2 */
          brother->setBlk();
          x_parent->setRed();
          __leftRotate(x_parent);
          brother = x_parent->rchild();
        }
        /* case 2.2.1.2 */
        if ((brother->lchild() == nullptr || brother->lchild()->isBlk()) 
         && (brother->rchild() == nullptr || brother->rchild()->isBlk())) { /* case 2.2.1.2.1 */
          brother->setRed();
          if (x_parent->isRed()) {  /* case 2.2.1.2.1.1 */
            x_parent->setBlk();
            return;
          } else {  /* case 2.2.1.2.1.2 */
            x = x_parent;
            x_parent = x_parent->parent();
          }
        } else {  /* case 2.2.1.2.2, case 2.2.1.2.3 and case 2.2.1.2.4 */
          if (brother->rchild() == nullptr || brother->rchild()->isBlk()) { /* case 2.2.1.2.2 */
            /* transform to case 2.2.1.2.3 */
            if (brother->lchild() == nullptr) printf("__ll__error__\n");
            brother->lchild()->setBlk();
            brother->setRed();
            __rightRotate(brother);
            brother = x_parent->rchild();
          }
          /* case 2.2.1.2.3 and case 2.2.1.2.4, their operation are the same. */
          brother->color() = x_parent->color();
          x_parent->setBlk();
          if (brother->rchild() == nullptr) printf("__lr__error__\n");
          brother->rchild()->setBlk();
          __leftRotate(x_parent);
          return;
        }
      }
      else {  /* case 2.2.2 */
        node* brother = x_parent->lchild();
        if (brother->isRed()) {
          brother->setBlk();
          x_parent->setRed();
          __rightRotate(x_parent);
          brother = x_parent->lchild();
        }
        if ((brother->lchild() == nullptr || brother->lchild()->isBlk()) 
         && (brother->rchild() == nullptr || brother->rchild()->isBlk())) {
          brother->setRed();
          if (x_parent->isRed()) {
            x_parent->setBlk();
            return;
          } else {
            x = x_parent;
            x_parent = x_parent->parent();
          }
        } else {
          if (brother->lchild() == nullptr || brother->lchild()->isBlk()) {
            if (brother->rchild() == nullptr) printf("__rr__error__\n");
            brother->rchild()->setBlk();
            brother->setRed();
            __leftRotate(brother);
            brother = x_parent->lchild();
          }
          brother->color() = x_parent->color();
          x_parent->setBlk();
          if (brother->lchild() == nullptr) printf("__rl__error__\n");
          brother->lchild()->setBlk();
          __rightRotate(x_parent);
          return;
        }
      }
    }
    return; /* would not reach. */
  }
  
};
