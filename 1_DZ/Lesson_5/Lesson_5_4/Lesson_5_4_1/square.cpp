#include "square.h"

// –еализаци€ основного шаблона дл€ простых типов данных
template<typename T>
T square(const T& value) {
    return value * value;
}
// явные инстанциации дл€ всех нужных типов
template int square<int>(const int&);
template double square<double>(const double&);
template float square<float>(const float&);