#include <iostream>
#include <string>  // Ensure std::string is included
using namespace std;

// Define a class with different member functions
class MyClass {
public:
    // Member function with no parameters
    void sayHello() {
        cout << "Hello from MyClass!" << endl;
    }

    // Member function with one parameter
    void greet(std::string& a) {
        cout << "Sum: " << a << endl;
    }

    // Member function with two parameters
    void addAndPrint(int a, int b) {
        cout << "Sum: " << (a + b) << endl;
    }
};

// Template function to call a member function with any parameters
template <typename ReturnType, typename... Args>
void callMemberFunction(ReturnType(MyClass::* func)(Args...), MyClass& obj, Args&&... args) {
    // Call the member function with the provided arguments
    (obj.*func)(std::forward<Args>(args)...);  // Forward the arguments properly
}

int main() {
    MyClass obj;

    // Call member function with no parameters
    callMemberFunction(&MyClass::sayHello, obj);

    // Call member function with one parameter
    std::string v("2");
    callMemberFunction(&MyClass::greet, obj, v);  // Explicit conversion to std::string

    // Call member function with two parameters
    callMemberFunction(&MyClass::addAndPrint, obj, 10, 20);

    return 0;
}
