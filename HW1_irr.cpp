#include <iostream>
#include <vector>
#include <cmath>

double irr_bisection(std::vector<double> cashflow, double guess_a, double guess_b) {
    const int MAX_ITERATIONS = 100; // �̤j���զ���
    const double PRECISION = 1E-4; // ����~�t
    double xa = guess_a; // �G���k�U��
    double xb = guess_b; // �G���k�W��

    auto calc = [&](std::vector<double> cashflow, double r) { // �p��{���y��{�� 
        double result = 0.0;
        for (size_t t = 0; t < cashflow.size(); ++t) {
            result += cashflow[t] / pow((r + 1), t);
        }
        return result;
    };

    double rate = 0;
    int i = 0;
    while (i < MAX_ITERATIONS) { // ���٥���F�̤j���զ��ơA���S�vc(rate)=a+b/2
        rate = (xa + xb) / 2;
        double npv = calc(cashflow, rate);

        if (npv * calc(cashflow, xa) > 0) { // �Yf(c)f(a)>0, a=c
            xa = rate;
        } else { // f(c)f(b)>0, b=c
            xb = rate;
        }

        if (npv == 0 || std::abs(xa - xb) <= PRECISION) { // ��npv=0�Ϊ̤W�U�ɤp�����~�t�A��^���G
            return rate;
        }

        i++;
    }

    return NAN;
}

double irr_newton_raphson(std::vector<double> cashflow, double guess) {
    const int MAX_ITERATIONS = 100; // �̤j���զ���
    const double PRECISION = 1E-4; // ����~�t
    double x0 = guess; // �q��rate
    int i = 0;
    while (i < MAX_ITERATIONS) { // ���٥���F�̤j���զ��ơAXk+1 = Xk - f(Xk)/f'(Xk)
        double npv = 0;
        double ddx = 0;
        for (size_t t = 0; t < cashflow.size(); ++t) {
            npv += cashflow[t] / pow((x0 + 1), t); // f(Xk)
            ddx += -abs(t) * cashflow[t] / pow((x0 + 1), (t + 1)); // f'(Xk)
            // std::cout<<-abs(t) * cashflow[t]<<"/"<<pow((x0 + 1), (t + 1))<<std::endl;
            // std::cout<<"ddx = "<<ddx<<std::endl;
        }
        // std::cout<<double(npv / ddx)<<std::endl;
		double x1 = x0 - double(npv / ddx); // Xk+1 = Xk - f(Xk)/f'(Xk)
    
        if (std::abs(x1 - x0) <= PRECISION) { // ��W�U�ɤp�����~�t�A��^���G
        	//std::cout<<"x1= "<<x1<<"error= "<<std::abs(x1 - x0)<<std::endl;
            return x1;
        }
        x0 = x1;
        ++i;
    }
    return NAN;
}

int main() { // ��X���G

    // Test1 (Bisection) 
    std::vector<double> cashflow1 = {-1000, 350, 350, 350};
    double guess_a1 = 0.02;
    double guess_b1 = 0.03;
    std::cout << "Ans (Bisection): " << irr_bisection(cashflow1, guess_a1, guess_b1) << std::endl;

    // Test2 (Bisection) 
    /*std::vector<double> cashflow2 = {-1000, 350, 350, 350};
    double guess_a2 = 0.02;
    double guess_b2 = 0.03;
    std::cout << "2nd Ans (Bisection): " << irr_bisection(cashflow2, guess_a2, guess_b2) << std::endl;

    // Test1 (Newton-Raphson)
    double guess1 = 0;
    std::cout << "1st Ans (Newton-Raphson): " << irr_newton_raphson(cashflow1, guess1) << std::endl;

    // Test2 (Newton-Raphson)
    double guess2 = 1;
    std::cout << "2nd Ans (Newton-Raphson): " << irr_newton_raphson(cashflow2, guess2) << std::endl;*/

    return 0;
}

