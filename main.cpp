
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
 #include <fftw3.h>

#define N 1024

void *print_message_function( void *ptr );



fftw_complex *in1, *out1;
fftw_plan p1;

main()
{
     pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;
     clock_t start, end;
     double cpu_time_used;

    /* Create independent threads each of which will execute function */

     //iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
     //iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
     int a;
     printf("Load 1024 point for 1000 repeat thread fftw3 \n");

     start = clock();
     for (int i=0;i<1000;i++) {
          iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
          pthread_join( thread1, NULL);
     };
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time %lf \n",cpu_time_used);

     printf("Load 1024 point for 1000 repeat only fftw3 \n");
     start = clock();
    for (int i=0;i<1000;i++) {
         print_message_function(&a);
     };
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time %lf \n",cpu_time_used);


     printf("Thread 1 returns: %d\n",iret1);
     printf("Thread 2 returns: %d\n",iret2);
     exit(0);
};

void *print_message_function( void *ptr )
{
         fftw_complex *in, *out;
         fftw_plan p;
         
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
         //printf("finish \n");
}