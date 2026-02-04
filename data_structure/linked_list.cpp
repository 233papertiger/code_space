#include <iostream>
#include <new>
using namespace std;


class node {
    public:
        int val;
        node *next;
        node(): val(0), next(nullptr) {}
        node(int v): val(v), next(nullptr) {}
};

class link_list{
    public:
        int get_value(int idx);
        bool add_at_head(int val);
        bool add_at_tail(int val);

        bool add_at_index(int idx, int val);
        bool delete_at_index(int idx);
        void print_all_elements();
        link_list(): _size(0), head(nullptr) {
            head = new (std::nothrow) node();
        }
        link_list(const link_list &l);
        link_list& operator=(const link_list &other);
        ~link_list();

    private:
        int _size;   // 链表元素个数
        node *head;  // 虚拟头节点
};

link_list::link_list(const link_list &l): _size(0), head(nullptr) {
    head = new (std::nothrow) node();
    if ( !head ) throw std::bad_alloc();

    node *cur = head, *cur_l = l.head->next;
    for ( int i = 1; i <= l._size; ++i ) {
        node *tmp = new (std::nothrow)node(cur_l->val);
        if ( !tmp ) {
            while ( head->next ) {
                node* to_delete = head->next;
                head->next = to_delete->next;
                delete to_delete;
            }
            delete head;
            head = nullptr;
            throw std::bad_alloc();
        }
        cur_l = cur_l->next;
        cur->next = tmp;
        cur = cur->next;
    }
    _size = l._size;

}

link_list& link_list::operator=(const link_list &other) {
    // 自我赋值检查
    if (this == &other) {
        return *this;
    }

    // 先创建新副本（异常安全：如果失败，原对象不变）
    node *new_head = new (std::nothrow) node();
    if (!new_head) throw std::bad_alloc();

    node *cur = new_head;
    node *cur_other = other.head->next;

    for (int i = 1; i <= other._size; ++i) {
        node *tmp = new (std::nothrow) node(cur_other->val);
        if (!tmp) {
            // 清理已分配的节点
            while (new_head->next) {
                node* to_delete = new_head->next;
                new_head->next = to_delete->next;
                delete to_delete;
            }
            delete new_head;
            throw std::bad_alloc();
        }
        cur->next = tmp;
        cur = cur->next;
        cur_other = cur_other->next;
    }

    // 新副本创建成功，释放原有资源
    node *old_head = head;
    for (int i = 1; i <= _size; ++i) {
        node *to_delete = head->next;
        head->next = to_delete->next;
        delete to_delete;
    }
    delete old_head;

    // 使用新资源
    head = new_head;
    _size = other._size;

    return *this;
}

int link_list::get_value(int idx) {
    if ( idx > _size || idx <= 0 ) return -1;
    node *cur = head;
    for ( int i = 1; i <= idx; ++i ) {
        cur = cur->next;
    }
    return cur->val;
}

bool link_list::add_at_head(int val) {
    node *node1 = new (std::nothrow) node(val);
    if ( !node1 ) return false;

    if ( _size > 0 ) {
        node *tmp = head->next;
        head->next = node1;
        node1->next = tmp;
    } else {
        head->next = node1;
    }
    _size += 1;
    return true;
}

bool link_list::add_at_tail(int val) {
    node *node1 = new (std::nothrow) node(val);
    if ( !node1 ) return false;

    node *cur = head;
    for ( int i = 1; i <= _size; ++i )
        cur = cur->next;

    cur->next = node1;
    _size += 1;
    return true;
}

bool link_list::add_at_index(int idx, int val) {
    if ( idx <= 0 ) {
        bool ret = add_at_head(val);
        cout << "add node status " << ret << endl;
        return ret;
    }
    else if ( idx > _size ) {
        bool ret = add_at_tail(val);
        cout << "add node status " << ret << endl;
        return ret;
    }
    else {
        node *node1 = new (std::nothrow) node(val);
        if ( !node1 )  return false;
        node *cur = head;
        for ( int i=1; i < idx; ++i ) {
            cur = cur->next;
        }
        
        node *tmp = cur->next;
        cur->next = node1;
        node1->next = tmp;
        _size += 1;
    }
    return true;
}

bool link_list::delete_at_index(int idx) {
    if ( idx < 1 || idx > _size ) 
        return false;

    node *cur = head;
    for ( int i = 1; i < idx; ++i ) 
        cur = cur->next;

    node *tmp = cur->next->next;
    node *tar = cur->next;
    delete tar;

    cur->next = tmp;
    _size -= 1;
    return true;
}

void link_list::print_all_elements() {
    node *cur = head->next;
    cout << "here are elements at list: " << endl;
    for ( int i = 1; i <= _size; ++i ) {
        cout << cur->val << ", " ;
        cur = cur->next;
    }
    cout << endl;
}



link_list::~link_list() {
    node *cur = head->next;
    cout << "~link_list()" << endl;
    
    for ( int i = 1; i <= _size; ++i ) {
        node *tmp = cur;
        cout << cur->val << ", " << endl;
        cur = cur->next;
        delete tmp;
    }
    delete head;
    _size = 0;
}


int main() {
    link_list list;
    for ( int i=1; i<11; ++i )
        list.add_at_tail(i);
    
    list.print_all_elements();
    list.delete_at_index(5);
    list.print_all_elements();

    link_list list1(list);
    list1.print_all_elements();
    return 0;
}