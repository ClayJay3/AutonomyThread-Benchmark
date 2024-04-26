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

    // TEST 1: Single prime calculator running in different thread.
    PrimeCalculatorThreadSingleThreaded PrimeCalculator = PrimeCalculatorThreadSingleThreaded();

    PrimeCalculator.SetPrimeCount(100);
    PrimeCalculator.Start();
    PrimeCalculator.Join();

    /////////////////////////////////////////
    // Cleanup.
    /////////////////////////////////////////

    // Successful exit.
    return 0;
}