#include "Point3D.h"
#include <cmath>
using namespace std;

#pragma region Constructors / destructor
/*Default constructor for Point3D
Creates a point at location 0,0,0*/
Point3D::Point3D() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

/*Constructor for Point3D
creates a point at the given x, y, and z locations
@param x: x position
@param y: y position
@param z: z position*/
Point3D::Point3D(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

/*Copy constructor for Point3D
Creates a Point3D with the x, y, and z values of the passed Point3D
@param other: Point3D to copy*/
Point3D::Point3D(const Point3D& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

/*destructor for Point3D*/
Point3D::~Point3D() {
}
#pragma endregion


#pragma region member functions

/*distance
calculates the 3D distance between this Point3D and another
@param other: Point3D being compared
@return: distance between this and other*/
double Point3D::distance(const Point3D& other) const{
	double dx = this->getX() - other.getX();
	double dy = this->getY() - other.getY();
	double dz = this->getZ() - other.getZ();

	return sqrt(dx*dx + dy*dy + dz*dz);
}

/*angle2D
Returns angle (in radians) of a from this Point3D to another on a 2D plane
@param other: Point3D being compared
@return: angle between other and this*/
double Point3D::angle2D(const Point3D& other) const{
	return atan2(yDifference(other), xDifference(other));
}

/*xDifference
returns the difference in x values between this Point3D and another
@param other: Point3D being compared
@return: difference in x values between this and other*/
double Point3D::xDifference(const Point3D & other) const {
	return this->x - other.x;
}

/*yDifference
returns the difference in y values between this Point3D and another
@param other: Point3D being compared
@return: difference in y values between this and other*/
double Point3D::yDifference(const Point3D & other) const {
	return this->y - other.y;
}

/*zDifference
returns the difference in z values between this Point3D and another
@param other: Point3D being compared
@return: difference in z values between this and other*/
double Point3D::zDifference(const Point3D & other) const {
	return this->z - other.z;
}

#pragma endregion


#pragma region getters / setters
/*getX
returns the X position of this Point3D
@returns: x*/
double Point3D::getX() const {
	return this->x;
}

/*setX
sets the x position of the Point3D
@param newX: new x position*/
void Point3D::setX(double newX) {
	x = newX;
}

/*getY
returns the y position of this Point3D
@returns: y*/
double Point3D::getY() const {
	return this->y;
}

/*setY
sets the y position of the Point3D
@param newY: y position of the Point3D*/
void Point3D::setY(double newY) {
	y = newY;
}

/*getZ
returns the z position of this Point3D
@returns: z*/
double Point3D::getZ() const {
	return this->z;
}

/*setZ
sets the z position of the Point3D
@param newZ: new z position of the Point3D*/
void Point3D::setZ(double newZ) {
	z = newZ;
}



/*Assignment operator
assigns the values of this Point3D to those of the passed Point3D
@param other: Point3D whose values will be copied.*/
void Point3D::operator=(const Point3D& other) {
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

#pragma endregion

