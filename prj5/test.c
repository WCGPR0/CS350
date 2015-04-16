#include <pthread.h>
#include "time_functions.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


sem_t mutex;

int main() {

sem_init(&mutex, 0, 1);

printf("Works\n");
return 0;
}
