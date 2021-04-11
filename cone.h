#ifndef CONE_H
#define CONE_H

#include "rtweekend.h"
#include "hittable.h"

class cone : public hittable {
public:
	cone() {}

	cone(point3 position, double r, double h)
        : position(position), radius(r), height(h) {};

	cone(point3 cen, double r, double h, shared_ptr<material> m)
		: position(position), radius(r), height(h), mat_ptr(m) {};

    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;

	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    point3 position;
    double radius;
    double height;
	shared_ptr<material> mat_ptr;
};

bool cone::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double root = (-height - (r.origin().z() -  position.z())) / r.direction().z();

	point3 point = (r.origin() + root * r.direction());
	double rad = (point.x() -  position.x()) * (point.x() -  position.x()) + (point.y() -  position.y()) * (point.y() -  position.y());

	if (rad < radius * radius && dot(r.direction(), vec3(0, 0, point.z() -  position.z())) < 0.0f) {
		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 diff = normalize(point - position);
		vec3 outward_normal = vec3(0, 0, -diff.z());
		rec.set_face_normal(r, outward_normal);
		return true;
	}

	vec3 dir_sq = r.direction() * r.direction();
	vec3 orig_sq = (r.origin() -  position) * (r.origin() -  position);
	vec3 or_dir = (r.origin() -  position) * r.direction();

	double a = dir_sq.x() + dir_sq.y() - dir_sq.z();
	double b = 2.f * (or_dir.x() + or_dir.y() - or_dir.z());
	double c = orig_sq.x() + orig_sq.y() - orig_sq.z();
	double delta = b * b - 4.f * a * c;

	//No intersection
	if (delta < 0.f) return false;

	double root1 = (-b + sqrt(delta)) / (2.f * a);
	double root2 = (-b - sqrt(delta)) / (2.f * a);

	if (root1 > root2) {
		double aux = root1;
		root1 = root2;
		root2 = aux;
	}
	if (root2 < 0) return false;

	root = root1 < 0.f ? root2 : root1;

	point = (r.origin() + root * r.direction());
	if (point.z() < - height +  position.z() || point.z() >  position.z()) return false;
	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 diff = normalize(point - position);
	vec3 outward_normal = vec3(-diff.x(), -diff.y(), diff.z());
	rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
	rec.u = 1;
	rec.v = 1;

}

bool cone::bounding_box(double t0, double t1, aabb& box) const {
  box = aabb(vec3(-radius,0,-radius), vec3(radius,height,radius));
  return(true);
}

#endif