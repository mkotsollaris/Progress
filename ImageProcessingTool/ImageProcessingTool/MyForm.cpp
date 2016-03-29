/*
	Author: Menelaos Kotsollaris
	Email: mkotsollari@gmail.com

	All rights reserved 2016.
	Image Processing & Computer Vision
	Dr. Yun Zhang, UNB - GGE
*/
#include "MyForm.h"
#include <opencv2\core\core.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "Utilities.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <windows.h>
#include <stdio.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ImageProcessingTool::MyForm form;
	Application::Run(%form);
}
void ImageProcessingTool::MyForm::UnBlur(const char* _initial_image, const char* _psf_image_path,
	const char* _output_path)
{
	IplImage* img = NULL;
	IplImage *psf, *psfa, *fftpsf;
	IplImage *grayImg, *z, *zz, *ft, *result, *resulti;
	int i, j;
	CvScalar s, s1;
	double p, q, rad = 10.0, rr, ri;
	/* Read in the image to be processed */
	img = cvLoadImage(_initial_image, CV_LOAD_IMAGE_UNCHANGED);
	if (img == 0)
	{
		printf("Input file not open.\n");
		return;
	}
	/* Convert the (possibly) color image into grey */
	grayImg = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);
	grayImg->origin = img->origin;
	if (img->nChannels > 1)
		cvCvtColor(img, grayImg, CV_BGR2GRAY);
	else
		grayImg = img;
	cvNamedWindow("Original", 0);
	cvShowImage("Original", grayImg);
	cvWaitKey(0);
	// Computer the Fourier transform
	z = fftImageUnBlur(grayImg);
	displayFDimage(z, "Transform");
	// Read the PSF image
	psf = cvLoadImage(_psf_image_path, CV_LOAD_IMAGE_UNCHANGED);
	// Make PSF the same size, zero fill.
	psfa = alignUnBlur(psf, img->height, img->width);
	 // Fourier transform of the PSF
	zz = fftImageUnBlur(psfa);
	fftpsf = fftImageUnBlur(psfa);
	displayFDimage(zz, "FFT of PSF");
	// Now do the work - multiply corresponding elements
	for (i = 0; i<z->height; i++)
	{
		for (j = 0; j<z->width; j++)
		{
			s = cvGet2D(z, i, j);
			s1 = cvGet2D(zz, i, j);
			p = s.val[0] * s1.val[0] - s.val[1] * s1.val[1];
			q = s.val[0] * s1.val[1] + s.val[1] * s1.val[0];
			s.val[0] = p; s.val[1] = q;
			/*printf("%d",p);
			printf(",");
			printf("%d", q);
			printf(" ");*/
			cvSet2D(z, i, j, s);
		}
	}
	cvReleaseImage(&psf);
	cvReleaseImage(&psfa);
	// Inverse transform
	zz = fftImageInvUnBlur(z);
	result = fftImageInvUnBlur(z);
	displayFDimage(zz, "Result");
	char _output_file[120];
	strcpy(_output_file, _output_path);
	strcat(_output_file, "\\unblurred_image.jpg");
	puts(_output_file);
	saveImage(_output_file, zz);
	//saveImage(_output_file, result);
	printf("Radius is now %lf /n.", rad);
	//the counter of the iterations
	int counter = 0;
	do
	{
	// Now do the work - multiply corresponding elements
	for (i = 0; i<z->height; i++)
	{
		for (j = 0; j<z->width; j++)
		{
			s = cvGet2D(z, i, j);
			s1 = cvGet2D(fftpsf, i, j);
			if (norm(s1.val[0], s1.val[1]) >= rad)
			{
				cdiv(s.val[0], s.val[1], s1.val[0], s1.val[1], &rr, &ri);
				s1.val[0] = rr; s1.val[1] = ri;
			}
			else
			{
				s1.val[0] = 0.0; s1.val[1] = 0.0;
			}
			cvSet2D(result, i, j, s1);
		}
	}
	//	At this point, fftpsf is the fourier transform of the psf.
	// Inverse transform
	origin_centerUnBlur(result);
	resulti = fftImageInvUnBlur(result);
	displayFDimage(resulti, "Inverse Filter");
	saveImage(_output_file, resulti);
	cvReleaseImage(&resulti);
	rad /= 2.0;
	counter++;
	} while (rad > 0 && counter<15);
	return;
}
void ImageProcessingTool::MyForm::NotchFilter
(const char * _initial_image, const char * _psf_image, const char * _output_path)
{
	IplImage* img = NULL;
	IplImage *psf, *psfa;
	IplImage *grayImg, *z, *zz;
	int i, j;
	CvScalar s, s1;
	double p, q;

	/* Read in the image to be processed */
	img = cvLoadImage(_initial_image, CV_LOAD_IMAGE_UNCHANGED);
	if (img == 0)
	{
		printf("Input file not open.\n");
		return;
	}
	/* Convert the (possibly) color image into grey */
	grayImg = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);
	grayImg->origin = img->origin;
	if (img->nChannels > 1)
		cvCvtColor(img, grayImg, CV_BGR2GRAY);
	else
		grayImg = img;
	cvNamedWindow("Original", 0);
	cvShowImage("Original", grayImg);
	cvWaitKey(0);
	// Computer the Fourier transform
	z = fftImage(grayImg);
	editFDimage(z);
	// Inverse transform
	zz = fftImageInv(z);
	displayFDimage(zz, "Result");
	char _output_file[120];
	strcpy(_output_file, _output_path);
	strcat(_output_file, "\\notch_filter_image.jpg ");
	puts(_output_file);
	saveImage(_output_file, zz);
	return;
}