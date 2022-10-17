# KWS example
It is simple KWS example for tinymaix.
Dataset is [MobvoiHotwords](http://www.openslr.org/87/)  
We use 2second audio fbank as input, output 3 classes (0: hi xiaowen, 1: nihao wenwen, 2: background)   

## audio data preprocess
Original wav is single channel, 16KHz WAV, keyword time length about 1~3s, background wav about 1~10s.
We preprocess keyword and background wav to 2 second, and apply some data augment on it.   
We calculate 512 piont FFT (easy accelerate via DSP or FPGA IP) every 32ms window, and apply MEL_N=40's mel filters.   
Now we get input dataset in shape 64x40x1

## model struct
use 3x3 Conv + Nx1 DWConv

model param 8.3KB, bin size 9.2KB, buffer size 5KB.






