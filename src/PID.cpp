#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double kp, double ki, double kd) {
	Kd = kd;
	Ki = ki;
	Kp = kp;

	p_error = 0.;
	i_error = 0.;
	d_error = 0.;

	cte_prev = 0.;
}

void PID::UpdateError(double cte) {

	d_error = cte - cte_prev;
	//d_error = cte - cte_prev;

	// Anti-saturateion of integral channel 
	if (i_error > 100)
	{
		if (cte < 0) i_error += cte;
	}else if(i_error < -100){
		if(cte > 0) i_error += cte;
	}else{
		i_error += cte;
	}


	p_error = cte;

	cte_prev = cte;
}

double PID::Control(double err_p, double err_i, double err_d){

	double output;

	output = Kp * err_p + Ki * err_i + Kd * err_d;
}



