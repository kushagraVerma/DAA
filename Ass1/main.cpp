#include <bits/stdc++.h>
#include "dcel.hpp"
#include "physvector.hpp"

using namespace std;

double area(double x1, double y1, double x2, double y2, double x3, double y3) {
    return 0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
}

bool isInterior(Vertex* p, Vertex* a, Vertex* b, Vertex* c) {
    double a1 = area(toCoord(p), toCoord(a), toCoord(b));
    double a2 = area(toCoord(p), toCoord(b), toCoord(c));
    double a3 = area(toCoord(p), toCoord(a), toCoord(c));
    double ar = area(toCoord(a), toCoord(b), toCoord(c));
    return (ar == a1 + a2 + a3);
}
bool isInterior(Vertex* p, int xy[4]) {
    return (p -> x < xy[0] || p -> x > xy[1] || p -> y < xy[2] || p -> y > xy[3]);
}

void sortCW1(vector<Vertex*> &vlist){
    Vertex* cent = Vertex::centroid(vlist);
    sort(vlist.begin(), vlist.end(), [&cent](Vertex* a, Vertex* b) -> bool {
        PhysicalVector vectorA(toCoord(cent), toCoord(a)), vectorB(toCoord(cent), toCoord(b));
        double angleA = vectorA.getAngle(), angleB = vectorB.getAngle();
        return (angleA == angleB ? vectorA.getMagnitude() < vectorB.getMagnitude(): angleA > angleB);
    });
}

void sortCW2(vector<Vertex*>&vlist) {
    Vertex* xmin = *min_element(vlist.begin(), vlist.end(), [](Vertex* a, Vertex* b) -> bool{
        return a -> x < b -> x;
    });
    sort(vlist.begin(), vlist.end(), [&xmin](Vertex* a, Vertex* b) -> bool {
        double aydiff = a -> y - xmin -> y, bydiff = b -> y - xmin -> y;
        if(aydiff >= 0){
            if(bydiff>=0){
                return (a->x==b->x? aydiff<bydiff : a->x<b->x);
            }else{
                return true;
            }
        }else{
            if(bydiff>=0){
                return false;
            }else{
                return (a->x==b->x? aydiff<bydiff : a->x>b->x);
            }
        }
    });
}

void getBoundRect(vector<Vertex*> vlist, int _xy[4]){
    int xy[] = {INT_MAX, INT_MIN, INT_MAX, INT_MIN};
    for(int j = 0; j < vlist.size(); j++) {
        if (xy[0] > vlist[j] -> x) {
            xy[0] = vlist[j] -> x;
        }
        if (xy[1] < vlist[j] -> x){
            xy[1] = vlist[j] -> x;
        }
        if (xy[2] > vlist[j] -> y){
            xy[2] = vlist[j] -> y;
        }
        if (xy[3] < vlist[j] -> y){
            xy[3] = vlist[j] -> y;
        }
    }
    for(int i = 0; i < 4; i++){
        _xy[i] = xy[i];
    }
}

Vertex* getNxt(vector<Vertex*> &vlist, Vertex* v) {
    for (int i = 0; i < vlist.size(); i++) {
        if (Vertex :: coincides(v,vlist[i])){
            if (i<vlist.size()-1) {
                return vlist[i+1];
            }
            else {
                return nullptr;
            }
        }
    }
    return nullptr;
}

