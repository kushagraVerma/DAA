#ifndef DCEL_HPP
#define DCEL_HPP

#include <vector>
#include <functional>

using namespace std;

#define toCoord(v) (v) -> x, (v) -> y

class Edge;

/**
 * A class to define a DCEL face.
*/
class Face{
    public:
        Edge* inc_edge = nullptr; ///< pointer to any incident edge
        /**
         * Constructor function for Face class
         * @param e_ pointer to incident edge
        */
        Face(Edge* e_){
            inc_edge = e_;
        }
};

/**
 * A class to define a DCEL vertex.
*/
class Vertex{
    public:
        long double x = 0.0, y = 0.0; ///< Coordinate points of the vertex in two dimensional Cartesian plane
        Edge* inc_edge = nullptr; ///< pointer to any incident edge

        /**
         * Constructor function for Vertex class
         * @param _x x-coordinate of the vertex
         * @param _y y-coordinate of the vertex
        */
        Vertex(long double _x, long double _y){ 
            x = _x; y = _y;
        }

        Edge* edgeTo(Vertex* v);
        void clearEdge();
        void foreachIncEdge(function<void(Edge*)> func);
        Vertex* nextV();
        Vertex* prevV();
        static Edge* getEdge(Vertex* a, Vertex* b);

        /**
         * Static function that returns the midpoint of two vertices
         * @param a pointer to first vertex
         * @param b pointer to second vertex
        */
        static Vertex* midpoint(Vertex* a, Vertex* b){
            vector<Vertex*> vlist = {a,b};
            return centroid(vlist);
        }

        /**
         * Static function that returns the centroid of a set of vertices
         * @param vlist vector of vertices
        */
        static Vertex* centroid(vector<Vertex*> &vlist){
            long double centroidX = 0, centroidY = 0;
            for(auto v : vlist){
                centroidX += v -> x;
                centroidY += v -> y;
            }
            centroidX /= vlist.size();
            centroidY /= vlist.size();
            return new Vertex(centroidX, centroidY);
        }

        /**
         * Static function that returns boolean value indicating whether two vertices coincide
         * @param a pointer to first vertex
         * @param b pointer to second vertex
        */
        static bool coincides(Vertex* a, Vertex* b){
            return ((a -> x == b -> x) && (a -> y == b -> y));
        }

        /**
         * Overrides "<" operator to judge whether vertex is less than another vertex
         * @param other pointer to the other vertex
        */
        bool operator<(Vertex* other){
            return (x == (other -> x) ? y < (other -> y) : x < (other->x));
        }

        /**
         * Overrides "==" operator to judge whether vertex is less than another vertex
         * @param other pointer to the other vertex
        */
        bool operator==(Vertex* other){
            return ((x == (other -> x)) && (y == (other -> y)));
        }
};

/**
 * A class to define a DCEL edge.
*/
class Edge{
    public:
        Edge* twin = nullptr; ///< pointer to the twin edge of this edge
        Edge* next = nullptr; ///< pointer to the next edge of this edge
        Edge* prev = nullptr; ///< pointer to the previous edge of this edge
        Vertex* org = nullptr; ///< pointer to the origin vertex of this edge
        Face* left = nullptr; ///< pointer to the face incident to the left of this edge

        /**
         * Constructor function for Edge class
         * @param _org pointer to the origin vertex
        */
        Edge(Vertex* _org){
            org = _org;
        }

        /**
         * Overload constructor function for Edge class
         * @param _org pointer to the origin vertex 
         * @param _twin pointer to the twin edge
        */
        Edge(Vertex* _org, Edge* _twin) : Edge(_org){
            twin = _twin;
        }

        /**
         * Overload constructor function for Edge class
         * @param _org pointer to the origin vertex 
         * @param _dest pointer to the destination vertex
        */
        Edge(Vertex* _org, Vertex* _dest) : Edge(_org){
            twin = new Edge(_dest, this);
        }

        /**
         * Destructor function for Edge class
        */
        ~Edge(){
            if(twin){
                twin -> twin = nullptr;
                delete twin;
            }
        }

        /**
         * Function that returns the destination vertex of this edge
        */
        Vertex* dest(){
            return twin -> org;
        }

        /**
         * Function that returns the face incident to the right of this edge
        */
        Face* right(){
            return twin->left;
        }

        /**
         * Function that establishes the next and previous connections between this edge and another edge
         * @param e pointer to the edge with which we want to establish the relation
         * @param setTwins when set, connects the twins of this edge and the given edge as well
        */
        void setNext(Edge* e, bool setTwins = true){
            next = e;
            e -> prev = this;
            if(setTwins){
                e -> twin -> next = twin;
                twin -> prev = e -> twin;
            }
        }

