from h5_to_tflite import h5_to_tflite
from tflite2tmdl import tflite2tmdl, mdl_type_dict


# This script use to convert a group of preset models for tinymaix's examples

# h5_to_tflite.py h5/mnist_valid.h5 tflite/mnist_valid_f.tflite 0
# h5_to_tflite.py h5/mnist_valid.h5 tflite/mnist_valid_q.tflite 1 quant_img_mnist/ 0to1
# h5_to_tflite.py h5/mbnet128_0.25.h5 tflite/mbnet128_0.25_f.tflite 0
# h5_to_tflite.py h5/mbnet128_0.25.h5 tflite/mbnet128_0.25_q.tflite 1 quant_img128/ 0to1

# tflite2tmdl.py tflite/mnist_valid_q.tflite tmdl/mnist_valid_q.tmdl int8 1 28,28,1 10
# tflite2tmdl.py tflite/mnist_valid_f.tflite tmdl/mnist_valid_f.tmdl fp32 1 28,28,1 10
# tflite2tmdl.py tflite/mbnet128_0.25_f.tflite tmdl/mbnet128_0.25_f.tmdl fp32 1 128,128,3 1000
# tflite2tmdl.py tflite/mbnet128_0.25_q.tflite tmdl/mbnet128_0.25_q.tmdl int8 1 128,128,3 1000


def tm_examples_tflite():
    # mnist
    h5_to_tflite("h5/mnist_valid.h5", "tflite/mnist_valid_q.tflite", 1, "quant_img_mnist/", "0to1")
    h5_to_tflite("h5/mnist_valid.h5", "tflite/mnist_valid_f.tflite", 0)

    # cifar10

    # vww

    # mbnet
    h5_to_tflite("h5/mbnet128_0.25.h5", "tflite/mbnet128_0.25_q.tflite", 1, "quant_img128/", "0to1")
    h5_to_tflite("h5/mbnet128_0.25.h5", "tflite/mbnet128_0.25_f.tflite", 0)


def tm_examples_header():
    # mnist
    tflite2tmdl("tflite/mnist_valid_q.tflite", "tmdl/mnist_valid_q.tmdl", mdl_type_dict["int8"], 1, [28, 28, 1], [10])
    tflite2tmdl("tflite/mnist_valid_f.tflite", "tmdl/mnist_valid_f.tmdl", mdl_type_dict["fp32"], 1, [28, 28, 1], [10])

    # cifar10

    # vww

    # mbent
    tflite2tmdl("tflite/mbnet128_0.25_q.tflite",
                "tmdl/mbnet128_0.25_q.tmdl",
                mdl_type_dict["int8"],
                1, [128, 128, 3], [1000])

    tflite2tmdl("tflite/mbnet128_0.25_f.tflite",
                "tmdl/mbnet128_0.25_f.tmdl",
                mdl_type_dict["fp32"],
                1, [128, 128, 3], [1000])


if __name__ == '__main__':
    # tm_examples_tflite()
    tm_examples_header()
