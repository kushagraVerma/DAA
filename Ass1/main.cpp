#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include "dcel.hpp"
#include "physvector.hpp"
#include "timer.hpp"

using namespace std;
/**
 * Returns the area of a triangle given its coordinates
 * @param x1 x-coordinate of point 1
 * @param y1 y-coordinate of point 1
 * @param x2 x-coordinate of point 2
 * @param y2 y-coordinate of point 2
 * @param x3 x-coordinate of point 3
 * @param y3 y-coordinate of point 4
 */
double area(double x1, double y1, double x2, double y2, double x3, double y3) {
    double total = 0.5 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)); /** Area of Triangle formula using 3 point coordinates*/
    if(total < 0) total *= -1;
    return total;
}

/**
 * Checks if a point is interior to the triangle bounded by three vertices
 * @param p vertex to be checked for interiority
 * @param a first vertex
 * @param b second vertex
 * @param c third vertex
 */
bool isInterior(Vertex* p, Vertex* a, Vertex* b, Vertex* c) {
    double a1 = area(toCoord(p), toCoord(a), toCoord(b));
    double a2 = area(toCoord(p), toCoord(b), toCoord(c));
    double a3 = area(toCoord(p), toCoord(a), toCoord(c));
    double total = area(toCoord(a), toCoord(b), toCoord(c));
    return abs(total - (a1 + a2 + a3))<(1e-12);
}

/**
 * Checks if a point is interior to a rectangular region
 * @param p vertex to be checked for interiority
 * @param rect integer array that describes the coordinates of the rectangular region
 */
bool isInterior(Vertex* p, double rect[4]) {
    return (((p -> x) > rect[0]) && ((p -> x) < rect[1]) && ((p -> y) > rect[2]) && ((p -> y) < rect[3]));
}

/**
 * Finds the smallest rectangle that bounds all the specified vertices
 * @param vlist a list of vertices
 * @param _rect list describing the coordinates of the resultant rectangle
 */
