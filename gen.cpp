#include <set>
#include <vector>
#include <iostream>
#include <assert.h>
#include <string>
#include <unistd.h>
#include <random>
using namespace std;


const int MAX_N = 9500;

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

typedef enum {
    NOWY_PRZEDMIOT,
    NOWY_WOREK,
    WLOZ_PRZEDMIOT,
    WLOZ_WOREK,
    WYJMIJ_PRZEDMIOT,
    WYJMIJ_WOREK,
    KTORY_PRZEDMIOT,
    KTORY_WOREK,
    ILE,
    NA_ODWROT
} op_type;

int main(){
    srand(getpid());
    object* przedmioty[10000];
    object* worki[10000];

    assert(MAX_N < 10000);

    int nr_p = 0;
    int nr_w = 0;
    int n = 1 + rand() % MAX_N;

    cout << n << '\n';

    int op_realized = 0;

    bool ost_p_wloz = true;
    bool ost_w_wloz = true;

    while (op_realized < n){
        int action = rand() % (NA_ODWROT + 1);

        if(action == NOWY_PRZEDMIOT){
            cout << "NOWY_PRZEDMIOT\n";
            przedmioty[nr_p++] = nowy_przedmiot();
            op_realized++;
            ost_p_wloz = false;
        }
        else if(action == NOWY_WOREK){
            cout << "NOWY_WOREK\n";
            worki[nr_w++] = nowy_worek();
            op_realized++;
            ost_w_wloz = false;
        }
        else if(action == WLOZ_PRZEDMIOT && !ost_p_wloz && nr_w > 1 && nr_p > 1){
            ost_p_wloz = true;
            int w = rand() % nr_w;
            if(przedmioty[nr_p - 1]->parent == &biurko && worki[w]->parent == &biurko){
                cout << "WLOZ_PRZEDMIOT " << (nr_p - 1) << ' ' << w << '\n';
                wloz(przedmioty[nr_p-1], worki[w]);
                op_realized++;
            }
        }
        else if(action == WLOZ_WOREK && !ost_w_wloz && nr_w > 1 && nr_p > 1){
            ost_w_wloz = true;
            int w = rand() % (nr_w - 1);
            if(worki[nr_w-1]->parent == &biurko && worki[w]->parent == &biurko){
                cout << "WLOZ_WOREK " << (nr_w - 1) << ' ' << w << '\n';
                wloz(worki[nr_w-1], worki[w]);
                op_realized++;
            }
        }
        else if(action == WYJMIJ_PRZEDMIOT && nr_p > 0){
            int p = rand()% nr_p;
            if(przedmioty[p]->parent->parent == &biurko){
                cout << "WYJMIJ_PRZEDMIOT " << p << '\n';
                wyjmij(przedmioty[p]);
                op_realized++;
            }
        }
        else if(action == WYJMIJ_WOREK && nr_w > 0){
            int w = rand() % nr_w;
            if(worki[w]->parent->parent == &biurko){
                cout << "WYJMIJ_WOREK " << w << '\n';
                wyjmij(worki[w]);
                op_realized++;
            }
        }
        else if(action == KTORY_PRZEDMIOT && nr_p > 0){
            int p = rand()% nr_p;
            cout << "KTORY_PRZEDMIOT " << p << '\n';
            op_realized++;
        }
        else if(action == KTORY_WOREK && nr_w > 0){
            int w = rand()% nr_w;
            cout << "KTORY_WOREK " << w << '\n';
            op_realized++;
        }
        else if(action == ILE && nr_w > 0){
            int w = rand()% nr_w;
            cout << "ILE " << w << '\n';
            op_realized++;
        }
        else if(action == NA_ODWROT && nr_w > 0){
            int w = rand()%nr_w;
            if(worki[w]->parent == &biurko){
                cout << "NA_ODWROT " << w << '\n';
                na_odwrot(worki[w]);
                op_realized++;
            }
        }
    }

}