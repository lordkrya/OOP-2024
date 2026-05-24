#ifndef ORDEREDTABLE_DEFENDER
#define ORDEREDTABLE_DEFENDER

#include <concepts>
#include <limits>
#include <algorithm>
#include <compare>
#include <memory>

#include "OTableIterator.hpp"

namespace table {

template<typename Tk, typename T>
concept copyConstructible = std::copy_constructible<Tk> && std::copy_constructible<T>;
template<typename Tk, typename T>
concept moveConstructible = std::move_constructible<Tk> && std::move_constructible<T>;

template<std::totally_ordered Tk, typename T>
struct Elem {
    private:
        Tk key_;
        T val_;

        static void swap(Elem& first, Elem& second)
        requires std::move_constructible<T> {
            Tk tkTemp = std::move(first.key_);
            T tTemp = std::move(first.val_);

            first.key_ = std::move(second.key_);
            first.val_ = std::move(second.val_);

            second.key_ = std::move(tkTemp);
            second.val_ = std::move(tTemp);
        }

    public:
        Elem(): key_(), val_() {}

        template<typename keyArg, typename valArg>
        Elem(keyArg&& key, valArg&& val):
        key_(std::forward<keyArg>(key)), val_(std::forward<valArg>(val)) {}

        Elem(const Elem& other) requires std::copy_constructible<Tk> && std::copy_constructible<T>:
        key_(other.key_), val_(other.val_) {}

        Elem(Elem&& other) noexcept(std::is_nothrow_move_constructible_v<Tk> && std::is_nothrow_move_constructible_v<T>)
        requires std::move_constructible<T> && std::move_constructible<T>:
        key_(std::move(other.key_)), val_(std::move(other.val_)) {}

        Elem& operator = (const Elem& other)
        requires copyConstructible<Tk, T> {
            Elem temp(other);
            swap(*this, temp);
            return *this;
        }
        Elem& operator = (Elem&& other)
        requires std::move_constructible<T> {
            swap(*this, other);
            return *this;
        }

        ~Elem() = default;

        const Tk& getKey() const noexcept {return key_;}
        T& getVal() noexcept {return val_;}

        bool operator == (const Elem& other) const noexcept {return key_ == other.key_;}
        std::partial_ordering operator <=> (const Elem& other) const noexcept {return key_ <=> other.key_;}
        bool operator == (const Tk& key) const noexcept {return key_ == key;}
        std::partial_ordering operator <=> (const Tk key) const noexcept {return key_ <=> key;}
};

template<std::random_access_iterator It>
class InitializationVector {
    public:

        InitializationVector(): size_(0), table_(), arr_(nullptr) {};
        InitializationVector(It table, size_t size): size_(size), table_(table), arr_(new bool[size]) {
            for(size_t i = 0; i < size; ++i)
                arr_[i] = false;
        }
        InitializationVector(const InitializationVector& other): 
        size_(other.size_), table_(other.table_), arr_(new bool[other.size_]) {
            std::copy(other.arr_, other.arr_ + size_, arr_);
        }

        InitializationVector(InitializationVector&& other) noexcept:
        size_(other.size_), table_(other.table_), arr_(other.arr_) {
            other.size_ = 0;
            other.table_ = It();
            other.arr_ = nullptr;
        }

        ~InitializationVector() noexcept {
            delete[] arr_;
        }

        friend void swap(InitializationVector& first, InitializationVector& second) noexcept {
            std::swap(first.size_, second.size_);
            std::swap(first.table_, second.table_);
            std::swap(first.arr_, second.arr_);
        }

        InitializationVector& operator = (const InitializationVector& other) {
            InitializationVector temp(other);
            swap(*this, temp);
            return *this;
        }

        InitializationVector& operator = (InitializationVector&& other) noexcept {
            swap(*this, other);
            return *this;
        }

        size_t getSize() const noexcept {return size_;}

        bool checkPos(It ptr) const noexcept {return arr_[ptr - table_];}
        void changePos(It ptr) noexcept {arr_[ptr - table_] = !arr_[ptr - table_];}
        void changePos(It first, It second) const noexcept {
            for (; first != second; ++first) {
                arr_[first - table_] = !arr_[first - table_];
            }
        }

    private:

        size_t size_;
        It table_;
        bool* arr_;
};

template<std::totally_ordered Tk, typename T>
class OrderedTable {

    public:

