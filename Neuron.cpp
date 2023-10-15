#include"pch.h"
#include"Neuron.h"


double rand(double left, double right)
{
	return left + (right - left) * double(rand()) / double(RAND_MAX);
}


neuron::neuron(int size)
{
	in = vector<double>(size, 0);
	w = vector<double>(size, 0);
	for (int i = 0; i < size; i++)
	{
		w[i] = rand(-1, 1);
	}
	fi = rand(-1, 1);
}


NW::NW(vector<int>& LS, int Enters)
{
	ls = LS;

	Layer.push_back(vector<neuron>());
	for (int i = 0; i < LS[0]; i++)
	{
		Layer[0].push_back(neuron(Enters));
	}
	for (int i = 1; i < LS.size(); i++)
	{
		Layer.push_back(vector<neuron>());
		for (int j = 0; j < LS[i]; j++)
		{
			Layer[i].push_back(neuron(LS[i - 1]));
		}
	}
}


void NW::ProcessNeuron(neuron& n, vector<double>& in, int len)
{
	n.in = in;
	double res = 0;
	for (int i = 0; i < len; i++)
	{
		res += n.w[i] * in[i];
	}
	res -= n.fi;
	n.out = f(res);
}

vector<double>Tin(100, 0);
int g_len = 0;
template<typename T>
double NW::Calc(vector<T>& in)
{
	for (int i = 0; i < in.size(); i++)Tin[i] = in[i];
	g_len = in.size();
	for (int i = 0; i < Layer.size(); i++)
	{
		for (int j = 0; j < Layer[i].size(); j++)
		{
			ProcessNeuron(Layer[i][j], Tin, g_len);
		}
		for (int j = 0; j < Layer[i].size(); j++)
		{
			Tin[j] = Layer[i][j].out;
		}
		g_len = Layer[i].size();
	}
	return Tin[0];
}

double NW::f(double x)
{
	return 1. / (1. + exp(-2 * x));
}


void NW::MakeChild(NW& left, NW& right)
{
	double chance = 0;
	for (int i = 0; i < Layer.size(); i++)
	{
		for (int j = 0; j < Layer[i].size(); j++)
		{
			for (int k = 0; k < Layer[i][j].w.size(); k++)
			{
				chance = rand(0, 1);
				Layer[i][j].w[k] = chance * left.Layer[i][j].w[k] + (1 - chance) * right.Layer[i][j].w[k];
			}
			chance = rand(0, 1);
			Layer[i][j].fi = chance * left.Layer[i][j].fi + (1 - chance) * right.Layer[i][j].fi;
		}
	}
}

void NW::Mutate()
{
	double M = 0.075 * (PERFECT_SCORE - avscore) / PERFECT_SCORE;
	for (int i = 0; i < Layer.size(); i++)
	{
		for (int j = 0; j < Layer[i].size(); j++)
		{
			for (int k = 0; k < Layer[i][j].w.size(); k++)
			{
				Layer[i][j].w[k] *= rand(1 - M, 1 + M);
			}
			Layer[i][j].fi *= rand(1 - M, 1 + M);
		}
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


Trainer::Trainer()
{
	srand(time(NULL));


	for (int i = 0; i < _size; i++)
	{
		NW nw1(LS, 9);
		NW nw2(LS, 9);
		P1.push_back(nw1);
		P2.push_back(nw2);
	}
}


inline int FindWinner(vector<int>& data, int zeros)
{
	if ((data[0] == data[1]) && (data[0] == data[2]) && (data[0] != 0))return data[0];
	if ((data[3] == data[4]) && (data[3] == data[5]) && (data[3] != 0))return data[3];
	if ((data[6] == data[7]) && (data[6] == data[8]) && (data[6] != 0))return data[6];

	if ((data[0] == data[3]) && (data[0] == data[6]) && (data[0] != 0))return data[0];
	if ((data[1] == data[4]) && (data[1] == data[7]) && (data[1] != 0))return data[1];
	if ((data[2] == data[5]) && (data[2] == data[8]) && (data[2] != 0))return data[2];

	if ((data[0] == data[4]) && (data[0] == data[8]) && (data[0] != 0))return data[0];
	if ((data[2] == data[4]) && (data[2] == data[6]) && (data[2] != 0))return data[2];

	if (zeros == 0)return DRAW;
	return 0;
}

int NW::MakePredictions(vector<int>& in, int turn)
{
	vector<double> res(9, INT_MAX);

	for (int i = 0; i < 9; i++)
	{
		if (in[i] == 0)
		{
			
			in[i] = turn;
			res[i] = Calc(in);
			in[i] = 0;
		}
	}
	return Max(res);
}

void Trainer::score(NW& p1, NW& p2)
{
	vector<int> data(9, 0);
	int deep = 0;
	int zeroes = 9;
	int winner = 0;
	while (zeroes > 0)
	{
		static int pos = p1.MakePredictions(data, 1);
		deep++;
		zeroes--;
		data[pos] = 1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p1.score = deep;
			p2.score = deep;
			break;
		}
		if (winner == 1)
		{
			p1.score = 10 - deep;
			p2.score = deep - 10;
			break;
		}
		if (winner == -1)
		{
			p1.score = deep - 10;
			p2.score = 10 - deep;
			break;
		}


		pos = p2.MakePredictions(data, -1);
		deep++;
		zeroes--;
		data[pos] = -1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p1.score = deep;
			p2.score = deep;
			break;
		}
		if (winner == 1)
		{
			p1.score = 10 - deep;
			p2.score = deep - 10;
			break;
		}
		if (winner == -1)
		{
			p1.score = deep - 10;
			p2.score = 10 - deep;
			break;
		}
	}
}

