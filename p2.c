#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<semaphore.h>
#include<sys/mman.h>

sem_t *s1,*s2;

int main(int argc, char const *argv[])
{
    char txt[128]={};
    int fd[2],shm;
    
    shm=shm_open("/shm",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);

    s1=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);
    s2=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);

    sem_init(s1,0,1);
    sem_init(s2,0,1);
    
    pipe(fd);


    sem_wait(s2);
    read(fd[0],txt,sizeof(txt));
    printf("%s\n",txt);
    sem_post(s1);

    sem_destroy(s1);
    sem_destroy(s2);
}