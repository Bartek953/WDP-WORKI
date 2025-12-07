#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include "worki.h"

//będę reprezentował znajdowanie się przedmiotu/worka w innym worku
//poprzez posiadanie przez dziecko wskaźnika na rodzica

//biurko to tak naprawdę "największy" worek, w ktorym znajdują się wszystkie inne
//samo biurko nie zawiera się w niczym (nullptr)

//poniewaz będę potrzebował zamieniać wskaźniki(funkcja na_odwrot)
//to worki/przedmioty to tak naprawdę wrappery trzymające wskaźniki do właściwych obiektow
//(mozna zauwazyć, ze przedmiot i worek są do siebie bardzo podobne)
//(przedmiot to tak naprawdę worek do ktorego nic nie wkladamy i ma licznik = 1)

//biurko jest zmienną globalną statyczną, inicjalizowaną w momencie wywołania pierwszej funkcji z biblioteki
//reszte wskaźnikow przechowuje w vectorach, by moc je poźniej posprzątać

static std::vector<obiekt*> zaalokowane_obiekty;
static std::vector<przedmiot*> zaalokowane_przedmioty;
static std::vector<worek*> zaalokowane_worki;

obiekt* nowy_obiekt(typ typ, obiekt* rodzic, int ile, int nr_worka){
    obiekt* wynik = new obiekt;
    assert(wynik != nullptr);
    zaalokowane_obiekty.push_back(wynik);

    wynik->typ = typ;
    wynik->rodzic = rodzic;
    wynik->licznik = ile;
    wynik->nr_worka = nr_worka;
    return wynik;
}

static int aktualny_nr = 0;

static worek biurko = {
    .element = nullptr
};

przedmiot *nowy_przedmiot(){
    if(biurko.element == nullptr){
        biurko.element = nowy_obiekt(BIURKO, nullptr, 0, -1);
        std::cerr<<"TWORZENIE BIURKA\n";
    }
    przedmiot* wynik = new przedmiot;
    assert(wynik != nullptr);
    zaalokowane_przedmioty.push_back(wynik);

    wynik->element = nowy_obiekt(PRZEDMIOT, biurko.element, 1, -1);
    biurko.element->licznik++;

    return wynik;
}

worek *nowy_worek(){
    if(biurko.element == nullptr){
        biurko.element = nowy_obiekt(BIURKO, nullptr, 0, -1);
        std::cerr<<"TWORZENIE BIURKA\n";
    }
    worek* wynik = new worek;
    assert(wynik != nullptr);
    zaalokowane_worki.push_back(wynik);

    wynik->element = nowy_obiekt(WOREK, biurko.element, 0, aktualny_nr++);

    return wynik;
}

// Wkłada przedmiot co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
//przepinamy rodzica "co" na element "gdzie"
//poniewaz "gdzie" lezy na biurku, to wystarczy zaktualizowac licznik "gdzie"
//(skoro oba elementy lezaly na biurku, to licznik biurka się nie zmienia)
void wloz(przedmiot *co, worek *gdzie){
    assert(co != nullptr && gdzie != nullptr && biurko.element !=nullptr);
    assert(co->element != nullptr && gdzie->element != nullptr);
    assert(gdzie->element->rodzic->typ == BIURKO);
    assert(co->element->rodzic->typ == BIURKO);

    co->element->rodzic = gdzie->element;
    gdzie->element->licznik++;
}

// Wkłada worek co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
//przepinamy rodzica "co" na element "gdzie"
//poniewaz "gdzie" lezy na biurku, to wystarczy zaktualizowac licznik "gdzie"
//(skoro oba elementy lezaly na biurku, to licznik biurka się nie zmienia)
void wloz(worek *co, worek *gdzie){
    assert(co != nullptr && gdzie != nullptr && biurko.element !=nullptr);
    assert(co->element != nullptr && gdzie->element != nullptr);
    assert(gdzie->element->rodzic->typ == BIURKO);
    assert(co->element->rodzic->typ == BIURKO);

    if(co == gdzie){
        return;
    }
    co->element->rodzic = gdzie->element;
    gdzie->element->licznik += co->element->licznik;
}

// Wyjmuje przedmiot p z worka i kładzie na biurku.
// Założenie: Przedmiot p był w worku leżącym na biurku.
//skoro worek-rodzic jest na biurku, to wystarczy zaktualizowac tylko go
//licznik biurka sie nie zmienia
void wyjmij(przedmiot *p){
    assert(p != nullptr && p->element != nullptr && biurko.element != nullptr);
    assert(p->element->rodzic->rodzic->typ == BIURKO);

    p->element->rodzic->licznik--;
    p->element->rodzic = biurko.element;
}

// Wyjmuje worek w z worka i kładzie na biurku.
// Założenie: Worek w był w worku leżącym na biurku.
//skoro worek-rodzic jest na biurku, to wystarczy zaktualizowac tylko go
//licznik biurka sie nie zmienia
void wyjmij(worek *w){
    assert(w != nullptr && w->element != nullptr && biurko.element != nullptr);
    assert(w->element->rodzic->rodzic->typ == BIURKO);

    w->element->rodzic->licznik -= w->element->licznik;
    w->element->rodzic = biurko.element;
}

// Wynik: numer worka, w którym znajduje się przedmiot p (-1 jeśli na biurku).
int w_ktorym_worku(przedmiot *p){
    assert(p != nullptr && p->element != nullptr && biurko.element != nullptr);

    return p->element->rodzic->nr_worka;
}

// Wynik: numer worka, w którym znajduje się worek w (-1 jeśli na biurku).
int w_ktorym_worku(worek *w){
    assert(w != nullptr && w->element != nullptr && biurko.element != nullptr);

    return w->element->rodzic->nr_worka;
}

// Wynik: liczba przedmiotów zawartych (bezpośrednio i pośrednio) w worku w
int ile_przedmiotow(worek *w){
    assert(w != nullptr && w->element != nullptr && biurko.element != nullptr);

    return w->element->licznik;
}

//Załozenie: worek w znajduje się bezpośrednio na biurku
// Cała zawartość worka w ląduje na biurku, a wszystko, co poza workiem w
// znajdowało się bezpośrednio na biurku, ląduje wewnątrz worka w.
//"zamiana wskaźnikow" w i biurka
void na_odwrot(worek *w){
    assert(w != nullptr && w->element != nullptr && biurko.element != nullptr);

    obiekt* stare_w = w->element;
    obiekt* stare_biurko = biurko.element;

    //B.licznik = W.licznik + R.licznik
    stare_biurko->licznik -= stare_w->licznik;
    //B.licznik = R.licznik
    stare_w->licznik += stare_biurko->licznik;
    //W.licznik = W.licznik + R.licznik

    stare_biurko->nr_worka = stare_w->nr_worka;
    stare_w->nr_worka = -1;

    stare_biurko->typ = WOREK;
    stare_w->typ = BIURKO;

    stare_biurko->rodzic = stare_w;
    stare_w->rodzic = nullptr;

    biurko.element = stare_w;
    w->element = stare_biurko;
}

void gotowe(){
    for(obiekt* el : zaalokowane_obiekty){
        delete el;
    }
    zaalokowane_obiekty.clear();

    for(przedmiot* el : zaalokowane_przedmioty){
        delete el;
    }
    zaalokowane_przedmioty.clear();

    for(worek* el : zaalokowane_worki){
        delete el;
    }
    zaalokowane_worki.clear();

    std::cerr << "POSPRZATANE\n";
}
