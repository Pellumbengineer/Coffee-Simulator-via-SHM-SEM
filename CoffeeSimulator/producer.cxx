/*

The PRODUCER ...
 */
#include "local.h"
int main(int argc, char *argv[])
{
    int i, r, generationOfCoffes, semid, shmid;
    pid_t ppid = getppid();
    pid_t producerID = getpid();
    char *shmptr;
    union semun arg;
    struct MEMORY *memptr;

    // Check command line

    if (argc != 2)
    {
        cerr << argv[0] << " sleep_time" << endl;
        return 20;
    }

    // Access, attach & ref mem

    if ((shmid = shmget((int)ppid, 0, 0)) != -1)
    {
        if ((shmptr = (char *)shmat(shmid, (char *)0, 0)) == (char *)-1)
        {
            perror("shmat -- producer -- attach ");
            return 21;
        }
        memptr = (struct MEMORY *)shmptr;
    }
    else
    {
        perror("shmget -- producer -- access ");
        return 22;
    }

    // Access semaphore set

    if ((semid = semget((int)ppid, 4, 0)) == -1)
    {
        perror("semget -- producer -- access ");
        return 23;
    }

    srand((unsigned)getpid());
    pid_t gid = getpgid(ppid);

    while (1)
    {
        acquire.sem_num = TO_PRODUCE;
        if (semop(semid, &acquire, 1) == -1)
        {
            perror("semop -- producer -- acquire ");
            return 52;
        }
                                                                                    /*
                                                                                        CRITICAL REGION
                                                                                    */
        
        
        printf("\nProducer pid-- %d \n", producerID);
        cout<< "Barista preparing coffes...\n"<<endl;

        sleep(8);

        generationOfCoffes = rand() % 4 + 1;
        for (r = 0; r < generationOfCoffes; r++) {
         int numberOfFullSlots = semctl(semid, 1, GETVAL, 0);
            

            if (numberOfFullSlots == 15) {
                break;
            }

            acquire.sem_num = EMPTY;
            if (semop(semid, &acquire, 1) == -1)
            {
                perror("semop -- EMPTY -- acquire ");
                return 24;
            }

            if (memptr->counter == 50){
                shmdt(shmptr);
                cout<<"Every think is done, BYE !!!"<< endl;
                kill(gid, SIGKILL);

                shmctl(shmid, IPC_RMID, (struct shmid_ds *)0);

                semctl(semid, 0, IPC_RMID, 0);

            }

            printf("Slot:[ %d ]  Barista with id -> %d produced %d. coffe \n", memptr->tail, producerID, r+1);
            fflush(stdout);
            memptr->tail = (memptr->tail + 1) % N_SLOTS;
            

            release.sem_num = FULL;
            if (semop(semid, &release, 1) == -1)
            {
                perror("semop -- FULL -- release ");
                return 53;
            }
        }

       

        release.sem_num = TO_CONSUME;
        if (semop(semid, &release, 1) == -1)
        {
            perror("semop -- producer -- release ");
            return 25;
        }

        sleep(2);
    }

    return 0;
}