        typedef Tk key_type;
        typedef T mapped_type;
        typedef Elem<Tk, T> value_type;
        typedef Elem<Tk, T>& reference;
        typedef const Elem<Tk, T>& const_reference;
        typedef std::less<Tk> key_compare;
        typedef std::less<Elem<Tk, T>> value_compare;
        typedef OTableIterator<Tk, T, false> iterator;
        typedef OTableIterator<Tk, T, true> const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        key_compare key_comp() const noexcept {
            return key_compare();
        }

        value_compare value_comp() const noexcept {
            return value_compare();
        }

        size_type size() const noexcept {
            return size_;
        }

        size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max();
        }

        bool empty() const noexcept {
            return size_ == 0;
        }

        template<std::totally_ordered Tk2, typename T2>
        friend void swap(OrderedTable<Tk2, T2>& first, OrderedTable<Tk2, T2>& second) noexcept;

        iterator begin() noexcept {
            return iterator(arr_);
        }

        iterator end() noexcept {
            return iterator(arr_ + size_);
        }

        const_iterator begin() const noexcept {
            return const_iterator(arr_);
        }

        const_iterator end() const noexcept {
            return const_iterator(arr_ + size_);
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(arr_);
        }

        const_iterator cend() const noexcept {
            return const_iterator(arr_ + size_);
        }

        // Благодаря p, присутствует амортизирующая постоянная
        template<typename ... Args>
        iterator emplace_hint(const_iterator p, Args&& ... args) requires 
        std::constructible_from<value_type, Args ...> && std::move_constructible<T> && std::move_constructible<Tk> {
            ptrdiff_t delta = p - begin();

            value_type newEl(std::forward<Args>(args)...);
            checkLimitAndResize_(1);

            p = begin() + delta;

            iterator it;
            if (checkPos_(p, newEl))
                it = iterator(const_cast<Elem<Tk, T>*>(p.value_));
            else
                it = std::lower_bound(begin(), end(), newEl);

            moveBackwardAndInit_(it, end(), end() + 1);     // При исключении поведение не опр.
            if (initVec_.checkPos(it))
                *it = std::move(newEl);
            else {
                new(it.value_) value_type(std::move(newEl));    // При исключении поведение не опр.
                initVec_.changePos(it);
            }
            size_++;
            return it;
        }

        template<typename ... Args>
        iterator emplace(Args&& ... args) requires std::constructible_from<value_type, Args ...> && 
        std::move_constructible<T> && std::move_constructible<Tk> {
            value_type newEl(std::forward<Args>(args)...);
            checkLimitAndResize_(1);
            iterator it = std::lower_bound(begin(), end(), newEl);

            moveBackwardAndInit_(it, end(), end() + 1);  // При исключении поведение не опр.
            if (initVec_.checkPos(it))
                *it = std::move(newEl);
            else {
                new(it.value_) value_type(std::move(newEl));        // При исключении поведение не опр.
                initVec_.changePos(it);
            }
            size_++;
            return it;
        }

        iterator insert(const_iterator p, const value_type& t)
        requires std::move_constructible<T> && copyConstructible<Tk, T> {
            return emplace_hint(p, t);
        }
        
        iterator insert(const value_type& t)
        requires std::move_constructible<T> && copyConstructible<Tk, T> {
            return emplace(t);
        }

