#include "MyForm.h"
#include <opencv2\core\core.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>

void cdiv(double a, double b, double c, double d, double *rr, double *ri)
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

double norm(double a, double b)
{
	return  ((a*a + b*b));
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
			s.val[0] = (s.val[0] - m) / (M - m) * 255;
			cvSet2D(savepart, i, j, s);
		}
	// Save image
	cvSaveImage(name, savepart);
	cvReleaseImage(&realpart);
	cvReleaseImage(&savepart);
}

float norm(float a, float b)
{
	return (float)sqrt((double)(a*a + b*b));
}

void extract(IplImage *dest, IplImage *source)
{

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
	//commented in order to print all the images
	/*cvWaitKey(0);
	cvReleaseImage(&realpart);
	cvDestroyWindow(title);*/
}

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

IplImage* fftImage(IplImage *img)
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

IplImage *fftImageInv(IplImage *img)
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

int vlog2(int x)
{
	int k, j;

	j = 0; k = 1;
	while (k<x) {
		k *= 2;
		j += 1;
	}
	return j;
}

#include <stdio.h>
#include <stdarg.h>

/*Function///////////////////////////////////////////////////////////////

Name:       cvShowManyImages

Purpose:    This is a function illustrating how to display more than one
image in a single window using Intel OpenCV

Parameters: char *title: Title of the window to be displayed
int nArgs:   Number of images to be displayed
...:         IplImage*, which contains the images

18Language:   C++

The method used is to set the ROIs of a Single Big image and then resizing
and copying the input images on to the Single Big Image.

This function does not stretch the image...
It resizes the image without modifying the width/height ratio..

This function can be called like this:

cvShowManyImages("Images", 2, img1, img2);
or
cvShowManyImages("Images", 5, img2, img2, img3, img4, img5);

This function can display upto 12 images in a single window.
It does not check whether the arguments are of type IplImage* or not.
The maximum window size is 700 by 660 pixels.
Does not display anything if the number of arguments is less than
one or greater than 12.

If you pass a pointer that is not IplImage*, Error will occur.
Take care of the number of arguments you pass, and the type of arguments,
which should be of type IplImage* ONLY.

Idea was from [[BettySanchi]] of OpenCV Yahoo! Groups.

If you have trouble compiling and/or executing
this code, I would like to hear about it.
You could try posting on the OpenCV Yahoo! Groups
[url]http://groups.yahoo.com/group/OpenCV/messages/ [/url]

Parameswaran,
Chennai, India.

cegparamesh[at]gmail[dot]com
...
///////////////////////////////////////////////////////////////////////*/

void cvShowManyImages(char* title, int nArgs, ...) {

	// img - Used for getting the arguments 
	IplImage *img;

	// [[DispImage]] - the image in which input images are to be copied
	IplImage *DispImage;

	int size;
	int i;
	int m, n;
	int x, y;

	// w - Maximum number of images in a row 
	// h - Maximum number of images in a column 
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying 
	if (nArgs <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if (nArgs > 12) {
		printf("Number of arguments too large....\n");
		return;
	}
	// Determine the size of the image, 
	// and the number of rows/cols 
	// from number of arguments 
	else if (nArgs == 1) {
		w = h = 1;
		size = 300;
	}
	else if (nArgs == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (nArgs == 3 || nArgs == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (nArgs == 5 || nArgs == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (nArgs == 7 || nArgs == 8) {
		w = 4; h = 2;
		size = 200;
	}
	else {
		w = 4; h = 3;
		size = 150;
	}

	// Create a new 3 channel image
	DispImage = cvCreateImage(cvSize(100 + size*w, 60 + size*h), 8, 3);

	// Used to get the arguments passed
	va_list args;
	va_start(args, nArgs);

	// Loop for nArgs number of arguments
	for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {

		// Get the Pointer to the IplImage
		img = va_arg(args, IplImage*);

		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		if (img == 0) {
			printf("Invalid arguments");
			cvReleaseImage(&DispImage);
			return;
		}

		// Find the width and height of the image
		x = img->width;
		y = img->height;
		// Find whether height or width is greater in order to resize the image
		max = (x > y) ? x : y;

		// Find the scaling factor to resize the image
		scale = (float)((float)max / size);

		// Used to Align the images
		if (i % w == 0 && m != 20) {
			m = 20;
			n += 20 + size;
		}

		// Set the image ROI to display the current image
		cvSetImageROI(DispImage, cvRect(m, n, (int)(x / scale), (int)(y / scale)));

		// Resize the input image and copy the it to the Single Big Image
		cvResize(img, DispImage);

		// Reset the ROI in order to display the next image
		cvResetImageROI(DispImage);
	}

	// Create a new window, and show the Single Big Image
	cvNamedWindow(title, 1);
	cvShowImage(title, DispImage);

	cvWaitKey(0);
	cvDestroyWindow(title);

	// End the number of arguments
	va_end(args);

	// Release the Image Memory
	cvReleaseImage(&DispImage);
}