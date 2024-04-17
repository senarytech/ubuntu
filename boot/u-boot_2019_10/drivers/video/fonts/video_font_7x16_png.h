#ifndef _FONT_7X16_H
#define _FONT_7X16_H
static struct {
  unsigned width;
  unsigned height;
  unsigned cwidth;
  unsigned cheight;
  unsigned char rundata[];
} font = {
  .width = 668,
  .height = 16,
  .cwidth = 7,
  .cheight = 16,
  .rundata = {
 0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7f,0x7e,0x82,0x03,0x82,0x7f,0x7f,0x5f,0x82,0x0b,0x82,0x14,0x81,0x0b,0x81,0x11,0x81,0x0c,0x82,0x09,0x81,0x08,0x81,0x07,0x81,0x03,0x81,0x06,0x83,0x68,0x83,0x04,0x81,0x04,0x83,0x17,0x81,0x05,0x81,0x01,0x81,0x0c,0x81,0x04,0x82,0x07,0x83,0x04,0x81,0x07,0x81,0x05,0x81,0x06,0x81,0x25,0x81,0x02,0x84,0x02,0x83,0x05,0x84,0x03,0x84,0x05,0x82,0x02,0x85,0x04,0x83,0x02,0x86,0x02,0x84,0x03,0x84,0x27,0x83,0x0b,0x82,0x03,0x85,0x04,0x83,0x02,0x84,0x03,0x86,0x01,0x86,0x03,0x83,0x02,0x81,0x04,0x81,0x01,0x85,0x04,0x83,0x02,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x82,0x03,0x81,0x02,0x84,0x02,0x85,0x03,0x84,0x02,0x85,0x03,0x84,0x01,0x87,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x82,0x05,0x81,0x01,0x81,0x04,0x82,0x05,0x81,0x01,0x86,0x03,0x81,0x04,0x81,0x08,0x81,0x05,0x82,0x0e,0x81,0x0a,0x81,0x11,0x81,0x0b,0x81,0x0b,0x81,0x14,0x81,0x08,0x81,0x37,0x81,0x30,0x81,0x06,0x81,0x06,0x81,0x17,0x81,0x05,0x81,0x01,0x81,0x05,0x81,0x01,0x81,0x03,0x83,0x02,0x81,0x02,0x81,0x05,0x81,0x07,0x81,0x07,0x81,0x05,0x81,0x04,0x81,0x01,0x81,0x01,0x81,0x22,0x81,0x03,0x81,0x02,0x81,0x04,0x81,0x04,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x04,0x82,0x02,0x81,0x07,0x81,0x03,0x81,0x05,0x82,0x01,0x81,0x04,0x81,0x01,0x82,0x02,0x81,0x26,0x81,0x03,0x81,0x03,0x83,0x04,0x82,0x03,0x81,0x04,0x81,0x02,0x81,0x03,0x81,0x01,0x81,0x03,0x81,0x02,0x81,0x06,0x81,0x07,0x81,0x03,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x08,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x06,0x82,0x02,0x82,0x01,0x82,0x03,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x01,0x81,0x04,0x82,0x02,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x03,0x81,0x06,0x82,0x03,0x81,0x05,0x81,0x07,0x81,0x04,0x81,0x02,0x81,0x18,0x81,0x11,0x81,0x0b,0x81,0x0b,0x81,0x14,0x81,0x08,0x81,0x37,0x81,0x30,0x81,0x06,0x81,0x06,0x81,0x17,0x81,0x05,0x81,0x01,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x05,0x81,0x07,0x81,0x06,0x81,0x07,0x81,0x04,0x83,0x05,0x81,0x1d,0x81,0x02,0x81,0x04,0x81,0x03,0x81,0x09,0x81,0x06,0x81,0x03,0x81,0x01,0x81,0x02,0x81,0x06,0x81,0x0a,0x81,0x02,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x14,0x81,0x08,0x81,0x0b,0x81,0x02,0x81,0x02,0x82,0x03,0x82,0x03,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x06,0x81,0x06,0x81,0x04,0x81,0x03,0x81,0x08,0x81,0x02,0x81,0x02,0x81,0x03,0x81,0x06,0x82,0x02,0x82,0x01,0x81,0x01,0x81,0x02,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x08,0x81,0x04,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x01,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x03,0x81,0x01,0x81,0x07,0x81,0x04,0x81,0x05,0x81,0x07,0x81,0x03,0x81,0x04,0x81,0x11,0x83,0x03,0x84,0x04,0x83,0x04,0x84,0x03,0x83,0x03,0x85,0x03,0x84,0x02,0x81,0x01,0x82,0x03,0x83,0x05,0x83,0x03,0x81,0x03,0x81,0x04,0x81,0x04,0x85,0x02,0x81,0x01,0x82,0x04,0x83,0x03,0x84,0x04,0x84,0x03,0x84,0x03,0x83,0x03,0x85,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x01,0x81,0x05,0x81,0x01,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x85,0x04,0x81,0x06,0x81,0x06,0x81,0x17,0x81,0x0b,0x86,0x01,0x81,0x01,0x81,0x04,0x82,0x02,0x81,0x03,0x82,0x0d,0x81,0x07,0x81,0x04,0x83,0x05,0x81,0x1c,0x81,0x03,0x81,0x04,0x81,0x03,0x81,0x09,0x81,0x06,0x81,0x02,0x82,0x01,0x81,0x02,0x85,0x02,0x81,0x01,0x83,0x06,0x81,0x02,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x06,0x81,0x06,0x83,0x0a,0x83,0x06,0x82,0x02,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x06,0x81,0x06,0x81,0x04,0x81,0x03,0x81,0x08,0x81,0x02,0x81,0x01,0x81,0x04,0x81,0x06,0x81,0x01,0x82,0x01,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x01,0x82,0x07,0x81,0x04,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x03,0x82,0x04,0x81,0x01,0x81,0x06,0x81,0x05,0x81,0x06,0x81,0x06,0x81,0x19,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x82,0x05,0x81,0x03,0x81,0x02,0x82,0x02,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x02,0x82,0x02,0x81,0x04,0x81,0x07,0x81,0x03,0x81,0x02,0x81,0x05,0x81,0x04,0x81,0x01,0x81,0x01,0x81,0x02,0x82,0x02,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x03,0x82,0x02,0x81,0x01,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x01,0x81,0x05,0x81,0x02,0x81,0x01,0x81,0x03,0x81,0x03,0x81,0x06,0x81,0x04,0x81,0x06,0x81,0x06,0x81,0x17,0x81,0x0c,0x81,0x01,0x81,0x03,0x83,0x06,0x82,0x04,0x82,0x0d,0x81,0x07,0x81,0x03,0x81,0x01,0x81,0x01,0x81,0x04,0x81,0x1c,0x81,0x03,0x81,0x02,0x81,0x01,0x81,0x03,0x81,0x08,0x81,0x04,0x83,0x03,0x81,0x02,0x81,0x06,0x82,0x01,0x82,0x02,0x82,0x04,0x81,0x04,0x84,0x02,0x81,0x03,0x82,0x03,0x81,0x06,0x81,0x04,0x82,0x05,0x86,0x05,0x82,0x03,0x82,0x03,0x81,0x02,0x83,0x02,0x81,0x02,0x81,0x02,0x85,0x02,0x81,0x06,0x81,0x04,0x81,0x01,0x86,0x01,0x86,0x01,0x81,0x03,0x82,0x01,0x86,0x03,0x81,0x08,0x81,0x02,0x83,0x04,0x81,0x06,0x81,0x01,0x82,0x01,0x81,0x01,0x81,0x01,0x82,0x01,0x81,0x01,0x81,0x04,0x81,0x01,0x85,0x02,0x81,0x04,0x81,0x01,0x85,0x03,0x84,0x04,0x81,0x04,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x03,0x82,0x05,0x81,0x06,0x82,0x05,0x81,0x06,0x81,0x06,0x81,0x1d,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x06,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x04,0x81,0x07,0x81,0x03,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x03,0x81,0x05,0x81,0x08,0x81,0x04,0x81,0x03,0x81,0x03,0x81,0x01,0x81,0x03,0x81,0x01,0x81,0x01,0x81,0x03,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x03,0x82,0x07,0x81,0x07,0x82,0x02,0x83,0x10,0x81,0x0c,0x81,0x01,0x81,0x05,0x83,0x02,0x82,0x01,0x82,0x02,0x81,0x02,0x81,0x01,0x81,0x0a,0x81,0x07,0x81,0x05,0x81,0x03,0x87,0x09,0x83,0x0c,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x07,0x81,0x08,0x81,0x01,0x81,0x03,0x81,0x07,0x81,0x01,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x04,0x81,0x02,0x83,0x01,0x81,0x0f,0x82,0x10,0x82,0x03,0x81,0x04,0x81,0x01,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x02,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x08,0x81,0x02,0x81,0x02,0x81,0x03,0x81,0x06,0x81,0x01,0x82,0x01,0x81,0x01,0x81,0x02,0x81,0x01,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x03,0x81,0x07,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x02,0x81,0x02,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x01,0x81,0x03,0x82,0x05,0x81,0x06,0x81,0x06,0x81,0x07,0x81,0x05,0x81,0x1a,0x84,0x02,0x81,0x03,0x81,0x02,0x81,0x06,0x81,0x03,0x81,0x02,0x85,0x04,0x81,0x04,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x04,0x81,0x07,0x81,0x03,0x82,0x07,0x81,0x04,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x03,0x81,0x06,0x83,0x05,0x81,0x04,0x81,0x03,0x81,0x03,0x81,0x01,0x81,0x03,0x81,0x01,0x81,0x01,0x81,0x04,0x81,0x05,0x81,0x01,0x81,0x05,0x81,0x06,0x81,0x06,0x81,0x06,0x81,0x07,0x83,0x18,0x86,0x04,0x81,0x01,0x81,0x04,0x81,0x02,0x81,0x01,0x81,0x02,0x83,0x0a,0x81,0x07,0x81,0x0c,0x81,0x1b,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x06,0x81,0x09,0x81,0x01,0x86,0x06,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x06,0x81,0x11,0x83,0x02,0x86,0x02,0x83,0x0a,0x81,0x01,0x81,0x02,0x81,0x02,0x84,0x02,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x08,0x81,0x02,0x81,0x02,0x82,0x02,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x02,0x81,0x01,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x06,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x03,0x82,0x03,0x82,0x01,0x82,0x03,0x81,0x02,0x81,0x04,0x81,0x05,0x81,0x07,0x81,0x07,0x81,0x05,0x81,0x19,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x06,0x81,0x03,0x81,0x02,0x81,0x08,0x81,0x04,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x04,0x81,0x07,0x81,0x03,0x81,0x01,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x03,0x81,0x09,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x03,0x81,0x01,0x81,0x03,0x82,0x01,0x82,0x03,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x07,0x81,0x06,0x81,0x06,0x81,0x17,0x81,0x0b,0x81,0x02,0x81,0x03,0x81,0x01,0x81,0x01,0x81,0x04,0x81,0x02,0x81,0x01,0x82,0x02,0x81,0x0c,0x81,0x05,0x81,0x0d,0x81,0x06,0x81,0x0d,0x81,0x05,0x81,0x06,0x81,0x02,0x81,0x04,0x81,0x05,0x81,0x05,0x81,0x04,0x81,0x05,0x81,0x02,0x81,0x03,0x82,0x02,0x81,0x02,0x82,0x03,0x81,0x04,0x81,0x04,0x81,0x01,0x81,0x03,0x81,0x04,0x81,0x06,0x81,0x09,0x81,0x08,0x81,0x08,0x81,0x04,0x81,0x02,0x83,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x02,0x81,0x03,0x81,0x01,0x81,0x03,0x81,0x02,0x81,0x06,0x81,0x07,0x81,0x03,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x04,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x06,0x81,0x04,0x81,0x01,0x81,0x03,0x82,0x02,0x81,0x02,0x81,0x02,0x81,0x07,0x81,0x02,0x82,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x03,0x82,0x03,0x81,0x03,0x81,0x03,0x81,0x02,0x81,0x04,0x81,0x04,0x82,0x07,0x81,0x08,0x81,0x04,0x81,0x19,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x82,0x05,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x04,0x81,0x07,0x81,0x03,0x81,0x02,0x81,0x05,0x81,0x04,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x02,0x81,0x03,0x81,0x03,0x81,0x05,0x81,0x03,0x81,0x04,0x81,0x04,0x81,0x03,0x81,0x04,0x81,0x05,0x81,0x01,0x81,0x04,0x81,0x01,0x81,0x04,0x82,0x04,0x81,0x08,0x81,0x06,0x81,0x06,0x81,0x17,0x81,0x0b,0x81,0x01,0x81,0x05,0x83,0x06,0x82,0x03,0x83,0x01,0x81,0x0b,0x81,0x05,0x81,0x0d,0x81,0x06,0x81,0x0d,0x81,0x05,0x81,0x06,0x84,0x02,0x85,0x02,0x86,0x02,0x84,0x06,0x81,0x03,0x84,0x03,0x84,0x03,0x81,0x06,0x84,0x03,0x83,0x05,0x81,0x06,0x81,0x1b,0x81,0x04,0x82,0x05,0x81,0x04,0x81,0x01,0x85,0x04,0x83,0x02,0x84,0x03,0x86,0x01,0x81,0x08,0x83,0x02,0x81,0x04,0x81,0x01,0x85,0x03,0x83,0x03,0x81,0x04,0x81,0x01,0x86,0x01,0x81,0x04,0x81,0x01,0x81,0x03,0x82,0x02,0x84,0x02,0x81,0x07,0x84,0x02,0x81,0x05,0x81,0x01,0x84,0x04,0x81,0x05,0x84,0x04,0x82,0x03,0x81,0x03,0x81,0x02,0x81,0x04,0x81,0x03,0x81,0x04,0x86,0x03,0x81,0x08,0x81,0x04,0x81,0x1a,0x84,0x02,0x84,0x04,0x83,0x04,0x84,0x03,0x83,0x05,0x81,0x05,0x84,0x02,0x81,0x03,0x81,0x02,0x85,0x05,0x81,0x03,0x81,0x03,0x81,0x05,0x82,0x02,0x81,0x01,0x81,0x01,0x81,0x02,0x81,0x03,0x81,0x03,0x83,0x03,0x84,0x04,0x84,0x03,0x81,0x06,0x83,0x05,0x83,0x03,0x84,0x04,0x81,0x05,0x81,0x01,0x81,0x03,0x81,0x03,0x81,0x04,0x81,0x04,0x85,0x04,0x81,0x06,0x81,0x06,0x81,0x2c,0x81,0x1c,0x82,0x03,0x82,0x13,0x81,0x13,0x81,0x54,0x81,0x22,0x81,0x79,0x81,0x43,0x82,0x08,0x81,0x02,0x82,0x47,0x81,0x13,0x81,0x26,0x81,0x0a,0x81,0x35,0x81,0x0d,0x83,0x04,0x81,0x04,0x83,0x2c,0x81,0x7f,0x44,0x83,0x76,0x81,0x7f,0x17,0x81,0x02,0x81,0x13,0x81,0x26,0x81,0x0a,0x81,0x34,0x81,0x15,0x81,0x7f,0x7f,0x7f,0x50,0x87,0x34,0x82,0x12,0x82,0x27,0x81,0x0a,0x81,0x33,0x82,0x7f,0x7f,0x7f,0x7f,0x7f,0x4b,0x00,0x49,0x48,0x44,0x52,0x00,0x00,0x00,0x00,0x49,0x44,0x41,0x54,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0x00,0x00,0x00,0x00,0x50,0x4c,0x54,0x45,0x00,0x00,0x00,0x00,0x62,0x4b,0x47,0x44,0x00,0x00,0x00,0x00,0x63,0x48,0x52,0x4d,0x00,0x00,0x00,0x00,0x67,0x41,0x4d,0x41,0x00,0x00,0x00,0x00,0x68,0x49,0x53,0x54,0x00,0x00,0x00,0x00,0x69,0x43,0x43,0x50,0x00,0x00,0x00,0x00,0x69,0x54,0x58,0x74,0x00,0x00,0x00,0x00,0x6f,0x46,0x46,0x73,0x00,0x00,0x00,0x00,0x70,0x43,0x41,0x4c,0x00,0x00,0x00,0x00,0x73,0x43,0x41,0x4c,0x00,0x00,0x00,0x00,0x70,0x48,0x59,0x73,0x00,0x00,0x00,0x00,0x73,0x42,0x49,0x54,0x00,0x00,0x00,0x00,0x73,0x50,0x4c,0x54,0x00,0x00,0x00,0x00,0x73,0x52,0x47,0x42,0x00,0x00,0x00,0x00,0x74,0x45,0x58,0x74,0x00,0x00,0x00,0x00,0x74,0x49,0x4d,0x45,0x00,0x00,0x00,0x00,0x74,0x52,0x4e,0x53,0x00,0x00,0x00,0x00,0x7a,0x54,0x58,0x74,0x00,
  }
};
#endif