void getBoundRect(vector<Vertex*> vlist, double _rect[4]){
    double rect[] = {INT_MAX, INT_MIN, INT_MAX, INT_MIN};
    for(int j = 0; j < vlist.size(); j++) {
        if (rect[0] > vlist[j] -> x) {/// Lower rectangle edge parallel to x-axis
            rect[0] = vlist[j] -> x;
        }
        if (rect[1] < vlist[j] -> x){/// Upper rectangle edge parallel to x-axis
            rect[1] = vlist[j] -> x;
        }
        if (rect[2] > vlist[j] -> y){/// Left rectangle edge parallel to y-axis
            rect[2] = vlist[j] -> y;
        }
        if (rect[3] < vlist[j] -> y){/// Right rectangle edge parallel to y-axis
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
 */
Vertex* getNxt(vector<Vertex*> &vlist, Vertex* v) {
    for (int i = 0; i < vlist.size(); i++) {
        if (Vertex :: coincides(v,vlist[i])){
            if (i < (vlist.size() - 1)) {
                return vlist[i + 1];
            }
            else {
                return nullptr;
            }
        }
    }
    return nullptr;
}

/**
 * Checks if the angle formed by three vertices is reflex or not
 * @param a vertex 1
 * @param b vertex 2
 * @param c vertex 3
 */
bool isReflex(Vertex* a, Vertex* b, Vertex* c){
    return PhysicalVector::angleBetween(toCoord(a), toCoord(b), toCoord(c)) > PI;
}

/**
 * Checks if the vertices of a vertex list are in counter-clockwise order
 * @param vlist a vertex list
 */
bool isCCW(vector<Vertex*> &vlist){
    double summed = 0;
    for(int i = 0; i < vlist.size()-1; i++){
        Vertex* v1 = vlist[i];
        Vertex* v2 = vlist[i+1];
        summed += (v2->x - v1->x)*(v2->y + v1->y);
    }
    return summed < 0;
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
        cout << "(" << v -> x << "," << v -> y << ")@";
        if(centAng) 
            cout << PhysicalVector::toDegree((PhysicalVector(toCoord(cent), toCoord(v))).getAngle()) << " ";
    }
    delete cent;
    cout << "}" << endl;
}

/**
 * Prints the x and y coordinates of a vertex
 * @param v vertex
 */
void printVert(Vertex* v){
    cout << "(" << v -> x << "," << v -> y << ")";
}

/**
 * Prints the two vertices of an edge in the format (origin <=> destination)
 */
void printEdge(Edge* e){
    printVert(e -> org);
    cout << "<=>";
    printVert(e -> dest());
}

/**
 * Prints the vertices of a DCEL
 * @param dcel a doubly connected edge list
 */
void printDCEL(DCEL &dcel){
    for(auto f : dcel.faces){
        dcel.forEdgesAlong(f -> inc_edge,[](Edge* e){
            printVert(e -> org); cout << " ";
        },true);
        cout << endl;
    }
}

/**
 * Main function
 */
int main(int argc, char *argv[]) {
    Timer timer,timer_noprint,timer_decomp,timer_merge;
    bool timeit = (argc>1 && string(argv[1]).compare(string("--time"))==0);

    vector<Vertex*> vlist; /// A list of all vertices
    int n; /// No. of vertices
    double x, y; /// Variables to take vertex coordinates as input
    cin >> n;
    /** Taking vertex coordinate information in order of edges of initial polygon */
    set<pair<int,int> > unq;
    for (int i = 0; i < n; i++) {
        cin >> x >> y; 
        Vertex* v = new Vertex(x, y);
        if(unq.insert({x,y}).second) vlist.push_back(v);
    }

    if(timeit){
        timer = Timer(); 
        timer_noprint = Timer();
    }

    if(isCCW(vlist)) reverse(vlist.begin(),vlist.end());

    DCEL dcel(vlist); /// Making the initial Polygon in DCEL

    if(timeit){
        timer_noprint.stopClock();
    }
    cout << "Original DCEL: " << endl;
    printDCEL(dcel);

    if(timeit){
        timer_noprint.restartClock();
        timer_decomp = Timer();
    }
    
    vector<vector<Vertex*>> L(1, vector<Vertex*>(1,vlist[0])); /// A vector to store all the decomposed polygons
    vector<Edge*> diagonals(0); /// A list of diagonals added to the polynomial 
    
    for (int m = 1; vlist.size() > 3; m++) {
        Vertex* v1 = *L[m - 1].rbegin(); /// Last vertex in remaining vertex list assigned v1
        Vertex* v2 = getNxt(vlist, v1); /// The vertex next to v1 in the polynomial
        L.push_back({v1, v2}); /// Adding the 2 vertices to the current convex polygon list
        int i = 1;
        /** Until the polygon covers all vertices. Check angle between (i-1)th,ith and (i+1)th vertex is Reflex.Check angle between ith, (i+1)th and 1st vertex is Reflex.
         Check angle between (i+1)th, 1st and 2nd vertex is Reflex. If one of the conditions is true then,the point cannot be added to current convex polygon. */
        while (L[m].size() < vlist.size()){ 
            if (isReflex(vlist[i - 1], vlist[i], vlist[i + 1]) 
                || isReflex(vlist[i], vlist[i + 1], vlist[0]) 
                || isReflex(vlist[i + 1], vlist[0], vlist[1])) { 
                break; 
            }
            L[m].push_back(vlist[i + 1]);
            i++;
        }
        
        vector<Vertex*> notchlist; /// List of notches. Notches being vertices of polygon with reflexive inner angle.
        for(i = 0; i < vlist.size(); i++){
            if (isReflex(vlist[(i + vlist.size() - 1) % vlist.size()], vlist[i], vlist[(i + 1) % vlist.size()])){
                notchlist.push_back(vlist[i]); /// Adds vertex to notchlist if the vertex,it's previous and it's next form a reflexive angle.
            }
        }
        if(L[m].size() != vlist.size()) {
            vector<Vertex*> LPVS; /// List of notches in the polygon not part of L[m]
            
            for (int j = 0; j < notchlist.size(); j++) {
                int chk = 0;
                for (int k = 0; k < L[m].size(); k++) {
                    if (Vertex::coincides(notchlist[j], L[m][k])){ /// Check if the notch is a part of L[m]
                        chk = 1;
                        break;
                    }
                }
                if (chk == 0) {
                    LPVS.push_back(notchlist[j]);
                }
            }
            /** Check if the notch is a part of the smallest rectangle containing all vertices of L[m]. This acts as a preliminary check for a 
            a notch being interior. As a definite check to the notch being in the interior of the DCEL, we check if the notch tmp from LPVS
            is in the interior of the triangle formed by the 1st, (n - 1) th and nth vertex using the function isInterior for the vertices. If inside, we 
            reduce the size of the DCEL until the notch is thrown out. This is done for all notches in the LPVS list.*/
            while (LPVS.size() > 0) { 
                double rect[4];
                getBoundRect(L[m], rect);
                bool Backward = false;
                while(!Backward && LPVS.size() > 0){
                    Vertex* tmp;
                    do {
                        tmp = LPVS.front();
                        if (!isInterior(tmp, rect)) {
                            LPVS.erase(LPVS.begin());
                        }
                        else {
                            break;
                        }
                    } while(LPVS.size() > 0);
                    
                    if (LPVS.size() > 0) {
                        vector<Vertex*> L_1(L[m]);
                        bool inside = false;
                        while (L_1.size() > 2 && !inside) {
                            inside |= isInterior(LPVS[0], L_1[0], L_1[L_1.size() - 1], L_1[L_1.size() - 2]);
                            L_1.pop_back();
                        }
                        if (inside) {
                            while ((L_1.size() > 2) && isInterior(LPVS[0], L_1[0], L_1[L_1.size() - 1], L_1[L_1.size() - 2])) {
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
        if (L[m].back() != vlist[1]) { ///< Checking if the polygon contains only 2 vertices
            bool status;
            Edge* e = dcel.splitFace(L[m].front(), L[m].back(), status); ///< Splitting a face wrt 2 edges 
            if (status) diagonals.push_back(e); ///< Adding this newly formed diagonal to the list of diagonals 

            vector<Vertex*> vlist_;
            for (auto v : vlist){
                bool ok = true;
                for(int idx = 1; idx < L[m].size() - 1; idx++){
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
        Edge* e = dcel.splitFace(vlist.front(), vlist.back(), status);
        if(status) diagonals.push_back(e);
    }
    if(timeit){
        timer_decomp.stopClock();
        timer_noprint.stopClock();
    }

    cout << "\nAfter decomposition: " << dcel.faces.size() << " polygons" << endl;
    printDCEL(dcel);

    if(timeit){
        timer_noprint.restartClock();
        timer_merge = Timer();
    }
    
    /**The original algorithm for merging states 3 conditions to check and proceed with the merging process.
     These 3 conditions are handled by the definition of DCEL as it encapsulates adjacency information. 
     Hence the only super-condition to be checked is whether the vertices of the merged polygon would be non-reflex.
     We iterate through the list of diagonals and perform this check.*/
    for (auto it = diagonals.rbegin(); it != diagonals.rend(); it++) {
        auto d = *it;
        bool flag1 = isReflex(d -> twin -> next -> dest(), d -> org, d -> prev -> org);
        bool flag2 = isReflex(d -> next -> dest(), d -> dest(), d -> twin -> prev -> org);
        if (flag1 || flag2) continue;
        if (dcel.mergeFace(d)) {
            delete d;
            diagonals.erase(next(it).base());
        }
    }

    if(timeit){
        timer_noprint.stopClock();
        timer_merge.stopClock();
    }
    
    /**Print the individual polygon DCEL vertices using the printDCEL function.*/
    cout << "\nAfter merging: " << dcel.faces.size() << " polygons" << endl;
    printDCEL(dcel); 

    if(timeit){
        timer.stopClock();
        cout << "\nTimings in microseconds:" << endl;
        cout << "\tOverall: " << timer.getDuration() << endl;
        cout << "\tExcluding printing: " << timer_noprint.getDuration() << endl;
        cout << "\tFor decomposition: " << timer_decomp.getDuration() << endl;
        cout << "\tFor merging: " << timer_merge.getDuration() << endl;
    }
    
    return 0;
}