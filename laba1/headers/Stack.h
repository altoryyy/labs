#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>
#include <memory>
#include "StackIterator.h" // Подключаем заголовок с определением итератора

template<typename T>
class Stack {
public:
    Stack() = default;

    void push(T value) {
        data.push_back(std::move(value));
    }

    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("Стек пуст, невозможно удалить элемент.");
        }
        data.pop_back();
    }

    T& top() {
        if (isEmpty()) {
            throw std::underflow_error("Стек пуст, невозможно получить верхний элемент.");
        }
        return data.back();
    }

    const T& top() const {
        if (isEmpty()) {
            throw std::underflow_error("Стек пуст, невозможно получить верхний элемент.");
        }
        return data.back();
    }

    bool isEmpty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }

    void clear() {
        data.clear();
    }

    using Iterator = StackIterator<T>;

    Iterator begin() {
        return Iterator(data.rbegin());
    }

    Iterator end() {
        return Iterator(data.rend());
    }

private:
    std::vector<T> data;
};

#endif
