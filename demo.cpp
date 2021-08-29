#include "cantera/thermo.h"
#include <iostream>
#include<string.h>

using namespace Cantera;
using namespace std;

std::map<std::string, double> gastherm(double T, double P, double R_fluid, std::string model_eos)
{
	// Thermally and calorically perfect
	double C_sound, zcf, gamma;
	double Cp, Cv, H, S;

	// Create a new phase
	auto sol = newSolution("air.yaml");
	auto gas = sol->thermo();
	//gas->setState_TP(T, P);

	cout << gas->report() << endl;
	
	if (model_eos == "tc_ideal")
	{
		Cp = 1005; // Remove and take from input
		//h0 = h_ref + (Cp*(T0-T_ref));  Add these also if possiblr
		//s0 = s_ref + (Cp*log(T0/ T_ref)) - (R_fluid * log(P0 / P_ref)); Add these also if possible
		gamma = Cp / (Cp - R_fluid);    // Mayer's Relation only applicable in this case
		C_sound = sqrt(gamma * R_fluid * T);
		zcf = 1;   // Compressibility factor (unity for thermally ideal gas)
	}

	else if (model_eos == "cantera") {
		Cp = gas->cp_mass();
		Cv = gas->cv_mass();
		H = gas->enthalpy_mass();
		S = gas->entropy_mass();
		gamma = Cp / (Cp - R_fluid);
		C_sound = sqrt(gamma * R_fluid * T);
		zcf = 1;   // Compressibility factor (unity for thermally ideal gas)
	}
	cout << "cp = " << Cp << "  cv = " << Cv << "  gamma = " << gamma << " C_sound  = " << C_sound << endl;
	cout << "Gamma = " << (Cp / Cv) << endl;
	return { {"Cp",Cp},{"gamma",gamma},{"C_sound",C_sound},{"zcf",zcf} };
}

// the main program just calls function simple_demo within
// a 'try' block, and catches CanteraError exceptions that
// might be thrown
int main()
{
    try {
		gastherm(527.69, 538657.7,287,"cantera");
	}
    catch (CanteraError& err) {
        std::cout << err.what() << std::endl;
    }
}

