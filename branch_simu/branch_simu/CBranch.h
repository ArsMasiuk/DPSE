#pragma once

#include <vector>

#include <QList>
#include <QFile>


class CBranch
{
public:
	CBranch();
	~CBranch();

	void init(double L, double S, int M);
	void setP(double beg, double end);
	void setQt(double Qtgt);

	void prepare();
	void prepare(double r);
	void exchange();
	void stepQ(double m_h);
	void stepP(double m_h);
	void stepRK4(double m_h);
	void fQ(std::vector<double>& dQdt, const std::vector<double>& P, const std::vector<double>& Q);
	void fP(std::vector<double>& dPdt, const std::vector<double>& P, const std::vector<double>& Q);

	void createOutputs(const QString& path, int id);
	void dump(int step);

	double getQ() const;
	double getQbeg() const;

	double getP() const;
	double getPl() const;
	double getPbeg() const;

	QList<CBranch*> m_inBeg, m_outBeg, m_inEnd, m_outEnd;

protected:
	double m_L, m_S;
	int m_M;
	double m_Pbeg, m_Pend, m_Qtgt;
	double m_dX;
	double m_alpha, m_beta, m_gamma;

	std::vector<double> m_P, m_Q;

	QFile m_fileP, m_fileQ;
};

