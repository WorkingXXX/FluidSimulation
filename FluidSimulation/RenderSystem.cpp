
#include "RenderSystem.h"

RenderSystem::RenderSystem() {

}

void RenderSystem::display_render_window(int argc, char ** argv) const {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 400);
	glutInitContextVersion(4, 5);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("FluidSimulation");

}

void draw() {}

void RenderSystem::execute_render() const {

	glutDisplayFunc(draw);

	glutMainLoop();

}

void RenderSystem::init_render_context() const{

	// 初始化 glew 环境
	GLenum initGlewResult = glewInit();

	if (initGlewResult != GLEW_OK) 
		std::cout << "Error : Fail to initialize GLEW\n";
	else 
		std::cout << "Using GLEW version : "<< glewGetString(GLEW_VERSION) << "\n";

	std::cout << "Using OpenGL version : " << glGetString(GL_VERSION) << "\n";

}

GLuint RenderSystem::compiler_shader(const char * shaderPath, GLenum shaderType) const{

	/** 从glsl文件中读入着色器代码 **/
	using namespace std;

	ifstream shaderFile;
	shaderFile.open(shaderPath, ios::in);

	if(!shaderFile.is_open()){
		cout << "Error : Fail to open file " << shaderPath << endl;
		return 0;
	}

	// 获取文件长度
	shaderFile.seekg(0);
	int startPos = shaderFile.tellg();
	shaderFile.seekg(0, ios_base::end);
	int endPos = shaderFile.tellg();
	shaderFile.seekg(0);

	int fileLength = endPos - startPos;
	
	if (fileLength == 0) return 0;

	// 读取文件内容
	vector<string> shaderCode;
	string shaderLine = "";
	char c;

	while (shaderFile.get(c)) {
		shaderLine += c;
	}
	shaderCode.push_back(shaderLine);
	cout << shaderLine.c_str();

	GLubyte * shaderCodePtr = (GLubyte*)new char[fileLength+1];
	shaderCodePtr[fileLength] = 0;

	int i;
	for (i = 0; i < shaderLine.size(); i++) {
		shaderCodePtr[i] = shaderLine[i];
	}
	shaderCodePtr[i++] = 0;

	/*for (int i = 0; i < shaderCode.size(); i++) {
		shaderCodePtr[i] = new GLchar[shaderCode[i].size()+1];
		int j;
		for (j = 0; j < shaderCode[i].size(); j++) {
			shaderCodePtr[i][j] = shaderCode[i][j];
		}
		shaderCodePtr[i][j++] = 0;
	}*/
	
	// 编译shader
	GLuint shaderObject = glCreateShader(shaderType);
	glShaderSourceARB(shaderObject, 1, (const GLcharARB**)&shaderCodePtr, &fileLength);

	GLint shaderCompiledResult;
	glCompileShaderARB(shaderObject);
	glGetObjectParameterivARB(shaderObject, GL_COMPILE_STATUS, &shaderCompiledResult);

	// 如果编译shader失败，输出编译器日志
	if(!shaderCompiledResult){

		cout << "Error : Fail to compile shader " << shaderPath << endl;

		GLint logLength;
		GLsizei size;
		
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 1) {
			GLchar * compilerLog = new GLchar[logLength];
			glGetInfoLogARB(shaderObject, logLength, &size, compilerLog);
			cout << "The Compiler Log : " << endl;
			cout << compilerLog << endl;
			delete compilerLog;
		}

		return 0;
	}
	else {
		cout << "Compiled shader " << shaderPath << endl;
	}

	shaderFile.close();

	return shaderObject;
}

GLuint RenderSystem::link_shader(const GLuint v, const GLuint g_, const GLuint f) {

	using namespace std;

	GLuint shaderProgram = glCreateProgram();

	if (v) glAttachShader(shaderProgram, v);
	if (g_) glAttachShader(shaderProgram, g_);
	if (f) glAttachShader(shaderProgram, f);

	glLinkProgram(shaderProgram);

	GLint shaderLinkedResult;
	glGetProgramivARB(shaderProgram, GL_LINK_STATUS, &shaderLinkedResult);

	if (shaderLinkedResult) {
		return shaderProgram;
	}
	// 若链接失败，输出链接日志
	else {
		cout << "Error : Fail to link shader program" << endl;

		GLint logLength = 0;
		GLsizei size = 0;

		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 1) {
			GLchar * linkerLog = new GLchar[logLength];
			glGetInfoLogARB(shaderProgram, logLength, &size, linkerLog);
			cout << "The Linker Log : " << endl;
			cout << linkerLog << endl;
			delete linkerLog;
		}

		return 0;
	}

}

GLint RenderSystem::create_shader(const char * shadersPath[]) {

	using namespace std;

	GLuint v = 0, g_ = 0, f = 0;

	if (shadersPath[0] != "") v = compiler_shader(shadersPath[0], GL_VERTEX_SHADER);
	if (shadersPath[1] != "") g_ = compiler_shader(shadersPath[1], GL_GEOMETRY_SHADER);
	if (shadersPath[2] != "") f = compiler_shader(shadersPath[2], GL_FRAGMENT_SHADER);
	//cout << endl << v << ", " << g_ << ", " << f << endl;
	GLuint shaderId = link_shader(v, g_, f);
	//cout << "shaderId : " << shaderId << endl;
	if (shaderId > 0) {
		m_shaders.push_back(shaderId);
		m_shadersNum = m_shaders.size();
		//return m_shadersNum-1;
		return shaderId;
	}
	else {
		cout << "Error : Fail to create shader" << endl;
		return -1;
	}

}

