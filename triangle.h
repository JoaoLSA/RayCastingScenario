#ifndef TRIANGLE_H
#define TRIANGLE_H



#include "rtweekend.h"

#include "hittable.h"

class triangle : public hittable {
    public:
        triangle() {}

        triangle(point3 v1,point3 v2, point3 v3, shared_ptr<material> m)
            : vertex1(v1), vertex2(v2), vertex3(v3), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;


    public:
        point3 vertex1; 
        point3 vertex2;
        point3 vertex3;
        shared_ptr<material> mat_ptr;
};



bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // printf("The Ray origin  is %f %f %f \n",r.origin().x() ,r.origin().y(),r.origin().z());
    // printf("The Ray direction  is %f %f %f \n",r.direction().x() ,r.direction().y(),r.direction().z());
    vec3 t_center = (vertex1+vertex2+vertex3)/3;
    // printf("The Triangle center is %f %f %f \n",t_center.x() ,t_center.y(),t_center.z());
    vec3 w = t_center - r.origin();
    //calculate the edges vector and Calculate the plane normal
    vec3 v2v1 = vertex2 - vertex1;
    // printf("v2v1 vector t is %f %f %f \n",v2v1.x() ,v2v1.y(),v2v1.z());
    vec3 v3v1 = vertex3 - vertex1;
    // printf("v3v1 vector is %f %f %f \n",v3v1.x() ,v3v1.y(),v3v1.z());
    vec3 nomral = unit_vector (cross(v2v1,v3v1));
    double a = dot(w,nomral);
    // printf("a value is : %f \n",a);
    double b = dot(r.direction(),nomral);
    // printf("b value is : %f \n",b);
    auto k = a/b;
    // printf("k value is : %f \n",k);
    // vec3 I = r.direction().operator*=(k).operator+=(r.origin());
    vec3 I = r.at(k);
    // printf("The Intersection point is %f %f %f \n",I.x() ,I.y(),I.z());

    double t_area = cross(v2v1,v3v1).length()/2;
    // printf("triangle area: %f \n",t_area);

    //alpha
    auto IV3 = I - vertex3;
    // printf("IV3 vector is %f %f %f \n",IV3.x() ,IV3.y(),IV3.z());
    auto IV2 = I - vertex2;
    // printf("IV2 vector is %f %f %f \n",IV2.x() ,IV2.y(),IV2.z());
    double A_a = cross(IV2,IV3).length()/2;
    // printf("area_a value: %f \n",A_a);
    double alpha = A_a/t_area;
    // printf("alpha value: %f \n",alpha);

    //beta
    auto IV1 = I - vertex1;
    double A_b = cross(IV1,IV3).length()/2;
    // printf("area_b value: %f \n",A_b);
    double beta = A_b/t_area;
    // printf("beta value: %f \n",beta);

    //gamma
    double A_c = cross(IV1,IV2).length()/2;
    // printf("area_c value: %f \n",A_c);
    double gamma = A_c/t_area;
    // printf("gamma value: %f \n",gamma);
    
    double result = alpha+beta+gamma;
    // printf("Result: %f \n",result);
    
    double x = 0.01;

    auto check = abs(result-1);
    // printf("check value is %f:",check);
    if( check>x)
    {
        // printf("barycentric fail\n");
        return false;
    }

    if(k < t_min || t_max < k)
    {
        return false;
    }
    // printf("yooooo we made it\n");
    rec.t = k;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, -nomral);
    rec.mat_ptr = mat_ptr;
    return true;
}


#endif