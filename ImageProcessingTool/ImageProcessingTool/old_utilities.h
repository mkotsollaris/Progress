#pragma once
#include "MyForm.h"
#include <opencv2\core\core.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>

void cdiv(double a, double b, double c, double d, double *rr, double *ri);
double norm(double a, double b);
void saveImage(char *name, IplImage *x);
void cvShowManyImages(char* title, int nArgs, ...);
void origin_center(IplImage *array);
void displayFD(CvMat *x, char *name);
void displayFDimage(IplImage *x, char *name);
IplImage * cvMatToImage(CvMat *x);
IplImage *fftImage(IplImage *img);
IplImage *align(IplImage *psf, int height, int width);
IplImage *fftImageInv(IplImage *img);