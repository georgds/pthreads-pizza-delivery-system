#pragma once
#define NCOOK 2
#define NOVEN 15
#define NPACKER 2
#define NDELIVERER 10
#define TORDERLOW 1
#define TORDERHIGH 3
#define NORDERLOW 1
#define NORDERHIGH 5
#define PPLAIN 0.6
#define TPAYMENTLOW 1
#define TPAYMENTHIGH 3
#define PFAIL 0.1
#define CPLAIN 10
#define CSPECIAL 12
#define TPREP 1
#define TBAKE 10
#define TPACK 1
#define TDELLOW 5
#define TDELHIGH 15
#define CLOCK_REALTIME 0 

int plainPizzaSales = 0;
int specialPizzaSales = 0;
int successfulPayments = 0;
int notSuccessfulPayments = 0;
int revenue = 0;
int maxServiceTime = INT_MIN;
int maxCoolingTime = INT_MIN;
int availableOvens = NOVEN;
int avgServiceTime;
int avgCoolingTime;
int sumServiceTime;
int sumCoolingTime;

pthread_mutex_t cookMutex[NCOOK];
pthread_mutex_t packerMutex[NPACKER];
pthread_mutex_t deliveryMutex[NDELIVERER];
pthread_mutex_t salesMutex;
pthread_mutex_t orderMutex;
pthread_mutex_t paymentMutex;

pthread_cond_t ovenAvailableCond;
pthread_cond_t cookAvailableCond;
pthread_cond_t packerAvailableCond;
pthread_cond_t deliveryAvailableCond;

struct timespec beginOrder, beginCool, endOrder, endPrep, endCool;