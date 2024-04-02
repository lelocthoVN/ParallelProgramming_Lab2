#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <random>
#include <chrono>
#include <iomanip>
#include <omp.h>

class Matrix
{
private:
    std::vector<std::vector<int>> _data;
    size_t _size;

public:
    Matrix() : _size(0) {}

    Matrix(size_t size, int value = 0) : _size(size)
    {
        _data.resize(size, std::vector<int>(size, value));
    }

    ~Matrix() {}

    Matrix(const Matrix& src) : _size(src._size), _data(src._data) {}

    Matrix& operator=(const Matrix& src)
    {
        if (this != &src)
        {
            _size = src._size;
            _data = src._data;
        }
        return *this;
    }

    void clear_matrix()
    {
        _data.clear();
        _size = 0;
    }

    void random_fill_element()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(-100, 100);

#pragma omp parallel for collapse(2)
        for (size_t i = 0; i < _size; ++i)
        {
            for (size_t j = 0; j < _size; ++j)
            {
                _data[i][j] = dis(gen);
            }
        }
    }

    void print() const
    {
        for (size_t i = 0; i < _size; ++i)
        {
            for (size_t j = 0; j < _size; ++j)
            {
                std::cout << _data[i][j] << '\t';
            }
            std::cout << std::endl;
        }
    }

    Matrix dot(const Matrix& rhs)
    {
        if (_size != rhs._size)
        {
            throw std::invalid_argument("Несоответствие размеров матрицы.");
        }

        Matrix result(_size);

#pragma omp parallel for collapse(2)
        for (size_t i = 0; i < _size; ++i)
        {
            for (size_t j = 0; j < _size; ++j)
            {
                int sum = 0;
                for (size_t k = 0; k < _size; ++k)
                {
                    sum += _data[i][k] * rhs._data[k][j];
                }
                result._data[i][j] = sum;
            }
        }
        *this = result;
        return *this;
    }

    void save_file(const std::string& filename) const
    {
        std::ofstream out(filename);
        if (!out.is_open())
        {
            throw std::runtime_error("Невозможно открыть файл для записи.");
        }
        out << _size << std::endl;
        for (size_t i = 0; i < _size; ++i)
        {
            for (size_t j = 0; j < _size; ++j)
            {
                out << _data[i][j] << ' ';
            }
            out << std::endl;
        }

        out.close();
    }

    void load_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Невозможно открыть файл для чтения: " + filename);
        }
        if (!(file >> _size))
        {
            throw std::runtime_error("Ошибка чтения размера матрицы из файла: " + filename);
        }
        _data.clear();
        _data.resize(_size, std::vector<int>(_size, 0));
        for (size_t i = 0; i < _size; ++i)
        {
            for (size_t j = 0; j < _size; ++j)
            {
                if (!(file >> _data[i][j]))
                {
                    throw std::runtime_error("Ошибка чтения элементов матрицы из файла: " + filename);
                }
            }
        }
        file.close();
    }
};

int main()
{
    srand((int)time(0));
    const std::string folder_first = "matrix/First_Matrix/";
    const std::string folder_second = "matrix/Second_Matrix/";
    const std::string folder_result = "matrix/Result_Matrix/";
    const std::string folder_statistics = "matrix/Working_Time/";
    std::chrono::steady_clock::time_point begin, end;
    std::chrono::duration<double> duration;
    std::string user_input;
    while (true)
    {
        std::cout << "Входная команда -> ";
        std::cin >> user_input;
        if (user_input == "-h")
        {
            std::cout << std::endl;
            std::cout << "Доступные команды:" << std::endl;
            std::cout << "[-e] - выйти из программы." << std::endl;
            std::cout << "[-g] - генерировать матрицы." << std::endl;
            std::cout << "[-c] - вычислить матричные произведения." << std::endl;
            std::cout << "[-h] - помогите с командами." << std::endl;
            std::cout << std::endl;
        }
        else if (user_input == "-e")
            break;
        else if (user_input == "-g")
        {
            try
            {
                std::cout << "Первая матрица:" << std::endl;
                for (size_t size = 10; size <= 100; size += 10)
                {
                    Matrix matrix1(size);
                    matrix1.random_fill_element();
                    std::stringstream f1;
                    f1 << folder_first << "matrix1_" << size << ".txt";
                    matrix1.save_file(f1.str());
                    std::cout << "Матрица " + std::to_string(size) + "x" + std::to_string(size) + " - создана." << std::endl;
                }
                std::cout << "Вторая матрица:" << std::endl;
                for (size_t size = 10; size <= 100; size += 10)
                {
                    Matrix matrix2(size);
                    matrix2.random_fill_element();
                    std::stringstream f2;
                    f2 << folder_second << "matrix2_" << size << ".txt";
                    matrix2.save_file(f2.str());
                    std::cout << "Матрица " + std::to_string(size) + "x" + std::to_string(size) + " - создана." << std::endl;
                }
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Ошибка: " << ex.what() << std::endl;
            }
        }

        else if (user_input == "-c")
        {
            try
            {

                for (size_t size = 10; size <= 100; size += 10)
                {
                    double time = 0;
                    Matrix matrix1(size), matrix2(size);
                    matrix1.load_file(folder_first + "matrix1_" + std::to_string(size) + ".txt");
                    matrix2.load_file(folder_second + "matrix2_" + std::to_string(size) + ".txt");
                    begin = std::chrono::high_resolution_clock::now();
                    matrix1.dot(matrix2);
                    end = std::chrono::high_resolution_clock::now();
                    duration = end - begin;
                    std::vector<double> stats;
                    stats.push_back(duration.count());
                    matrix1.save_file(folder_result + std::to_string(size) + ".txt");

                    std::ofstream out;
                    out.open(folder_statistics + "Time.txt", std::ios_base::app);
                    if (out.is_open())
                    {
                        for (size_t i = 0; i < stats.size(); i++)
                        {
                            std::stringstream time_stream;
                            time_stream << std::fixed << std::setprecision(6) << stats[i];
                            std::string time_str = time_stream.str();
                            out << "Матрица " + std::to_string(size) + "x" + std::to_string(size) + " - рассчитывается за " << time_str << "s" << '\n';
                        }
                    }
                    out.close();
                    for (size_t i = 0; i < stats.size(); i++)
                    {
                        std::cout << "Матрица " + std::to_string(size) + "x" + std::to_string(size) + " - рассчитывается за " << std::fixed << std::setprecision(6) << stats[i] << "s" << '\n';
                    }
                }
            }
            catch (const std::exception& ex)
            {
                std::cerr << "Ошибка: " << ex.what() << std::endl;
            }
        }
        else
        {
            std::cout << "Нераспознанная команда " + user_input + ". Введите команду --h." << std::endl;
        }
    }
    return 0;
}
