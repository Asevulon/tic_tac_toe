#include"pch.h"
#include"Neuron.h"
#include<fstream>
#include<thread>
double rand(double left, double right)
{
	return left + (right - left) * double(rand()) / double(RAND_MAX);
}


neuron::neuron(int size)
{
	w = vector<double>(size, 0);
	for (int i = 0; i < size; i++)
	{
		w[i] = rand(-0.5, 0.5);
	}
	fi = rand(-0.5, 0.5);
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

double g_res = 0;

void NW::ProcessNeuron(neuron& n, vector<double>& in, int len)
{
	for (int i = 0; i < len; i++)
	{
		g_res += n.w[i] * in[i];
	}
	g_res -= n.fi;
	n.out = f(g_res);
}

vector<double>Tin(500, 0);
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
	return tanh(x);//1. / (1. + exp(-2 * x));
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
	double M = MutationVal * (PERFECT_SCORE - avscore) / PERFECT_SCORE;
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

ULONG64 g_gamectr = 0;
ofstream gamelog;
void Trainer::score(NW& p1, NW& p2)
{
	g_gamectr++;
	//gamelog << " game: " << g_gamectr << endl;

	vector<int> data(9, 0);
	int deep = 0;
	int zeroes = 9;
	int winner = 0;
	int pos = 0;
	while (zeroes > 0)
	{
		pos = p1.MakePredictions(data, 1);
		//gamelog << pos << ' ';
		deep++;
		zeroes--;
		data[pos] = 1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p1.score = deep + DRAW_SCORE;
			p2.score = deep + DRAW_SCORE;
			break;
		}
		if (winner == 1)
		{
			p1.score = PERFECT_SCORE - deep;
			p2.score = deep;
			break;
		}
		if (winner == -1)
		{
			p1.score = deep;
			p2.score = PERFECT_SCORE - deep;
			break;
		}


		pos = p2.MakePredictions(data, -1);
		//gamelog << pos << ' ';

		deep++;
		zeroes--;
		data[pos] = -1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p1.score = deep + DRAW_SCORE;
			p2.score = deep + DRAW_SCORE;
			break;
		}
		if (winner == 1)
		{
			p1.score = PERFECT_SCORE - deep;

			p2.score = deep;
			break;
		}
		if (winner == -1)
		{
			p1.score = deep;
			p2.score = PERFECT_SCORE - deep;
			break;
		}
	}
	//gamelog << endl << endl;
}

inline int closestpos(vector<int>& data, int pos)
{
	if (data[pos] == 0)return pos;
	int l = pos;
	int r = pos;
	while (l > -1)
	{
		if (data[l] == 0)break;
		l--;
	}
	while (r < 9)
	{
		if (data[r] == 0)break;
		r++;
	}
	if (l == -1)return r;
	if (r == 9)return l;
	if (pos - l < r - pos)return l;
	else return r;
}
void Trainer::scoreVSrandom(NW& p, int turn)
{
	vector<int> data(9, 0);
	int deep = 0;
	int zeroes = 9;
	int winner = 0;
	int pos = 0;
	
	if (turn == -1)
	{
		pos = closestpos(data, rand(0, 8.9999));
		deep++;
		zeroes--;
		data[pos] = -1;
	}
	while (zeroes > 0)
	{
		pos = p.MakePredictions(data, 1);
		//gamelog << pos << ' ';
		deep++;
		zeroes--;
		data[pos] = 1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p.score = deep + DRAW_SCORE;
			break;
		}
		if (winner == 1)
		{
			p.score = PERFECT_SCORE - deep;
			break;
		}
		if (winner == -1)
		{
			p.score = deep;
			break;
		}


		pos = closestpos(data, rand(0, 8.9999));
		//gamelog << pos << ' ';

		deep++;
		zeroes--;
		data[pos] = -1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p.score = deep + DRAW_SCORE;
			break;
		}
		if (winner == 1)
		{
			p.score = PERFECT_SCORE - deep;
			break;
		}
		if (winner == -1)
		{
			p.score = deep;
			break;
		}
	}
}
double avscore1 = 0;
double avscore2 = 0; 
TrainerInfo TF;

