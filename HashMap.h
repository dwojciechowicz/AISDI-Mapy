#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>
#include <stack>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Vector = std::vector<value_type>;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:
    std::hash <int> makeHash;
    size_type my_size, size;
    Vector *hmap;

public:
    HashMap()
    {
        size=10000;
        hmap=new Vector[size];
        my_size=0;
    }

    HashMap(std::initializer_list<value_type> list) :HashMap()
    {
        // my_size=list.size();
        //std::hash <int> Hashval;
        for (auto it = list.begin(); it != list.end(); ++it)
            operator[](it->first) = it->second;
        // hmap[Hashval((*it).first)%size].push_back(*it);
    }

    HashMap(const HashMap& other) :HashMap()
    {
        // my_size=other.my_size;
        //  std::hash <int> Hashval;
        for (iterator it = other.begin(); it != other.end(); ++it)
            operator[](it->first) = it->second;
        // hmap[Hashval((*it).first)%size].push_back(*it);
    }

    HashMap(HashMap&& other) :HashMap()
    {
        std::swap(my_size,other.my_size);
        std::swap(hmap,other.hmap);
        size=other.size;
        other.size=0;
    }

    ~HashMap()
    {
        delete[] hmap;
        my_size = 0;
        size = 0;
    }

    HashMap& operator=(const HashMap& other)
    {
        if (other == *this)
            return *this;
        my_size = 0;
        delete [] hmap;
        //std::hash <int> Hashval;
        hmap = new Vector[size];
        for (auto it = other.begin(); it != other.end(); ++it)
            operator[](it->first) = it->second;
        //hmap[Hashval((*it).first)%size].push_back(*it);
        return *this;
    }

    HashMap& operator=(HashMap&& other)
    {
        if (other == *this)
            return *this;
        delete[] hmap;
        hmap = new Vector[size];
        Vector *tmp = hmap;
        hmap = other.hmap;
        other.hmap = tmp;
        //std::swap(my_size,other.my_size);
        //std::swap(hmap,other.hmap);
        //czemu swap z³y?
        my_size = other.my_size;
        other.my_size = 0;
        //std::swap(size,other.size);
        return *this;
    }

    bool isEmpty() const
    {
        if(my_size==0) return true;
        else return false;
    }

    mapped_type& operator[](const key_type& key)
    {
        int nr=makeHash(key) % size;
        for(auto it=hmap[nr].begin(); it!=hmap[nr].end(); ++it)
        {
            if ((*it).first==key)
                return (*it).second;
        }
        hmap[nr].push_back(value_type(key,mapped_type {}));
        ++my_size;
        return hmap[nr].back().second;
    }

    const mapped_type& valueOf(const key_type& key) const
    {
        int nr = makeHash(key) % size;
        for (auto it=hmap[nr].begin(); it!=hmap[nr].end(); ++it)
        {
            if ((*it).first==key)
                return (*it).second;
        }
        throw std::out_of_range("Position not found");
    }

    mapped_type& valueOf(const key_type& key)
    {
        int nr = makeHash(key) % size;
        for (auto it=hmap[nr].begin(); it!=hmap[nr].end(); ++it)
        {
            if ((*it).first==key)
                return (*it).second;
        }
        throw std::out_of_range("Position not found");
    }

    const_iterator find(const key_type& key) const
    {
        int nr=makeHash(key) % size;
        int i=0;
        for (auto it=hmap[nr].begin(); it!=hmap[nr].end(); ++it)
        {
            if ((*it).first==key)
            {
                ConstIterator m(this,nr,i);
                return m;
            }
            ++i;
        }
        return cend();
    }

    iterator find(const key_type& key)
    {
        int nr=makeHash(key) % size;
        int i=0;
        for (auto it=hmap[nr].begin(); it!=hmap[nr].end(); ++it)
        {
            if ((*it).first==key)
            {
                Iterator m(this,nr,i);
                return m;
            }
            ++i;
        }
        return end();
    }

    void Erase(int i, int h)
    {
        std::stack<std::pair<const key_type, mapped_type>> s;
        int y = hmap[h].size()-1;

        while(i < y)
        {
            s.push(hmap[h].back());
            hmap[h].pop_back();
            --y;
        }
        hmap[h].pop_back();
        while(s.size())
        {
            hmap[h].push_back(s.top());
            s.pop();
        }
    }

    void remove(const key_type& key)
    {

        int index = makeHash(key) % size;
        if(isEmpty()||hmap[index].size() == 0) throw std::out_of_range("");
        int i = 0;
        for(auto it = hmap[index].begin(); it != hmap[index].end(); ++it)
        {
            if(hmap[index][i].first == key)
            {
                --my_size;
                Erase(i,index);
                break;
            }
            ++i;
        }
    }

    void remove(const const_iterator& it)
    {
        if(hmap[it.vec].empty() || my_size==0  ||it.pos >= hmap[it.vec].size() || it==this->end())
            throw std::out_of_range("");
        --my_size;
        Erase(it.pos, it.vec);
    }

    size_type getSize() const
    {
        return my_size;
    }

    bool operator==(const HashMap& other) const
    {
        if(my_size!=other.my_size) return false;
        if(size!=other.size) return false;
        for(auto it=begin(),it2=other.begin(); it!=end(); ++it,++it2)
        {
            if(*it!=*it2)
                return false;
        }
        return true;
    }

    bool operator!=(const HashMap& other) const
    {
        return !(*this == other);
    }

    iterator begin()
    {
        if(isEmpty())
        {
            ConstIterator i(this,0,0);
            return i;
        }
        size_type s=0;
        while(s<size && hmap[s].empty())
            ++s;
        Iterator i(this,s,0);
        return i;
    }

    iterator end()
    {
        /*
        int last;
        for(last=size-1; last>=0; --last)
        {
            if(!hmap[last].empty())
                {
                    ++last;
        Iterator i(this,last,0);
        return i;
                }
        }
        ++last;
        Iterator i(this,last,0);
        return i;*/

        /*int v;
                if(isEmpty())
           v=0;
                else
           v=size;
                return Iterator(this,v,0);*/
        if(isEmpty())
        {
            Iterator iter(this,0,0);
            return iter;
        }
        size_type i = size - 1;
        size_type v;
        while (i >= 0 && hmap[i].size() == 0)
            --i;
        v = i + 1;
        Iterator iter(this,v,0);
        return iter;
    }

    const_iterator cbegin() const
    {
        if(isEmpty())
        {
            ConstIterator i(this,0,0);
            return i;
        }
        size_type s=0;
        while(s<size && hmap[s].empty())
            ++s;
        ConstIterator i(this,s,0);
        return i;
    }

    const_iterator cend() const
    {
        /*
        int last;
        for(last=size-1; last>=0; --last)
        {
            if(!hmap[last].empty())
                break;
        }
        ++last;
        ConstIterator i(this,last,0);
        return i;*/
        if(isEmpty())
        {
            ConstIterator ci(this,0,0);
            return ci;
        }
        size_type i = size - 1;
        size_type v;
        while (i >= 0 && hmap[i].size() == 0)
            --i;
        v = i + 1;
        ConstIterator ci(this,v,0);
        return ci;

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
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename HashMap::const_reference;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename HashMap::value_type;
    using pointer = const typename HashMap::value_type*;

public:
    const HashMap *ptr;
    //const std::vector<value_type> *vec;
    size_type vec;
    size_type pos;

    explicit ConstIterator()
    {
        ptr=nullptr;
        //vec=nullptr;
        vec=0;
        pos=0;
    }

//    ConstIterator(const HashMap *a,const std::vector<value_type> *b,int c):ptr(a),vec(b),pos(c)
    ConstIterator(const HashMap *a,int b,int c):ptr(a),vec(b),pos(c)
    {}

    ConstIterator(const ConstIterator& other)
    {
        ptr = other.ptr;
        vec = other.vec;
        pos = other.pos;
    }

    ConstIterator& operator++()
    {
        if (*this==ptr->cend()) throw std::out_of_range("Out of range");
        ++pos;
        if (pos >= (ptr->hmap[vec].size()))
        {
            for (auto i=vec+1; i!=ptr->size; ++i)
            {
                if(!ptr->hmap[i].empty())
                {
                    //pos=ptr->hmap[i].begin();
                    pos=0;
                    vec=i;
                    return *this;
                }
            }
            *this=ptr->cend();
        }
        return *this;
    }

    ConstIterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    ConstIterator& operator--()
    {
        if(*this == ptr->begin())  throw std::out_of_range("Out of range");
        if (pos == 0)
        {
            while (vec > 0 && ptr->hmap[vec].size() == 0)
                --vec;
            pos = ptr->hmap[vec].size() - 1;
        }
        else
            --pos;
        return *this;
    }

    ConstIterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    reference operator*() const
    {
        if(ptr->hmap[vec].size()==0) throw std::out_of_range("Out of range");
        if (*this == ptr->end()) throw std::out_of_range("Out of range");
        return ptr->hmap[vec][pos];
    }

    pointer operator->() const
    {
        return &this->operator*();
    }

    bool operator==(const ConstIterator& other) const
    {
        if(ptr==other.ptr&&vec==other.vec&&pos==other.pos) return true;
        else return false;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return !(*this == other);
    }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
    using reference = typename HashMap::reference;
    using pointer = typename HashMap::value_type*;

    explicit Iterator()
    {}

    Iterator(const HashMap *a, int b, int c): ConstIterator(a, b, c)
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

#endif /* AISDI_MAPS_HASHMAP_H */