        /**
         * Function that splits this edge at given vertex by creating another edge
         * @param v pointer to the vertex to split the edge at
        */
        Edge* split(Vertex* v){
            Vertex* dst = dest();
            twin -> org = v;
            v -> edgeTo(dst);
            v -> inc_edge -> setNext(next);
            setNext(v -> inc_edge);
            return v -> inc_edge;
        }

        /**
         * Overloaded function that splits this edge at its midpoint
        */
        Edge* split(){
            Vertex* midPoint = Vertex::midpoint(org, dest());
            Edge* e = split(midPoint);
            delete midPoint;
            return e;
        }

        /**
         * Static function that performs the splice operation between two split edges
         * @param a1 pointer to first half of first edge
         * @param a2 pointer to second half of first edge
         * @param b1 pointer to first half of second edge
         * @param b2 pointer to second half of second edge
        */
        static void splice(Edge* a1, Edge* a2, Edge* b1, Edge* b2){
            delete b2 -> org;
            b1 -> twin -> org = b2 -> org = a2 -> org;
            a1 -> setNext(b1 -> twin, false);
            b1 -> setNext(a2, false);
            a2 -> twin -> setNext(b2, false);
            b2 -> twin -> setNext(a1 -> twin, false);
        }

        /**
         * Static function that performs the merge operation on two edges
         * @param a1 pointer to the first edge
         * @param b1 pointer to the second edge
        */
        static void merge(Edge* a1, Edge* b1){
            Edge* a2 = a1 -> split();
            Edge* b2 = b1 -> split();
            splice(a1, a2, b1, b2);
        }

        /**
         * Static function to check if two edges coincide
         * @param a pointer to the first edge
         * @param b pointer to the second edge
        */
        static int coincides(Edge* a, Edge* b){
            if(Vertex::coincides(a -> org, b -> org) && Vertex::coincides(a -> dest(), b -> dest())){
                return 1;
            }
            if(Vertex::coincides(a -> org, b -> dest()) && Vertex::coincides(a -> dest(), b -> org)){
                return -1;
            }
            return 0;
        }
};

/**
 * Static function of the Vertex class to get edge from one vertex to another, if any
 * @param a pointer to first vertex
 * @param b pointer to second vertex
*/
Edge* Vertex::getEdge(Vertex* a, Vertex* b){
    Edge* E = nullptr;
    a -> foreachIncEdge([&b, &E](Edge* e){
        if(Vertex::coincides(b, e -> dest())){
            E = e;
        }
    });
    return E;
}

/**
 * Function of the Vertex class to make an edge from this vertext to another unless one already exists
 * @param v pointer to the other vertex
*/
Edge* Vertex::edgeTo(Vertex* v){
    Edge* existing = Vertex::getEdge(this, v);
    return (inc_edge = (existing ? existing : new Edge(this, v)));
}

/**
 * Function of the Vertex class to remove this vertex's incident edge
*/
void Vertex::clearEdge(){
    delete inc_edge;
    inc_edge = nullptr;
}

/**
 * Function of the Vertex class to iterate over all incident edges of this vertex
 * @param func function argument that is executed on each iterated edge
*/
void Vertex::foreachIncEdge(function<void(Edge*)> func){
    if(!(this -> inc_edge)) return;
    Edge* start = this -> inc_edge;
    do{
        func(start);
        if((start -> twin) && (start -> twin -> next)){
            start = start -> twin -> next;
        }else{
            return;
        }
    }while(start != (this -> inc_edge));
}

/**
 * Function of the Vertex class to get the vertex next from this vertex, if any
*/
Vertex* Vertex::nextV(){
    if(inc_edge){
        return inc_edge -> dest();
    }else{
        return nullptr;
    }
}

/**
 * Function of the Vertex class to get the vertex previous from this vertex, if any
*/
Vertex* Vertex::prevV(){
    if(inc_edge && (inc_edge -> prev)){
        return inc_edge -> prev -> org;
    }else{
        return nullptr;
    }
}

