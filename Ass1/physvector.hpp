#ifndef PHYSVECTOR_HPP
#define PHYSVECTOR_HPP

#include <cmath>

const double PI = acos(-1);

/**
 * A class to define a physical vector
 */
class PhysicalVector{
    private:
        double x = 0.0, y = 0.0; ///< Coordinates of the start point of the vector
        double magnitude = 0.0; ///< Magnitude of the vector
        double angle = 0.0; ///< Angle subtended by the vector wrt the x-axis in anticlockwise direction
        void update(){
            magnitude = sqrt(x * x + y * y);
            if(magnitude == 0) { /** Case handling zero vector */
                angle = INFINITY;
                return;
            }
            angle = (y >= 0 ? acos(x / magnitude) : 2 * PI - acos(x / magnitude)); /** Case handling reflex angled vector */
            if(angle == (2 * PI)) angle = 0;
        }
    public:
        /**
         * Constructor for a physical vector with a start point
         * @param _x x-coordinate
         * @param _y y-coordinate
         */
        PhysicalVector(double _x, double _y){
            x = _x; y = _y;
            update();
        }

        /**
         * Constructor for a physical vector with a start and end point
         * @param x1 x-coordinate of first vertex
         * @param y1 y-coordinate of first vertex
         * @param x2 x-coordinate of second vertex
         * @param y2 y-coordinate of second vertex
         */
        PhysicalVector(double x1, double y1, double x2, double y2) : PhysicalVector(x2 - x1, y2 - y1){}

        /**
         * setter function for start coordinates of the vector
         */
        void getXY(double& _x, double& _y){
            _x = x; _y = y;
        }

        /**
         * getter function for magnitude of the vector
         */
        double getMagnitude(){
            return magnitude;
        }

        /**
         * getter function for angle of the vector
         */
        double getAngle(){
            return angle;
        }

        /**
         * conversion function for radians to degrees
         * @param theta angle value in radians
         */
        static double toDegree(double theta){
            return (theta * 180) / PI;
        }

        /**
         * conversion function for degrees to radians
         * @param theta angle value in degrees
         */
        static double toRadian(double theta){
            return (theta * PI) / 180;
        }

        /**
         * calculates the angle between two vectors
         * @param from a physical vector
         * @param to a physical vector
         */
        static double angleBetween(PhysicalVector from, PhysicalVector to){
            double toAngle = to.angle, fromAngle = from.angle;
            if(toAngle == INFINITY || fromAngle == INFINITY) return INFINITY;
            double theta = toAngle - fromAngle;
            if(theta < 0) theta += 2 * PI;
            return theta;
        }

        /**
         * calculates angle between two vectors given their common starting point and two end points
         * @param x1 x-coordinate of common starting point
         * @param y1 y-coordinate of common starting point
         * @param x2 x-coordinate of end point of first vector
         * @param y2 y-coordinate of end point of first vector
         * @param x3 x-coordinate of end point of second vector
         * @param y3 y-coordinate of end point of second vector
         */
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