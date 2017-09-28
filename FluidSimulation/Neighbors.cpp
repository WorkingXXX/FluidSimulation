
#include "Neighbors.h"

void Neighbors::find_neighbors(vector<FluidParticle*> allParticles, FluidParticle * p) const {

	int particlesNum = allParticles.size();
	vector<FluidParticle*> neighbors;
	//cout << "paritcle " << p->index << " neighbors : ";
	for (int i = 0; i < particlesNum; i++) {
		if (i == p->index) continue;
		float dist = p->pos.distance_to(allParticles[i]->pos);
		//cout << i << " : " << dist << endl;
		if (dist <= p->smoothRadius) neighbors.push_back(allParticles[i]);
	}

	p->neighborsParticles = neighbors;
	
}