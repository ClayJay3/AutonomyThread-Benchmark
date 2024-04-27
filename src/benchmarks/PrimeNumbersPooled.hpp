/******************************************************************************
 * @brief Example file that creates multiple threads that calculate a random amount
 *      of prime numbers.
 *
 * @file PrimeNumbers.hpp
 * @author ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-07-22
 *
 * @copyright Copyright Mars Rover Design Team 2023 - All Rights Reserved
 ******************************************************************************/

#include "../interfaces/AutonomyThread.hpp"

/// \cond
#include <iostream>
#include <vector>
#include <shared_mutex>

/// \endcond

/******************************************************************************
 * @brief This class creates a thread for calculating N prime numbers.
 *     This is an example class demonstrating the use of the threading interface.
 *
 *
 * @author ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-07-28
 ******************************************************************************/
class PrimeCalculatorThreadPooled : public AutonomyThread<void>
{
private:
    // Declare and define private methods and variables.
    std::vector<int> m_vThreadPrimes;
    int m_nCount = 10;
    int m_nCurrentCount = 2;
    double m_dCalculationTime = -1.0;
    std::chrono::system_clock::time_point m_tmStartTime = std::chrono::system_clock::time_point::min();
    std::shared_mutex m_muVectorWriteMutex;
    std::shared_mutex m_muCurrentCountWriteMutex;

    /******************************************************************************
     * @brief Check if a number if prime.
     *
     * @param num - The number to check.
     * @return true - The number is prime.
     * @return false - The number is not prime.
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-22
     ******************************************************************************/
    bool IsPrime(int &nNum)
    {
        if (nNum <= 1)
        {
            return false;
        }
        for (int i = 2; i * i <= nNum; ++i)
        {
            if (nNum % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    /******************************************************************************
     * @brief This code will run in a separate thread. Main code goes here.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-22
     ******************************************************************************/
    void ThreadedContinuousCode() override
    {
        // Start a thread pool with 100 threads. This will run the code in the PooledLinearCode() method.
        this->RunDetachedPool(m_nCount, 100);
        // Wait for Pool to finish.
        this->JoinPool();
        // Store end time.
        std::chrono::system_clock::time_point tmEndTime = std::chrono::system_clock::now();
        // Calculate elapsed time.
        m_dCalculationTime = std::chrono::duration_cast<std::chrono::microseconds>(tmEndTime - m_tmStartTime).count();
        // Request stop of main thread.
        this->RequestStop();
    }

    /******************************************************************************
     * @brief Any highly parallelizable code that can be used in the main thread
     *       goes here.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-25
     ******************************************************************************/
    void PooledLinearCode() override
    {
        // Create instance variables.
        bool bFoundPrime = false;
        // Measure the amount of time it takes to run this code.
        if (m_tmStartTime == std::chrono::system_clock::time_point::min())
            m_tmStartTime = std::chrono::system_clock::now();

        // Continue working until this thread finds 1 prime.
        while (!bFoundPrime)
        {
            // Acquire write lock to current count.
            // std::unique_lock<std::shared_mutex> lkWriteLockCount(m_muCurrentCountWriteMutex);
            // Get current count number.
            int nCurrentPrimeTestNumber = m_nCurrentCount;
            // Increment count, this pool thead has the current number.
            ++m_nCurrentCount;
            // Release lock.
            // lkWriteLockCount.unlock();

            // Check if number is prime.
            if (this->IsPrime(nCurrentPrimeTestNumber))
            {
                // Acquire write lock for prime vector.
                // std::unique_lock<std::shared_mutex> lkWriteLockVector(m_muVectorWriteMutex);
                // Append new prime number to vector.
                m_vThreadPrimes.emplace_back(nCurrentPrimeTestNumber);
                // Set toggle to stop looping.
                bFoundPrime = true;
            }
        }
    }

public:
    // Declare and define public methods and variables.
    PrimeCalculatorThreadPooled() = default;

    /******************************************************************************
     * @brief Mutator for the Prime Count private member
     *
     * @param nNum - The amount of primes to calculate.
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-22
     ******************************************************************************/
    void SetPrimeCount(int nNum) { m_nCount = nNum; }

    /******************************************************************************
     * @brief Accessor for the Prime Counter private member.
     *
     * @return int - The amount of primes the calculator will produce.
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-22
     ******************************************************************************/
    int GetDesiredPrimeAmount() { return m_nCount; }

    /******************************************************************************
     * @brief Accessor for the Primes private member.
     *
     * @return std::vector<int> - A vector of the resultant primes.
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-22
     ******************************************************************************/
    std::vector<int> GetPrimes() { return m_vThreadPrimes; }

    /******************************************************************************
     * @brief Accessor for the Calculation Time private member.
     *
     * @return double - The total time in microseconds that is took to fully calculate the
     *          set number of primes.
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2024-04-26
     ******************************************************************************/
    double GetCalculationTime() { return m_dCalculationTime; }

    /******************************************************************************
     * @brief Clears the prime results vector.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-25
     ******************************************************************************/
    void ClearPrimes()
    {
        // Clear number vector.
        m_vThreadPrimes.clear();
        // Reset other vars.
        m_nCurrentCount = 2;
        m_dCalculationTime = -1.0;
        // Reset start time.
        m_tmStartTime = std::chrono::system_clock::time_point::min();
    }
};