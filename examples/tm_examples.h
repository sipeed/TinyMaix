#ifndef TM_EXAMPLES_H
#define TM_EXAMPLES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../include/tinymaix.h"

#ifndef TM_USE_EXAMPLE_MNIST
#define TM_USE_EXAMPLE_MNIST 0
#endif

#ifndef TM_USE_EXAMPLE_CIFAR10
#define TM_USE_EXAMPLE_CIFAR10 0
#endif

#ifndef TM_USE_EXAMPLE_VWW
#define TM_USE_EXAMPLE_VWW 0
#endif

#ifndef TM_USE_EXAMPLE_MBNET
#define TM_USE_EXAMPLE_MBNET 0
#endif

#if TM_USE_EXAMPLE_MNIST
#include "mnist/tm_example_mnist.h"
#endif

#if TM_USE_EXAMPLE_CIFAR10
#include "cifar10/tm_example_cifar10.h"
#endif

#if TM_USE_EXAMPLE_VWW
#include "vww/tm_example_vww.h"
#endif

#if TM_USE_EXAMPLE_MBNET
#include "mbnet/tm_example_mbnet.h"
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*TM_EXAMPLES_H*/
