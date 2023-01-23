#pragma once

#include <atomic>
#include <memory>
#include <sstream>
#include <iostream>

#define __TINY_MEMPOOL__

class tiny_mempool {
 protected:
 struct memNode { memNode *nextnode = nullptr; };

 protected:
  std::atomic<memNode*> m_free_head[16];

 private:
  tiny_mempool() {}

  ~tiny_mempool()
  { for (int i = 0; i < 16; i++)
    { if (m_free_head[i] != nullptr)
      { memNode *ptr = m_free_head[i];
        while (ptr != nullptr)
        { auto nptr = ptr->nextnode;
          free(ptr);
          ptr = nptr;
        }
      }
      m_free_head[i] = nullptr;
    }
  }

  int getindex(int size)
  { static const unsigned int sizetable[16]
    = { 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128 };
    int __distance = 0;
    for (; __distance < 16; __distance++)
      if (sizetable[__distance] >= size)
        break;
    return __distance;
  }

 public:
  static tiny_mempool &instance()
  { static tiny_mempool pool;
    return pool;
  }

 public:
  void *alloc(int size)
  { if (size > 128) return malloc(size);
    int index = getindex(size);
    int realsize = (index + 1) << 3;
    memNode *p = m_free_head[index];
    if (p == nullptr)
      return malloc(realsize);
    else
    { while (!m_free_head[index].compare_exchange_weak(p, p->nextnode))
        if (p == nullptr) return malloc(realsize);
      return p;
    }
    return nullptr;
  }

  void delloc(void *ptr, int size)
  { if (ptr == nullptr) return;
    if (size > 128) return free(ptr);
    int index = getindex(size);
    memNode *pNew = (memNode *)ptr;
    pNew->nextnode = m_free_head[index];
    while (!(m_free_head[index].compare_exchange_weak(pNew->nextnode, pNew)))
    ;
  }
  
  /**
   * @brief report memory distribute in the pool.
   * @attention May cause undefined result if 
   * allocate memory use current pool before this
   * function return.
   */
  void report()
  { printf("\033[32m\033[1mtiny_mempool report\033[0m\n");
    printf("\033[34mindex\tnode size   node count\033[0m\n");
    for (int i = 0; i < 16; ++i)
    { int n = 0;
      memNode *p = m_free_head[i];
      while (p)
      { n++;
        p = p->nextnode;
      }
      printf("\033[31m%5d\t %3d \033[35mbyte\033[31m   %10d"
             "\033[0m\n", i, (i + 1) << 3, n);
    }
  }

};

template<class T>
class tiny_allocator { 
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  tiny_allocator() {}

  tiny_allocator(tiny_allocator const &) {}

  tiny_allocator &operator=(tiny_allocator const &)
  { return *this; }

  template<class Other>
  tiny_allocator(tiny_allocator<Other> const &) {}

  template<class Other>
  tiny_allocator &operator=(tiny_allocator<Other> const &)
  { return *this; }

  pointer allocate(size_type count)
  { return (pointer)tiny_mempool::instance()
      .alloc(count * sizeof(value_type));
  }

  void deallocate(pointer ptr, size_type count)
  { return tiny_mempool::instance()
      .delloc(ptr, count * sizeof(value_type));
  }
};

template <typename _Tp1, typename _Tp2>
std::ostream& operator<<(std::ostream& os, 
  const std::pair<_Tp1, _Tp2>& pair) {
  return os << '{' << pair.first << ", " << pair.second << '}';
}

template <typename T, size_t M>
struct b_node {
  int m_key_count;
  T*  m_parent;
  T   m_data[M];
  T*  m_ptr[M];
};

template <typename T, size_t M=5, typename Alloc=tiny_allocator<T>>
class b_tree {
 protected:

  template <typename _Tp, typename _Up>
  struct alloc_rebind {};

  template <template <typename, typename...> class _Template,
            typename _Up, typename _Tp, typename... _Types>
  struct alloc_rebind<_Template<_Tp, _Types...>, _Up>
  { using type = _Template<_Up, _Types...>; };

  using node = b_node<T, M+1>;
  using allocator_type = typename alloc_rebind<Alloc, node>::type;

  struct b_tree_impl : public allocator_type
  { node* m_root;
    size_t m_cnt;
  };

  b_tree_impl m_impl;

 public:
  b_tree() {}
  ~b_tree() {}

  bool insert(const T& value) {
    int index, exist;
    node* nd = this->find(value, index, exist);
    if (exist != 0) return false;
    insert_node(value, nd, index);
    return true;
  }

  bool erase(const T& value);

  size_t size() const { return m_impl.m_cnt; }

  bool empty() const { return size() == 0; }

 protected:
  template <typename _Tp>
  static bool less(const _Tp& x, const _Tp& y) 
  { return x < y; }

  template <typename _Tp1, typename _Tp2>
  static bool less(const std::pair<_Tp1, _Tp2>& x, 
            const std::pair<_Tp1, _Tp2>& y) {
    return x.first < y.first;
  }

  template <typename _Tp1, typename _Tp2>
  static bool less(const std::pair<_Tp1, _Tp2>& x, const _Tp1& y) {
    return x.first < y;
  }

