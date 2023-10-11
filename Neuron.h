#pragma once

#include<afxwin.h>
#include<vector>
using namespace std;

#define PERFECT_SCORE 10.
#define MS_INCRBAR WM_USER + 5
struct neuron
{
	vector<double> in;
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

protected:
	inline void ProcessNeuron(neuron& n, vector<double>& in);
	inline double f(double x);
public:
	NW::NW(vector<int>& LS, int Enters);

	inline int NW::Calc(vector<int>& in);
	inline int Calc(vector<double>& in);
	inline NW NW::MakeChild(NW& nw);
	inline void NW::Mutate();

	double score;
	double avscore;
	bool killme = false;
};



typedef  NW* link;
class Trainer
{
private:
	vector<NW>P1;
	vector<NW>P2;
	static const int _size = 500;
	vector<int>LS = { 9,14,14,14,14,14,14,14,14,9,4 };


protected:
public:
	int TrainLimit = 100;
	Trainer();
	HWND parent;
	void GetBest(link& p1, link& p2);
	inline void score(NW& p1, NW& p2);
	void train();
};

#define DRAW 1000

inline int FindWinner(vector<int>& data, int zeros);