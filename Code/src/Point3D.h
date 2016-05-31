#pragma once

//the number of doubles that need to be passed by a game to create
//a full Point3D
const unsigned int DOUBLES_PER_POINT3D = 3;

class Point3D
{
public:
	//Constructors / destructor
	Point3D();
	Point3D(double x, double y, double z);
	Point3D(const Point3D& other);
	virtual ~Point3D();

	//Member Functions
	double distance(const Point3D& other) const;
	double angle2D(const Point3D& other) const;

	double xDifference(const Point3D& other) const;
	double yDifference(const Point3D& other) const;
	double zDifference(const Point3D& other) const;

	//getters / setters
	double getX() const;
	void setX(double newX);
	double getY() const;
	void setY(double newY);
	double getZ() const;
	void setZ(double newZ);
	void operator=(const Point3D& other);

protected:
	//x, y, and z positions of the Point3D (Cartesian coordinates)
	double x;
	double y;
	double z;
};

