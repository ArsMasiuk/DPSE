
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <CBranch.h>


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	// multibranch
	const int m = 8;

	CBranch b[m];
	b[2].init(1925.0, 4.67, 65);
	b[4].init(1720.0, 4.5, 58);
	b[3].init(1410.0, 5.98, 47);
	b[5].init(934.0, 4.97, 32);
	b[0].init(800.0, 6.5, 27);
	b[7].init(600.0, 6.5, 20);
	b[6].init(150.0, 6.5, 5);
	b[1].init(150.0, 6.5, 5);



	//b[0].m_outEnd << &b[4] << &b[5];

	//b[7].m_inBeg << &b[4] << &b[5];

	//b[4].m_inBeg << &b[0];
	//b[4].m_outEnd << &b[7];
	//b[4].m_outBeg << &b[5];
	//b[4].m_inEnd << &b[5];

	//b[5].m_inBeg << &b[0];
	//b[5].m_outEnd << &b[7];
	//b[5].m_outBeg << &b[4];
	//b[5].m_inEnd << &b[4];


	b[0].m_outEnd << &b[1] << &b[4] << &b[5];
	b[7].m_inBeg << &b[4] << &b[5] << &b[6];

	b[1].m_inBeg << &b[0];
	b[1].m_outBeg << &b[4] << &b[5];
	b[1].m_outEnd << &b[2] << &b[3];

	b[2].m_inBeg << &b[1];
	b[2].m_outBeg << &b[3];
	b[2].m_outEnd << &b[6];
	b[2].m_inEnd << &b[3];

	b[3].m_inBeg << &b[1];
	b[3].m_outBeg << &b[2];
	b[3].m_outEnd << &b[6];
	b[3].m_inEnd << &b[2];

	b[4].m_inBeg << &b[0];
	b[4].m_outEnd << &b[7];
	b[4].m_outBeg << &b[5] << &b[1];
	b[4].m_inEnd << &b[5] << &b[6];

	b[5].m_inBeg << &b[0];
	b[5].m_outEnd << &b[7];
	b[5].m_outBeg << &b[4] << &b[1];
	b[5].m_inEnd << &b[4] << &b[6];

	b[6].m_inBeg << &b[2] << &b[3];
	b[6].m_inEnd << &b[4] << &b[5];
	b[6].m_outEnd << &b[7];



	b[0].setP(0, 0);
	b[7].setP(0, -4100);

	b[2].prepare(0.001056);
	b[4].prepare(0.00112);
	b[3].prepare(0.0019);
	b[5].prepare(0.00137);
	b[0].prepare(0.000484);
	b[7].prepare(0.000484);
	b[6].prepare(0.000484);
	b[1].prepare(0.000484);

	//const int m = 1;
	//CBranch b[m];

	//b[0].init(800, 6.72, 20);
	//b[0].setP(0, -38.72);
	//b[0].setQt(10);

	//b[0].prepare();

	for (int i = 0; i < m; ++i)
		b[i].createOutputs(QString("c:\\temp"), i);


	int steps = 300000;	// 300s
	for (int s = 0; s < steps; ++s)
	{
		QString res; res = QString("step: %1").arg(s);

		for (int i = 0; i < m; ++i)
		{
			b[i].exchange();
			b[i].stepRK4(0.001);
			b[i].exchange();
		}


		if (s % 1000 == 0)
		{
			for (int i = 0; i < m; ++i)
			{
				b[i].dump(s);
			}

			qDebug() << res;
		}
	}

	return 0;
	//return a.exec();
}
