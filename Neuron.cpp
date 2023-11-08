#include"pch.h"
#include"Neuron.h"
#include<fstream>
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

NW& NW::operator =(const NW& nw)
{
	Layer = nw.Layer;
	return *this;
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
	double g_res = 0;

	for (int i = 0; i < len; i++)
	{
		g_res += n.w[i] * in[i];
	}
	g_res -= n.fi;
	n.out = f(g_res);
}
vector<double>Tin(15, 0);


MyTree mt;
template<typename T>
double NW::Calc(vector<T>& in)
{
	int g_len = 0;

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


template<typename T>  inline void NW::RotateLeft(vector<T>& data)
{
	/*
	|0|1|2|		|2|5|8|
	|3|4|5| --> |1|4|7|
	|6|7|8|		|0|3|6|
	*/
	T temp[9];
	temp[0] = data[2];
	temp[1] = data[5];
	temp[2] = data[8];
	temp[3] = data[1];
	temp[4] = data[4];
	temp[5] = data[7];
	temp[6] = data[0];
	temp[7] = data[3];
	temp[8] = data[6];
	for (int i = 0; i < 9; i++)data[i] = temp[i];
}
template<typename T>  inline void NW::RotateRight(vector<T>& data)
{
	/*
	|0|1|2|		|6|3|0|
	|3|4|5| --> |7|4|1|
	|6|7|8|		|8|5|2|
	*/
	T temp[9];
	temp[0] = data[6];
	temp[1] = data[3];
	temp[2] = data[0];
	temp[3] = data[7];
	temp[4] = data[4];
	temp[5] = data[1];
	temp[6] = data[8];
	temp[7] = data[5];
	temp[8] = data[2];
	for (int i = 0; i < 9; i++)data[i] = temp[i];
}
template<typename T>  inline void NW::ReflectV(vector<T>& data)
{
	/*
		|0|1|2|		|2|1|0|
		|3|4|5| --> |5|4|3|
		|6|7|8|		|8|7|6|
		*/
	Swap(data, 0, 2);
	Swap(data, 3, 5);
	Swap(data, 6, 8);
}
template<typename T>  inline void NW::ReflectH(vector<T>& data)
{
	/*
		|0|1|2|		|6|7|8|
		|3|4|5| --> |3|4|5|
		|6|7|8|		|0|1|2|
		*/
	Swap(data, 0, 6);
	Swap(data, 1, 7);
	Swap(data, 2, 8);
}
template<typename T>  inline void NW::Swap(vector<T>& data, int i, int j)
{
	T temp = data[i];
	data[i] = data[j];
	data[j] = temp;
}

inline bool NW::TryToFind(vector<int>& data, stack<int>& actions)
{
	for (int i = 0; i < 4; i++)
	{
		if (mt.Seek(data)) return true;

		ReflectH(data);
		actions.push(REFLECTH);
		if (mt.Seek(data)) return true;

		ReflectV(data);
		actions.push(REFLECTV);
		if (mt.Seek(data)) return true;

		ReflectH(data);
		actions.push(REFLECTH);
		if (mt.Seek(data)) return true;

		actions.pop();
		actions.pop();
		actions.pop();
		ReflectV(data);

		RotateRight(data);
		actions.push(ROTATERIGHT);
	}
	return false;
}

inline void NW::RestoreData(vector<int>& data, stack<int>& actions)
{
	while (!actions.empty())
	{
		int top = actions.top();
		switch (top)
		{
		case REFLECTH:ReflectH(data);
			break;

		case REFLECTV: ReflectV(data);
			break;

		case ROTATELEFT: RotateRight(data);
			break;
		case ROTATERIGHT: RotateLeft(data);
		default:
			break;
		}
		actions.pop();
	}
}

inline void NW::RestoreMax(vector<double>& data, stack<int>actions)
{
	while (!actions.empty())
	{
		int top = actions.top();
		switch (top)
		{
		case REFLECTH:ReflectH(data);
			break;

		case REFLECTV: ReflectV(data);
			break;

		case ROTATELEFT: RotateRight(data);
			break;
		case ROTATERIGHT: RotateLeft(data);
		default:
			break;
		}
		actions.pop();
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
	stack<int> actions;
	bool wasfound = TryToFind(in, actions);
	if (!wasfound)mt.Add(in);

	for (int i = 0; i < 9; i++)
	{
		if (in[i] == 0)
		{
			
			in[i] = turn;
			res[i] = Calc(in);
			in[i] = 0;
		}
	}
	if (wasfound)
	{
		RestoreMax(res, actions);
		RestoreData(in, actions);
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

void Trainer::scorefirst(NW& p1, NW& p2)
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
			break;
		}
		if (winner == 1)
		{
			p1.score = PERFECT_SCORE - deep;
			break;
		}
		if (winner == -1)
		{
			p1.score = deep;
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
			break;
		}
		if (winner == 1)
		{
			p1.score = PERFECT_SCORE - deep;
			break;
		}
		if (winner == -1)
		{
			p1.score = deep;
			break;
		}
	}
}
void Trainer::scoresecond(NW& p1, NW& p2)
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
		deep++;
		zeroes--;
		data[pos] = 1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p2.score = deep + DRAW_SCORE;
			break;
		}
		if (winner == 1)
		{
			p2.score = deep;
			break;
		}
		if (winner == -1)
		{
			p2.score = PERFECT_SCORE - deep;
			break;
		}


		pos = p2.MakePredictions(data, -1);
		deep++;
		zeroes--;
		data[pos] = -1;
		winner = FindWinner(data, zeroes);
		if (winner == DRAW)
		{
			p2.score = deep + DRAW_SCORE;
			break;
		}
		if (winner == 1)
		{

			p2.score = deep;
			break;
		}
		if (winner == -1)
		{
			p2.score = PERFECT_SCORE - deep;
			break;
		}
	}
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

