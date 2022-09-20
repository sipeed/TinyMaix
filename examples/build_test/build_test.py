import os
import time
import platform
from subprocess import run, CalledProcessError

# This script do auto testing before release code.
# use convert.py in tool directory to create the model header files for tinymaix's example use.

# test on macOS, Linux maybe also work
# assert platform.system() in ["Windows", "Linux", "Darwin"]

# select the test
opt_list = {"OPT_0": 1, "OPT_1": 1}
test_list = {"MNIST": 1, "CIFAR10": 0, "VWW": 0, "MBNET": 0}


def build_test():
    os.chdir("../")
    if not os.path.exists("build"):
        os.mkdir("build")
    os.chdir("build")

    cmake_list = ["cmake"]
    msg_list = []

    for test, test_value in test_list.items():
        if not test_value:
            continue

        for opt, opt_value in opt_list.items():
            if not opt_value:
                continue

            print("\r\n{} {}...\r\n".format(test, opt))
            t_begin = time.time()

            # fill out cmake list
            for key in test_list.keys():
                if key == test:
                    cmake_list.append("-D{}=1".format(key))
                else:
                    cmake_list.append("-D{}=0".format(key))
            for key in opt_list.keys():
                if key == opt:
                    cmake_list.append("-D{}=1".format(opt))
                else:
                    cmake_list.append("-D{}=0".format(opt))
            cmake_list.append("..")

            # msg_list.append(cmake_list)
            try:
                # https://docs.python.org/zh-cn/3.9/library/subprocess.html#security-considerations
                run(cmake_list, check=True, shell=False)
                run(["make", "clean"], check=True, shell=False)
                run(["make"], check=True, shell=False)
                run(["./tinymaix_examples"], check=True, capture_output=False, shell=False)
            except CalledProcessError:
                exit(1)

            msg_list.append("{} {} from build to exec use {:.3f}s".format(test, opt, time.time() - t_begin))
            # reset cmake list
            cmake_list = ["cmake"]

    print("\r\nbuild test result:\r\n")
    for msg in msg_list:
        print(msg)


if __name__ == '__main__':
    build_test()
