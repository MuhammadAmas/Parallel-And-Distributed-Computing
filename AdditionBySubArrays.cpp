#include <iostream>
#include <pthread.h>

#define ARRAY_SIZE 644567
#define THREAD_CAPACITY 20
#define NUM_THREADS (ARRAY_SIZE / THREAD_CAPACITY + (ARRAY_SIZE % THREAD_CAPACITY > 0 ? 1 : 0))

using namespace std;

// Thread function argument structure
struct ThreadArg
{
    int *start;
    int length;
    ThreadArg(int *s, int l) : start(s), length(l) {}
};

// Thread function to calculate the sum of a sub-array
void *threadFunc(void *arg)
{
    ThreadArg *arguments = (ThreadArg *)arg;
    int *sum = new int(0); // Dynamically allocated sum to return from thread
    for (int i = 0; i < arguments->length; ++i)
    {
        *sum += arguments->start[i];
    }
    return sum;
}

int main()
{
    int *arr = new int[ARRAY_SIZE]; // Dynamic array
    // Fill the array with 1s
    for (int i = 0; i < ARRAY_SIZE; ++i)
    {
        arr[i] = 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadArg *threadArgs[NUM_THREADS];
    int totalSum = 0;

    // Creating threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        int startIdx = i * THREAD_CAPACITY;
        int length = min(THREAD_CAPACITY, ARRAY_SIZE - startIdx);
        threadArgs[i] = new ThreadArg(&arr[startIdx], length);
        if (pthread_create(&threads[i], nullptr, threadFunc, (void *)threadArgs[i]) != 0)
        {
            cerr << "Failed to create thread " << i << endl;
            delete[] arr; // Clean up the dynamic array on failure
            for (int j = 0; j < i; ++j)
            {
                delete threadArgs[j]; // Clean up any already created ThreadArg objects
            }
            return 1;
        }
    }

    // Joining threads and accumulating the sum
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        int *threadSum;
        pthread_join(threads[i], (void **)&threadSum);
        totalSum += *threadSum;
        delete threadSum;
        delete threadArgs[i];
    }

    cout << "Total sum of the array is: " << totalSum << endl;

    delete[] arr; // Clean up the dynamic array
    return 0;
}
