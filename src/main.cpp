#include <atomic>

// Create a boolean used to handle a SIGINT and exit gracefully.
volatile sig_atomic_t bMainStop = false;

/******************************************************************************
 * @brief Help function given to the C++ csignal standard library to run when
 *      a CONTROL^C is given from the terminal.
 *
 * @param nSignal - Integer representing the interrupt value.
 *
 * @author clayjay3 (claytonraycowen@gmail.com)
 * @date 2024-04-19
 ******************************************************************************/
void SignalHandler(int nSignal)
{
    // Check signal type.
    if (nSignal == SIGINT || nSignal == SIGTERM)
    {
        // Submit logger message.
        LOG_INFO(logging::g_qSharedLogger, "Ctrl+C or SIGTERM received. Cleaning up...");

        // Update stop signal.
        bMainStop = true;
    }
    // The SIGQUIT signal can be sent to the terminal by pressing CNTL+\.
    else if (nSignal == SIGQUIT)
    {
        // Submit logger message.
        LOG_INFO(logging::g_qSharedLogger, "Quit signal key pressed. Cleaning up...");

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
 * @date 2024-04-19
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

    while (!bMainStop)
    {
        // Update IPS tick.
        IterPerSecond.Tick();

        // No need to loop as fast as possible. Sleep...
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }

    /////////////////////////////////////////
    // Cleanup.
    /////////////////////////////////////////

    // Successful exit.
    return 0;
}