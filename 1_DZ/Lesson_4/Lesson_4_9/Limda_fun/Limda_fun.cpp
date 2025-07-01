
#include <iostream>



int main()
{
    const auto func = [](int val)
        {
            std::cout << val << std::endl;
        };
    func(100);
    return 0;
}

