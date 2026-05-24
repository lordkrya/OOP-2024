#ifndef BINARYSIGNAL_DEFENDER
#define BINARYSIGNAL_DEFENDER

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

#include "utilities/utilities.h"
#include "signals/signalStatus.h"


namespace signals {

/**
 * @brief Последовательность бинарных сигналов
 * 
 * Корректное состояние объекта: capacity_ >= size_, capacity_ > 0 и signs_ != nullptr;
 */
class BinarySignal{
    private:
        SignalStatus *signs_;   ///< Динамический массив единичных бинарных сигналов
        size_t size_;           ///< Количество сигналов в массиве
        size_t capacity_;       ///< Выделенная память под массив

        size_t signalSearchIndex(unsigned long int time, unsigned long int &now) const;
    public:
        static const size_t DEFAULT_CAPACITY = 8;   ///< Выделяемая память для массивов изначально

        /**
         * @brief Конструктор по умолчанию
         * 
         * signs_ = new SignalStatus[DEFAULT_CAPACITY] {}, size_ = 0, capacity_ = DEFAULT_CAPACITY.
         */
        BinarySignal(): signs_(new SignalStatus[DEFAULT_CAPACITY] {}), size_(0), capacity_(DEFAULT_CAPACITY) {}

        /**
         * @brief Конструктор со сначением выделяемой памяти под массив
         * 
         * При size == 0: size_ = 1
         * 
         * @param size - размер выделяемой памяти
         */
        explicit BinarySignal(size_t size): 
            signs_(new SignalStatus[size ? size : DEFAULT_CAPACITY] {}), size_(0), capacity_(size ? size : DEFAULT_CAPACITY) {}

        /**
         * @brief Конструктор с первым единичным бинарным сигналом
         * 
         * signs_ = new SignalStatus[DEFAULT_CAPACITY] {sign}, size_ = 1, capacity_ = DEFAULT_CAPACITY
         * 
         * @param sign - экземпляр единичного бинарного сигнала
         */
        explicit BinarySignal(const SignalStatus &sign):
            signs_(new SignalStatus[DEFAULT_CAPACITY] {sign}), size_(1), capacity_(DEFAULT_CAPACITY) {}

        /**
         * @brief Конструктор со сначениями первого единичного бинарного сигнала
         * 
         * signs_ = new SignalStatus[DEFAULT_CAPACITY] {SignalStatus(status, time)},  size_ = 1, capacity_ = DEFAULT_CAPACITY
         * 
         * @param status - статус первого сигнала
         * @param time - длительность первого сигнала
         */
        BinarySignal(bool status, unsigned long int time): 
            signs_(new SignalStatus[DEFAULT_CAPACITY] {SignalStatus(status, time)}), size_(1), capacity_(DEFAULT_CAPACITY) {}

        /**
         * @brief Шаблонный конструтор работающий с итераторами
         * 
         * Если элементы контейнера соответствуют типу char, то status_ = *start != '0',
         * Иначе status_ = *start != 0.
         * 
         * @tparam Iterator - для итераторов должны быть определены операторы сравнения и разыменования
         * @param start - итератор указывающий на начало контейнера
         * @param end - итератор указывающий на конец контейнера
         */
        template <typename Iterator>
        BinarySignal(Iterator start, Iterator end): 
            signs_(new SignalStatus[DEFAULT_CAPACITY] {}), size_(0), capacity_(DEFAULT_CAPACITY) {
            
            Iterator last = start;
            while (last != end) {
                last = std::find_if(std::next(start), end, [last](auto i) { return i != *last; });

                if (size_ >= capacity_) {
                    signs_ = utilities::resize<SignalStatus>(signs_, capacity_, capacity_*2);
                    capacity_ *= 2;
                }

                if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::value_type, char>) 
                    signs_[size_] = SignalStatus(*start != '0', std::distance(start, last));
                else 
                    signs_[size_] = SignalStatus(*start != 0, std::distance(start, last));

                size_++;
                start = last;
            }
        }

        /**
         * @brief Копирующий конструктор
         * 
         * @param signs - экземпляр, который будет скопирован
         */
        BinarySignal(const BinarySignal &signs);

        /**
         * @brief Перемещающий конструктор
         * 
         * Думаю, очевидно, что не надо после перемещения пытаться работать со старым экземпляром)
         * 
         * @param signs - экземпляр, который должен быть перенесён
         */
        BinarySignal(BinarySignal &&signs) noexcept;

        /**
         * @brief Деструктор
         * 
         * Освобождает динамически выделенную память
         */
        ~BinarySignal() {delete [] signs_;};

