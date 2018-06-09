#pragma once

#include <vector>

#include <QList>
#include <QFile>


class CBranch
{
public:
	CBranch();
    CBranch(const CBranch&)=default;
    ~CBranch();

	void setNodes(int n1, int n2);

	void init(double L, double S, int M);
	void setP(double beg, double end);
	void setQt(double Qtgt);

	void prepare();
	void prepare(double r);
	void exchange();
	void stepRK4(double m_h);
	void fQ(std::vector<double>& dQdt, const std::vector<double>& P, const std::vector<double>& Q);
	void fP(std::vector<double>& dPdt, const std::vector<double>& P, const std::vector<double>& Q);

	void createOutputs(const QString& path, int id);
	void dump(int step);

	double getQ() const;
	double getQbeg() const;

	double getP() const;
	double getPbeg() const;

	// will be deprecated
	QList<CBranch*> m_inBeg, m_outBeg, m_inEnd, m_outEnd;

protected:
	// topology
	int m_n1 = 0, m_n2 = 0;

	// params
	double m_L = 0.0, m_S = 0.0;
	int m_M = 0;
	double m_dX = 0.0;

	// temp
	double m_Pbeg = 0.0, m_Pend = 0.0, m_Qtgt = 0.0;
	double m_alpha = 0.0, m_beta = 0.0, m_gamma = 0.0;

	// results
	std::vector<double> m_P, m_Q;

	// io
	QFile m_fileP, m_fileQ;
};

