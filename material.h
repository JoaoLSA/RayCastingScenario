// classe de material abstrato que encapsula o comportamento
#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "texture.h"

struct hit_record;

//Produza um raio espalhado (ou diga que ele absorveu o raio incidente).
//Se espalhado, diga o quanto o raio deve ser atenuado.
class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;

        virtual color emitted(double u, double v, const point3& p) const {
            return color(0,0,0);
        }
};
/*Para o caso lambertiano (difuso) que já temos, ele pode espalhar sempre e atenuar por sua refletância R, 
ou pode se espalhar sem atenuação, mas absorver a fração 1 - R dos raios, ou pode ser uma mistura dessas estratégias.*/
class lambertian : public material {
    public:
        //fazer materiais texturizados com um ponteiro de textura
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};
//para fazer um material emissor de luz
//apenas informa ao raio de que cor é e não realiza reflexão
class diffuse_light : public material  {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            return false;
        }

        virtual color emitted(double u, double v, const point3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};

#endif