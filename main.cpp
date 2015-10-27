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

#define N 1024
#define M 10


#define GRID_SIZE 64
#define FFT_BUFFER_SIZE 1024

using namespace std;

void test_fft1();
void test_fft2();
void test_fft3();


int horizontalNum = 8;
int verticalNum = 8;;
int fftBufferSize = 1024;

fftw_complex *in1, *out1;
fftw_plan p1;


int finish_fft;
int start_fft;
// std::condition_variable data_cond;
// std::mutex m;

std::complex <double> koeffs[GRID_SIZE][FFT_BUFFER_SIZE];
std::vector < fftw_complex > delayedVectors[GRID_SIZE];
fftw_complex summ[FFT_BUFFER_SIZE];

// pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;    
// pthread_cond_t count_threshold_cv = PTHREAD_COND_INITIALIZER;


// pthread_mutex_t count_mutex1 = PTHREAD_MUTEX_INITIALIZER;    
// pthread_cond_t count_threshold_cv1 = PTHREAD_COND_INITIALIZER;


void *rec(){
     int i=0;
          while(1) {
          finish_fft=0;
          //pthread_mutex_lock(&count_mutex);  
          //while (!finish_fft) pthread_cond_wait(&count_threshold_cv,&count_mutex);          
          //pthread_mutex_unlock(&count_mutex);               
     }
}
main()
{
     //pthread_t thread1, thread2 , thread3;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     clock_t start, end;
     double cpu_time_used;

    /* Create independent threads each of which will execute function */

     //iret1 = pthread_create( &test_fft1, NULL, print_message_function, (void*) message1);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
     int a;
     printf("Load 1024 points for 1000 repeat thread fftw3 \n");
     //pthread_create( &thread3, NULL, rec, (void*) message1);
	

	for (int j = 0; j < horizontalNum * verticalNum; j++)	{
		fftw_complex *tt;         
        tt = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftBufferSize);
		delayedVectors[j].assign(tt,tt+fftBufferSize);	
	 }
     start = clock();

     
	//std::thread iret1(test_fft1);
     for (int i=0;i<M+1;i++) {
         // pthread_mutex_lock(&count_mutex1);
         // start_fft=1;
         // pthread_cond_signal(&count_threshold_cv1);     
         // pthread_mutex_unlock(&count_mutex1);

         // pthread_mutex_lock(&count_mutex);  
         // while (!finish_fft) pthread_cond_wait(&count_threshold_cv,&count_mutex);          
         // finish_fft=0;
         // pthread_mutex_unlock(&count_mutex);               
     };
//     pthread_join( thread1, NULL);
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

fftw_complex tmp_summ ;
double  ac,bd,ad,bc;

void f_array_mul(void){


for (int i = 0; i < fftBufferSize ; i++){//for (int i = 0; i < GlobalData::fftOversamplingK * (GlobalData::fftBufferSize - GlobalData::fftOverload); i++){//= GlobalData::fftOversamplingK) 
        tmp_summ[0]=0;
        tmp_summ[1]=0;
        for (int j = 0; j < horizontalNum * verticalNum; j++) {
            try {
              ac = delayedVectors[j].at(i)[0] * koeffs[j][i].real();
              bd = delayedVectors[j].at(i)[1] * koeffs[j][i].imag();
              ad = delayedVectors[j].at(i)[0] * koeffs[j][i].imag();
              bc = delayedVectors[j].at(i)[1] * koeffs[j][i].real();
            } catch (const std::out_of_range& oor) {
                std::cout << "Out of Range error: " << oor.what() << '\n';
            }
            tmp_summ[0] += (ac - bd);
            tmp_summ[1] += (bc + ad);
        }
        tmp_summ[0] = tmp_summ[0] / ((horizontalNum * verticalNum) * fftBufferSize );
        tmp_summ[1] = tmp_summ[1] / ((horizontalNum * verticalNum) * fftBufferSize );
        
        summ[i][0] = tmp_summ[0];
        summ[i][1] = tmp_summ[1];
    }

 }


// void *test_fft1( void *ptr )
// {
//          fftw_complex *in, *out;
//          fftw_plan p;
//          int j=0;

//          in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
//          out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
         
// while(1){
//          pthread_mutex_lock(&count_mutex1);  
//          while (!start_fft) pthread_cond_wait(&count_threshold_cv1,&count_mutex1);
//          start_fft=0;          
//          pthread_mutex_unlock(&count_mutex1);               


//          // for(int i=0;i<N;i++) in[i][0]=rand() % 1000 + 1,in[i][1]=0;

//          // p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
         
//          // fftw_execute(p);  repeat as needed 
          
//          pthread_mutex_lock(&count_mutex);
//          finish_fft=1;
//          pthread_cond_signal(&count_threshold_cv);     
//          pthread_mutex_unlock(&count_mutex);

// 		 f_array_mul();

//          if (j++>M-1) {
//                // fftw_destroy_plan(p);
//                // fftw_free(in); 
//                // fftw_free(out);
//                printf("Finish test1 \n");
//                pthread_exit(0);
//           }
//      };
     
// }

void test_fft2()
{
         // fftw_complex *in, *out;
         // fftw_plan p;
         // int i=0;

         // in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
         // out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

         // for(int i=0;i<N;i++) in[i][0]=rand() % 1000 + 1,in[i][1]=0;

         // p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
         
         // fftw_execute(p); /* repeat as needed */
         
         // fftw_destroy_plan(p);
         // fftw_free(in); 
         // fftw_free(out);
		f_array_mul();

         
}

void test_fft3()
{
     

      for (int j=0;j<M;j++) {    
     //     fftw_complex *in, *out;
     //     fftw_plan p;
         
     //     in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
     //     out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

     //     for(int i=0;i<N;i++) in[i][0]=rand() % 1000 + 1,in[i][1]=0;

     //     p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
         
     //     fftw_execute(p); /* repeat as needed */
         
     //     fftw_destroy_plan(p);
     //     fftw_free(in); 
     //     fftw_free(out);

		f_array_mul();
         //usleep(4600);
     }         
     
}