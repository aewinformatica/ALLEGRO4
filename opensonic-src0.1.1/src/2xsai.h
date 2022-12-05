/*
 * 2xSaI
 *
 * File: 2xsai.h
 * Desc: 2xSaI module. This file was obtained from
 *       http://bob.allegronetwork.com/projects.html
 * Authors: Derek Liauw Kie Fa/Kreed, Robert J Ohannessian, Peter Wang
 */

#ifndef _2XSAI_H
#define _2XSAI_H

#define uint32 unsigned long
#define uint16 unsigned short
#define uint8 unsigned char

int Init_2xSaI(int depth);
void Super2xSaI(BITMAP * src, BITMAP * dest, int s_x, int s_y, int d_x, int d_y, int w, int h);
void Super2xSaI_ex(uint8 *src, uint32 src_pitch, uint8 *unused, BITMAP *dest, uint32 width, uint32 height);

void SuperEagle(BITMAP * src, BITMAP * dest, int s_x, int s_y, int d_x, int d_y, int w, int h);
void SuperEagle_ex(uint8 *src, uint32 src_pitch, uint8 *unused, BITMAP *dest, uint32 width, uint32 height);

#endif
