# makefile
# project telepantin
# prerequisites : cairo / gtk4
#  for osx : 
#  	- brew install cairo
# 	- brew install gtk+4


# The wildcard function is part of Make syntax. 
SRC:=$(wildcard *.c) # c files
HDR:=$(wildcard *.h) # headers
OBJ:=$(program_C_SRCS:.c=.o) 
PROG= app # app name
CC = gcc
CFLAGS = -Wall -O2 
# cairo flags already included in GTK
# CFLAGS += $(shell pkg-config --cflags cairo)
# LIBS += $(shell pkg-config --libs cairo)
CFLAGS += $(shell pkg-config --cflags gtk4)
LIBS += $(shell pkg-config --libs gtk4)

# top-level rule, to compile everything.
default: $(PROG)

$(PROG): $(SRC) $(HDR)
#	gcc $(SRC) -o $(PROG)  `/usr/local/bin/pkg-config --cflags --libs cairo` 
	$(CC) $(SRC) -o $(PROG)  $(CFLAGS) $(LIBS)

run: $(PROG)
	./$(PROG)
#	gs -sDEVICE=jpeg -o out-%d.jpg out.ps

.PHONY: clean
clean:
	# the - in front of the command makes sure that make ignores a non-zero return code
	-rm app
	-rm ./png/*.png
	-rm ./mp4/*.mp4
	-rm ./gif/*.gif
	-rm -d ./png/
	-rm -d ./mp4/
	-rm -d ./gif/

.PHONY: gif
gif:
	-mkdir gif
	-rm ./gif/out.gif
	ffmpeg -framerate 24 -pattern_type glob -i './png/*.png' ./gif/out.gif


.PHONY: mp4	
mp4:
	-mkdir mp4
	-rm ./mp4/out.mp4
	ffmpeg -framerate 24 -pattern_type glob -i './png/*.png' -c:v libx264 -c:a copy -shortest -r 30 -pix_fmt yuv420p ./mp4/out.mp4

