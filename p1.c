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
    int fd[2],s=1,e=1,i=0,shm;

    shm=shm_open("/shm",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);

    int p;

    s1=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);
    s2=(sem_t*)mmap(NULL,sizeof(sem_t),PROT_READ|PROT_WRITE,MAP_SHARED,shm,0);
    
    sem_init(s1,0,1);
    sem_init(s2,0,1);

    p=open("dictionary.txt",O_RDONLY);
    pipe(fd);

    if(p==-1)
    {
        perror("Error :");
        return 0;
    }
    else
    {
        while(s)
        {
            char buff[128]={};
            i=0;

            s=read(p,&buff[0],1);
            if(buff[0]=='m')
            {
                sem_wait(s1);
                while(buff[i]!='\n')
                {
                    read(p,buff,128);
                    i++;
                }
                write(fd[1],buff,sizeof(buff));
                sem_post(s2);
            }
        }
    }

    sem_destroy(s1);
    sem_destroy(s2);

    close(p);
}