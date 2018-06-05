default: init

init: init.c kons
	gcc -o init init.c -lrt

kons: kons.c prod
	gcc -o kons kons.c -lrt

prod: prod.c
	gcc -o prod prod.c -lrt