        iterator insert(const_iterator p, value_type&& t)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            return emplace_hint(p, std::move(t));
        }

        iterator insert(value_type&& t)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            return emplace(std::move(t));
        }

        iterator insert(const_iterator p, size_type n, const value_type& t)
        requires std::move_constructible<T> && copyConstructible<Tk, T> {
            if (n == 0)
                return iterator(const_cast<value_type*>(p.value_));
            ptrdiff_t delta = p - begin();

            checkLimitAndResize_(n);

            p = begin() + delta;
            iterator it;
            if (checkPos_(p, t))
                it = iterator(const_cast<Elem<Tk, T>*>(p.value_));
            else 
                it = std::lower_bound(begin(), end(), t);

            moveBackwardAndInit_(it, end(), end() + n);   // При исключении поведение не опр.
            fillAndInit_(it, it + n, t);                  // При исключении поведение не опр.
            size_ += n;
            return it;
        }

        iterator insert(size_type n, const value_type& t)
        requires std::move_constructible<T> && copyConstructible<Tk, T> {
            if (n == 0)
                return end();
            checkLimitAndResize_(n);

            iterator it = std::lower_bound(begin(), end(), t);
            moveBackwardAndInit_(it, end(), end() + n);   // При исключении поведение не опр.
            fillAndInit_(it, it + n, t);                  // При исключении поведение не опр.
            size_ += n;
            return it;
        }

        template<std::input_iterator It>
        void insert(It i, It j) requires std::same_as<std::iter_value_t<It>, value_type> &&
        std::move_constructible<T> && std::move_constructible<Tk> {
            if (i == j)
                return;

            for (; i != j; i++)
                insert(cend(), *i);
        }

        void insert(std::initializer_list<value_type> il)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            insert(std::move_iterator(il.begin()), std::move_iterator(il.end()));
        }

        void merge(OrderedTable& other) 
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            insert(std::move_iterator(other.begin()), std::move_iterator(other.end()));
        }

        iterator erase(const_iterator q)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            iterator _q(const_cast<Elem<Tk, T>*>(q.value_));
            if (q >= begin() && q < end()) {
                moveAndInit_(_q + 1, end(), _q);
                size_--;
            }
            return _q;
        }

        iterator erase(const_iterator q1, const_iterator q2)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            iterator _q1(const_cast<Elem<Tk, T>*>(q1.value_));
            iterator _q2(const_cast<Elem<Tk, T>*>(q2.value_));
            if (q1 >= begin() && q2 <= end()) {
                moveAndInit_(_q2, end(), _q1);
                size_ -= (q2 - q1);
            }
            return _q2;
        }

        void clear()
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            erase(begin(), end());
        }

        template<std::input_iterator It>
        void assign(It i, It j)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            OrderedTable newTable = {};
            newTable.insert(i, j);   // Утечки не будет, т.к. вызовется деструктор
            *this = std::move(newTable);
        }

        void assign(std::initializer_list<value_type> il)
        requires std::move_constructible<T> && std::move_constructible<Tk> {
            OrderedTable newTable = {};
            newTable.insert(std::move(il));  // Утечки не будет, т.к. вызовется деструктор
            *this = std::move(newTable);
        }

        void assign(size_type n, const value_type& t)
        requires copyConstructible<Tk, T> {
            OrderedTable newTable = {};
            newTable.insert(n, t);   // Утечки не будет, т.к. вызовется деструктор
            *this = std::move(newTable);
        }

        OrderedTable() noexcept: arr_(nullptr), size_(0), capacity_(0), initVec_() {}
        
        OrderedTable(const OrderedTable& other)
        requires copyConstructible<Tk, T>: OrderedTable() {
            insert(other.cbegin(), other.cend());
        }

        OrderedTable(OrderedTable&& other) noexcept: OrderedTable() {
            swap(*this, other);
        }

        OrderedTable(size_type n, const value_type& t)
        requires copyConstructible<Tk, T>: OrderedTable() {
            insert(n, t);
        }

        template<std::forward_iterator It>
        OrderedTable(It i, It j)
        requires copyConstructible<Tk, T> && std::move_constructible<T>: OrderedTable() {
            insert(i, j);
        }

        OrderedTable(std::initializer_list<value_type> il)
        requires std::move_constructible<T> && std::copy_constructible<Tk>: OrderedTable() {
            insert(std::move(il));
        }

        ~OrderedTable() {
            free_(begin(), begin() + capacity_);
            ::operator delete(static_cast<void*>(arr_));
        }
    
        OrderedTable& operator = (const OrderedTable& other)
        requires std::move_constructible<T> && copyConstructible<Tk, T> {
            OrderedTable temp(other);
            swap(*this, temp);
            return *this;
        }

        OrderedTable& operator = (OrderedTable&& other) {
            swap(*this, other);
            return *this;
        }

        OrderedTable& operator = (std::initializer_list<T> il)
        requires std::move_constructible<T> && std::copy_constructible<Tk> {
            assign(std::move(il));
            return *this;
        }

        bool operator == (const OrderedTable& other) const {
            return size_ == other.size_ &&
                std::equal(begin(), end(), other.begin(), other.end());
        }

        iterator find(const key_type& key) {
            return std::lower_bound(begin(), end(), key);
        }

        const_iterator find(const key_type& key) const {
            return std::lower_bound(begin(), end(), key);
        }

        reference operator [] (size_type n) {
            return *(begin() + n);
        }

        const_reference operator [] (size_type n) const {
            return *(begin() + n);
        }

        bool contains(const key_type& key) {
            return find(key) != end();
        }

        iterator lower_bound(const key_type& key) {
            return std::lower_bound(begin(), end(), key);
        }
        
        const_iterator lower_bound(const key_type& key) const {
            return std::lower_bound(begin(), end(), key);
        }

        iterator upper_bound(const key_type& key) {
            return std::upper_bound(begin(), end(), key);
        }

        const_iterator upper_bound(const key_type& key) const {
            return std::upper_bound(begin(), end(), key);
        }

        std::pair<iterator, iterator> equal_range(const key_type& key) {
            return std::make_pair(lower_bound(key), upper_bound(key));
        }

        std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
            return std::make_pair(lower_bound(key), upper_bound(key));
        }

        size_type count(const key_type& key) const {
            return upper_bound(key) - lower_bound(key);
        }

    private:

        value_type *arr_;
        size_type size_;
        size_type capacity_;
        InitializationVector<iterator> initVec_;

        OrderedTable(value_type *arr, size_type newSize, size_type newCapacity):
        arr_(arr), size_(newSize), capacity_(newCapacity), initVec_(iterator(arr), newCapacity) {}

        bool checkPos_(const_iterator it, const value_type& el) {
            if (empty())
                return it == end();

            if (it == end())
                return el >= *(it.operator-(1));

            if (it == begin())
                return el <= *it;

            if (begin() < it, it < end())
                return (el <= (*it) && el >= *(it.operator-(1)));
            return false;
        }

        // Возможны throw от оператора new
        static value_type *allocate_(size_type capacity) {
            value_type *newArr = static_cast<value_type*>(::operator new(sizeof(value_type) * capacity));
            return newArr;
        }

        // Возможны throw от перемещающего конструктора value_type
        iterator moveAndInit_(iterator first, iterator last, iterator it) {
            if (first < last) {
                for (; first != last; first++, it++) {
                    if (initVec_.checkPos(it))
                        *it = std::move(*first);
                    else {
                        new(it.value_) value_type{std::move(*first)};
                        initVec_.changePos(it);
                    }
                }
            }
            return it;
        }

        // Возможны throw от перемещающего конструктора value_type
        iterator moveBackwardAndInit_(iterator first, iterator last, iterator it) {
            if (first < last) {
                while (first != last) {
                    if (initVec_.checkPos(--it))
                        *it = std::move(*(--last));
                    else {
                        new(it.value_) value_type{std::move(*(--last))};
                        initVec_.changePos(it);
                    }
                }
            }
            return it;
        }

