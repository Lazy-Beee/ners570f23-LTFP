#ifndef __ExampleSingleton__
#define __ExampleSingleton__

#include "Common.hpp"
#include <vector>

namespace LTFP
{
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
        
		inline Real getExampleVariable(int i) const { return exampleVariable[i]; }
        inline Real& getExampleVariable(int i) { return exampleVariable[i]; }
        inline void setExampleVariable(int i, Real val) { exampleVariable[i] = val; }
	};
}


#endif