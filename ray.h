#ifndef RAY_H
#define RAY_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "vec3.h"

// Raio é uma função P(t)=A + t.b
//A (point3) = raio de origem; b (direction) = direção do raio; t (tm)= um número real
//mudar t faz o ponto P(t) se mover ao longo do raio;
// t negativo move antes de A, t positivo move para frente de A
class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction), tm(0)
        {}
        //armazenamento de raios no momento em que ele existe
        ray(const point3& origin, const vec3& direction, double time)
            : orig(origin), dir(direction), tm(time)
        {}

        point3 origin() const  { return orig; }
        vec3 direction() const { return dir; }
        double time() const    { return tm; }
        //Resultado da função P(t)=A + t.b
        point3 at(double t) const {
            return orig + t*dir;
        }

    public:
        point3 orig;
        vec3 dir;
        double tm;
};

#endif
