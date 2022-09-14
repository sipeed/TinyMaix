# Copyright 2022 Sipeed Technology Co., Ltd. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

from PIL import Image
import os,sys
import numpy as np

def print_usage():
    print("Usage:")
    print("       python3 img2c.py img_name")
    print("       python3 img2c.py img_name resize_width resize_height")


if __name__ == '__main__':
    if len(sys.argv) != 2 and len(sys.argv) != 4:
        print_usage()
        exit()
    img_name  = sys.argv[1]
    img = Image.open(img_name)
    if len(sys.argv) >= 4:
        img_width = int(sys.argv[2])
        img_height = int(sys.argv[3])
        img = img.resize((img_width, img_height), Image.ANTIALIAS)

    img = np.array(img)
    out_path = os.path.splitext(img_name)[0] + ".c"
    print("Write c file to ", out_path)
    fw = open(out_path, "w")
    fw.writelines("const unsigned char pic[%d*%d*%d]={\\\n"%(img.shape[0],img.shape[1],img.shape[2]))
    for y in range(img.shape[0]):
        for x in range(img.shape[1]):
            for c in range(img.shape[2]):
                fw.writelines("%3d,"%(img[y,x,c]))
            fw.writelines(" ")
        fw.writelines("\n")
    fw.writelines("};\n")
    fw.close()
    print("Done!")





