#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <unistd.h> 
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <cstdlib>

#include <complex>
#include <fftw3.h>
#include <vector>


#include <iostream>
#include <cmath>
#include <chrono>

#define N 1024
#define M 10


#define GRID_SIZE 64
#define FFT_BUFFER_SIZE 1024

using namespace std;

void test_fft1();
void test_fft2();
void test_fft3();
void f_array_mul();

int horizontalNum = 8;
int verticalNum = 8;;
int fftBufferSize = 1024;

//fftwf_complex *in1, *out1;
//fftwf_plan p1;


int finish_fft;
int start_fft;

typedef double point[2];

typedef union _vfftw_complex {
     fftwf_complex p;
     struct sfftw_complex{
          float re;
          float im;
     } sp;
} vfftw_complex;

std::complex <float> koeffs[GRID_SIZE][FFT_BUFFER_SIZE];
std::vector < vfftw_complex > delayedVectors[GRID_SIZE];
fftw_complex summ[FFT_BUFFER_SIZE];

 std::mutex count_mutex;    
 std::condition_variable count_threshold_cv;


 std::mutex count_mutex1;    
 std::condition_variable count_threshold_cv1;

main()
{
     //pthread_t thread1, thread2 , thread3;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     clock_t start, end;
     double cpu_time_used;

     int a;
     printf("Load 1024 points for 1000 repeat thread fftw3 \n");
     //pthread_create( &thread3, NULL, rec, (void*) message1);
	

	for (int j = 0; j < horizontalNum * verticalNum; j++)	{
	   fftwf_complex *tt;         
        tt = ( fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * fftBufferSize);
        delayedVectors[j].assign((vfftw_complex*)tt,(vfftw_complex*)tt+fftBufferSize);	
	 }

     start = clock();
     std::thread iret1(test_fft1);
     for (int i=0;i<M+1;i++) {

          {
            std::unique_lock<std::mutex> locker(count_mutex);     
            start_fft=1;
            count_threshold_cv.notify_one();
          }
          {           
           std::unique_lock<std::mutex> locker(count_mutex1);
           while (!finish_fft) count_threshold_cv1.wait(locker);          
           finish_fft=0;
          }
          //std::this_thread::sleep_for(std::chrono::milliseconds(1000));     
     };
     iret1.join();
     end = clock();
     cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC) / M;
     printf("time %lf \n",cpu_time_used);


     printf("Load 1024 points for 1000 repeat only fftw3 \n");
     start = clock();
    for (int i=0;i<M;i++) {         
        std::thread iret1(test_fft2);
        iret1.join();
     };
     end = clock();
     cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC )/ M;
     printf("time %lf \n",cpu_time_used);



     printf("Load 1024 points for 1000 repeat only fftw3 \n");
     start = clock();
    for (int i=0;i<M;i++) {         
        test_fft2();
     };
     end = clock();
     cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC )/ M;
     printf("time %lf \n",cpu_time_used);

     printf("Load 1024 points thred fftw3 \n");
     start = clock();
     std::thread iret2(test_fft3);
     iret2.join();

     end = clock();
     cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC )/ M;
     printf("time %lf \n",cpu_time_used);

     exit(0);
};

fftw_complex tmp_summ[FFT_BUFFER_SIZE];

float  ac,bd,ad,bc;
float  ac1,bd1,ad1,bc1;

void f_array_mul(void){

for (int j = 0; j < fftBufferSize ; j++) tmp_summ[j][0]=0,tmp_summ[j][1]=0;

for (int j = 0; j < horizontalNum * verticalNum ; j++){//for (int i = 0; i < GlobalData::fftOversamplingK * (GlobalData::fftBufferSize - GlobalData::fftOverload); i++){//= GlobalData::fftOversamplingK) 

        for (int i = 0; i < fftBufferSize; i+=2) {
            try {
              ac = delayedVectors[j].at(i).p[0] * koeffs[j][i].real();
              bd = delayedVectors[j].at(i).p[1] * koeffs[j][i].imag();
              ad = delayedVectors[j].at(i).p[0] * koeffs[j][i].imag();
              bc = delayedVectors[j].at(i).p[1] * koeffs[j][i].real();

              ac1 = delayedVectors[j].at(i+1).p[0] * koeffs[j][i+1].real();
              bd1 = delayedVectors[j].at(i+1).p[1] * koeffs[j][i+1].imag();
              ad1 = delayedVectors[j].at(i+1).p[0] * koeffs[j][i+1].imag();
              bc1 = delayedVectors[j].at(i+1).p[1] * koeffs[j][i+1].real();

            } catch (const std::out_of_range& oor) {
                std::cout << "Out of Range error: " << oor.what() << '\n';
            }
            tmp_summ[i][0] += (ac - bd); 
            tmp_summ[i+1][0] += (ac1 - bd1);
            tmp_summ[i][1] += (bc + ad);
            tmp_summ[i+1][1] += (bc1 + ad1);
        }        
    }
    for (int j = 0; j < fftBufferSize ; j++) {
        summ[j][0] = tmp_summ[j][0] / ((horizontalNum * verticalNum) * fftBufferSize );
        summ[j][1] = tmp_summ[j][1] / ((horizontalNum * verticalNum) * fftBufferSize );     
     }
 }


void test_fft1()
{
int j=0;

while(1){
          {           
           std::unique_lock<std::mutex> locker(count_mutex);
           while (!start_fft) count_threshold_cv.wait(locker);          
           start_fft=0;
          }               

	     f_array_mul();

          {
            std::unique_lock<std::mutex> locker(count_mutex1);
            finish_fft=1;
            count_threshold_cv1.notify_one();
          }


         if (j++>M-1) {
               printf("Finish test1 \n");
               pthread_exit(0);
          }

     };
     
}

void test_fft2()
{

		f_array_mul();

         
}

void test_fft3()
{     
      for (int j=0;j<M;j++) {    
		f_array_mul();
     }         
     
}