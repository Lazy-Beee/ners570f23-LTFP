#ifndef __ExampleSingleton__
#define __ExampleSingleton__

#include "Common.hpp"
#include <vector>

namespace LTFP
{
	/*
	How to access this singleton class:
	- Include header file: #include "ExampleClass.hpp"
	- Get the pointer to the class object: ExampleClass* ec = ExampleClass::getCurrent()
	- Or call the class function in one line: ExampleClass::getCurrent()->step()
	Usage of accessor functions:
	- get with const: const int var = ec->getVar(i)
	- get with &: const int& var = ec->getVar(i) OR int& var = ec->getVar(i)
	- set: ec->setVar(i, value)
	*/

	class ExampleClass
	{
	private:
		static ExampleClass* current;

	protected:
        std::vector<Real> exampleVariable;

	public:
		ExampleClass();
		ExampleClass(const ExampleClass&) = delete;
		ExampleClass& operator=(const ExampleClass&) = delete;
		~ExampleClass();

		static ExampleClass* getCurrent();
        
		inline const Real getExampleVariable(int i) const { return exampleVariable[i]; }
        inline Real& getExampleVariable(int i) { return exampleVariable[i]; }
        inline void setExampleVariable(int i, Real val) { exampleVariable[i] = val; }
	};
}


#endif