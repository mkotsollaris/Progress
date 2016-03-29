/*
Author: Menelaos Kotsollaris
Email: mkotsollari@gmail.com

All rights reserved 2016.
Image Processing & Computer Vision
Dr. Yun Zhang, UNB - GGE
*/
#include <stdio.h>
#include <math.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <stdlib.h>

/*Methods used for Notch Filter*/
void cdiv(double a, double b, double c, double d, double *rr, double *ri);
float norm(float a, float b);
void editFDimage(IplImage* x);
void on_mouse(int event, int x, int y, int flags, void* param);
void origin_center(IplImage *array);
void displayFD(CvMat *x, char *name);
void displayFDimage(IplImage *x, char *name);
IplImage *cvMatToImage(CvMat *x);
IplImage *fftImage(IplImage *img);
IplImage *align(IplImage *psf, int height, int width);
IplImage *fftImageInv(IplImage *img);
void saveImage(char *name, IplImage *x);
void cdiv(double a, double b, double c, double d, double *rr, double *ri);

/*Methods used for blurring*/
void displayFDimageBlur(IplImage *x, char *title);
IplImage * cvMatToImageBlur(CvMat *x);
void displayFDBlur(CvMat *x, char *title);
IplImage *alignBlur(IplImage *psf, int height, int width);
void origin_centerBlur(IplImage *array);
IplImage *fftImageBlur(IplImage *img);
IplImage *fftImageInvBlur(IplImage *img);

/*Methods used for unblurring*/
void cdivUnBlur(double a, double b, double c, double d, double *rr, double *ri);
void displayFDUnBlur(CvMat *x, char *title);
IplImage *alignUnBlur(IplImage *psf, int height, int width);
IplImage *cvMatToImageUnBlur(CvMat *x);
void displayFDimageUnBlur(IplImage *x, char *title);
void origin_centerUnBlur(IplImage *array);
IplImage *fftImageUnBlur(IplImage *img);
IplImage *fftImageInvUnBlur(IplImage *img);