#include <bits/stdc++.h>

using namespace std;

int main(){
    int r;
    int *quantity;
    cin >> r;
    quantity = new int[r];
    for ( int i = 0 ; i < r ; ++i){
        cin >> quantity[i];
    }

    // while (1) {
        
    // }
    for ( int i = 0 ; i < r ; ++i){
        cout << quantity[i];
    }
    delete quantity;
    return 0;
}