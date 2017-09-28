
#pragma region HEADERS

#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkAutoInit.h>
#include <vtkConeSource.h>
#include <vtkAnimationCue.h>
#include <vtkAnimationScene.h>
#include <vtkCommand.h>
#include <vtkProgrammableFilter.h>
#include <vtkCallbackCommand.h>
#include <vtkPolyLine.h>
#include <vtkCubeSource.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>

#include "Collision.h"
#include "FluidSystem.h"
#include "RenderSystem.h"
#include "BoundingBox.h"
#include "PhysicsConstant.h"

#pragma endregion

#pragma region INCLUDE_VTK_LIB
#pragma comment(lib, "freeglutd.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "vtkIOCore-7.1.lib")
#pragma comment(lib, "vtkFiltersCore-7.1.lib")
#pragma comment(lib, "vtkCommonCore-7.1.lib")
#pragma comment(lib, "vtkViewsCore-7.1.lib")
#pragma comment(lib, "vtkCommonDataModel-7.1.lib")
#pragma comment(lib, "vtkCommonExecutionModel-7.1.lib")
#pragma comment(lib, "vtkRenderingCore-7.1.lib")
#pragma comment(lib, "vtkCommonComputationalGeometry-7.1.lib")
#pragma comment(lib, "vtkexoIIc-7.1.lib")
#pragma comment(lib, "vtksys-7.1.lib")
#pragma comment(lib, "vtkexpat-7.1.lib")
#pragma comment(lib, "vtkChartsCore-7.1.lib")
#pragma comment(lib, "vtkCommonMisc-7.1.lib")
#pragma comment(lib, "vtkCommonSystem-7.1.lib")
#pragma comment(lib, "vtkCommonTransforms-7.1.lib")
#pragma comment(lib, "vtkFiltersExtraction-7.1.lib")
#pragma comment(lib, "vtkFiltersFlowPaths-7.1.lib")
#pragma comment(lib, "vtkFiltersGeneral-7.1.lib")
#pragma comment(lib, "vtkFiltersGeneric-7.1.lib")
#pragma comment(lib, "vtkFiltersHybrid-7.1.lib")
#pragma comment(lib, "vtkFiltersModeling-7.1.lib")
#pragma comment(lib, "vtkfreetype-7.1.lib")
#pragma comment(lib, "vtkGeovisCore-7.1.lib")
#pragma comment(lib, "vtkgl2ps-7.1.lib")
#pragma comment(lib, "vtkFiltersSources-7.1.lib")
#pragma comment(lib, "vtkglew-7.1.lib")
#pragma comment(lib, "vtkRenderingContextOpenGL2-7.1.lib")
#pragma comment(lib, "vtkRenderingOpenGL2-7.1.lib")
#pragma comment(lib, "vtkInteractionImage-7.1.lib")
#pragma comment(lib, "vtkInteractionStyle-7.1.lib")
#pragma comment(lib, "vtkInteractionWidgets-7.1.lib")
#pragma comment(lib, "vtkParallelCore-7.1.lib")
#pragma comment(lib, "vtkFiltersPoints-7.1.lib")
#pragma comment(lib, "vtkFiltersSources-7.1.lib")
#pragma comment(lib, "vtkFiltersGeometry-7.1.lib")
#pragma comment(lib, "vtkFiltersProgrammable-7.1.lib")
#pragma endregion

VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2  
VTK_MODULE_INIT(vtkInteractionStyle);

vector<FluidParticle*> particles;
vector<Vector3d> positions;
FluidSystem fluidSys;
SPH sph;

float time_ = 0.0;
float deta_time = 0.01;
int index = -1;

vtkSmartPointer<vtkActor> draw_bounding_box_wireframe(const BoundingBox & box);

class ParticlePositionModifier : public vtkCommand {

public :

	static ParticlePositionModifier * New() {
		return new ParticlePositionModifier;
	}

