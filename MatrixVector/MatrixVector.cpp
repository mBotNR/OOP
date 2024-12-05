#include <iostream>
#include <vector>

using namespace std;

class Vector {
private:
    int* data;
    size_t size;

public:
    Vector(size_t size = 0, int value = 0) : size(size), data(new int[size]) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    }

    Vector(const Vector& other) : size(other.size), data(new int[other.size]) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    Vector(Vector&& other) noexcept : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = other.data;
            other.size = 0;
            other.data = nullptr;
        }
        return *this;
    }

    ~Vector() {
        delete[] data;
    }

    int sum() const {
        int total = 0;
        for (size_t i = 0; i < size; ++i) {
            total += data[i];
        }
        return total;
    }

    void resize(size_t new_size, int value = 0) {
        int* new_data = new int[new_size];
        for (size_t i = 0; i < new_size; ++i) {
            new_data[i] = value;
        }
        size_t min_size = (new_size < size) ? new_size : size;
        for (size_t i = 0; i < min_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        size = new_size;
    }

    int find(int value) const {
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    int& operator[](size_t index) {
        return data[index];
    }

    const int& operator[](size_t index) const {
        return data[index];
    }

    size_t get_size() const {
        return size;
    }
};

class Matrix {
private:
    vector<Vector> rows;
    size_t rows_count;
    size_t cols_count;

public:
    Matrix(size_t rows_count, size_t cols_count) : rows_count(rows_count), cols_count(cols_count) {
        rows.reserve(rows_count);
        for (size_t i = 0; i < rows_count; ++i) {
            rows.push_back(Vector(cols_count, 0));
        }
    }

    Matrix(const Matrix& other) : rows_count(other.rows_count), cols_count(other.cols_count) {
        for (size_t i = 0; i < rows_count; ++i) {
            rows.push_back(other.rows[i]);
        }
    }

    Matrix(Matrix&& other) noexcept : rows_count(other.rows_count), cols_count(other.cols_count), rows(move(other.rows)) {
        other.rows_count = 0;
        other.cols_count = 0;
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            rows_count = other.rows_count;
            cols_count = other.cols_count;
            rows = other.rows;
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            rows_count = other.rows_count;
            cols_count = other.cols_count;
            rows = move(other.rows);
            other.rows_count = 0;
            other.cols_count = 0;
        }
        return *this;
    }

    void transpose() {
        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = i + 1; j < cols_count; ++j) {
                swap(rows[i][j], rows[j][i]);
            }
        }
        swap(rows_count, cols_count);
    }

    void resize(size_t new_rows, size_t new_cols) {
        rows.resize(new_rows, Vector(new_cols, 0));
        rows_count = new_rows;
        cols_count = new_cols;
    }

    Matrix slice(size_t row_start, size_t row_end, size_t col_start, size_t col_end) const {
        Matrix result(row_end - row_start + 1, col_end - col_start + 1);
        for (size_t i = row_start; i <= row_end; ++i) {
            for (size_t j = col_start; j <= col_end; ++j) {
                result.rows[i - row_start][j - col_start] = rows[i][j];
            }
        }
        return result;
    }

    pair<int, int> find(int value) const {
        for (size_t i = 0; i < rows_count; ++i) {
            int col = rows[i].find(value);
            if (col != -1) {
                return { static_cast<int>(i), col };
            }
        }
        return { -1, -1 };
    }

    void print() const {
        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = 0; j < cols_count; ++j) {
                cout << rows[i][j] << " ";
            }
            cout << endl;
        }
    }

    void input() {
        cout << "Введите элементы матрицы размером " << rows_count << "x" << cols_count << " построчно, разделяя их пробелами:" << endl;
        for (size_t i = 0; i < rows_count; ++i) {
            for (size_t j = 0; j < cols_count; ++j) {
                cin >> rows[i][j];
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    size_t rows, cols;

    cout << "Введите количество строк и столбцов матрицы: ";
    cin >> rows >> cols;

    Matrix mat(rows, cols);

    // Показываем пользователю, как правильно вводить данные
    mat.input();

    cout << "Матрица:" << endl;
    mat.print();

    mat.transpose();
    cout << "Транспонированная матрица:" << endl;
    mat.print();

    int value;
    cout << "Введите значение для поиска: ";
    cin >> value;
    pair<int, int> pos = mat.find(value);
    if (pos.first != -1) {
        cout << "Элемент найден на позиции: (" << pos.first << ", " << pos.second << ")" << endl;
    }
    else {
        cout << "Элемент не найден." << endl;
    }

    return 0;
}
