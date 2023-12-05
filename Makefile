OBJS  = tema2.o 
OUT	  = quadtree
FLAGS = -c -Wall -g

build: $(OBJS)
	gcc $(FLAGS) tema2.c
	gcc $(OBJS) -o $(OUT)

clean:
	rm -f $(OBJS) $(OUT)