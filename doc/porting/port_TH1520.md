# Port TinyMaix to T-head TH1520

### Test Chip

| Item         | Parameter     |
| ------------ | ------------- |
| Chip         | T-head TH1520 |
| Arch         | Xuantie C920  |
| Freq         | 1848MHz       |
| Flash        | 16GByte       |
| RAM          | 4GByte        |
| Acceleration | RVV 0.7.1     |

### Board

![](assets/TH1520.jpg)


### Development Environment
Tina Linux 

### Step/Project
Nothing, just compile with `-mcpu=c920`

### Result

TM_MDL_INT8 (ms)

| config | mnist | cifar | vww96  | mbnet128 |
| ------ | ----- | ----- | ------ | -------- |
| O0 RV64V | 0.147 | 4.775 | 13.343 | 25.406 |
| O1 RV64V | 0.179 | 4.324 | 12.479 | 20.655 |

TM_MDL_FP16 (ms)

| config | mbnet128 | mbnet224 |
| ------ | -------- | -------- |
| O0 RV64V | 21.845 | 245.959 |
| O1 RV64V | 17.203 | 288.661 |


### Author
YuzukiTsuru <gloomyghost@gloomyghost.com>
