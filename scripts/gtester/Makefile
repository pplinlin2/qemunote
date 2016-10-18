SRC = test.c
BIN = test
OBJ = cal.o

%.o: %.c
	$(CC) -o $@ -c $^

all: $(OBJ)
	$(CC) $^ $(SRC) -o $(BIN) `pkg-config --cflags --libs glib-2.0`

clean:
	-rm $(OBJ) $(BIN)