void Trainer::train()
{
	vector<NW> newp1;
	vector<NW> newp2;
	int counter = 0;
	gamelog.open("gamelog.txt");
	while (1)
	{
		avscore1 = 0;
		avscore2 = 0;
		for (int i = 0; i < _size; i++)
		{
			P1[i].avscore = 0;
			P2[i].avscore = 0;
		}

		/*for (int i = 0; i < _size; i++)
		{
			for (int j = 0; j < _size; j++)
			{
				score(P1[i], P2[j]);
				P1[i].avscore += P1[i].score;
				P2[j].avscore += P2[j].score;
			}
		}*/
		thread thr1([this]()
			{
				for (int i = 0; i < _size / 4; i++)
				{
					for (int j = 0; j < _gamesvsrand; j++)
					{
						scoreVSrandom(P1[i], 1);
						P1[i].avscore += P1[i].score;

						scoreVSrandom(P2[i], -1);
						P2[i].avscore += P2[i].score;

					}
				}
			}
		); 
		thread thr2([this]()
			{
				for (int i = _size/4; i < _size / 2; i++)
				{
					for (int j = 0; j < _gamesvsrand; j++)
					{
						scoreVSrandom(P1[i], 1);
						P1[i].avscore += P1[i].score;

						scoreVSrandom(P2[i], -1);
						P2[i].avscore += P2[i].score;

					}
				}
			}
		);
		thread thr3([this]()
			{
				for (int i = _size/2; i < 3 * _size / 4; i++)
				{
					for (int j = 0; j < _gamesvsrand; j++)
					{
						scoreVSrandom(P1[i], 1);
						P1[i].avscore += P1[i].score;

						scoreVSrandom(P2[i], -1);
						P2[i].avscore += P2[i].score;

					}
				}
			}
		);
		for (int i = 3 * _size / 4; i < _size; i++)
		{
			for (int j = 0; j < _gamesvsrand; j++)
			{
				scoreVSrandom(P1[i], 1);
				P1[i].avscore += P1[i].score;

				scoreVSrandom(P2[i], -1);
				P2[i].avscore += P2[i].score;

			}
		}

		thr1.join();
		thr2.join();
		thr3.join();

		for (int i = 0; i < _size; i++)
		{
			P1[i].avscore /= _gamesvsrand;
			P2[i].avscore /= _gamesvsrand;
			avscore1 += P1[i].avscore;
			avscore2 += P2[i].avscore;
		}
		avscore1 /= _size;
		avscore2 /= _size;

		TF.av1 = avscore1;
		TF.av2 = avscore2;
		TF.win1 = _size;
		TF.win2 = _size;

		/*for (int i = 0; i < _size; i+=2)
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
		}*/
		for (int i = 0; i < _size; i++)
		{
			if (P1[i].avscore < avscore1)P1[i].killme = true;
			else P1[i].killme = false;
			if (P2[i].avscore < avscore2)P2[i].killme = true;
			else P2[i].killme = false;
		}

		for (int i = 0; i < _size; i++)
		{
			if (P1[i].killme)
			{
				int id1 = rand(0, _size - 1e-6);
				int id2 = rand(0, _size - 1e-6);
				while (P1[id1].killme)id1 = rand(0, _size - 1e-6);
				while (P1[id2].killme)id2 = rand(0, _size - 1e-6);
				P1[i].MakeChild(P1[id1], P1[id2]);
			}
			if(P2[i].killme)
			{
				int id1 = rand(0, _size - 1e-6);
				int id2 = rand(0, _size - 1e-6);
				while (P2[id1].killme)id1 = rand(0, _size - 1e-6);
				while (P2[id2].killme)id2 = rand(0, _size - 1e-6);
				P2[i].MakeChild(P2[id1], P2[id2]);
			}
		}

		/*double var = 0;
		for (int i = 0; i < _size; i++)
		{
			var = KillVaraity * (PERFECT_SCORE - P1[i].avscore) / PERFECT_SCORE;
			if (rand(0, 1) < var)P1[i].MakeChild(P1[rand(0, _size - 1e-6)], P1[rand(0, _size - 1e-6)]);

			var = KillVaraity * (PERFECT_SCORE - P2[i].avscore) / PERFECT_SCORE;
			if (rand(0, 1) < var)P2[i].MakeChild(P2[rand(0, _size - 1e-6)], P2[rand(0, _size - 1e-6)]);
		}*/
		for (int i = 0; i < _size; i++)
		{
			//var = MutationVaraity * (PERFECT_SCORE - P1[i].avscore) / PERFECT_SCORE;
			if (rand(0, 1) < MutationVaraity)P1[i].Mutate();

			//var = MutationVaraity * (PERFECT_SCORE - P2[i].avscore) / PERFECT_SCORE;
			if (rand(0, 1) < MutationVaraity)P2[i].Mutate();

		}

		

		SendMessageW(parent, MS_INCRBAR, NULL, (LPARAM) & TF);
		counter++;
		if (counter > TrainLimit)break;
		if ((fabs(avscore1 - 9) < 0.1) && (fabs(avscore2 - 9) < 0.1))break;
		if (stop)break;
	}
	gamelog.close();
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