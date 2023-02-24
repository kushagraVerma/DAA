#ifndef DCEL_HPP
#define DCEL_HPP

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
        Edge* edgeTo(Vertex* v){
            return (inc_edge = new Edge(this,v));
        }
        bool isInc(Edge* e){
            return e==inc_edge;
        }
        void clearEdge(){
            delete inc_edge;
            inc_edge = nullptr;
        }
        static Vertex* midpoint(Vertex* a, Vertex* b){
            double xm = (a->x + b->x)/2;
            double ym = (a->y + b->y)/2;
            return new Vertex(xm,ym);
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
            e->prev = next;
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
};

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
        int n = 0;
        DCEL(Vertex* _start){
            start = _start;
            last = start;
            n = 1;
        }
        DCEL(int _n, Vertex** vlist) : DCEL(vlist[0]){
            for(int i = 1; i < _n; i++){
                addVert(vlist[i]);
            }
            Edge* e = last->inc_edge; //...<-e-last
            last->edgeTo(start);//...<-e-last--->start
            e->twin->setNext(last->inc_edge);
            last->inc_edge->setNext(start->inc_edge);
        }
};

#endif