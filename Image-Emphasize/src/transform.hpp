#include <iostream>
#include <cstdlib>
#include <cstring>
// feel free to explore what has been added to C++ since C++11!
#include <tuple>
// use assert to comsolidate your programe!
#include <cassert>
#include <fstream>
#include <cmath>
using namespace std;

int Hist[256] = {};

class Array2d;
class Mask;
class Image;
class Array2d {
	protected:
		int **data;
		int32_t h, w;
	public:
		// dummy constructor
		Array2d(): data(nullptr), h(-1), w(-1) {}
		// destructor
		~Array2d() {
			// TODO
			// deallocate memory on heap
			for (int i = 0; i < h; i++)
				delete [] data[i];
			delete [] data;

		}
		// allocate memory to data with spectified size
		void create(const int32_t hh, const int32_t ww) {
			// TODO
			// !NOTICE
			// Don't forget to free previous allocated memory
			h = hh;
			w = ww;
			data = new int *[h];
			for (int i = 0; i < h; i++)
				data[i] = new int[w];
			assert(empty ());
		}
		// helper functions
		bool empty() const {
			return data != nullptr;
		}
		// URL of reference to std::tuple
		// https://en.cppreference.com/w/cpp/utility/tuple
		std::tuple<int32_t, int32_t> getSize() const {
			return std::make_tuple(h, w);
		}
		// *BONUS: use copy constructor like follows
		Array2d(const Array2d &array2) {
			// TODO
			h = array2.h;
			w = array2.w;
			data = new int *[h];
			for (int i = 0; i < h; i++)
				data[i] = new int[w];
			assert(empty());
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					data[i][j] = array2.data[i][j];

		}
};

class Image: public Array2d {
		// default constructor
	public:
		Image() {}
		// constructor from Array2d instance
		Image(const Array2d &array2): Array2d(array2) {
			// TODO
			/*h = array2.h;
			w = array2.w;
			data = new int *[h];
			for (int i = 0; i < h, i++)
				data[i] = new int[w];
			assert(empty());
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					data[i][j] = array2.data[i][j];*/
		}
		// constructor from intermediate file
		Image(const char *filename) {
			// TODO
			// open and read intermediate file, get image information
			// !NOTICE
			// call create() method to set intrinsic value
			ifstream fin(filename);
			if (!fin) {
				cerr << "IN ERROR\n";
				return ;
			}
			fin >> h >> w;
			create(h, w);
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					fin >> data[i][j];
			fin.close();
		}
		// *BONUS: add a copy constructor
		// destructor
		// destructor of Array2d will deallocate memory, which is called automatically
		~Image() {}
		// method performing Histogram Equalization
		Image equalizeHist()const {
			// TODO
			// return a newly constructed Image instance
			Image temp;
			temp.create(h, w);
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					int num = 0;
					for (int k = 0; k < h; k++)
						for (int l = 0; l < w; l++) {
							if (data[k][l] <= data[i][j]) {
								num++;
							}
						}
					temp.data[i][j] = round((255 * num) / (h * w));
				}
			}
			int minn = 0, maxn = 255;
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					maxn = (maxn > temp.data[i][j] ? maxn : temp.data[i][j]);
					minn = (minn < temp.data[i][j] ? minn : temp.data[i][j]);
				}
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					temp.data[i][j] = round(255 * (temp.data[i][j] - minn) / (maxn - minn));
				}
			/*	for (int i = 0; i < h; i++)
					for (int j = 0; j < w; j++) {
						data[i][j] = temp[i][j];
					}*/
			return temp;
		}
		// method showing Histogram
		void drawHist(const char *filename) const {
			// TODO
			// open/create a file to draw histograme
			// return the filename
			ofstream fout(filename);
			if (!fout) {
				cerr << "Hist ERROR\n";

			}
			for (int k = 0; k < 256; k++)
				Hist[k] = 0;
			for (int k = 0; k <= 255; k++) {
				for (int i = 0; i < h; i++)
					for (int j = 0; j < w; j++)
						Hist[data[i][j]]++;
			}
			for (int k = 0; k <= 255; k++) {
				fout << k << " || ";
				for (int l = 1; l <= (Hist[k] / 4000) ; l++)
					fout << "бя";
				fout << endl;
			}

		}
		// dump array2d into intermediate file
		void dump2File(const char *filename = "out.txt") const {
			// TODO
			// format: first line: h w
			//         other line: a number of gray scale
			// please refer to annotations in python scripts
			// for more information
			ofstream fout("out.txt");
			if (!fout) {
				cerr << "OUT ERROR\n";

			}
			fout << h << " " << w << endl;
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					fout << data[i][j] << endl;
			fout.close();
		}
		// operator override to perform a convolution
		friend class Mask;
		Image operator *(const Mask &mask) const {
			// TODO
			// return a newly constructed image instance
			// after convolution operation
			Image temp;
			temp.create(h, w);
			int ex[h + 2][w + 2] = {};
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					ex[i + 1][j + 1] = data[i][j];
				}
			}
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					temp.data[i][j] = data[i][j];
					for (int k = 0; k < 3; k++)
						for (int l = 0; l < 3; l++) {
							if ((k != 1) || (l != 1))
								temp.data[i][j] -= ex[i + k][j + l];
							else
								temp.data[i][j] += 9 * ex[i + k][j + l];
						}
				}
			}

			int minn = 0, maxn = 255;
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					maxn = (maxn > temp.data[i][j] ? maxn : temp.data[i][j]);
					minn = (minn < temp.data[i][j] ? minn : temp.data[i][j]);
				}
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					temp.data[i][j] = round(255 * (temp.data[i][j] - minn) / (maxn - minn));


				}
			return temp;
		}
};

class Mask: public Array2d {
		// you can use Mask::create() to construct
	public:
		Mask() {
			// TODO
			create(3, 3);
			int h = 3, w = 3;
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					if ((i != h / 2 - 1) || (j != w / 2 - 1))
						data[i][j] = -1;
					else
						data[i][j] = 9;
				}
		}
		// *BONUS: use initializer_list after C++11 to
		// *build another parameterized constructor
		// URL to std::initializer_list
		// https://en.cppreference.com/w/cpp/utility/initializer_list
		// operator override to perform a convolution
		friend class Image;
		Image operator *(const Image &image) const {
			// TODO
			// similar in what you have previously done
			Image temp;
			int ex[h + 2][w + 2] = {};
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					ex[i + 1][j + 1] = image.data[i][j];
				}
			}
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					temp.data[i][j] = data[i][j];
					for (int k = 0; k < 3; k++)
						for (int l = 0; l < 3; l++) {
							if ((k != 1) || (l != 1))
								temp.data[i][j] -= ex[i + k][j + l];
							else
								temp.data[i][j] += 9 * ex[i + k][j + l];
						}
				}
			}
			return temp;
		}

};