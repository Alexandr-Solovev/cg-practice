#include "Filters.h"

template<class T>
T clamp(T v, int max, int min) {
	if (v > max)
		return max;
	if (v < min)
		return min;
	return v;
}

bool compareColors(const QColor& c1, const QColor& c2) {
	float intensity_c1 = 0.36 * c1.red() + 0.53 * c1.green() + 0.11 * c1.blue();
	float intensity_c2 = 0.36 * c2.red() + 0.53 * c2.green() + 0.11 * c2.blue();
	return(intensity_c1 < intensity_c2);
}

QImage Matrix_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result_Image(_image);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor image_color = calculateNewPixelColor(_image, x, y);
			result_Image.setPixelColor(x, y, image_color);
		}

	return result_Image;
}

QColor Matrix_filter::calculateNewPixelColor(QImage image, int x, int y) {
	int returnR = 0;
	int returnG = 0;
	int returnB = 0;
	int size = 2 * mRadius + 1;
	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++) {
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = image.pixelColor(clamp<int>(x + i, image.width() - 1, 0), clamp<int>(y + j, image.height() - 1, 0));
			returnR += color.red() * vector[idx];
			returnG += color.green() * vector[idx];
			returnB += color.blue() * vector[idx];
		}
	return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));
}

QImage Invert_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result_Image(_image);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor image_color = _image.pixelColor(x, y);
			image_color.setRgb(255 - image_color.red(), 255 - image_color.green(), 255 - image_color.blue());
			result_Image.setPixelColor(x, y, image_color);
		}

	return result_Image;
}

QImage Brightness_Increase_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result_Image(_image);
	int k = 30;

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor image_color = _image.pixelColor(x, y);
			image_color.setRgb(clamp<int>(image_color.red() + k, 255, 0), clamp<int>(image_color.green() + k, 255, 0), clamp<int>(image_color.blue() + k, 255, 0));
			result_Image.setPixelColor(x, y, image_color);
		}

	return result_Image;
}

QImage Gray_scale_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result_Image(_image);
	int Intensity;

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor image_color = _image.pixelColor(x, y);
			Intensity = 0.36 * image_color.red() + 0.53 * image_color.green() + 0.11 * image_color.blue();
			image_color.setRgb(Intensity, Intensity, Intensity);
			result_Image.setPixelColor(x, y, image_color);
		}

	return result_Image;
}

QImage Sepia_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result_Image(_image);
	int Intensity;
	float k = 20.0f;

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor image_color = _image.pixelColor(x, y);
			Intensity = 0.36 * image_color.red() + 0.53 * image_color.green() + 0.11 * image_color.blue();
			image_color.setRgb(clamp<int>(Intensity + 2 * k, 255, 0), clamp<int>(Intensity + 0.5 * k, 255, 0), clamp<int>(Intensity - 1 * k, 255, 0));
			result_Image.setPixelColor(x, y, image_color);
		}

	return result_Image;
}

void Gaussian_blur_filter::createGaussianVector(int radius, int sigma) {
	const unsigned int size = 2 * radius + 1;
	float norm = 0;
	mRadius = radius;
	vector = new float[size * size];

	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + (j + radius);
			vector[idx] = exp(-(i * i + j * j) / (sigma * sigma));
			norm += vector[idx];
		}

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			vector[i * size + j] /= norm;
}

void Sobel_filter::createSobelVector() {
	vector = new float[18];
	float tmp[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 , -1, -2, -1, 0, 0, 0, 1, 2, 1 };

	for (int i = 0; i < 18; i++) {
		vector[i] = tmp[i];
	}
}

QColor Sobel_filter::calculateNewPixelColor(QImage image, int x, int y) {
	int RX = 0;
	int GX = 0;
	int BX = 0;
	int RY = 0;
	int GY = 0;
	int BY = 0;
	int radius = 1;
	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * 3 + j + radius;
			int idy = (i + radius) * 3 + j + radius + 9;
			QColor color = image.pixelColor(clamp<int>(x + i, image.width() - 1, 0), clamp<int>(y + j, image.height() - 1, 0));
			RX += color.red() * vector[idx];
			GX += color.green() * vector[idx];
			BX += color.blue() * vector[idx];
			RY += color.red() * vector[idy];
			GY += color.green() * vector[idy];
			BY += color.blue() * vector[idy];
		}
	return QColor(clamp(sqrt(RX * RX + RY * RY), 255, 0), clamp(sqrt(GX * GX + GY * GY), 255, 0), clamp(sqrt(BX * BX + BY * BY), 255, 0));
}

QImage Morphology::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++)
			result.setPixelColor(x, y, calculateNexPixelColor(_image, x, y));
	return result;
}

QColor Erosion::calculateNexPixelColor(const QImage& _image, int x, int y) {
	QColor result(255, 255, 255);
	int size = 2 * radius + 1;

	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + j + radius;
			QColor color = _image.pixelColor(clamp<int>(x + i, _image.width() - 1, 0), clamp<int>(y + j, _image.height() - 1, 0));
			if (slink[idx] && !compareColors(result, color))
				result = color;
		}
	return result;
}

