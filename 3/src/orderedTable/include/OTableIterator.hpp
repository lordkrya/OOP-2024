#ifndef OTABLEITERATOR_DEFENDER
#define OTABLEITERATOR_DEFENDER

#include <iterator>

#include "orderedTable.hpp"

namespace table {

template<std::totally_ordered Tk, typename T>
struct Elem;

template<std::totally_ordered Tk, typename T>
class OrderedTable;

template<std::totally_ordered Tk, typename T, bool is_const = true>
class OTableIterator {

    public:

        // Соблюдение требований std::iterator_traits
        typedef ptrdiff_t difference_type;
        typedef Elem<Tk, T> value_type;
        typedef std::conditional_t<is_const, const Elem<Tk, T>*, Elem<Tk, T>*> pointer;
        typedef std::conditional_t<is_const, const Elem<Tk, T>&, Elem<Tk, T>&> reference;
        typedef std::random_access_iterator_tag iterator_category;

        // Соблюдение требований concept std::moveble
        template<bool other_const>
        OTableIterator(const OTableIterator<Tk, T, other_const>& other) noexcept
        requires (is_const >= other_const):
            value_(other.value_) {}

        template<bool other_const>
        OTableIterator& operator = (const OTableIterator<Tk, T, other_const>& other) noexcept
        requires (is_const >= other_const) {
            value_ = other.value_;
            return *this;
        }

        // Соблюдение concept std::weakly_incrementable
        OTableIterator& operator ++ () noexcept {
            value_++;
            return *this;
        }

        OTableIterator operator ++ (int) noexcept {
            OTableIterator res(value_);
            value_++;
            return res;
        }

        // Соблюдение concept std::input_or_output_iterator
        reference operator * () const noexcept {
            return *value_;
        }
        // Опционально
        pointer operator -> () const noexcept {
            return value_;
        }

        // Уже выполняется concept std::input_iterator
        // Соблюдение concept std::forward_iterator
        OTableIterator() noexcept: value_(nullptr) {}

        template<bool other_const>
        bool operator == (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ == other.value_;
        }

        // Соблюдение concept bidirectional_iterator
        OTableIterator& operator -- () noexcept {
            value_--;
            return *this;
        }

        OTableIterator operator -- (int) noexcept {
            OTableIterator res(value_);
            value_--;
            return res;
        }

        // Соблюдение concept random_access_iterator
        OTableIterator& operator += (ptrdiff_t n) noexcept {
            value_ += n;
            return *this;
        }

        OTableIterator operator + (ptrdiff_t n) const noexcept {
            return OTableIterator(value_ + n);
        }

        friend OTableIterator operator + (ptrdiff_t n, const OTableIterator& other) noexcept { 
            return OTableIterator(other.value_ + n);
        }

        OTableIterator& operator -= (ptrdiff_t n) noexcept {
            value_ -= n;
            return *this;
        }

        OTableIterator operator - (ptrdiff_t n) const noexcept {
            return OTableIterator(value_ - n);
        }

        reference operator [] (ptrdiff_t n) const {
            return *(value_ + n);
        }

        template<bool other_const>
        bool operator < (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ < other.value_;
        }

        template<bool other_const>
        bool operator > (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ > other.value_;
        }

        template<bool other_const>
        bool operator <= (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ <= other.value_;
        }

        template<bool other_const>
        bool operator >= (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ >= other.value_;
        }

        template<bool other_const>
        std::strong_ordering operator <=> (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ <=> other.value_;
        }

        template<bool other_const>
        ptrdiff_t operator - (const OTableIterator<Tk, T, other_const>& other) const noexcept {
            return value_ - other.value_;
        }

        explicit operator bool() {
            return value_ != nullptr;
        }

        template<std::totally_ordered Tk2, typename T2, bool is_const2>
        friend void swap(OTableIterator<Tk2, T2, is_const2>& first, OTableIterator<Tk2, T2, is_const2>& second) noexcept;

    private:

        pointer value_;

        OTableIterator(pointer value):
            value_(value) {}

        friend OrderedTable<Tk, T>;
        friend OTableIterator<Tk, T, !is_const>;
};

static_assert(std::random_access_iterator<OTableIterator<char, int, true>>);
static_assert(std::random_access_iterator<OTableIterator<char, int, false>>);

}

#include "../OTableIterator.tpp"

#endif
