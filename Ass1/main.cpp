#include <iostream>
#include <algorithm>
#include <set>
#include "dcel.hpp"
#include "physvector.hpp"

using namespace std;

double area(double x1, double y1, double x2, double y2, double x3, double y3) {
    double ar = 0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
    if(ar<0) ar*=-1;
    return ar;
}

bool isInterior(Vertex* p, Vertex* a, Vertex* b, Vertex* c) {
    // printf("p=(%lf,%lf) in a=(%lf,%lf), b=(%lf,%lf), c=(%lf,%lf)?\n",
    //     toCoord(p), toCoord(a), toCoord(b), toCoord(c));
    double a1 = area(toCoord(p), toCoord(a), toCoord(b));
	// printf("pab=%lf\n",a1);
    double a2 = area(toCoord(p), toCoord(b), toCoord(c));
	// printf("pbc=%lf\n",a2);
    double a3 = area(toCoord(p), toCoord(a), toCoord(c));
	// printf("pac=%lf\n",a3);
    double ar = area(toCoord(a), toCoord(b), toCoord(c));
	// printf("abc=%lf\n",ar);
    return (ar == a1 + a2 + a3);
}
bool isInterior(Vertex* p, int rect[4]) {
    return (p -> x > rect[0] && p -> x < rect[1] && p -> y > rect[2] && p -> y < rect[3]);
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

void getBoundRect(vector<Vertex*> vlist, int _rect[4]){
    int rect[] = {INT_MAX, INT_MIN, INT_MAX, INT_MIN};
    for(int j = 0; j < vlist.size(); j++) {
        if (rect[0] > vlist[j] -> x) {
            rect[0] = vlist[j] -> x;
        }
        if (rect[1] < vlist[j] -> x){
            rect[1] = vlist[j] -> x;
        }
        if (rect[2] > vlist[j] -> y){
            rect[2] = vlist[j] -> y;
        }
        if (rect[3] < vlist[j] -> y){
            rect[3] = vlist[j] -> y;
        }
    }
    for(int i = 0; i < 4; i++){
        _rect[i] = rect[i];
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
void printVert(Vertex* v){
    cout << "(" << v->x << "," << v->y << ")";
}
void printEdge(Edge* e){
    printVert(e->org);
    cout << "<=>";
    printVert(e->dest());
}
void printDCEL(DCEL &dcel){
    for(auto f : dcel.faces){
        dcel.forEdgesAlong(f->inc_edge,[](Edge* e){
            printVert(e->org); cout << " ";
        },true);
        cout << endl;
    }
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
    const vector<Vertex*> constlist(vlist);
    DCEL dcel(vlist);
    cout << "Original DCEL: " << endl;
    printDCEL(dcel);
    // sortCW1(vlist);
    
    vector<vector<Vertex*>> L(1,vector<Vertex*>(1,vlist[0]));
    vector<vector<Vertex*>> finalists(0);
    vector<Edge*> diagonals(0);
    
    for (int m = 1; vlist.size() > 3; m++) {
        // cout << m << " vlist: "; printVlist(vlist);
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
        // cout << "OG Lm: "; printVlist(L[m]);
        vector<Vertex*> notchlist;
        for(i = 0; i < vlist.size(); i++){
            // cout << i << endl;
            if (isReflex(vlist[(i + vlist.size() - 1) % vlist.size()], vlist[i], vlist[(i + 1) % vlist.size()])){
                notchlist.push_back(vlist[i]);
            }
        }
        // printVlist(notchlist);
        if(L[m].size() != vlist.size()) {
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
                // cout << "LPVS: "; printVlist(LPVS);
                // cout << "Lm: "; printVlist(L[m]);
                //xmin,xmax,ymin,ymax-->Rectangle
                int rect[4];
                getBoundRect(L[m],rect);
                // cout << "rect: ";
                // for(auto x : rect){
                //     cout << x << " ";
                // }
                // cout << endl;
                
                bool Backward = false;
                
                while(!Backward && LPVS.size() > 0){
                    Vertex* tmp;
                    do {
                        tmp = LPVS.front();
                        // cout << "tmp: " << tmp->x << " " << tmp->y << endl;
                        if (!isInterior(tmp,rect)) {
                            LPVS.erase(LPVS.begin());
                        }else{
                            break;
                        }
                    } while(LPVS.size()>0);
                    // cout << "LPVS size: " << LPVS.size() << endl;
                    if (LPVS.size() > 0) {
                        vector<Vertex*> L_1(L[m]);
                        bool inside = false;
                        while (L_1.size() > 2 && !inside) {
                            inside |= isInterior(LPVS[0], L_1[0], L_1[L_1.size()-1], L_1[L_1.size()-2]);
                            L_1.pop_back();
                        }
                        if(inside){
                            while(
                                L_1.size() > 2 
                                && isInterior(LPVS[0], L_1[0], L_1[L_1.size()-1], L_1[L_1.size()-2])
                            ){
                                L_1.pop_back();
                            }
                            // cout << "L_1 size: " << L_1.size() << endl; 
                            L[m] = L_1;
                        }
                        Backward=true;
                        LPVS.erase(LPVS.begin());                        
                    }
                }
            }
        }
        if (L[m].back() != vlist[1]) {
            
            bool status;
            Edge* e = dcel.splitFace(L[m].front(),L[m].back(),status);
            if(status) diagonals.push_back(e);

            finalists.push_back(L[m]);
            // cout << "FIN: ";
            // for(auto l : finalists){
            //     printVlist(l);
            // }
            // cout << endl;
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
        }        
        Vertex* tmp = vlist.front();
        vlist.erase(vlist.begin());
        vlist.push_back(tmp);
        // cout << vlist.size() << endl;
    }
    // exit(-1);
    if(vlist.size()>2){
        bool status;
        Edge* e = dcel.splitFace(vlist.front(),vlist.back(),status);
        if(status) diagonals.push_back(e);
    }
    // exit(-1);
    // cout << "AAAA";
    // printDCEL(dcel);

    // for(int i = 0; i < dcel.edges.size(); i++){
    //     Edge* e = dcel.edges[i];
    //     cout << i << " ";
    //     printEdge(e);
    //     cout << endl;
    // }
    // cout << endl;
    cout << "\nAfter decomposition: " << dcel.faces.size() << " polygons" << endl;
    printDCEL(dcel);
    
    for(auto it = diagonals.rbegin(); it != diagonals.rend(); it++){
        auto d = *it;
        // printEdge(d);
        bool flag1 = isReflex(d->twin->next->dest(),d->org,d->prev->org);
        bool flag2 = isReflex(d->next->dest(),d->dest(),d->twin->prev->org);
        if(flag1 || flag2) continue;
        if(dcel.mergeFace(d)){
            delete d;
            diagonals.erase(next(it).base());
        }
    }
    
    cout << "\nAfter merging: " << dcel.faces.size() << " polygons" << endl;
    printDCEL(dcel);

    // vector<DCEL> dcels(0);
    // for(auto l : finalists){
    //     // printVlist(l);
    //     dcels.push_back(DCEL(l));
    //     printDCEL(dcels.back());
    // }

    return 0;
}