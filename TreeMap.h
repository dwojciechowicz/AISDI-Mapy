#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
private:
    struct Node
    {
        Node *parent;
        Node *lson;
        Node *rson;
        value_type *data;
        int bf;
        Node()
        {
            rson=nullptr;
            lson=nullptr;
            parent=nullptr;
            data=nullptr;
            bf=0;
        }
        Node(value_type object)
        {
            lson=nullptr;
            rson=nullptr;
            parent=nullptr;
            data=new value_type(object);
            bf=0;
        }
        ~Node()
        {
            lson=nullptr;
            rson=nullptr;
            parent=nullptr;
            delete data;
            bf=0;
        }
        /*key_type getKey()
        {
            return data->first;
        }*/
    };
   /* void removeTree(Node* nd)
    {
        if(nd)
        {
            removeTree(nd->lson);
            removeTree(nd->rson);
            delete nd->data;
            delete nd;
        }
    }*/

private:
    Node *root;
    size_type my_size;
public:

    TreeMap()
    {
        root=nullptr;
        my_size=0;
    }

    TreeMap(std::initializer_list<value_type> list):TreeMap()
    {
        for(auto it=list.begin(); it!=list.end(); ++it)
            operator[](it->first)=it->second;

    }

    TreeMap(const TreeMap& other):TreeMap()
    {
        for (auto it=other.begin(); it!=other.end(); ++it)
            operator[](it->first)=it->second;
    }

    TreeMap(TreeMap&& other):TreeMap()
    {
        
           root=other.root;
           my_size=other.my_size;
           other.root=nullptr;
           other.my_size=0;

        /*std::swap(root,other.root);
        std::swap(my_size,other.my_size);*/
        //*this = std::move(other);
    }
    void clear_all(Node *A)
    {
        if(A)
        {
            clear_all(A->lson);
            clear_all(A->rson);
           // delete A->data;
            delete A;
        }
    }

    ~TreeMap()
    {
        /*size_type s=my_size;
        for(size_type i=0; i<s; ++i)
            remove(begin());*/
       clear_all(root);
        //my_size = 0;
        //removeTree(root);
    }

    TreeMap& operator=(const TreeMap& other)
    {
        if(other==*this) return *this;
        /*size_type s=my_size;
        for(size_type i=0; i<s; ++i)
            remove(begin());*/
        //removeTree(root);
        clear_all(root);
        my_size = 0;
        root = nullptr;
        for(auto i=other.begin(); i!=other.end(); ++i)
            operator[](i->first)=i->second;
        return *this;
    }

    TreeMap& operator=(TreeMap&& other)
    {
        if(other==*this) return *this;
        /*size_type s=my_size;
        for(size_type i=0; i<s; ++i)
            remove(begin());*/
        //removeTree(root);
        clear_all(root);
        my_size = 0;
        root = nullptr;
        /*root=other.root;
        my_size=other.my_size;
        other.root=nullptr;
        other.my_size=0;*/
        std::swap(root, other.root);
        std::swap(my_size, other.my_size);
        return *this;
    }

    bool isEmpty() const
    {
        if(my_size==0) return true;
        else return false;
    }

    void insert(Node *w)
    {
        Node *p, *r;
        bool t;
        ++my_size;
        p = root;
        if (!p)
            root = w;
        else
        {
            while (true)
                if (w->data->first < p->data->first)
                {
                    if (!p->lson)
                    {
                        p->lson = w;
                        break;
                    }
                    p = p->lson;
                }
                else
                {
                    if (!p->rson)
                    {
                        p->rson = w;
                        break;
                    }
                    p = p->rson;
                }

            w->parent = p;
            if (p->bf)
                p->bf = 0;
            else
            {
                if (p->lson == w)
                    p->bf = 1;
                else
                    p->bf = -1;

                r = p->parent;
                t = false;
                while (r)
                {
                    if (r->bf)
                    {
                        t = true;
                        break;
                    };
                    if (r->lson == p)
                        r->bf = 1;
                    else
                        r->bf = -1;

                    p = r;
			r = r->parent;
                }

                if (t)
                {
                    if (r->bf == 1)
                    {
                        if (r->rson == p)
                            r->bf = 0;
                        else if (p->bf == -1)
                            LR(r);
                        else
                            LL(r);
                    }
                    else
                    {

                        if (r->lson == p)
                            r->bf = 0;
                        else if (p->bf == 1)
                            RL(r);
                        else
                            RR(r);
                    }
                }
            }
        }
    }

    mapped_type& operator[](const key_type& key)
    {
        Node *nd = search_node(key);
	  if (nd!= nullptr) {
          return nd->data->second;
      }
	  //trzeba stworzyc nowy element
	  nd = new Node();
	  nd->data=new value_type(key, mapped_type {});
	  insert(nd);
	  return nd->data->second;
        /*
        Node *temp=new Node();
        temp->data=new value_type(key, mapped_type {});
        Node *curpar=root;
        if(curpar==nullptr)
        {
            root=temp;
            ++my_size;
            return temp->data->second;
        }
        while(true)
        {
            if(key==curpar->data->first)
            {
                delete curpar->data;
                curpar->data=temp->data;
                delete temp;
                return curpar->data->second;
            }
            if(key<curpar->data->first)
            {
                if(curpar->lson==nullptr)
                {
                    curpar->lson=temp;
                    break;
                }
                curpar=curpar->lson;
            }
            else
            {
                if(curpar->rson==nullptr)
                {
                    curpar->rson=temp;
                    break;
                }
                curpar=curpar->rson;
            }
        }
//nie znaleziono klucza
        temp->parent=curpar;
        if(curpar->bf)
        {
            curpar->bf=0;
            ++my_size;
            return temp->data->second;
        }
        if(curpar->lson==temp)
        {
            curpar->bf=1;
        }
        else
        {
            curpar->bf=-1;
        }
        Node *pre_par=curpar->parent;
        bool unbalanced=false;
        while(pre_par!=nullptr)
        {
            if(pre_par->bf)
            {
                unbalanced=true;
                break;
            }
            if(pre_par->lson==curpar)
                pre_par->bf=1;
            else
                pre_par->bf=-1;
            curpar=pre_par;
            pre_par=pre_par->parent;
        }
        if(unbalanced==true)
        {
            if(pre_par->bf==1)
            {
                if(pre_par->rson==curpar)
                    pre_par->bf=0;
                else if(curpar->bf==-1)
                    LR(pre_par);
                else
                    LL(pre_par);
            }
            else
            {
                if(pre_par->lson==curpar)
                    pre_par->bf=0;
                else if(curpar->bf==1)
                    RL(pre_par);
                else
                    RR(pre_par);
            }
        }
        ++my_size;
        return temp->data->second;*/
    }

    Node* search_node(key_type key) const
    {
        Node *temp;
        temp=root;
        while((temp!=nullptr)&&(temp->data->first != key))
        {
            if(key<temp->data->first)
                temp=temp->lson;
            else
                temp=temp->rson;
        }
        return temp;
    }

    const mapped_type& valueOf(const key_type& key) const
    {
        Node *temp;
        temp=search_node(key);
        if(temp==nullptr) throw std::out_of_range("Position not found");
        return temp->data->second;
    }

    mapped_type& valueOf(const key_type& key)
    {
        Node *temp;
        temp=search_node(key);
        if(temp==nullptr) throw std::out_of_range("Position not found");
        return temp->data->second;
    }

    const_iterator find(const key_type& key) const
    {
        auto it=begin();
        for(; it!=end(); ++it)
        {
            if(it.tmp->data->first==key)
                return it;
        }
        auto const i=it;
        return i;
    }

    iterator find(const key_type& key)
    {
        auto it=begin();
        for(; it!=end(); ++it)
        {
            if(it.tmp->data->first==key)
                return it;
        }
        return it;
    }

    void RR(Node *A)
    {
        Node *B=A->rson;
        Node *par=A->parent;
        A->rson=B->lson;
        if(A->rson)
            A->rson->parent=A;
        B->lson=A;
        B->parent=par;
        A->parent=B;
        if(par)
        {
            if(par->lson==A)
                par->lson=B;
            else
                par->rson=B;
        }
        else
            root=B;
        if(B->bf==-1)
            A->bf=B->bf=0;
        else
        {
            A->bf=-1;
            B->bf=1;
        }
    }

    void RL(Node *A)
    {
        Node *B=A->rson;
        Node *C=B->lson;
        Node *par=A->parent;
        B->lson=C->rson;
        if(B->lson)
            B->lson->parent=B;
        A->rson=C->lson;
        if(A->rson)
            A->rson->parent=A;
        C->lson=A;
        C->rson=B;
        A->parent=B->parent=C;
        C->parent=par;
        if(par)
        {
            if(par->lson==A)
                par->lson=C;
            else
                par->rson=C;
        }
        else
            root=C;
        if(C->bf==-1)
            A->bf=1;
        else
            A->bf=0;
        if(C->bf==1)
            B->bf=-1;
        else
            B->bf=0;
        C->bf=0;
    }

    void LL(Node *A)
    {
        Node *B=A->lson;
        Node *par=A->parent;
        A->lson=B->rson;
        if(A->lson)
            A->lson->parent=A;
        B->rson=A;
        B->parent=par;
        A->parent=B;
        if(par)
        {
            if(par->lson==A)
                par->lson=B;
            else
                par->rson=B;
        }
        else
            root=B;
        if(B->bf==1)
            A->bf=B->bf=0;
        else
        {
            A->bf=1;
            B->bf=-1;
        }
    }

    void LR(Node *A)
    {
        Node *B=A->lson;
        Node *C=B->rson;
        Node *par=A->parent;
        B->rson=C->lson;
        if(B->rson)
            B->rson->parent=B;
        A->lson=C->rson;
        if(A->lson)
            A->lson->parent=A;
        C->rson=A;
        C->lson=B;
        A->parent=B->parent=C;
        C->parent=par;
        if(par)
        {
            if(par->lson==A)
                par->lson=C;
            else
                par->rson=C;
        }
        else
            root=C;
        if(C->bf==1)
            A->bf=-1;
        else
            A->bf=0;
        if(C->bf==-1)
            B->bf=1;
        else
            B->bf=0;
        C->bf=0;
    }

    Node* remove_node(Node *x)
    {
        --my_size;
        Node  *t,*y,*z;
        bool nest;

        if(x->lson&&x->rson)
        {
            Iterator i(this,x);
            --i;
            y=remove_node(i.tmp);//previous_node(x)
            nest=false;
        }
        else
        {
            if(x->lson)
            {
                y=x->lson;
                x->lson=NULL;
            }
            else
            {
                y=x->rson;
                x->rson=NULL;
            }
            x->bf=0;
            nest=true;
        }
        if(y)
        {
            y->parent=x->parent;
            y->lson=x->lson;
            if(y->lson)
                y->lson->parent=y;
            y->rson=x->rson;
            if(y->rson)
                y->rson->parent=y;
            y->bf=x->bf;
        }
        if(x->parent)
        {
            if(x->parent->lson==x)
                x->parent->lson=y;
            else
                x->parent->rson=y;
        }
        else
            root=y;
        if(nest)
        {
            z=y;
            y=x->parent;
            while(y)
            {
                if(!y->bf)
                {
                    if(y->lson==z)
                        y->bf=-1;
                    else
                        y->bf=1;
                    break;
                }
                else
                {
                    if(((y->bf==1)&&(y->lson==z))||((y->bf==1)&&(y->rson==z)))
                    {
                        y->bf=0;
                        z=y;
                        y=y->parent;
                    }
                    else
                    {
                        if(y->lson==z)
                            t=y->rson;
                        else
                            t=y->lson;
                        if(!t->bf)
                        {
                            if(y->bf==1)
                                LL(y);
                            else
                                RR(y);
                            break;
                        }
                        else if(y->bf==t->bf)
                        {
                            if(y->bf==1)
                                LL(y);
                            else
                                RR(y);
                            z=t;
                            y=t->parent;
                        }
                        else
                        {
                            if(y->bf==1)
                                LR(y);
                            else
                                RL(y);
                            z = y->parent;
                            y = z->parent;
                        }
                    }
                }
            }
        }
        return x;
    }

    void remove(const key_type& key)
    {
        Node *temp;
        temp=search_node(key);
        if(temp==nullptr) throw std::out_of_range("Position not found");
        remove_node(temp);
        //delete temp->data;
        delete temp;
    }

    void remove(const const_iterator& it)
    {
        if(it==end()) throw std::out_of_range("Out of range");
        Node *temp=search_node(it.tmp->data->first);
        if(temp==nullptr) throw std::out_of_range("Position not found");
        remove_node(temp);
        //delete temp->data;
        delete temp;
    }

    size_type getSize() const
    {
        return my_size;
    }

    bool operator==(const TreeMap& other) const
    {
        if(my_size!=other.my_size) return false;
        Iterator temp=begin();
        for(auto it=other.begin(); it!=other.end(); ++it)
        {
            if(*temp!=*it) return false;
            ++temp;
        }
        return true;
    }

    bool operator!=(const TreeMap& other) const
    {
        return !(*this==other);
    }

    iterator begin()
    {
        Node *temp=root;
        if (isEmpty()) return end();
        while (temp->lson!=nullptr)
            temp=temp->lson;
        Iterator it(this,temp);
        return it;
    }

    iterator end()
    {
        Iterator it(this,nullptr);
        return it;
    }

    const_iterator cbegin() const
    {
        Node *temp=root;
        if (isEmpty()) return cend();
        while (temp->lson!=nullptr)
            temp=temp->lson;
        ConstIterator it(this,temp);
        return it;
    }

    const_iterator cend() const
    {
        ConstIterator it(this,nullptr);
        return it;
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename TreeMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename TreeMap::value_type;
    using pointer = const typename TreeMap::value_type*;

    const TreeMap *t;
    Node *tmp;

    explicit ConstIterator()
    {}

    ConstIterator(const TreeMap * ptr,Node * pointer): t(ptr),tmp(pointer)
    {}

    ConstIterator(const ConstIterator& other)
    {
        t=other.t;
        tmp=other.tmp;
    }

    ConstIterator& operator++()
    {
        if(tmp==nullptr) throw std::out_of_range("Out of range");
        if(tmp->rson!=nullptr)
        {
            tmp=tmp->rson;
            while(tmp->lson!=nullptr)
                tmp=tmp->lson;
        }
        else
        {
            while((tmp->parent!=nullptr)&&(tmp->parent->rson==tmp))
            {
                tmp=tmp->parent;
            }
            if(tmp->parent!=nullptr)
                tmp=tmp->parent;
            else
                tmp=nullptr;
        }
        return *this;
    }

    ConstIterator operator++(int)
    {
        if(tmp==nullptr) throw std::out_of_range("Out of range");
        ConstIterator nd=*this;
        if(tmp->rson!=nullptr)
        {
            tmp=tmp->rson;
            while(tmp->lson!=nullptr)
                tmp=tmp->lson;
        }
        else
        {
            while((tmp->parent!=nullptr)&&(tmp->parent->rson==tmp))
            {
                tmp=tmp->parent;
            }
            if(tmp->parent!=nullptr)
                tmp=tmp->parent;
            else
                tmp=nullptr;
        }
        return nd;
    }

    ConstIterator& operator--()
    {
        if (*this == t->begin() || t->my_size <= 0) throw std::out_of_range("decreasing begin or empty map");
        if (tmp == nullptr)
        {
            Node* temp=t->root;
            while(temp->rson!=nullptr)
                temp=temp->rson;
            tmp=temp;
            return *this;
        }
        else if (tmp->lson!=nullptr)
        {
            tmp=tmp->lson;
            while(tmp->rson!=nullptr)
                tmp=tmp->rson;
        }
        else
        {
            while(tmp->parent!=nullptr && tmp->parent->lson==tmp)
                tmp=tmp->parent;
            if (tmp->parent!=nullptr)
                tmp=tmp->parent;
            else
                tmp=nullptr;
        }
        return *this;
    }

    ConstIterator operator--(int)
    {
        if (*this==t->begin() || t->my_size<= 0) throw std::out_of_range("decreasing begin or empty map");
        ConstIterator nd=*this;
        if (tmp==nullptr)
        {
            Node* temp=t->root;
            while(temp->rson!=nullptr)
                temp=temp->rson;
            tmp=temp;
            return nd;
        }
        else if (tmp->lson!=nullptr)
        {
            tmp=tmp->lson;
            while(tmp->rson!=nullptr)
                tmp=tmp->rson;
        }
        else
        {
            while(tmp->parent!=nullptr && tmp->parent->lson==tmp)
                tmp=tmp->parent;
            if (tmp->parent!=nullptr)
                tmp=tmp->parent;
            else
                tmp=nullptr;
        }
        return nd;
    }

    reference operator*() const
    {
        if(tmp==nullptr) throw std::out_of_range("It's a nullptr");
        return *(tmp->data);
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const
    {
        if(tmp==nullptr && other.tmp==nullptr) return true;
        if(tmp==nullptr && other.tmp!=nullptr) return false;
        if(tmp!=nullptr && other.tmp==nullptr) return false;
        if(t==other.t && tmp->data==other.tmp->data) return true;
        else return false;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return !(*this == other);
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename TreeMap::reference;
    using pointer = typename TreeMap::value_type*;

    explicit Iterator()
    {}

    Iterator(TreeMap * ptr=nullptr,Node* tmp=nullptr): ConstIterator(ptr,tmp)
    {}

    Iterator(const ConstIterator& other)
        : ConstIterator(other)
    {}

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif /* AISDI_MAPS_MAP_H */
