#include <iostream>
#include "transform.hpp"

// * image :  input gray scale image
// * _image: output gray scale image

int main() {
	Image pixel("in.txt");
	pixel.drawHist("oldHist.txt");
	Image pixel_new = pixel.equalizeHist();
	pixel_new.drawHist("Hist.txt");
	Mask laplacian;
	Image ans = pixel_new * laplacian;
	Image answer = ans.equalizeHist();
	answer.dump2File("out.txt");
	return 0;
}
