#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>
#include <fstream>

#include <imgui.h>
#include "../build/bindings/imgui_impl_glfw.h"
#include "../build/bindings/imgui_impl_opengl3.h"
#include "opengl_shader.h"
#include "file_manager.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>

#include <nlohmann/json.hpp>

#include "student.h"
#include "json_writer.h"

#include <GL/glew.h> // Initialize with glewInit()

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

#define PI 3.14159265358979323846
#define IP "192.186.77.202"

using boost::asio::ip::tcp;

void toJSON(nlohmann::json* j, const Student* stud){
    *j = nlohmann::json{{"fname", stud->getFirstName()}, {"lname", stud->getLastName()}, {"grade", stud->getGrade()}, {"gpa", stud->getGpa()}};
}

const std::string jsonStudent(const Student* stud, nlohmann::json* json){

	toJSON(json,stud);

	return to_string(*json) + "\n";
}


void sendTCP(const Student* stud){
	JsonWriter jw;
	nlohmann::json json;
	
	jw.message = jsonStudent(stud,&json);

	jw.start(55555);
}


//this function populates the pointer stud with content from the GUI fields or nothing if no content is available
void getStudent(Student* stud){
	
	ImGui::Text("Enter Student Details Below");
	

	char buf[128] = {0};
	if(ImGui::InputText("Enter First Name", buf, 128)){
		std::string fname = buf;
		stud->setFirstName(fname);
	}

	char buf2[128] = {0};
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
	*window = glfwCreateWindow(1280, 720, "JSON Transfer 2.0", NULL, NULL);

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


// void recieve(Student* stud){
// 	boost::asio::io_context io_context;
//     boost::system::error_code error;
//     //create socket
//     boost::asio::ip::tcp::socket socket(io_context);

//     //connect to server
//     socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("192.168.0.46"),55555));

// 	// Construct a timer without setting an expiry time.
// 	boost::asio::deadline_timer timer(io_context);

// 	// Set an expiry time relative to now.
// 	timer.expires_from_now(boost::posix_time::seconds(5));

//     //read from server
//     boost::asio::streambuf recieve_buffer;
//     boost::asio::async_read(socket, recieve_buffer, boost::asio::transfer_all(), error);

//     std::string out = boost::asio::buffer_cast<const char *>(recieve_buffer.data());
//     io_context.run();
// }

int main(){
	GLFWwindow *window;

	if(!config(&window)){
		return 1;
	}

	Student senderStudent;	//student that will be sent
	Student recieverStudent;	//student that will be recieved

	while(!glfwWindowShouldClose(window)){	//main app loop
		glfwPollEvents();		
		// glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
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
	return 0;
}