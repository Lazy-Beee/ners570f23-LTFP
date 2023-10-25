#include "ExampleClass.h"


namespace LTFP
{
    ExampleClass* ExampleClass::current = nullptr;

    ExampleClass::ExampleClass()
    {

    }

    ExampleClass::~ExampleClass()
    {
        current = nullptr;
    }

    ExampleClass* ExampleClass::getCurrent()
    {
        if (current == nullptr)
            current = new ExampleClass();

        return current;
    }
}