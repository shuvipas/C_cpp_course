
CC = gcc
CCFLAGS = -std=c99 -Wall -Werror
MC_LL = markov_chain.o linked_list.o

tweets: tweets_generator.o $(MC_LL)
	$(CC) tweets_generator.o $(MC_LL) -o tweets_generator
snake: snakes_and_ladders.o $(MC_LL)
	$(CC) snakes_and_ladders.o $(MC_LL) -o snakes_and_ladders
tweets_generator.o:tweets_generator.c
	$(CC) $(CCFLAGS) -c tweets_generator.c -o tweets_generator.o
snakes_and_ladders.o: snakes_and_ladders.c
	$(CC) $(CCFLAGS) -c snakes_and_ladders.c -o snakes_and_ladders.o
linked_list.o: linked_list.c
	$(CC) $(CCFLAGS) -c linked_list.c -o linked_list.o
markov_chain.o: markov_chain.c
	$(CC) $(CCFLAGS) -c markov_chain.c -o markov_chain.o



