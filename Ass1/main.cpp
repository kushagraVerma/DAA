#include <iostream>
#include <algorithm>
#include <set>
#include "dcel.hpp"
#include "physvector.hpp"

using namespace std;
/**
 * Calculates the area of a triangle given its coordinates
 * @param x1 x-coordinate of point 1
 * @param y1 y-coordinate of point 1
 * @param x2 x-coordinate of point 2
 * @param y2 y-coordinate of point 2
 * @param x3 x-coordinate of point 3
 * @param y3 y-coordinate of point 4
 * @return Area bounded by points (x1, y1), (x2, y2), (x3, y3)
 */
double area(double x1, double y1, double x2, double y2, double x3, double y3) {
    double ar = 0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));/** Area of Triangle formula using 3 point coordinates*/
    if(ar<0) ar*=-1;
    return ar;
}

/**
 * Checks if a point is interior to the triangle bounded by three vertices
 * @param p vertex to be checked for interiority
 * @param a first vertex
 * @param b second vertex
 * @param c third vertex
 * @return a boolean value that tells if the point is interior or not
 */
bool isInterior(Vertex* p, Vertex* a, Vertex* b, Vertex* c) {
    double a1 = area(toCoord(p), toCoord(a), toCoord(b));
    double a2 = area(toCoord(p), toCoord(b), toCoord(c));
    double a3 = area(toCoord(p), toCoord(a), toCoord(c));
    double ar = area(toCoord(a), toCoord(b), toCoord(c));
    return (ar == a1 + a2 + a3);
}