double avscore1 = 0;
double avscore2 = 0; 
pair<double, double>avscr;

void Trainer::train()
{
	vector<NW> newp1;
	vector<NW> newp2;
	int counter = 0;
	
	while (1)
	{
		avscore1 = 0;
		avscore2 = 0;
		for (int i = 0; i < _size; i++)
		{
			P1[i].avscore = 0;
			P2[i].avscore = 0;
		}

		for (int i = 0; i < _size; i++)
		{
			for (int j = 0; j < _size; j++)
			{
				score(P1[i], P2[j]);
				P1[i].avscore += P1[i].score;
				P2[j].avscore += P2[j].score;
			}
		}


		for (int i = 0; i < _size; i+=2)
		{
			P1[i].avscore /= _size;
			P1[i + 1].avscore /= _size;
			P2[i].avscore /= _size;
			P2[i + 1].avscore /= _size;
			avscore1 += P1[i].avscore + P1[i + 1].avscore;
			avscore2 += P2[i].avscore + P2[i + 1].avscore;
			if (P1[i].avscore < P1[i + 1].avscore)
			{
				P1[i].killme = true;
				P1[i + 1].killme = false;
			}
			else
			{
				P1[i].killme = false;
				P1[i + 1].killme = true;
			}
			
			if (P2[i].avscore < P2[i + 1].avscore)
			{
				P2[i].killme = true;
				P2[i + 1].killme = false;
			}
			else
			{
				P2[i].killme = false;
				P2[i + 1].killme = true;
			}
		}
		avscore1 /= _size;
		avscore2 /= _size;
		avscr.first = avscore1;
		avscr.second = avscore2;

		for (int i = 0; i < _size; i++)
		{
			if (P1[i].killme)
			{
				static int id1 = rand(0, _size - 1);
				static int id2 = rand(0, _size - 1);
				while (P1[id1].killme)id1 = rand(0, _size - 1);
				while (P1[id2].killme)id2 = rand(0, _size - 1);
				P1[i].MakeChild(P1[id1], P1[id2]);
			}
			if(P2[i].killme)
			{
				static int id1 = rand(0, _size - 1);
				static int id2 = rand(0, _size - 1);
				while (P2[id1].killme)id1 = rand(0, _size - 1);
				while (P2[id2].killme)id2 = rand(0, _size - 1);
				P2[i].MakeChild(P2[id1], P2[id2]);
			}
		}

		for (int i = 0; i < P1.size(); i++)
		{
			P1[i].Mutate();
			P2[i].Mutate();

		}

		

		PostMessageW(parent, MS_INCRBAR, NULL, (LPARAM) & avscr);
		counter++;
		if (counter > TrainLimit)break;
		if ((fabs(avscore1 - 9) < 0.1) && (fabs(avscore2 - 9) < 0.1))break;
	}
}

void Trainer::GetBest(link& p1, link& p2)
{
	int maxp1 = 0;
	int maxp2 = 0;
	for (int i = 0; i < _size; i++)
	{
		if (P1[maxp1].avscore < P1[i].avscore)maxp1 = i;
		if (P2[maxp2].avscore < P2[i].avscore)maxp2 = i;
	}

	if (p1 != nullptr)delete p1;
	if (p2 != nullptr)delete p2;
	p1 = new NW(P1[maxp1]);
	p2 = new NW(P2[maxp2]);
}







int Max(vector<double>& p)
{
	int res = 0;
	bool move = true;
	if (p[0] != INT_MAX)move = false;
	for (int i = 1; i < p.size(); i++)
	{
		if (p[i] == INT_MAX)
		{
			if (move)res++;
			continue;
		}
		else
		{
			if (move)
			{
				res++;
				move = !move;
			}
		}
		if (p[res] < p[i])res = i;
	}
	return res;
}