import json,os,time,pickle,time
import numpy as np
import scipy.io.wavfile as wavfile
import librosa
from PIL import Image

MEL_N = 40
WAV_T = (32*2)  #2s

def compute_log_mel_fbank_fromsig(signal, sample_rate,n=80):

    MEL_N = n
    # 3.分帧
    frame_size, frame_stride = 0.032, 0.032
    frame_length, frame_step = int(round(frame_size * sample_rate)), int(round(frame_stride * sample_rate))
    signal_length = len(signal)
    num_frames = int(np.ceil(np.abs(signal_length - frame_length) / frame_step)) + 1
    pad_signal_length = (num_frames - 1) * frame_step + frame_length
    z = np.zeros((pad_signal_length - signal_length))
    pad_signal = np.append(signal, z)
    indices = np.arange(0, frame_length).reshape(1, -1) + np.arange(0, num_frames * frame_step, frame_step).reshape(-1,1)
    frames = pad_signal[indices]

    #2.预增强
    pre_emphasis = 0.97
    for i in range(frames.shape[0]):
        frames[i] = np.append(frames[i][0], frames[i][1:] - pre_emphasis * frames[i][:-1])

    # 4.加窗
    # hamming = np.hamming(frame_length)
    # frames *= hamming

    # 5.N点快速傅里叶变换（N-FFT）
    NFFT = 512
    mag_frames = np.absolute(np.fft.rfft(frames, NFFT))
    pow_frames = ((mag_frames ** 2))  
    # 6.提取mel Fbank
    low_freq_mel = 0
    high_freq_mel = 2595 * np.log10(1 + (sample_rate / 2) / 700)

    n_filter = MEL_N  
    mel_points = np.linspace(low_freq_mel, high_freq_mel, n_filter + 2)  
    hz_points = 700 * (10 ** (mel_points / 2595) - 1)

    fbank = np.zeros((n_filter, int(NFFT / 2 + 1)))  
    bin = (hz_points / (sample_rate / 2)) * (NFFT / 2)  
    for i in range(1, n_filter + 1):
        left = int(bin[i - 1])
        center = int(bin[i])
        right = int(bin[i + 1])
        for j in range(left, center):
            fbank[i - 1, j + 1] = (j + 1 - bin[i - 1]) / (bin[i] - bin[i - 1])
        for j in range(center, right):
            fbank[i - 1, j + 1] = (bin[i + 1] - (j + 1)) / (bin[i + 1] - bin[i])

    # 7.提取log mel Fbank
    filter_banks = np.dot(pow_frames, fbank.T)
    filter_banks = np.where(filter_banks == 0, np.finfo(float).eps, filter_banks)
    filter_banks = 1 * np.log(filter_banks) -3  # dB
    filter_banks[np.where(filter_banks<0)]=0
    filter_banks=filter_banks*10.0
    filter_banks = filter_banks.astype(np.uint8)
    return filter_banks

def compute_log_mel_fbank(wav_file,n=80):
    """
    计算音频文件的fbank特征
    :param wav_file: 音频文件
    """
    if 'wav' in wav_file:
        sample_rate, signal = wavfile.read(wav_file)
        # print(signal)
    elif 'flac' in wav_file:
        signal, sample_rate = librosa.load(wav_file, sr=None)
        signal = signal * 32768
        signal = signal.astype(np.int16)
        return compute_log_mel_fbank_fromsig(signal, sample_rate, n=n)

def wav2pic(wav_file):
    sample_rate, signal = wavfile.read(wav_file)
    out = compute_log_mel_fbank_fromsig(signal, sample_rate,n=MEL_N)
    #print(out.shape)
    if out.shape[0] < WAV_T:
        data = np.zeros((WAV_T, MEL_N)).astype(np.uint8)
        data[:out.shape[0]] = out
    else:
        data = out[:WAV_T]
    data = Image.fromarray(data)
    data.save("%s.jpg"%(wav_file[:-4]))

def pic2array(pic_file):
    img = Image.open(pic_file)
    img = np.array(img)
    fw = open("%s.c"%pic_file[:-4], "w")
    fw.writelines("const unsigned char mel_buf[%d*%d*1]={\\\n" % (img.shape[0], img.shape[1]))
    for y in range(img.shape[0]):
        for x in range(img.shape[1]):
            for c in range(1):
                fw.writelines("%3d," % (img[y, x]))
            fw.writelines(" ")
        fw.writelines("\n")
    fw.writelines("};\n")
    fw.close()

def print_usage():
    print("Usage: python3 wav2array.py xxx.wav")

#python wav2array.py xxx.wav
import sys
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print_usage()
        exit()
    wav_file = sys.argv[1]
    wav2pic(wav_file)
    pic2array(wav_file[:-4]+'.jpg')

