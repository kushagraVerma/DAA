#ifndef PHYSVECTOR_HPP
#define PHYSVECTOR_HPP

#include <cmath>

const double PI = acos(-1);

/**
 * A class to define a physical vector
 */
class PhysicalVector{
    private:
        double x = 0.0, y = 0.0; ///< Coordinates of the tail of the vector
        double magnitude = 0.0; ///< Magnitude of the vector
        double angle = 0.0; ///< Angle subtended by the vector wrt the x-axis in anticlockwise direction
        void update(){
            magnitude = sqrt(x * x + y * y);
            /** Case handling zero vector */
            if(magnitude == 0) {
                angle = INFINITY;
                return;
            }
            angle = (y >= 0 ? acos(x / magnitude) : 2 * PI - acos(x / magnitude)); /** Case handling reflex angled vector */
            if(angle == (2 * PI)) angle = 0;
        }
    public:
        /**
         * Constructor for a physical vector given a head point
         * @param _x x-coordinate of head
         * @param _y y-coordinate of head
         */
        PhysicalVector(double _x, double _y){
            x = _x; y = _y;
            update();
        }

        /**
         * Constructor for a physical vector given a tail and head point
         * @param x1 x-coordinate of tail
         * @param y1 y-coordinate of tail
         * @param x2 x-coordinate of head
         * @param y2 y-coordinate of head
         */
        PhysicalVector(double x1, double y1, double x2, double y2) : PhysicalVector(x2 - x1, y2 - y1){}

        /**
         * Getter function for start coordinates of the vector
         * @param _x reference to a double, takes the value of the x-coordinate
         * @param _y reference to a double, takes the value of the y-coordinate
         */
        void getXY(double& _x, double& _y){
            _x = x; _y = y;
        }

        /**
         * Getter function for magnitude of the vector
         */
        double getMagnitude(){
            return magnitude;
        }

        /**
         * Getter function for angle of the vector
         */
        double getAngle(){
            return angle;
        }

        /**
         * Conversion function for radians to degrees
         * @param theta angle value in radians
         */
        static double toDegree(double theta){
            return (theta * 180) / PI;
        }

        /**
         * Conversion function for degrees to radians
         * @param theta angle value in degrees
         */
        static double toRadian(double theta){
            return (theta * PI) / 180;
        }

        /**
         * Calculates the angle between two vectors
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
         * Given 3 points, creates 2 vectors having the 2nd point as the common tail and returns the angle between them
         * @param x1 x-coordinate of 1st point (head of 1st vector)
         * @param y1 y-coordinate of 1st point (head of 1st vector)
         * @param x2 x-coordinate of 2nd point (common tail)
         * @param y2 y-coordinate of 2nd point (common tail)
         * @param x3 x-coordinate of 3rd point (head of 2nd vector)
         * @param y3 y-coordinate of 3rd point (head of 2nd vector)
         */
        static double angleBetween(double x1, double y1, double x2, double y2, double x3, double y3){
            PhysicalVector from(x2, y2, x1, y1), to(x2, y2, x3, y3);
            return angleBetween(from, to);
        }

        /**
         * Returns a rotated vector rotated by an angle theta
         * @param theta angle offset
         */
        PhysicalVector rotated(double theta){
            if(angle == INFINITY) return *this;
            double a_ = angle + theta;
            return PhysicalVector(magnitude * cos(a_), magnitude * sin(a_));
        }

        /**
         * Assigns the rotated physical vector to the current vector
         * @param theta angle offset
         */
        void rotate(double theta){
            *this = rotated(theta);
        }

        /**
         * Operator overload to add a physical vector to another
         * @param other a physical vector
         */
        PhysicalVector operator + (PhysicalVector other){
            return PhysicalVector(x + other.x, y + other.y);
        }

        /**
         * Scale a vector by a factor
         * @param factor a double
         */
        PhysicalVector operator * (double factor){
            return PhysicalVector(x * factor, y * factor);
        }

        /**
         * Equate a vector to another vector
         * @param other a physical vector
         */        
        PhysicalVector operator = (PhysicalVector other){
            x = other.x; y = other.y;
            update();
            return *this;
        }

        /**
         * Add a vector to another vector
         * @param other a physical vector
         */
        PhysicalVector operator += (PhysicalVector other){
            return ((*this) = (*this) + other);
        }

        /**
         * Scale a vector by a factor
         * @param factor a double
         */
        PhysicalVector operator *= (double factor){
            return ((*this) = (*this) * factor);
        }
};

#endif