	virtual void Execute(vtkObject * caller, unsigned long eventId, void * vtkNotUsed(callData)) {
		cout << "execute sph algorithm " << endl;
		sph.execute_algorithm(particles);
#pragma omp parallel for
		for (int i = 0; i < particles.size(); i++) {
			//particles[i].pos.print();
			m_actors[i]->SetPosition(particles[i]->pos.get_x(), particles[i]->pos.get_z(), particles[i]->pos.get_y());
			//m_actors[i]->SetPosition(m_actors[i]->GetPosition()[0], m_actors[i]->GetPosition()[1] + 0.03, m_actors[i]->GetPosition()[2] + 0.03);
		}
		vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
		iren->GetRenderWindow()->Render();
	}

	void set_particles_positon(vector<Vector3d> particlesPos) {
		m_particlesPos = particlesPos;
	}

	void set_actors(vector<vtkSmartPointer<vtkActor>> actors) {
		m_actors = actors;
	}

private:

	vector<Vector3d> m_particlesPos;
	vector<vtkSmartPointer<vtkActor>> m_actors;

};

int main() {

	sph.set_time_step(deta_time);
	fluidSys.set_particles_init_dist(Particle_Radius * 2);

	particles = fluidSys.generate_fluid_particles(1);

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolyData> pointSet = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	/*for (int i = 0; i < particles.size(); i++) {
		double p[3] = { particles[i]->pos.get_x(), particles[i]->pos.get_y(), particles[i]->pos.get_z() };
		vertices->InsertNextCell(i + 1, points->InsertNextPoint(p));
	}
	pointSet->SetPoints(points);
	pointSet->SetVerts(vertices);
	pointMapper->SetInputData(pointSet);*/

	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetThetaResolution(10);
	sphere->SetPhiResolution(10);
	sphere->SetRadius(Particle_Radius);
	sphere->Update();

	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphere->GetOutputPort());

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.6, 0.6, 0.6);

#pragma omp parallel for
	vector<vtkSmartPointer<vtkActor>> actors;
	for (int i = 0; i < particles.size(); i++) {
		vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
		//sphereActor->GetProperty()->EdgeVisibilityOn();
		sphereActor->SetMapper(sphereMapper);
		sphereActor->GetProperty()->SetColor(0.1, 0.4, 0.8);
		sphereActor->SetPosition(particles[i]->pos.get_x(), particles[i]->pos.get_y(), particles[i]->pos.get_z());
		renderer->AddActor(sphereActor);
		positions.push_back(particles[i]->pos);
		actors.push_back(sphereActor);
	}	
/*
	BoundingBox box(Vector3d(0, 0, 0), Vector3d(5, 0, 0), Vector3d(5, 5, 0), Vector3d(0, 5, 0),
		Vector3d(0, 0, 5), Vector3d(5, 0, 5), Vector3d(5, 5, 5), Vector3d(0, 5, 5));
	vtkSmartPointer<vtkActor> boxframeActor = draw_bounding_box_wireframe(box);
	renderer->AddActor(boxframeActor);*/

	int scale_factor = 160;
	vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
	renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	renderer->GetActiveCamera()->SetPosition(Particle_Radius * scale_factor, Particle_Radius * scale_factor, Particle_Radius * scale_factor);
	renderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	//renderer->ResetCamera();
	//renderer->GetActiveCamera()->ParallelProjectionOn();
	
	//renderer->GetActiveCamera()->SetParallelScale(1.5);

	vtkSmartPointer<ParticlePositionModifier> ppm = vtkSmartPointer<ParticlePositionModifier>::New();
	ppm->set_actors(actors);
	ppm->set_particles_positon(positions);
	

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(1024, 768);
	renderWindow->AddRenderer(renderer);
	renderWindow->Render();

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, ppm);
	cout << renderWindowInteractor->CreateRepeatingTimer(100) << endl;
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;

}

