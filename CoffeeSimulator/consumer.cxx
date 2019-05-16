/*

The CONSUMER

*/
#include <signal.h>
#include "local.h"
int main(int argc, char *argv[])
{
    int  semid, shmid;
    pid_t ppid = getppid();
    pid_t gid = getpgid(ppid);
    pid_t cosumerID = getpid();
    char *shmptr;
    struct MEMORY *memptr;

    // Check command line

    if (argc != 2)
    {
        cerr << argv[0] << " sleep_time" << endl;
        return 30;
    }
    // Access, attach & ref memory
    if ((shmid = shmget((int)ppid, 0, 0)) != -1)
    {
        if ((shmptr = (char *)shmat(shmid, (char *)0, 0)) == (char *)-1)
        {
            perror("shmat -- consumer -- attach");
            return 31;
        }
        memptr = (struct MEMORY *)shmptr;
    }
    else
    {
        perror("shmget -- consumer -- access");
        return 32;
    }
    // Access semaphore set
    if ((semid = semget((int)ppid, 4, 0)) == -1)
    {
        perror("semget -- consumer -- access ");
        return 33;
    }
    srand((unsigned)getpid());

    //------------------------------------------------------------------
    acquire.sem_num = TO_CONSUME;

    if (semop(semid, &acquire, 1) == -1)
    {
        perror("semop -- consumer -- acquire ");
        return 34;
    }
                                                                                    /*
                                                                                        CRITICAL REGION
                                                                                    */
    
    
    printf("\nConsumer pid-- %d \n", cosumerID);

    // Creat random number for consumer to specify how many coffe will drink 
    int consumationOfCoffes, r;
    consumationOfCoffes = rand() % 2 + 1;
    for (r = 0; r < consumationOfCoffes; r++){
        
        int numberOfEmptySlots = semctl(semid, 0, GETVAL, 0);
        cout << "Number of available slots : " << numberOfEmptySlots << endl;
        

        if (numberOfEmptySlots == 15){
            cout << "\nThere is no coffe prepared please wait.\n" << endl;
            break;
        }
        
        acquire.sem_num = FULL;
        if (semop(semid, &acquire, 1) == -1)
        {
            perror("semop -- FULL -- acquire ");
            return 51;
        }
        printf("Slot:[ %d ]  Consumer with id -> %d consumed %d. coffe \n", memptr->head, cosumerID, r+1);
        fflush(stdout);
        memptr->head = (memptr->head + 1) % N_SLOTS;

        release.sem_num = EMPTY;
        if (semop(semid, &release, 1) == -1)
        {
            perror("semop -- EMPTY -- release ");
            return 35;
        }
    }

    release.sem_num = TO_PRODUCE;
    if (semop(semid, &release, 1) == -1)
    {
        perror("semop -- consumer -- release ");
        return 50;
    }


    memptr->counter = memptr->counter+1;

    
   pause();

    //------------------------------------------------------------------
    return 0;
}