# pthreads-pizza-delivery-system

## Purpose

This is an implementation of a pizza ordering and delivering system using the POSIX threads (pthreads) library for parallel processes execution. Customers place and pay for their orders, which then get prepared and delivered to them.

## Main characterists

- **Constants** - to define parameters
- **Variables** - to record sales, revenue and serving times
- **Mutexes** and **condition variables** - to synchronize threads and prevent race conditions
- **rand()** - to generate (pseudo)random numbers that influence various parts of the system
- **Statistics output** - to output a synopsis of sales and average serving times in the end of the simulation

## Functionality

We have a pizza store which makes two types of pizza, plain and special, and has an electronic orders and payments system. The system contains:
- **NCOOK** amount of pizza makers
- **NOVEN** amount of ovens
- **NPACKER** amount of employees that pack the pizzas and
- **NDELIVERER** delivery-drivers.

The first customer connects to the system at moment t=0, and each customer afterwards connects at a random integer time interval in the range [TORDERLOW, TORDERHIGH]. When a customer connects to the central orders system, he/she orders a random (integer) amount of pizzas in the range [NORDERLOW, NORDERHIGH]. Every single one of those has a **PPLAIN** probability to be plain, else, it's special. 

The system requires a random integer time interval in the range [**TPAYMENTLOW**, **TPAYMENTHIGH**] to charge the customer's credit card.

With **PFAIL** probability the credit card charging fails and the order is cancelled, else the order is registered, the store's revenus is increased by **CPLAIN** or **CSPECIAL** euros per pizza, depending on how many pizzas were ordered, and finally the amount of pizzas per type is updated. 

The registered order initially waits until a pizza maker is available so that all pizzas can be baked at the same time, because each over can only bake one pizza. When enough ovens are available, each pizza is placed in an onver and the pizza maker moves on to prepare the next order. Pizzas are baked for **TBAKE** time. When the baking is complete the ovens automatically turn off, and an employee is expected to take the pizzas out of the ovens and pack them in boxes, which takes **TPACK** time per pizza. At this moment the ovens are free. 

When a delivery driver becomes available, he receives a packed order which he/she delivers to the customer, with the delivery needed a randoom integer time interval in the range [**TDELLOW**, **TDELHIGH**]. Afte the delivery, the delivery driver needs the same exact amount to return to the store to pick up the next order. **It must be noted that** every pizza maker has to deal with one order at atime until the pizza is in the oven, the ovens are free iff the packing has been completed, and every delivery carries one and only one order per route.

## Inputs and data

_pizzeria.h_ is the header file where constants + variables are defined, as well as mutex initialization and conditional variables.

As CLI input we have exactly two parameters, the first one being **NCUST** (number of customers to be served) and a random **seed** (for the random number generator).

## Output

For each order, these are the possible scenarios:

- Order <order_id> failed/was accepted _(right when the order fails/is accepted)_
- Order <order_id> was prepared in X minutes _(right when the order packing is completed)_
- Order <order_id> was delivered in Y minutes _(right when the order is delievered)_

where 
- **X** = time from customer arrival to packaging
- **Y** = time from customer arrival to delivery

And in the very end of the execution, we get these outputs: 

- Total revenue, pizzas sold per type, and the number of successful and failed orders
- Average and maximum service time (arrival → delivery) _- only for successful orders_
- Average and maximum cooling time (it's the time from when the baking is over up until the moment it gets delivered) _- only for successful orders_

## Code structure

**customerThread** (main thread) creates a thread per customer/order (**NCUST** threads) to which a thread number is forwarded (**1** up to **NCUST**) so that it is used as an order number. Every thread will then execute the steps above up until the order is completed and the output is printed. Finally the main thread prints the output.




