MaixHub image classification demo
========

* Train your model on [MaixHub](https://maixhub.com) first
* Download model file and unzip, copy `model-*-final.h` to this directory, rename to `model_final.h`, and copy `model_info.h` too
* Change config in main.c according to your model(params can be found in report.json)
* Use `python3 ../../tools/img2c.py test_img.jpg width height` to generate test input image c file and copy to this directory
* `mkdir build && cd build && cmake .. && make`, then execute `./yolov2` if you use Linux


Change `include/tm_port.h` configs like `TM_MAX_KCSIZE` according to your model size.

