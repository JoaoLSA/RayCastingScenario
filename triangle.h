#pragma once
#ifndef TRIANGLEH
#define TRIANGLEH

#include "hittable.h"
#define EPS 0.0000001

class triangle : public hittable {

public:
	triangle() {}
	triangle(vec3 _v0, vec3 _v1, vec3 _v2, shared_ptr<material> m) : v0(_v0), v1(_v1), v2(_v2), mat_ptr(m) { }
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& b) const;

	vec3 v0, v1, v2;
	shared_ptr<material> mat_ptr;
};



bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

	vec3 e1, e2, h, s, q;
	double a, f, u, v;
	e1 = v1 - v0;
	e2 = v2 - v0;
	h = cross(r.direction(), e2);
	a = dot(e1, h);
	if (a > -EPS && a < EPS) {
		return false;
	}

	f = 1 / a;
	s = r.origin() - v0;
	u = f * dot(s, h);
	if (u < 0.0 || u > 1.0) {
		return false;
	}

	q = cross(s, e1);
	v = f * dot(r.direction(), q);
	if (v < 0.0 || u + v > 1.0) {
		return false;
	}

	double t = f * dot(e2, q);
	if (t > EPS) {

		rec.t = t;
		rec.p = r.at(t);
		rec.mat_ptr = mat_ptr;
		rec.normal = cross(e1, e2);
		return true;
	}
	else {
		return false;
	}
}

bool triangle::bounding_box(double t0, double t1, aabb& b) const {
	
	vec3 min(fmin(fmin(v0.x(), v1.x()), v2.x()),
		fmin(fmin(v0.y(), v1.y()), v2.y()),
		fmin(fmin(v0.z(), v1.z()), v2.z()));
	vec3 max(fmax(fmin(v0.x(), v1.x()), v2.x()),
		fmax(fmin(v0.y(), v1.y()), v2.y()),
		fmax(fmin(v0.z(), v1.z()), v2.z()));
	b = aabb(min, max);
	if (abs(b.max().x() - b.min().x()) < 0.0001f) {
		b.min().e[0] -= 0.0001f;
		b.max().e[0] += 0.0001f;
	}
	if (abs(b.max().y() - b.min().y()) < 0.0001f) {
		b.min().e[1] -= 0.0001f;
		b.max().e[1] += 0.0001f;
	}
	if (abs(b.max().z() - b.min().z()) < 0.0001f) {
		b.min().e[2] -= 0.0001f;
		b.max().e[2] += 0.0001f;
	}

	return true;
}

#endif