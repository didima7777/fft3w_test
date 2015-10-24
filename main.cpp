#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fftw3.h>
#include <unistd.h> 
#include <mutex>
#include <condition_variable>

#define N 10240

void *test_fft1( void *ptr );
void *test_fft2( void *ptr );
void *test_fft3( void *ptr );

fftw_complex *in1, *out1;
fftw_plan p1;


int finish_fft;
int start_fft;
std::condition_variable data_cond;
std::mutex m;

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;    
pthread_cond_t count_threshold_cv = PTHREAD_COND_INITIALIZER;


pthread_mutex_t count_mutex1 = PTHREAD_MUTEX_INITIALIZER;    
pthread_cond_t count_threshold_cv1 = PTHREAD_COND_INITIALIZER;


void *rec( void *ptr ){
     int i=0;
          while(1) {
          finish_fft=0;
          pthread_mutex_lock(&count_mutex);  
          while (!finish_fft) pthread_cond_wait(&count_threshold_cv,&count_mutex);          
          pthread_mutex_unlock(&count_mutex);               
     }
}
main()
{
     pthread_t thread1, thread2 , thread3;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;
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

     start = clock();
     iret1 = pthread_create( &thread1, NULL, test_fft1, (void*) message1);
     for (int i=0;i<1001;i++) {
         pthread_mutex_lock(&count_mutex1);
         start_fft=1;
         pthread_cond_signal(&count_threshold_cv1);     
         pthread_mutex_unlock(&count_mutex1);

         pthread_mutex_lock(&count_mutex);  
         while (!finish_fft) pthread_cond_wait(&count_threshold_cv,&count_mutex);          
         finish_fft=0;
         pthread_mutex_unlock(&count_mutex);               
     };
     //pthread_join( thread1, NULL);
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time %lf \n",cpu_time_used);


     printf("Load 1024 points for 1000 repeat only fftw3 \n");
     start = clock();
    for (int i=0;i<1000;i++) {         
        test_fft2(&a);
     };
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time %lf \n",cpu_time_used);

     printf("Load 1024 points thred fftw3 \n");
     start = clock();
     iret2 = pthread_create( &thread2, NULL, test_fft3, (void*) message2);
     pthread_join( thread2, NULL);
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time %lf \n",cpu_time_used);

     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     exit(0);
};



void *test_fft1( void *ptr )
{
         fftw_complex *in, *out;
         fftw_plan p;
         int j=0;

         in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
         out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
         
while(1){
         pthread_mutex_lock(&count_mutex1);  
         while (!start_fft) pthread_cond_wait(&count_threshold_cv1,&count_mutex1);
         start_fft=0;          
         pthread_mutex_unlock(&count_mutex1);               


         for(int i=0;i<N;i++) in[i][0]=i,in[i][1]=0;

         p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
         
         fftw_execute(p); /* repeat as needed */
         
         pthread_mutex_lock(&count_mutex);
         finish_fft=1;
         pthread_cond_signal(&count_threshold_cv);     
         pthread_mutex_unlock(&count_mutex);


         if (j++>999) {
               fftw_destroy_plan(p);
               fftw_free(in); 
               fftw_free(out);
               printf("Finish test1 \n");
               pthread_exit(0);
          }
};
     
}

void *test_fft2( void *ptr )
{
         fftw_complex *in, *out;
         fftw_plan p;
         int i=0;

         in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
         out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

         for(int i=0;i<N;i++) in[i][0]=i,in[i][1]=0;

         p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
         
         fftw_execute(p); /* repeat as needed */
         

          // FILE *fp=fopen("fft_out.raw","w");
          // fwrite(out , 1 , sizeof(out) , fp );
          // fclose(fp);

         fftw_destroy_plan(p);
         fftw_free(in); 
         fftw_free(out);

     
         //printf("finish test fft 1!!!\n");
}

void *test_fft3( void *ptr )
{
     

     for (int j=0;j<1000;j++) {    
         fftw_complex *in, *out;
         fftw_plan p;
         
         in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
         out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

         for(int i=0;i<N;i++) in[i][0]=i,in[i][1]=0;

         p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
         
         fftw_execute(p); /* repeat as needed */
         
         fftw_destroy_plan(p);
         fftw_free(in); 
         fftw_free(out);
         //usleep(4600);
     }         
     
}