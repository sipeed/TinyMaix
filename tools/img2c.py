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
    print("       python3 img2c.py img_name resize_width resize_height to_grayscale")
    print("e.g.   python3 img2c.py img.png 28 28 1")


if __name__ == '__main__':
    if len(sys.argv) != 2 and len(sys.argv) != 4 and len(sys.argv) != 5:
        print_usage()
        exit()
    img_name  = sys.argv[1]
    img = Image.open(img_name)
    if len(sys.argv) >= 4:
        img_width = int(sys.argv[2])
        img_height = int(sys.argv[3])
        img = img.resize((img_width, img_height), Image.ANTIALIAS)
    if len(sys.argv) >= 5:
        to_grayscale = int(sys.argv[4])
    else:
        to_grayscale = False

    img = np.array(img)

    if len(img.shape) < 3 or to_grayscale:
        channels = 1 # It's a grayscale image;
    else:
        channels = img.shape[2]; # It's a color image
    print(f"\n\nchannels {channels}\n\n")

    out_path = os.path.splitext(img_name)[0] + ".c"
    print("Write c file to ", out_path)
    fw = open(out_path, "w")
    fw.writelines("const unsigned char pic[%d*%d*%d]={\\\n"%(img.shape[0],img.shape[1],channels))
    for y in range(img.shape[0]):
        for x in range(img.shape[1]):
            if img.shape[2] >=3:
                if to_grayscale:
                    grayscale = int(0.299 * img[y,x,0] + 0.587 * img[y,x,1] + 0.114 * img[y,x,2])
                    fw.writelines("%3d,"%(grayscale))
                else:
                    for c in range(img.shape[2]):
                        fw.writelines("%3d,"%(img[y,x,c]))
            else:
                fw.writelines("%3d,"%(img[y,x]))
            fw.writelines(" ")
        fw.writelines("\n")
    fw.writelines("};\n")
    fw.close()
    print("Done!")
