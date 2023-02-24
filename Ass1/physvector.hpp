#ifndef PHYSVECTOR_HPP
#define PHYSVECTOR_HPP

#include <cmath>

const double PI = acos(-1);

class PVect{
    private:
        double x = 0.0, y = 0.0, m = 0.0, a = 0.0;
        void update(){
            m = mag();
            a = heading();
        }
    public:
        PVect(double _x, double _y){
            x = _x; y = _y;
            update();
        }
        PVect(double x1, double y1, double x2, double y2) : PVect(x2-x1,y2-y1){}
        void getXY(double& _x, double& _y){
            _x = x; _y = y;
        }
        double mag(){
            return sqrt(x*x+y*y);
        }
        double heading(){
            if(m==0) return INFINITY;
            return (y>0? acos(x/m) : 2*PI - acos(x/m));
        }
        static double toDeg(double theta){
            return theta*180/PI;
        }
        static double toRad(double theta){
            return theta*PI/180;
        }
        static double angleBetween(PVect from, PVect to){
            double ta = to.heading(), fa = from.heading();
            if(ta==INFINITY || fa==INFINITY) return INFINITY;
            double theta = ta - fa;
            if(theta<0) theta+=2*PI;
            return theta;
        }
        static double angleBetween(double x1, double y1, double x2, double y2, double x3, double y3){
            PVect from(x2,y2,x1,y1), to(x2,y2,x3,y3);
            return angleBetween(from,to);
        }
        PVect rotated(double theta){
            if(a==INFINITY) return *this;
            double a_ = a+theta;
            return PVect(m*cos(a_),m*sin(a_));
        }
        void rotate(double theta){
            *this = rotated(theta);
        }
        PVect operator + (PVect pv){
            return PVect(x+pv.x,y+pv.y);
        }
        PVect operator * (double f){
            return PVect(x*f,y*f);
        }
        PVect operator = (PVect pv){
            x = pv.x; y = pv.y;
            update();
            return *this;
        }
        PVect operator += (PVect pv){
            return ((*this) = (*this) + pv);
        }
        PVect operator *= (double f){
            return ((*this) = (*this) * f);
        }
};

#endif