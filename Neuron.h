#pragma once

#include<afxwin.h>
#include<vector>
using namespace std;

#define PERFECT_SCORE 27.
#define DRAW_SCORE 9
#define MS_INCRBAR WM_USER + 5
struct neuron
{
	double out = 0;

	vector<double> w;
	double fi = 0;

	neuron() {}
	neuron(int size);
};


class NW
{
private:
	vector<vector<neuron>>Layer;
	vector<int>LayerSize;

	vector<int>ls;
	const double MutationVal = 0.5;
protected:
	inline void ProcessNeuron(neuron& n, vector<double>& in, int len);
	inline double f(double x);
public:
	NW::NW(vector<int>& LS, int Enters);

	template<typename T>
	inline double Calc(vector<T>& in);
	inline void NW::MakeChild(NW& left, NW& right);
	inline void NW::Mutate();
	inline int MakePredictions(vector<int>& in, int turn);

	double score;
	double avscore;
	bool killme = false;
};


const double MutationVaraity = 0.5;
const double KillVaraity = 0.5;

typedef  NW* link;
class Trainer
{
private:
	vector<NW>P1;
	vector<NW>P2;
	static const int _size = 64;
	static const int _gamesvsrand = 50;
	vector<int>LS = { 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1 };

protected:
	inline int Roulete(vector<NW>& data);
	void Trainer::scoreVSrandom(NW& p, int turn);

public:
	int TrainLimit = 1000;
	Trainer();
	HWND parent;
	void GetBest(link& p1, link& p2);
	inline void score(NW& p1, NW& p2);
	void train();
	bool stop = false;
};

#define DRAW 1000

inline int FindWinner(std::vector<int>& data, int zeros);
inline int Max(std::vector<double>& p);


struct TrainerInfo
{
	double av1 = 0;
	double av2 = 0;
	int win1 = 0;
	int win2 = 0;
};