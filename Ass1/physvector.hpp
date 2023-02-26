#ifndef PHYSVECTOR_HPP
#define PHYSVECTOR_HPP

#include <cmath>

const double PI = acos(-1);

class PhysicalVector{
    private:
        double x = 0.0, y = 0.0, magnitude = 0.0, angle = 0.0;
        void update(){
            magnitude = sqrt(x * x + y * y);
            if(magnitude == 0){
                angle = INFINITY;
                return;
            }
            angle = (y >= 0 ? acos(x / magnitude) : 2 * PI - acos(x / magnitude));
            if(angle == (2 * PI)) angle = 0;
        }
    public:
        PhysicalVector(double _x, double _y){
            x = _x; y = _y;
            update();
        }
        PhysicalVector(double x1, double y1, double x2, double y2) : PhysicalVector(x2 - x1, y2 - y1){}
        void getXY(double& _x, double& _y){
            _x = x; _y = y;
        }
        double getMagnitude(){
            return magnitude;
        }
        double getAngle(){
            return angle;
        }
        static double toDegree(double theta){
            return (theta * 180) / PI;
        }
        static double toRadian(double theta){
            return (theta * PI) / 180;
        }
        static double angleBetween(PhysicalVector from, PhysicalVector to){
            double toAngle = to.angle, fromAngle = from.angle;
            if(toAngle == INFINITY || fromAngle == INFINITY) return INFINITY;
            double theta = toAngle - fromAngle;
            if(theta < 0) theta += 2 * PI;
            return theta;
        }
        static double angleBetween(double x1, double y1, double x2, double y2, double x3, double y3){
            PhysicalVector from(x2, y2, x1 ,y1), to(x2, y2, x3, y3);
            return angleBetween(from, to);
        }
        PhysicalVector rotated(double theta){
            if(angle == INFINITY) return *this;
            double a_ = angle + theta;
            return PhysicalVector(magnitude * cos(a_), magnitude * sin(a_));
        }
        void rotate(double theta){
            *this = rotated(theta);
        }
        PhysicalVector operator + (PhysicalVector physicalVector){
            return PhysicalVector(x + physicalVector.x, y + physicalVector.y);
        }
        PhysicalVector operator * (double factor){
            return PhysicalVector(x * factor, y * factor);
        }
        PhysicalVector operator = (PhysicalVector physicalVector){
            x = physicalVector.x; y = physicalVector.y;
            update();
            return *this;
        }
        PhysicalVector operator += (PhysicalVector physicalVector){
            return ((*this) = (*this) + physicalVector);
        }
        PhysicalVector operator *= (double factor){
            return ((*this) = (*this) * factor);
        }
};

#endif