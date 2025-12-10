#include <set>
#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
using namespace std;

typedef enum {
    BIURKO, WOREK, PRZEDMIOT
} type;


class object {
public:
    type typ;
    int nr;
    struct object* parent;
    set<struct object*> children;
    object(type typ, int nr, struct object* parent)
        : typ(typ), nr(nr), parent(parent) {}
};
typedef object worek;
typedef object przedmiot;

worek biurko(BIURKO, -1, nullptr);
int worki_counter = 0;

przedmiot* nowy_przedmiot(){
    przedmiot* res = new object(PRZEDMIOT, -1, &biurko);
    biurko.children.insert(res);
    return res;
}
worek *nowy_worek(){
    worek* res = new object(WOREK, worki_counter++, &biurko);
    biurko.children.insert(res);
    return res;
}

// Wkłada przedmiot co do worka gdzie.
// Założenie: co i gdzie leżą na biurku.
void wloz(przedmiot *co, worek *gdzie){
    biurko.children.erase(co);
    gdzie->children.insert(co);
    co->parent = gdzie;
}

/*
redefinition:
void wloz(worek *co, worek *gdzie);
*/

// Wyjmuje przedmiot p z worka i kładzie na biurku.
// Założenie: Przedmiot p był w worku leżącym na biurku.
void wyjmij(przedmiot *p){
    p->parent->children.erase(p);
    p->parent = &biurko;
    biurko.children.insert(p);
}

/*
// Wyjmuje worek w z worka i kładzie na biurku.
// Założenie: Worek w był w worku leżącym na biurku.
void wyjmij(worek *w);
*/

// Wynik: numer worka, w którym znajduje się przedmiot p (-1 jeśli na biurku).
int w_ktorym_worku(przedmiot *p){
    return p->parent->nr;
}
/*
// Wynik: numer worka, w którym znajduje się worek w (-1 jeśli na biurku).
int w_ktorym_worku(worek *w);
*/

// Wynik: liczba przedmiotów zawartych (bezpośrednio i pośrednio) w worku w
int ile_przedmiotow(worek *w){
    int cnt = 0;
    for(object* child : w->children){
        if(child->typ == PRZEDMIOT)cnt++;
        else cnt += ile_przedmiotow(child);
    }
    return cnt;
}

// Cała zawartość worka w ląduje na biurku, a wszystko, co poza workiem w
// znajdowało się bezpośrednio na biurku, ląduje wewnątrz worka w.
void na_odwrot(worek *w){
    biurko.children.erase(w);

    object kopia(w->typ, w->nr, w->parent);
    for(object* child : w->children){
        kopia.children.insert(child);
    }

    w->children.clear();
    for(object* child : biurko.children){
        w->children.insert(child);
        child->parent = w;
    }

    biurko.children.clear();
    for(object* child : kopia.children){
        biurko.children.insert(child);
        child->parent = &biurko;
    }

    biurko.children.insert(w);
}

// Kończy i zwalnia pamięć
void clean_dfs(object* obj){
    for(object* child : obj->children){
        clean_dfs(child);
    }
    if(obj != &biurko)
        delete obj;
}
void gotowe(){
    clean_dfs(&biurko);
}

int main(){
    object* przedmioty[60000];
    object* worki[60000];

    int nr_p = 0;
    int nr_w = 0;
    int n; //liczba operacji

    cin >> n;
    string input;
    int a, b;
    while(n--){
        cin >> input;
        if(input == "NOWY_PRZEDMIOT"){
            przedmioty[nr_p++] = nowy_przedmiot();
        }
        else if(input == "NOWY_WOREK"){
            worki[nr_w++] = nowy_worek();
        }
        else if(input == "WLOZ_PRZEDMIOT"){
            cin >> a >> b;
            assert(przedmioty[a]->parent == &biurko); 
            assert(worki[b]->parent == &biurko);
            wloz(przedmioty[a], worki[b]);
        }
        else if(input == "WLOZ_WOREK"){
            cin >> a >> b;
            assert(worki[a]->parent == &biurko);
            assert(worki[b]->parent == &biurko);
            wloz(worki[a], worki[b]);
        }
        else if(input == "WYJMIJ_PRZEDMIOT"){
            cin >> a;
            assert(przedmioty[a]->parent->parent == &biurko);
            wyjmij(przedmioty[a]);
        }
        else if(input == "WYJMIJ_WOREK"){
            cin >> a;
            assert(worki[a]->parent->parent == &biurko);
            wyjmij(worki[a]);
        }
        else if(input == "KTORY_PRZEDMIOT"){
            cin >> a;
            cout << w_ktorym_worku(przedmioty[a]) << '\n';
        }
        else if(input == "KTORY_WOREK"){
            cin >> a;
            cout << w_ktorym_worku(worki[a]) << '\n';
        }
        else if(input == "ILE"){
            cin >>  a;
            cout << ile_przedmiotow(worki[a]) << '\n';
        }
        else if(input == "NA_ODWROT"){
            cin >> a;
            assert(worki[a]->parent == &biurko);
            na_odwrot(worki[a]);
        }
        else {
            cout << "PROBLEM!!!\n";
            exit(EXIT_FAILURE);
        }
    }
    gotowe();
}

