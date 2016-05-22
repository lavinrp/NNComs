#pragma once
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
	double getY() const;
	double getZ() const;
	void operator=(const Point3D& other);

private:
	double x;
	double y;
	double z;
};

