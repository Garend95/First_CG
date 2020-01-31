#ifndef SHADOW_H
#define SHADOW_H
#include "Shader.h"
class Shadow {
public:
	Shadow(int shadowWidth, int shadowHeight);
	void setupDepthMap();
	void renderToDepthMap(float nearPlane, float farPlane, glm::vec3 lightPos, Shader shader);
	void renderWithShadowMapping(float nearPlane, float farPlane, glm::vec3 lightPos, Shader shader); 
	void configureLightViewMatrix(string matrixName ,glm::mat4 matrix, Shader shader);

private:
	unsigned int depthMap;
	unsigned int depthMapFBO;
	int SWidth;
	int SHeight;
	float nearPlane; // <- deciding the front and back portions of the frustum
	float farPlane;
};

#endif 
