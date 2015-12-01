CC = g++
NAME = tsniv2ni
OBJECTS = tsniv2ni.cpp

tsniv2ni: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS)
