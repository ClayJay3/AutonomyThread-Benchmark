#include "./benchmarks/PrimeNumbersSingleThread.hpp"
#include "./benchmarks/PrimeNumbersPooled.hpp"

#include <signal.h>
#include <thread>

// Create a boolean used to handle a SIGINT and exit gracefully.
volatile sig_atomic_t bMainStop = false;

/******************************************************************************
 * @brief Help function given to the C++ csignal standard library to run when
 *      a CONTROL^C is given from the terminal.
 *
 * @param nSignal - Integer representing the interrupt value.
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2024-01-08
 ******************************************************************************/
void SignalHandler(int nSignal)
{
    // Check signal type.
    if (nSignal == SIGINT || nSignal == SIGTERM)
    {
        // Print info.
        std::printf("Handling program abort...\n");
        // Update stop signal.
        bMainStop = true;
    }
    // The SIGQUIT signal can be sent to the terminal by pressing CNTL+\.
    else if (nSignal == SIGQUIT)
    {
        // Print info.
        std::printf("Handling program abort...\n");
        // Update stop signal.
        bMainStop = true;
    }
}

/******************************************************************************
 * @brief Autonomy main function.
 *
 * @return int - Exit status number.
 *
 * @author ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-06-20
 ******************************************************************************/
int main()
{
    // Setup signal interrupt handler.
    struct sigaction stSigBreak;
    stSigBreak.sa_handler = SignalHandler;
    stSigBreak.sa_flags = 0;
    sigemptyset(&stSigBreak.sa_mask);
    sigaction(SIGINT, &stSigBreak, nullptr);
    sigaction(SIGQUIT, &stSigBreak, nullptr);

    // Create instance objects.
    IPS IterPerSecond = IPS();

    // /////////////////////////////////////////
    // // TEST 1: Single prime calculator running in different thread.
    // /////////////////////////////////////////
    // PrimeCalculatorThreadSingleThreaded PrimeCalculatorTEST1 = PrimeCalculatorThreadSingleThreaded();
    // PrimeCalculatorTEST1.SetPrimeCount(9999);

    // // For calculating average time.
    // double dTotalMicroseconds100Runs = 0.0;
    // // Run test 100 times.
    // for (int nIter = 0; nIter < 1; ++nIter)
    // {
    //     // Run thread.
    //     PrimeCalculatorTEST1.Start();
    //     PrimeCalculatorTEST1.Join();
    //     // Add time taken to total.
    //     dTotalMicroseconds100Runs += PrimeCalculatorTEST1.GetCalculationTime();
    //     // Reset member variables for prime calculator.
    //     PrimeCalculatorTEST1.ClearPrimes();
    // }
    // // Run thread.
    // PrimeCalculatorTEST1.Start();
    // PrimeCalculatorTEST1.Join();
    // // Print TEST1 info.
    // std::cout << "AVERAGE Single Thread Primes Calculation Time: " << dTotalMicroseconds100Runs / 1e6 << " s" << std::endl;
    // // // Print out primes list.
    // // for (int nPrime : PrimeCalculatorTEST1.GetPrimes())
    // // {
    // //     std::cout << nPrime << " ";
    // // }
    // // std::cout << std::endl;

    /////////////////////////////////////////
    // TEST 2: Thread pool individually calculating primes.
    /////////////////////////////////////////
    PrimeCalculatorThreadPooled PrimeCalculatorTEST2 = PrimeCalculatorThreadPooled();
    PrimeCalculatorTEST2.SetPrimeCount(999999);
    // For calculating average time.
    double dTotalMicroseconds100Runs = 0.0;
    // Run test 100 times.
    for (int nIter = 0; nIter < 1; ++nIter)
    {
        // Reset member variables for prime calculator.
        PrimeCalculatorTEST2.ClearPrimes();
        // Run thread.
        PrimeCalculatorTEST2.Start();
        // Wait for calculator to finish.
        PrimeCalculatorTEST2.Join();
        // Add time taken to total.
        dTotalMicroseconds100Runs += PrimeCalculatorTEST2.GetCalculationTime();
    }
    // Print TEST1 info.
    std::cout << "\nAVERAGE Pooled Thread Primes Calculation Time: " << dTotalMicroseconds100Runs / 1e6 << " s" << std::endl;
    // // Print out primes list.
    // for (int nPrime : PrimeCalculatorTEST2.GetPrimes())
    // {
    //     std::cout << nPrime << " ";
    // }
    // std::cout << std::endl;

    /////////////////////////////////////////
    // Cleanup.
    /////////////////////////////////////////

    // Successful exit.
    return 0;
}