#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include <string>
#include <type_traits>
#include <utility>
#include <fstream>

#include <imgui.h>
#include "../build/bindings/imgui_impl_glfw.h"
#include "../build/bindings/imgui_impl_opengl3.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>

#include <nlohmann/json.hpp>

#include "student.h"
#include "json_writer.h"
#include "json_reader.h"

#include <GL/glew.h> // Initialize with glewInit()

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#define PI 3.14159265358979323846
#define PATH "ip.txt"

std::string message;	//content sent from client to server to be printed on UI
std::mutex message_mutex;	//protect message


void toJSON(nlohmann::json* j, const Student* stud){
    *j = nlohmann::json{{"fname", stud->getFirstName()}, {"lname", stud->getLastName()}, {"grade", stud->getGrade()}, {"gpa", stud->getGpa()}};
}

const std::string jsonStudent(const Student* stud, nlohmann::json* json){

	toJSON(json,stud);

	return to_string(*json) + "\n";
}

const char* getIP(std::string* ip){
	//read file into send message
	std::string line;
	
	std::fstream readFile;
	readFile.open(PATH, std::fstream::in);

	while(std::getline(readFile,line)){
		*ip += line;
	}
	readFile.close();

	return ip->c_str();
}

void sendTCP(const Student* stud){
	JsonWriter jw;
	nlohmann::json json;
	std::string ip;
	
	jw.message = jsonStudent(stud,&json);

	jw.start(getIP(&ip), 55555);

	jw.close();
}

void sendTCP(const char* message){
	JsonWriter jw;
	std::string ip;

	jw.message = message;

	jw.start(getIP(&ip),55555);

	jw.close();
}

void saveStudent(const Student* stud){
	const std::lock_guard<std::mutex> lock(message_mutex);
	message = stud->getFirstName() + ", " + stud->getLastName() + ", " + std::to_string(stud->getGrade()) + ", " + std::to_string(stud->getGpa());
}

//server thread
void startServer(GLFWwindow **window, JsonReader *jr){
	nlohmann::json json;
	Student stud;

	std::cout << "opening server\n";
	
	std::cout << "sevrer open\n";
	jr->start(55555);
}

//this function populates the pointer stud with content from the GUI fields or nothing if no content is available
void getStudent(Student* stud){
	
	ImGui::Text("Enter Student Details Below");
	

	char buf[128] = {0};
	int i;
	for(i = 0; i < strlen(stud->getFirstName().c_str()); i++){
		buf[i] = stud->getFirstName()[i];
	}
	buf[i] = '\0';

	if(ImGui::InputText("Enter First Name", buf, 128)){
		std::string fname = buf;
		stud->setFirstName(fname);
	}

	char buf2[128] = {0};

	for(i = 0; i < strlen(stud->getLastName().c_str()); i++){
		buf2[i] = stud->getLastName()[i];
	}
	buf2[i] = '\0';

	if(ImGui::InputText("Enter Last Name", buf2, 128)){
		std::string lname = buf2;
		stud->setLastName(lname);
	}

	int grade = stud->getGrade();
	if(ImGui::SliderInt("Enter the Grade", &grade, 1,12)){
		stud->setGrade(grade);
	}

	float gpa = stud->getGpa();
	if(ImGui::SliderFloat("Enter the gpa", &gpa, 0.0,4.0,"%.2f")){
		stud->setGpa(gpa);
	}
	
}



//Dear IMGUI Initalization
bool config(GLFWwindow **window){
	// Setup window
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
	#if __APPLE__
		// GL 3.2 + GLSL 150
		const char *glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
	#else
		// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif

	// Create window with graphics context
	*window = glfwCreateWindow(1280, 720, "JSON Transfer 3.0", NULL, NULL);

	if (window == NULL)
		return false;

	glfwMakeContextCurrent(*window);
	glfwSwapInterval(1); // Enable vsync

	bool err = glewInit() != GLEW_OK;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(*window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return false;
	}

	int screen_width, screen_height;
	glfwGetFramebufferSize(*window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);
	return true;
}


int main(){
	GLFWwindow *window;

	if(!config(&window)){
		return 1;
	}

	Student senderStudent;	//student that will be sent
	Student recieverStudent;	//student that will be recieved
	boost::asio::io_context io;
	JsonReader server(&message,&message_mutex,io);
	std::thread read(startServer, &window, &server);	//run server in background

	while(!glfwWindowShouldClose(window)){	//main app loop
		
		glfwPollEvents();		
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);	//clears screen buffer (prevents double)

		// feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	
       
		ImGui::Begin("Student Details");

		getStudent(&senderStudent);	//populate student from GUI

		if (ImGui::Button("Send")){
			std::cout << "Sending..." << "\n";
			sendTCP(&senderStudent);	//Send Student
			std::cout << "Sent!" << "\n";
		}

		message_mutex.lock();
		ImGui::Text("%s", message.c_str());
		message_mutex.unlock();

		ImGui::End();

		// Render dear imgui into screen
		ImGui::Render();	//prints main windows
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	//draws to the window that we rendered

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glfwSwapBuffers(window);


	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	server.close();	//sets error flag in async calls
	read.join();

	return 0;
}