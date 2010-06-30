#include "integrator.h"

Integrator::Integrator()
    : t(0)
{
}

void Integrator::set_h(double dt)
{
    h = dt;
}

void Integrator::set_y0(const dvector& y0)
{
    N = y0.size();
    y.assign(y0.begin(), y0.end());
}

void Integrator::set_t0(const double t0)
{
    t = t0;
}

double Integrator::get_h() const
{
    return h;
}

const dvector& Integrator::get_y() const
{
    return y;
}

double Integrator::get_t() const
{
    return t;
}

ExplicitIntegrator::ExplicitIntegrator(ODE& ode)
    : myODE(ode)
{
}

void ExplicitIntegrator::set_y0(const dvector& y0)
{
    Integrator::set_y0(y0);
    ydot.resize(N,0);
}

const dvector& ExplicitIntegrator::get_ydot() const
{
    return ydot;
}

void ExplicitIntegrator::step()
{
    // Integrate one time step using the explicit Euler's method
    myODE.f(t, y, ydot);
    y += h * ydot;
    t += h;
}

void ExplicitIntegrator::step_to_time(double tEnd)
{
    while (t < tEnd) {
        step();
    }
}

// ***********************
// * class BDFIntegrator *
// ***********************

BDFIntegrator::BDFIntegrator(LinearODE& ode)
    : myODE(ode)
    , A(NULL)
    , LU(NULL)
    , stepCount(0)
{
}

BDFIntegrator::~BDFIntegrator()
{
    delete LU;
    delete A;
}

void BDFIntegrator::set_size(int N_in, int upper_bw_in, int lower_bw_in)
{
    N = N_in;
    upper_bw = upper_bw_in;
    lower_bw = lower_bw_in;

    delete LU;
    delete A;
    LU = new sdBandMatrix(N, upper_bw, lower_bw);
    A = new sdBandMatrix(N, upper_bw, lower_bw);
    p.resize(N);
}

void BDFIntegrator::set_y0(const dvector& y0)
{
    Integrator::set_y0(y0);
    stepCount = 0;
}

void BDFIntegrator::set_t0(const double t0)
{
    Integrator::set_t0(t0);
    stepCount = 0;
}

void BDFIntegrator::set_dt(const double h_in)
{
    Integrator::set_h(h_in);
    stepCount = 0;
}

void BDFIntegrator::step()
{
    if (stepCount == 0) {
        yprev.assign(y.begin(), y.end());

        myODE.get_A(*A);
        myODE.get_C(c);
        BandCopy(A->forSundials(), LU->forSundials(), upper_bw, lower_bw);
        sdBandMatrix& M = *LU;
        for (int i=0; i<N; i++) {
            M(i,i) -= 1/h;
        }
        BandGBTRF(LU->forSundials(), &p[0]);

        // y_n -> y_n+1
        for (int i=0; i<N; i++) {
            y[i] = -y[i]/h - c[i];
        }
        BandGBTRS(LU->forSundials(), &p[0], &y[0]);
    } else {
        if (stepCount == 1) {
            BandCopy(A->forSundials(), LU->forSundials(), upper_bw, lower_bw);
            sdBandMatrix& M = *LU;
            for (int i=0; i<N; i++) {
                M(i,i) -= 3.0/(2.0*h);
            }
            BandGBTRF(LU->forSundials(), &p[0]);
        }
        dvector tmp(y);

        for (int i=0; i<N; i++) {
            y[i] = -2*y[i]/h + yprev[i]/(2*h) - c[i];
        }
        yprev.assign(tmp.begin(), tmp.end());
        BandGBTRS(LU->forSundials(), &p[0], &y[0]);
    }

    stepCount++;
    t += h;
}
