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

/// \endcond

/******************************************************************************
 * @brief This class creates a thread for calculating N prime numbers.
 *     This is an example class demonstrating the use of the threading interface.
 *
 *
 * @author ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-07-28
 ******************************************************************************/
class PrimeCalculatorThreadSingleThreaded : public AutonomyThread<void>
{
private:
    // Declare and define private methods and variables.
    std::vector<int> m_vThreadPrimes;
    int m_nCount = 10;
    int m_nCurrentCount = 2;

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
     * @brief Calculate 'count' number of primes.
     *
     * @param nCount - The number of primes to calculate.
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-22
     ******************************************************************************/
    void CalculatePrimes(int &nCount)
    {
        // Loop until we have the required amount of primes.
        if (int(m_vThreadPrimes.size()) < nCount)
        {
            // Check if our current number is a prime.
            if (IsPrime(m_nCurrentCount))
            {
                m_vThreadPrimes.push_back(m_nCurrentCount);
            }

            // Increment counter.
            ++m_nCurrentCount;
        }
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
        // Change this to calculate a different number of prime numbers.
        CalculatePrimes(m_nCount);

        // Check if we have reached the desired number of primes.
        if (int(this->GetPrimes().size()) >= this->GetDesiredPrimeAmount())
        {
            // Call thread stop.
            this->RequestStop();
        }
    }

    /******************************************************************************
     * @brief Any highly parallelizable code that can be used in the main thread
     *       goes here.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-25
     ******************************************************************************/
    void PooledLinearCode() override {}

public:
    // Declare and define public methods and variables.
    PrimeCalculatorThreadSingleThreaded() = default;

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
     * @brief Clears the prime results vector.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-25
     ******************************************************************************/
    void ClearPrimes() { m_vThreadPrimes.clear(); }
};