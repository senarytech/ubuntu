#!/usr/bin/env python3

#
# Convert an image to YUV, rescale and save the Y/UV components in separate binary files
#

import os
import cv2
import numpy as np
import argparse

def rgb_to_nv21(rgb_image, nv21_image, w, h):
    fullimg = cv2.imread(rgb_image, 3)
    width = w if w != 0 else fullimg.shape[1]
    height = h if h != 0 else fullimg.shape[0]
    y_size = height * width
    scale = min(width/fullimg.shape[1], height/fullimg.shape[0])
    if scale < 1:
        # Apply gaussian blur when downscaling to avoid artifacts
        sigma =  (1 / scale - 1) / 2
        fullimg = cv2.GaussianBlur(fullimg, (0, 0), sigma, sigma)
    img = cv2.resize(fullimg, (width,height), interpolation=cv2.INTER_AREA)

    # to YUV and normalize
    img_ycc = cv2.cvtColor(img, cv2.COLOR_BGR2YUV)
    img_y = img_ycc[:,:,0]

    sigma =  (1 / 0.5 - 1) / 2
    blurred_img = cv2.GaussianBlur(img_ycc, (0, 0), sigma, sigma)
    img_quarter = cv2.resize(blurred_img, (width//2,height//2), interpolation=cv2.INTER_AREA)

    img_uv = img_quarter[:,:,1:3]
    img_yuv = np.concatenate((np.reshape(img_y, y_size), np.reshape(img_uv, y_size // 2)))
    img_yuv.astype('uint8').tofile(nv21_image)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--src', help='Source RGB Image (.png or .jpg)')
    parser.add_argument('--dst', help='Destination nv21 file')
    parser.add_argument('--height', type=int, help='Height', default = 0)
    parser.add_argument('--width', type=int, help='Width', default = 0)
    args = parser.parse_args()
    rgb_to_nv21(args.src, args.dst, args.width, args.height)

   
