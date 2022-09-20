# Port TinyMaix to WebAssembly

## Chip

| Item         | Parameter                                    |
| ------------ |----------------------------------------------|
| Chip         | 4 x Intel(R) Core(TM) i5-4590T CPU @ 2.00GHz |
| Arch         | AMD64                                        |
| Freq         | 2.00GHz                                      |
| Flash        | 256G                                         |
| RAM          | 8GB                                          |
| Acceleration | None                                         |

## Board

Lenovo ThinkCentre M73

## Development Environment

Ubuntu Server 22.04  
gcc 11.2.0  
Emscripten 3.1.21  
nodejs v12.22.9  

## Step/Project

https://github.com/liux-pro/TinyMaix-WebAssembly/tree/main/examples/webAssembly_test

## Result

`Native` means gcc compile, run in linux directly.  
`WebAssembly` means emscripten compile, run in nodejs environment.  

| config         | mnist | cifar | vww96 | mbnet128 | kws    | Note |
| -------------- | ----- | ----- | ----- |----------| ------ | ---- |
| O0 Native      | 0.042 | 0.939 | 5.224 | 8.953    | 16.954 |      |
| O0 WebAssembly | <1    | 5     | 16    | 28       | 40     |      |
| O1 Native      | 0.039 | 0.889 | 5.147 | 7.090    | 14.127 |      |
| O1 WebAssembly | <1    | 4     | 17    | 24       | 35     |      |


## Author

[Legend](https://github.com/liux-pro) 