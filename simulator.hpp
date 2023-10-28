#include <array>
#include <algorithm>


template<typename MP, int N>
class State
{
	public:
		typedef MP ftype;
		typedef std::array<MP, N> state_type ;
		state_type state;
		State(state_type init){
			std::copy_n(init.begin(), init.size(), state.begin());
		}
		
		// Эти две функции нужны, чтобы класс считался Iterable (родной python...)
		const size_t size() const{return state.size();}
		ftype& operator[](int i){return state[i];}
		
};

template<class StateClass>
class Equation
{
	public:
		typedef StateClass state_type;
		static StateClass dfdt(StateClass& state){
			// Очевидно, что это для одномерного случая
			return state_type(typename state_type::state_type {state[1], -1*state[0]});
		}
};

template<class EquationClass>
class Euler
{
	public:
		typedef typename EquationClass::state_type State;
		EquationClass equation;
		Euler(EquationClass equation): equation(equation){}
		State make_step(State& state, const float dt){
			auto f = equation.dfdt(state);
			for (int index = 0; index < 2; index++){
			//for (auto it = f.state.begin(); it != f.state.end(); ++it) {
			   //const int index = std::distance(f.state.begin(), it);
			   f[index] = state[index] + dt*f[index];
			}
			return f;
		}
	
};