        /**
         * @brief Существует ли сигнал
         * 
         * Может быть заменена функцией getSize().
         * 
         * @return true - size_ > 0 => массив сигналов не пустой
         * @return false - size_ == 0 => массив сигналов пустой
         */
        operator bool() const;

        /**
         * @brief Получить количество единичных бинарных сигналов
         * 
         * @return size_t - количество сигналов
         */
        size_t getSize();

        /**
         * @brief Получить выделенную память под массив сигналов
         * 
         * Зачем вам это может понадобиться?
         * 
         * @return size_t - количество элементов под которые выделена память
         */
        size_t getCapacity();

        /**
         * @brief Инверсия всех сигналов
         * 
         * @return BinarySignal& - тот же самый экземпляр
         */
        BinarySignal &invert();

        /**
         * @brief Вставка другой последовательности сигналов
         * 
         * Для последовательности: "##\___/#".insert("/#\", 3) = "##\_/#\__/#"
         * 
         * @param signs - вставляемая последовательность
         * @param time - время в которое нужно будет вставить signs
         * @return BinarySignal& - тот же самый экземпляр
         */
        BinarySignal &insert(const BinarySignal &signs, unsigned long int time);
        
        /**
         * @brief Удаление фрагмента в заданное время
         * 
         * Для последовательности: "##\____/#".deleteFragment(1, 2) = "#\___/#"
         * 
         * @param time - время, в которое нужно удалить сигнал
         * @param duration - продолжительность удаляемого фрагмента
         * @return BinarySignal& - тот же самый экземпляр
         */
        BinarySignal &deleteFragment(unsigned long int time, unsigned long int duration);

        /**
         * @brief Оператор копирования
         * 
         * @param signs - копируемый экземпляр
         * @return BinarySignal& - скопированный экземпляр
         */
        BinarySignal &operator= (const BinarySignal &signs);

        /**
         * @brief Оператор перемещения
         * 
         * Думаю, очевидно, что не надо после перемещения пытаться работать со старым экземпляром)
         * 
         * @param signs - перемещаемый экземпляр
         * @return BinarySignal& - перемещённый экземпляр
         */
        BinarySignal &operator= (BinarySignal &&signs) noexcept;

        /**
         * @brief Добавляет сигналы в конец массива
         * 
         * Добавляет последовательность сигналов аргумента в конец последовательности экземпляра
         * 
         * @param signs - последовательность вставляемых сигналов
         * @return BinarySignal& - тот же самый экземпляр
         */
        BinarySignal &operator+= (const BinarySignal &signs);

        /**
         * @brief Создаёт экземпляр с дублируемой последовательностью сигналов
         * 
         * @param a - количество дубликатов
         * @return BinarySignal - Полученный экземпляр
         */
        BinarySignal operator* (unsigned int a) const;

        /**
         * @brief Получение статуса последовательности сигналов в определённый момент времени
         * 
         * @param time - время, в которое нужно получить статус
         * @return true - если status_ == true
         * @return false - если status_ == false
         */
        bool operator[] (unsigned long int time) const;

        /**
         * @brief Swap функция, тут и так понятно
         * 
         * @param first - первый аргумент
         * @param second - второй аргумент
         */
        friend void swap(BinarySignal &first, BinarySignal &second);

        /**
         * @brief Оператор ввода последовательности сигналов
         * 
         * Сначала вводится количество сигналов, после каждый сигнал отдельно: fin >> count...
         * Можно прервать ввод с помощью EOF.
         * Все корректно считанные сигналы до EOF будут записаны.
         * 
         * @param fin - поток ввода
         * @param sign - экземпляр последовательности сигналов
         * @return std::istream& - поток ввода
         */
        friend std::istream &operator>> (std::istream &fin, BinarySignal &sign);

        /**
         * @brief Оператор вывода последовательности сигналов
         * 
         * Формат вывода: fout << "Количество сигналов: " << size_ << " | Выделенная память: " << capacity_ << std::endl...
         * 
         * @param fout - поток вывода
         * @param sign - выводимый экземпляр
         * @return std::ostream& - поток вывода
         */
        friend std::ostream &operator<< (std::ostream &fout, const BinarySignal &sign);

        /**
         * @brief Форматирует последовательность сигналов в строку
         * 
         * Количество символов соответствует сумме всех длительностей сигналов.
         * '#' - для status_ == true.
         * '_' - для status_ == false.
         * '\' и '/' - переходные процессы и времени не занимают.
         * 
         * @param sign - экземпляр последовательности бинарных сигналов
         * @return std::string - полученная строка
         */
        friend std::string format(const BinarySignal &sign);
};

}

#endif
