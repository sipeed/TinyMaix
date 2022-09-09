# Introduction to tinyML
What is TinyML?  TinyML is mostly meaning run deep learning model on MCUs.  

If you are AI algorithm engineer, you may run models with 1M~1G params on server/PC/SBCs,   
they have at least hundreds MB system memory,   
it is hard to image running deep learning model on MCUs which have less 1MB ram.

If you are Embedded Engineer, you may want take advantages of deep learning in your offline or Bandwidth tension project,    
but likely your project have Cost/Space/Power/Stable limit,   
you can't switch to Expensive/Large/HighPowerConsumption/New platform.   

TinyML is useful in those situation, it makes your MCU smarter in the edge.  
The most common TinyML application scenarios are: 
1. **KWS**: Keyword Spotting, or Voice Wake Words, for voice control applications.
2. **VWW**: Visual Wake Words, for video intrusion detection
3. **Image Classification**: Identify different pictures and act accordingly
4. **Object Detection**: Detect target object position
5. **Object Counting**: Count target object
6. **Anomaly/Defect Detection**: Recognize Anomaly/Defect feature in sensor data
7. **Feature Recognition**: Recognize single-sensor or multi-sensor-fusing feature
8. ... 

# Compare TinyML with other AI deploy platform
|PlatformLevel|AI-Box  |AI-Camera    |AIoT         |TinyML       |
|---          |---     |---          |---          |---          |
|Storage Media|eMMC/SSD|eMMC/Nand/Nor|Nor/SIP Flash|Nor/SIP Flash|
|Storage Size |>=8GB   |16MB~8GB     |1~16MB       |16KB~16MB    |
|Memory Media |DDR4    |mostly DDR3  |SRAM/PSRAM   |SRAM/PSRAM   |
|Memory Size  |>=2GB   |64MB~2GB     |0.5~64MB     |2KB~8MB      |
|CPU Freq     |>=1.5G  |0.5~1.5G     |100~500M     |16~500M      |
|Computing Power|>=2TOPS|0.2~1TOPS   |50~200GOPS   |<1GOPS       |
|Deploy Language|python/c++|python/c++|mpy/C       |mostly C     |
|Typical Device|JetsonNano|RV1109 IPC|BL808/K210   |ESP32/BL618  |
|Typical Board Price|>$100|$20~$100  |$5~$20       |<$5          |
|Typical Chip Price|>$10|$4~$10      |$2~$5        |$1~$3        |


# Comparison of TinyML Inference Framework
|Name       |Model Type    |Arch support  |Porting |Code Size|Memory Size|Infer Time|Other Feature|
|---        |---           |---           |---     |---      |---        |---       |---     |
|TinyMaix   |INT8/FP16/FP32/FP8*|ARM SIMD/NEON/MVEI</br>RISC-V P/V extend|Easy|3~10KB|1.3~2X   | 1.1~1.5X    |support full static memory,</br>Maixhub online Model Training|
|NNoM       |INT8/INT16    |ARM SIMD      |Easy    |~10KB    |1.3~2X     | 1.2~1.5X |support full static memory,RNN|
|tinyengine |INT8/FP32     |ARM SIMD      |only for ARM|---  |1X         | 1X       |Strong optimization for ARM,</br>on device training|
|MicroTVM   |INT8/FP32     |ARM SIMD      |---     |---      |~3X        | 1.2~2.5X |support autotune |
|CMSIS-NN   |INT8/INT16    |ARM SIMD/MVE  |only for ARM|---  |1.3~2X     | 1.1~1.5X ||
|TFlite-micro|INT8         |ARM SIMD/Xtensa|Medium |20~100KB |~2.5X      | ~5X      ||
|NCNN       |INT8/FP32     |ARM SIMD/NEON/RVV/...|Need code clipping|200~1000KB| --- | --- |support many ops|




