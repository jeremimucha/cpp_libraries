#ifndef LINKEDLIST_HPP_
#define LINKEDLIST_HPP_

#include <memory>
#include <iterator>
#include <utility>
#include <initializer_list>

template<typename> class node;
template<typename> class List_iterator;
template<typename> class List_const_iterator;


/* List */
/* ------------------------------------------------------------------------- */
template< typename T,
          typename Allocator = std::allocator<node<T>> >
class List
{
    typedef node<T>                Node;
public:
    using value_type             = T;
    using allocator_type         = Allocator;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using iterator               = List_iterator<T>;
    using const_iterator         = List_const_iterator<T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

/* constructors */
    explicit List( const Allocator& al = Allocator() )
        : alloc(al), head(), tail()
        {
            link_nodes(&head, &tail);
        }
    explicit List( size_type count, const Allocator& al = Allocator() );
    // List( size_type count,
    //             const T& value,
    //             const Allocator& al = Allocator() );
    // template<typename InputIter>
    // List( InputIter first, InputIter last,
    //             const Allocator& al = Allocator() );
    List( std::initializer_list<T> init,
                const Allocator& al = Allocator() );
    ~List() noexcept;

/*  copy control */
    List( const List& other );
    List( List&& other ) noexcept;
    List& operator=( const List& other );
    List& operator=( List&& other );
    List& operator=( std::initializer_list<T> init );

/* member functions */
    allocator_type get_allocator() const noexcept(Allocator())
        { return alloc; }

/* element access */
    reference front() noexcept
        { return head.next->value; }
    const_reference front() const noexcept
        { return head.next->value; }
    reference back() noexcept
        { return tail.prev->value; }
    const_reference back() const noexcept
        { return tail.prev->value; }

/* capacity */
    bool empty() noexcept
        { 
            return head.next == &tail &&
                   tail.prev == &head;
        }
    size_type size() noexcept;

/* iterators */
    iterator begin() noexcept
        { return iterator(head.next); }
    iterator end() noexcept
        { return iterator(&tail); }
    const_iterator begin() const noexcept
        { return const_iterator(head.next); }
    const_iterator end() const noexcept
        { return const_iterator(&tail); }
    const_iterator cbegin() const noexcept
        { return const_iterator(head.next); }
    const_iterator cend() const noexcept
        { return const_iterator(&tail); }
    reverse_iterator rbegin() noexcept
        { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept
        { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept
        { return reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept
        { return reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const noexcept
        { return reverse_iterator(cend()); }
    const_reverse_iterator crend() const noexcept
        { return reverse_iterator(cbegin()); }
    

/* modifiers */
    void clear() noexcept
        { free(); }
    
    iterator insert(iterator pos, const T& value );
    iterator insert(iterator pos, T&& value);
    iterator insert(iterator pos, size_type count, const T& value);
    template<typename InputIterator>
    iterator insert(iterator pos, InputIterator first, InputIterator last);
    iterator insert(iterator pos, std::initializer_list<T> init);

    template<typename... Args>
    iterator emplace( iterator pos, Args&&... args );

    iterator erase( iterator pos );
    iterator erase( iterator first, iterator last );

    void push_back( const T& value );
    void push_back( T&& value );
    template<typename... Args>
    void emplace_back( Args&&... args );
    void pop_back() noexcept;
    
    void push_front( const T& value );
    void push_front( T&& value );
    template<typename... Args>
    void emplace_front( Args&&... args );
    void pop_front() noexcept;
    
    void resize( size_type count );
    void resize( size_type count, const value_type& value );

    void swap( List& other ) noexcept;

protected:
/* helper functions */
    template<typename... Args>
    Node* create_node(Args&&... args);

    template<typename... Args>
    std::pair<Node*,Node*> alloc_n( size_type n, Args&&... args);
    
    template<typename Iter>
    std::pair<Node*,Node*> alloc_range(Iter begin, Iter end);
    
    void link_nodes(Node* pred, Node* target_a, Node* target_b, Node* succ);
    void link_nodes(Node* pred, Node* succ);

    void free() noexcept;

private:
/* member variables */
    Allocator alloc;
    Node head;
    Node tail;
};
/* ------------------------------------------------------------------------- */



/* node */
/* ------------------------------------------------------------------------- */
template<typename T>
class node
{
friend class List<T>;
friend class List_iterator<T>;
friend class List_const_iterator<T>;
public:
    node() { }
    explicit node( const T& val )
        : prev(), next(), value(val)
        { }
    template<typename... Args>
    node( Args&&... args )
        : prev(), next(), value(std::forward<Args>(args)...)
        { }
private:
    node* prev;
    node* next;
    T value;
};
/* ------------------------------------------------------------------------- */


/* List_iterator */
/* ------------------------------------------------------------------------- */
template<typename T>
class List_iterator 
/*     : public std::iterator<
                std::bidirectional_iterator_tag // iterator_category
                ,T                               // value_type
                ,std::ptrdiff_t                  // difference_type
                ,T*                              // pointer
                ,T&                              // reference
                >  */           
{
    friend class List<T>;
    typedef List_iterator<T>                    self;
public:
    typedef node<T>                             Node;
    typedef std::bidirectional_iterator_tag     iterator_category;
    typedef T                                   value_type;
    typedef std::ptrdiff_t                      difference_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
    typedef List_const_iterator<T>              const_iterator;          
    /* constructors */
    List_iterator()
        : current()
        { }
    explicit List_iterator(Node* n)
        : current(n)
        { }
    
    reference operator*() const noexcept
        { return current->value; }
    pointer operator->() const noexcept
        { return &(current->value); }

    self& operator++() noexcept
        {
            current = current->next;
            return *this;
        } 
    self& operator++(int) noexcept
        {
            self temp(*this);
            current = current->next;
            return temp;
        }
    self& operator--() noexcept
        {
            current = current->prev;
            return *this;
        }
    self operator--(int) noexcept
        {
            self temp(*this);
            current = current->prev;
            return temp;
        }
    
    operator const_iterator()
        { return const_iterator(current); }

    bool operator==(const self& other) const
        { return current == other.current; }
    bool operator!=(const self& other) const
        { return current != other.current; }

private:
    Node* current;
};
/* ------------------------------------------------------------------------- */


/* List_const_iterator */
/* ------------------------------------------------------------------------- */
template<typename T>
class List_const_iterator
/*     : public std::iterator<
                std::bidirectional_iterator_tag  // iterator_category
                ,T                               // value_type
                ,std::ptrdiff_t                  // difference_type
                ,const T*                        // pointer
                ,const T&                        // reference
                > */
{
    friend class List<T>;
    typedef List_const_iterator<T>              self;
public:
    typedef const node<T>                       Node;
    typedef List_iterator<T>                    iterator;
    typedef std::bidirectional_iterator_tag     iterator_category;
    typedef T                                   value_type;
    typedef std::ptrdiff_t                      difference_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
    
    /* constructors */
    List_const_iterator()
        : current()
        { }
    explicit List_const_iterator(const Node* n)
        : current(n)
        { }
    
    /* access */
    reference operator*() const
        { return current->value; }
    pointer operator->() const
        { return &(current->value); }

    self& operator++() noexcept
        {
            current = current->next;
            return *this;
        }
    self operator++(int) noexcept
        {
            self temp(*this);
            current = current->next;
            return temp;
        }
    self& operator--() noexcept
        {
            current = current->prev;
            return *this;
        }
    self operator--(int) noexcept
        {
            self temp(*this);
            current = current->prev;
            return temp;
        }

    bool operator==(const self& other) const noexcept
        { return current == other.current; }
    bool operator!=(const self& other) const noexcept
        { return current != other.current; }

private:
    Node* current;
};
/* ------------------------------------------------------------------------- */


/* List implementation */
/* ------------------------------------------------------------------------- */

/* helper functions */
/* ------------------------------------------------------------------------- */
template<typename T, typename Allocator>
    template<typename... Args>
inline typename List<T,Allocator>::Node*
List<T,Allocator>::create_node(Args&&... args)
{
    Node* data = alloc.allocate(1);
    alloc.construct(data, std::forward<Args>(args)...);
    return data;
}

template<typename T, typename Allocator>
    template<typename... Args>
inline std::pair<node<T>*,node<T>*>
List<T,Allocator>::alloc_n( size_type n, Args&&... args )
{
    Node* data = alloc.allocate(1);
    alloc.construct( data, std::forward<Args>(args)... );
    Node* pred = data;
    for(size_type i=1; i<n; ++i){
        Node* succ = alloc.allocate(1);
        // alloc.construct(succ, val, pred);
        alloc.construct( succ, std::forward<Args>(args)... );
        link_nodes(pred, succ);
        // succ->prev = pred;
        // pred->next = succ;
        pred = succ;
    }
    // data->prev = &head;
    // pred->next =  &tail;
    return std::make_pair(data, pred);
}

template<typename T, typename Allocator>
    template<typename Iter>
inline std::pair<node<T>*,node<T>*>
List<T,Allocator>::alloc_range(Iter begin, Iter end)
{
    Node* data = alloc.allocate(1);
    alloc.construct(data, *begin);
    Node* pred = data;
    while(++begin != end){
        Node* succ = alloc.allocate(1);
        alloc.construct(succ, *begin);
        link_nodes(pred, succ);
        // succ->prev = pred;
        // pred->next = succ;
        pred = succ;
    }
    // data->prev = &head;
    // pred->next = &tail;
    return std::make_pair(data, pred);
}

/* template<typename T, typename Allocator>
inline void
List<T,Allocator>::link_nodes(Node* tgt_first, Node* tgt_last,
                             Node* pred, Node* succ)
{
    tgt_first->prev = pred;
    pred->next = tgt_first;

    tgt_last->next = succ;
    succ->prev = tgt_last;
} */

template<typename T, typename Allocator>
inline void
List<T,Allocator>::link_nodes( Node* pred,
                               Node* target_a, Node* target_b,
                               Node* succ )
{
    target_a->prev = pred;
    target_b->next = succ;
    succ->prev = target_b;
    pred->next = target_a;
}


template<typename T, typename Allocator>
inline void
List<T,Allocator>::link_nodes(Node* pred, Node* succ)
{
    pred->next = succ;
    succ->prev = pred;
}

template<typename T, typename Allocator>
inline void List<T,Allocator>::free() noexcept
{
    Node* next = head.next;
    Node* temp;
    while(next != &tail){
        temp = next->next;
        alloc.destroy(next);
        alloc.deallocate(next, 1);
        next = temp;
    }
}
/* ------------------------------------------------------------------------- */


/* constructors */
/* ------------------------------------------------------------------------- */
template<typename T, typename Allocator>
inline List<T,Allocator>::List( size_type count, const Allocator& al )
    : alloc(al)
{
    std::pair<Node*,Node*> data = alloc_n(count);
    // link_nodes(data.first, data.second, &head, &tail);;
    link_nodes(&head, data.first, data.second, &tail);
    // head.next = data.first;
    // tail.prev = data.second;
}

template<typename T, typename Allocator>
inline List<T,Allocator>::List( std::initializer_list<T> init
                              , const Allocator& al )
    : alloc(al)
{
    std::pair<Node*,Node*> data = alloc_range(init.begin(), init.end());

    // link_nodes(data.first, data.second, &head, &tail);
    link_nodes(&head, data.first, data.second, &tail);
    // head.next = data.first;
    // tail.prev = data.second;
}

/* destructor */
template<typename T, typename Allocator>
inline List<T,Allocator>::~List() noexcept
{
    free();
    // link_nodes(&tail, &head, &head, &tail);
    link_nodes(&head, &tail, &tail, &head);
}
/* ------------------------------------------------------------------------- */

/* copy control */
/* ------------------------------------------------------------------------- */
template<typename T, typename Allocator>
inline List<T,Allocator>::List( const List& other )
    : alloc(other.alloc)
{
    std::pair<Node*,Node*> data = alloc_range(other.cbegin(), other.cend());
    // link_nodes(data.first, data.second, &head, &tail);
    link_nodes(&head, data.first, data.second, &tail); // better ?
    // head.next = data.first;
    // tail.prev = data.second;
}

template<typename T, typename Allocator>
inline List<T,Allocator>::List( List&& other ) noexcept
    : alloc(std::move(other.alloc))
{
    // head.next = other.head.next;
    // other.head.next->prev = &head;
    // tail.prev = other.tail.prev;
    // other.tail.prev->next = &tail;
    // link_nodes(other.head.next, other.tail.prev, &head, &tail);
    link_nodes(&head, other.head.next, other.tail.prev, &tail);

    // other.head.next = other.tail.prev = nullptr;
    // link_nodes(&other.tail, &other.head, &other.head, &other.tail);
    link_nodes(&other.head, &other.tail, &other.tail, &other.head);
}

template<typename T, typename Allocator>
inline List<T,Allocator>&
List<T,Allocator>::operator=( const List& other )
{
    if( this == &other )
        return *this;

    free();
    std::pair<Node*,Node*> data = alloc_range(other.cbegin(), other.cend());
    alloc = other.alloc;
    // link_nodes(data.first, data.second, &head, &tail);
    link_nodes(&head, data.first, data.second, &tail);
    // head.next = data.first;
    // tail.prev = data.second;
    return *this;
}

template<typename T, typename Allocator>
inline List<T,Allocator>&
List<T,Allocator>::operator=( List&& other )
{
    if( this == &other )
        return *this;

    free();
    alloc = std::move(other.alloc);
    
    // link_nodes(other.head.next, other.tail.prev, &head, &tail);
    link_nodes(&head, other.head.next, other.tail.prev, &tail);

    // head.next = other.head.next;
    // other.head.next->prev = &head;
    // tail.prev = other.tail.prev;
    // other.tail.prev->next = &tail;
    // other.head.next = other.tail.prev = nullptr;
    // link_nodes(&other.tail, &other.head, &other.head, &other.tail);
    link_nodes(&other.head, &other.tail, &other.tail, &other.head);
    return *this;
}

template<typename T, typename Allocator>
inline List<T,Allocator>&
List<T,Allocator>::operator=( std::initializer_list<T> init )
{
    free();
    std::pair<Node*,Node*> data = alloc_range(init.begin(), init.end());
    // link_nodes(data.first, data.second, &head, &tail);
    link_nodes(&head, data.first, data.second, &tail);
    // head.next = data.first;
    // tail.prev = data.second;
    return *this;
}
/* ------------------------------------------------------------------------- */

/* capacity */
/* ------------------------------------------------------------------------- */
template<typename T, typename Allocator>
inline typename List<T,Allocator>::size_type List<T,Allocator>::size() noexcept
{
    size_type s{0};
    Node* pnode = head.next;
    while(pnode != &tail){
        ++s;
        pnode = pnode->next;
    }
    return s;
}
/* ------------------------------------------------------------------------- */


/* modifiers */
/* ------------------------------------------------------------------------- */
template<typename T, typename Allocator>
inline typename List<T,Allocator>::iterator
List<T,Allocator>::insert(iterator pos, const T& value )
{
    Node* target = create_node(value);
    // link_nodes(target, target, pos.current->prev, pos.current);
    link_nodes(pos.current->prev, target, target, pos.current);
    // pos.current->prev->next = target;
    // pos.current->prev = target;
    return iterator(target);
}
template<typename T, typename Allocator>
inline typename List<T,Allocator>::iterator
List<T,Allocator>::insert(iterator pos, T&& value)
{
    Node* target = create_node(std::move(value));
    // link_nodes(target, target, pos.current->prev, pos.current);
    link_nodes(pos.current->prev, target, target, pos.current);
    // pos.current->prev->next = target;
    // pos.current->prev = target;
    return iterator(target);
}
template<typename T, typename Allocator>
inline typename List<T,Allocator>::iterator
List<T,Allocator>::insert(iterator pos, size_type count, const T& value)
{
    std::pair<Node*,Node*> data = alloc_n(count, value);
    // link_nodes(data.first, data.second, pos.current->prev, pos.current);
    link_nodes(pos.current->prev, data.first, data.second, pos.current);
    // data.first->prev = pos.current->prev;
    // pos.current->prev->next = data.first;
    // pos.current->prev = data.second;
    // data.second->next = pos.current;
    
    return iterator(data.second);
}
template<typename T, typename Allocator>
    template<typename InputIterator>
inline typename List<T,Allocator>::iterator
List<T,Allocator>::insert(iterator pos, InputIterator first, InputIterator last)
{
    std::pair<Node*,Node*> data = alloc_range(first, last);
    // link_nodes(data.first, data.second, pos.current->prev, pos.current);
    link_nodes(pos.current->prev, data.first, data.second, pos.current);
    // data.first->prev = pos.current->prev;
    // pos.current->prev->next = data.first;
    // pos.current->prev = data.second;
    // data.second->next = pos.current;
    
    return iterator(data.second);
}
template<typename T, typename Allocator>
inline typename List<T,Allocator>::iterator
List<T,Allocator>::insert(iterator pos, std::initializer_list<T> init)
{
    std::pair<Node*,Node*> data = alloc_range(init.begin(), init.end());
    // link_nodes(data.first, data.second, pos.current->prev, pos.current);
    link_nodes(pos.current->prev, data.first, data.second, pos.current);
    // data.first->prev = pos.current->prev;
    // pos.current->prev->next = data.first;
    // pos.current->prev = data.second;
    // data.second->next = pos.current;
    return iterator(data.second);
}

// template<typename T, typename Allocator>
//     template<typename... Args>
// inline typename List<T,Allocator>::iterator
// List<T,Allocator>::emplace( iterator pos, Args&&... args )
// {
//     Node* target = create_node( std::forward<Args>(args)... );
//     link(pos.current->prev, target, target, pos.current);
//     return iterator(target);
// }

template<typename T, typename Allocator>
    template<typename... Args>
inline typename List<T,Allocator>::iterator 
List<T,Allocator>::emplace( iterator pos, Args&&... args )
{
    Node* target = create_node( std::forward<Args>(args)... );
    link_nodes(pos.current->prev, target, target, pos.current);
    return iterator(target);
}

template<typename T, typename Allocator>
inline typename List<T,Allocator>::iterator 
List<T,Allocator>::erase( iterator pos )
{
    Node* retval = pos.current->prev;
    link_nodes(pos.current->prev, pos.current->next);
    alloc.destroy( pos.current );
    alloc.deallocate( pos.current, 1 );
    return iterator(retval);
}
template<typename T, typename Allocator>
inline typename List<T,Allocator>::iterator 
List<T,Allocator>::erase( iterator first, iterator last )
{
    Node* retval = first.current->prev;
    link_nodes(first.current->prev, last.current->next);
    last.current->next = nullptr;
    while( first.current != nullptr ){
        Node* temp = first.current;
        first.current = first.current->next;
        alloc.deestroy( temp );
        alloc.deallocate( temp, 1 );
    }
    return iterator( retval );
}

template<typename T, typename Allocator>
inline void 
List<T,Allocator>::push_back( const T& value )
{
    Node* target = create_node( value );
    link_nodes( tail.prev, target, target, &tail );
}
template<typename T, typename Allocator>
inline void 
List<T,Allocator>::push_back( T&& value )
{
    Node* target = create_node( std::move(value) );
    link_nodes( tail.prev, target, target, &tail );
}
template<typename T, typename Allocator>
    template<typename... Args>
inline void 
List<T,Allocator>::emplace_back( Args&&... args )
{
    Node* target = create_node( std::forward<Args>(args)... );
    link_nodes( tail.prev, target, target, &tail );
}
template<typename T, typename Allocator>
inline void 
List<T,Allocator>::pop_back() noexcept
{
    Node* temp = tail.prev;
    temp->prev->next = &tail;
    tail.prev = temp->prev;
    alloc.destroy( temp );
    alloc.deallocate( temp, 1 );
}

template<typename T, typename Allocator>
inline void 
List<T,Allocator>::push_front( const T& value )
{
    Node* target = create_node( value );
    link_nodes(&head, target, target, head.next);
}
template<typename T, typename Allocator>
inline void 
List<T,Allocator>::push_front( T&& value )
{
    Node* target = create_node( std::move(value) );
    link_nodes(&head, target, target, head.next);
}
template<typename T, typename Allocator>
    template<typename... Args>
inline void 
List<T,Allocator>::emplace_front( Args&&... args )
{
    Node* target = create_node( std::forward<Args>(args)... );
    link_nodes(&head, target, target, head.next);
}
template<typename T, typename Allocator>
inline void 
List<T,Allocator>::pop_front() noexcept
{
    Node* temp = head.next;
    temp->next->prev = &head;
    head.next = temp->next;
    alloc.destroy( temp );
    alloc.deallocate( temp, 1 );
}

template<typename T, typename Allocator>
inline void 
List<T,Allocator>::resize( size_type count )
{
    resize( count, value_type() );
}
template<typename T, typename Allocator>
inline void 
List<T,Allocator>::resize( size_type count, const value_type& value )
{
    size_type cur_size = size();
    if( count > cur_size ){
        insert( end(), (cur_size - count), value );
    }
    else{
        while( count < cur_size-- ){
            pop_back();
        }
    }
}

template<typename T, typename Allocator>
inline void 
List<T,Allocator>::swap( List& other ) noexcept
{
    using std::swap;
    swap( alloc, other.alloc );
    swap( head, other.head );
    swap( tail, other.tail );
}

/* ------------------------------------------------------------------------- */


#endif /* List_HPP_ */
