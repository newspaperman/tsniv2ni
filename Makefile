CXX ?= g++
NAME = tsniv2ni
OBJECTS = tsniv2ni.cpp

tsniv2ni: $(OBJECTS)
	$(CXX) -o $(NAME) $(OBJECTS)

clean:
    rm -f $(NAME) $(OBJECTS)
