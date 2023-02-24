#include <iostream>
#include "dcel.hpp"
#include "physvector.hpp"

using namespace std;

#define N_PTS 12
#define PREC 10

double roundTo(double x, int exp = PREC){
    double p = pow((double)10.0,(double)exp);
    return round(x*p)/p;
}

vector<Vertex*> fill1(int N){
    vector<Vertex*> vlist(N,nullptr);
    PVect pv(1,0);
    double x,y;
    for(auto &v : vlist){
        pv.getXY(x,y);
        v = new Vertex(roundTo(x),roundTo(y));
        pv.rotate(2*PI/N);
    }
    return vlist;
}

int main(){
    cout << "Common vals:" << endl;
    cout << "> PI/2=" << PI/2 << endl;
    cout << "> PI=" << PI << endl;
    cout << "> 3*PI/2=" << 3*PI/2 << endl;
    cout << "> 2*PI=" << 2*PI << endl;
    cout << "> sqrt(2)=" << sqrt(2) << endl;
    cout << "> sqrt(3)/2=" << sqrt(3)/2 << endl;
    cout << "> 1/sqrt(2)=" << 1/sqrt(2) << endl;
    cout << endl;

    DCEL dcel(fill1(N_PTS));
    dcel.foreachVert([](Vertex* v){
        cout << PVect::toDeg(PVect(toCoord(v)).heading()) << " : ";
        cout << v->x << " " << v->y << endl;
    });
    cout << endl;

    PVect pvs[] = {PVect(0,1),PVect(0,1,1,2),PVect(-1,0),PVect(0,-1),PVect(-1,-1)};
    for(auto pv : pvs){
        cout << pv.mag() << "@" << PVect::toDeg(pv.heading()) << ", ";
    }
    cout << endl;
    
    double angles[] = {
        PVect::angleBetween(PVect(1,1,0,0),PVect(-1,0)),
        PVect::angleBetween(-1,-1,0,0,1,1),
        PVect::angleBetween(0,-1,1,0,0,1),
        PVect::angleBetween(0,1,0,0,1,1),
        PVect::angleBetween(0,-1,0,1,0,0)
    };
    for(auto angle : angles){
        cout << PVect::toDeg(angle) << ", ";
    }
    cout << endl;

    return 0;
}