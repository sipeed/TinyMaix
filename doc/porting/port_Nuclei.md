# Port TinyMaix to Nuclei RISC-V Processor

Nuclei System Technology is a RISC-V CPU IP processor vendor, click https://nucleisys.com/ to learn more.

Nuclei SDK is an open source software project to support Nuclei RISC-V CPU embedded software development,
support Nuclei 200/300/600/900 series processor, and integrated into Nuclei Studio IDE via NPK package.

TinyMaix port to Nuclei RISC-V Processor can be found in https://github.com/riscv-mcu/TinyMaix

## Test Nuclei RISC-V Processor

| Item         | Parameter      |
| ------------ | -------------- |
| CPU Core     | Nuclei 200/300/600/900 series |
| Arch         | RV32/RV64      |
| Freq         | 16MHz/100MHz   |
| Flash        | 16MBytes       |
| RAM          | ILM/DLM 512K, DDR 1.5G  |
| Acceleration | RVV 1.0, RVP 0.5.3    |

> - RVP support is WIP.
> - ILM/DLM need a 512K version bitstream.
> - Other Nuclei processor based chip can be easily supported.

## Board

- [Nuclei DDR200T for 200/300 CPU Series](https://nucleisys.com/developboard.php#ddr200t)
- [Xilinx VCU118 for 900 Series](https://www.xilinx.com/products/boards-and-kits/vcu118.html)

> Support can be easily ported to other Nuclei RISC-V processor based CPUs.

## Development Environment

- [Nuclei SDK](https://github.com/Nuclei-Software/nuclei-sdk)
- [Nuclei Studio](https://www.rvmcu.com/nucleistudio.html)

## Step/Project

1. Download Nuclei Studio

~~~shell
# Download Nuclei Studio from https://nucleisys.com/download.php#tools
~~~

2. Clone Nuclei SDK Project

~~~shell
git clone https://github.com/Nuclei-Software/nuclei-sdk
# export NUCLEI_SDK_ROOT environment variable is required
export NUCLEI_SDK_ROOT=(readlink -f nuclei-sdk)
# follow steps in https://doc.nucleisys.com/nuclei_sdk/quickstart.html#use-prebuilt-tools-in-nuclei-studio setup toolchain environment
~~~

3. Clone Tinymaix ported for Nuclei

~~~shell
git clone https://github.com/riscv-mcu/TinyMaix
~~~

4. Build and run tinymaix examples

> Currently supported cases are cifar10/kws/mnist/mbnet

Take cifar10 as example using Nuclei DDR200T board, N300 RISC-V CPU.

> Since the examples are ported to nuclei-sdk, so you can easily
> use it just like sdk application, see guide here
> https://doc.nucleisys.com/nuclei_sdk/quickstart.html#build-run-and-debug-sample-application

~~~shell
cd TinyMaix
# checkout branch for nuclei processor
git checkout nuclei-main
cd examples/cifar10/
# choose n300fd(rv32imafdc) as example
# clean and build project
make CORE=n300fd clean all
# connect fpga board, and program bitstream using xilinx vivado tools
# connect using hbird debugger to fpga board
# download program to fpga board, and monitor on com port
make CORE=n300fd upload
# you can also test it using qemu
make CORE=n300fd run_qemu
~~~

5. Future using tinymaix directly using Nuclei Studio NPK feature is WIP.

About NPK, check https://github.com/Nuclei-Software/nuclei-sdk/wiki/Nuclei-Studio-NPK-Introduction

## Result

### N300 16MHz

| config | mnist | cifar | vww  | kws |
| ------ | ----- | ----- | ------ | -------- |
| RV32IMAFDC | 11.102 | 795.068 | 2946.493 | 152.044  |

> Other CPU series can be easily tested using Nuclei SDK
> using different fpga bitstream.

## Author

[Huaqi Fang](https://github.com/fanghuaqi)
