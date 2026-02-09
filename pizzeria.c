#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include "pizzeria.h"

void* customerThread(void* arg) {
    int customerID = *((int*)arg);
    int orderSize = rand() % (NORDERHIGH - NORDERLOW + 1) + NORDERLOW;
    int plainCount = 0;
    int pos, prepTime, orderTime, coolingTime, i;
    if (customerID != 1) {
        sleep(rand() % (TORDERHIGH - TORDERLOW + 1) + TORDERLOW);
    }
    clock_gettime(CLOCK_REALTIME, &beginOrder);
    for (i = 0; i < orderSize; i++) {
        double p = (double)rand() / RAND_MAX;
        if (p < PPLAIN) {
            plainCount++;
        }
    }

    int paymentTime = rand() % (TPAYMENTHIGH - TPAYMENTLOW + 1) + TPAYMENTLOW;
    int failPayment = (double)rand() / RAND_MAX < (PFAIL);

    pthread_mutex_lock(&paymentMutex);
    sleep(paymentTime);
    if (failPayment) {
        printf("Customer %d payment failed. Order canceled.\n", customerID);
        pthread_mutex_unlock(&paymentMutex);
        notSuccessfulPayments++;
        pthread_exit(NULL);
    }
    else {
        successfulPayments++;
        pthread_mutex_lock(&salesMutex);
        revenue += (plainCount * CPLAIN) + ((orderSize - plainCount) * CSPECIAL);
        plainPizzaSales += plainCount;
        specialPizzaSales += (orderSize - plainCount);

        pthread_mutex_unlock(&salesMutex);
        pthread_mutex_unlock(&paymentMutex);
    }

    printf("Customer %d placed an order: %d pizzas (%d plain, %d special).\n", customerID, orderSize, plainCount, orderSize - plainCount);

    // Wait for a cook to become available
    pthread_mutex_lock(&orderMutex);
    i = 0;
    while (i < NCOOK) {
        if (pthread_mutex_trylock(&cookMutex[i]) == 0) {
            pos = i;
            //cook found
            pthread_mutex_unlock(&cookMutex[pos]);
            break;
        }
        else {
            if (i == NCOOK - 1) {
                pthread_cond_wait(&cookAvailableCond, &orderMutex);
                //No cook available yet,waiting...
                i = -1;
            }
            i++;
        }
    }
    pthread_mutex_unlock(&orderMutex);
    pthread_mutex_lock(&cookMutex[pos]);
    //Customer's order is being prepared by a cook.
    sleep(orderSize * TPREP);
    // Wait for enough ovens to become available	
    pthread_mutex_lock(&orderMutex);
    while (availableOvens - orderSize < 0) {
        //Customer's order is waiting for enough ovens to become available.
        pthread_cond_wait(&ovenAvailableCond, &orderMutex);
    }
    availableOvens -= orderSize;
    pthread_mutex_unlock(&orderMutex);

    //Customer's order is being baked in the oven.
    sleep(TBAKE);
    pthread_mutex_unlock(&cookMutex[pos]);
    pthread_cond_signal(&cookAvailableCond);

    //Customer is waiting for a packer to become available.
    pthread_mutex_lock(&orderMutex);
    clock_gettime(CLOCK_REALTIME, &beginCool);
    i = 0;
    while (i < NPACKER) {
        if (pthread_mutex_trylock(&packerMutex[i]) == 0) {
            pos = i;
            //packer found
            pthread_mutex_unlock(&packerMutex[pos]);
            break;
        }
        else {
            if (i == NPACKER - 1) {
                pthread_cond_wait(&packerAvailableCond, &orderMutex);
                //No packer available yet,waiting...
                i = -1;
            }
            i++;
        }
    }
    pthread_mutex_unlock(&orderMutex);
    pthread_mutex_lock(&packerMutex[pos]);
    //Customer's order is being packed by a packer.
    sleep(orderSize * TPACK);
    availableOvens += orderSize;
    pthread_cond_signal(&ovenAvailableCond);
    pthread_mutex_unlock(&packerMutex[pos]);
    pthread_cond_signal(&packerAvailableCond);
    clock_gettime(CLOCK_REALTIME, &endPrep);
    prepTime = endPrep.tv_sec - beginOrder.tv_sec;		//upologizei to xrono proetoimasias

    printf("Customer %d's order is done in : %d minutes.\n", customerID, prepTime);
    pthread_mutex_lock(&orderMutex);
    i = 0;
    while (i < NDELIVERER) {
        if (pthread_mutex_trylock(&deliveryMutex[i]) == 0) {
            pos = i;
            //deliverer found 
            pthread_mutex_unlock(&deliveryMutex[pos]);
            break;
        }
        else {
            if (i == NDELIVERER - 1) {
                // Wait for a delivery person to become available
                pthread_cond_wait(&deliveryAvailableCond, &orderMutex);
                //No deliverer available yet,waiting..
                i = -1;
            }
            i++;
        }
    }
    pthread_mutex_unlock(&orderMutex);
    pthread_mutex_lock(&deliveryMutex[pos]);
    //"Customer's order is out for delivery.
    sleep(rand() % (TDELHIGH - TDELLOW + 1) + TDELLOW);
    pthread_mutex_unlock(&deliveryMutex[pos]);
    clock_gettime(CLOCK_REALTIME, &endOrder);
    pthread_cond_signal(&deliveryAvailableCond);
    coolingTime = endOrder.tv_sec - beginCool.tv_sec;		//upologizei to xrono krywmatos tis pizzas
    sumCoolingTime += coolingTime;
    orderTime = endOrder.tv_sec - beginOrder.tv_sec;		//upologizei to xrono olokolirwsis tis paraggelias
    printf("Customer %d received the order. Order completed in : %d minutes.\n", customerID, orderTime);
    sumServiceTime += orderTime;

    if (coolingTime > maxCoolingTime) {
        maxCoolingTime = coolingTime;
    }

    if (orderTime > maxServiceTime) {
        maxServiceTime = orderTime;
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int Ncust;
    int seed;
    pthread_cond_init(&ovenAvailableCond, NULL);
    pthread_cond_init(&cookAvailableCond, NULL);
    pthread_cond_init(&packerAvailableCond, NULL);
    pthread_cond_init(&deliveryAvailableCond, NULL);
    pthread_mutex_init(&orderMutex, NULL);
    pthread_mutex_init(&paymentMutex, NULL);
    pthread_mutex_init(&salesMutex, NULL);
    for (int i = 0; i < NDELIVERER; i++) {
        pthread_mutex_init(&deliveryMutex[i], NULL);
    }
    for (int i = 0; i < NCOOK; i++) {
        pthread_mutex_init(&cookMutex[i], NULL);
    }
    for (int i = 0; i < NPACKER; i++) {
        pthread_mutex_init(&packerMutex[i], NULL);
    }

    if (argc != 3) {
        printf("Usage: %s <number_of_customers> <random_seed>\n", argv[0]);
        return 1;
    }

    Ncust = atoi(argv[1]);
    seed = atoi(argv[2]);

    srand(seed);

    pthread_t customerThreads[Ncust];
    int* customerIDs = malloc(Ncust * sizeof(int));

    for (int i = 0; i < Ncust; i++) {
        customerIDs[i] = i + 1;
        pthread_create(&customerThreads[i], NULL, customerThread, (void*)&customerIDs[i]);
    }

    for (int i = 0; i < Ncust; i++) {
        pthread_join(customerThreads[i], NULL);
    }
    avgServiceTime = sumServiceTime / successfulPayments;
    avgCoolingTime = sumCoolingTime / successfulPayments;
    printf("Plain pizza sales: %d\n", plainPizzaSales);
    printf("Special pizza sales: %d\n", specialPizzaSales);
    printf("Successful Payments: %d\n", successfulPayments);
    printf("Unuccessful Payments: %d\n", notSuccessfulPayments);
    printf("Average service time: %d minutes\n", avgServiceTime);
    printf("Average cooling time: %d minutes\n", avgCoolingTime);
    printf("Max service time: %d minutes\n", maxServiceTime);
    printf("Max cooling time: %d minutes\n", maxCoolingTime);
    printf("Total revenue: %d euros\n", revenue);
    pthread_cond_destroy(&ovenAvailableCond);
    pthread_cond_destroy(&cookAvailableCond);
    pthread_cond_destroy(&packerAvailableCond);
    pthread_cond_destroy(&deliveryAvailableCond);
    for (int i = 0; i < NCOOK; i++) {
        pthread_mutex_destroy(&cookMutex[i]);
    }
    for (int i = 0; i < NPACKER; i++) {
        pthread_mutex_destroy(&packerMutex[i]);
    }
    for (int i = 0; i < NDELIVERER; i++) {
        pthread_mutex_destroy(&deliveryMutex[i]);
    }
    pthread_mutex_destroy(&orderMutex);
    pthread_mutex_destroy(&paymentMutex);
    pthread_mutex_destroy(&salesMutex);
    free(customerIDs);
    return 0;
}
