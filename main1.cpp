

#include <cstdlib>
#include <complex>
#include <fftw3.h>
#include <vector>
#include <thread>

using namespace std;

void  test1(){

}

int main(int argc, char** argv) {
vector <fftwf_complex> gridRawDataAfterFft;
fftwf_complex *r;
//gridRawDataAfterFft.assign(r, r + (100));

std::thread th1(test1);
th1.join();

   return 0;
}