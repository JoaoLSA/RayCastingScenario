#ifndef TRIANGLE_H
#define TRIANGLE_H



#include "rtweekend.h"

#include "hittable.h"

class triangle : public hittable {
    public:
        triangle() {}

        triangle(point3 v1,point3 v2, point3 v3)
            : vertex1(v1), vertex2(v2), vertex3(v3) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;


    public:
        point3 vertex1; 
        point3 vertex2;
        point3 vertex3;
};



bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // printf("The Ray origin  is %f %f %f \n",r.origin().x() ,r.origin().y(),r.origin().z());
    // printf("The Ray direction  is %f %f %f \n",r.direction().x() ,r.direction().y(),r.direction().z());
    vec3 t_center = (vertex1+vertex2+vertex3)/3;
    // printf("The Triangle center is %f %f %f \n",t_center.x() ,t_center.y(),t_center.z());
    vec3 w = t_center - r.origin();
    //calculate the edges vector and Calculate the plane normal
    vec3 v2v1 = vertex2 - vertex1;
    vec3 v3v1 = vertex3 - vertex1;
    vec3 nomral = unit_vector (cross(v2v1,v3v1));
    double a = dot(w,nomral);
    double b = dot(r.direction(),nomral);
    auto k = a/b;
    vec3 I = r.at(k);

    double t_area = cross(v2v1,v3v1).length()/2;

    //alpha
    auto IV3 = I - vertex3;
    auto IV2 = I - vertex2;
    double A_a = cross(IV2,IV3).length()/2;
    double alpha = A_a/t_area;

    //beta
    auto IV1 = I - vertex1;
    double A_b = cross(IV1,IV3).length()/2;
    double beta = A_b/t_area;

    //gamma
    double A_c = cross(IV1,IV2).length()/2;
    double gamma = A_c/t_area;
    
    double result = alpha+beta+gamma;
    
    double x = 0.01;

    auto check = abs(result-1);
    if( check>x)
    {
        // printf("barycentric fail\n");
        return false;
    }

    if(k < t_min || t_max < k)
    {
        return false;
    }
    rec.t = k;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, -nomral);
    return true;
}


#endif