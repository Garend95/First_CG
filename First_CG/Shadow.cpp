#include "Shadow.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Global.h"

Shadow::Shadow(int shadowWidth, int shadowHeight) {
	SWidth = shadowWidth;
	SHeight = shadowHeight;
}

void Shadow::setupDepthMap() {
	
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SWidth, SHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shadow::renderToDepthMap(float nearPlane, float farPlane, glm::vec3 lightPos, Shader shader) {
	nearPlane = nearPlane;
	farPlane = farPlane;
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix, viewSpaceMatrix;
	
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	

	lightSpaceMatrix = lightProjection * lightView;
	
	

	glViewport(0, 0, SWidth, SHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	configureLightViewMatrix("lightSpaceMatrix", lightSpaceMatrix, shader);
	

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2 
	
	//glBindTexture(GL_TEXTURE_2D, w)
	
}

void Shadow::renderWithShadowMapping(float nearPlane, float farPlane, glm::vec3 lightPos, Shader shader) {
	nearPlane = nearPlane;
	farPlane = farPlane;

	glm::mat4 Projection, camView;
	glm::mat4 lightSpaceMatrix, viewSpaceMatrix;
	

	Projection = glm::perspective(glm::radians(60.0f),(float)4.0 / (float)3.0, 0.1f, 100.0f);
	camView = glm::lookAt(globCam.Front, globCam.Pos, globCam.Up);
	lightSpaceMatrix = Projection * camView;

	configureLightViewMatrix("lightSpaceMatrix", lightSpaceMatrix, shader);
	glm::vec3 viewPos = globCam.Pos;
	shader.setFloat("lightPos",1,lightPos);
	shader.setFloat("viewPos", 1, viewPos);
	//shader.setFloat("viewPos",1,view)
	//glViewport(0, 0, 1366, 768);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	

}
void Shadow::configureLightViewMatrix(string matrixName, glm::mat4 matrix, Shader shader) {
	shader.use();
	shader.setFloat(matrixName, 1, matrix);
}