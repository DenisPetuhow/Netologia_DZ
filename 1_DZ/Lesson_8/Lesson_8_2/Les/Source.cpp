#include <cmath>
#include <iostream>

struct PolarPoint { // Полярная СС 
    float r; // растояние до точки
    float phi;  // угол между линиейи и осью

    PolarPoint(float r, float phi) noexcept : r(r), phi(phi) {}
};

struct CartesianPoint { //2-ая СС
    float x, y;
    CartesianPoint() = default;
    CartesianPoint(float x, float y) : x(x), y(y) {};
};

double distance(CartesianPoint* p1, CartesianPoint* p2) noexcept
{
    return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
};


struct PointAdapter : public CartesianPoint // реализовали через наследника
{
    explicit PointAdapter(const PolarPoint& p) noexcept : CartesianPoint() {
        x = p.r * cos(p.phi);
        y = p.r * sin(p.phi);
    }
};

struct PointAdapter2  // реализовали через композицию
{
    CartesianPoint c_pnt1;  // лежит объект 
    PointAdapter2(const PolarPoint& p)  : c_pnt1() 
    {
        c_pnt1.x = p.r * cos(p.phi);
        c_pnt1.y = p.r * sin(p.phi);
    }
};

int main() {
    std::cout << "Working with cartesian coordinates" << std::endl;
    auto cartesian_point_1 = new CartesianPoint(12.23, 34.43);
    auto cartesian_point_2 = new CartesianPoint(56.78, 90.12);
    std::cout << distance(cartesian_point_1, cartesian_point_2) << std::endl;

    std::cout << "Working with polar coordinates" << std::endl;
    PolarPoint polar_point_1(5, 0);
    PolarPoint polar_point_2(12.0, 3.14 / 2);
    PointAdapter adapted_point_1(polar_point_1);
    PointAdapter adapted_point_2(polar_point_2);
    std::cout << distance(std::addressof(adapted_point_1), std::addressof(adapted_point_2)) << std::endl;

    PointAdapter2 adapted_point_3(polar_point_1);
    PointAdapter2 adapted_point_4(polar_point_2);
    std::cout << distance(&adapted_point_3.c_pnt1, &adapted_point_4.c_pnt1);
    return 0;
}