//class ActorAnimator {
//
//public:
//
//	ActorAnimator() {
//		this->Actor = 0;
//		this->Observer = AnimationCueObserver::New();
//		this->Observer->Animator = this;
//		this->StartPosition.resize(3);
//		this->StartPosition.insert(this->StartPosition.begin(), 3, 0.0);
//		this->EndPosition.resize(3);
//		this->EndPosition.insert(this->EndPosition.begin(), 3, 0.5);
//	}
//
//	~ActorAnimator() {
//		if (this->Actor) {
//			this->Actor->UnRegister(0);
//			this->Actor = 0;
//		}
//		this->Observer->UnRegister(0);
//	}
//
//	void SetActor(vtkActor * actor) {
//		if (this->Actor) {
//			this->Actor->UnRegister(0);
//		}
//		this->Actor = actor;
//		this->Actor->Register(0);
//	}
//
//	void SetStartPosition(std::vector<double> position){
//		this->StartPosition = position;
//	}
//
//	void SetEndPosition(std::vector<double> position){
//		this->EndPosition = position;
//	}
//
//	void AddObserversToCue(vtkAnimationCue * cue) {
//		cue->AddObserver(vtkCommand::StartAnimationCueEvent, this->Observer);
//		cue->AddObserver(vtkCommand::EndAnimationCueEvent, this->Observer);
//		cue->AddObserver(vtkCommand::AnimationCueTickEvent, this->Observer);
//	}
//
//	void Start(vtkAnimationCue::AnimationCueInfo * vtkNotUsed(info)) {
//		this->Actor->SetPosition(this->StartPosition[0], this->StartPosition[1], this->StartPosition[2]);
//	}
//
//	void Tick(vtkAnimationCue::AnimationCueInfo * info) {
//		double t = (info->AnimationTime - info->StartTime) / (info->EndTime - info->StartTime);
//		double position[3];
//		for (int i = 0; i < 3; i++)
//		{
//			position[i] = this->StartPosition[i] + (this->EndPosition[i] - this->StartPosition[i]) * t;
//		}
//		this->Actor->SetPosition(position);
//	}
//
//	void End(vtkAnimationCue::AnimationCueInfo * vtkNotUsed(info)) {
//		this->Actor->SetPosition(this->EndPosition[0],this->EndPosition[1],this->EndPosition[2]);
//	}
//
//protected:
//	
//	class AnimationCueObserver : public vtkCommand {
//
//	public :
//
//		static AnimationCueObserver * New() {
//			return new AnimationCueObserver;
//		}
//
//		virtual void Execute(vtkObject * vtkNotUsed(caller), unsigned long event, void * calldata) {
//			if (this->Animator != 0) {
//				vtkAnimationCue::AnimationCueInfo * info =
//					static_cast<vtkAnimationCue::AnimationCueInfo*>(calldata);
//				switch (event) {
//					case vtkCommand::StartAnimationCueEvent:
//						this->Animator->Start(info); break;
//					case vtkCommand::EndAnimationCueEvent:
//						this->Animator->End(info); break;
//					case vtkCommand::AnimationCueTickEvent:
//						this->Animator->Tick(info); break;
//				}
//			}
//		}
//
//		AnimationCueObserver() {
//			this->Animator = 0;
//		}
//
//		ActorAnimator * Animator;
//
//	};
//
//	vtkActor * Actor;
//	AnimationCueObserver * Observer;
//	vector<double> StartPosition;
//	vector<double> EndPosition;
//
//};
//
//class AnimationSceneObserver : public vtkCommand {
//
//public:
//
//	static AnimationSceneObserver * New() {
//		return new AnimationSceneObserver;
//	}
//
//	virtual void Execute(vtkObject * vtkNotUsed(caller), unsigned long event, void * calldata) {
//		if (this->RenderWindow != 0) {
//			switch (event) {
//			case vtkCommand::AnimationCueTickEvent:
//				this->RenderWindow->Render();
//				break;
//			}
//		}
//	}
//
//	void SetRenderWindow(vtkRenderWindow * renWin) {
//		if (this->RenderWindow) {
//			this->RenderWindow->UnRegister(this);
//		}
//		this->RenderWindow = renWin;
//		this->RenderWindow->Register(this);
//	}
//
//protected:
//	
//	AnimationSceneObserver() {
//		this->RenderWindow = 0;
//	}
//
//	~AnimationSceneObserver() {
//		if (this->RenderWindow) {
//			this->RenderWindow->UnRegister(this);
//			this->RenderWindow = 0;
//		}
//	}
//
//	vtkRenderWindow * RenderWindow;
//};
//
////int main() {
////
////	vtkSmartPointer<vtkRenderWindowInteractor> iren =
////		vtkSmartPointer<vtkRenderWindowInteractor>::New();
////	vtkSmartPointer<vtkRenderWindow> renWin =
////		vtkSmartPointer<vtkRenderWindow>::New();
////	vtkSmartPointer<vtkRenderer> ren1 =
////		vtkSmartPointer<vtkRenderer>::New();
////	iren->SetRenderWindow(renWin);
////	renWin->SetMultiSamples(0);
////	renWin->AddRenderer(ren1);
////
////	vtkSmartPointer<vtkSphereSource> sphereSource =
////		vtkSmartPointer<vtkSphereSource>::New();
////	vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
////		vtkSmartPointer<vtkPolyDataMapper>::New();
////	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
////
////	vtkSmartPointer<vtkActor> sphereActor =
////		vtkSmartPointer<vtkActor>::New();
////	sphereActor->SetMapper(sphereMapper);
////	ren1->AddActor(sphereActor);
////
////	vtkSmartPointer<vtkConeSource> coneSource =
////		vtkSmartPointer<vtkConeSource>::New();
////	vtkSmartPointer<vtkPolyDataMapper> coneMapper =
////		vtkSmartPointer<vtkPolyDataMapper>::New();
////	coneMapper->SetInputConnection(coneSource->GetOutputPort());
////
////	vtkSmartPointer<vtkActor> coneActor =
////		vtkSmartPointer<vtkActor>::New();
////	coneActor->SetMapper(coneMapper);
////	ren1->AddActor(coneActor);
////
////
////	vtkSmartPointer<vtkAnimationScene> scene =
////		vtkSmartPointer<vtkAnimationScene>::New();
////	scene->SetModeToSequence();
////
////	scene->SetLoop(0);
////	scene->SetFrameRate(5);
////	scene->SetStartTime(0);
////	scene->SetEndTime(20);
////
////	vtkSmartPointer<AnimationSceneObserver> sceneObserver =
////		vtkSmartPointer<AnimationSceneObserver>::New();
////	sceneObserver->SetRenderWindow(renWin);
////
////	scene->AddObserver(vtkCommand::AnimationCueTickEvent, sceneObserver);
////
////	vtkSmartPointer<vtkAnimationCue> cue1 =
////		vtkSmartPointer<vtkAnimationCue>::New();
////	cue1->SetStartTime(5);
////	cue1->SetEndTime(23);
////	scene->AddCue(cue1);
////
////	vtkSmartPointer<vtkAnimationCue> cue2 =
////		vtkSmartPointer<vtkAnimationCue>::New();
////	cue2->SetStartTime(1);
////	cue2->SetEndTime(10);
////	scene->AddCue(cue2);
////
////	ActorAnimator animateSphere;
////	animateSphere.SetActor(sphereActor);
////	animateSphere.AddObserversToCue(cue1);
////
////	ActorAnimator animateCone;
////	vector<double> endCone(3);
////	endCone[0] = -1;
////	endCone[1] = -1;
////	endCone[2] = -1;
////	animateCone.SetEndPosition(endCone);
////	animateCone.SetActor(coneActor);
////	animateCone.AddObserversToCue(cue2);
////
////	renWin->Render();
////	ren1->ResetCamera();
////	ren1->GetActiveCamera()->Dolly(0.5);
////	ren1->ResetCameraClippingRange();
////
////	scene->Play();
////	scene->Stop();
////
////	iren->Start();
////
////	return EXIT_SUCCESS;
////
////}
//