thread* Trainer::makethread(int left, int right)
{
	thread* res = new thread([this, &left, &right]()
		{
			for (int i = left; i < right; i++)
			{
				for (int j = 0; j < _gamesvsrand; j++)
				{
					scorefirst(P1[i], P2[rand(0, _size - 1e-6)]);
					scoresecond(P1[rand(0, _size - 1e-6)], P2[i]);
					P1[i].avscore += P1[i].score;
					P2[i].avscore += P2[i].score;
				}
			}
		});
	return res;
}

void Trainer::train()
{
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
		for (int i = 0; i < _size; i++)
		{
			for (int j = 0; j < _gamesvsrand; j++)
			{
				scorefirst(P1[i], P2[rand(0, _size - 1e-6)]);
				scoresecond(P1[rand(0, _size - 1e-6)], P2[i]);
				P1[i].avscore += P1[i].score;
				P2[i].avscore += P2[i].score;
			}
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
		/*thread thr1([this]()
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
		thr3.join();*/

		/*int step = _size / 8;
		thread* threads[16];
		for (int i = 0; i < 8; i++)
		{
			threads[i] = makethread(i * step, (i + 1) * step);
		}
		for (int i = 0; i < 8; i++)
		{
			threads[i]->join();
			delete threads[i];
		}*/

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
		/*for (int i = 0; i < _size; i++)
		{
			if (P1[i].avscore < avscore1)P1[i].killme = true;
			else P1[i].killme = false;
			if (P2[i].avscore < avscore2)P2[i].killme = true;
			else P2[i].killme = false;
		}*/


		for (int i = 0; i < _size; i++)
		{
			if (P1[i].killme)
			{
				/*if (P1[i].avscore < 12)
				{
					P1[i] = NW(LS, 9);
					continue;
				}*/
				int l = i - 1;
				int r = i + 1;
				if (l < 0)l = _size - 1;
				if (r > _size - 1)r = 0;
				while (P1[l].killme)
				{
					l--;
					if (l < 0)l = _size - 1;
				}
				while (P1[r].killme)
				{
					r++;
					if (r > _size - 1)r = 0;
				}
				P1[i].MakeChild(P1[l], P1[r]);
			}
			if (P2[i].killme)
			{
				/*if (P2[i].avscore < 7)
				{
					P2[i] = NW(LS, 9);
					continue;
				}*/
				int l = i - 1;
				int r = i + 1;
				if (l < 0)l = _size - 1;
				if (r > _size - 1)r = 0;
				while (P2[l].killme)
				{
					l--;
					if (l < 0)l = _size - 1;
				}
				while (P2[r].killme)
				{
					r++;
					if (r > _size - 1)r = 0;
				}
				P2[i].MakeChild(P2[l], P2[r]);
			}
		}

		/*for (int i = 0; i < _size; i++)
		{
			if (P1[i].killme)P1[i] = NW(LS, 9);
			if (P2[i].killme)P2[i] = NW(LS, 9);
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
		if ((fabs(avscore1 - 18) < 0.1) && (fabs(avscore2 - 18) < 0.1))break;
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


void MyTree::Add(vector<int>& data)
{
	count++;
	Node* cur = &start;
	for (int i = 0; i < data.size(); i++)
	{
		if (cur->next[data[i] + 1] == nullptr)
		{
			cur->next[data[i] + 1] = new Node;
			cur->next[data[i] + 1]->val = data[i];
		}
		cur = cur->next[data[i] + 1];
	}
}
bool MyTree::Seek(vector<int>& data)
{
	Node* cur = &start;
	for (int i = 0; i < data.size(); i++)
	{
		if (cur->next[data[i] + 1] == nullptr)return false;
		cur = cur->next[data[i] + 1];
	}
	return true;
}