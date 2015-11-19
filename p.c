#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_rwlock_t conn_mapper_datastruct_lock;
pthread_rwlockattr_t rwattr ;

void WriteSomething(void threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Write Thread ID #%ld!\n",tid);
    pthread_rwlock_wrlock(&conn_mapper_datastruct_lock);
    printf("Printing After taking Write lock\n");
    pthread_exit(NULL);
}

void ReadSomething(void threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Read Thread ID #%ld!\n",tid);
    pthread_rwlock_rdlock(&conn_mapper_datastruct_lock);
    printf("After taking read lock, sleeping for 3 seconds\n");
    sleep(3);
    pthread_rwlock_rdlock(&conn_mapper_datastruct_lock);
    printf("After taking read lock 2\n");
    pthread_exit(NULL);
}

int main(void)
{
    int case_value =1 ;
    pthread_t threads[2];
    int rc;
    long t;


    pthread_rwlockattr_init(&rwattr);
    pthread_rwlockattr_setkind_np( &rwattr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP ) ;
    pthread_rwlock_init( &conn_mapper_datastruct_lock, &rwattr ) ;

    printf("In main: creating thread %ld\n", 1);
    rc = pthread_create(&threads[0], NULL, ReadSomething, (void *)1);
    if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    printf("In main: creating thread %ld\n", 2);
    rc = pthread_create(&threads[1], NULL, WriteSomething, (void *)2);
    if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    pthread_exit(NULL);

return 0;

}
