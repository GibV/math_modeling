#include <iostream>
#include <fstream>
#include <string>
#include "simulator.hpp"
#include "json.hpp"
#include <cmath>
#include <string>

using json = nlohmann::json;
typedef float precision_type;

template<class T>
class PhysEq{
	public:
		typedef T state_type;
		precision_type omega;
		PhysEq(precision_type omega): omega(omega) {}
		state_type dfdt(state_type& state){
			// Очевидно, что это для одномерного случая
			return state_type(typename state_type::state_type {state[1], -omega*omega*std::sin(state[0])});
		}
};
template<class T>
class MathEq{
	public:
		typedef T state_type;
		precision_type omega;
		MathEq(precision_type omega): omega(omega) {}
		state_type dfdt(state_type& state){
			// Очевидно, что это для одномерного случая
			return state_type(typename state_type::state_type {state[1], -omega*omega*state[0]});
		}
};

typedef State<precision_type, 2> state;
// typedef Euler<Equation<state, ??>> method;

int main(int argc, char** argv) 
{
	if(argc == 1){
		std::cout << "Whhh, programm exited 2 comand-line arguments, but you give " << argc-1;
		return 1;
	}
	std::ifstream config_file(argv[1]);
	json config = json::parse(config_file);
	
	precision_type r, last_r;
	precision_type v, last_v;
	r = last_r = config["start_r"];
	v = last_v = config["start_v"];
	unsigned int Niterations = config["iterations"];
	precision_type delta_t = config["delta_t"];
	precision_type omega = config["omega"];
	std::string fout_name = config["output_file"];
	unsigned int write_freq = config["write_freq"];
	//config_file >> last_r >> last_v >> Niterations >> delta_t >> omega >> write_freq;
	//std::cout << "WTF" << std::endl << last_r << std::endl << last_v << std::endl << Niterations << std::endl << delta_t << std::endl << omega << std::endl << fout_name << std::endl << write_freq;
	config_file.close();
		
	std::ofstream fout;
	fout.open(fout_name);
	
	if(config["mode"] == "math"){
		MathEq<state> equation(omega);
		Euler<MathEq<state>> method(equation);
		
		state init {std::array<precision_type,2>{last_r, last_v}};
		// std::cout << "hello" << std::endl;
		auto step = method.make_step(init, delta_t);
		for(int i = 0; i < Niterations; i++){
			if(i%write_freq==0){
				fout << step[0] << " " << step[1] << std::endl;
			}
			std::swap(step, init);
			step = method.make_step(init, delta_t);
		}
		fout << step[0] << " " << step[1];
	}else if(config["mode"] == "phys"){
		PhysEq<state> equation(omega);
		Euler<PhysEq<state>> method(equation);
		
		state init {std::array<precision_type,2>{last_r, last_v}};
		// std::cout << "hello" << std::endl;
		auto step = method.make_step(init, delta_t);
		for(int i = 0; i < Niterations; i++){
			if(i%write_freq==0){
				fout << step[0] << " " << step[1] << std::endl;
			}
			std::swap(step, init);
			step = method.make_step(init, delta_t);
		}
		fout << step[0] << " " << step[1];
	}else{
		std::cout << "Arghh, mode is unrecognised";
		return 1;
	}
	// step = std::Euler<std::Equation<std::State<float, 2>>>::make_step(init, 0.01);
    // std::cout << step[0] << step[1] << std::endl;
	
	fout.close();
	
    return 0;
}