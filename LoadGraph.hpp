#pragma once
#include "Common.hpp"

class LoadGraph : public App::Scene
{
public:

	LoadGraph(const InitData& init);

	void update() override;

	void draw() const override;

private:

	String edgePath = U"text/grundy/edge.bin";
	String grundyPath = U"text/grundy/grundy.bin";
	String statePath = U"text/grundy/state.bin";

	BinaryReader edgeReader{ Resource(edgePath) };
	BinaryReader grundyReader{ Resource(grundyPath) };
	BinaryReader stateReader{ Resource(statePath) };

	int32 OptimalNode, OptimalEdge;

	void PreReadData() {
		stateReader.read(OptimalNode);
		edgeReader.read(OptimalEdge);

		loadBlock = (OptimalNode * 3 + OptimalEdge) / 100 + 1;
	}

	int32 ReadState(int32 id, int32 n) { // N
		int32 num;
		int32 res = 0;
		for (auto i : step(n)) {
			if (id >= OptimalNode) { break; }

			stateReader.read(num);
			getData().conv_state[id] = num;
			res++;
			id++;
		}
		return res;
	}
	int32 ReadGrundy(int32 id, int32 n) { // N
		int32 num;
		int32 res = 0;
		
		for (auto i : step(n)) {
			if (id >= OptimalNode) { break; }

			grundyReader.read(num);
			getData().grundy[id] = num;
			res++;
			id++;
		}

		return res;
	}
	int32 ReadInvState(int32 id, int32 n) {	// N
		int32 res = 0;

		for (auto i : step(n)) {
			if (id >= OptimalNode) { break; }

			getData().inv_state.insert({ getData().conv_state[id], id });

			res++;
			id++;
		}

		return res;
	}
	int32 ReadEdge(int32 id, int32 n) {	// M
		int32 num;
		int32 num2;
		int32 res = 0;

		for (auto i : step(n)) {
			if (id >= OptimalEdge) { break; }

			edgeReader.read(num);
			edgeReader.read(num2);
			getData().edge[num] << num2;

			res++;
			id++;
		}

		return res;
	}
	
	int64 progress = 0;
	int64 progressPercent = 0;

	int32 loadBlock;
};

