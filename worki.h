#ifndef WORKI_H
#define WORKI_H

typedef enum typ {
    BIURKO, WOREK, PRZEDMIOT
} typ;

typedef struct obiekt {
    typ typ;
    struct obiekt* rodzic;
    int licznik;
    int nr_worka;
} obiekt;

obiekt* nowy_obiekt(typ typ, obiekt* rodzic, int ile, int nr_worka);

// Należy wypełnić
struct przedmiot {
    obiekt* element;
};

// Należy wypełnić
struct worek {
    obiekt* element;
};

// Nowy przedmiot na biurku
przedmiot *nowy_przedmiot();

// Nowy worek na biurku; otrzymuje kolejny numer, począwszy od 0.
worek *nowy_worek();

// Wkłada przedmiot co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(przedmiot *co, worek *gdzie);

// Wkłada worek co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(worek *co, worek *gdzie);

// Wyjmuje przedmiot p z worka i kładzie na biurku.
// Założenie: Przedmiot p był w worku leżącym na biurku.
void wyjmij(przedmiot *p);

// Wyjmuje worek w z worka i kładzie na biurku.
// Założenie: Worek w był w worku leżącym na biurku.
void wyjmij(worek *w);

// Wynik: numer worka, w którym znajduje się przedmiot p (-1 jeśli na biurku).
int w_ktorym_worku(przedmiot *p);

// Wynik: numer worka, w którym znajduje się worek w (-1 jeśli na biurku).
int w_ktorym_worku(worek *w);

// Wynik: liczba przedmiotów zawartych (bezpośrednio i pośrednio) w worku w
int ile_przedmiotow(worek *w);

// Cała zawartość worka w ląduje na biurku, a wszystko, co poza workiem w
// znajdowało się bezpośrednio na biurku, ląduje wewnątrz worka w.
void na_odwrot(worek *w);

// Kończy i zwalnia pamięć
void gotowe();

#endif
