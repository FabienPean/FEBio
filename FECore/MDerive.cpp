#include "MMath.h"
#include "MEvaluate.h"

#ifndef PI
#define PI	3.1415926535897932385
#endif

//-----------------------------------------------------------------------------
MITEM MDerive(MITEM& a, MVariable& x)
{
	MITEM e = MEvaluate(a);
	switch (e.Type())
	{
	case MCONST:
	case MFRAC:
	case MNAMED: return 0.0; 
	case MVAR: if (e == x) return 1.0; else return 0.0;
	case MNEG: return -MDerive(-e, x); 
	case MADD: return (MDerive(e.Left(), x) + MDerive(e.Right(), x));
	case MSUB: 
		{
			MITEM l = MDerive(e.Left(), x);
			MITEM r = MDerive(e.Right(), x);
			return l - r;
		}
	case MMUL:
		{
			MITEM l = e.Left();
			MITEM dl = MDerive(l, x);
			MITEM r = e.Right();
			MITEM dr = MDerive(r, x);

			return (dl*r + l*dr);
		}
	case MDIV:
		{
			MITEM l = e.Left();
			MITEM r = e.Right();
			MITEM dl = MDerive(l, x)*r;
			MITEM dr = l*MDerive(r, x);
			// be careful here that we are not subtracting two pointers
			MITEM a = dl/(r^2.0);
			MITEM b = dr/(r^2.0);
			return (a-b);
		}
	case MPOW:
		{
			MITEM l = e.Left();
			MITEM r = e.Right();
			if (isConst(r) || is_named(r) || is_frac(r)) return (r*(l^(r-1.0)))*MDerive(l, x);
			else if (isConst(l) || is_named(l) || is_frac(l)) return (Log(l)*e)*MDerive(r, x);
			else
			{
				MITEM dl = MDerive(l, x);
				MITEM dr = MDerive(r, x);
				return (e*(dr*Log(l) + (r*dl)/l));
			}
		}
	case MF1D:
		{
			const string& s = mfnc1d(e)->Name();
			MITEM p = mfnc1d(e)->Item()->copy();
			MITEM dp = MDerive(p,x);
			if (s.compare("cos") == 0) return -Sin(p)*dp;
			if (s.compare("sin") == 0) return  Cos(p)*dp;
			if (s.compare("tan") == 0) return (Sec(p)^2.0)*dp;
			if (s.compare("sec") == 0) return (Sec(p)*Tan(p))*dp;
			if (s.compare("csc") == 0) return (-Csc(p)*Cot(p))*dp;
			if (s.compare("cot") == 0) return -((Csc(p)^2.0)*dp);
			if (s.compare("abs") == 0) return Sgn(p)*dp;
			if (s.compare("ln" ) == 0) return (dp/p);
			if (s.compare("log") == 0) return (dp/p)/Log(MITEM(10.0));
			if (s.compare("asin") == 0) return (dp/Sqrt(1.0 - (p^2)));
			if (s.compare("acos") == 0) return (-dp/Sqrt(1.0 - (p^2)));
			if (s.compare("atan") == 0) return (dp/((p^2) + 1.0));
			if (s.compare("cosh") == 0) return (dp*Sinh(p));
			if (s.compare("sinh") == 0) return (dp*Cosh(p));
			if (s.compare("sqrt") == 0) return (dp/(2.0*Sqrt(p)));
			if (s.compare("J0"  ) == 0) return (-J1(p))*dp;
			if (s.compare("J1"  ) == 0) return dp*(J0(p) - Jn(2, p))/2.0;
			if (s.compare("Y0"  ) == 0) return (-Y1(p))*dp;
			if (s.compare("Y1"  ) == 0) return dp*(Y0(p) - Yn(2, p))/2.0;
			if (s.compare("erf" ) == 0)
			{
				MITEM Pi = new MNamedCt(PI, "pi");
				return 2/Sqrt(Pi)*Exp(-(p^2))*dp;
			}
			if (s.compare("erfc") == 0)
			{
				MITEM Pi = new MNamedCt(PI, "pi");
				return -(2/Sqrt(Pi))*Exp(-(p^2))*dp;
			}
			assert(false);
		}
		break;
	case MF2D:
		{
			const string& s = mfnc2d(e)->Name();
			MITEM a = e.Left();
			MITEM b = e.Right();
			MITEM db = MDerive(b,x);
			if (s.compare("Jn") == 0)
			{
				if (is_int(a))
				{
					int n = (int) a.value();
					if (n==0) return (-J1(b))*db;
					else return ((Jn(n-1, b) - Jn(n+1,b))/2.0)*db;
				}
			}
			else if (s.compare("Yn") == 0)
			{
				if (is_int(a))
				{
					int n = (int) a.value();
					if (n==0) return (-Y1(b))*db;
					else return ((Yn(n-1, b) - Yn(n+1,b))/2.0)*db;
				}
			}
		}
		break;
	case MMATRIX:
		{
			MMatrix& m = *mmatrix(e);
			int ncol = m.columns();
			int nrow = m.rows();

			MMatrix* pdm = new MMatrix;
			pdm->Create(nrow, ncol);
			for (int i=0; i<nrow; ++i)
				for (int j=0; j<ncol; ++j)
				{
					MITEM mij(m.Item(i,j)->copy());
					MITEM dmij = MDerive(mij, x);
					(*pdm)[i][j] = dmij.copy();
				}
			return MITEM(pdm);
		}
		break;
	case MSFNC:
		{
			MSFuncND& f = *msfncnd(e);
			MITEM v(f.Value()->copy());
			return MDerive(v, x);
		}
		break;
	}
	assert(false);
	return e;
}

//-----------------------------------------------------------------------------
MITEM MDerive(MITEM& e, MVariable& x, int n)
{
	MITEM d = e;
	for (int i=0; i<n; ++i) d = MDerive(d, x);
	return d;
}

//-----------------------------------------------------------------------------
MITEM MDerive(MITEM& e, MSequence& x)
{
	MITEM d = e;
	for (int i=0; i<(int) x.size(); ++i)
	{
		MVariable& xi = *(mvar(x[i])->GetVariable());
		d = MDerive(d, xi);
	}
	return d;
}

//-----------------------------------------------------------------------------
MITEM MTaylor(MITEM& e, MVariable& v, double z, int n)
{
	MITEM a(z);
	MITEM x(v);

	MITEM dx = x - a;
	MITEM s = MReplace(e, v, a);
	MITEM t(e);
	double d = 1;

	for (int i=1; i<=n; ++i)
	{
		t = MDerive(t /d, v);
		d = (double) i;

		MITEM f = MReplace(t, v, a);
		MITEM Di = dx^d;
		MITEM ds = ((f/d)*Di);
		s = s + ds;
	}

	return s;
}
