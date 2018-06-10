#include "CBranch.h"

#include <QTextStream>


CBranch::CBranch()
{
}


CBranch::~CBranch()
{
}


void CBranch::setNodes(int n1, int n2)
{
	m_n1 = n1;
	m_n2 = n2;
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


void CBranch::setR(double R)
{
    double r = R / m_L;

    prepare(r);
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
        //double L = DBL_MAX;
        //m_Pbeg = 0;

        //for (int i = 0; i < m_inBeg.size(); ++i)
        //	if (m_inBeg[i]->m_L < L){
        //		m_Pbeg = m_inBeg[i]->getP();
        //		L = m_inBeg[i]->m_L;
        //	}

        double S = 0;
        m_Pbeg = 0;

        for (int i = 0; i < m_inBeg.size(); ++i)
            if (m_inBeg[i]->m_S > S) {
                m_Pbeg = m_inBeg[i]->getP();
                S = m_inBeg[i]->m_S;
            }
    }


    if (m_outEnd.isEmpty())
    {
    }
    else
    {
        if (m_inEnd.isEmpty())
        {
            m_Pend = getP();
        }
        else
        {
            // one with min L?
            //double L = m_L;
            //m_Pend = getP();

            //for (int i = 0; i < m_inEnd.size(); ++i)
            //	if (m_inEnd[i]->m_L < L)
            //	{
            //		m_Pend = m_inEnd[i]->getP();
            //		L = m_inEnd[i]->m_L;
            //	}

            double S = m_S;
            m_Pend = getP();

            for (int i = 0; i < m_inEnd.size(); ++i)
                if (m_inEnd[i]->m_S > S)
                {
                    m_Pend = m_inEnd[i]->getP();
                    S = m_inEnd[i]->m_S;
                }
        }
    }
}


void CBranch::stepRK4(double m_h)
{
	std::vector<double> k1(m_M), k2(m_M), k3(m_M), k4(m_M);
	std::vector<double> m1(m_M), m2(m_M), m3(m_M), m4(m_M);

	// step 1
	fQ(k1, m_P, m_Q);
	fP(m1, m_P, m_Q);

	// step 2
	std::vector<double> P(m_M), Q(m_M);
	for (int i = 0; i < m_M; i++)
	{
		Q[i] = m_Q[i] + k1[i] * m_h / 2.0;
		P[i] = m_P[i] + m1[i] * m_h / 2.0;
	}

	fQ(k2, P, Q);
	fP(m2, P, Q);

	// step 3
	for (int i = 0; i < m_M; i++)
	{
		Q[i] = m_Q[i] + k2[i] * m_h / 2.0;
		P[i] = m_P[i] + m2[i] * m_h / 2.0;
	}

	fQ(k3, P, Q);
	fP(m3, P, Q);

	// step 4
	for (int i = 0; i < m_M; i++)
	{
		Q[i] = m_Q[i] + k3[i] * m_h;
		P[i] = m_P[i] + m3[i] * m_h;
	}

	fQ(k4, P, Q);
	fP(m4, P, Q);

	// final step
	for (int i = 0; i < m_M; i++)
	{
		m_Q[i] += (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) * m_h / 6.0;
		m_P[i] += (m1[i] + 2.0 * m2[i] + 2.0 * m3[i] + m4[i]) * m_h / 6.0;
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
		double q = getQ();

		for (int i = 0; i < m_inEnd.size(); ++i)
			q += m_inEnd[i]->getQ();

		for (int i = 0; i < m_outEnd.size(); ++i)
			q -= m_outEnd[i]->getQbeg();

		// max.gamma = gamma of branch with max.S
		double max_gamma = m_gamma;
		double S = m_S;

		for (int i = 0; i < m_inEnd.size(); ++i)
			if (m_inEnd[i]->m_S > S) {
				max_gamma = m_inEnd[i]->m_gamma;
				S = m_inEnd[i]->m_S;
			}

		dPdt[m_M - 1] = max_gamma * q;
	}
}


double CBranch::getQ() const { return m_Q[m_M - 1]; }
double CBranch::getQbeg() const { return m_Q[0]; }

double CBranch::getP() const {
	return m_P[m_M - 1]; 
}

double CBranch::getPbeg() const {
	return m_Pbeg;
}


// IO

void CBranch::createOutputs(const QString& path, int id)
{
	QString nameQ = path + "\\Q" + QString::number(id) + ".txt";
	QString nameP = path + "\\P" + QString::number(id) + ".txt";

	m_fileQ.setFileName(nameQ);
	m_fileQ.open(QFile::WriteOnly | QFile::Truncate | QFile::Append);

	m_fileP.setFileName(nameP);
	m_fileP.open(QFile::WriteOnly | QFile::Truncate | QFile::Append);

	//QTextStream tsQ(&m_fileQ);
	//tsQ << "Step\t\t";
}


void CBranch::dump(int step)
{
	QTextStream tsQ(&m_fileQ);

	tsQ << "Step: " << step << "\t";

	for (int i = 0; i < m_M; ++i)
		tsQ << QString("Q[%1]=%2  ").arg(i).arg(m_Q[i], 14);

	tsQ << "\n";



	QTextStream tsP(&m_fileP);

	tsP << "Step: " << step << "\t";

	tsP << QString("Pbeg=%1  ").arg(m_Pbeg, 14);

	for (int i = 0; i < m_M; ++i)
		tsP << QString("P[%1]=%2  ").arg(i).arg(m_P[i], 14);

	tsP << QString("Pend=%1  ").arg(m_Pend, 14);

	tsP << "\n";
}


