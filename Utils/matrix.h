#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

template <typename T>
class Matrix
{
private:
    std::vector<T> data;

    size_t strNum;
    size_t colNum;
public:
    Matrix(size_t strNum, size_t colNum, std::function<T(int i, int j)> initilizeFunction = [](int i, int j){ return 0; })
        : strNum(strNum), colNum(colNum)
    {
        for (int i = 0; i < strNum; i++)
        {
            for (int j = 0; j < colNum; j++)
            {
                data.push_back(initilizeFunction(i, j));
            }
        }
    }

    Matrix(size_t strNum, size_t colNum, std::initializer_list<T> il)
        : strNum(strNum), colNum(colNum)
    {
        data.insert(data.end(), il.begin(), il.end());
    }

    void print()
    {
        for (int i = 0; i < strNum; i++)
        {
            for (int j = 0; j < colNum; j++)
            {
                std::cout << data[getIndex(i, j)] << " ";
            }
            std::cout << std::endl;
        }
    }

    T getValue(int y, int x, T defaultValue = 0) const
    {
        T value = defaultValue;

        if (checkIndexes(y, x))
            value = data[getIndex(y, x)];

        return value;
    }

    void setValue(int y, int x, T value)
    {
        if (checkIndexes(y, x))
            data[getIndex(y, x)] = value;
    }

    void sort()
    {
        std::sort(data.begin(), data.end());
    }

    void forEach(std::function<T(int i, int j)> func)
    {
        for (int i = 0; i < strNum; i++)
        {
            for (int j = 0; j < colNum; j++)
            {
                data[getIndex(i, j)] = func(i, j);
            }
        }
    }

private:
    int getIndex(int i, int j) const
    {
        return i * colNum + j;
    }

    bool checkIndexes(int y, int x) const
    {
        if (x >= 0 && y >= 0 && x < colNum && y < strNum)
            return true;

        return false;
    }
};

#endif
