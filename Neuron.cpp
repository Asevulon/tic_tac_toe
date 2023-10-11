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

	for (int i = 0; i < LS[0]; i++)
	{
		Layer[0][i] = neuron(Enters);
	}
	for (int i = 1; i < LS.size(); i++)
	{
		for (int j = 0; j < LS[i]; j++)
		{
			Layer[i][j] = neuron(LS[i - 1]);
		}
	}
}


void NW::ProcessNeuron(neuron& n, vector<double>& in)
{
	n.in = in;
	double res = 0;
	for (int i = 0; i < in.size(); i++)
	{
		res += n.w[i] * in[i];
	}
	res -= n.fi;
	n.out = f(res);
}

int NW::Calc(vector<double>& in)
{
	vector<double>tin = in;
	for (int i = 0; i < Layer.size(); i++)
	{
		for (int j = 0; j < Layer[i].size(); j++)
		{
			ProcessNeuron(Layer[i][j], tin);
		}
		tin.clear();
		for (int j = 0; j < Layer[i].size(); j++)
		{
			tin.push_back(Layer[i][j].out);
		}
	}

	int res = 0;
	for (int i = 3; i > -1; i--)
	{
		res = (res << 1) & (int)round(tin[i]);
	}
}
int NW::Calc(vector<int>& in)
{
	vector<double>tin;
	for (auto& item : in)tin.push_back(item);


	for (int i = 0; i < Layer.size(); i++)
	{
		for (int j = 0; j < Layer[i].size(); j++)
		{
			ProcessNeuron(Layer[i][j], tin);
		}
		tin.clear();
		for (int j = 0; j < Layer[i].size(); j++)
		{
			tin.push_back(Layer[i][j].out);
		}
	}

	int res = 0;
	for (int i = 3; i > -1; i--)
	{
		res = (res << 1) & (int)round(tin[i]);
	}
}

double NW::f(double x)
{
	return 1. / (1. + exp(-2 * x));
}


NW NW::MakeChild(NW& nw)
{
	NW res(ls, 9);

	double chance = 0;
	for (int i = 0; i < Layer.size(); i++)
	{
		for (int j = 0; j < Layer[i].size(); j++)
		{
			for (int k = 0; k < Layer[i][j].w.size(); k++)
			{
				chance = rand(0, 1);
				res.Layer[i][j].w[k] = chance * Layer[i][j].w[k] + (1 - chance) * nw.Layer[i][j].w[k];
			}
			chance = rand(0, 1);
			res.Layer[i][j].fi = chance * Layer[i][j].fi + (1 - chance) * nw.Layer[i][j].fi;
		}
	}
	
	return res;
}

void NW::Mutate()
{
	double M = 0.1 * (PERFECT_SCORE - score) / PERFECT_SCORE;
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

	vector<int>LS = { 9,14,9,4 };

	for (int i = 0; i < _size; i++)
	{
		NW nw1(LS, 9);
		NW nw2(LS, 9);
		P1.push_back(nw1);
		P2.push_back(nw2);
	}
}


#define DRAW 1000
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
void Trainer::score(NW& p1, NW& p2)
{
	vector<int> data(9, 0);
	int deep = 0;
	int zeroes = 9;
	int winner = 0;
	while (zeroes > 0)
	{
		int pos = p1.Calc(data);
		deep++;
		zeroes--;
		if (data[pos] != 0)
		{
			p1.score = deep - 100;
			p2.score = 10 - deep;
			break;
		}
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


		pos = p2.Calc(data);
		deep++;
		zeroes--;
		if (data[pos] != 0)
		{
			p1.score = 10 - deep;
			p2.score = deep - 100;
			break;
		}
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


void Trainer::train()
{
	int counter = 0;
	while (1)
	{
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
		for (auto iter = P1.begin(); iter != P1.end(); iter++)
		{
			if (iter->killme)P1.erase(iter);
		}
		for (auto iter = P2.begin(); iter != P2.end(); iter++)
		{
			if (iter->killme)P2.erase(iter);
		}

		for (int i = 0; i < P1.size(); i++)
		{
			P1[i].Mutate();
		}
		for (int i = 0; i < P2.size(); i++)
		{
			P2[i].Mutate();
		}

		for (int i = 0; i < _size / 2; i++)
		{
			int id = rand(0, _size / 2. - 1);
			while (id == i)id = rand(0, _size / 2. - 1);
			P1.push_back(P1[i].MakeChild(P1[id]));
			P2.push_back(P2[i].MakeChild(P2[id]));
		}

		counter++;
		if (counter > 100)break;
	}
}