/*        // Возможны throw от копирующего конструктора value_type
        static iterator copyAndInit_(const_iterator first, const_iterator last, iterator it) {
            if (first < last) {
                for (; first != last; first++, it++)
                    new(it.value_) value_type{*first};
            }
            return it;
        }

        // Возможны throw от копирующего конструктора value_type
        static iterator copyBackwardAndInit_(const_iterator first, const_iterator last, iterator it) {
            if (first < last) {
                while (first != last)
                    new((--it).value_) value_type{*(--last)};
            }
            return it;
        }*/

        // Возможны throw от копирующего конструктора value_type
        void fillAndInit_(iterator first, iterator last, const value_type& el) {
            if (first < last) {
                for(; first != last; first++) {
                    if (initVec_.checkPos(first))
                        *first = el;
                    else {
                        new(first.value_) value_type{el};
                        initVec_.changePos(first);
                    }
                }
            }
        }

        // Сильная защита, вы же не собирались кидать исключение в деструкторе... Не собирались ведь?
        void free_(iterator first, iterator last) noexcept {
            if (first < last)
                for (; first != last; first++)
                    if (initVec_.checkPos(first)) {
                        (*first).~value_type();
                        initVec_.changePos(first);
                    }
        }

        // Сильная защита
        void resize_(size_type newCapacity) 
        requires std::is_nothrow_move_constructible_v<value_type> {
            OrderedTable newTable(allocate_(newCapacity), std::min(newCapacity, size_), newCapacity);
            std::uninitialized_move(arr_, arr_ + std::min(newCapacity, size_), newTable.arr_);
            newTable.initVec_.changePos(newTable.begin(), newTable.end());

            (*this) = std::move(newTable);
        }

        void resize_(size_type newCapacity) 
        requires (!std::is_nothrow_move_constructible_v<value_type>) {
            OrderedTable newTable(allocate_(newCapacity), std::min(newCapacity, size_), newCapacity);
            std::uninitialized_copy(arr_, arr_ + std::min(newCapacity, size_), newTable.arr_);
            newTable.initVec_.changePos(newTable.begin(), newTable.end());

            (*this) = std::move(newTable);
        }

        // Сильная защита
        void checkLimitAndResize_(size_type newElement) {
            if (size_ + newElement > capacity_) {
                resize_((size_ + newElement) * 2);
            }
        }

};

}

#include "../orderedTable.tpp"

#endif
