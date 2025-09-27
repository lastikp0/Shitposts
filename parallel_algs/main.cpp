#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <iomanip>

// Matrix class with basic operators
class Matrix
{
private:
    std::vector<std::vector<double>> data;
    size_t rows, cols;

public:
    Matrix(size_t r, size_t c) :
        rows(r),
        cols(c),
        data(r, std::vector<double>(c, 0))
    {}
    
    Matrix(const std::vector<std::vector<double>>& input) :
        data(input),
        rows(input.size()),
        cols(input.empty() ? 0 : input[0].size())
    {}
    
    size_t getRows() const
    {
        return rows;
    }
    size_t getCols() const
    {
        return cols;
    }
    
    double& operator()(size_t i, size_t j)
    {
        return data[i][j];
    }
    const double& operator()(size_t i, size_t j) const
    {
        return data[i][j];
    }
    
    std::vector<double>& operator[](size_t i)
    {
        return data[i];
    }
    const std::vector<double>& operator[](size_t i) const
    {
        return data[i];
    }
    
    bool operator==(const Matrix& other) const
    {
        if (rows != other.rows || cols != other.cols)
        {
            return false;
        }

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                if (std::abs(data[i][j] - other.data[i][j]) > 1e-9)
                {
                    return false;
                }
            }
        }

        return true;
    }
    
    void fillRandom()
    {
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                data[i][j] = static_cast<double>(rand() % 100);
            }
        }
    }
    
    void print() const
    {
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                std::cout << std::setw(8) << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Single-thread realisation of block multiplication
Matrix multiplyBlockSequential(const Matrix& A, const Matrix& B, size_t blockSize)
{
    assert(A.getCols() == B.getRows());
    
    // Matrices sizes
    size_t n = A.getRows();
    size_t m = A.getCols();
    size_t p = B.getCols();
    
    Matrix result(n, p);
    
    // Dividing into blocks
    for (size_t i = 0; i < n; i += blockSize)
    {
        for (size_t j = 0; j < p; j += blockSize)
        {
            for (size_t k = 0; k < m; k += blockSize)
            {
                // Calculating position of block end
                size_t i_end = std::min(i + blockSize, n);
                size_t j_end = std::min(j + blockSize, p);
                size_t k_end = std::min(k + blockSize, m);
                
                // Iterating over block
                for (size_t ii = i; ii < i_end; ii++)
                {
                    for (size_t jj = j; jj < j_end; jj++)
                    {
                        double sum = result(ii, jj);

                        for (size_t kk = k; kk < k_end; kk++)
                        {
                            sum += A(ii, kk) * B(kk, jj);
                        }

                        result(ii, jj) = sum;
                    }
                }
            }
        }
    }
    
    return result;
}

// Function for multi-thread block multiplication
// Takes block start and end (rows), instead of calculating them
void multiplyBlockThread(const Matrix& A, const Matrix& B, Matrix& result, 
                        size_t startRow, size_t endRow, size_t blockSize)
{
    size_t m = A.getCols();
    size_t p = B.getCols();
    
    for (size_t i = startRow; i < endRow; i += blockSize)
    {
        for (size_t j = 0; j < p; j += blockSize)
        {
            for (size_t k = 0; k < m; k += blockSize)
            {
                size_t i_end = std::min(i + blockSize, endRow);
                size_t j_end = std::min(j + blockSize, p);
                size_t k_end = std::min(k + blockSize, m);
                
                for (size_t ii = i; ii < i_end; ii++)
                {
                    for (size_t jj = j; jj < j_end; jj++)
                    {
                        double sum = result(ii, jj);

                        for (size_t kk = k; kk < k_end; kk++)
                        {
                            sum += A(ii, kk) * B(kk, jj);
                        }

                        result(ii, jj) = sum;
                    }
                }
            }
        }
    }
}

// Multi-thread realisation of block multiplication (std::thread)
Matrix multiplyThreads(const Matrix& A, const Matrix& B, size_t blockSize, size_t numThreads)
{
    assert(A.getCols() == B.getRows());
    
    size_t n = A.getRows();
    Matrix result(n, B.getCols());
    
    std::vector<std::thread> threads;
    // Same as ceil function
    size_t rowsPerThread = (n + numThreads - 1) / numThreads;
    
    // Iterating over threads
    for (size_t t = 0; t < numThreads; t++)
    {
        size_t startRow = t * rowsPerThread;
        size_t endRow = std::min((t + 1) * rowsPerThread, n);
        
        // Creating threads that cover all rows
        if (startRow < n) 
        {
            threads.emplace_back(multiplyBlockThread, std::cref(A), std::cref(B), 
                               std::ref(result), startRow, endRow, blockSize);
        }
    }
    
    // Waiting for threads to finish
    for (auto& thread : threads)
    {
        thread.join();
    }
    
    return result;
}

// Multi-thread realisation of block multiplication (std::async and std::future)
Matrix multiplyAsync(const Matrix& A, const Matrix& B, size_t blockSize, size_t numThreads)
{
    assert(A.getCols() == B.getRows());
    
    size_t n = A.getRows();
    Matrix result(n, B.getCols());
    
    std::vector<std::future<void>> futures;
    size_t rowsPerThread = (n + numThreads - 1) / numThreads;
    
    for (size_t t = 0; t < numThreads; t++)
    {
        size_t startRow = t * rowsPerThread;
        size_t endRow = std::min((t + 1) * rowsPerThread, n);
        
        // Creating async tasks that cover all rows
        if (startRow < n)
        {
            futures.push_back(std::async(std::launch::async, 
                [&A, &B, &result, startRow, endRow, blockSize]() {
                    multiplyBlockThread(A, B, result, startRow, endRow, blockSize);
                }));
        }
    }
    
    // Waiting for tasks to finish
    for (auto& future : futures)
    {
        future.get();
    }
    
    return result;
}

// Class for measuring time of work
class Benchmark
{
private:
    std::vector<long long> times;
    
public:
    template<typename Func, typename... Args>
    auto measure(Func&& func, Args&&... args)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        
        long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        times.push_back(duration);
        
        return result;
    }
    
    double getAverageTime() const
    {
        if (times.empty())
        {
            return 0.0;
        }

        return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    }
    
    double getMinTime() const
    {
        if (times.empty())
        {
            return 0.0;
        }

        return *std::min_element(times.begin(), times.end());
    }
    
    double getMaxTime() const
    {
        if (times.empty())
        {
            return 0.0;
        }

        return *std::max_element(times.begin(), times.end());
    }
    
    void clear()
    {
        times.clear();
    }
};

