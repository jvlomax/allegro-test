all: app

sheep.o:
	g++ platformer.cpp -I/usr/include/allegro5 -L/usr/lib -lallegro -lallegro_image -lallegro_dialog -lallegro_ttf -lallegro_font
beaker.o:
	g++ -o test.cpp -I/usr/include/allegro5 -L/usr/lib -lallegro -lallegro_image -lallegro_dialog -lallegro_ttf -lallegro_font
app:
	g++ -o main.cpp -I/usr/include/allegro5 -L/usr/lib -lallegro -lallegro_image -lallegro_dialog -lallegro_ttf -lallegro_font

clean:
	rm *.out *.o