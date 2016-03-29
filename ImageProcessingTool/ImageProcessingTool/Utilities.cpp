#include <stdio.h>
#include <math.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace System;
using namespace System::Windows::Forms;
void cdiv(double a, double b, double c, double d, double *rr, double *ri)
{
	/* res = c1/c2 ... Complex */
	double z, real, imag;
	z = (c*c + d*d);
	if (z != 0.0)
	{
		real = (a*c + b*d) / z;
		imag = (b*c - a*d) / z;
		*rr = real;
		*ri = imag;
		return;
	}
	*rr = 0.0;     *ri = 0.0;
}
IplImage *editimage;
int ulx, uly, lrx, lry;
float norm(float a, float b)
{
	return (float)sqrt((double)(a*a + b*b));
}
void on_mouse(int event, int x, int y, int flags, void* param)
{
	static int ux = -1, uy = -1;
	int i, j;
	CvScalar s;

	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		ux = x; uy = y;
		printf(" Button down.\n");
		break;

	case CV_EVENT_LBUTTONUP:
		if (ux < 0) return;
		s.val[0] = 0.0; s.val[1] = 0.0;
		printf(" Button up.\n");
		printf("Fill rows %d to %d and columns %d to %d\n", uly, y, ulx, x);
		for (i = uy; i<y; i++)
			for (j = ux; j<x; j++)
				cvSet2D(editimage, i, j, s);
		ulx = ux; uly = uy;
		ux = uy = -1;
		lrx = x; lry = y;
		break;
	}
}
/* Display a frequency domain image - needs to be made real and grey scaled */
void displayFD(CvMat *x, char *title)
{
	IplImage *realpart, *imgpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->rows, x->cols), IPL_DEPTH_64F, 1);
	imgpart = cvCreateImage(cvSize(x->rows, x->cols), IPL_DEPTH_64F, 1);
	cvSplit(x, realpart, imgpart, 0, 0);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	for (i = 0; i<realpart->height; i++)
		for (j = 0; j<realpart->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			z = s.val[0];
			s = cvGet2D(imgpart, i, j);
			y = s.val[0];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}

	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);

	// scale
	for (i = 0; i<realpart->height; i++)
		for (j = 0; j<realpart->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			cvSet2D(realpart, i, j, s);
		}

	// display image in window
	cvNamedWindow(title, 0);
	cvShowImage(title, realpart);
	cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvReleaseImage(&imgpart);
	cvDestroyWindow(title);
}
IplImage *align(IplImage *psf, int height, int width)
{
	int i, j, is, js;
	IplImage *a;
	CvScalar s;

	a = cvCreateImage(cvSize(height, width), IPL_DEPTH_64F, 1);
	is = height - psf->height;
	js = width - psf->width;
	if ((is < 0) || (js < 0))
	{
		printf("PSF is bigger than the image.\n");
		exit(0);
	}

	is = is / 2; js = js / 2;
	cvSetZero(a);
	for (i = 0; i<psf->height; i++)
		for (j = 0; j<psf->width; j++)
		{
			s = cvGet2D(psf, i, j);
			cvSet2D(a, i + is, j + js, s);
		}
	return a;
}
IplImage * cvMatToImage(CvMat *x)
{
	IplImage *a;
	double *dp;
	int i, j;
	CvSize s;
	CvScalar S;

	dp = x->data.db;
	s.height = x->rows;
	s.width = x->cols;
	a = cvCreateImage(s, IPL_DEPTH_64F, 2);

	for (i = 0; i<s.height; i++)
		for (j = 0; j<s.width; j++)
		{
			S.val[0] = *dp++;
			S.val[1] = *dp++;
			cvSet2D(a, i, j, S);
		}
	//cvSplit( dft_A, image_Re, image_Im, 0, 0 );
	return a;
}
/* Display a frequency domain image - needs to be made real and grey scaled */
void displayFDimage(IplImage *x, char *title)
{
	IplImage *realpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->height, x->width), IPL_DEPTH_64F, 1);

	// Compute the magnitude of the transform  = log((norm) + 1)
	for (i = 0; i<x->height; i++)
	{
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(x, i, j);
			z = s.val[0];
			y = s.val[1];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}
	}
	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);
	// scale
	for (i = 0; i < x->height; i++)
	{
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			//printf("%d",s.val[0]);
			//printf(" ");
			cvSet2D(realpart, i, j, s);
		}
		//printf("\n");
	}

	// display image in window
	cvNamedWindow(title, 0);
	cvShowImage(title, realpart);
	cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvDestroyWindow(title);
}
/* Display a frequency domain image - needs to be made real and grey scaled */
void editFDimage(IplImage *x)
{
	IplImage *realpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;
	char c;
	editimage = x;
	realpart = cvCreateImage(cvSize(x->height, x->width), IPL_DEPTH_64F, 1);
	cvNamedWindow("Edit", 0);

	// Compute the magnitude of the transform  = log((norm) + 1)
	for (i = 0; i<x->height; i++)
	{
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(x, i, j);
			z = s.val[0];
			y = s.val[1];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}
	}
	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);
	// scale
	for (i = 0; i<x->height; i++)
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			cvSet2D(realpart, i, j, s);
		}
	cvSetMouseCallback("Edit", on_mouse, 0);
	s.val[0] = 0.0; s.val[1] = 0.0;
	for (;;)
	{
		cvShowImage("Edit", realpart);
		c = cvWaitKey(0);
		if (lrx <= 0) continue;

		for (i = uly; i<lry; i++)
			for (j = ulx; j<lrx; j++)
				cvSet2D(realpart, i, j, s);
		ulx = uly = lrx = lry = 0;
		if (c == 'x') break;
	}
	cvReleaseImage(&realpart);
	cvDestroyWindow("Edit");
}
/* Moves the origin to the image center */
void origin_center(IplImage *array)
{
	int  i, j;
	CvScalar s;

	for (i = 0; i < array->height; i++)
		for (j = 0; j < array->width; j++)
			if ((i + j) % 2)
			{
				s = cvGet2D(array, i, j);
				s.val[0] = -s.val[0];
				s.val[1] = -s.val[1];
				cvSet2D(array, i, j, s);
			}

}
/* Computes the FFT of the given Image and returns a pointer to that image */
IplImage *fftImage(IplImage *img)
{
	IplImage *realpart, *imgpart, *complexpart, *ret;
	CvMat *ft;
	int sizeM, sizeN;
	CvMat tmp;

	realpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 1);
	imgpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 1);
	complexpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 2);
	cvScale(img, realpart, 1.0, 0.0);// copy grey input image to realpart
	cvZero(imgpart);// Set imaginary part to 0
	cvMerge(realpart, imgpart, NULL, NULL, complexpart); //real+imag to complex

	/* Messy bit - fft needs sizes to be a power of 2, so the images have to be
	// embedded into a background of 0 pixels. */
	sizeM = cvGetOptimalDFTSize(img->height - 1);
	sizeN = cvGetOptimalDFTSize(img->width - 1);
	printf("Size of image to be transformed is %dx%d\n", sizeM, sizeN);
	ft = cvCreateMat(sizeM, sizeN, CV_64FC2);
	origin_center(complexpart);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(ft, &tmp, cvRect(0, 0, img->width, img->height));// tmp points to sub of dft_A
	cvCopy(complexpart, &tmp, NULL);// Copy complex image into sub of dft_A
	cvGetSubRect(ft, &tmp, cvRect(img->width, 0,
		ft->cols - img->width, img->height));// Get sub of dft_A on right side
	if ((ft->cols - img->width) > 0) cvZero(&tmp);// Set right margin to zero


	cvDFT(ft, ft, CV_DXT_FORWARD, complexpart->height);
	ret = cvMatToImage(ft);
	cvReleaseMat(&ft);
	cvReleaseImage(&realpart);
	cvReleaseImage(&imgpart);
	cvReleaseImage(&complexpart);
	return ret;
}
IplImage *fftImageInv(IplImage *img)
{
	IplImage *ret;
	CvMat *ft;
	CvMat tmp;

	ft = cvCreateMat(img->height, img->width, CV_64FC2);
	//    origin_center (img);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(ft, &tmp, cvRect(0, 0, img->width, img->height));  // tmp points to sub of dft_A
	cvCopy(img, &tmp, NULL);                                       // Copy complex image into sub of dft_A
	cvDFT(ft, ft, (CV_DXT_SCALE | CV_DXT_INVERSE), img->height);

	ret = cvMatToImage(ft);
	cvReleaseMat(&ft);
	printf("INVFFT Return image is: Depth=%d channels=%d\n", ret->depth, ret->nChannels);
	return ret;
}
void saveImage(char *name, IplImage *x)
{
	IplImage *realpart, *savepart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->height, x->width), IPL_DEPTH_64F, 1);
	savepart = cvCreateImage(cvSize(x->height, x->width), IPL_DEPTH_8U, 1);
	std::string tempString;
	// Compute the magnitude of the transform  = log((norm) + 1)
	for (i = 0; i<x->height; i++)
	{
		tempString = "";
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(x, i, j);
			z = s.val[0];
			y = s.val[1];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			//printf("%d", log(z));
			//printf(" ");
			cvSet2D(realpart, i, j, s);
		}
		//printf("\n");
	}
	printf("\n ------- NEW ------- \n");

	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);

	// scale
	for (i = 0; i<x->height; i++)
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m) * 255;
			cvSet2D(savepart, i, j, s);
		}

	// Save image
	cvSaveImage(name, savepart);
	cvReleaseImage(&realpart);
	cvReleaseImage(&savepart);
}
int vlog2(int x)
{
	int k, j;

	j = 0; k = 1;
	while (k<x)
	{
		k *= 2;
		j += 1;
	}
	return j;
}
/* Display a frequency domain image - needs to be made real and grey scaled */
void displayFDBlur(CvMat *x, char *title)
{
	IplImage *realpart, *imgpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->rows, x->cols), IPL_DEPTH_64F, 1);
	imgpart = cvCreateImage(cvSize(x->rows, x->cols), IPL_DEPTH_64F, 1);
	cvSplit(x, realpart, imgpart, 0, 0);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	for (i = 0; i<realpart->height; i++)
		for (j = 0; j<realpart->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			z = s.val[0];
			s = cvGet2D(imgpart, i, j);
			y = s.val[0];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}

	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);

	// scale
	for (i = 0; i<realpart->height; i++)
		for (j = 0; j<realpart->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			cvSet2D(realpart, i, j, s);
		}

	// display image in window
	cvNamedWindow(title, 0);
	cvShowImage(title, realpart);
	cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvReleaseImage(&imgpart);
	cvDestroyWindow(title);
}
IplImage *alignBlur(IplImage *psf, int height, int width)
{
	int i, j, is, js;
	IplImage *a;
	CvScalar s;

	a = cvCreateImage(cvSize(height, width), IPL_DEPTH_64F, 1);
	is = height - psf->height;
	js = width - psf->width;
	if ((is < 0) || (js < 0))
	{
		printf("PSF is bigger than the image.\n");
		exit(0);
	}

	is = is / 2; js = js / 2;
	cvSetZero(a);
	for (i = 0; i<psf->height; i++)
		for (j = 0; j<psf->width; j++)
		{
			s = cvGet2D(psf, i, j);
			cvSet2D(a, i + is, j + js, s);
		}
	return a;
}
IplImage * cvMatToImageBlur(CvMat *x)
{
	IplImage *a;
	double *dp;
	int i, j;
	CvSize s;
	CvScalar S;

	dp = x->data.db;
	s.height = x->rows;
	s.width = x->cols;
	a = cvCreateImage(s, IPL_DEPTH_64F, 2);

	for (i = 0; i<s.height; i++)
		for (j = 0; j<s.width; j++)
		{
			S.val[0] = *dp++;
			S.val[1] = *dp++;
			cvSet2D(a, i, j, S);
		}
	//cvSplit( dft_A, image_Re, image_Im, 0, 0 );
	return a;
}
/* Display a frequency domain image - needs to be made real and grey scaled */
void displayFDimageBlur(IplImage *x, char *title)
{
	IplImage *realpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->height, x->width), IPL_DEPTH_64F, 1);

	// Compute the magnitude of the transform  = log((norm) + 1)
	for (i = 0; i<x->height; i++)
	{
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(x, i, j);
			z = s.val[0];
			y = s.val[1];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}
	}

	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);

	// scale
	for (i = 0; i<x->height; i++)
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			cvSet2D(realpart, i, j, s);
		}

	// display image in window
	cvNamedWindow(title, 0);
	cvShowImage(title, realpart);
	cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvDestroyWindow(title);
}
void origin_centerBlur(IplImage *array)
{
	int  i, j;
	CvScalar s;

	for (i = 0; i < array->height; i++)
		for (j = 0; j < array->width; j++)
			if ((i + j) % 2)
			{
				s = cvGet2D(array, i, j);
				s.val[0] = -s.val[0];
				s.val[1] = -s.val[1];
				cvSet2D(array, i, j, s);
			}

}
IplImage* fftImageBlur(IplImage *img)
{
	IplImage *realpart, *imgpart, *complexpart, *ret;
	CvMat *ft;
	int sizeM, sizeN;
	CvMat tmp;

	realpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 1);
	imgpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 1);
	complexpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 2);
	cvScale(img, realpart, 1.0, 0.0);              // copy grey input image to realpart
	cvZero(imgpart);                                   // Set imaginary part to 0
	cvMerge(realpart, imgpart, NULL, NULL, complexpart); // real+imag to complex

														 /* Messy bit - fft needs sizes to be a power of 2, so the images have to be
														 // embedded into a background of 0 pixels. */
	sizeM = cvGetOptimalDFTSize(img->height - 1);
	sizeN = cvGetOptimalDFTSize(img->width - 1);
	printf("Size of image to be transformed is %dx%d\n", sizeM, sizeN);

	ft = cvCreateMat(sizeM, sizeN, CV_64FC2);
	origin_center(complexpart);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(ft, &tmp, cvRect(0, 0, img->width, img->height));  // tmp points to sub of dft_A
	cvCopy(complexpart, &tmp, NULL);                                       // Copy complex image into sub of dft_A
	cvGetSubRect(ft, &tmp, cvRect(img->width, 0,
		ft->cols - img->width, img->height));   // Get sub of dft_A on right side
	if ((ft->cols - img->width) > 0) cvZero(&tmp);                   // Set right margin to zero


	cvDFT(ft, ft, CV_DXT_FORWARD, complexpart->height);

	ret = cvMatToImage(ft);
	cvReleaseMat(&ft);
	cvReleaseImage(&realpart);
	cvReleaseImage(&imgpart);
	cvReleaseImage(&complexpart);
	return ret;
}
IplImage *fftImageInvBlur(IplImage *img)
{
	IplImage *ret;
	CvMat *ft;
	CvMat tmp;

	ft = cvCreateMat(img->height, img->width, CV_64FC2);
	origin_center(img);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(ft, &tmp, cvRect(0, 0, img->width, img->height));  // tmp points to sub of dft_A
	cvCopy(img, &tmp, NULL);                                       // Copy complex image into sub of dft_A
	cvDFT(ft, ft, (CV_DXT_SCALE | CV_DXT_INVERSE), img->height);

	ret = cvMatToImage(ft);
	cvReleaseMat(&ft);
	printf("INVFFT Return image is: Depth=%d channels=%d\n", ret->depth, ret->nChannels);
	return ret;
}
/*Inverse Functions*/
void cdivUnBlur(double a, double b, double c, double d, double *rr, double *ri)
{                               /* res = c1/c2 ... Complex */
	double z, real, imag;

	z = (c*c + d*d);
	if (z != 0.0)
	{
		real = (a*c + b*d) / z;
		imag = (b*c - a*d) / z;
		*rr = real;
		*ri = imag;
		return;
	}
	*rr = 0.0;     *ri = 0.0;
}
/* Display a frequency domain image - needs to be made real and grey scaled */
void displayFDUnBlur(CvMat *x, char *title)
{
	IplImage *realpart, *imgpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->rows, x->cols), IPL_DEPTH_64F, 1);
	imgpart = cvCreateImage(cvSize(x->rows, x->cols), IPL_DEPTH_64F, 1);
	cvSplit(x, realpart, imgpart, 0, 0);

	// Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
	for (i = 0; i<realpart->height; i++)
		for (j = 0; j<realpart->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			z = s.val[0];
			s = cvGet2D(imgpart, i, j);
			y = s.val[0];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}

	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);

	// scale
	for (i = 0; i<realpart->height; i++)
		for (j = 0; j<realpart->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			cvSet2D(realpart, i, j, s);
		}

	// display image in window
	cvNamedWindow(title, 0);
	cvShowImage(title, realpart);
	cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvReleaseImage(&imgpart);
	cvDestroyWindow(title);
}
IplImage *alignUnBlur(IplImage *psf, int height, int width)
{
	int i, j, is, js;
	IplImage *a;
	CvScalar s;

	a = cvCreateImage(cvSize(height, width), IPL_DEPTH_64F, 1);
	is = height - psf->height;
	js = width - psf->width;
	if ((is < 0) || (js < 0))
	{
		printf("PSF is bigger than the image.\n");
		exit(0);
	}

	is = is / 2; js = js / 2;
	cvSetZero(a);
	for (i = 0; i<psf->height; i++)
		for (j = 0; j<psf->width; j++)
		{
			s = cvGet2D(psf, i, j);
			cvSet2D(a, i + is, j + js, s);
		}
	return a;
}
IplImage *cvMatToImageUnBlur(CvMat *x)
{
	IplImage *a;
	double *dp;
	int i, j;
	CvSize s;
	CvScalar S;

	dp = x->data.db;
	s.height = x->rows;
	s.width = x->cols;
	a = cvCreateImage(s, IPL_DEPTH_64F, 2);

	for (i = 0; i<s.height; i++)
		for (j = 0; j<s.width; j++)
		{
			S.val[0] = *dp++;
			S.val[1] = *dp++;
			cvSet2D(a, i, j, S);
		}
	//cvSplit( dft_A, image_Re, image_Im, 0, 0 );
	return a;
}
void displayFDimageUnBlur(IplImage *x, char *title)
{
	IplImage *realpart;
	CvScalar s;
	int i, j;
	double z, y, m, M;

	// Split Fourier in real and imaginary parts
	realpart = cvCreateImage(cvSize(x->height, x->width), IPL_DEPTH_64F, 1);

	// Compute the magnitude of the transform  = log((norm) + 1)
	for (i = 0; i<x->height; i++)
	{
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(x, i, j);
			z = s.val[0];
			y = s.val[1];
			z = sqrt(z*z + y*y) + 1.0f;
			s.val[0] = log(z);
			cvSet2D(realpart, i, j, s);
		}
	}

	// scale image for display
	cvMinMaxLoc(realpart, &m, &M, NULL, NULL, NULL);
	printf("Minimum value is %lf  max value is %lf\n", m, M);

	// scale
	for (i = 0; i<x->height; i++)
		for (j = 0; j<x->width; j++)
		{
			s = cvGet2D(realpart, i, j);
			s.val[0] = (s.val[0] - m) / (M - m);
			cvSet2D(realpart, i, j, s);
		}

	// display image in window
	cvNamedWindow(title, 0);
	cvShowImage(title, realpart);
	cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvDestroyWindow(title);
}
void origin_centerUnBlur(IplImage *array)
{
	int  i, j;
	CvScalar s;

	for (i = 0; i < array->height; i++)
		for (j = 0; j < array->width; j++)
			if ((i + j) % 2)
			{
				s = cvGet2D(array, i, j);
				s.val[0] = -s.val[0];
				s.val[1] = -s.val[1];
				cvSet2D(array, i, j, s);
			}

}
int NORMALIZE = 0;
IplImage *fftImageUnBlur(IplImage *img)
{
	IplImage *realpart, *imgpart, *complexpart, *ret;
	CvMat *ft;
	int sizeM, sizeN;
	CvMat tmp;
	double xmax = 0.0;
	CvScalar s;
	int i, j;

	realpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 1);
	imgpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 1);
	complexpart = cvCreateImage(cvGetSize(img), IPL_DEPTH_64F, 2);
	cvScale(img, realpart, 1.0, 0.0); // copy grey input image to realpart
	if (NORMALIZE)
	{
		for (i = 0; i<img->height; i++)
			for (j = 0; j<img->width; j++)
			{
				s = cvGet2D(realpart, i, j);
				if (s.val[0] > xmax) xmax = s.val[0];
			}
		for (i = 0; i<img->height; i++)
			for (j = 0; j<img->width; j++)
			{
				s = cvGet2D(realpart, i, j);
				s.val[0] /= xmax;
				cvSet2D(realpart, i, j, s);
			}
	}
	cvZero(imgpart);                                   // Set imaginary part to 0
	cvMerge(realpart, imgpart, NULL, NULL, complexpart); // real+imag to complex

														 /* Messy bit - fft needs sizes to be a power of 2, so the images have to be
														 // embedded into a background of 0 pixels. */
	sizeM = cvGetOptimalDFTSize(img->height - 1);
	sizeN = cvGetOptimalDFTSize(img->width - 1);
	printf("Size of image to be transformed is %dx%d\n", sizeM, sizeN);

	ft = cvCreateMat(sizeM, sizeN, CV_64FC2);
	origin_center(complexpart);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(ft, &tmp, cvRect(0, 0, img->width, img->height));  // tmp points to sub of dft_A
	cvCopy(complexpart, &tmp, NULL); // Copy complex image into sub of dft_A
	cvGetSubRect(ft, &tmp, cvRect(img->width, 0,
		ft->cols - img->width, img->height));   // Get sub of dft_A on right side
	if ((ft->cols - img->width) > 0) cvZero(&tmp);                   // Set right margin to zero


	cvDFT(ft, ft, CV_DXT_FORWARD, complexpart->height);

	ret = cvMatToImage(ft);
	cvReleaseMat(&ft);
	cvReleaseImage(&realpart);
	cvReleaseImage(&imgpart);
	cvReleaseImage(&complexpart);
	return ret;
}
IplImage *fftImageInvUnBlur(IplImage *img)
{
	IplImage *ret;
	CvMat *ft;
	CvMat tmp;

	ft = cvCreateMat(img->height, img->width, CV_64FC2);
	origin_center(img);

	// copy A to dft_A and pad dft_A with zeros
	cvGetSubRect(ft, &tmp, cvRect(0, 0, img->width, img->height));  // tmp points to sub of dft_A
	cvCopy(img, &tmp, NULL);                                       // Copy complex image into sub of dft_A
	cvDFT(ft, ft, (/*CV_DXT_SCALE|*/CV_DXT_INVERSE), img->height);

	ret = cvMatToImage(ft);
	cvReleaseMat(&ft);
	printf("INVFFT Return image is: Depth=%d channels=%d\n", ret->depth, ret->nChannels);
	origin_center(img);
	return ret;
}