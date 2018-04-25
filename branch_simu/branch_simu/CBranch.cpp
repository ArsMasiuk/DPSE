#include "CBranch.h"


CBranch::CBranch()
{
}


CBranch::~CBranch()
{
}


void CBranch::init(double L, double S, int M)
{
	m_L = L;
	m_S = S;
	m_M = M;
	m_dX = (m_L / m_M);

	m_Pbeg = m_Pend = m_Qtgt = 0;

	m_P.resize(m_M, 0);
	m_Q.resize(m_M, 0); 
}


void CBranch::setP(double beg, double end)
{
	m_Pbeg = beg;
	m_Pend = end;
}


void CBranch::setQt(double Qtgt)
{
	m_Qtgt = Qtgt;
}


void CBranch::prepare()
{
	double R = qAbs(m_Pend) / (m_Qtgt * m_Qtgt);
	double r = R / m_L;

	prepare(r);
}


void CBranch::prepare(double r)
{
	const double ro = 1.293;
	const double a = 331.5;

	m_alpha = m_S / (ro * m_dX);
	m_beta = (r * m_S) / ro;
	m_gamma = (ro * a * a) / (m_S * m_dX);
}


void CBranch::exchange()
{
	if (m_inBeg.isEmpty())
		m_Pbeg = 0;
	else 
	{
		// one with min L?
		double L = m_L;
		m_Pbeg = getP();
		for (int i = 0; i < m_inBeg.size(); ++i)
			if (m_inBeg[i]->m_L < L){
				m_Pbeg = m_inBeg[i]->getP();
				L = m_inBeg[i]->m_L;
			}
	}


	if (m_outEnd.isEmpty())
	{
	}
	else
	{
		// one with min L?
		double L = m_L;
		m_Pend = getP();
		for (int i = 0; i < m_inEnd.size(); ++i)
			if (m_inEnd[i]->m_L < L) {
				m_Pend = m_inEnd[i]->getP();
				L = m_inEnd[i]->m_L;
			}
	}
}


void CBranch::stepQ(double m_h)
{
	std::vector<double> k1(m_M), k2(m_M), k3(m_M), k4(m_M);

	// write last P
	//m_P[m_M - 1] = m_Pend;

	fQ(k1, m_P, m_Q);

	std::vector<double> P(m_M), Q(m_M);
	for (int i = 0; i < m_M; i++)
	{
		P[i] = m_P[i] + m_h / 2.0;
		Q[i] = m_Q[i] + m_h / 2.0 * k1[i];
	}

	fQ(k2, P, Q);

	for (int i = 0; i < m_M; i++)
	{
		P[i] = m_P[i] + m_h / 2.0;
		Q[i] = m_Q[i] + m_h / 2.0 * k2[i];
	}

	fQ(k3, P, Q);

	for (int i = 0; i < m_M; i++)
	{
		P[i] = m_P[i] + m_h;
		Q[i] = m_Q[i] + m_h * k3[i];
	}

	fQ(k4, P, Q);

	for (int i = 0; i < m_M; i++)
	{
		m_Q[i] += (m_h / 6.0) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
}


void CBranch::stepP(double m_h)
{
	std::vector<double> k1(m_M), k2(m_M), k3(m_M), k4(m_M);

	// write last P
	//m_P[m_M - 1] = m_Pend;

	fP(k1, m_P, m_Q);

	std::vector<double> P(m_M), Q(m_M);
	for (int i = 0; i < m_M; i++)
	{
		Q[i] = m_Q[i] + m_h / 2.0;
		P[i] = m_P[i] + m_h / 2.0 * k1[i];
	}

	fP(k2, P, Q);

	for (int i = 0; i < m_M; i++)
	{
		Q[i] = m_Q[i] + m_h / 2.0;
		P[i] = m_P[i] + m_h / 2.0 * k2[i];
	}

	fP(k3, P, Q);

	for (int i = 0; i < m_M; i++)
	{
		Q[i] = m_Q[i] + m_h;
		P[i] = m_P[i] + m_h * k3[i];
	}

	fP(k4, P, Q);

	for (int i = 0; i < m_M; i++)
	{
		m_P[i] += (m_h / 6.0) * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
}


void CBranch::fQ(std::vector<double>& dQdt, const std::vector<double>& P, const std::vector<double>& Q)
{
	// 1st
	dQdt[0] = m_alpha * (m_Pbeg - P[0]) - m_beta * (Q[0] * fabs(Q[0]));

	// j
	for (int j = 1; j < m_M - 1; j++)
	{
		dQdt[j] = m_alpha * (P[j - 1] - P[j]) - m_beta * (Q[j] * fabs(Q[j]));
	}

	// last 
	dQdt[m_M - 1] = m_alpha * (P[m_M - 2] - m_Pend) - m_beta * (Q[m_M - 1] * fabs(Q[m_M - 1]));
}


void CBranch::fP(std::vector<double>& dPdt, const std::vector<double>& P, const std::vector<double>& Q)
{
	// 1st
	dPdt[0] = m_gamma * (Q[0] - Q[1]);

	// j
	for (int j = 1; j < m_M - 1; j++)
	{
		dPdt[j] = m_gamma * (Q[j] - Q[j + 1]);
	}

	// last
	if (m_outEnd.isEmpty())
	{
		dPdt[m_M - 1] = 0;
	}
	else
	{
		dPdt[m_M - 1] = getQ();

		for (int i = 0; i < m_inEnd.size(); ++i)
			dPdt[m_M - 1] += m_inEnd[i]->getQ();

		for (int i = 0; i < m_outEnd.size(); ++i)
			dPdt[m_M - 1] -= m_outEnd[i]->getQbeg();

		// max.gamma = gamma of branch with max.S
		double max_gamma = m_gamma;
		double S = m_S;
		for (int i = 0; i < m_inEnd.size(); ++i)
			if (m_inEnd[i]->m_S > S) {
				max_gamma = m_inEnd[i]->m_gamma;
				S = m_inEnd[i]->m_S;
			}

		dPdt[m_M - 1] *= max_gamma;
	}
}


double CBranch::getQ() const { return m_Q[m_M - 1]; }
double CBranch::getQbeg() const { return m_Q[0]; }

double CBranch::getP() const { 
	return m_P[m_M - 1]; 
}

double CBranch::getPl() const {
	return m_P[m_M - 2];
}

double CBranch::getPbeg() const {
	return m_Pbeg;
}

