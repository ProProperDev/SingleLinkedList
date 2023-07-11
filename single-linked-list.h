#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <iostream>

template <typename Type>
class SingleLinkedList {


   // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template<typename ValueType>
    class BasicIterator {

        // Разрешаем SingleLinkedList обращаться к приватной области
        friend class SingleLinkedList;
        explicit BasicIterator(Node* node)
        :node_(node) {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

         BasicIterator(const BasicIterator<Type>& other) noexcept {
           node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (node_ == rhs.node_);
        }
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;      
        }

        BasicIterator& operator++() noexcept {
        assert(node_ != nullptr);
        node_ = (*(node_)).next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
        auto copy_this(*this);
        node_ = (*(node_)).next_node;
            return copy_this;
        }

        [[nodiscard]] reference operator*() const noexcept {
        assert(node_ != nullptr);
            return (*(node_)).value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
        assert(node_ != nullptr);
            return &((*(node_)).value);
        }

    private:
        Node* node_ = nullptr;
    };


public:
  SingleLinkedList()
  : head_(Node())
  , size_(0u) {
  }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

     // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator (&(head_));
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return before_begin();
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        Iterator end_ ;
        end_.node_ = this->head_.next_node;
        while (end_.node_ != nullptr) {
            ++end_;        
        }
        return end_;       
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        ConstIterator end_;
        end_.node_ = this->head_.next_node;
        while (end_.node_ != nullptr) {
            ++end_;        
        }
        return end_;
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(begin());
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(end());
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
       if (head_.next_node == nullptr) {
        return true;       
       }
       return false;        
    }

    void PushFront(const Type& value) {
       head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* new_node = new Node(value, (*(pos.node_)).next_node);
        (*(pos.node_)).next_node = new_node;
        ++size_;
        return Iterator(new_node);       
    }

    void PopFront() noexcept {
       if (IsEmpty()) {
        return;       
       }
     EraseAfter(before_begin());
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);      
        Node* delete_elem = (*(pos.node_)).next_node;
        (*(pos.node_)).next_node = (*(delete_elem)).next_node;
        Node* return_ptr = (*(pos.node_)).next_node;
        delete delete_elem;
        --size_;
        return Iterator(return_ptr);
    }


    // Очищает список за время O(N)
    void Clear() noexcept {
       while(head_.next_node!=nullptr) {
        auto delet_node = head_.next_node;       
        head_.next_node =  head_.next_node->next_node;
        delete delet_node;    
       }
       size_ = 0u;       
    }

    ~SingleLinkedList() {
        Clear();   
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        auto pos = before_begin();
        for(auto value : values) {
            pos = InsertAfter(pos, value);
        } 
    }

    SingleLinkedList(const SingleLinkedList& other) {
       SingleLinkedList copy_other;
       auto pos = copy_other.before_begin();
        for(auto node_ : other) {
            pos = copy_other.InsertAfter(pos, node_.value);
        }
        swap(copy_other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
      assert(head_.next_node != rhs.head_.next_node);
      SingleLinkedList Copy_rhs (rhs);
      swap(Copy_rhs);
      return *this;    
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
   lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(lhs.GetSize() != rhs.GetSize()) {
        return true;
    }
    return (!(lhs==rhs)); 
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
   return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
   return !(lhs>rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs<rhs);
}







