#ifndef STACKITERATOR_H
#define STACKITERATOR_H

#include <vector>

template<typename T>
class StackIterator {
public:
    explicit StackIterator(typename std::vector<T>::reverse_iterator it) : current(it) {}

    T& operator*() {
        return *current;
    }

    StackIterator& operator++() {
        ++current;
        return *this;
    }

    bool operator!=(const StackIterator& other) const {
        return current != other.current;
    }

private:
    typename std::vector<T>::reverse_iterator current;
};

#endif