//void TimerCallbackFunction(vtkObject * caller, long unsigned int eventId, void * clientData, void* vtkNotUsed(callData));
//
//unsigned int counter2 = 0;
//
//void AdjustPoint(void * arguments) {
//
//	vtkProgrammableFilter * programmableFilter = static_cast<vtkProgrammableFilter*>(arguments);
//	
//
//}
//
//void AdjustPoints2(void * arguments) {
//
//	vtkProgrammableFilter * programmableFilter =
//		static_cast<vtkProgrammableFilter*>(arguments);
//
//	vtkPoints * inPts = programmableFilter->GetPolyDataInput()->GetPoints();
//
//	vtkIdType numPts = inPts->GetNumberOfPoints();
//	vtkSmartPointer<vtkPoints> newPts = vtkSmartPointer<vtkPoints>::New();
//	newPts->SetNumberOfPoints(numPts);
//
//	for (vtkIdType i = 0; i < numPts; i++) {
//		double p[3];
//		inPts->GetPoint(i, p);
//		newPts->SetPoint(i, p);
//	}
//
//	double p0[3];
//	inPts->GetPoint(0, p0);
//	p0[2] = p0[2] + counter2 * 0.1;
//	newPts->SetPoint(0, p0);
//
//	programmableFilter->GetPolyDataOutput()->CopyStructure(programmableFilter->GetPolyDataInput());
//	programmableFilter->GetPolyDataOutput()->SetPoints(newPts);
//
//}
//
//
//int main() {
//
//	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
//	sphereSource->Update();
//	
//	vtkSmartPointer<vtkProgrammableFilter> programmableFilter =
//		vtkSmartPointer<vtkProgrammableFilter>::New();
//	programmableFilter->SetInputConnection(sphereSource->GetOutputPort());
//	programmableFilter->SetExecuteMethod(AdjustPoints2, programmableFilter);
//
//	vtkSmartPointer<vtkPolyDataMapper> mapper =
//		vtkSmartPointer<vtkPolyDataMapper>::New();
//	mapper->SetInputConnection(programmableFilter->GetOutputPort());
//	
//	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
//	actor->SetMapper(mapper);
//
//	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//	renderWindow->AddRenderer(renderer);
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	renderWindowInteractor->SetRenderWindow(renderWindow);
//	renderWindowInteractor->Initialize();
//	renderWindowInteractor->CreateRepeatingTimer(25);
//
//	vtkSmartPointer<vtkCallbackCommand> timerCallback = vtkSmartPointer<vtkCallbackCommand>::New();
//	timerCallback->SetCallback(TimerCallbackFunction);
//	timerCallback->SetClientData(programmableFilter);
//
//	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, timerCallback);
//	
//	renderer->AddActor(actor);
//	renderer->SetBackground(1, 1, 1);
//
//	renderWindow->Render();
//	renderWindowInteractor->Start();
//
//	return EXIT_SUCCESS;
//
//}
//
//void TimerCallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* clientData, void* vtkNotUsed(callData)) {
//
//	cout << "timer callback" << endl;
//
//	vtkSmartPointer<vtkProgrammableFilter> programmableFilter =
//		static_cast<vtkProgrammableFilter*>(clientData);
//
//	vtkRenderWindowInteractor * iren =
//		static_cast<vtkRenderWindowInteractor*>(caller);
//
//	programmableFilter->Modified();
//
//	iren->Render();
//
//	counter2++;
//
//}