/**
 * A class to define a DCEL.
*/
class DCEL{
    private:
        /**
         * Function which adds a vertex to the DCEL
         * @param v pointer to the vertex
        */
        void addVert(Vertex* v){
            Edge* e = last -> inc_edge; //...<-e-last
            last -> edgeTo(v);//...<-e-last--->v
            v -> inc_edge = last -> inc_edge -> twin;//...<-e-last<===>v
            if(n>1){ //last!=start
                v -> inc_edge -> setNext(e);
            }
            last = v;
            n++;
        }
    public:
        Vertex* start = nullptr; ///< pointer to starting vertex of DCEL
        Vertex* last = nullptr; ///< pointer to last vertex of DCEL
        vector<Face*> faces; ///< Vector of faces in the DCEL
        int n = 0; ///< Number of vertices in the DCEL
        
        /**
         * Constructor function for DCEL class
         * @param _start first vertex of the DCEL
         */
        DCEL(Vertex* _start){
            start = _start;
            last = start;
            n = 1;
        }
        
        /**
         * Overloaded constructor function for DCEL class
         * @param vlist vertices to be added to the DCEL
         */
        DCEL(vector<Vertex*> &vlist) : DCEL(vlist[0]){
            int _n = vlist.size();
            if(_n > 1){
                for(int i = 1; i < _n; i++){
                    addVert(vlist[i]);
                }
                Edge* e = last -> inc_edge; //...<-e-last
                last -> edgeTo(start);//...<-e-last--->start
                e -> twin -> setNext(last -> inc_edge);
                last -> inc_edge -> setNext(start -> inc_edge);
                Face* f0 = new Face(e -> prev);
                forEdgesAlong(e,[&f0](Edge* curr){
                    curr -> left = f0;
                    curr -> org -> inc_edge = curr;
                });
                faces.push_back(f0);
            }
        }
        
        /**
         * Function to iterate over chain of edges having the next connection from given edge
         * @param e pointer to the edge
         * @param func function argument that is exectuted on each iterated edge
         * @param reverse when set, iterates over chain of edges having the previous connection instead
        */
        Edge* forEdgesAlong(Edge* e, function<void(Edge*)> func, bool reverse = false){
            if(!e) return nullptr;
            Edge* curr = e;
            do{
                func(curr);
                curr = (reverse ? curr -> prev : curr -> next);
            }while(curr && (curr != e));
            return curr;
        }
        
        /**
         * Function to add the given face to chain of edges having the next connection from given edge
         * @param e pointer to the edge
         * @param f pointer to the face; when null, new face is created
        */
        Face* addFaceAlong(Edge* e, Face* f = nullptr){
            if(!e || !(e -> next)) return nullptr;
            if(!f) f = new Face(e);
            if(!forEdgesAlong(e, [&f](Edge* e_){
                e_ -> left = f;
            })) return nullptr;
            return f;
        }
        
        /**
         * Function to split a face along given vertices if such a split does not already exist
         * @param v1 pointer to the first vertex
         * @param v2 pointer to the second vertex
         * @param status reference to a boolean; when unset, implies that such a split already existed
         */
        Edge* splitFace(Vertex* v1, Vertex* v2, bool &status){
            status = false;
            Edge* e = Vertex::getEdge(v1, v2);
            if(!e){
                status = true;
                e = new Edge(v1, v2);
                v1 -> inc_edge -> prev -> setNext(e, false);
                e -> twin -> setNext(v1 -> inc_edge, false);
                v2 -> inc_edge -> prev -> setNext(e -> twin, false);
                e -> setNext(v2 -> inc_edge, false);
                v2 -> inc_edge = e -> twin;
                Face* f1 = e -> next -> left;
                e -> left = f1;
                f1 -> inc_edge = e;
                Face* f2 = addFaceAlong(e -> twin);
                faces.push_back(f2);
            }
            return e;
        }

        /**
         * Function to merge two faces along the given edge
         * @param d pointer to the edge (a diagonal of the face)
         */
        Face* mergeFace(Edge* d){
            if(!d) return nullptr;
            Face* f1 = d -> left;
            Face* f2 = d -> right();
            if(!(f1) || !(f2)) return nullptr;
            if((d -> org -> inc_edge) == d){
                d -> org -> inc_edge = d -> twin -> next;
            }
            if((d -> dest() -> inc_edge) == (d -> twin)){
                d -> dest() -> inc_edge = d -> next;
            }
            if((f1 -> inc_edge) == d){
                f1 -> inc_edge = d -> next;
            }
            addFaceAlong(d -> twin, f1);
            d -> prev -> setNext(d -> twin -> next, false);
            d -> twin -> prev -> setNext(d -> next, false);
            for(auto it = faces.begin(); it != faces.end(); it++){
                if(*it == f2){
                    faces.erase(it);
                    break;
                }
            }
            delete f2;
            return f1;
        }
};

#endif