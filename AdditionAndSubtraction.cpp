#include <iostream>
#include <pthread.h>

void *addNumbers(void *args)
{
    int *input = (int *)args;
    int *result = new int;
    *result = input[0] + input[1];
    return (void *)result;
}

void *subtractNumbers(void *args)
{
    int *input = (int *)args;
    int *result = new int;
    *result = input[0] - input[1];
    return (void *)result;
}

int main()
{
    pthread_t addThread, subtractThread;
    int data[2] = {10, 5};
    int *addResult, *subtractResult;

    pthread_create(&addThread, nullptr, addNumbers, (void *)data);
    pthread_create(&subtractThread, nullptr, subtractNumbers, (void *)data);

    pthread_join(addThread, (void **)&addResult);
    pthread_join(subtractThread, (void **)&subtractResult);

    std::cout << "Addition of " << data[0] << " and " << data[1] << " is " << *addResult << std::endl;
    // std::cout << "Subtraction of " << data[0] << " from " << data[1] << " is " << *subtractResult << std::endl;

    delete addResult;
    delete subtractResult;

    return 0;
}