bool RenderSystem::use_shader(const int shaderId) const{

	if (0 <= shaderId && shaderId < m_shadersNum) {
		glUseProgram(m_shaders[shaderId]);
		return true;
	}

	return false;
}

void RenderSystem::input_particles_pos(const vector<Vector3d> particlesPos) {

	const int particlesNum = particlesPos.size();
	m_particlesNum = particlesNum;

	*m_particlesPos = new GLfloat[particlesNum];

	for (int i = 0; i < particlesNum; i++) {
		m_particlesPos[i][0] = particlesPos[i].get_x();
		m_particlesPos[i][1] = particlesPos[i].get_y();
		m_particlesPos[i][2] = particlesPos[i].get_z();
	}

}

void RenderSystem::input_particles_pos(const vector<FluidParticle> particles) {

	const int particlesNum = particles.size();
	m_particlesNum = particlesNum;

	m_particlesPos = new GLfloat*[particlesNum];

	for (size_t i = 0; i < particlesNum; i++) {
		m_particlesPos[i] = new GLfloat[3];
		//particles[i].pos.print();
		m_particlesPos[i][0] = particles[i].pos.get_x();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
		m_particlesPos[i][1] = particles[i].pos.get_y();
		m_particlesPos[i][2] = particles[i].pos.get_z();
	}

}

GLuint RenderSystem::draw_particles() {

	using namespace std;

	// 生成并绑定粒子顶点数组
	GLuint particlesVertexArray;
	glGenVertexArrays(1, &particlesVertexArray);
	glBindVertexArray(particlesVertexArray);

	GLfloat vertices[6][3] = {
		{-0.3, -0.3, 3},
		{0.3, -0.3, 3},
		{-0.3, 0.3, 3},
		{0.9, -0.85,3},
		{0.9, 0.9, 3},
		{-0.85, 0.9, 3}
	};

	// 生成并绑定粒子位置数据缓存
	GLuint particlesBuffer;
	glGenBuffers(1, &particlesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlesBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_particlesPos), m_particlesPos, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// 将粒子位置数据与顶点着色器in变量相关联
	glVertexAttribPointer(PARTICLES_POS, 4, GL_FLOAT, FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(PARTICLES_POS);

	// 创建绘制粒子的着色器
	const char * shadersPath[] = { "DrawParticlesVertexShader.glsl", "", "DrawParticlesFragmentShader.glsl" };

	m_shaderProgram = create_shader(shadersPath);
	//cout << "shaderProgram : " << shaderProgram << endl;
	//cout << (0<= shaderProgram && shaderProgram< m_shadersNum) << endl;
	///*if (use_shader(shaderProgram))
	//	cout << "use_shader : " << use_shader(shaderProgram) << endl;*/
	//else
	//	cout << "use_shader : failure" << endl;
	/*if (!(shaderProgram > -1 && use_shader(shaderProgram))) {
		cout << "Error : Fail to create or use shader" << endl;
		return 0;
	}
	use_shader(shaderProgram);*/
	glUseProgram(m_shaderProgram);

	// 设置粒子颜色
	Vector3d particleColor(0.7, 0.7, 0.7);
	GLint particleColor_location = glGetUniformLocation(m_shaderProgram, "particleColor");
	if (particleColor_location != -1)
		glUniform4f(particleColor_location, 
			particleColor.get_x(), particleColor.get_y(), 
			particleColor.get_z(), particleColor.get_w());

	/*for (int i = 0; i < m_particlesNum; i++)
		cout << m_particlesPos[i][0] << endl;*/

	return particlesVertexArray;
	
}

void RenderSystem::draw_collision_triangles_wireframe(const CollisionManager & collisionMgr) {

	vector<CollisionTriangle> ctris = collisionMgr.get_collision_triangles();

	float * points[3];
	*points = new float[ctris.size()*3];

	for (int i = 0; i < ctris.size(); i+=3) {

		float point[3];

		point[0] = ctris[i].get_a().get_x();
		point[1] = ctris[i].get_a().get_y();
		point[2] = ctris[i].get_a().get_z();
		points[i] = point;
		
		point[0] = ctris[i+1].get_b().get_x();
		point[1] = ctris[i+1].get_b().get_y();
		point[2] = ctris[i+1].get_b().get_z();
		points[i+1] = point;

		point[0] = ctris[i+2].get_c().get_x();
		point[1] = ctris[i+2].get_c().get_y();
		point[2] = ctris[i+2].get_c().get_z();
		points[i+2] = point;

	}

	GLuint clpointsVertexArray;
	glGenVertexArrays(1, &clpointsVertexArray);
	glBindVertexArray(clpointsVertexArray);

	GLuint clpointsBuffer;
	glGenBuffers(1, &clpointsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, clpointsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

}

RenderSystem::~RenderSystem() {

}