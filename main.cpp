#include "worki.h"
#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

const int Asize = 10000;


int main(){
    przedmiot* przedmioty[Asize];
    worek* worki[Asize];

    int nr_p = 0;
    int nr_w = 0;
    int n; //liczba operacji

    cin >> n;
    assert(n < Asize);
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
            //assert(przedmioty[a]->parent == &biurko);
            //assert(worki[a]->parent == &biurko);
            wloz(przedmioty[a], worki[b]);
        }
        else if(input == "WLOZ_WOREK"){
            cin >> a >> b;
            wloz(worki[a], worki[b]);
        }
        else if(input == "WYJMIJ_PRZEDMIOT"){
            cin >> a;
            wyjmij(przedmioty[a]);
        }
        else if(input == "WYJMIJ_WOREK"){
            cin >> a;
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
            na_odwrot(worki[a]);
        }
        else {
            cout << "PROBLEM!!!\n";
            exit(EXIT_FAILURE);
        }
    }
    gotowe();


    return 0;
}