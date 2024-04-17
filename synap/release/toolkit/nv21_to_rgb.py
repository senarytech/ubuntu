#!/usr/bin/env python3

#
# Convert an image from nv21 or two files (y, uv) to RGB (.png or .jpg)
#

import os
import cv2
import numpy as np
import argparse

def nv21_to_rgb(nv21file, width, height, dst:str):
    y_size = height * width
    yuv = np.fromfile(nv21file, dtype=np.uint8)
    y = yuv[0:y_size].reshape(height, width)
    uv = yuv[y_size:].reshape(height//2, width//2, 2)

    u_up = np.zeros_like(y)
    v_up = np.zeros_like(y)
    h, w = y.shape[0], y.shape[1]
    u_up = cv2.resize(uv[..., 0], None, fx=2, fy=2, interpolation=cv2.INTER_CUBIC)
    v_up = cv2.resize(uv[..., 1], None, fx=2, fy=2, interpolation=cv2.INTER_CUBIC)
    yuv = cv2.merge([y, u_up, v_up])

    bgr = cv2.cvtColor(yuv, cv2.COLOR_YUV2BGR)
    cv2.imwrite(dst, bgr)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--src', help='Source nv21 file')
    parser.add_argument('--height', type=int, help='Image height', default = 2160)
    parser.add_argument('--width', type=int, help='Image width', default = 3840)
    parser.add_argument('--dst', help='Destination image file (.png or .jpg)')
    args = parser.parse_args()
    nv21_to_rgb(args.src, args.width, args.height, args.dst)