void runTests() {
    std::cout << "=== Validation tests ===" << std::endl;
    
    // Test 1
    Matrix A1(2, 2);
    Matrix B1(2, 2);
    A1.fillRandom();
    B1.fillRandom();
    
    Matrix result1_block = multiplyBlockSequential(A1, B1, 1);
    Matrix result1_thread = multiplyThreads(A1, B1, 1, 2);
    Matrix result1_async = multiplyAsync(A1, B1, 1, 2);
    
    assert(result1_thread == result1_block);
    assert(result1_async == result1_block);
    std::cout << "Test 1 passed" << std::endl;
    
    // Тест 2
    Matrix A2(5, 5);
    Matrix B2(5, 5);
    A1.fillRandom();
    B1.fillRandom();

    Matrix result2_block = multiplyBlockSequential(A2, B2, 2);
    Matrix result2_thread = multiplyThreads(A2, B2, 2, 2);
    Matrix result2_async = multiplyAsync(A2, B2, 2, 2);
    
    assert(result2_thread == result2_block);
    assert(result2_async == result2_block);
    std::cout << "Test 2 passed" << std::endl;
    
    // Тест 3
    Matrix A3(100, 100);
    Matrix B3(100, 100);
    A3.fillRandom();
    B3.fillRandom();
    
    Matrix result3_block = multiplyBlockSequential(A3, B3, 16);
    Matrix result3_thread = multiplyThreads(A3, B3, 16, 4);
    Matrix result3_async = multiplyAsync(A3, B3, 16, 4);
    
    assert(result3_thread == result3_block);
    assert(result3_async == result3_block);
    std::cout << "Test 3 passed" << std::endl;
    
    std::cout << "All tests passed" << std::endl << std::endl;
}

void runPerformanceTests()
{
    std::cout << "=== Performance tests ===" << std::endl;

    Benchmark bench;
    const size_t numRuns = 5;
    
    std::vector<size_t> matrixSizes = {256, 512, 1024};
    std::vector<size_t> threadCounts = {1, 2, 4, 8, 16};
    size_t blockSize = 64;
    
    std::cout << "Threads";
    for (size_t threads : threadCounts)
    {
        std::cout << std::setw(8) << threads;
    }
    std::cout << std::endl;
    
    for (size_t size : matrixSizes)
    {
        std::cout << size << "x" << size;
        
        Matrix A(size, size);
        Matrix B(size, size);
        A.fillRandom();
        B.fillRandom();
        
        for (size_t threads : threadCounts)
        {
            bench.clear();

            for (size_t i = 0; i < numRuns; i++)
            {
                bench.measure(multiplyThreads, A, B, blockSize, threads);
            }

            std::cout << std::setw(8) << std::fixed << std::setprecision(1) 
                     << bench.getAverageTime() / 1000.0;
        }
        
        std::cout << std::endl;
    }
}

void findOptimalThreads()
{
    std::cout << "\n=== Optimal threads number search ===" << std::endl;
    
    std::vector<size_t> sizes = {256, 512, 1024, 2048};
    size_t blockSize = 64;
    const size_t numRuns = 3;
    
    Benchmark bench;
    
    for (size_t size : sizes)
    {
        std::cout << "\nMatrix " << size << "x" << size << ":" << std::endl;
        std::cout << "Threads  Time(ms)  Speedup" << std::endl;
        
        Matrix A(size, size);
        Matrix B(size, size);
        A.fillRandom();
        B.fillRandom();
        
        // Base (single-thread)
        bench.clear();
        for (size_t i = 0; i < numRuns; i++)
        {
            bench.measure(multiplyThreads, A, B, blockSize, 1);
        }
        double baseTime = bench.getAverageTime();
        
        std::cout << std::setw(6) << 1 
                 << std::setw(11) << std::fixed << std::setprecision(1) << baseTime / 1000.0
                 << std::setw(10) << "1.00x" << std::endl;
        
        for (size_t threads : {2, 4, 8, 16, 32, 64})
        {
            bench.clear();
            for (size_t i = 0; i < numRuns; i++)
            {
                bench.measure(multiplyThreads, A, B, blockSize, threads);
            }
            double currentTime = bench.getAverageTime();
            double speedup = baseTime / currentTime;
            
            std::cout << std::setw(6) << threads 
                     << std::setw(11) << std::fixed << std::setprecision(1) << currentTime / 1000.0
                     << std::setw(10) << std::fixed << std::setprecision(2) << speedup << "x" 
                     << std::endl;
        }
    }
}

int main() {
    std::srand((std::time(0)));
    
    try
    {
        runTests();
        
        runPerformanceTests();
        
        findOptimalThreads();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}