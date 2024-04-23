#include "../interfaces/AutonomyThread.hpp"

#include <opencv/opencv>

/******************************************************************************
 * @brief This class inherits the AutonomyThread interface and implements the threaded
 * container methods. It also utilizes the ability to create a thread pool
 * of subroutines and the ability to parallelize loops.
 *
 *
 * @author ClayJay3 (claytonraycowen@gmail.com)
 * @date 2023-07-27
 ******************************************************************************/
class ArucoGenerateTagsThreaded : public AutonomyThread<void>
{
private:
    // Declare and define private member variables.
    int m_nNumTagsToGenerate = 0;
    std::vector<cv::aruco::PredefinedDictionaryType> m_vDictionaries;
    std::mutex m_muDictMutex; // This mutex is used for locking write access to the dictionary in threads.

    /******************************************************************************
     * @brief This code will run in a separate thread. This is the main code.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-23
     ******************************************************************************/
    void ThreadedContinuousCode() override
    {
        // Start thread pool. Run detached since the threads aren't returning anything.
        // This is much faster than the normal RunPool function.
        this->RunDetachedPool(m_vDictionaries.size(), m_vDictionaries.size());

        // Wait for pool tasks to finish.
        this->JoinPool();

        // Stop threaded code.
        this->RequestStop();
    }

    /******************************************************************************
     * @brief Any highly parallelizable code that can be used in the main thread
     *       goes here.
     *
     *      This is not required and is only used at the users convenience.
     *
     *
     * @author ClayJay3 (claytonraycowen@gmail.com)
     * @date 2023-07-25
     ******************************************************************************/
    void PooledLinearCode() override
    {
        // Acquire resource lock for dictionary vector.
        std::unique_lock<std::mutex> lock(m_muDictMutex);
        // Get dictionary enum from back of dictionary vector.
        cv::aruco::PredefinedDictionaryType cvDictType = m_vDictionaries.back();
        m_vDictionaries.pop_back();
        // Release resource lock.
        lock.unlock();

        //////////////////////////////////////////////////
        // Change this bool to test the two different
        // blocks of code!
        //////////////////////////////////////////////////
        bool bUseParallelLoop = false;
        if (bUseParallelLoop)
        {
            //////////////////////////////////////////////////
            // This code splits the loop into parts and
            // completes it in different threads.
            //////////////////////////////////////////////////
            this->ParallelizeLoop(50,
                                  m_nNumTagsToGenerate,
                                  [&cvDictType](const int a, const int b)
                                  {
                                      // Loop through and generate each of the tags.
                                      for (int i = a; i < b; ++i)
                                          GenerateOpenCVArucoMarker(cvDictType, i);
                                  });
        }
        else
        {
            //////////////////////////////////////////////////
            // This code linearly runs through every tag.
            //////////////////////////////////////////////////
            for (int i = 0; i < m_nNumTagsToGenerate; ++i)
            {
                GenerateOpenCVArucoMarker(cvDictType, i);
            }
        }
    }
}