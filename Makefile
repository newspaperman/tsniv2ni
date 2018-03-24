CXX ?= g++
NAME = tsniv2ni
OBJECTS = tsniv2ni.cpp
DESTDIR ?=

tsniv2ni: $(OBJECTS)
	$(CXX) -o $(NAME) $(OBJECTS)

clean:
	rm -f $(NAME) $(OBJECTS)

install:
	install -d $(DESTDIR)/usr/bin
	install -m 755 $(NAME) $(DESTDIR)/usr/bin
