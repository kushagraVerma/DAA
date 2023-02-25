#include <iostream>
#include <algorithm>
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
vector<Vertex*> fill2(int N){
    vector<Vertex*> vlist(N,nullptr);
    PVect pv(-1,0);
    double x,y;
    for(auto &v : vlist){
        pv.getXY(x,y);
        v = new Vertex(roundTo(x),roundTo(y));
        pv.rotate(2*PI/N);
    }
    return vlist;
}

void sortCCW(vector<Vertex*> &vlist){
    Vertex* cent = Vertex::centroid(vlist);
    sort(vlist.begin(),vlist.end(),[&cent](Vertex* a, Vertex* b)->bool{
        PVect va(toCoord(cent),toCoord(a)), vb(toCoord(cent),toCoord(b));
        double aa = va.getA(), ba = vb.getA();
        return (aa==ba?va.getM()<vb.getM():aa<ba);
    });
}

void printVlist(vector<Vertex*> &vlist, bool centAng = true){
    Vertex* cent = Vertex::centroid(vlist);
    cout << "{ ";
    for(auto v : vlist){
        cout << "(" << v->x << "," << v->y << ")@";
        if(centAng) 
            cout << PVect::toDeg((PVect(toCoord(cent),toCoord(v))).getA()) << " ";
    }
    cout << "}" << endl;
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

    vector<Vertex*> vlist(fill2(N_PTS));
    printVlist(vlist);
    sortCCW(vlist);
    printVlist(vlist);
    cout << endl;

    DCEL dcel(vlist);
    dcel.foreachVert([](Vertex* v){
        cout << PVect::toDeg(PVect(toCoord(v)).getA()) << " : ";
        cout << v->x << " " << v->y << endl;
    });
    cout << endl;

    PVect pvs[] = {
        PVect(0,1),
        PVect(0,1,1,2),
        PVect(-1,0),
        PVect(0,-1),
        PVect(-1,-1)
    };
    for(auto pv : pvs){
        cout << pv.getM() << "@" << PVect::toDeg(pv.getA()) << ", ";
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