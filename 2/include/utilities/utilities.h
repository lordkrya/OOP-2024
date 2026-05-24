#ifndef UTILITIES_DEFENDER
#define UTILITIES_DEFENDER

#include <cstring>
#include <iostream>
#include <limits>


/// @brief Библиотека с набором полезных функций, используемых в проекте
namespace utilities {

/**
 * @brief Считывает определенный тип данных из потока
 * 
 * В случае fin.full() он ожидает повторного ввода.
 * 
 * @tparam T - тип считываемых данных
 * @param fin - входной поток
 * @param err - текст, который будет выведен в std::out при выполнении: fin.fail().
 * @return T - считанное значение
 * 
 * @throw std::runtime_error - ошибка чтения
 * @throw std::range_error - встрчено EOF
 */
template<class T>
T getSth(std::istream &fin, std::string err = "") {
    T a;
    fin >> a;
    while(!fin.eof()) {
        if(fin.bad()) throw std::runtime_error(std::string("Failed to read: ") + strerror(errno));
        
        else if(fin.fail()) {
            fin.clear();
            fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!err.empty()) std::cout << err << std::endl;
        }

        else return a;
        fin >> a;
    }
    throw std::range_error("EOF received");
}

/**
 * @brief Самописная функция realloc()
 * 
 * Исходный массив будет удален с помощью команды delete[].
 * Убедитесь, что вы выделили память с помощью оператора NEW[].
 * 
 * @tparam T - тип данных массива
 * @param arr - указатель на расширяемый массив
 * @param oldSize - длина массива изначально
 * @param newSize - новая длина массива
 * @return T* - указатель на измененный массив
 * 
 * @throw std::logic_error - был получен нулевой указатель
 */
template <typename T>
T *resize(T *arr, size_t oldSize, size_t newSize) {
    if (arr == nullptr) throw std::logic_error("A null pointer was received");

    T *newArr = new T[newSize] {};
    std::move(arr, arr + std::min(oldSize, newSize), newArr);

    delete[] arr;
    return newArr;
}

/**
 * @brief Самописная функция strdup()
 * 
 * @param str - указатель на исходную строку
 * @return char* - скопированная строка в новую область памяти
 * 
 * @throw std::logic_error - был получен нулевой указатель
 */
char *newStrdup(const char *str);

}

#endif
