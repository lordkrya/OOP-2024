#ifndef SIGNALSTATUS_DEFENDER
#define SIGNALSTATUS_DEFENDER

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "utilities/utilities.h"

/// @brief Библиотека для работы с бинарными сигналами
namespace signals {

/**
 * @brief Единичный бинарный сигнал
 * 
 * Корректное состояние объекта: time_ > 0
 */
class SignalStatus {
    private:
        bool status_;               ///< Статус сигнала
        unsigned long int time_;    ///< Продолжительность сигнала
    public:
        /**
         * @brief Конструктор по умолчанию
         * 
         * status_ = false, time_ = 1
         */
        SignalStatus(): status_(false), time_(1) {}
        
        /**
         * @brief Конструктор с заданными полями
         * 
         * @param status = status_
         * @param time = time_
         */
        SignalStatus(bool status, unsigned long int time): status_(status), time_(time ? time : 1) {}

        /**
         * @brief Шаблонный конструтор работающий с итераторами
         * 
         * Если элементы контейнера соответствуют типу char, то status_ = *start != '0',
         * Иначе status_ = *start != 0.
         * Убедитесь, что start <= end, в ином случае поведение не определено.
         * 
         * @tparam Iterator - для итераторов должны быть определены операторы сравнения и разыменования
         * @param start - итератор указывающий на начало контейнера
         * @param end - итератор указывающий на конец контейнера
         */
        template <typename Iterator>
        SignalStatus(Iterator start, Iterator end) {
            if (start == end) {
                status_ = false;
                time_ = 1;
                return;
            }

            auto val = *(start);
            auto pos = std::find_if(std::next(start), end, [val](auto i) { return i != val; });

            time_ = std::distance(start, pos);
            if constexpr (std::is_same_v<typename std::iterator_traits<Iterator>::value_type, char>)
                status_ = *start != '0';
            else
                status_ = *start != 0;
        }
        
        /**
         * @brief Деструктор по умолчанию
         * 
         * По сути, ничего не делает
         */
        ~SignalStatus() = default;

        /**
         * @brief Возвращает состояние сигнала
         * 
         * Является альтернативой getStatus().
         * 
         * @return true - если status_ == true
         * @return false - если status_ == false
         */
        operator bool() const;

        /**
         * @brief Возвращает состояние сигнала
         * 
         * Является альтернативой bool().
         * 
         * @return true - если status_ == true
         * @return false - если status_ == false
         */
        bool getStatus() const;
        
        /**
         * @brief Возвращает продолжительность сигнала 
         * 
         * @return unsigned long int 
         */
        unsigned long int getTime() const;

        /**
         * @brief Устанавливает значение статуса
         * 
         * @param status - новый статус
         */
        void setStatus(bool status);

        /**
         * @brief Устанавливает значение продолжительности
         * 
         * При time = 0 (не ползволяет сломать состояние класса) time_ = 1. 
         * 
         * @param time - новое время
         */
        void setTime(unsigned long int time);

        /**
         * @brief Изменение значения продолжительности
         * 
         * При 1 > time_ + dtime устанавливается значение по умолчанию time_ = 1.
         * Тип принимаемого аргумента int long, поэтому установить time_ = UINT32_MAX за один вызов функции не получится.
         * Отслеживается переполнение, при time_ = UINT32_MAX + 1 устанавливается значение по умолчанию time_ = 1.
         * 
         * @param dtime - дельта времени (на сколько нужно изменить)
         */
        void changeTime(int long dtime);

        /**
         * @brief Инверсия сигнала
         * 
         * Копирует сигнал с обратным значением статуса
         * 
         * @return SignalStatus - полученный сигнал
         */
        SignalStatus operator~ () const;
};

/**
 * @brief Оператор ввода
 * 
 * Читает сначала статус, после продолжительность: fin >> status >> time.
 * 
 * @param fin - поток ввода
 * @param sign - экземпляр единичного бинарного сигнала
 * @return std::istream& - поток ввода
 */
std::istream &operator>> (std::istream &fin, SignalStatus &sign);

/**
 * @brief Оператор вывода
 * 
 * Формат вывода: fout << "Статус сигнала: " << status_ << " | Длительность: " << time_ << std::endl.
 * 
 * @param fin - поток вывода
 * @param sign - экземпляр единичного бинарного сигнала
 * @return std::istream& - поток вывода
 */
std::ostream &operator<< (std::ostream &fout, const SignalStatus &sign);

/**
 * @brief Форматирует сигнал в строку
 * 
 * Количество символов соответствует time_.
 * '#' - для status_ == true.
 * '_' - для status_ == false.
 * 
 * @param sign - экземпляр единичного бинарного сигнала
 * @return std::string - полученная строка
 */
std::string formatStatus(const SignalStatus &sign);

}

#endif
