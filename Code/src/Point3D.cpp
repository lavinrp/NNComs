#include "Point3D.h"
#include <cmath>
using namespace std;

#pragma region Constructors / destructor
Point3D::Point3D() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point3D::Point3D(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}


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
double Point3D::getX() const {
	return this->x;
}

double Point3D::getY() const {
	return this->y;
}

double Point3D::getZ() const {
	return this->z;
}
#pragma endregion

