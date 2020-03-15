#pragma once
#include <qimage.h>
#include <math.h>

const double pi = 3.1415926535897932384626433832795;

class Filters {
public:
	Filters() {};
	~Filters() {};

	virtual QImage calculateNewImagePixMap(const QImage&) = 0;
};

class Matrix_filter : public Filters {
public:
	float* vector;
	int mRadius;

	Matrix_filter(int radius = 1) : mRadius(radius) {};
	~Matrix_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
	virtual QColor calculateNewPixelColor(QImage, int, int);
};

class Invert_filter : public Filters {
public:
	Invert_filter() {};
	~Invert_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Brightness_Increase_filter : public Filters {
public:
	Brightness_Increase_filter() {};
	~Brightness_Increase_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Gray_scale_filter : public Filters {
public:
	Gray_scale_filter() {};
	~Gray_scale_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Sepia_filter : public Filters {
public:
	Sepia_filter() {};
	~Sepia_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Befinition_filter : public Matrix_filter {
public:
	Befinition_filter() {
		mRadius = 1;
		vector = new float[9]{ 0, -1, 0, -1, 5, -1, 0, -1, 0 };
	}
};

class Blur_filter : public Matrix_filter {
public:
	Blur_filter(int radius = 1) : Matrix_filter(radius) {
		int size = 2 * mRadius + 1;
		vector = new float[size * size];
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				vector[i * size + j] = 1.0f / (size * size);
	}
};

class Gaussian_blur_filter : public Matrix_filter {
public:
	Gaussian_blur_filter() {
		createGaussianVector(3, 2);
	}
	~Gaussian_blur_filter() {};

	void createGaussianVector(int, int);
};

class Sobel_filter : public Matrix_filter {
private:
	void createSobelVector();
public:
	Sobel_filter() {
		createSobelVector();
	};
	~Sobel_filter() {};

	QColor calculateNewPixelColor(QImage, int, int);
};

class Morphology {
public:
	int radius;
	int* slink;

	Morphology(int _radius, int* _slink) {
		radius = _radius;
		int size = 2 * radius + 1;
		slink = new int[size * size];

		for (int i = 0; i < size * size; i++)
			slink[i] = _slink[i];
	};
	~Morphology() {
		delete[] slink;
	};

	QImage calculateNewImagePixMap(const QImage&);
	virtual QColor calculateNexPixelColor(const QImage&, int, int) = 0;
};

class Erosion : public Morphology {
public:
	Erosion(int _radius, int* _slink) : Morphology(_radius, _slink) {};
	~Erosion() {};

	QColor calculateNexPixelColor(const QImage&, int, int);
};

class Dilation : public Morphology {
public:
	Dilation(int _radius, int* _slink) : Morphology(_radius, _slink) {};
	~Dilation() {};

	QImage calculateNewImagePixMap(const QImage&);
	QColor calculateNexPixelColor(const QImage&, int, int);
};

class Closing : public Morphology {
public:
	Closing(int _radius, int* _slink) : Morphology(_radius, _slink) {};
	~Closing() {};

	QImage calculateNewImagePixMap(const QImage&);
	QColor calculateNexPixelColor(const QImage&, int, int);
};

class Opening : public Morphology {
public:
	Opening(int _radius, int* _slink) : Morphology(_radius, _slink) {};
	~Opening() {};

	QImage calculateNewImagePixMap(const QImage&);
	QColor calculateNexPixelColor(const QImage&, int, int);
};

class Embossing_filter : public Matrix_filter {
public:
	Embossing_filter() {
		mRadius = 1;
		int size = 2 * mRadius + 1;
		vector = new float[size * size]{ 0, 1, 0, 1, 0, -1, 0, -1, 0 };
	}
	~Embossing_filter() {};

	QColor calculateNewPixelColor(QImage, int, int);
};

class Median_filter : public Matrix_filter {
public:
	Median_filter(int _radius = 1) : Matrix_filter(_radius) {};
	~Median_filter() {};

	QColor calculateNewPixelColor(QImage, int, int);
};

class Glowing_edges : public Filters {
public:
	Glowing_edges() {};
	~Glowing_edges() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Transfer_filter : public Filters {
public:
	Transfer_filter() {};
	~Transfer_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Turn_filter : public Filters {
private:
	double angle;
public:
	Turn_filter(int _angle) {
		angle = _angle * pi / 180;
	};
	~Turn_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};

class Waves_filter : public Filters {
private:
	double Waves;
	int r;
public:
	Waves_filter(bool _typeOfWaves = true) {
		if (_typeOfWaves) {
			r = 1;
			Waves = 2 * pi / 60;
		}
		else {
			r = 2;
			Waves = 2 * pi / 30;
		}
	};
	~Waves_filter() {};

	QImage calculateNewImagePixMap(const QImage&);
};