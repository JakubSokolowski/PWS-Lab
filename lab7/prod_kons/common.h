#include <sys/mman.h>
#include <semaphore.h>

#define BSIZE 10   // Rozmiar bufora
#define LSIZE 160   //  Dlugosc linii

typedef struct
{
	    int head; // Tutaj producent wstawia nowy element
	    int tail; // Stąd pobiera element konsument
	    int cnt;  // Liczba elementów w buforze
	    sem_t mutex; // Semafor chroniący sekcję krytyczną
	    sem_t empty; // Semafor wstrzymujący producenta
	    sem_t full;  // Semafor wstrzymujący konsumenta
	    char buf[BSIZE][LSIZE];
} bufor_t;