vtkSmartPointer<vtkActor> draw_bounding_box_wireframe(const BoundingBox & box) {

	vector<Vector3d> vertices = box.get_vertices();

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

	polyLine->GetPointIds()->SetNumberOfIds(24);

	for (size_t i = 0; i < vertices.size(); i++) {
		double p[3] = { vertices[i].get_x(), vertices[i].get_y(), vertices[i].get_z() };
		points->InsertNextPoint(p);
		polyLine->GetPointIds()->SetId(2 * i, i);
		polyLine->GetPointIds()->SetId(2 * i + 1, i + 1);
	}

	polyLine->GetPointIds()->SetId(16, 0);
	polyLine->GetPointIds()->SetId(17, 4);
	polyLine->GetPointIds()->SetId(18, 1);
	polyLine->GetPointIds()->SetId(19, 5);
	polyLine->GetPointIds()->SetId(20, 2);
	polyLine->GetPointIds()->SetId(21, 6);
	polyLine->GetPointIds()->SetId(22, 3);
	polyLine->GetPointIds()->SetId(23, 7);

	cells->InsertNextCell(polyLine);
	polyData->SetPoints(points);
	polyData->SetLines(cells);
	mapper->SetInputData(polyData);
	actor->SetMapper(mapper);

	return actor;

}