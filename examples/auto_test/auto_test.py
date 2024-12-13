import os,sys,time
import datetime
import subprocess


### This script do auto testing before release code.
test_list = ["mnist_f", "mnist_q", "mbnet_f", "mbnet_q"] 


def runcmd(cmd, cwd=None):

    stdout = subprocess.check_output(cmd, shell=True, cwd=cwd)
    # stdin=PIPE,stdout=PIPE,stderr=PIPE
    a=[]
    for line in stdout.decode("utf8").strip().split('\n'):
        a.append(line.strip())
    return a

def run_h5_to_tflite(cmd, **kwargs):
    program = 'python -m tinymaix.tools.h5_to_tflite'
    cmd = program + ' ' + cmd
    return runcmd(cmd, **kwargs)

def run_tflite2tmdl(cmd, **kwargs):
    program = 'python -m tinymaix.tools.tflite2tmdl'
    cmd = program + ' ' + cmd
    return runcmd(cmd, **kwargs)

tools_dir = '../../tools/'
mnist_dir = '../mnist'
mbnet_dir = '../mbnet'


print("This script only test INT8/FP32, you need change OPT0&OPT1")
t00= time.time()
print("================Step1: test MNIST")
if "mnist_f" in test_list:
    t0= time.time()
    print("========Step1.1: test MNIST fp32")
    print("====Step1.1.1: MNIST fp32 cvt")
    run_h5_to_tflite("h5/mnist_valid.h5 tflite/mnist_valid_f.tflite 0", cwd=tools_dir)
    res = run_tflite2tmdl("tflite/mnist_valid_f.tflite tmdl/mnist_valid_f.tmdl fp32 1 28,28,1 10", cwd=tools_dir)
    assert 'Saved to tmdl/mnist_valid_f.tmdl' in res[-1], ('Step 1.1.1 ERR', res)


    print("====Step1.1.2: MNIST fp32 compile&run")
    cmd="sed -i 's/#define TM_MDL_TYPE     TM_MDL_INT8/#define TM_MDL_TYPE     TM_MDL_FP32/g' ../../include/tm_port.h"
    res = runcmd(cmd, cwd=mnist_dir)
    res = runcmd("rm -rf build && mkdir build && cd build && cmake .. && make && ./mnist", cwd=mnist_dir)
    print(res[-1])
    assert 'Predict output is: Number 2, prob 1.0' in res[-1], res[-1]

    runcmd("rm ../../tools/tmdl/mnist_valid_f.tmdl")  #clean tmdl

    t1= time.time()
    print("========Step1.1: test MNIST fp32 OK~  use %.1fs"%(t1-t0))


if "mnist_q" in test_list:
    t0= time.time()
    print("========Step1.2: test MNIST int8")
    print("====Step1.2.1: MNIST int8 cvt")
    res = run_h5_to_tflite("h5/mnist_valid.h5 tflite/mnist_valid_q.tflite 1 quant_img_mnist/ 0to1", cwd=tools_dir)
    res = run_tflite2tmdl("tflite/mnist_valid_q.tflite tmdl/mnist_valid_q.tmdl int8 1 28,28,1 10", cwd=tools_dir)

    print("====Step1.2.2: MNIST int8 compile&run")
    cmd="sed -i 's/#define TM_MDL_TYPE     TM_MDL_FP32/#define TM_MDL_TYPE     TM_MDL_INT8/g' ../../include/tm_port.h"
    res = runcmd(cmd, cwd=mnist_dir)
    res = runcmd("rm -rf build && mkdir build && cd build && cmake .. && make && ./mnist", cwd=mnist_dir)
    print(res[-1])
    assert 'Predict output is: Number 2, prob 0.996' in res[-1], res[-1]

    runcmd("rm ../../tools/tmdl/mnist_valid_q.tmdl")  #clean tmdl

    t1= time.time()
    print("========Step1.2: test MNIST int8 OK~  use %.1fs"%(t1-t0))


print("================Step2: test MBNET")
if "mbnet_f" in test_list:
    t0= time.time()
    print("========Step2.1: test MBNET fp32")
    print("====Step2.1.1: MBNET fp32 cvt")
    res = run_h5_to_tflite("h5/mbnet128_0.25.h5 tflite/mbnet128_0.25_f.tflite 0", cwd=tools_dir)
    res = run_tflite2tmdl("tflite/mbnet128_0.25_f.tflite tmdl/mbnet128_0.25_f.tmdl fp32 1 128,128,3 1000", cwd=tools_dir)

    print("====Step2.1.2: MBNET fp32 compile&run")
    cmd="sed -i 's/#define TM_MDL_TYPE     TM_MDL_INT8/#define TM_MDL_TYPE     TM_MDL_FP32/g' ../../include/tm_port.h"
    res = runcmd(cmd, cwd=mbnet_dir)
    res = runcmd("rm -rf build && mkdir build && cd build && cmake .. && make && ./mbnet", cwd=mbnet_dir)
    print(res[-1])
    assert 'Class 292 (tiger, Panthera tigris), Prob 0.8' in res[-1], res[-1]

    runcmd("rm ../../tools/tmdl/mbnet128_0.25_f.tmdl")  #clean tmdl

    t1= time.time()
    print("========Step2.1: test MBNET fp32 OK~  use %.1fs"%(t1-t0))


if "mbnet_q" in test_list:
    t0= time.time()
    print("========Step2.2: test MBNET int8")
    print("====Step2.2.1: MBNET int8 cvt")
    res = run_h5_to_tflite("h5/mbnet128_0.25.h5 tflite/mbnet128_0.25_q.tflite 1 quant_img128/ 0to1", cwd=tools_dir)
    res = run_tflite2tmdl("tflite/mbnet128_0.25_q.tflite tmdl/mbnet128_0.25_q.tmdl int8 1 128,128,3 1000", cwd=tools_dir)

    print("====Step2.2.2: MBNET int8 compile&run")
    cmd="sed -i 's/#define TM_MDL_TYPE     TM_MDL_FP32/#define TM_MDL_TYPE     TM_MDL_INT8/g' ../../include/tm_port.h"
    res = runcmd(cmd, cwd=mbnet_dir)
    res = runcmd("rm -rf build && mkdir build && cd build && cmake .. && make && ./mbnet", cwd=mbnet_dir)
    print(res[-1])
    assert 'Class 292 (tiger, Panthera tigris), Prob 0.8' in res[-1]

    runcmd("rm ../../tools/tmdl/mbnet128_0.25_q.tmdl")  #clean tmdl

    t1= time.time()
    print("========Step2.2: test MBNET int8 OK~  use %.1fs"%(t1-t0))

t11= time.time()
print("================ALL PASS~  cost %.1fs"%(t11-t00))