bool isReflex(Vertex* a, Vertex* b, Vertex* c){
    return PhysicalVector::angleBetween(toCoord(a), toCoord(b), toCoord(c)) > PI;
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

void printDCEL(DCEL dcel){
    dcel.foreachVert([](Vertex* v){
        cout << PhysicalVector::toDegree(PhysicalVector(toCoord(v)).getAngle()) << " : ";
        cout << v->x << " " << v->y << endl;
    });
    cout << endl;
}


int main() {
    vector<Vertex*> vlist;
    int n;
    double x, y;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x >> y;
        vlist.push_back(new Vertex(x, y));
    }
    sortCW1(vlist);
    
    vector<vector<Vertex*>> L(1,vector<Vertex*>(1,vlist[0]));
    vector<vector<Vertex*>> finals(0);
    
    for (int m = 1; vlist.size() > 3; m++) {
        printVlist(vlist);
        // cout << m <<  " " << L[m - 1].size() << endl;
        Vertex* v1 = *L[m - 1].rbegin();
        Vertex* v2 = getNxt(vlist, v1);
        L.push_back({v1, v2});
        int i = 1;
        while (L[m].size() < vlist.size()){
            // cout << i << " ";
            // cout << isReflex(vlist[i - 1], vlist[i], vlist[i + 1]);
            // cout << isReflex(vlist[i], vlist[i + 1], vlist[0]);
            // cout << isReflex(vlist[i + 1], vlist[0], vlist[1]) << endl;
            if (isReflex(vlist[i - 1], vlist[i], vlist[i + 1])
                || isReflex(vlist[i], vlist[i + 1], vlist[0])
                || isReflex(vlist[i + 1], vlist[0], vlist[1])) {
                break;
            }
            L[m].push_back(vlist[i+1]);
            i++;
        }
        // printVlist(L[m]);
        vector<Vertex*> notchlist;
        for(i = 0; i < vlist.size(); i++){
            if (isReflex(vlist[i - 1], vlist[i], vlist[(i + 1) % vlist.size()])){
                notchlist.push_back(vlist[i]);
            }
        }
        if(L.size() != vlist.size()) {
            vector<Vertex*> LPVS;
            for (int j = 0; j < notchlist.size(); j++) {
                int chk = 0;
                for (int k = 0; k < L[m].size(); k++) {
                    if (Vertex::coincides(notchlist[j],L[m][k])){
                        chk = 1;
                        break;
                    }
                }
                if (chk == 0) {
                    LPVS.push_back(notchlist[j]);
                }
            }
            while (LPVS.size() > 0) {
                //xmin,xmax,ymin,ymax-->Rectangle
                int xy[4];
                getBoundRect(L[m],xy);
                
                bool Backward = false;
                
                while(!Backward && LPVS.size() > 0){
                    Vertex* tmp;
                    do {
                        tmp = LPVS.front();
                        if (!isInterior(tmp,xy)) {
                            LPVS.erase(LPVS.begin());
                        }
                    } while(!isInterior(tmp,xy) && LPVS.size()>0);
                    if (LPVS.size() > 0) {
                        vector<Vertex*> L_1;
                        L_1 = L[m];
                        while (L_1.size() > 2) {
                            if (!isInterior(LPVS[0], L_1[0], *(--L_1.end()), *(--(--L_1.end())))) {
                                L_1.pop_back();
                            }else {
                                L_1.pop_back();
                                L[m] = L_1;
                                break;
                            }
                        }
                        Backward=true;
                        LPVS.erase(LPVS.begin());
                        
                    }
                }
            }
        }
        if (L[m].back() != vlist[1]) {
            finals.push_back(L[m]);
            // cout << "FIN: ";
            // for(auto l : finals){
            //     printVlist(l);
            // }
            // cout << endl << endl;
            vector<Vertex*> vlist_;
            for(auto v : vlist){
                bool ok = true;
                for(int idx = 1; idx < L[m].size()-1; idx++){
                    if(L[m][idx] == v) ok = false;
                }
                if(ok) vlist_.push_back(v);
            }
            vlist.clear();
            vlist = vlist_;
            Vertex* tmp = vlist.front();
            vlist.erase(vlist.begin());
            vlist.push_back(tmp);
            // cout << vlist.size() << endl;
        }        
    }

    
    for(auto l : finals){
        printVlist(l);
    }
    cout << endl << endl;
    printVlist(vlist);

    return 0;
}