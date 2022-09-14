import os,sys,time
from subprocess import *
import datetime,time
import threading

### This script do auto testing before release code.
test_list = ["mnist_f", "mnist_q", "mbnet_f", "mbnet_q"] 


def runcmd(cmd):
    r=Popen(cmd,stdin=PIPE,stdout=PIPE,stderr=PIPE, shell=True)
    a=[]
    for line in r.stdout.readlines():
        a.append(line.decode("utf8").strip())
    return a


print("This script only test INT8/FP32, you need change OPT0&OPT1")
t00= time.time()
print("================Step1: test MNIST")
if "mnist_f" in test_list:
    t0= time.time()
    print("========Step1.1: test MNIST fp32")
    print("====Step1.1.1: MNIST fp32 cvt")
    cmd="cd ../../tools/ && python3 h5_to_tflite.py h5/mnist_valid.h5 tflite/mnist_valid_f.tflite 0 && python3 tflite2tmdl.py tflite/mnist_valid_f.tflite tmdl/mnist_valid_f.tmdl fp32 1 28,28,1 10"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "Saved to tinymaix model header to tmdl/mnist_valid_f.h":
        print("====Step1.1.1: OK~")
    else:
        print("====Step1.1.1: ERR!!!")
        exit(-1)

    print("====Step1.1.2: MNIST fp32 compile&run")
    cmd="cd ../mnist && sed -i 's/#define TM_MDL_TYPE     TM_MDL_INT8/#define TM_MDL_TYPE     TM_MDL_FP32/g' ../../include/tm_port.h && rm -rf build && mkdir build && cd build && cmake .. && make && ./mnist"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "### Predict output is: Number 2, prob 1.000":
        print("====Step1.1.2: OK~")
        runcmd("rm ../../tools/tmdl/mnist_valid_f.tmdl")  #clean tmdl
    else:
        print("====Step1.1.2: ERR!!!")
        exit(-2)
    t1= time.time()
    print("========Step1.1: test MNIST fp32 OK~  use %.1fs"%(t1-t0))


if "mnist_q" in test_list:
    t0= time.time()
    print("========Step1.2: test MNIST int8")
    print("====Step1.2.1: MNIST int8 cvt")
    cmd="cd ../../tools/ && python3 h5_to_tflite.py h5/mnist_valid.h5 tflite/mnist_valid_q.tflite 1 quant_img_mnist/ 0to1 && python3 tflite2tmdl.py tflite/mnist_valid_q.tflite tmdl/mnist_valid_q.tmdl int8 1 28,28,1 10"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "Saved to tinymaix model header to tmdl/mnist_valid_q.h":
        print("====Step1.2.1: OK~")
    else:
        print("====Step1.2.1: ERR!!!")
        exit(-1)

    print("====Step1.2.2: MNIST int8 compile&run")

    cmd="cd ../mnist && sed -i 's/#define TM_MDL_TYPE     TM_MDL_FP32/#define TM_MDL_TYPE     TM_MDL_INT8/g' ../../include/tm_port.h && rm -rf build && mkdir build && cd build && cmake .. && make && ./mnist"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "### Predict output is: Number 2, prob 0.996":
        print("====Step1.2.2: OK~")
        runcmd("rm ../../tools/tmdl/mnist_valid_q.tmdl")  #clean tmdl
    else:
        print("====Step1.2.2: ERR!!!")
        exit(-2)

    t1= time.time()
    print("========Step1.2: test MNIST int8 OK~  use %.1fs"%(t1-t0))


print("================Step2: test MBNET")
if "mbnet_f" in test_list:
    t0= time.time()
    print("========Step2.1: test MBNET fp32")
    print("====Step2.1.1: MBNET fp32 cvt")
    cmd="cd ../../tools/ && python3 h5_to_tflite.py h5/mbnet128_0.25.h5 tflite/mbnet128_0.25_f.tflite 0 && python3 tflite2tmdl.py tflite/mbnet128_0.25_f.tflite tmdl/mbnet128_0.25_f.tmdl fp32 1 128,128,3 1000"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "Saved to tinymaix model header to tmdl/mbnet128_0.25_f.h":
        print("====Step2.1.1: OK~")
    else:
        print("====Step2.1.1: ERR!!!")
        exit(-1)

    print("====Step2.1.2: MBNET fp32 compile&run")

    cmd="cd ../mbnet && sed -i 's/#define TM_MDL_TYPE     TM_MDL_INT8/#define TM_MDL_TYPE     TM_MDL_FP32/g' ../../include/tm_port.h && rm -rf build && mkdir build && cd build && cmake .. && make && ./mbnet"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "### Predict output is: Class 292 (tiger, Panthera tigris), Prob 0.866" or \
        res[-1] == "### Predict output is: Class 292 (tiger, Panthera tigris), Prob 0.891":
        print("====Step2.1.2: OK~")
        runcmd("rm ../../tools/tmdl/mbnet128_0.25_f.tmdl")  #clean tmdl
    else:
        print("====Step2.1.2: ERR!!!")
        exit(-2)

    t1= time.time()
    print("========Step2.1: test MBNET fp32 OK~  use %.1fs"%(t1-t0))


if "mbnet_q" in test_list:
    t0= time.time()
    print("========Step2.2: test MBNET int8")
    print("====Step2.2.1: MBNET int8 cvt")
    cmd="cd ../../tools/ && python3 h5_to_tflite.py h5/mbnet128_0.25.h5 tflite/mbnet128_0.25_q.tflite 1 quant_img128/ 0to1 && python3 tflite2tmdl.py tflite/mbnet128_0.25_q.tflite tmdl/mbnet128_0.25_q.tmdl int8 1 128,128,3 1000"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "Saved to tinymaix model header to tmdl/mbnet128_0.25_q.h":
        print("====Step2.2.1: OK~")
    else:
        print("====Step2.2.1: ERR!!!")
        exit(-1)

    print("====Step2.2.2: MBNET int8 compile&run")

    cmd="cd ../mbnet && sed -i 's/#define TM_MDL_TYPE     TM_MDL_FP32/#define TM_MDL_TYPE     TM_MDL_INT8/g' ../../include/tm_port.h && rm -rf build && mkdir build && cd build && cmake .. && make && ./mbnet"
    res = runcmd(cmd)
    print(res[-1])
    if res[-1] == "### Predict output is: Class 292 (tiger, Panthera tigris), Prob 0.824":
        print("====Step2.2.2: OK~")
        runcmd("rm ../../tools/tmdl/mbnet128_0.25_q.tmdl")  #clean tmdl
    else:
        print("====Step2.2.2: ERR!!!")
        exit(-2)

    t1= time.time()
    print("========Step2.2: test MBNET int8 OK~  use %.1fs"%(t1-t0))

t11= time.time()
print("================ALL PASS~  cost %.1fs"%(t11-t00))

