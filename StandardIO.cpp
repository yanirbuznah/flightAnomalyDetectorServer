//
// Created by yanir on 31/12/2020.
//

#include "StandardIO.h"
void StandardIO::write(string text){
    cout << text ;
}
string StandardIO::read(){
    string text;
    cin >> text;
    return text;
}
void StandardIO::write(float f){
    cout << f ;
}
void StandardIO::read(float* f){

}