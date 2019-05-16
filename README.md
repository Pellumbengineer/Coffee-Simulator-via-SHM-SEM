# INTRODUCTION
In this project I have to do a coffee simulator for a new open shop. This is simply a producer/consumer problem and is solved by using Semaphore and Shared memory.We have 3 hired Baristas(Producer) and they are able to make at most 4 cop of cups of coffee per a period of time and we have clients(consumer) the total number is 50.They can consume at most 2 cups. Baristas and consumers have to use the same place for consummation and preparation of coffees(shared memory) which is 15 slots.

# EVALUATION PROCEDURE

We have 3 producer and 50 consumer. Our 3 producers produce coffees until all consumers have drank coffee. It can not produce if the slot in the shared memory is full, it has to wait for consumer to consume. Also the same for the consumer it consumes until there is something in the slots if not it has to wait for the producer to produce. But to synchronize this scenario it is not as simple as it seems. So I have used semaphores, two of them are binary semaphores (TO_PRODUCE and TO_CONSUME) to synchronize the access to the shared memory. And two counting semaphores one is for full slots and the other one is for empty slots. The initial values are TO_PRODUCE-1 , TO_CONSUME-0 , EMPTY-15 and FULL-0. 

Below I have tried to describe the main concept and scenario of this project.

![Screenshot from 2019-05-17 01-44-21](https://user-images.githubusercontent.com/26312757/57891899-c1bd5c80-7845-11e9-82e5-a4fea9c382b6.png)



The location of head will have the index of the next production to retrieve, while tail will have the index of where the next production should be stored.

TO_PRODUCE → 1
TO_CONSUME → 0
EMPTY      →15
FULL       → 0

Here we have our semaphores





When first producer comes it decrements the Producer semaphore which means no other producer or consumer can operate and then produces coffee it checks if slots are full if not it decrements the Empty semaphore and changes the tail if yes it signals directly the consumer. After it updates the tail it increments the Full semaphore. Now it is time for the consumer to consume by incrementing the Consume semaphore. It consumes and checks if the number of empty slots is less then 15 it decrements Full semaphore and changes the index of head, also increments the empty semaphore. And it is the turn for producer to produce and so on.
