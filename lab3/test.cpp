#include <iostream>

int test(int *y) {
std::cout << y << std::endl;

}
int main () {
int y = 0;
int *x = &y;
test(x);
std::cout << x << std::endl;

return 0;
}