/**
 * Checks if a point is interior to a rectangular region
 * @param p vertex to be checked for interiority
 * @param rect integer array that describes the coordinates of the rectangular region
 * @return a boolean value that tells if the point is interior or not
 */
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
    Vertex* xmin = *min_element(vlist.begin(), vlist.end(), [](Vertex* a, Vertex* b) -> bool {
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

/**
 * Finds the smallest rectangle that bounds all the specified vertices
 * @param vlist a list of vertices
 * @param _rect list describing the coordinates of the resultant rectangle
 */
void getBoundRect(vector<Vertex*> vlist, int _rect[4]){
    /**Finding the smallest rectangle that bounds all specified vertices*/
    int rect[] = {INT_MAX, INT_MIN, INT_MAX, INT_MIN};
    for(int j = 0; j < vlist.size(); j++) {
        if (rect[0] > vlist[j] -> x) {/**Lower rectangle edge parallel to x-axis*/
            rect[0] = vlist[j] -> x;
        }
        if (rect[1] < vlist[j] -> x){/**Upper rectangle edge parallel to x-axis*/
            rect[1] = vlist[j] -> x;
        }
        if (rect[2] > vlist[j] -> y){/**Left rectangle edge parallel to y-axis*/
            rect[2] = vlist[j] -> y;
        }
        if (rect[3] < vlist[j] -> y){/**Right rectangle edge parallel to y-axis*/
            rect[3] = vlist[j] -> y;
        }
    }
    for(int i = 0; i < 4; i++){
        _rect[i] = rect[i];
    }
}

/**
 * Gives the next vertex of a given vertex
 * @param vlist a list of vertices
 * @param v a vertex
 * @return the next vertex of the given vertex
 */
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

/**
 * checks if the angle formed by three vertices is reflex or not
 * @param a vertex 1
 * @param b vertex 2
 * @param c vertex 3
 * @return boolean value that is true when the angle formed by the vertices is reflex
 */
bool isReflex(Vertex* a, Vertex* b, Vertex* c){
    return PhysicalVector::angleBetween(toCoord(a), toCoord(b), toCoord(c)) > PI;/** Check if angle between a,b & c is reflex*/
}

/**
 * Prints the vertices of a vertex list
 * @param vlist a vertex list
 * @param centAng boolean to conditionally print angle
 */
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

/**
 * Prints the x and y coordinates of a vertex
 * @param v vertex
 */
void printVert(Vertex* v){
    cout << "(" << v->x << "," << v->y << ")";
}

/**
 * prints the two vertices of an edge in the format (origin <=> destination)
 */
void printEdge(Edge* e){
    printVert(e->org);
    cout << "<=>";
    printVert(e->dest());
}

/**
 * Prints the vertices of a DCEL
 * @param dcel a doubly connected edge list
 */
void printDCEL(DCEL &dcel){
    for(auto f : dcel.faces){
        dcel.forEdgesAlong(f->inc_edge,[](Edge* e){
            printVert(e->org); cout << " ";
        },true);
        cout << endl;
    }
}

/**
 * Main function
 */
int main() {
    vector<Vertex*> vlist; /** A list of all vertices */
    int n; /** No. of vertices */
    double x, y; /** Variables to take vertex coordinates as input */
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x >> y; /** Taking vertex coordinate information in order of edges of initial polygon */
        vlist.push_back(new Vertex(x, y));
    }
    const vector<Vertex*> constlist(vlist);
    DCEL dcel(vlist);/**Making the initial Polygon in DCEL*/
    cout << "Original DCEL: " << endl;
    printDCEL(dcel);
    
    vector<vector<Vertex*>> L(1,vector<Vertex*>(1,vlist[0])); /** A vector to store all the decomposed polygons */
    vector<Edge*> diagonals(0); /** A list of diagonals added to the polynomial */
    
    for (int m = 1; vlist.size() > 3; m++) {
        ///D
        // if(m>vlist.size()){
        //     cout << "WAT" << endl;
        //     exit(-1);
        // }
        // cout << m << endl;
        ///D
        Vertex* v1 = *L[m - 1].rbegin(); /** Get the last vertex in remaining vertex list */
        Vertex* v2 = getNxt(vlist, v1); /** The vertex next to v1 in the polynomial */
        L.push_back({v1, v2}); /** Add the 2 vertices to the current convex polygon list */
        int i = 1;
        while (L[m].size() < vlist.size()){ /** Until the polygon covers all vertices */
            if (isReflex(vlist[i - 1], vlist[i], vlist[i + 1]) /** Check angle between (i-1)th,ith and (i+1)th vertex is Reflex */
                || isReflex(vlist[i], vlist[i + 1], vlist[0]) /** Check angle between ith, (i+1)th and 1st vertex is Reflex */
                || isReflex(vlist[i + 1], vlist[0], vlist[1])) { /** Check angle between (i+1)th, 1st and 2nd vertex is Reflex */
                break; /** Point cannot be added to current convex polygon */
            }
            L[m].push_back(vlist[i+1]); /** Point can be added to current convex polygon */
            i++;
        }
        vector<Vertex*> notchlist; /** List of notches. Notches being vertices of polygon with reflexive inner angle. */
        for(i = 0; i < vlist.size(); i++){
            if (isReflex(vlist[(i + vlist.size() - 1) % vlist.size()], vlist[i], vlist[(i + 1) % vlist.size()])){
                notchlist.push_back(vlist[i]); /** Adds vertex to notchlist if the vertex,it's previous and it's next form a reflexive angle. */
            }
        }
        if(L[m].size() != vlist.size()) {
            vector<Vertex*> LPVS; /** Contains notches in the polygon not part of L[m] */
            
            for (int j = 0; j < notchlist.size(); j++) {
                int chk = 0;
                for (int k = 0; k < L[m].size(); k++) {
                    if (Vertex::coincides(notchlist[j],L[m][k])){ /** Check if the notch is a part of L[m] */
                        chk = 1;
                        break;
                    }
                }
                if (chk == 0) {
                    LPVS.push_back(notchlist[j]);
                }
            }
            while (LPVS.size() > 0) { /** Check if the notch is a part of the smallest rectangle containing all vertices of L[m] */
                int rect[4];
                getBoundRect(L[m],rect);
                bool Backward = false;
                while(!Backward && LPVS.size() > 0){
                    Vertex* tmp;
                    do {
                        tmp = LPVS.front();
                        if (!isInterior(tmp,rect)) {
                            LPVS.erase(LPVS.begin());
                        }else{
                            break;
                        }
                    } while(LPVS.size()>0);
                    if (LPVS.size() > 0) {
                        vector<Vertex*> L_1(L[m]);
                        bool inside = false;
                        while (L_1.size() > 2 && !inside) {
                            inside |= isInterior(LPVS[0], L_1[0], L_1[L_1.size()-1], L_1[L_1.size()-2]);
                            L_1.pop_back();
                        }
                        if (inside) {
                            while (L_1.size() > 2 && isInterior(LPVS[0], L_1[0], L_1[L_1.size()-1], L_1[L_1.size()-2])) {
                                L_1.pop_back();
                            }
                            L[m] = L_1;
                        }
                        Backward = true;
                        LPVS.erase(LPVS.begin());                        
                    }
                }
            }
        }
        if (L[m].back() != vlist[1]) { /** Checking if the polygon contains only 2 vertices */
            ///D
            // printVlist(L[m]);
            ///D
            bool status;
            Edge* e = dcel.splitFace(L[m].front(),L[m].back(),status); /** Splitting a face wrt 2 edges */
            if (status) diagonals.push_back(e); /** Adding this newly formed diagonal to the list of diagonals */

            vector<Vertex*> vlist_;
            for (auto v : vlist){
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
    }
    if (vlist.size() > 2) {
        bool status;
        Edge* e = dcel.splitFace(vlist.front(),vlist.back(),status);
        if(status) diagonals.push_back(e);
    }
    cout << "\nAfter decomposition: " << dcel.faces.size() << " polygons" << endl;
    printDCEL(dcel);
    
    for (auto it = diagonals.rbegin(); it != diagonals.rend(); it++) {
        auto d = *it;
        bool flag1 = isReflex(d->twin->next->dest(),d->org,d->prev->org);
        bool flag2 = isReflex(d->next->dest(),d->dest(),d->twin->prev->org);
        if (flag1 || flag2) continue;
        if (dcel.mergeFace(d)) {
            delete d;
            diagonals.erase(next(it).base());
        }
    }
    
    cout << "\nAfter merging: " << dcel.faces.size() << " polygons" << endl;
    printDCEL(dcel);
    return 0;
}