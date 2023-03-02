#ifndef DCEL_HPP
#define DCEL_HPP

#include <vector>
#include <functional>
// #include <iostream>

using namespace std;

#define toCoord(v) v->x, v->y
#define vpvect vector<Vertex*>

class Edge;
class Face{
    public:
        Edge* inc_edge = nullptr;
        bool isInc(Edge* e){
            return e==inc_edge;
        }
};
class Vertex{
    public:
        double x = 0.0, y = 0.0;
        Edge* inc_edge = nullptr;
        Vertex(double _x, double _y){
            x = _x; y = _y;
        }
        Edge* edgeTo(Vertex* v);
        bool isInc(Edge* e){
            return e==inc_edge;
        }
        void clearEdge();
        void foreachIncEdge(function<void(Edge*)> func);
        Vertex* nextV();
        Vertex* prevV();
        static Edge* getEdge(Vertex* a, Vertex* b);
        static Vertex* midpoint(Vertex* a, Vertex* b){
            vector<Vertex*> vlist = {a,b};
            return centroid(vlist);
        }
        static Vertex* centroid(vector<Vertex*> &vlist){
            double xm = 0, ym = 0;
            for(auto v : vlist){
                xm+=v->x; ym+=v->y;
            }
            xm/=vlist.size(); ym/=vlist.size();
            return new Vertex(xm,ym);
        }
        static bool coincides(Vertex* a, Vertex* b){
            return (a->x == b->x && a->y == b->y);
        }
        bool operator<(Vertex* other){
            return (x==other->x?y<other->y:x<other->x);
        }
        bool operator==(Vertex* other){
            return (x==other->x && y==other->y);
        }
};
class Edge{
    public:
        Edge* twin = nullptr;
        Edge* next = nullptr;
        Edge* prev = nullptr;
        Vertex* org = nullptr;
        Face* left = nullptr;
        Edge(Vertex* _org){
            org = _org;
        }
        Edge(Vertex* _org, Edge* _twin) : Edge(_org){
            twin = _twin;
        }
        Edge(Vertex* _org, Vertex* _dest) : Edge(_org){
            twin = new Edge(_dest, this);
        }
        ~Edge(){
            if(twin){
                twin->twin = nullptr;
                delete twin;
            }
        }
        Vertex* dest(){
            return twin->org;
        }
        void setNext(Edge* e, bool setTwins = true){
            next = e;
            e->prev = this;
            if(setTwins){
                e->twin->next = twin;
                twin->prev = e->twin;
            }
        }
        Edge* splitAt(Vertex* v){
            Vertex* dst = dest();
            twin->org = v;
            v->edgeTo(dst);
            v->inc_edge->setNext(next);
            setNext(v->inc_edge);
            return v->inc_edge;
        }
        Edge* split(){
            Vertex* mp = Vertex::midpoint(org,dest());
            Edge* e = splitAt(mp);
            delete mp;
            return e;
        }
        static void splice(Edge* a1, Edge* a2, Edge* b1, Edge* b2){
            delete b2->org;
            b1->twin->org = b2->org = a2->org;
            a1->setNext(b1->twin,false);
            b1->setNext(a2,false);
            a2->twin->setNext(b2,false);
            b2->twin->setNext(a1->twin,false);
        }
        static void merge(Edge* a1, Edge* b1){
            Edge* a2 = a1->split();
            Edge* b2 = b1->split();
            splice(a1,a2,b1,b2);
        }
        static int coincides(Edge* a, Edge* b){
            if(Vertex::coincides(a->org,b->org) && Vertex::coincides(a->dest(),b->dest())){
                return 1;
            }
            if(Vertex::coincides(a->org,b->dest()) && Vertex::coincides(a->dest(),b->org)){
                return -1;
            }
            return 0;
        }
};

Edge* Vertex::getEdge(Vertex* a, Vertex* b){
    Edge* E = nullptr;
    a->foreachIncEdge([&b,&E](Edge* e){
        if(Vertex::coincides(b,e->dest())){
            E = e;
        }
    });
    return E;
}
Edge* Vertex::edgeTo(Vertex* v){
    Edge* existing = Vertex::getEdge(this,v);
    return (inc_edge = (existing? existing : new Edge(this,v)));
}
void Vertex::clearEdge(){
    delete inc_edge;
    inc_edge = nullptr;
}
void Vertex::foreachIncEdge(function<void(Edge*)> func){
    if(!this->inc_edge) return;
    Edge* start = this->inc_edge;
    do{
        func(start);
        if(start->twin && start->twin->next){
            start = start->twin->next;
        }else{
            return;
        }
    }while(start!=this->inc_edge);
}
Vertex* Vertex::nextV(){
    if(inc_edge){
        return inc_edge->dest();
    }else{
        return nullptr;
    }
}
Vertex* Vertex::prevV(){
    if(inc_edge && inc_edge->prev){
        return inc_edge->prev->org;
    }else{
        return nullptr;
    }
}

class DCEL{
    private:
        void addVert(Vertex* v){
            Edge* e = last->inc_edge; //...<-e-last
            last->edgeTo(v);//...<-e-last--->v
            v->inc_edge = last->inc_edge->twin;//...<-e-last<===>v
            if(n>1){ //last!=start
                v->inc_edge->setNext(e);
            }
            last = v;
            n++;
        }
    public:
        Vertex* start = nullptr;
        Vertex* last = nullptr;
        vector<Edge*> edges;
        int n = 0;
        DCEL(){}
        DCEL(Vertex* _start){
            start = _start;
            last = start;
            n = 1;
        }
        DCEL(vpvect &vlist) : DCEL(vlist[0]){
            int _n = vlist.size();
            if(_n>1){
                for(int i = 1; i < _n; i++){
                    addVert(vlist[i]);
                }
                Edge* e = last->inc_edge; //...<-e-last
                last->edgeTo(start);//...<-e-last--->start
                e->twin->setNext(last->inc_edge);
                last->inc_edge->setNext(start->inc_edge);
                Edge* curr = e;
                do{
                    edges.push_back(curr);
                    curr->org->inc_edge = curr;
                    // curr->twin->next = nullptr;
                    // curr->twin->prev = nullptr;
                    curr = curr->next;
                }while(curr!=e);
            }
        }
        void foreachVert(function<void(Vertex*)> func){
            Vertex* curr = start;
            do{
                func(curr);
                if(curr->inc_edge && curr->inc_edge->next) 
                    curr = curr->inc_edge->next->org;
            }while(curr!=start);
        }
};

#endif