  template <typename _Tp1, typename _Tp2>
  static bool less(const _Tp1& x, const std::pair<_Tp1, _Tp2>& y) {
    return x < y.first;
  }

  constexpr size_t m_M() const { return M; }

  constexpr size_t max_keycnt() const { return M - 1; }

  constexpr size_t min_keycnt() const { return (M >> 1) - 1; }

  /**
   * @brief find @p value in the tree.
   * 
   * @note if success, @p exist = 1, and return a pointer, 
   * pointer at offset @p index is @p value .
   * 
   * @note if failed, @p exist = 0, and return a pointer, 
   * if we want to insert @p value to the tree, 
   * we can insert @p value at the return pointer with offset
   * @p index . 
   * 
   * @param value 
   * @param index 
   * @param exist
   * @return node* 
   */
  node* find(const T& value, int& index, int& exist) {
    int found = 0, i = 0;
    node* p = m_impl.m_root, *q = nullptr;
    while (p != nullptr && found == 0) {
      i = search_node(p, value);
      if (i > 0 && !less(p->m_data[i], value) && !less(value, p->m_data[i]))
        found = 1;
      else {
        q = p;
        p = p->ptr[i];
      }
    }
    index = i;
    exist = found;
    return found ? p : q;
  }

  int search_node(node* nd, const T& value) {
    int l = 1, h = nd->m_key_count, m = 0;
    while (l <= h) {
      m = (l + h) >> 1;
      if (less(value, nd->m_data[m]))
        h = m - 1;
      else
        l = m + 1;
    }
    return l - 1;
  }

  /**
   * @brief insert @p value between @p nd + @p index 
   * and @p nd + @p index + 1.
   * 
   * @param value 
   * @param nd 
   * @param index 
   */
  void insert_node(const T& value, node* nd, int index) { 
    node* ap;
    int finished, need_new_root, s;
    T x;
    if (nd == nullptr)
      newroot(nullptr, value, nullptr);
    else {
      x = value;
      ap = nullptr;
      finished = need_new_root = 0;
      while (need_new_root == 0 && finished == 0) {
        insert_node(nd, index, std::move(x), ap);
        if (nd->m_key_count <= max_keycnt())
          finished = 1;
        else {
          s = (m_M() + 1) / 2;
          split(nd, ap);
          x = std::move(nd->key[s]);
          if (nd->m_parent) {
            nd = nd->m_parent;
            index = search_node(nd, x);
          } else {
            need_new_root = 1;
          }
        }
      }
      if (need_new_root == 1)
        newroot(nd, x, ap);
    }
  }

  void insert_node(node* q, int i, T&& x, node* ap) {
    int j;
    for (j = q->m_key_count; j > i; j--) {
      q->m_data[j + 1] = q->m_data[j]; // TODO: move_backward ?
      q->m_ptr[j + 1] = q->m_ptr[j];
    }
    q->m_data[i + 1] = x;
    q->m_ptr[i + 1] = ap;
    if (ap != nullptr)
        ap->m_parent = q;
    ++q->m_key_count;
  }

  void split(node*& q, node*& ap) {
    int i, s = (m_M() + 1) / 2;
    ap = create_node(q->m_data + s + 1, q->m_data + q->m_key_count + 1);
    ap->m_ptr[0] = q->m_ptr[s];
    // TODO...


    for (i = s + 1; i <= m_M(); i++) {
      ap->m_data[i - s] = q->m_data[i];
      ap->m_ptr[i - s] = q->m_ptr[i];
      if (ap->m_ptr[i - s] != nullptr)
        ap->m_ptr[i - s]->m_parent = ap;
    }
    ap->m_key_count = q->m_key_count - s;
    ap->m_parent = q->m_parent;
    for (i = 0; i <= q->m_key_count - s; i++)
      if (ap->m_ptr[i] != nullptr)
        ap->m_ptr[i]->m_parent = ap;
    q->m_key_count = s - 1;
  }

  /**
   * @brief new a root node compose with 
   * ( @p p, @p x, @p ap ).
   * 
   * @param p 
   * @param x 
   * @param ap 
   */
  void newroot(node* p, const T& x, node* ap) {
    m_impl.m_root = create_node(x, 1);
    m_impl.m_root->m_key_count = 1;
    m_impl.m_root->m_ptr[0] = p;
    m_impl.m_root->m_ptr[1] = ap;
    if (p != nullptr)
      p->m_parent = m_impl.m_root;
    if (ap != nullptr)
      ap->m_parent = m_impl.m_root;
  }

  node* create_node(const T& value, int i) {
    node* p = m_impl.allocate(1);
    new (p->m_data + i) T(value);
  }

  node* create_node(T* first, T* last) {
    node* p = m_impl.allocate(1);
    for (int i = 1; first != last; ++i, ++first)
      new (p->m_data + i) T(std::move(first));
  }

  void destroy_node(node* p) {
    for (int i = 0; i < p->m_key_count; i++)
      (p->m_data + i + 1)->~T();
    m_impl.deallocate(p, 1);
  }
};