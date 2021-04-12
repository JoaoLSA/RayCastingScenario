#ifndef CYLINDER_H
#define CYLINDER_H

#include "rtweekend.h"
#include "hittable.h"
#include <iostream>
#include <cassert>

class cylinder : public hittable {
public:
    cylinder() {}

    cylinder(point3 position, double r, double h)
        : position(position), radius(r), height(h) {};

	cylinder(point3 position, double r, double h, shared_ptr<material> m)
		: position(position), radius(r), height(h), mat_ptr(m) {};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;

public:
    point3 position;
    double radius;
    double height;
	shared_ptr<material> mat_ptr;
};

bool cylinder::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	const double EPS = 0.006f;
	double root1 = (-height - (r.origin().z() - position.z())) / r.direction().z();
	double root2 = (height - (r.origin().z() - position.z())) / r.direction().z();

	if (root1 > root2) {
		double aux = root1;
		root1 = root2;
		root2 = aux;
	}

	double root = root1 < 0.f ? root2 : root1;
	vec3 point = (r.origin() + root * r.direction());
	double rad = (point.x() - position.x()) * (point.x() -  position.x()) + (point.y() -  position.y()) * (point.y() -  position.y());

	if (rad <  radius *  radius && dot(r.direction(), vec3(0, 0, point.z() - position.z())) < 0.0f) {
		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = vec3(0, 0, 0);
		if (abs(root - root1) <= EPS) {  // closest dist is actual root
			outward_normal = vec3(0, 0, -1);
		} else if (abs(root - root2) <= EPS) {
			outward_normal = vec3(0, 0, 1);
		} else {
			vec3 diff = normalize(point - position);
			double theta = atan(diff.y() / diff.x());
			outward_normal = vec3(cos(theta), sin(theta), 0);
		}
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = mat_ptr;


		return true;
	}

	double a = dot_xy(r.direction(), r.direction());
	double b = 2.f * dot_xy(r.origin() - position, r.direction());
	double c = -(radius * radius) + dot_xy(r.origin() - position, r.origin() - position);

	double delta = b * b - 4.f * a * c;

	//No intersection
	if (delta < 0.f) return false;

	root1 = (-b + sqrt(delta)) / (2.f * a);
	root2 = (-b - sqrt(delta)) / (2.f * a);

	if (root1 > root2) {
		double aux = root1;
		root1 = root2;
		root2 = aux;
	}
	if (root2 < 0) return false;

	root = root1 < 0.f ? root2 : root1;

	point = (r.origin() + root * r.direction());
	if (point.z() < -height + position.z()) return false;
	if (point.z() > height + position.z()) return false;

	if (root < t_min || t_max < root)
		return false;

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = vec3(0,0,0);
	if (abs(root - root1) <= EPS) {  // closest dist is actual root
		outward_normal = vec3(0, 0, -1);
	} else if (abs(root - root2) <= EPS) {
		outward_normal = vec3(0, 0, 1);
	} else {
		vec3 diff = normalize(point - position);
		double theta = atan(diff.y() / diff.x());
		outward_normal = vec3(cos(theta), sin(theta), 0);
	}
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;
	return true;
}

bool cylinder::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(position - point3(radius, height/2, radius), position + point3(radius, height/2, radius));
    return true;
}

#endif