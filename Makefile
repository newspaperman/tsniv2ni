CXX ?= g++
NAME = tsniv2ni
OBJECTS = tsniv2ni.o
DESTDIR ?=

tsniv2ni: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(NAME) $(OBJECTS)

clean:
	rm -f $(NAME) $(OBJECTS)

install:
	install -d $(DESTDIR)/usr/bin
	install -m 755 $(NAME) $(DESTDIR)/usr/bin
