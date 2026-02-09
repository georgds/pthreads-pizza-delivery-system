# pthreads-pizza-delivery-system

## Purpose

This is an implementation of a pizza ordering and delivering system using the POSIX threads (pthreads) library for parallel processes execution. Customers place and pay for their orders, which then get prepared and delievered to them.

## Functionality

We have a pizza store which makes two types of pizza; plain and special. It also has an electronic orders and payments system, NCOOK amount of pizza makers, NOVEN amount of ovens, NPACKER amount of employees that pack the pizzas in boxes, and NDELIVERER delievery-drivers.

The first customer connects to the system at t=0, and each customer after him connects at a random integer time interval in the range [TORDERLOW, TORDERHIGH]. When a customer connects to the central orders system, he/she orders a random (integerr) amount of pizzas in the range [NORDERLOW, NORDERHIGH]. Every single one of those has a PPLAIN probability to be plain, else it's special. 

The system requires a random integer time interval in the range [TPAYMENTLOW, TPAYMENTHIGH] to charge the customer's credit card.

With PFAIL probability the credit card charging fails and the order is cancelled, else the order is registered, the store's revenus is increased by CPLAIN or CSPECIAL euros per pizza, depending on how many pizzas were ordered, and finally the amount of pizzas per type is updated. 

The registered order initially waits until a pizza maker is available so that all pizzas can be baked at the same time, because each over can only bake one pizza. When enough ovens are available, each pizza is placed in an onver and the pizza maker moves on to prepare the next order. 

The pizzas are baked for TBAKE time, and when the baking is complete the ovens automatically shut off, and an employee is expected to take the pizzas out of the ovens and pack them in boxes, which takes TPACK time per pizza. At this moment the ovens are free. 

When a delievery driver becomes available, he receives a packed order which he/she delievers to the customer, with the delievery needed a randoom integer time interval in the range [TDELLOW, TDELHIGH]. Afte the delievery, the delievery driver needs the same exact amount to return to the store to pick up the next order. 


There are limited points of service and so to prevent race conditions mutual, exclusion (mutexes) 
