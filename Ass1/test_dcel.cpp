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
    PhysicalVector physicalVector(1,0);
    double x,y;
    for(auto &v : vlist){
        physicalVector.getXY(x,y);
        v = new Vertex(roundTo(x),roundTo(y));
        physicalVector.rotate(2*PI/N);
    }
    return vlist;
}
vector<Vertex*> fill2(int N){
    vector<Vertex*> vlist(N,nullptr);
    PhysicalVector physicalVector(-1,0);
    double x,y;
    for(auto &v : vlist){
        physicalVector.getXY(x,y);
        v = new Vertex(roundTo(x),roundTo(y));
        physicalVector.rotate(2*PI/N);
    }
    return vlist;
}

void sortCCW(vector<Vertex*> &vlist){
    Vertex* cent = Vertex::centroid(vlist);
    sort(vlist.begin(),vlist.end(),[&cent](Vertex* a, Vertex* b)->bool{
        PhysicalVector va(toCoord(cent),toCoord(a)), vb(toCoord(cent),toCoord(b));
        double aa = va.getAngle(), ba = vb.getAngle();
        return (aa==ba?va.getMagnitude()<vb.getMagnitude():aa<ba);
    });
}

void printVlist(vector<Vertex*> &vlist, bool centAng = true){
    Vertex* cent = Vertex::centroid(vlist);
    cout << "{ ";
    for(auto v : vlist){
        cout << "(" << v->x << "," << v->y << ")@";
        if(centAng) 
            cout << PhysicalVector::toDegree((PhysicalVector(toCoord(cent),toCoord(v))).getAngle()) << " ";
    }
    delete cent;
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
        cout << PhysicalVector::toDegree(PhysicalVector(toCoord(v)).getAngle()) << " : ";
        cout << v->x << " " << v->y << endl;
    });
    cout << endl;

    PhysicalVector physicalVectors[] = {
        PhysicalVector(0,1),
        PhysicalVector(0,1,1,2),
        PhysicalVector(-1,0),
        PhysicalVector(0,-1),
        PhysicalVector(-1,-1)
    };
    for(auto physicalVector : physicalVectors){
        cout << physicalVector.getMagnitude() << "@" << PhysicalVector::toDegree(physicalVector.getAngle()) << ", ";
    }
    cout << endl;
    
    double angles[] = {
        PhysicalVector::angleBetween(PhysicalVector(1,1,0,0),PhysicalVector(-1,0)),
        PhysicalVector::angleBetween(-1,-1,0,0,1,1),
        PhysicalVector::angleBetween(0,-1,1,0,0,1),
        PhysicalVector::angleBetween(0,1,0,0,1,1),
        PhysicalVector::angleBetween(0,-1,0,1,0,0)
    };
    for(auto angle : angles){
        cout << PhysicalVector::toDegree(angle) << ", ";
    }
    cout << endl;

    return 0;
}