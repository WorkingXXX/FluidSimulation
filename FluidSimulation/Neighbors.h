#pragma once

#ifndef NEIGHBORS_H
#define NEIGHBORS_H

#include <vector>

#include "FluidParticle.h"

using namespace std;

class Neighbors {

public:

	void find_neighbors(vector<FluidParticle*> allParticles, FluidParticle * p) const;

};

#endif
