#include <iostream>
#include <chrono>
using namespace std;

int secuencia[14] = {};

void relleno(int iteraciones){
    for(int i = 0; i < iteraciones; i++){
        secuencia[i] = 7;
    }
}

void visualizar(){
    for(int i = 0; i < 14; i++){
        cout << secuencia[i] << endl;
        }
}

int randomNum()
{ 
  int next = 8;
         
  next = ((next * next) / 81 ) % 19189 ; 
  return next ; 
} 
 
int randomRange( int min, int max, int past_num )  
{ 
  return past_num % (max+1-min) + min ;  
}

void fill_rands(int max){
    int lb = 1, ub = 4;
    int random = (rand() % (ub - lb + 1)) + lb;
    // random = randomRange(1, 4, random);
    for(int i = 0; i < max; i++){
        secuencia[i] = random;
        random = (rand() % (ub - lb + 1)) + lb;
        //random = randomRange(1, 4, random);
  }
}

int get_rand(int lo, int hi) {
    auto moment = std::chrono::steady_clock::now().time_since_epoch().count();
    int num = moment % (hi - lo + 1);
    return num + lo;
}

void printRandoms(int lower, int upper,
                            int count)
{
    int i;
    for (i = 0; i < count; i++) {
        int num = (rand() %
        (upper - lower + 1)) + lower;
        printf("%d ", num);
    }
}

int main(){
    srand(getpt());
    visualizar();
    fill_rands(14);
    cout << "--------------------------" << endl;
    visualizar();
    //cout << get_rand(1,4) << endl;
    //printRandoms(1, 4,4);
   return 0;

}


