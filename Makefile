#
#  Makefile
#
#  Author: Leandro Beretta <lea.beretta@gmail.com>
#
#  Informática I - UTN.BA
#  2013
#

LIBS = -lallegro -lallegro_main -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_dialog -I/usr/local/include -L/usr/local/lib
CFLAGS = -Wall

SRC = $(wildcard *.c)

mb: $(SRC)
	gcc -o bin/$@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f bin/mb

play:
	bin/mb
