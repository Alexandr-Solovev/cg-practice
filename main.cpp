#include <QtCore/QCoreApplication>
#include <QImage>
#include <string>
#include <iostream>
#include "Filters.h"

using namespace std;

int main(int argc, char *argv[])
{
	string s;
	QImage photo;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-p") && (i + 1 < argc))
			s = argv[i + 1];
	}
	int g[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	photo.load(QString(s.c_str()));
	Invert_filter* invert = new Invert_filter();
	Blur_filter* blur = new Blur_filter(3);
	Gaussian_blur_filter* gaussian_blur = new Gaussian_blur_filter();
	Gray_scale_filter* gray_scale = new Gray_scale_filter();
	Sepia_filter* sepia = new Sepia_filter();
	Brightness_Increase_filter* brightness_increase = new Brightness_Increase_filter();
	Sobel_filter* sobel = new Sobel_filter();
	Befinition_filter* befinition = new Befinition_filter();
	Erosion* erosion = new Erosion(1, g);
	Dilation* dilation = new Dilation(1, g);
	Closing* closing = new Closing(1, g);
	Opening* opening = new Opening(1, g);
	Embossing_filter* embossing_filter = new Embossing_filter();
	Median_filter* median_filter = new Median_filter(3);
	Glowing_edges* glowing_filter = new Glowing_edges();
	Transfer_filter* transfer = new Transfer_filter();
	Turn_filter* turn = new Turn_filter(90);
	Waves_filter* waves = new Waves_filter(false);
	QImage invertImage = invert->calculateNewImagePixMap(photo);
	cout << "invert success" << endl;
	QImage blurImage = blur->calculateNewImagePixMap(photo);
	cout << "blur success" << endl;
	QImage gaussianBlurImage = gaussian_blur->calculateNewImagePixMap(photo);
	cout << "gaussianBlur success" << endl;
	QImage grayScaleImage = gray_scale->calculateNewImagePixMap(photo);
	cout << "grayScale success" << endl;
	QImage sepiaImage = sepia->calculateNewImagePixMap(photo);
	cout << "sepia success" << endl;
	QImage brightnessIncreaseImage = brightness_increase->calculateNewImagePixMap(photo);
	cout << "brightness success" << endl;
	QImage sobelImage = sobel->calculateNewImagePixMap(photo);
	cout << "sobel success" << endl;
	QImage befinitionImage = befinition->calculateNewImagePixMap(photo);
	cout << "befinition success" << endl;
	QImage erosionImage = erosion->calculateNewImagePixMap(photo);
	cout << "erosion success" << endl;
	QImage dilationImage = dilation->calculateNewImagePixMap(photo);
	cout << "dilation success" << endl;
	QImage closingImage = closing->calculateNewImagePixMap(photo);
	cout << "closing success" << endl;
	QImage openingImage = opening->calculateNewImagePixMap(photo);
	cout << "opening success" << endl;
	QImage embossingImage = embossing_filter->calculateNewImagePixMap(photo);
	cout << "embossing success" << endl;
	QImage medianImage = median_filter->calculateNewImagePixMap(photo);
	cout << "median success" << endl;
	QImage glowingImage = glowing_filter->calculateNewImagePixMap(photo);
	cout << "glowing success" << endl;
	QImage transferImage = transfer->calculateNewImagePixMap(photo);
	cout << "transfer success" << endl;
	QImage turnImage = turn->calculateNewImagePixMap(photo);
	cout << "turn success" << endl;
	QImage wavesImage = waves->calculateNewImagePixMap(photo);
	cout << "waves success" << endl;
	wavesImage.save("WavesFilter.PNG");
	turnImage.save("TurnFilter.PNG");
	transferImage.save("TransferFilter.PNG");
	glowingImage.save("GlowingFilter.PNG");
	medianImage.save("MedianFilter.PNG");
	embossingImage.save("EmbossingFilter.PNG");
	openingImage.save("Opening.PNG");
	closingImage.save("Closing.PNG");
	dilationImage.save("DilationFilter.PNG");
	erosionImage.save("Erosion.PNG");
	brightnessIncreaseImage.save("BrightnessIncreaseFilter.PNG");
	gaussianBlurImage.save("GaussianBlurFilter.PNG");
	sobelImage.save("SobelFilter.PNG");
	befinitionImage.save("BefinitionFilter.PNG");
	sepiaImage.save("SepiaFilter.PNG");
	grayScaleImage.save("GrayScaleFilter.PNG");
	invertImage.save("Invert.PNG");
	blurImage.save("Blur.PNG");
	photo.save("Starting.PNG");

	cout << "save completed" << endl;
}