QImage Dilation::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image.width() + 1, _image.height() + 1, QImage::Format_RGB32);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++)
			result.setPixelColor(x, y, calculateNexPixelColor(_image, x, y));
	return result;
}

QColor Dilation::calculateNexPixelColor(const QImage& _image, int x, int y) {
	QColor result(0, 0, 0);
	int size = 2 * radius + 1;

	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + j + radius;
			QColor color = _image.pixelColor(clamp<int>(x + i, _image.width() - 1, 0), clamp<int>(y + j, _image.height() - 1, 0));
			if (slink[idx] && compareColors(result, color))
				result = color;
		}
	return result;
}

QImage Closing::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);

	Dilation* dilation = new Dilation(radius, slink);
	Erosion* erosion = new Erosion(radius, slink);

	result = dilation->calculateNewImagePixMap(result);
	result = erosion->calculateNewImagePixMap(result);

	return result;
}

QColor Closing::calculateNexPixelColor(const QImage& _image, int x, int y) {
	return _image.pixelColor(x, y);
}

QImage Opening::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);

	Dilation* dilation = new Dilation(radius, slink);
	Erosion* erosion = new Erosion(radius, slink);

	result = erosion->calculateNewImagePixMap(result);
	result = dilation->calculateNewImagePixMap(result);

	return result;
}

QColor Opening::calculateNexPixelColor(const QImage& _image, int x, int y) {
	return _image.pixelColor(x, y);
}

QColor Embossing_filter::calculateNewPixelColor(QImage _image, int x, int y) {
	int returnR = 0;
	int returnG = 0;
	int returnB = 0;
	int size = 2 * mRadius + 1;
	int k = 32;

	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++) {
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = _image.pixelColor(clamp<int>(x + i, _image.width() - 1, 0), clamp<int>(y + j, _image.height() - 1, 0));

			returnR += color.red() * vector[idx];
			returnG += color.green() * vector[idx];
			returnB += color.blue() * vector[idx];
		}

	int Itensity = 0.36 * (returnR + k) + 0.53 * (returnG + k) + 0.11 * (returnB + k);
	return QColor(clamp(Itensity, 255, 0), clamp(Itensity, 255, 0), clamp(Itensity, 255, 0));
}

QColor Median_filter::calculateNewPixelColor(QImage _image, int x, int y) {
	int size = 2 * mRadius + 1;
	int* returnR = new int[size * size];
	int* returnG = new int[size * size];
	int* returnB = new int[size * size];

	for (int i = -mRadius; i <= mRadius; i++)
		for (int j = -mRadius; j <= mRadius; j++) {
			int idx = (i + mRadius) * size + j + mRadius;
			QColor color = _image.pixelColor(clamp<int>(x + i, _image.width() - 1, 0), clamp<int>(y + j, _image.height() - 1, 0));
			returnR[idx] = color.red();
			returnG[idx] = color.green();
			returnB[idx] = color.blue();
		}

	std::sort(returnR, returnR + size * size - 1);
	std::sort(returnG, returnG + size * size - 1);
	std::sort(returnB, returnB + size * size - 1);

	return QColor(returnR[size * size / 2], returnG[size * size / 2], returnB[size * size / 2]);
}

QImage Glowing_edges::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);

	Median_filter* median = new Median_filter();
	Sobel_filter* sobel = new Sobel_filter();

	result = median->calculateNewImagePixMap(result);
	result = sobel->calculateNewImagePixMap(result);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor color = result.pixelColor(x, y);
			if (color.red() > color.green() && color.red() > color.blue())
				color.setRgb(color.red(), 0, 0);
			if (color.green() > color.red() && color.green() > color.blue())
				color.setRgb(0, color.green(), 0);
			if (color.blue() > color.red() && color.blue() > color.green())
				color.setRgb(0, 0, color.blue());
			result.setPixelColor(x, y, color);
		}
	return result;
}

QImage Transfer_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			QColor color(0, 0, 0);
			if (x + 50 < _image.width())
				color = _image.pixelColor(x + 50, y);
			result.setPixelColor(x, y, color);
		}

	return result;
}

QImage Turn_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);
	int i0 = result.width() / 2;
	int j0 = result.height() / 2;

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			int i = (x - i0) * cos(angle) - (y - j0) * sin(angle) + i0;
			int j = (x - i0) * sin(angle) + (y - j0) * cos(angle) + j0;

			QColor color(0, 0, 0);
			if (i < _image.width() && i > 0 && j < _image.height() && j > 0)
				color = _image.pixelColor(i, j);
			result.setPixelColor(x, y, color);
		}

	return result;
}

QImage Waves_filter::calculateNewImagePixMap(const QImage& _image) {
	QImage result(_image);

	for (int x = 0; x < _image.width(); x++)
		for (int y = 0; y < _image.height(); y++) {
			int i;
			if (r == 1) i = x + 20 * sin(Waves * y);
			i = x + 20 * sin(Waves * x);
			int j = y;
			QColor color(0, 0, 0);
			if (i < _image.width() && i > 0)
				color = _image.pixelColor(i, j);
			result.setPixelColor(x, y, color);
		}

	return result;
}