#include "stdio.h"
#include "tm_examples.h"

int main(int argc, char **argv)
{
#if TM_USE_EXAMPLE_MNIST
    tm_example_mnist();
#endif

#if TM_USE_EXAMPLE_CIFAR10
    tm_example_cifar10();
#endif

#if TM_USE_EXAMPLE_VWW
    tm_example_vww();
#endif

#if TM_USE_EXAMPLE_MBNET
    tm_example_mbnet();
#endif
}
